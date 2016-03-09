# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

# Build directories
ARM_BUILD_PATH_$(d)	:=	$(d)/build/arm
ARM_OBJ_PATH_$(d)	:=	$(ARM_BUILD_PATH_$(d))/obj
ARM_BIN_PATH_$(d) 	:= 	$(ARM_BUILD_PATH_$(d))/bin

# Local targets
TGTS_$(d)	:= \
	$(ARM_BIN_PATH_$(d))/application.elf \
	$(ARM_BIN_PATH_$(d))/application.hex \
	$(ARM_BIN_PATH_$(d))/application.lst \
	$(ARM_BIN_PATH_$(d))/application_debug.elf \
	$(ARM_BIN_PATH_$(d))/application_debug.hex \
	$(ARM_BIN_PATH_$(d))/application_debug.lst

CXXSRC_$(d)	:= $(shell $(FIND) $(d) -name *.cpp)
CSRC_$(d)	:= $(shell $(FIND) $(d) -name *.c)
ASRC_$(d)	:= $(shell $(FIND) $(d) -name *.S)
OBJS_$(d)	:= \
	$(patsubst %.cpp, %.o, $(CXXSRC_$(d):%=$(ARM_OBJ_PATH_$(d))/%)) \
	$(patsubst %.c, %.o, $(CSRC_$(d):%=$(ARM_OBJ_PATH_$(d))/%)) \
	$(patsubst %.S, %.o, $(ASRC_$(d):%=$(ARM_OBJ_PATH_$(d))/%))

DEPS_$(d)	:= $(OBJS_$(d):%.o=%.d)
LST_$(d)	:= $(OBJS_$(d):%=%.lst)

# Append global targets
TGT_BIN		:= $(TGT_BIN) $(TGTS_$(d))
CLEAN		:= $(CLEAN) CLEAN_$(d)

# Local rules
.PHONY: CLEAN_$(d)
CLEAN_$(d): d := $(d)
CLEAN_$(d):
		rm -f $(OBJS_$(d))
		rm -f $(TGTS_$(d))
		rm -f $(DEPS_$(d))
		rm -f $(LST_$(d))
		rm -f $(ARM_BIN_PATH_$(d))/application.map
		rm -f $(ARM_BIN_PATH_$(d))/application_debug.map

$(ARM_OBJ_PATH_$(d))/%.o: %.c
		$(MAKE_DIRECTORY)
		@echo CC $<
		$(COMP)

$(ARM_OBJ_PATH_$(d))/%/application/FirmwareVersion.o: CF_TGT := -I$(d)/include -I$(d)/Drivers/STM32L0xx_HAL_Driver_inc -I$(d)/CMSIS/Include \
	-DFW_VERSION='"$(FW_VERSION)"' \
	-DFW_PARTNUMBER='"$(APP_FW_PARTNUMBER)"' \
	-DFW_HGID='"$(MERCURIAL_TAG)"' \
	-DFW_DATE='"$(BUILD_DATE)"'

$(ARM_OBJ_PATH_$(d))/%.o: %.cpp
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMPXX)

$(ARM_OBJ_PATH_$(d))/%.o: %.S
		$(MAKE_DIRECTORY)
		@echo AS $<
		@$(ASM)

$(ARM_BIN_PATH_$(d))/application.elf:	CF_TGT := -I$(d)/include -I$(d)/Drivers/STM32L0xx_HAL_Driver/Inc -I$(d)/CMSIS/Include -I$(d)/Drivers/CMSIS/Device/ST/STM32L0xx/Include -I$(d)/Drivers/CMSIS/Include -I$(d)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I$(d)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -IlibStringUI/include
$(ARM_BIN_PATH_$(d))/application.elf:	LF_TGT := -T $(d)/linker/stm32_flash.ld -Llinkerscripts -Wl,-Map,$(ARM_BIN_PATH_$(d))/application.map -LlibStringUI/build/arm/lib
$(ARM_BIN_PATH_$(d))/application.elf:	LL_TGT := -lStringUI
$(ARM_BIN_PATH_$(d))/application.elf:	$(OBJS_$(d)) libStringUI/build/arm/lib/libStringUI.a
		$(MAKE_DIRECTORY)
		@echo "Linking $@"
		$(LINK)
		$(SIZE) --format=berkeley $@

$(ARM_BIN_PATH_$(d))/application_debug.elf:	CF_TGT := -I$(d)/include -I$(d)/Drivers/STM32L0xx_HAL_Driver/Inc -I$(d)/CMSIS/Include -I$(d)/Drivers/CMSIS/Device/ST/STM32L0xx/Include -I$(d)/Drivers/CMSIS/Include -I$(d)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I$(d)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
$(ARM_BIN_PATH_$(d))/application_debug.elf:	LF_TGT := -T $(d)/linker/stm32_flash_debug.ld -Llinkerscripts -Wl,-Map,$(ARM_BIN_PATH_$(d))/application_debug.map -LlibStringUI/build/arm/lib
$(ARM_BIN_PATH_$(d))/application_debug.elf:	LL_TGT := -lStringUI
$(ARM_BIN_PATH_$(d))/application_debug.elf:	$(OBJS_$(d)) libStringUI/build/arm/lib/libStringUI.a
		$(MAKE_DIRECTORY)
		@echo "Linking $@"
		$(LINK)
		$(SIZE) --format=berkeley $@

$(ARM_BIN_PATH_$(d))/application.lst: $(ARM_BIN_PATH_$(d))/application.elf
		$(MAKE_DIRECTORY)
		$(OD) -h -S $^ > $@

$(ARM_BIN_PATH_$(d))/application_debug.lst: $(ARM_BIN_PATH_$(d))/application_debug.elf
		$(MAKE_DIRECTORY)
		$(OD) -h -S $^ > $@

$(ARM_BIN_PATH_$(d))/application.hex: $(ARM_BIN_PATH_$(d))/application.elf $(ARM_BIN_PATH_$(d))/application.lst
		$(MAKE_DIRECTORY)
		$(OC) -O ihex $< $@

$(ARM_BIN_PATH_$(d))/application_debug.hex: $(ARM_BIN_PATH_$(d))/application_debug.elf $(ARM_BIN_PATH_$(d))/application_debug.lst
		$(MAKE_DIRECTORY)
		$(OC) -O ihex $< $@

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
