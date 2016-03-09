### Build flags for all targets
#
INCLUDES_ALL 	= $(LIBSTM32_INCLUDES)

### Frontier build variables
HSE_CRYSTAL_HZ  = 25000000

### Compiler flags
#
CF_ALL 			= -g3 -Wall $(INCLUDES_ALL) -Wa,-adhlns=$@.lst -fmessage-length=0 -fdata-sections -std=gnu11
CF_ARM 			= -O0 -ffunction-sections -DSTM32L052xx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=$(HSE_CRYSTAL_HZ) -mcpu=cortex-m0plus -mthumb -gdwarf-2
CF_X86 			= -O0 -pg -fprofile-arcs -ftest-coverage

CPPF_ALL 		= -g3 -Wall $(INCLUDES_ALL) -Wa,-adhlns=$@.lst -fmessage-length=0 -fdata-sections -fno-rtti $(DEBUG_LOGGING)
CPPF_ARM 		= -O0 -ffunction-sections -DSTM32L052xx -fno-exceptions -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=$(HSE_CRYSTAL_HZ) -mcpu=cortex-m0plus -mthumb -gdwarf-2
CPPF_X86 		= -O0 -pg -fprofile-arcs -ftest-coverage

LF_ALL 			= -Xlinker --gc-sections -Wl,--gc-sections -fno-rtti -fno-exceptions
LF_ARM 			= -mcpu=cortex-m0plus -mthumb
LF_X86 			=

LL_ALL          =
LL_ARM 			=
LL_X86 			=

### Target build tools
#
TOOL_BASE		= arm-none-eabi-
CC              = $(TOOL_BASE)gcc
CXX				= $(TOOL_BASE)g++
AS				= $(TOOL_BASE)as
COMP            = $(CC) $(CF_ALL) $(CF_ARM) $(CF_TGT) $(CMD_TGT) -MMD -MP -o $@ -c $<
COMPXX          = $(CXX) $(CPPF_ALL) $(CPPF_ARM) $(CF_TGT) $(CMD_TGT) -MMD -MP -o $@ -c $<
ASM				= $(AS) -g -o $@ $<
LINK            = $(CXX) $(LF_ALL) $(LF_ARM) $(LF_TGT) -o $@ $(filter-out %.a, $^) $(LL_TGT) $(LL_ALL) $(LL_ARM)
COMPLINK        = $(CXX) $(CPPF_ALL) $(CPPF_ARM) $(CF_TGT) $(LF_ALL) $(LF_ARM) $(LF_TGT) -o $@ $(filter-out %.a, $^) $(LL_TGT) $(LL_ALL) $(LL_ARM)
AR				= $(TOOL_BASE)ar
OC				= $(TOOL_BASE)objcopy
OD				= $(TOOL_BASE)objdump
SIZE			= $(TOOL_BASE)size
MAKE_DIRECTORY	= @mkdir -p $(dir $@)

### Native build tools
#
CC_X86 			= gcc
CXX_X86 		= g++
AR_X86 			= ar
COMP_X86 		= $(CC_X86) $(CF_X86) $(CF_TGT) $(CMD_TGT) -MMD -MP -o $@ -c $<
COMPXX_X86 		= $(CXX_X86) $(CPPF_ALL) $(CPPF_X86) $(CF_TGT) $(CMD_TGT) -MMD -MP -o $@ -c $<
LINK_X86 		= $(CXX_X86) $(LF_X86) $(LF_TGT) -o $@ $(filter-out %.a, $^) $(LL_TGT) $(LL_ALL) $(LL_X86)
COMPLINK_X86 	= $(CXX_X86) $(CPPF_ALL) $(CPPF_X86) $(CF_TGT) $(LF_X86) $(LF_TGT) -o $@ $(filter-out %.a, $^) $(LL_TGT) $(LL_X86)
UNAME 			= $(shell uname -o)

ifeq ($(UNAME), Msys)
FIND 			= c:/mingw/msys/1.0/bin/find
SREC 			= ./srec_cat
else
FIND 			= find
SREC 			= srec_cat
endif


### Standard parts
#
include Rules.mk
