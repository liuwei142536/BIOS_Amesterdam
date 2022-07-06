//
// This file contains 'Framework Code' and is licensed as such 
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.                 
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SPIOpcode.h

Abstract:

  define SPI opcode index

--*/

#ifndef _SPI_OPCODE_H_
#define _SPI_OPCODE_H_

//
// SPI default opcode index
//
#define SPI_OPCODE_JEDEC_ID_INDEX        0
#define SPI_OPCODE_WRITE_S2_INDEX        1
#define SPI_OPCODE_WRITE_S_INDEX         2
#define SPI_OPCODE_WRITE_INDEX           3 
#define SPI_OPCODE_READ_INDEX            4
#define SPI_OPCODE_ERASE_INDEX           5
#define SPI_OPCODE_READ_S_INDEX          6
#define SPI_OPCODE_READ_S2_INDEX         7

#endif
