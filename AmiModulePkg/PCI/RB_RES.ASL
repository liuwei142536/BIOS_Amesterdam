//****************************************************************************
//****************************************************************************
//**                                                                        **
//**         (C)Copyright 1985-2019, American Megatrends, Inc.              **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**          5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/**
   @file RB_RES.ASL

   PCI Root Bridge resources buffer and CRS Method
   MUST be included from NB.ASL directly using "include" ASL term
   Suppose to be already in ROOT BRIDGE Device ASL Scope.

@note:  NO Porting needed 
**/

//Defining some place holders, we need to update at runtime.
        Name(CPRB, 0x0)         //BOOLEAN  Compatible Root Bridge if !=0
        Name(LVGA, 0x55)        //BOOLEAN  Legacy Video Present if !=0
        Name(STAV,0x0F)
//Information Provided in RootBridgeIo->Configuration() function.
        Name(BRB, 0x0100)       //Bus Range Base 
        Name(BRL, 0x0100)       //Bus Range Length

        Name(IOB, 0x0100)       //IO Range BASE (16bit resource)
        Name(IOL, 0x0100)       //IO Range LENGTH

        Name(MBB, 0x10000)      //Memory Range Below 4G BASE (32bit resource)
        Name(MBL, 0x10000)      //Memory Range Below 4G LENGTH (32bit resource)

        Name(MAB, 0x1000000000000)     //Memory Range Above 4G BASE QSDW (64bit resource)
        Name(MAL, 0x1000000000000)     //Memory Range Above 4G LENGTH QSDW (64bit resource)
        Name(MAM, 0x1000000000000)     //Memory Range Above 4G LENGTH QSDW (64bit resource)

//---------------------------------------------------------------------------
    //Resource buffer to be used for COMPATIBILITY ROOT BRIDGE (one that decodes ISA resources)
    Name(CRS1, ResourceTemplate() {
        WORDBusNumber(	        //Bus number resource (0); the bridge produces bus numbers for its subsequent buses
            ResourceProducer,      // bit 0 of general flags is 1
            MinFixed,	        // Range is not fixed
            MaxFixed,	        // Range is not fixed
            PosDecode,	        // PosDecode
            0x0000,		// Granularity
            0x0000,		// Min
            0x007f,		// Max; half of the available bus numbers are reserved for this root bus
            0x0000,		// Translation
            0x0080,      // Range Length
            ,,
            BUS1
        )

		// CF8 - CFC
		IO(			//Consumed resource for 1st root bus(CF8-CFF)
            Decode16,
            0x0cf8,
            0xcf8,
            1,
            8
        )

		// All IO below 3B0
		WORDIO(			//Consumed-and-produced resource (all I/O below 3B0 is assigned to this root bus)
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x0000,		// Min
            0x03af,		// Max
            0x0000,		// Translation
            0x03b0		// Range Length
        )

		// 3E0 - CF7
		WORDIO(			//Consumed-and-produced resource (all I/O between 3E0 and CF8 is assigned to this root bus)
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x03e0,		// Min
            0x0cf7,		// Max
            0x0000,		// Translation
            0x0918		// Range Length
        )

		// For Legacy Video IO (3B0 - 3DF)
		WORDIO(			//Consumed-and-produced resource (Used for legacy video)
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x0000,		// Min
            0x0000,		// Max
            0x0000,		// Translation
            0x0000,		// Range Length
            ,,
            VIO1
        )

		// Other IO used by this root bridge
		WORDIO(			//Consumed-and-produced resource
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x0d00,		// Min
            0x0fff,		// Max
            0x0000,		// Translation
            0x0300,		// Range Length
            ,,
            IOW2
        )

		// For Legacy Video Memory (A000 - BFFF)
		DWORDMEMORY(			// descriptor for video RAM behind ISA bus
	        ResourceProducer,	// bit 0 of general flags is 0
            PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        Cacheable,
	        ReadWrite,
	        0x00000000,		// Granularity
	        0x00000000,		// Min
	        0x00000000,		// Max
	        0x00000000,		// Translation
	        0x00000000,		// Range Length
			,,
			VGA1
	    )

		// ISA Option ROMs
		DWORDMEMORY(		// for device ROMs for ISA plugins devices
	        ResourceProducer,	// bit 0 of general flags is 0
            PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        NonCacheable,
	        ReadWrite,
	        0x00000000,		// Granularity
	        0x000c0000,		// Min
            0x000dffff,		// Max
        	0x00000000,		// Translation
	        0x00020000		// Range Length
	    )

		// Memory < 4GB
		DWORDMEMORY(		// Consumed-and-produced resource(all of memory space)
            ResourceProducer, 	// bit 0 of general flags is 0
            PosDecode,		// positive Decode
            MinFixed,		// Range is fixed
            MaxFixed,		// Range is fixed
            Cacheable,
            ReadWrite,
            0x00000000,		// Granularity
            0x02000000,		// Min (calculated dynamically)
            0xffdfffff,		// Max = 4GB - 2MB
            0x00000000,		// Translation
            0xFDE00000,		// Range Length (calculated dynamically)
            ,               // Optional field left blank
	        ,               // Optional field left blank
    	    MEM3            // Name declaration for this descriptor
            )

		// Memory > 4GB
        QWORDMEMORY(			// descriptor for MMIO ABOVE 4G
	        ResourceProducer,	// bit 0 of general flags is 0
                PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        Cacheable,
	        ReadWrite,
	        0x00000000,	    // Granularity
	        0x000000000,	    // Min
	        0x000000000,	    // Max
	        0x00000000,	    // Translation
	        0x000000000,	    // Range Length
            ,,
            MEM8
	    )

    })//End resource template Name CRS1


    //Resource buffer to be used for NONE COMPATIBILITY ROOT BRIDGE
    Name(CRS2, ResourceTemplate() {
        WORDBusNumber(	        //Bus number resource (0); the bridge produces bus numbers for its subsequent buses
            ResourceProducer,      // bit 0 of general flags is 1
            MinFixed,	        // Range is not fixed
            MaxFixed,	        // Range is not fixed
            PosDecode,	        // PosDecode
            0x0000,		// Granularity
            0x0080,		// Min
            0x00ff,		// Max; half of the available bus numbers are reserved for this root bus
            0x0000,		// Translation
            0x0080,     // Range Length
            ,,
            BUS2
        )

		// For Legacy Video IO (3B0 - 3DF)
        WORDIO(			//Consumed-and-produced resource (Used for legacy video)
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x0000,		// Min
            0x0000,		// Max
            0x0000,		// Translation
            0x0000,		// Range Length
            ,,
            VIO2
        )

		// Other IO used by this root bridge
        WORDIO(			//Consumed-and-produced resource
            ResourceProducer, 	// bit 0 of general flags is 0
            MinFixed,	        // Range is fixed
            MaxFixed,		// Range is fixed
            PosDecode,
            EntireRange,
            0x0000,		// Granularity
            0x0000,		// Min
            0x0000,		// Max
            0x0000,		// Translation
            0x0000,		// Range Length
            ,,
            IOW3
        )

		// For Legacy Video Memory (A000 - BFFF)
        DWORDMEMORY(			// descriptor for video RAM behind ISA bus
	        ResourceProducer,	// bit 0 of general flags is 0
            PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        Cacheable,
	        ReadWrite,
	        0x00000000,		// Granularity
	        0x00000000,		// Min
	        0x00000000,		// Max
	        0x00000000,		// Translation
	        0x00000000,		// Range Length
			,,
			VGA2
	    )

		// Memory < 4GB
        DWORDMEMORY(			// descriptor for video RAM behind ISA bus
	        ResourceProducer,	// bit 0 of general flags is 0
            PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        Cacheable,
	        ReadWrite,
	        0x00000000,	    // Granularity
	        0x80000000,	    // Min
	        0xffffffff,	    // Max
	        0x00000000,	    // Translation
	        0x80000000,	    // Range Length
            ,,
            MEM1
	    )

		// Memory > 4GB
        QWORDMEMORY(			// descriptor for MMIO ABOVE 4G
	        ResourceProducer,	// bit 0 of general flags is 0
            PosDecode,
	        MinFixed,		// Range is fixed
	        MaxFixed,		// Range is Fixed
	        Cacheable,
	        ReadWrite,
	        0x00000000,	    // Granularity
	        0x00000000,	    // Min
	        0x00000000,	    // Max
	        0x00000000,	    // Translation
	        0x00000000,	    // Range Length
            ,,
            MEM9
	    )
    } ) //End resource template Name CRS2

    Method(_STA, 0){
        Return(STAV)
    }

    Method(_CRS, 0, Serialized) {

            //breakpoint


            //This is for compatibility bridges
            If(CPRB){

                //Bus Range
                CreateWordField(CRS1, ^BUS1._MIN, MIN0)	// Min
                CreateWordField(CRS1, ^BUS1._MAX, MAX0)	// Max
                CreateWordField(CRS1, ^BUS1._LEN, LEN0)	// Length

                Store(BRB, MIN0)
                Store(BRL, LEN0)
                Store(LEN0,Local0)
                Add(MIN0, Decrement(Local0), MAX0)

                //Update Upper IO from 0x0D00 up to IOB+IOL
		        CreateWordField(CRS1, ^IOW2._MIN, MIN1)	// Min
		        CreateWordField(CRS1, ^IOW2._MAX, MAX1)	// Max
        		CreateWordField(CRS1, ^IOW2._LEN, LEN1)	// Length


                If(LEqual(IOB, 0x1000)){
                    Store(IOL, Local0)
                    Add(IOB, Decrement(Local0), MAX1)
                    Subtract(MAX1,MIN1,Local0)
                    Add(Local0,1,LEN1)
                } else {
                    Store(IOB, MIN1)
                    Store(IOL, LEN1)
                    Store(LEN1,Local0)
                    Add(MIN1, Decrement(Local0), MAX1)
                }

                If(LOr(LEqual(LVGA,1),LEqual(LVGA,0x55))){
                    if(VGAF){//will update this only once.
                        CreateWordField(CRS1, ^VIO1._MIN, IMN1)	// Min
                        CreateWordField(CRS1, ^VIO1._MAX, IMX1)	// Max
                        CreateWordField(CRS1, ^VIO1._LEN, ILN1)	// Length
    
                        Store(0x03b0, IMN1)
                        Store(0x03df, IMX1)
                        Store(0x0030, ILN1)
    
                        CreateDWordField(CRS1, ^VGA1._MIN, VMN1)	// Min
                        CreateDWordField(CRS1, ^VGA1._MAX, VMX1)	// Max
                        CreateDWordField(CRS1, ^VGA1._LEN, VLN1)	// Length
    
                        Store(0x000a0000, VMN1)
                        Store(0x000bffff, VMX1)
                        Store(0x00020000, VLN1)
                        Store(0, VGAF)
                    }
                }

                //Update MMIO Below 4G
		        CreateDWordField(CRS1, ^MEM3._MIN, MIN3)	// Min
		        CreateDWordField(CRS1, ^MEM3._MAX, MAX3)	// Max
        		CreateDWordField(CRS1, ^MEM3._LEN, LEN3)	// Length

                Store(MBB, MIN3)
                Store(MBL, LEN3)
                Store(LEN3,Local0)
                Add(MIN3, Decrement(Local0), MAX3)

                If(MAL){
                    CreateQWordField(CRS1, ^MEM8._MIN, MN8)	// Min                 
		        	CreateQwordField(CRS1, ^MEM8._MAX, MX8)	// Max                  
	                CreateQwordField(CRS1, ^MEM8._LEN, LN8)	// Len_LO
                  
                    //Update fields.
                    //1. _MIN 64bits...
                    Store(MAB, MN8)           
                    //2. _LEN 64Bits
                    Store(MAL, LN8)                  
                    //2. _MAX 64Bits
                    Store(MAM, MX8)                
                }


                Return(CRS1)
            } else {
                //Bus Range
                CreateWordField(CRS2, ^BUS2._MIN, MIN2)	// Min
                CreateWordField(CRS2, ^BUS2._MAX, MAX2)	// Max
                CreateWordField(CRS2, ^BUS2._LEN, LEN2)	// Length

                Store(BRB, MIN2)
                Store(BRL, LEN2)
                Store(LEN2,Local1)
                Add(MIN2, Decrement(Local1), MAX2)

                //Update Upper IO from IOB up to IOB+IOL
		        CreateWordField(CRS2, ^IOW3._MIN, MIN4)	// Min
		        CreateWordField(CRS2, ^IOW3._MAX, MAX4)	// Max
        		CreateWordField(CRS2, ^IOW3._LEN, LEN4)	// Length

                Store(IOB, MIN4)
                Store(IOL, LEN4)
                Store(LEN4,Local1)
                Add(MIN4, Decrement(Local1), MAX4)

				If(LVGA){
		        	CreateWordField(CRS2, ^VIO2._MIN, IMN2)	// Min
		        	CreateWordField(CRS2, ^VIO2._MAX, IMX2)	// Max
        			CreateWordField(CRS2, ^VIO2._LEN, ILN2)	// Length

                	Store(0x03b0, IMN2)
                	Store(0x03df, IMX2)
                	Store(0x0030, ILN2)

		        	CreateDWordField(CRS2, ^VGA2._MIN, VMN2)	// Min
		        	CreateDWordField(CRS2, ^VGA2._MAX, VMX2)	// Max
        			CreateDWordField(CRS2, ^VGA2._LEN, VLN2)	// Length

                	Store(0x000a0000, VMN2)
                	Store(0x000bffff, VMX2)
                	Store(0x00020000, VLN2)
				}

                //Update MMIO Below 4G
		        CreateDWordField(CRS2, ^MEM1._MIN, MIN5)	// Min
		        CreateDWordField(CRS2, ^MEM1._MAX, MAX5)	// Max
        		CreateDWordField(CRS2, ^MEM1._LEN, LEN5)	// Length

                Store(MBB, MIN5)
                Store(MBL, LEN5)
                Store(LEN5,Local1)
                Add(MIN5, Decrement(Local1), MAX5)

                //Update Above 4G memory space if any

                If(MAL){
                    CreateQWordField(CRS1, ^MEM9._MIN, MN9)	// Min                 
		        	CreateQwordField(CRS1, ^MEM9._MAX, MX9)	// Max                  
	                CreateQwordField(CRS1, ^MEM9._LEN, LN9)	// Len_LO
                  
                    //Update fields.
                    //1. _MIN 64bits...
		    Store(MAB, MN8)           
                    //2. _LEN 64Bits
                    Store(MAL, LN8)                  
                    //2. _MAX 64Bits
                    Store(MAM, MX8)                
                    }

                Return(CRS2)
            }

	} // end ROOT BRIDGE PCIx._CRS Method
//----------------------------------------------------------------------------

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**         (C)Copyright 1985-2019, American Megatrends, Inc.              **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**          5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
