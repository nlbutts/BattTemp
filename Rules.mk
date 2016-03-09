
# Standard stuff

.SUFFIXES:
.SUFFIXES:	.c .o

.DEFAULT_GOAL := all
all:		targets

# Global variables used by the makefile fragments
MERCURIAL_TAG := $(shell hg --debug id -i)

ifeq ($(UNAME), Msys)
BUILD_DATE := $(shell c:/mingw/msys/1.0/bin/date -u +"%m%d%y-%H:%M")
else
BUILD_DATE := $(shell date -u +"%m%d%y-%H:%M")
endif


ifdef NO_DEBUG_LOGGING
DEBUG_LOGGING =
else
DEBUG_LOGGING = -DDEBUG_LOGGING
endif


# Subdirectories, in random order
dir		:= cube
include		$(dir)/Rules.mk

dir		:= libStringUI
include		$(dir)/Rules.mk

dir		:= ut
include		$(dir)/Rules.mk

# The variables TGT_*, CLEAN and CMD_INST* may be added to by the Makefile
# fragments in the various subdirectories.
.PHONY: 	targets
targets:	$(TGT_BIN) $(TGT_SBIN) $(TGT_ETC) $(TGT_LIB)

.PHONY:		clean
clean:		$(CLEAN)

.PHONY: 	clean-all
clean-all:	clean stm32libs-clean

# Prevent make from removing any build targets, including intermediate ones
.SECONDARY: $(CLEAN)

.PHONY: HELP_TGT
HELP_TGT:
	@echo "==== Targets ===="
	@echo "all			: Build all target artifacts"
	@echo "clean			: Clean the project folder"
	@echo "clean-all		: Clean the project folder and all external projects"
	@echo "ut 			: Make just the unit tests"
	@echo "run_ut			: Run the unit tests"
	@echo "coverage		: Generate the XML output for the Jenkins coverage"
	@echo "lcov			: Generate an HTML coverage report for local development"

.PHONY: HELP_SEPARATOR
HELP_SEPARATOR:
	@echo ""
	@echo "==== Build Variables ===="
	@echo "The following variables may be passed to make to parameterize the build:"

.PHONY: help
help: HELP_TGT $(HELP_TARGETS) HELP_SEPARATOR $(HELP_VARS)
	@echo "FW_VERSION 		: Application / Bootblock firmware version"
	@echo "APP_FW_PARTNUMBER 	: Application firmware part number"
	@echo "BB_FW_PARTNUMBER 	: Bootblock firmware part number"
	@echo "ASSEMBLY_PARTNUMBER 	: Firmware assembly part number"
	@echo "HSE_CRYSTAL_HZ 		: HSE oscillator crystal frequency in Hz. Defaults to $(HSE_CRYSTAL_HZ)"
	@echo "NO_DEBUG_LOGGING 	: Defining and setting this equal to any value will disable debug logging"

