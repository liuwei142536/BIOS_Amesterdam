//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file VirtualSerialPort.asl
    Defines ACPI methods for VirtualSerial device.

**/    

         
 
    Name(VBAN, 0x03F8)
    Name(VIRQ, 0x04)
    Name(UIDN, 0x0)
    Name(VINM, 0x1) //InterruptMode
    Name(VILL, 0x0) //InterruptLevel
    Name(VISH, 0x0) //InterruptShareType
     
    Name(_HID, EISAID("PNP0501"))         //PnP Device ID 16550 Type
    
    Method(_UID, 0) {
        Return(UIDN)
    }
    
    Method(_STA, 0, Serialized) {
        Return(0x000F)
    }

    // Current Resource Setting Method for internal UART 3.
    Method(_CRS,0,Serialized) {
    
        If(LGreaterEqual(VIRQ,0x10)) {
            Name(BUF1,ResourceTemplate() {
                IO(Decode16, 0, 0, 0x01, 0x08, IOP1)
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive,,,INTI) {0}
                DMA(Compatibility, NotBusMaster, Transfer8) {}
            })
            CreateWordField(BUF1, IOP1._MIN, IOBL)
            CreateWordField(BUF1, IOP1._MAX, IOBH)
            Store(VBAN, IOBL )
            Store(VBAN, IOBH )
            CreateDWordField(BUF1, INTI._INT, INUM) // Interrupt Number
            CreateBitField(BUF1, INTI._HE, LEV1)    // Level or Edge
            CreateBitField(BUF1, INTI._LL, ACT1)    // Active High or Low
            CreateField(BUF1, INTI._SHR,2,SHR1) // Interrupt Sharable or not
            Store(VIRQ, INUM)
            Store(VINM, LEV1)
            Store(VILL, ACT1)
            Store(VISH, SHR1)
            Return(BUF1)
        } 
        Else {
            Name(BUF0,ResourceTemplate() {
                IO(Decode16, 0, 0, 0x01, 0x08, IOP0)
                IRQ(Edge, ActiveHigh, Exclusive,IRQx) {}
                DMA(Compatibility, NotBusMaster, Transfer8) {}
            })
            CreateWordField(BUF0, IOP0._MIN, IOPL)
            CreateWordField(BUF0, IOP0._MAX, IOPH)
            Store(VBAN, IOPL )
            Store(VBAN, IOPH )
            CreateWordField(BUF0, IRQx._INT, IRQ) // IRQ Mask
            CreateBitField(BUF0, IRQx._HE, LEV0) // Level or Edge
            CreateBitField(BUF0, IRQx._LL, ACT0) // Active High or Low
            CreateField(BUF0, IRQx._SHR,2,SHR0) // Interrupt Sharable or not
            ShiftLeft(1,VIRQ,IRQ)
            Store(VINM, LEV0)
            Store(VILL, ACT0)
            Store(VISH, SHR0)
            Return(BUF0)
        }
    }
