#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

#**********************************************************************
## @file
# Makes sure required version of the build tools is available.
#**********************************************************************
ifeq ($(call __ge, $(BUILD_TOOLS_VERSION), 35),no)
$(error This version of MdePkg requires build tools 35 or newer (you are using tools $(BUILD_TOOLS_VERSION)))
endif
