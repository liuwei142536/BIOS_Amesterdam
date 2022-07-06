/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  Tcmsdrv.c

Abstract:

   Product:    TCM PEI Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/

#include <Efi.h>
#include <Pei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <AmiTcg/Tcm.h>
#include "TisPcPei.h"
#include <Ppi/TcgTcmService.h>
#include <Guid/AmiTcgGuidIncludes.h>
#include <Token.h>


//carlPEI-s
#include "SM3.h"
#include "Type.h"

//#define RESULT_WORD 5
#define T1 0x79CC4519
#define T2 0x7A879D8A
#define IVA 0x7380166f
#define IVB 0x4914b2b9
#define IVC 0x172442d7
#define IVD 0xda8a0600
#define IVE 0xa96f30bc
#define IVF 0x163138aa
#define IVG 0xe38dee4d
#define IVH 0xb0fb0e4e

/* Various logical functions */
#define p1(x)   (x^rotl32(x,15)^rotl32(x,23))
#define p0(x)   (x^rotl32(x,9)^rotl32(x,17))
#define ff0(a,b,c)   (a^b^c)
#define ff1(a,b,c)   ((a&b)|(a&c)|(b&c))
#define gg0(e,f,g)   (e^f^g)
#define gg1(e,f,g)   ((e&f)|((~e)&g))
#define rotl32(x,n)   (((x) << n) | ((x) >> (32 - n)))
#define rotr32(x,n)   (((x) >> n) | ((x) << (32 - n)))
//carlPEI-e

EFI_STATUS
EFIAPI
TcmLibPeiPassThrough (
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize
);

TCM_PC_REGISTERS_PTR          TCMHandle = (TCM_PC_REGISTERS_PTR)(UINTN)PORT_TPM_IOMEMBASE;
 
EFI_STATUS
EFIAPI
Tcm_StartUp (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle,
  IN      UINT16		            StartupType
  )
{
    EFI_STATUS                   Status;
    TCM_CMD_STARTUP		         cmdStartup;   
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdStartup.Header.tag       = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdStartup.Header.paramSize = TCM_H2NL (sizeof (cmdStartup));
    cmdStartup.Header.ordinal   = TCM_H2NL (TCM_ORD_Startup);
    cmdStartup.StartupType      = TCM_H2NS (StartupType) ;

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdStartup,
                sizeof(cmdStartup),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_ContinueSelfTest (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdSelfTest;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdSelfTest.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdSelfTest.paramSize    = TCM_H2NL (sizeof (cmdSelfTest));
    cmdSelfTest.ordinal      = TCM_H2NL (TCM_ORD_ContinueSelfTest);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdSelfTest,
                sizeof(cmdSelfTest),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_PhysicalEnable (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdPhysicEn;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdPhysicEn.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicEn.paramSize    = TCM_H2NL (sizeof (cmdPhysicEn));
    cmdPhysicEn.ordinal      = TCM_H2NL (TCM_ORD_PhysicalEnable);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicEn,
                sizeof(cmdPhysicEn),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}
 
EFI_STATUS
EFIAPI
Tcm_PhysicalDisable (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdPhysicDis;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdPhysicDis.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicDis.paramSize    = TCM_H2NL (sizeof (cmdPhysicDis));
    cmdPhysicDis.ordinal      = TCM_H2NL (TCM_ORD_PhysicalDisable);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicDis,
                sizeof(cmdPhysicDis),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_PhysicalSetDeactivated (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle,
  IN      BOOLEAN                   TureFalse
  )
{
    EFI_STATUS                                Status;   
    TCM_CMD_PHYSICAL_SET_DEACTIVATED          cmdPhysicSetDea;
    TCM_RSP_COMMAND_HDR                       RspHdr;

    cmdPhysicSetDea.Header.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicSetDea.Header.paramSize    = TCM_H2NL (sizeof (cmdPhysicSetDea));
    cmdPhysicSetDea.Header.ordinal      = TCM_H2NL (TCM_ORD_PhysicalSetDeactivated);
    cmdPhysicSetDea.Flag                = TureFalse;

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicSetDea,
                sizeof(cmdPhysicSetDea),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_ForceClear (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdForceClr;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdForceClr.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdForceClr.paramSize    = TCM_H2NL (sizeof (cmdForceClr));
    cmdForceClr.ordinal      = TCM_H2NL (TCM_ORD_ForceClear);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdForceClr,
                sizeof(cmdForceClr),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_GetCapability (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle,
  IN      UINT32 	                capArea,
  IN      UINT32 	                subCap,
  IN      UINT32                    respSize,  
  OUT     UINT8                     *resp
  )
{
    EFI_STATUS                   Status;  
    TCM_GET_CAPABILITY_IN        cmdGetCap;
    TCM_CMD_GET_CAPABILITY_OUT   RspHdr;
    

    cmdGetCap.Header.tag        = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdGetCap.Header.paramSize  = TCM_H2NL (sizeof (cmdGetCap));
    cmdGetCap.Header.ordinal    = TCM_H2NL (TCM_ORD_GetCapability);
    cmdGetCap.capArea           = TCM_H2NL (capArea); 
    cmdGetCap.subCapSize        = TCM_H2NL (TCM_CAP_FLAG);
    cmdGetCap.subCap            = TCM_H2NL (subCap);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdGetCap,
                sizeof(cmdGetCap),
                (UINT8*)&RspHdr,
                sizeof(RspHdr.Header) + sizeof(RspHdr.subSize) + respSize
                );

    //Copy the response data to speci buffer.
    while(respSize--)
    {
        resp[respSize] = RspHdr.resp[respSize];
    }


    return TcmCommCheckStatus(Status, &RspHdr.Header, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_StartUp_Clear (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_StartUp (TCMHandle, TCM_ST_CLEAR);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm StartUp Clear error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_ContinueSelfTest (TCMHandle);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm ContinueSelfTest error, returned %r\n", Status));
    }

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_StartUp_State (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_StartUp (TCMHandle, TCM_ST_STATE);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Startup state error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_ContinueSelfTest (TCMHandle);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm ContinueSelfTest error, returned %r\n", Status));
    }

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_Enable (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_PhysicalEnable (TCMHandle);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Physical enable error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_PhysicalSetDeactivated (TCMHandle, FALSE);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Set activate error, returned %r\n", Status));
    }    
 
    return Status;
}

EFI_STATUS
EFIAPI
Tcm_Disable (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_PhysicalSetDeactivated (TCMHandle, TRUE);  
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Set deactivate error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_PhysicalDisable (TCMHandle);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Physical disable error, returned %r\n", Status));
    }    

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_ClearOwner (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_ForceClear (TCMHandle);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Force clear error, returned %r\n", Status));
    }    

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_GetStatus (
  IN OUT UINT8         *EnabledSts,
  IN OUT UINT8         *ActivedSts,
  IN OUT UINT8         *OwnerSts 
  )
{
    EFI_STATUS              Status;
    UINT8                   PFlag[PFlagLen];
    UINT8                   ownerSet;
 
    Status = Tcm_GetCapability(TCMHandle, TCM_CAP_FLAG, TCM_CAP_FLAG_PERMANENT, sizeof(PFlag), PFlag);
    if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Get Pflag error, returned %r\n", Status));
        return Status;
    }

     if(PFlag[2] == 0)
	        *EnabledSts = 1;
	 else
	        *EnabledSts = 0;
    
     if(PFlag[4] == 0)
	        *ActivedSts = 1;
	 else
	        *ActivedSts = 0;

    Status = Tcm_GetCapability(TCMHandle, TCM_CAP_PROPERTY, TCM_CAP_PROP_OWNER, sizeof(ownerSet), &ownerSet);        	
	if(Status)
    {
        DEBUG ((EFI_D_ERROR, "Tcm Get Vflag error, returned %r\n", Status));
        return Status;
    }
	 
	 if( ownerSet == 0)
	        *OwnerSts = 0;
	 else
	        *OwnerSts = 1;

	 return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
TcmCommCheckStatus (
    IN      EFI_STATUS              Status,
    IN      TCM_RSP_COMMAND_HDR     *RspHdr,
    IN      TCM_STRUCTURE_TAG       RspHdrTag
 )
{  
      if(Status != EFI_SUCCESS)
        return Status;

      if( RspHdr->tag != TCM_H2NS(RspHdrTag) )
      {
        DEBUG ((EFI_D_ERROR, "Tcm no response.\n"));
        Status = EFI_NO_RESPONSE;
      }

      return Status;
}
//carlPEI-s

//sm3-s
/*typedef struct {
    unsigned long	state[8]={0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e};
	unsigned long	length=0;
	unsigned long	curlen=0;
    unsigned char	buf[64];
} SM3_STATE;
*/

SM3_STATE SM_TTest_Vector33;

U32 SM3_192[6] = {0};
U32 SM3_160[5] = {0};


/*
BiToWj is used to do Ext from Bi to Wj.
*/
void BiToWj(unsigned long *Bi, unsigned long *Wj)
{
    int j;
	unsigned long temp1;
	for(j=0;j<=15;j++)
	{
	    *Wj=*Bi;
        Wj++;
		Bi++;
	}
	for(j=16;j<=67;j++)
	{
        temp1=(*(Wj-16))^(*(Wj-9))^rotl32(*(Wj-3),15);
		*Wj=p1(temp1)^(rotl32(*(Wj-13),7))^(*(Wj-6));
		Wj++;
	}
}

/*
BiToWj is used to calculate Wj1 from Wj.
*/
void WjToWj1(unsigned long *Wj, unsigned long *Wj1)
{
    int j;
	for(j=0;j<=63;j++)
	{
	  *Wj1=*Wj^(*(Wj+4));
	  Wj++;
	  Wj1++;
	}
}

void Vload(unsigned long *A, unsigned long *B, unsigned long *C, unsigned long *D, 
unsigned long *E, unsigned long *F, unsigned long *G, unsigned long *H, unsigned long *V)
{
    *V=*A^*V;
	*(V+1)=*B^(*(V+1));
	*(V+2)=*C^(*(V+2));
	*(V+3)=*D^(*(V+3));
	*(V+4)=*E^(*(V+4));
	*(V+5)=*F^(*(V+5));
	*(V+6)=*G^(*(V+6));
	*(V+7)=*H^(*(V+7));
}
#if 1
void reg_init(unsigned long *A, unsigned long *B, unsigned long *C, unsigned long *D, 
unsigned long *E, unsigned long *F, unsigned long *G, unsigned long *H, unsigned long *V)
{
	*A=*V;
	*B=*(V+1);
	*C=*(V+2);
	*D=*(V+3);
	*E=*(V+4);
	*F=*(V+5);
	*G=*(V+6);
	*H=*(V+7);
}
#endif
/*
CF is used to do CF function.
input: Wj, Wj1, V
output: next V
*/
void CF(unsigned long *Wj, unsigned long *Wj1, unsigned long *V)
{
	unsigned long SS1;
	unsigned long SS2;
	unsigned long TT1;
	unsigned long TT2;
	unsigned long A,B,C,D,E,F,G,H;
	unsigned long T=T1;
	unsigned long FF;
	unsigned long GG;
	int j;
	//reg init
    reg_init(&A,&B,&C,&D,&E,&F,&G,&H,&V[0]);
	for(j=0;j<=63;j++)
	{
	    //SS1
		if(j==0)
		  T=T1;
		else if(j==16)
		  T=rotl32((unsigned long) T2,16);
		else
		  T=rotl32(T,1);
        SS1=rotl32((rotl32(A,12)+E+T),7);
		//SS2
		SS2=SS1^rotl32(A,12);
		//TT1
		if(j<=15)
		  FF=ff0(A,B,C);
		else
		  FF=ff1(A,B,C);
		TT1=FF+D+SS2+*Wj1;
		Wj1++;
		//TT2
		if(j<=15)
		  GG=gg0(E,F,G);
		else
		  GG=gg1(E,F,G);
		TT2=GG+H+SS1+*Wj;
		Wj++;
		//D
		D=C;
		//C
		C=rotl32(B,9);
		//B
		B=A;
		//A
		A=TT1;
		//H
		H=G;
		//G
		G=rotl32(F,19);
		//F
		F=E;
		//E
		E=p0(TT2);
	}
	Vload(&A,&B,&C,&D,&E,&F,&G,&H,&V[0]);
}

/* init the SM3 state */

void SM3_init(SM3_STATE *md)
{
    md->curlen = md->length = 0;
    md->state[0] = IVA;
    md->state[1] = IVB;
    md->state[2] = IVC;
    md->state[3] = IVD;
    md->state[4] = IVE;
    md->state[5] = IVF;
    md->state[6] = IVG;
    md->state[7] = IVH;

}

/* U32 endian converse.
 *	INPUT:
 * c[bytelen]: input buffer
 * bytelen: bytelen of c, must be multiple of 4
 *	OUTPUT:
 * a[bytelen]: output buffer
 *Note: inplace converse is permitted!
 */
void converse(unsigned char *c, unsigned int bytelen, unsigned char *a)
{
	unsigned char tmp = 0;
    unsigned long i = 0;
    
    for(i=0; i<bytelen/4; i++)
    {
		tmp = *(a+4*i);
        *(a+4*i) = *(c+4*i+3);
        *(c+4*i+3) = tmp;

		tmp = *(a+4*i+1);
        *(a+4*i+1) = *(c+4*i+2);
        *(a+4*i+2) = tmp;
    }
   

}
void SM3_compress(SM3_STATE * md)
{
	unsigned long Wj[68];
	unsigned long Wj1[64];
	//unsigned char tmp[64] = {0};
	
	converse(md->buf, 64, md->buf);
	//memcpy(md->buf, (unsigned char *)tmp, 64);

	BiToWj((unsigned long *)md->buf,Wj);
	WjToWj1(Wj,Wj1);
	CF(Wj, Wj1, md->state);	
}

void SM3_process(SM3_STATE *md, unsigned char *buf, U32 len)
{
    while (len--) 
	{
        /* copy byte */
        md->buf[md->curlen] = *buf++;
        md->curlen++; 
        /* is 64 bytes full? */
        if (md->curlen == 64)
		{
            SM3_compress(md);
            md->length += 512;
            md->curlen = 0;
        }
    }
}

void SM3_done(SM3_STATE *md, unsigned char *hash)
{
    int i;

    /* increase the length of the message */
    md->length += md->curlen <<3;

    /* append the '1' bit */
    *((U8 *)md->buf+md->curlen) = 0x80;
    md->curlen++;
    /* if the length is currenlly above 56 bytes we append zeros
     * then compress.  Then we can fall back to padding zeros and length
     * encoding like normal.
     */
    if (md->curlen >56) 
	{
        while(md->curlen < 64)
        {
		    *((U8 *)md->buf+md->curlen) = 0;
			md->curlen++;
        }
		SM3_compress(md);
        md->curlen = 0;
    }

    /* pad upto 56 bytes of zeroes */
    while(md->curlen < 56)
    {
		*((U8 *)md->buf+md->curlen) = 0;
		md->curlen++;
	}

    /* since all messages are under 2^32 bits we mark the top bits zero -- done by the front step*/
    for (i = 56; i < 60; i++)
        md->buf[i] = 0;
	
	md->buf[63] = (UINT8)(md->length) & 0xff;//CARLPEI UINT8
	md->buf[62] = (UINT8)(md->length >> 8) & 0xff;//CARLPEI UINT8
	md->buf[61] = (UINT8)(md->length >> 16) & 0xff;//CARLPEI UINT8
	md->buf[60] = (UINT8)(md->length >> 24) & 0xff;//CARLPEI UINT8

    SM3_compress(md);
	
	converse((U8*)md->state, 32, (U8*)md->state);

//carlPEI	memcpy(hash,md->state,32);   //???,??????  
     
}

/* SM3-256 a block of memory */
void SM3Hash(unsigned char *buf, U32 len, unsigned char *hash)     // len = size of buf;
{
    SM3_STATE md;
    
    SM3_init(&md);
    SM3_process(&md, buf, len);
    SM3_done(&md, hash);

//	converse(hash, out_len, hash);
}
//sm3-e

EFI_STATUS
EFIAPI
MAHashAllGlobal (
  IN    TCM_PC_REGISTERS_PTR    TcmHandle,//carlPEIpcr
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT16                  PCRIndex
  )
{
    EFI_STATUS   Status;
//carlPEIpcr-s
	TCM_CMD_EXTENDPCR			 cmdExtendPCR;
    TCM_RSP_COMMAND_HDR          RspHdr;
//	int i;                       //ami override
//carlPEIpcr-e
    //mpInitTCM (TisReg);

//    Status = Tcm_StartUp (TCMHandle, TCM_ST_CLEAR);

	SM3Hash((unsigned char *)TcmInputParameterBlock, TcmInputParameterBlockSize, (unsigned char *)TcmOutputParameterBlock);
//pcr-s


    cmdExtendPCR.Header.tag       = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdExtendPCR.Header.paramSize = TCM_H2NL (sizeof (cmdExtendPCR));
    cmdExtendPCR.Header.ordinal   = TCM_H2NL (TCM_ORD_Extend);
	cmdExtendPCR.PCRIndex		  = PCRIndex;
	
	/*for(i=0;i<32;i++)
	{	
		cmdExtendPCR.TCM_INPUT_BUFFER.digest[i] = TcmOutputParameterBlock[i];
	}*///AMIOverride
	CopyMem(cmdExtendPCR.TCM_INPUT_BUFFER.digest, TcmOutputParameterBlock, 32);

    Status = TcmLibPeiPassThrough (
                TcmHandle,
                (UINT8*)&cmdExtendPCR,
                sizeof(cmdExtendPCR),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
//pcr-e
//    return Status;
}
//carlPEI-e

//
// TCM PEI Driver PPI 
//
STATIC PEI_TCM_PPI g_TcmFunc = {
    Tcm_StartUp_Clear,
    Tcm_StartUp_State,
    Tcm_Enable,
    Tcm_Disable,
    Tcm_ClearOwner,
    Tcm_GetStatus,
    TcmLibPeiPassThrough,
    MAHashAllGlobal //carlPEI
};

STATIC EFI_PEI_PPI_DESCRIPTOR       mPpiList[] = {
  EFI_PEI_PPI_DESCRIPTOR_PPI |
  EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTcmPpiGuid,
  &g_TcmFunc
};

//----------------------------------------------------------------------------
// Procedure:	TcmPEI_EntryPoint
//
// Description:
//  This function is called after the permanent memory is installed in the
//  system. This function initializes CPU in the memory present environment.
//
// Input:
//  FfsHeader       Pointer to the FFS file header.
//  PeiServices     Pointer to the PEI services table.
//
// Output:		EFI_SUCCESS
//
//----------------------------------------------------------------------------
EFI_STATUS
EFIAPI TcmPEI_EntryPoint(
    IN EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES    **PeiServices)
{
    EFI_STATUS 			 Status = EFI_SUCCESS;
    UINT16               VID;

    //Check if TCM
    VID = TCMHandle->vid;
    if( VID != TCM_VID )
        return EFI_UNLOAD_IMAGE;        

    Status = (*PeiServices)->InstallPpi (PeiServices, mPpiList);

    return Status;
}
