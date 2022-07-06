//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/**
 * @file FileList.c
 * @brief Implements FILENAME file browser menu
 */

#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/AMIPostMgr.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SortLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>

#include "FileBrowser.h"

#define INIT_DIR_SIZE 100
#define INCREMENT_DIR_SIZE 50

static CHAR16 *FileMenuTitle = NULL;
static CHAR16 *FileMenuLegend = NULL;

#define MAX_NESTED_DIRS 20
static EFI_FILE_PROTOCOL *fp_stack[MAX_NESTED_DIRS] = { 0 };
static UINT32 fp_stack_p = 0;
static AMI_VECTOR *fv_stack[MAX_NESTED_DIRS] = { 0 };
static UINT32 fv_stack_p = 0;

extern EFI_HII_HANDLE HiiHandle;
extern AMI_POST_MANAGER_PROTOCOL *AmiPostMgr;
extern CHAR16 *FileTypes;

VOID ClearFileList (AMI_VECTOR * fv);

VOID fp_stack_push (EFI_FILE_PROTOCOL *fp)
{
	fp_stack[fp_stack_p++] = fp;
}

EFI_FILE_PROTOCOL * fp_stack_pop (VOID)
{
	if (fp_stack[fp_stack_p] != NULL) {
		EFI_FILE_PROTOCOL *t = fp_stack[fp_stack_p];
		t->Close (t);
		fp_stack[fp_stack_p] = NULL;
	}
	if (fp_stack_p)
	    return fp_stack[--fp_stack_p];
	else
	    return NULL;
}

EFI_FILE_PROTOCOL * fp_stack_peek (VOID)
{
    if(fp_stack_p)
        return fp_stack[fp_stack_p - 1];
    else
        return NULL;
}

VOID fv_stack_push (AMI_VECTOR *fv)
{
	fv_stack[fv_stack_p++] = fv;
}

AMI_VECTOR * fv_stack_pop (VOID)
{
	if (fv_stack[fv_stack_p] != NULL) {
		ClearFileList (fv_stack[fv_stack_p]);
		fv_stack[fv_stack_p] = NULL;
	}
	if (fv_stack_p)
	    return fv_stack[--fv_stack_p];
	else 
	    return NULL;
}

/**
 * @brief Check file extension against predefined list
 *
 * @param FileName Pointer to file name
 *
 * @return TRUE if check passed, FALSE otherwise
 */
BOOLEAN CheckFileType (CHAR16 *FileName)
{
	UINTN idx;
	CHAR16 *ext, *tmp;
	idx = StrLen (FileName) - 1;
	while (FileName[idx] != L'.' && idx)
		idx--;
	if (idx == 0)
		return TRUE;	//File doesn't have extension, so it's valid
	FileName = &FileName[idx + 1];
	ext = AllocatePool (StrSize (FileName));
	tmp = ext;
	while (*FileName != 0)
		*tmp++ = ToUpper(*FileName++);
	*tmp = 0;
	tmp = StrStr (FileTypes, ext);
	FreePool (ext);
	return (tmp != NULL);
}

/**
 * @brief Check if filename is valid file
 *
 * @param FileInfo Pointer to file descriptor
 *
 * @return TRUE if file is valid, FALSE otherwise
 */
BOOLEAN IsValidFile (EFI_FILE_INFO *FileInfo)
{
	if (FileInfo->FileName[0] == '.' &&	//current directory
		FileInfo->FileName[1] == 0)
		return FALSE;
	if ((FileInfo->Attribute & EFI_FILE_HIDDEN) ||
		(FileInfo->Attribute & EFI_FILE_SYSTEM))
		return FALSE;
	if (FileTypes != NULL)
		return CheckFileType (FileInfo->FileName);
	return TRUE;
}

/**
 * @brief Check if descriptor is parent directory
 *
 * @param FileInfo Pointer to file descriptor
 *
 * @return TRUE if parent directory, FALSE otherwise
 */
BOOLEAN IsParentDir (EFI_FILE_INFO *FileInfo)
{
	if (FileInfo->FileName[0] == '.' &&
	    FileInfo->FileName[1] == '.' &&
		FileInfo->FileName[2] == 0)
		return TRUE;
	return FALSE;
}

/**
 * @brief Check if item is a file
 *
 * @param item Pointer to item
 *
 * @return True if file, FALSE if directory
 */
BOOLEAN IsFile (FILE_MENU_ITEM *item)
{
	EFI_FILE_INFO *a;
	
	a = (EFI_FILE_INFO *)item->Info;
	return (a->Attribute & EFI_FILE_DIRECTORY) ? FALSE : TRUE;
}

/**
 * @brief Cut string to given length adding ellipses
 *
 * @param str Pointer to string
 * @param len Length to fit
 *
 * @return Pointer to new string
 */
CHAR16 * CutString (CHAR16 *str, UINTN len)
{
	CHAR16 *r;
	
	r = AllocatePool ((len + 1) * sizeof (CHAR16));
	CopyMem (r, str, (len - 3) * sizeof (CHAR16));
	r[len - 3] = L'.';
	r[len - 2] = L'.';
	r[len - 1] = L'.';
	r[len] = 0;
	return r;
}

/**
 * @brief Prepares menu item name for display
 *
 * @param mi Pointer to menu item
 */
VOID SetMenuName (FILE_MENU_ITEM *mi)
{
	EFI_FILE_INFO *a;
	UINTN l;
	UINTN maxlen;
	UINTN maxsz = sizeof (CHAR16) * MAX_MENU_ITEM_NAME;
	
	a = (EFI_FILE_INFO *)mi->Info;
	maxlen = (a->Attribute & EFI_FILE_DIRECTORY) ? 
            MAX_MENU_ITEM_NAME - 3 : MAX_MENU_ITEM_NAME - 1; //0-term + <> for dirs
	l = StrLen (a->FileName);
	if (l < maxlen) {
		if (a->Attribute & EFI_FILE_DIRECTORY)
			UnicodeSPrintAsciiFormat (mi->Name, maxsz, "<%s>", a->FileName);
		else
			UnicodeSPrintAsciiFormat (mi->Name, maxsz, "%s", a->FileName);
	} else {
		CHAR16 *tmp = CutString (a->FileName, maxlen);
		if (a->Attribute & EFI_FILE_DIRECTORY)
			UnicodeSPrintAsciiFormat (mi->Name, maxsz, "<%s>", a->FileName);
		else
			UnicodeSPrintAsciiFormat (mi->Name, maxsz, "%s", a->FileName);
		FreePool (tmp);
	}
}

/**
 * @brief Extend file path with given directory/file name
 *
 * @param node Pointer to name to add
 */
VOID AppendFilePath (CHAR16 *node)
{
	UINTN l1;
	UINTN l2;
	CHAR16 *str;
	UINTN maxlen;
	
	if (node == NULL) {	//root node
		FileMenuTitle = AllocatePool (4); //four bytes for "/", including 0-terminator
		FileMenuTitle[0] = L'\\';
		FileMenuTitle[1] = 0;
		return;
	}
	
	l1 = StrLen (FileMenuTitle);
	l2 = StrLen (node);
	if (l1 == 1 && FileMenuTitle[0] == L'\\') {
		maxlen = l1 + l2;
		str = AllocatePool ((maxlen + 1) * sizeof (CHAR16)); //l1 + l2 + 0 for root node
		UnicodeSPrintAsciiFormat (str, (maxlen + 1) * sizeof (CHAR16), "%s%s", FileMenuTitle, node);
	} else {
		maxlen = l1 + l2 + 1;
		str = AllocatePool ((maxlen + 1) * sizeof (CHAR16)); //l1 + / + l2 + 0 for non-root node
		UnicodeSPrintAsciiFormat (str, (maxlen + 1) * sizeof (CHAR16), "%s\\%s", FileMenuTitle, node);
	}
	FreePool (FileMenuTitle);
	FileMenuTitle = str;
}

/**
 * @brief Remove last node from file path
 */
VOID CutFilePath (VOID)
{
	UINTN l;
	l = StrLen (FileMenuTitle);
	while (FileMenuTitle[l] != L'\\')
		l--;
	if (l > 0)
		FileMenuTitle[l] = 0;
	else
		FileMenuTitle[l + 1] = 0;
}

/**
 * @brief Pad given string with spaces
 *
 * @param s Pointer to string
 * @param len Number of spaces to add
 *
 * @return Pointer to new string
 */
CHAR16 * PadString (CHAR16 *s, UINTN len)
{
	UINTN s_len;
	CHAR16 *r;
	
	s_len = StrLen (s);
	r = AllocatePool (sizeof (CHAR16) * (s_len + len + 1));
	CopyMem (r, s, sizeof (CHAR16) * s_len);
	while (len--)
		r[s_len++] = L' ';
	r[s_len] = 0;
	return r;
}

/**
 * @brief Convert character to upper case
 *
 * @param c Character to convert
 *
 * @return Converted character
 */
CHAR16 ToUpper (CHAR16 c)
{
	return ((c >= L'a' && c <= L'z') ||
			(c >= 0xe0 && c <= 0xf6) ||
			(c >= 0xf8 && c <= 0xfe)) ? c - 0x20 : c;
}

/**
 * @brief Compare two strings
 *
 * @param a Pointer to first string
 * @param b Pointer to second string
 *
 * @return -1 if a<b, 1 if a>b, 0 if a=b
 */
INTN CompareStrings (CHAR16 *a, CHAR16 *b)
{
	UINTN a_len, b_len;
	INTN r= 0;
	CHAR16 *p = NULL;
	
	a_len = StrLen (a);
	b_len = StrLen (b);
	
	if (a_len < b_len) {
		a = PadString (a, b_len - a_len);
		p = a;
	} else if (b_len < a_len) {
		b = PadString (b, a_len - b_len);
		p = b;
	}
	
	while (*a) {
		r = ToUpper (*a) - ToUpper (*b);
		if (r)
			break;
		a++;
		b++;
	}
	//clean up
	if (p != NULL)
		FreePool (p);
	
	return r;
}

/**
 * @brief Compare two file names by UEFI compare rules
 *
 * @param a Pointer to first file name
 * @param b Pointer to second file name
 *
 * @return -1 if a<b, 1 if a>b, 0 if a=b
 */
INTN CompareFileNames (CHAR16 *a, CHAR16 *b)
{
	UINTN a_len, b_len;
	UINTN a_dot, b_dot;
	INTN r;
	
	a_len = StrLen (a);
	b_len = StrLen (b);
	
	//find right-most dot
	for (a_dot = a_len; a_dot > 0; a_dot--) {
		if (a[a_dot] == L'.')
			break;
	}
	
	for (b_dot = b_len; b_dot > 0; b_dot--) {
		if (b[b_dot] == L'.')
			break;
	}

	if (a_dot == 0 && b_dot == 0) {		//both filenames don't have dots
		return CompareStrings (a, b);
	} else if (a_dot == 0 && b_dot > 0) {
		b[b_dot] = 0;
		r = CompareStrings (a, b);
		b[b_dot] = L'.';
		return (r <= 0) ? -1 : 1;
	} else if (a_dot > 0 && b_dot == 0) {
		a[a_dot] = 0;
		r = CompareStrings (a, b);
		a[a_dot] = L'.';
		return (r < 0) ? -1 : 1;
	}
	
	//both files have dots in their names
	a[a_dot] = 0;
	b[b_dot] = 0;
	r = CompareStrings (a, b);
	a[a_dot] = L'.';
	b[b_dot] = L'.';
	if (r != 0)
		return r;
	
	//names before dots are same
	return CompareStrings (&a[++a_dot], &b[++b_dot]);
}

/**
 * @brief Compare to pointers with certain rules
 *
 * @param aa First pointer
 * @param bb Second pointer
 *
 * @return -1 if a<b, 1 if a>b, 0 if a=b
 */
INTN EFIAPI Compare (
		CONST VOID *aa,
		CONST VOID *bb
)
{
	FILE_MENU_ITEM *ma = (FILE_MENU_ITEM *)aa;
	FILE_MENU_ITEM *mb = (FILE_MENU_ITEM *)bb;
	EFI_FILE_INFO *a;
	EFI_FILE_INFO *b;
	BOOLEAN a_dir, b_dir;
	
	a = (EFI_FILE_INFO *)ma->Info;
	b = (EFI_FILE_INFO *)mb->Info;
	
	if (IsParentDir (a))
		return -1;
	if (IsParentDir (b))
		return 1;
	
	a_dir = (a->Attribute & EFI_FILE_DIRECTORY) ? 1 : 0;
	b_dir = (b->Attribute & EFI_FILE_DIRECTORY) ? 1 : 0;
	if (a_dir && !b_dir)
		return -1;
	if (b_dir && !a_dir)
		return 1;
	return CompareFileNames (a->FileName, b->FileName);
}

/**
 * @brief Return sorted FILE MENU ITEMS list
 *
 * @param dir Pointer to directory descriptor
 *
 * @return Pointer to array of sorted items
 */
AMI_VECTOR * GetFileMenuList (EFI_FILE_PROTOCOL *dir)
{
    AMI_VECTOR *fv;
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize;
    EFI_STATUS Status;
    
    fv = VectorInit (INIT_DIR_SIZE, sizeof (FILE_MENU_ITEM), INCREMENT_DIR_SIZE);
    do {
        FileInfoSize = 0;
        FileInfo = NULL;
        Status = dir->Read (dir, &FileInfoSize, FileInfo);
        if (Status == EFI_BUFFER_TOO_SMALL) {   //increase buffer and re-read
            FileInfo = AllocatePool (FileInfoSize);
            Status = dir->Read (dir, &FileInfoSize, FileInfo);
        }
        
        if (!EFI_ERROR (Status) && FileInfoSize != 0 && IsValidFile (FileInfo)) {
        FILE_MENU_ITEM *mi;
            mi = AllocatePool (sizeof (*mi));
            mi->Info = FileInfo;
            SetMenuName (mi);
            VectorAdd (fv, mi);
            FreePool (mi);
        }
        else if(FileInfo)
            FreePool(FileInfo);

            
    } while (FileInfoSize != 0 && !EFI_ERROR (Status));
    PerformQuickSort (fv->buf, VectorGetSize (fv) , fv->e_size, Compare);
    
    return fv;
}

/**
 * @brief Return sorted EFI_FILE_INFO list
 *
 * @param Dir Pointer to directory
 *
 * @return Pointer to array of sorted items
 */
AMI_VECTOR * GetFileList (VOID *Dir)
{
    AMI_VECTOR *fv;
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize;
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL *dir = (EFI_FILE_PROTOCOL *)Dir;
    VOID *end = NULL;
    
    fv = VectorInit (INIT_DIR_SIZE, sizeof (VOID *), INCREMENT_DIR_SIZE);
    do {
        FileInfoSize = 0;
        FileInfo = NULL;
        Status = dir->Read (dir, &FileInfoSize, FileInfo);
        if (Status == EFI_BUFFER_TOO_SMALL) {   //increase buffer and re-read
            FileInfo = AllocatePool (FileInfoSize);
            Status = dir->Read (dir, &FileInfoSize, FileInfo);
        }
        
        if (!EFI_ERROR (Status) && FileInfoSize != 0 && IsValidFile (FileInfo))
            VectorAdd (fv, &FileInfo);

    } while (FileInfoSize != 0 && !EFI_ERROR (Status));
    PerformQuickSort (fv->buf,  VectorGetSize (fv), fv->e_size, Compare);
    VectorAdd (fv, &end);
    
    return fv;
}

/**
 * @brief Return user selection
 *
 * @param fv Pointer to sorted array of menu items
 *
 * @return Pointer to selected menu item
 */
VOID * GetMenuSelection (AMI_VECTOR *fv)
{
	EFI_STATUS st;
	UINTN num_files;
	UINTN i;
	POSTMENU_TEMPLATE *items, *p;
	UINT16 r_idx = 0;
	
	num_files = VectorGetSize (fv);
	items = p = AllocateZeroPool (sizeof (*items) * num_files);
	
	for (i = 0; i < num_files; i++, p++) {
            FILE_MENU_ITEM *mi = (FILE_MENU_ITEM *) VectorGetElement (fv, i);
            p->ItemToken = NewString (mi->Name);
	}
	
	SetString (FileMenuTitle, STRING_TOKEN(STR_DIALOG_TITLE));
	SetString (FileMenuLegend, STRING_TOKEN(STR_DIALOG_LEGEND));
	st = AmiPostMgr->DisplayPostMenu (HiiHandle, 
			STRING_TOKEN(STR_DIALOG_TITLE),
			STRING_TOKEN(STR_DIALOG_LEGEND),
			items, (UINT16) num_files, &r_idx);
	FreePool (items);
	ResetStrings();

	return (st) ? NULL : VectorGetElement (fv, r_idx);
}

/**
 * @brief Free menu list
 *
 * @param fv Pointer to menu item list
 */
VOID ClearFileList (AMI_VECTOR * fv)
{
	UINTN num_files;
	UINTN i;
	
	num_files = VectorGetSize (fv);
	for (i = 0; i < num_files; i++) {
            FILE_MENU_ITEM *mi = (FILE_MENU_ITEM *) VectorGetElement (fv, i);
            if (mi)
                FreePool (mi->Info);
	}
	VectorDestroy (fv);
}

/**
 * @brief Free menu resources
 */
VOID ClearAll (VOID)
{
	UINT32 i;
	for (i = 0; i < fp_stack_p; i++) {
		EFI_FILE_PROTOCOL *t = fp_stack[i];
		t->Close (t);
		fp_stack[i] = NULL;
	}
	for (i = 0; i < fv_stack_p; i++)
	{	    
		ClearFileList (fv_stack[i]);
		fv_stack[i] = NULL;
	}
	fp_stack_p = fv_stack_p = 0;
}

/**
 * @brief Get user selection from sub-menu
 *
 * @param fm Pointer to upper-level menu item
 *
 * @return Pointer to user selected item
 */
VOID * GetChildFileSelection (FILE_MENU_ITEM *fm)
{
	EFI_FILE_INFO *a = (EFI_FILE_INFO *)fm->Info;
	EFI_FILE_PROTOCOL *dir;
	EFI_STATUS st;
	AMI_VECTOR * fv;
	FILE_MENU_ITEM *r;
	
	if (IsParentDir (a)) {
            CutFilePath ();
            fv = fv_stack_pop ();
            dir = fp_stack_pop ();
	} else {
            AppendFilePath (a->FileName);
            dir = fp_stack_peek ();
            if(!dir)
                return NULL;
            st = dir->Open (dir, &dir, a->FileName, EFI_FILE_MODE_READ, 0);
            if (st)
                return NULL;
            fv = GetFileMenuList(dir);
	}
	
	if (fv == NULL)
            return NULL;
	
	r = (FILE_MENU_ITEM *)GetMenuSelection (fv);
	if (r == NULL || IsFile (r))
            return r;	//either ESC was pressed or user picked particular file
	
	a = (EFI_FILE_INFO *)r->Info;
	if (!IsParentDir (a)) {
            fp_stack_push (dir);
            fv_stack_push (fv);
	}
	
	r = (FILE_MENU_ITEM *)GetChildFileSelection (r);
	return r;
}

/**
 * @brief Return user selected filename
 *
 * @param dev Pointer to filesystem device
 *
 * @return Pointer to filename string
 */
CHAR16 * GetFileSelection (DEVICE_MENU_ITEM *dev)
{
	EFI_STATUS st;
	AMI_VECTOR * fv;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
	EFI_FILE_PROTOCOL *root;
	FILE_MENU_ITEM *r;
	
	fs = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *)dev->Fs;
	st = fs->OpenVolume (fs, &root);
	if (st) //can't open volume or directory
            return NULL;
	
	fv = GetFileMenuList(root);
	if (fv == NULL)
            return NULL;

	FileMenuLegend = dev->Name;
	AppendFilePath (NULL);

	r = (FILE_MENU_ITEM *)GetMenuSelection (fv);
	if (r != NULL && !IsFile (r)) {
            fp_stack_push (root);
            fv_stack_push (fv);
            r = (FILE_MENU_ITEM *)GetChildFileSelection (r);
	}
	
	if (r != NULL) {
            EFI_FILE_INFO *a = (EFI_FILE_INFO *)r->Info;
            AppendFilePath (a->FileName);
	}

	if(!fp_stack_p && (r != NULL))
	    ClearFileList(fv);

	ClearAll ();
	return (r != NULL) ? FileMenuTitle : NULL;
}



//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
