#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************

#**********************************************************************
## @file
# Used by makefile and Main.mak. Contains utility
#  definitions, autodetects OS, builds and exports path environment variables,
#  and some common character definitions (such as COMMA, SPACE, and EOL)
#**********************************************************************

# Detect OS, based on Windows WINDIR env variable
# 0 - Windows OS
# 1 - Linux OS
BUILD_OS_WINDOWS := 0
BUILD_OS_LINUX := 1

ifndef BUILD_OS
ifeq ($(OS),Windows_NT)
  # Windows OS
  BUILD_OS := $(BUILD_OS_WINDOWS)
else
  # Linux OS
  BUILD_OS := $(BUILD_OS_LINUX)
endif	#ifdef (windir)
export BUILD_OS
endif	#ifndef BUILD_OS

# Check paths 
# Make sure TOOLS_DIR is defined, otherwise error out!
ifdef TOOLS_DIR
  TOOLS_DIR := $(subst \,/,$(TOOLS_DIR))
else
  $(error TOOLS_DIR is not defined. Set TOOLS_DIR to an absolute path of the build tools directory.)
endif #ifdef (TOOLS_DIR)

ifndef BUILD_DIR
  BUILD_DIR = Build
export BUILD_DIR
endif #ifndef BUILD_DIR
export BUILD_ROOT=$(BUILD_DIR)

# If VEB isn't defined by VeB (or shell), try to guess which one
ifndef VEB
  __TMP_VEB_SUFFIX__:= $(suffix $(wildcard *.veb))
  ifneq ($(__TMP_VEB_SUFFIX__), .veb)
    ifndef __TMP_VEB_SUFFIX__
      $(error No VeB file found! Set VEB to a base name(no extention) of the project .veb file)
    endif
    $(error Multiple project VeB files found! Set VEB to a base name(no extention) of the project .veb file)
  endif
  VEB := $(basename $(wildcard *.veb) )
  $(warning VEB is not defined, using $(VEB).veb)
  export VEB
endif #ifndef VEB

# EDK2 variables
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
  EFI_TOOLS_DIR := $(TOOLS_DIR)/Bin/Win32
else
  ifeq ("$(shell getconf LONG_BIT)","64")
    EFI_TOOLS_DIR = $(TOOLS_DIR)/Bin/Linux64
  else
    EFI_TOOLS_DIR = $(TOOLS_DIR)/Bin/Linux32
  endif
endif	#ifeq($(BUILD_OS),$(BUILD_OS_WINDOWS))
ifndef EDK_TOOLS_PATH
  WORKSPACE =$(CURDIR)
  EDK_TOOLS_PATH=$(TOOLS_DIR)
  BASE_TOOLS_PATH=$(TOOLS_DIR)
  export WORKSPACE EDK_TOOLS_PATH BASE_TOOLS_PATH
  # Set the paths correctly, based on OS
  ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
    PATH := $(TOOLS_DIR);$(EFI_TOOLS_DIR);$(PATH)
  else
    PATH := $(TOOLS_DIR):$(EFI_TOOLS_DIR):$(PATH)
  endif	#ifeq($(BUILD_OS),$(BUILD_OS_WINDOWS))
endif

# Useful build utilities
JAVA = java
MAKE = make
EDII_BUILD = $(EFI_TOOLS_DIR)/build

# Define some general utilities
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
  SHELL:=cmd.exe
  UNIX_UTILS_DIR := $(TOOLS_DIR)/
  RM = del /F /Q
  RMDIR = cmd /C rd /S /Q
  CP = copy
# Can't use plain "PATH_SLASH=\" here because Make treats 
# back slash followed by the new line as a line continuation character.
  PATH_SLASH := $(subst /,\,/)
  PATH_SLASH_ESC = \\
  DOUBLEDOLLAR = $$
  FWBUILD = FWBuild
  FWBUILD_OPTION_CHAR = /
  MKDIR = mkdir
  __gt = $(shell if $1 GTR $2 (echo yes) else (echo no) )
  __ge = $(shell if $1 GEQ $2 (echo yes) else (echo no) )
  # $(call _os_path, <file-path>) - Replaces / woth \ to convert <file-path> to native OS format
  _os_path=$(subst /,$(PATH_SLASH),$1)
else
  # Set default shell
  SHELL:=/bin/bash
  # This only needs to be defined on non-Windows OS. NUMBER_OF_PROCESSORS is a standard system variable on Windows.
  NUMBER_OF_PROCESSORS = $(shell cat /proc/cpuinfo | grep processor | wc -l)
  UNIX_UTILS_DIR = 
  RM = rm -fr
  RMDIR = rm -fr
  CP = cp -f
  PATH_SLASH = /
  PATH_SLASH_ESC = /
  DOUBLEDOLLAR = \$$
  FWBUILD_OPTION_CHAR = -
  MKDIR = mkdir -p
  __gt = $(shell if [ $(1) -gt $(2) ] ; then echo yes ; else echo no ; fi)
  __ge = $(shell if [ $(1) -ge $(2) ] ; then echo yes ; else echo no ; fi)
   # $(call _os_path, <file-path>) - Replaces \ with / to convert <file-path> to native OS format  
  _os_path=$(subst $(subst /,\,/),$(PATH_SLASH),$1)
ifeq ($(call __ge, $(BUILD_TOOLS_VERSION), 27),yes)
  FWBUILD = FWBuild
endif
endif #ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
# $(SPACE) at the end of the ECHO definitions is required to supports commands like "$(ECHO)abc"
# with no space between $(ECHO) and the argument
ECHO = @$(UNIX_UTILS_DIR)echo -e$(SPACE)
ECHO_NO_ESC = @$(UNIX_UTILS_DIR)echo -E$(SPACE)
DATE = $(UNIX_UTILS_DIR)date
CAT = $(UNIX_UTILS_DIR)cat
GAWK = $(UNIX_UTILS_DIR)gawk

# Useful definitions
EOL = \n
COMMA := ,
SPACE :=
SPACE +=
TAB := \t

################
# Definitions of make functions that are intended for use in conditional expressions.
# Here is example of conditional expression with a function invokation:
#   ifeq ($(call _ge, $(BUILD_TOOLS_VERSION), 28),no)
#     $(error This version of AmiPkg requires build tools 28 or newer)
#   endif
################
# $(call _ge, <int1>, <int2>) - Expands to 'yes' if <int1> is greater then <int2> and to 'no' otherwise.
_gt=$(__gt)
# $(call _ge, <int1>, <int2>) - Expands to 'yes' if <int1> is greater or equal then <int2> and to 'no' otherwise.
_ge=$(__ge)
# $(call _exist, <file-path>) - Expands to 'yes' is file exists and to 'no' otherwise. 
_exist=$(if $(wildcard $(call _os_path,$1)),yes,no)

################
# Definitions of file opeartion make functions that are intended for use in command blocks (a.k.a. target receipies).
# See also _os_path definition above.
################
# $(call _copy,<file1>,<file2>) - Copies <file1> to file or directory defined by <file2>
_copy=$(CP) $(call _os_path,$1) $(call _os_path,$2)
# $(call _delete,<file>) - Deletes <file> if it exists, otherwise does nothing
_delete=$(if $(wildcard $(call _os_path,$1)),$(call _delete_or_fail,$1),)
# $(call _delete_or_fail,<file>) - Tries to delete <file>.  Breaks build process if <file> does not exists.
_delete_or_fail=$(RM) $(call _os_path,$1)

# Export PATH variable
export $(PATH)

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************