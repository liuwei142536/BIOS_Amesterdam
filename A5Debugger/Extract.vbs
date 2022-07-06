'
'This script is used for extracting the content of AptioVDebugger UI plugin jar file, read the manifest file 
'and return the errorlevel status based on version number comparison.
'

' This function is responsible for extracting the content of jar file, read the manifest file and return the version number.
Function ReadManiFest(filename)
		'If file is read-only make it writeable
	Set objExtractableFile = fso.GetFile(filename)
	If objExtractableFile.Attributes AND 1 Then
		objExtractableFile.Attributes = objExtractableFile.Attributes XOR 1
	End If
	 
	If fso.FolderExists(ExtractTo) Then
		'If folder VeBExtractTemp already exist at temporary location then delete it
		fso.DeleteFolder (ExtractTo)		
	End If
	'create VeBExtractTemp folder at temporary location
	fso.CreateFolder(ExtractTo)

	'Copy the jar file to temporary location and rename it as .zip for extraction
	 Set fso = CreateObject("Scripting.FileSystemObject")
	 DstFile = ExtractTo + "\debuggerUI.zip"
	 fso.CopyFile filename, DstFile

	 'Extract the contants of the zip file.
	 set objShell = CreateObject("Shell.Application")
	 set FilesInZip=objShell.NameSpace(DstFile).items
	 objShell.NameSpace(ExtractTo).CopyHere(FilesInZip)

	 'Read the Manifest file
	 ManifestFile = ExtractTo + "\META-INF\MANIFEST.MF"	
	 'check that Is manifest file exist, then read version number.
	 If (fso.FileExists(ManifestFile)) Then
		Set ObjFile = fso.openTextFile(ManifestFile)
		Do Until ObjFile.AtEndOfStream
		sLine =Trim( ObjFile.ReadLine )
		if "" <> sLine Then
			'take the Bundle-Version
			If "Bundle-Version" = Left(Trim(sLine), 14) Then
				version =Split(sline,":")			
			Exit Do
			end if
		end if
		loop	 
		ObjFile.close
		Set objShell = Nothing		
	 
		'Set the function's return value
		ReadManiFest = version(1)
	 end if 
End Function


'Main function
SourceFile = WScript.Arguments.Item(0) 
DestFile = WScript.Arguments.Item(1)

'Create a temporary folder for extracting the jar file
Set fso = CreateObject("Scripting.FileSystemObject")
'check whether src file or destination file exist or not if src file is not there give error message if destination file is not there return value should be 2
If NOT (fso.FileExists(SourceFile)) Then
	msgbox "Source Debugger jars not Present"
	WScript.Quit (-1)
	End If 
	
If NOT (fso.FileExists(DestFile)) Then
	WScript.Quit (3)
	End If


Const TemporaryFolder = 2
Set ExtractTo = fso.GetSpecialFolder(TemporaryFolder)
ExtractTo = ExtractTo + "\VeBExtractTemp"

'Call function ReadManiFest() to read manifest file and get the version number.
version1 = ReadManiFest(SourceFile)
version2 = ReadManiFest (DestFile)

'Clear before exit
fso.DeleteFolder (ExtractTo)
Set fso = Nothing

'Check the version
if version1 = version2 Then
	WScript.Quit (0)
ElseIf version1 < version2 Then
    WScript.Quit (1)
ElseIf version1 > version2 Then
    WScript.Quit (2)
Else
	msgbox "Error Occured While Comparing"
	WScript.Quit (-1)
End If