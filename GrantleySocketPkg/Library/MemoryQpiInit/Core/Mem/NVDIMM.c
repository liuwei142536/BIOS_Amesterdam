#include "SysFunc.h"
//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)

#ifdef MEM_NVDIMM_EN

#define SPD_MODULE_PN_SIZE  18
#define NV_NAME_LEN         18
#define NV_REPORT           1
#define NV_REPORT_NOT       0

typedef UINT32(*NVDIMM_FUNC)(PSYSHOST host, UINT8 socket, struct smbDevice dev, UINT32 vendorIndex);

typedef struct _nvDev {
  UINT16         nvVid;                              /* SPD Vender ID                                                       */
  UINT8          nvDid[SPD_MODULE_PN_SIZE + 1];        /* SPD Device ID                                                       */
  UINT32         nvDidCnt;                           /* Str Len for nvDid match: Multiple matched devices use this control table  */
  UINT8          nvName[NV_NAME_LEN];                /* sZ Vender name                                                      */
  UINT8          smbusAdr;                           /* SMBus address used for NVDIMM operations                            */
  UINT32         vendorIndex;  // Unique number per vendor to allow handling multiple NVDIMMs from one function
  NVDIMM_FUNC    getStatus;
  NVDIMM_FUNC    startRestore;
  NVDIMM_FUNC    restoreDone;
  NVDIMM_FUNC    arm;
} nvDev;


UINT32
ReadSmbTimeout(
PSYSHOST         host,
UINT8            socket,
struct smbDevice smbusdev,
  UINT8            byteOffset,
  UINT8            *data,
  UINT8            mask,
  UINT8            expectedvalue,
  UINT16           timeout
  )
{
  UINT32        passnum = 0;
  UINT32        status;
  UINT64_STRUCT startTsc;
  UINT64_STRUCT currentTsc;

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "    Starting a Poll Reg=0x%x, Mask=0x%x, Expected Value=0x%x\n", byteOffset, mask, expectedvalue));

  ReadTsc64(&startTsc);
  do {
    status = ReadSmb(host, socket, smbusdev, byteOffset, data);
    if (status == FAILURE) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "      SMBUS Read FAILED!!!! Reg=0x%x, Mask=0x%x, Expected Value=0x%x, last read=0x%x\n", byteOffset, mask, expectedvalue, *data));
      return FAILURE;
    }

    if ((passnum & 0xFF) == 1)
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "        Data=0x%x\n", *data));

    if (passnum != 0)
      FixedDelay(host, 10000);

    passnum++;

    ReadTsc64(&currentTsc);
    if (TimeDiff(host, startTsc, currentTsc, TDIFF_UNIT_S) > timeout) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "      ReadSmbTimeoutTimeout after %d seconds, Reg=0x%x, Mask=0x%x, Expected Value=0x%x, last read=0x%x\n", timeout, byteOffset, mask, expectedvalue, *data));
      return FAILURE;
    }
  } while ((*data & mask) != expectedvalue);
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "      Final Data=0x%x\n", *data));
  return SUCCESS;
} // ReadSmbTimeout

UINT32
NetlistStatus(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT32  status;
  UINT8   data;

  // Wait for NVDIMM to init
  status = ReadSmbTimeout(host, socket, smbusdev, 0x9, &data, (1 << 3), (1 << 3), 6);
  if (status == FAILURE) return NVDIMM_ERROR;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "  Making sure there aren't any operations in progress\n"));
#endif // SERIAL_DBG_MSG
  status = ReadSmbTimeout(host, socket, smbusdev, 0x8, &data, 0x7, 0, 5 * 60);

  FixedDelay(host, 1000000);

  status = ReadSmb(host, socket, smbusdev, 0x20, &data);
  if (status == FAILURE) return NVDIMM_ERROR;
  if (data & (1 << 7)) return NVDIMM_ERROR;

  // Read Nonvolatile Status Flag
  status = ReadSmb(host, socket, smbusdev, 0xC, &data);
  if (status == FAILURE)
    return NVDIMM_ERROR;

  switch (data) {
  case 0:
  case 1:
    return NVDIMM_NODATA;
    break;

  case 3:
    return NVDIMM_RESTORE;
    break;

  default:
  case 2:
    return NVDIMM_ERROR;
    break;
  }

  //Should never make it here
  //return NVDIMM_ERROR;
} // NetlistStatus


UINT32
NetlistRestoreStart(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  data = 1;
  status = WriteSmb(host, socket, smbusdev, 0x4, &data);

  return SUCCESS;
} // NetlistRestoreStart

UINT32
NetlistRestoreDone(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT8   i;
  UINT32  status;

  //Wait for State to change to 0x8
  status = ReadSmbTimeout(host, socket, smbusdev, 0x10, &data, 0xFF, 0x8, 5 * 60);
  if (status == FAILURE) return FAILURE;

  // Paranoia: Also check for RIP bit to go low
  status = ReadSmbTimeout(host, socket, smbusdev, 0x8, &data, (1 << 1), 0, 60);
  if (status == FAILURE) return FAILURE;

  if (data & (1 << 7)) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Error during restore\n"));
#endif // SERIAL_DBG_MSG

    for (i = 0x20; i < 0x28; i++) {
      status = ReadSmb(host, socket, smbusdev, i, &data);
#ifdef SERIAL_DBG_MSG
      if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Error Reg 0x%x=0x%x\n", i, data));
#endif // SERIAL_DBG_MSG
      status = WriteSmb(host, socket, smbusdev, i, &data);
    } // i loop
  }

  return SUCCESS;
} // NetlistRestoreDone

UINT32
NetlistArm(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;


#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Waiting for NVDIMM to charge\n"));
#endif // SERIAL_DBG_MSG

  status = ReadSmbTimeout(host, socket, smbusdev, 8, &data, (1 << 4), (1 << 4), 10 * 60);
  if (status == FAILURE) return FAILURE;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Erasing\n"));
#endif // SERIAL_DBG_MSG

  // Always erase
  // Initiate Erase
  data = (1 << 2) | (1 << 0);
  WriteSmb(host, socket, smbusdev, 0x4, &data);
  // Wait for Erase to Finish
  status = ReadSmbTimeout(host, socket, smbusdev, 8, &data, (1 << 2), 0, 60);
  if (status == FAILURE) return FAILURE;

  // Enable SAVE_N pin
  data = (1 << 6) | (1 << 1) | (1 << 0);
  status = WriteSmb(host, socket, smbusdev, 0x4, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Making sure NVDIMM is armed\n"));
#endif // SERIAL_DBG_MSG

  status = ReadSmbTimeout(host, socket, smbusdev, 0x9, &data, (1 << 1) | (1 << 2), (1 << 1) | (1 << 2), 60);
  if (status == FAILURE) {
#ifdef SERIAL_DBG_MSG
    status = ReadSmb(host, socket, smbusdev, 0xC, &data);
    if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "NV STATUS Register = 0x%x\n", data));
#endif // SERIAL_DBG_MSG
    return FAILURE;
  }

  return SUCCESS;
} // NetlistArm


UINT32
AgigaStatus(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Wait for NVDIMM Enabled\n"));
#endif // SERIAL_DBG_MSG
  status = ReadSmbTimeout(host, socket, smbusdev, 9, &data, 0xff, 0xa5, 6);
  if (status == FAILURE) return FAILURE;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX))   rcPrintf((host, "Making sure there isn't a Restore in Progress\n"));
#endif // SERIAL_DBG_MSG

  status = ReadSmbTimeout(host, socket, smbusdev, 0xA, &data, 0xff, 0, (2 * 60));
  if (status == FAILURE) return FAILURE;

  status = ReadSmb(host, socket, smbusdev, 20, &data);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Backup Status Register = 0x%x\n", data));
#endif // SERIAL_DBG_MSG

  if (data & (1 << 1))
    return NVDIMM_RESTORE;
  else
    return NVDIMM_NODATA;
} // AgigaStatus

UINT32
AgigaRestoreStart(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8 data;

  // Start the Restore
  data = 0x96;
  WriteSmb(host, socket, smbusdev, 11, &data);

  return SUCCESS;
} // AgigaRestoreStart

UINT32
AgigaRestoreDone(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  // Wait until Restore is complete
  status = ReadSmbTimeout(host, socket, smbusdev, 11, &data, 0xff, 0, (5 * 60));
  if (status == FAILURE) return FAILURE;

  ReadSmb(host, socket, smbusdev, 21, &data);
  if (!(data & 0x1)) {

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) {
      rcPrintf((host, "Restore Status Register = 0x%x\n", data));
      rcPrintf((host, "Restore FAILED\n", data));
    }
#endif // SERIAL_DBG_MSG
    return FAILURE;
  }

  return SUCCESS;
} // AgigaRestoreDone

UINT32
AgigaArm(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  // Make sure Cap is charged

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Waiting for Cap to Charge\n"));
#endif // SERIAL_DBG_MSG
  status = ReadSmbTimeout(host, socket, smbusdev, 18, &data, 7, 7, (10 * 60));
  if (status == FAILURE) return FAILURE;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Erasing the NVDIMM\n"));
#endif // SERIAL_DBG_MSG
  data = 0x37;
  status = WriteSmb(host, socket, smbusdev, 0xC, &data);

  status = ReadSmbTimeout(host, socket, smbusdev, 0x12, &data, 0x7F, 0x7F, 60);
  if (status == FAILURE) return FAILURE;
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "GTG1=0x%x,", data));
#endif // SERIAL_DBG_MSG
  ReadSmb(host, socket, smbusdev, 0x13, &data);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, " GTG2=0x%x\n", data));
#endif // SERIAL_DBG_MSG
  // Arming the NVDIMM
  data = 0x18;
  status = WriteSmb(host, socket, smbusdev, 8, &data);

  return SUCCESS;
} // AgigaArm

UINT32 VikingStatus(
  PSYSHOST          host,
  UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  // Disable All Save Triggers
  data = 0xC0;
  WriteSmb(host, socket, smbusdev, 0x1, &data);


  // Make sure a Save isn't in progress
  status = ReadSmbTimeout(host, socket, smbusdev, 0, &data, 3, 0, 2 * 60);

  // Issue a Reset Command
  data = 0x8F;
  WriteSmb(host, socket, smbusdev, 0x5, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Starting 7s Delay\n"));
#endif
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);
  FixedDelay(host, 1000000);

  status = ReadSmb(host, socket, smbusdev, 0x0, &data);
  if (status == FAILURE) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "FAILED to read Status register1\n"));
#endif // SERIAL_DBG_MSG

    return NVDIMM_ERROR;
  }

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Status Register = 0x%x\n", data));
#endif // SERIAL_DBG_MSG

  if (data & (1 << 2)) {
    // Check for Host control again
    status = ReadSmbTimeout(host, socket, smbusdev, 0xC, &data, 1, 1, 120);
    return NVDIMM_RESTORE;
  }
  else {
    // Switch Memory control to FPGA
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX))
      rcPrintf((host, "Switching control to FPGA\n"));
#endif
    data = 0x80;
    WriteSmb(host, socket, smbusdev, 0x5, &data);

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Waiting for DDR Init Bit to go high\n"));
#endif
    // Wait for DDR Init Bit to go high
    status = ReadSmbTimeout(host, socket, smbusdev, 0xC, &data, (1 << 1), (1 << 1), 30);
    if (status == FAILURE) return FAILURE;

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Switching Control back to host\n"));
#endif
    // Switch Memory control back to CPU
    data = 0x88;
    WriteSmb(host, socket, smbusdev, 0x5, &data);
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Checking for Host Control\n"));
#endif
    // Check for Host control again
    status = ReadSmbTimeout(host, socket, smbusdev, 0xC, &data, 1, 1, 120);
    // Figure Out how to Handle this better
    if (status == FAILURE) return NVDIMM_ERROR;

    return NVDIMM_NODATA;
  }
} // VikingStatus

UINT32
VikingRestoreStart(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  // Switch Memory control to FPGA
  data = 0x80;
  WriteSmb(host, socket, smbusdev, 0x5, &data);

  FixedDelay(host, 5000);

  // Wait for DDR Init Bit to go high
  status = ReadSmbTimeout(host, socket, smbusdev, 0xC, &data, (1 << 1), (1 << 1), 30);
  if (status == FAILURE) return FAILURE;

  FixedDelay(host, 5000);

  // Make sure NVDIMM is idle
  status = ReadSmbTimeout(host, socket, smbusdev, 0, &data, 3, 0, 30);
  if (status == FAILURE) return FAILURE;

  FixedDelay(host, 5000);

  // Make sure SSD is Ready
  status = ReadSmbTimeout(host, socket, smbusdev, 6, &data, (1 << 4), (1 << 4), 30);
  if (status == FAILURE) return FAILURE;

  FixedDelay(host, 5000);

  // Start Restore
  data = 0x82;
  WriteSmb(host, socket, smbusdev, 0x5, &data);

  FixedDelay(host, 5000);

  // Make sure Restore has started
  status = ReadSmbTimeout(host, socket, smbusdev, 0, &data, 3, 3, 30);
  if (status == FAILURE) return FAILURE;

  FixedDelay(host, 5000);

  return SUCCESS;
} // VikingRestoreStart


UINT32
VikingRestoreDone(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  restorestatus;
  UINT32  status;
  volatile int a = 0;

  // Waiting for restore to complete
  restorestatus = ReadSmbTimeout(host, socket, smbusdev, 0, &data, (1 << 3), (1 << 3), 5 * 60);

  FixedDelay(host, 5000);

  // Switch Memory control back to CPU
  data = 0x88;
  WriteSmb(host, socket, smbusdev, 0x5, &data);

  // Check for Host control again
  status = ReadSmbTimeout(host, socket, smbusdev, 0xC, &data, 1, 1, 30);

  // Figure Out how to Handle this better
  if (status == FAILURE) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Couldn't switch back control to host\n"));
#endif
    while (a == 0);

    return FAILURE;
  }

  return restorestatus;
} // VikingRestoreDone


UINT32
VikingArm(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Waiting for Cap to charge\n"));
#endif
  status = ReadSmbTimeout(host, socket, smbusdev, 0, &data, (1 << 5), (1 << 5), 10 * 60);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Clearing SDV\n"));
#endif
  data = 0x40;
  WriteSmb(host, socket, smbusdev, 0x5, &data);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "Making sure SAVE pin is only save trigger\n"));
#endif
  data = 0;
  WriteSmb(host, socket, smbusdev, 0x1, &data);

  status = ReadSmb(host, socket, smbusdev, 0x0, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Status Register0 = 0x%x\n", data));
#endif

  status = ReadSmb(host, socket, smbusdev, 0xC, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "Status Register1 = 0x%x\n", data));
#endif

  return status;
} // VikingArm

UINT32
SmartUnlock(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev
  )
{
  UINT32  status;
  UINT8   data;
  UINT8   unlockdata[] = { 'S', 'M', 'O', 'D' };

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Unlocking\n"));
#endif // SERIAL_DBG_MSG

  WriteSmb(host, socket, smbusdev, 0x3D, &unlockdata[0]);
  WriteSmb(host, socket, smbusdev, 0x3D, &unlockdata[1]);
  WriteSmb(host, socket, smbusdev, 0x3D, &unlockdata[2]);
  WriteSmb(host, socket, smbusdev, 0x3D, &unlockdata[3]);

  status = ReadSmbTimeout(host, socket, smbusdev, 0x3D, &data, 0x80, 0x80, 10);

  if (status != SUCCESS) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "    Smart Unlock:FAILED\n"));
#endif // SERIAL_DBG_MSG
    return FAILURE;
  }

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Unlock:Passed\n"));
#endif // SERIAL_DBG_MSG
  return SUCCESS;
} // SmartUnlock

UINT32 SmartStatus(
  PSYSHOST          host,
  UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  //Make Sure NVDIMM is alive
  status = ReadSmbTimeout(host, socket, smbusdev, 0xF, &data, 0x80, 0x80, 10);

  status = SmartUnlock(host, socket, smbusdev);
  if (status == FAILURE) return NVDIMM_ERROR;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Making sure there isn't a Restore in Progress\n"));
#endif // SERIAL_DBG_MSG

  status = ReadSmbTimeout(host, socket, smbusdev, 0x8, &data, (1 << 1), 0, 4 * 60);
  if (status == FAILURE) return NVDIMM_ERROR;

  status = ReadSmb(host, socket, smbusdev, 0xA, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Status/Event #1(0xA)=0x%x\n", data));
#endif // SERIAL_DBG_MSG
  // Clear the register
  WriteSmb(host, socket, smbusdev, 0xA, &data);

  status = ReadSmb(host, socket, smbusdev, 0xB, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Status/Event #2(0xB)=0x%x\n", data));
#endif // SERIAL_DBG_MSG
  // Clear the register
  WriteSmb(host, socket, smbusdev, 0xB, &data);

  status = ReadSmb(host, socket, smbusdev, 0xE, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    State Information #1(0xE)=0x%x\n", data));
#endif

  status = ReadSmb(host, socket, smbusdev, 0xF, &data);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    State Information #2(0xF)=0x%x\n", data));
#endif
  if (status == FAILURE) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) rcPrintf((host, "    FAILED to read State Information Register"));
#endif // SERIAL_DBG_MSG
    return NVDIMM_ERROR;
  }

  if (data & (1 << 2))
    return NVDIMM_RESTORE;
  else
    return NVDIMM_NODATA;
} // SmartStatus


UINT32
SmartRestoreStart(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8 data;

  // Start the Restore
  data = 1 << 4;
  WriteSmb(host, socket, smbusdev, 0x8, &data);

  return SUCCESS;
} // SmartRestoreStart

UINT32
SmartRestoreDone(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

  // Waiting for restore to complete
  status = ReadSmbTimeout(host, socket, smbusdev, 0x8, &data, (1 << 4), 0, 2 * 60);
  if (status == FAILURE) return FAILURE;

  status = ReadSmbTimeout(host, socket, smbusdev, 0xB, &data, 1, 1, 10);
  if (status == FAILURE) return FAILURE;

  // Check for Restore Failure
  status = ReadSmb(host, socket, smbusdev, 0xB, &data);
  if (status == FAILURE) return FAILURE;

  if (data & (1 << 3)) return FAILURE;

  // Clear Trigger Done bit
  WriteSmb(host, socket, smbusdev, 0xB, &data);

  return SUCCESS;
} // SmartRestoreDone


UINT32
SmartArm(
PSYSHOST          host,
UINT8             socket,
struct smbDevice  smbusdev,
  UINT32            vendorIndex
  )
{
  UINT8   data;
  UINT32  status;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Waiting for the CAP to charge\n"));
#endif  // SERIAL_DBG_MSG
  status = ReadSmbTimeout(host, socket, smbusdev, 0xF, &data, (1 << 3), (1 << 3), 3 * 60);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Erasing Flash\n"));
#endif  // SERIAL_DBG_MSG
  data = 1 << 5;
  WriteSmb(host, socket, smbusdev, 0x8, &data);

  status = ReadSmbTimeout(host, socket, smbusdev, 0x8, &data, (1 << 5), 0, 10);
  if (status == FAILURE) return FAILURE;

  status = ReadSmbTimeout(host, socket, smbusdev, 0xB, &data, 1, 1, 10);
  if (status == FAILURE) return FAILURE;

  // Clear Trigger Done bit
  WriteSmb(host, socket, smbusdev, 0xB, &data);

  // Check for Erase errors
  status = ReadSmb(host, socket, smbusdev, 0xA, &data);
  if (status == FAILURE)
    return FAILURE;
  if (data & (1 << 1))
    return FAILURE;

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) rcPrintf((host, "    Arming NVDIMM\n"));
#endif  // SERIAL_DBG_MSG
  data = 1;
  WriteSmb(host, socket, smbusdev, 0x7, &data);

  // Verify NVDIMM Armed
  status = ReadSmbTimeout(host, socket, smbusdev, 0xE, &data, 0x80, 0x80, 10);
  if (status == FAILURE) return FAILURE;

  return SUCCESS;
} // SmartArm

nvDev nv[] =
{
  { // Dummy Entry, so none of the devices have an index of 0
    0xFFFF,
  },
  { /*----         Viking         ----*/
    0x4001,              /* nvVid    */
    "VRA7EA",            /* nvDid    */
    6,                   /* nvDidCnt */
    "Viking",            /* nvName   */
    0xB,                /* smbusAdr */
    0,         /* Vendor index */
    VikingStatus,
    VikingRestoreStart,
    VikingRestoreDone,
    VikingArm
  },
  { /*----         Viking         ----*/
    0x4001,              /* nvVid    */
    "VRA9MR",            /* nvDid    */
    6,                   /* nvDidCnt */
    "Viking",            /* nvName   */
    0x8,                /* smbusAdr */
    0,         /* Vendor index */
    VikingStatus,
    VikingRestoreStart,
    VikingRestoreDone,
    VikingArm
  },

  { /*----          Smart        ----*/
    0x9401,              /* nvVid    */
    "SGN",   /* nvDid    */
    3,                  /* nvDidCnt */
    "Smart",             /* nvName   */
    0x0B,                /* smbusAdr */
    0,
    SmartStatus,
    SmartRestoreStart,
    SmartRestoreDone,
    SmartArm
  },
  { /*----          Smart        ----*/
    0x9401,              /* nvVid    */
    "S",   /* nvDid    */
    1,                  /* nvDidCnt */
    "Smart",             /* nvName   */
    0x08,                /* smbusAdr */
    0,
    SmartStatus,
    SmartRestoreStart,
    SmartRestoreDone,
    SmartArm
  },
  { /**----      Agigatech      -----*/
    0x3400,               /* nvVid   3480 BIT7 is cleard on SPD read??? */
    "AGIGA8",             /* nvDid    */
    5,                   /* nvDidCnt max len 18 ch*/
    "Agigatech",          /* nvName   */
    0x02,                 /* smbusAdr    */
    0,
    AgigaStatus,
    AgigaRestoreStart,
    AgigaRestoreDone,
    AgigaArm
  },
  { /**----      Micron      -----*/
    0x2C00,               /* nvVid   3480 BIT7 is cleard on SPD read??? */
    "",             /* nvDid    */
    0,                   /* nvDidCnt max len 18 ch*/
    "Micron",          /* nvName   */
    0x02,                 /* smbusAdr    */
    0,
    AgigaStatus,
    AgigaRestoreStart,
    AgigaRestoreDone,
    AgigaArm
  },
  { /*----       Netlist         ----*/
    0x1603,              /* nvVid    */
    "NV3",        /* nvDid    */
    3,                  /* nvDidCnt */
    "Netlist",           /* nvName   */
    0x0B,                /* smbusAdr    */
    0,
    NetlistStatus,
    NetlistRestoreStart,
    NetlistRestoreDone,
    NetlistArm
  },
  {
    0x00000000    /* Table End */
  }
};

#define NV_OFFSET 206
#define MFGID_OFFSET 203
#define PN_OFFSET 201

UINT32
CoreDetectNVDIMM(
PSYSHOST          host,
UINT8             socket,
UINT8             ch,
UINT8             dimm,
struct smbDevice  spd
  )
{
  UINT32  nvIdx = 1;
  UINT8   i;
  UINT8   SPDReg;
  UINT16  SPDMMfgId;
  UINT16 offset;

  offset = 0;
  if (host->nvram.mem.dramType == SPD_TYPE_DDR4)
    offset = NV_OFFSET;
  else
    offset = 0;

  ReadSpd(host, socket, ch, dimm, SPD_NVDIMM_ID_N + offset, &SPDReg);
  MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "NVDIMM:N(%d)=0x%x\n", SPD_NVDIMM_ID_N + offset, SPDReg));
  if (SPDReg != 'N') return NOT_NVDIMM;

  ReadSpd(host, socket, ch, dimm, SPD_NVDIMM_ID_V + offset, &SPDReg);
  MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "NVDIMM:V(%d)=0x%x\n", SPD_NVDIMM_ID_V + offset, SPDReg));
  if (SPDReg != 'V') return NOT_NVDIMM;


  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) offset = MFGID_OFFSET;
  ReadSpd(host, socket, ch, dimm, SPD_MMID_MSB + offset, &SPDReg);
  SPDMMfgId = (UINT16)(SPDReg << 8);
  ReadSpd(host, socket, ch, dimm, SPD_MMID_LSB + offset, &SPDReg);
  SPDMMfgId |= (UINT16)(SPDReg &~BIT7);

  MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Found NVDIMM MfgId=0x%x, searching table\n", SPDMMfgId));

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) offset = PN_OFFSET;
  while (nv[nvIdx].nvVid != 0) {
    if (nv[nvIdx].nvVid == SPDMMfgId) {
      for (i = 0; i < nv[nvIdx].nvDidCnt; i++) {
        ReadSpd(host, socket, ch, dimm, SPD_MODULE_PN + i + offset, &SPDReg);
        if (nv[nvIdx].nvDid[i] != SPDReg) break;
      } // i loop

      if (i == nv[nvIdx].nvDidCnt) {
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "NVDIMM(MemSPD): Found a %s NVDIMM vendorIndex=%d\n", nv[nvIdx].nvName, nv[nvIdx].vendorIndex));
        return nvIdx;
      }
    }

    nvIdx++;
  } // while loop

  MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Could not find NVDIMM in NVDIMM List!!!!!\n"));
  return NOT_NVDIMM;
}


UINT32
CoreNVDIMMStatus(
PSYSHOST host
)
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT32              nvVenIdx;
  UINT32              status;
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
  struct channelNvram(*channelNvList)[MAX_CH];
  struct smbDevice    smbusdev;

  socket = host->var.mem.currentSocket;

  MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "CoreNVDIMMStatus\n"));

  // First Loop is to detect status for all NVDIMMs, and start the restore process
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      nvVenIdx = (*dimmNvList)[dimm].nvDimmType;
      if (nvVenIdx) {

        (*dimmNvList)[dimm].nvDimmStatus = 0;

        smbusdev.compId = SPD;
        smbusdev.address.controller = PROCESSOR;
        GetSmbAddress(host, socket, ch, dimm, &smbusdev);
        smbusdev.address.deviceType = nv[nvVenIdx].smbusAdr;

        // Check if the NVDIMM has data to restore
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Calling %s getStatus with vendorIndex %d\n", nv[nvVenIdx].nvName, nv[nvVenIdx].vendorIndex));
        status = nv[nvVenIdx].getStatus(host, socket, smbusdev, nv[nvVenIdx].vendorIndex);

        switch (status) {
        case NVDIMM_RESTORE:
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM HAS data to restore\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= STATUS_RESTORE_NEEDED;
          break;

        case NVDIMM_NODATA:
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM does NOT have data to restore\n"));
          break;

        case NVDIMM_ERROR:
        default:
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM in ERROR condition\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= ERROR_DETECT;
          break;
        }
      }
    } // dimm loop
  } // ch loop
  return 0;
}

UINT32
CoreRestoreNVDIMMs(
PSYSHOST host
)
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT32              status;
  UINT32              nvVenIdx;
  UINT32              nv_ckeMask = 0;
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
  struct channelNvram(*channelNvList)[MAX_CH];
  struct ddrRank(*rankList)[MAX_RANK_DIMM];
  struct smbDevice    smbusdev;


  if (!host->setup.mem.restoreNVDIMMS) return 0;

  socket = host->var.mem.currentSocket;
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "PlatformRestoreNVDIMMs\n"));

  // First Loop to start the restore on all NVDIMMs
  channelNvList = &host->nvram.mem.socket[socket].channelList;
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++)    {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      nvVenIdx = (*dimmNvList)[dimm].nvDimmType;
      nv_ckeMask = 0;
      if (!nvVenIdx) {
        rankList = &(*dimmNvList)[dimm].rankList;
        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          nv_ckeMask |= 1 << (*rankList)[rank].CKEIndex;
        } // rank loop
      }

      if ((nvVenIdx) && ((*dimmNvList)[dimm].nvDimmStatus & STATUS_RESTORE_NEEDED)) {

        smbusdev.compId = SPD;
        smbusdev.address.controller = PROCESSOR;
        GetSmbAddress(host, socket, ch, dimm, &smbusdev);
        smbusdev.address.deviceType = nv[nvVenIdx].smbusAdr;

        (*dimmNvList)[dimm].nvDimmStatus &= ~(STATUS_RESTORE_NEEDED);

        ChipDeassertCKE(host, socket, ch, (*channelNvList)[ch].ckeMask);

        host->var.mem.subBootMode = NvDimmResume;
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Calling %s startRestore with vendorIndex %d\n", nv[nvVenIdx].nvName, nv[nvVenIdx].vendorIndex));
        status = nv[nvVenIdx].startRestore(host, socket, smbusdev, nv[nvVenIdx].vendorIndex);

        if (status == FAILURE) {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    ERROR starting NVDIMM Recovery\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= ERROR_RESTORE;
        }
        else {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    Started NVDIMM Recovery\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= STATUS_RESTORE_INPROGRESS;
        }
      }
    } // dimm loop
  } // ch loop



  // Second Loop is to verify that all NVDIMMs have finished their Restore
  channelNvList = &host->nvram.mem.socket[socket].channelList;
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++)   {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      nvVenIdx = (*dimmNvList)[dimm].nvDimmType;

      if ((nvVenIdx) && ((*dimmNvList)[dimm].nvDimmStatus & STATUS_RESTORE_INPROGRESS)) {

        smbusdev.compId = SPD;
        smbusdev.address.controller = PROCESSOR;
        GetSmbAddress(host, socket, ch, dimm, &smbusdev);
        smbusdev.address.deviceType = nv[nvVenIdx].smbusAdr;

        MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Calling %s restoreDone with vendorIndex %d\n", nv[nvVenIdx].nvName, nv[nvVenIdx].vendorIndex));
        (*dimmNvList)[dimm].nvDimmStatus &= ~(STATUS_RESTORE_INPROGRESS);
        status = nv[nvVenIdx].restoreDone(host, socket, smbusdev, nv[nvVenIdx].vendorIndex);
        if (status == FAILURE) {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    ERROR during NVDIMM Recovery\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= ERROR_RESTORE;
        }
        else {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    Recovery Successful\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= STATUS_RESTORE_SUCCESSFUL;
        }
      }
    } // dimm loop
  } // ch loop
  return 0;
} // PlatformRestoreNVDIMMs

UINT32
CoreArmNVDIMMs(
PSYSHOST host
)
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT32              nvVenIdx; // index into nv[] for vender data
  UINT32              status;
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
  struct channelNvram(*channelNvList)[MAX_CH];
  struct smbDevice    smbusdev;

  if (!host->setup.mem.eraseArmNVDIMMS) return 0;
  socket = host->var.mem.currentSocket;
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "PlatformArmNVDIMMs\n"));

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      nvVenIdx = (*dimmNvList)[dimm].nvDimmType;
      if (nvVenIdx) {
        // We have an NVDIMM, setup the structures and then call routines

        smbusdev.compId = SPD;
        smbusdev.address.controller = PROCESSOR;
        GetSmbAddress(host, socket, ch, dimm, &smbusdev);
        smbusdev.address.deviceType = nv[nvVenIdx].smbusAdr;

        // Check if the NVDIMM has data to restore
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Calling %s arm with vendorIndex %d\n", nv[nvVenIdx].nvName, nv[nvVenIdx].vendorIndex));
        status = nv[nvVenIdx].arm(host, socket, smbusdev, nv[nvVenIdx].vendorIndex);
        if (status == FAILURE) {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    ERROR Arming NVDIMM\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= ERROR_ARM;
        }
        else {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM armed\n"));
          (*dimmNvList)[dimm].nvDimmStatus |= STATUS_ARMED;
        }
      }
    } // dimm loop
  } // ch loop
  return 0;
} // PlatformArmNVDIMMs
#endif  //MEM_NVDIMM_EN
