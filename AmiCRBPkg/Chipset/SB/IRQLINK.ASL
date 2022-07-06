// Generic PCI IRQ Link devices
// File is included to ASL root
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
// $Header: /Alaska/BIN/Chipset/Intel/SouthBridge/PatsburgPCH/Chipset/IRQLINK.asl 1     12/09/11 1:19p Archanaj $
//
// $Revision: 1 $
//
// $Date: 12/09/11 1:19p $
//**********************************************************************;
// Revision History
// ----------------
// $Log: /Alaska/BIN/Chipset/Intel/SouthBridge/PatsburgPCH/Chipset/IRQLINK.asl $
// 
// 1     12/09/11 1:19p Archanaj
// 
// 2     7/07/10 2:52p Bhimanadhunik
// Intial check-in for Patsburg.
// 
// 1     2/24/10 11:04p Tonywu
// Intel Cougar Point/SB eChipset initially releases.
//
//**********************************************************************;
//      Generic PCI IRQ Link devices
//-----------------------------------------------------------------------
// Note. All Link devices are placed under \_SB scope
//-----------------------------------------------------------------------
//-	OperationRegion(\_SB.PCI0.SBRG.PIX0, PCI_Config, 0x60, 12)
//-	Field(\_SB.PCI0.SBRG.PIX0,ByteAcc,NoLock,Preserve){
	OperationRegion(\_SB.PCI0.LPC0.PIX0, PCI_Config, 0x60, 12)
	Field(\_SB.PCI0.LPC0.PIX0,ByteAcc,NoLock,Preserve){
		PIRA,8, 		// IRQA route register
		PIRB,8, 		// IRQB route register
		PIRC,8, 		// IRQC route register
		PIRD,8, 		// IRQD route register
		, 32,
		PIRE,8, 		// IRQE route register
		PIRF,8, 		// IRQF route register
		PIRG,8, 		// IRQG route register
		PIRH,8, 		// IRQH route register
		}

Scope(\_SB) {

// Not required as SDL wizard creates PWRB device when Power button notification is enabled for any PCI device by setting "PWRBnotify = Yes" in SDL object
//	Device(PWRB){
//		Name(_HID,EISAID("PNP0C0C"))
//		Name(_UID, 0xAA)	// UID is hardwired to 0xAA
//		Name(_STA, 0x0B)	// device present/hidden in OS Device Manager
//		Method(_PRW, 0){ Return(GPRW(0x1D, 3)) }
//	}

	Name(BUFA,
		ResourceTemplate()
		{
			IRQ(Level, ActiveLow, Shared ) {15}
	})
	CreateWordField(BUFA, 0x1, IRA0)	// IRQ Mask
//--------------------------------------------------------------------------//
	Device(LNKA){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 1)

                Method(_STA,0) {
			And(PIRA, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKA desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKA enabled
		}

		Method(_PRS,0){Return(PRSA)}

		Method(_DIS){Or(PIRA, 0x80 ,PIRA)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRA, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRA)
		}					// End Method _SRS
	} // end Device LNKA
//--------------------------------------------------------------------------//
	Device(LNKB){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 2)

                Method(_STA,0) {
			And(PIRB, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKB desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKB enabled
		}

		Method(_PRS,0){Return(PRSB)}

		Method(_DIS){Or(PIRB, 0x80 ,PIRB)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRB, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRB)
		}					// End Method _SRS
	} // end Device LNKB
//--------------------------------------------------------------------------//
	Device(LNKC){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 3)

                Method(_STA,0) {
			And(PIRC, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKC desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKC enabled
		}

		Method(_PRS,0){Return(PRSC)}

		Method(_DIS){Or(PIRC, 0x80 ,PIRC)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRC, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRC)
		}					// End Method _SRS
	} // end Device LNKC
//--------------------------------------------------------------------------//
	Device(LNKD){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 4)

                Method(_STA,0) {
			And(PIRD, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKD desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKD enabled
		}

		Method(_PRS,0){Return(PRSD)}

		Method(_DIS){Or(PIRD, 0x80 ,PIRD)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRD, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRD)
		}					// End Method _SRS
	} // end Device LNKD
//--------------------------------------------------------------------------//
	Device(LNKE){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 5)

                Method(_STA,0) {
			And(PIRE, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKE desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKE enabled
		}

		Method(_PRS,0){Return(PRSE)}

		Method(_DIS){Or(PIRE, 0x80 ,PIRE)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRE, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRE)
		}					// End Method _SRS
	} // end Device LNKE
//--------------------------------------------------------------------------//
	Device(LNKF){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 6)

                Method(_STA,0) {
			And(PIRF, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKF desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKF enabled
		}

		Method(_PRS,0){Return(PRSF)}

		Method(_DIS){Or(PIRF, 0x80 ,PIRF)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRF, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRF)
		}					// End Method _SRS
	} // end Device LNKF
//--------------------------------------------------------------------------//
	Device(LNKG){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 7)

                Method(_STA,0) {
			And(PIRG, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKG desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKG enabled
		}

		Method(_PRS,0){Return(PRSG)}

		Method(_DIS){Or(PIRG, 0x80 ,PIRG)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRG, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRG)
		}					// End Method _SRS
	} // end Device LNKG
//--------------------------------------------------------------------------//
	Device(LNKH){
                Name(_HID, EISAID("PNP0C0F"))
                Name(_UID, 8)

                Method(_STA,0) {
			And(PIRH, 0x80, Local0)
			If(Local0) {Return (0x9)} // LNKH desabled/present/not visible in UI
			Else   {Return (0xb)} // LNKH enabled
		}

		Method(_PRS,0){Return(PRSH)}

		Method(_DIS){Or(PIRH, 0x80 ,PIRH)} // Disable PIRQ routing  

		Method(_CRS){
			And(PIRH, 0x0f, Local0)
			ShiftLeft(0x1, Local0, IRA0)
			Return(BUFA)
		}					// End Method _CRS

		Method(_SRS, 1){	// ARG0 = PnP Resource String 
			CreateWordField(ARG0, 0x01, IRA)// IRQ Low Mask
			FindSetRightBit(IRA, Local0)
			Decrement(Local0)
			Store(Local0, PIRH)
		}					// End Method _SRS
	} // end Device LNKH
//--------------------------------------------------------------------------//
} // end _SB scope
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//		DON'T CHANGE ANY CODE UNLESS REQUIRED !!!
//*************************************************************************;
