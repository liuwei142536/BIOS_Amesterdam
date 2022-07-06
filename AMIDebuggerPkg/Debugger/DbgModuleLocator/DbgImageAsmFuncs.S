#------------------------------------------------------------------------------
#
# Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php.
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
# Module Name:
#
#   AsmFuncs.S
#
# Abstract:
#
#   Debug interrupt handle functions.
#
#------------------------------------------------------------------------------

#include "token.h"

ASM_GLOBAL ASM_PFX(DbgModDriverSpecialBreakpoint);

.text

ASM_PFX(DbgModDriverSpecialBreakpoint):
	push %rax
	push %rdx
	mov 0x494D4124, %rax
	mov 0x4442414E, %rdx
	int 3
	pop %rdx
	pop %rax
	ret



