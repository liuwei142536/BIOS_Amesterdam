//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file TerminalSetupVar.h
    Description: Contains definition of PCI_SERIAL_PORTS_LOCATION_VAR 
    and of varstore declaration SERIAL_PORTS_ENABLED_VAR.
**/

#ifndef _TERMINAL_SETUP_VAR_H
#define _TERMINAL_SETUP_VAR_H
#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
#include <Token.h>

//----------------------------------------------------------------------------
#define TERMINAL_VAR_GUID \
{0x560bf58a, 0x1e0d, 0x4d7e, 0x95, 0x3f, 0x29, 0x80, 0xa2, 0x61, 0xe0, 0x31}

#define TERMINAL_PCISERIAL_VAR_GUID \
{0x1d35ffe, 0x2e82, 0x4ce7, 0xa1, 0xa6, 0x26, 0x55, 0xd1, 0xec, 0x1a, 0xee}

#define TERMINAL_SERIAL_VAR_C_NAME         L"TerminalSerialVar"

#define MAX_SIO_SERIAL_PORTS 10
#define MAX_PCI_SERIAL_PORTS 4
#define TERMINAL_PORT0 0x0
#define TERMINAL_PORT1 0x1
#define TERMINAL_PORT2 0x2
#define TERMINAL_PORT3 0x3
#define TERMINAL_PORT4 0x4
#define TERMINAL_PORT5 0x5
#define TERMINAL_PORT6 0x6
#define TERMINAL_PORT7 0x7
#define TERMINAL_PORT8 0x8
#define TERMINAL_PORT9 0x9

#define HARDWARE_FLOW_CONTROL_SETUP_OPTION 1
#define SOFTWARE_FLOW_CONTROL_SETUP_OPTION 2

#define PUTTY_VT100   0x1
#define PUTTY_LINUX   0x2
#define PUTTY_XTERMR6 0x4
#define PUTTY_SCO     0x8
#define PUTTY_ESCN    0x10
#define PUTTY_VT400   0x20

#pragma pack(1)

#ifndef TYPEDEF_DEBUGGER_SERIAL_PORTS_ENABLED_VAR
#define TYPEDEF_DEBUGGER_SERIAL_PORTS_ENABLED_VAR
    typedef struct {
        UINT8 PortEnabled;
    } DEBUGGER_SERIAL_PORTS_ENABLED_VAR;
#endif

#if (TOTAL_SERIAL_PORTS > 0)
#ifndef TYPEDEF_SERIAL_PORTS_ENABLED_VAR
#define TYPEDEF_SERIAL_PORTS_ENABLED_VAR
    typedef struct { 
        UINT8 PortsEnabled[ TOTAL_SERIAL_PORTS ];
    } SERIAL_PORTS_ENABLED_VAR;
#endif
#endif

#if (TOTAL_SIO_SERIAL_PORTS > 0)
#ifndef TYPEDEF_SIO_SERIAL_PORTS_LOCATION_VAR
#define TYPEDEF_SIO_SERIAL_PORTS_LOCATION_VAR
    typedef struct { 
        UINT8 PortUid[ TOTAL_SIO_SERIAL_PORTS ];
        UINT8 Valid[ TOTAL_SIO_SERIAL_PORTS ];
    } SIO_SERIAL_PORTS_LOCATION_VAR;
#endif
#endif

#if (TOTAL_PCI_SERIAL_PORTS > 0)
#ifndef TYPEDEF_PCI_SERIAL_PORTS_LOCATION_VAR
#define TYPEDEF_PCI_SERIAL_PORTS_LOCATION_VAR
    typedef struct { 
        UINT8 Segment[ TOTAL_PCI_SERIAL_PORTS ];
        UINT8 Bus[ TOTAL_PCI_SERIAL_PORTS ];
        UINT8 Device[ TOTAL_PCI_SERIAL_PORTS ];
        UINT8 Function[ TOTAL_PCI_SERIAL_PORTS ];
        UINT8 AmiPciSerialPresent[ TOTAL_PCI_SERIAL_PORTS ];
        UINT8 Port[ TOTAL_PCI_SERIAL_PORTS ];
    } PCI_SERIAL_PORTS_LOCATION_VAR;
#endif
#endif
#ifndef VFRCOMPILE
typedef struct {
    UINT16      VendorID;
    UINT16      DeviceID;
    UINTN      	UartClock;
    UINT8       MmioWidth;
}PCI_DEVICE_UART_CLOCK_WIDTH;
#endif

typedef struct {
    
#if (TOTAL_SERIAL_PORTS > 0)
    SERIAL_PORTS_ENABLED_VAR              SerialPortsEnabledVar;
#if (TOTAL_PCI_SERIAL_PORTS > 0)
    PCI_SERIAL_PORTS_LOCATION_VAR         PciSerialPortsLocationVar;
#endif
#if (TOTAL_SIO_SERIAL_PORTS > 0)
    SIO_SERIAL_PORTS_LOCATION_VAR         SioSerialPortsLocationVar;
#endif
#endif   
    DEBUGGER_SERIAL_PORTS_ENABLED_VAR     DebuggerSerialPortsEnabledVar;
}TERMINAL_SERIAL_SUPPORT;     

#pragma pack()

#ifdef __cplusplus
}
#endif
#endif

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
