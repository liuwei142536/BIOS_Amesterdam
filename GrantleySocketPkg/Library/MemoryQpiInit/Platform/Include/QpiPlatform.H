//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*************************************************************************
 *
 * QPI Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains platform defintions used by the MINIBIOS to
 *      select the proper TX equalization values 
 *
 ************************************************************************/

#ifndef  _qpi_platform_h
#define  _qpi_platform_h

#include "PlatformHost.h"

// These two are mutually exclusive
#define QPI_HW_PLATFORM    1
//#define QPI_SW_SIMULATION    1


// HSX s4906062 / Grantley BIOS s4987499
// <WBG ClkSkew>IERR while booting CentOS or running PTU : R2 ingress IDI or NCS credit overflow
// Activate this defined if issue is encountered.
#define BGF_PTR_SEP_IIO_WA


//
// choose minibios platform
//
#define QPI_PLATFORM_TYPE GrantelyDVP

#endif   // _qpi_platform_h
