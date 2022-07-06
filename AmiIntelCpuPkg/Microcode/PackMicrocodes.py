#***********************************************************************
#*                                                                     *
#*                  Copyright (c) 1985-2021, AMI.                      *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

# -*- coding: utf-8 -*-

import os
import sys
import argparse
import struct

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I',bytearray (A + B + C + D, 'ascii'))[0]

class MpdtClass (object):
    #
    # typedef struct {
    #   UINT32  Signature;  'MPDT'
    #   UINT8   Revision;
    #   UINT8   IsBootBlock;
    #   UINT8   Manufacture;  0 = Intel
    #   UINT8   Reserverd1;
    #   UINT32  MicrocodeAligment;
    #   UINT16  Reserved2;
    #   UINT16  TableSize;    16
    # } FMP_PAYLOAD_HEADER;
    #
    # #define MPDT_SIGNATURE SIGNATURE_32 ('M', 'P', 'D', 'T')
    #
    _StructFormat = '<I4BI2H'
    _StructSize   = struct.calcsize (_StructFormat)

    MPDT_SIGNATURE = _SIGNATURE_32 ('M', 'P', 'D', 'T')

    def __init__ (self):
        self.Signature              = self.MPDT_SIGNATURE
        self.Revision               = 0
        self.IsBootBlock            = 0
        self.Manufacture            = 0
        self.Reserverd1             = 0
        self.MicrocodeAligment      = 0
        self.Reserverd2             = 0
        self.TableSize              = 16

    def Encode (self):
        Mpdt = struct.pack (
                                self._StructFormat,
                                self.Signature,
                                self.Revision,
                                self.IsBootBlock,
                                self.Manufacture,
                                self.Reserverd1,
                                self.MicrocodeAligment,
                                self.Reserverd2,
                                self.TableSize
                                )
        return Mpdt

def HexToInt(x):
        return int(x, 0)

def main():
    parser = argparse.ArgumentParser(prog="PackMicrocodes", description="Pack microcode with fixed slot size")
    parser.add_argument("InputMicrocodes", help="Input Microcodes", nargs='+')
    parser.add_argument("-o", "--OutputPackMicrocodes", help="Output packed Microcodes")
    parser.add_argument("-s", "--SlotSize", help="Microcode slot size, Please input Hex value", type=HexToInt)
    parser.add_argument("-Mpdt", "--AddMpdt", help="create MPDT in the end of output binary", action="store_true")
    parser.add_argument("-v", "--verbose", help="increase output verbosity", action="store_true")
    args = parser.parse_args()
    
    if args.SlotSize == None:
        print("Please input slot size")
        sys.exit(1)
        return

    if args.verbose and args.SlotSize:
        print("0x%x" %(args.SlotSize))
    
    if args.OutputPackMicrocodes == None:
        print("Please input output packed file path/name")
        sys.exit(1)
        return
		
    #Check slot size and microcode exist
    for MicrocodeName in args.InputMicrocodes:
        try:
            MicrocodeSize = os.path.getsize(MicrocodeName)
            if args.SlotSize < MicrocodeSize:
                print("The microcode slot size(0x%x) is smaller than microcode size(0x%x)" %(args.SlotSize, MicrocodeSize))
                sys.exit(2)
                return
        except SystemExit as e:
            if str(e) != '2':
                print("Don't find microcode %s" %(MicrocodeName))
            sys.exit(1)
            return

    PackMicrocodes = open(args.OutputPackMicrocodes, "wb+")
    BlankByte = bytes([0xFF])
    for MicrocodeName in args.InputMicrocodes:
        MicrocodeSize = os.path.getsize(MicrocodeName)
        Microcode = open(MicrocodeName, "rb")
        #Copy microcode
        PackMicrocodes.write(Microcode.read())
        Position = PackMicrocodes.tell()
        if args.verbose:
            print("Microcode size(0x%x), Microcode end Position(0x%x)" %(MicrocodeSize, Position))
        BlankSize = args.SlotSize - MicrocodeSize
        for i in range(Position, Position + BlankSize):
            PackMicrocodes.write(BlankByte)
        if args.verbose:    
            print("Microcode add blank end Position(0x%x)" %(PackMicrocodes.tell()))
        Microcode.close()

    #Mpdt
    if args.AddMpdt:
        Mpdtstru = MpdtClass()
        Mpdtstru.MicrocodeAligment = args.SlotSize
        Mpdt = Mpdtstru.Encode()
        if args.verbose:
            print(Mpdt.hex())
        PackMicrocodes.write(Mpdt)

    PackMicrocodes.close()
    sys.exit(0)
    return


if __name__ == '__main__':
    try:
        main()
        sys.exit(0)
    except SystemExit as e:
        print('Run PackMicrocodes fail')
        sys.exit(0)

    
