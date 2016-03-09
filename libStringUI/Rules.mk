# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Build Directories
ARM_BUILD_PATH_$(d)	:=	$(d)/build/arm
ARM_OBJ_PATH_$(d)	:=	$(ARM_BUILD_PATH_$(d))/obj
ARM_LIB_PATH_$(d)	:=	$(ARM_BUILD_PATH_$(d))/lib

X86_BUILD_PATH_$(d)	:=	$(d)/build/x86
X86_OBJ_PATH_$(d)	:=	$(X86_BUILD_PATH_$(d))/obj
X86_LIB_PATH_$(d)	:=	$(X86_BUILD_PATH_$(d))/lib

# Local variables
TARGET_$(d)	:= $(ARM_LIB_PATH_$(d))/libStringUI.a
TARGET_X86_$(d)	:= $(X86_LIB_PATH_$(d))/libStringUI.a

CSRC_$(d)	:= $(shell $(FIND) $(d) -name *.c)
CSRC_X86_$(d) := $(filter-out %/SystemIO.cpp, $(CSRC_$(d)))

OBJS_$(d) 	:= \
	$(patsubst %.c, %.o, $(CSRC_$(d):%=$(ARM_OBJ_PATH_$(d))/%))
X86_OBJS_$(d) 	:= \
	$(patsubst %.c, %.o, $(CSRC_X86_$(d):%=$(X86_OBJ_PATH_$(d))/%))

DEPS_$(d)	:= $(OBJS_$(d):%.o=%.d) $(X86_OBJS_$(d):%.o=%.d)
LST_$(d)	:= $(OBJS_$(d):%=%.lst) $(X86_OBJS_$(d):%=%.lst)
GCOV_$(d)	:= $(OBJS_$(d):%.o=%.gcda) $(OBJS_$(d):%.o=%.gcno) $(X86_OBJS_$(d):%.o=%.gcda) $(X86_OBJS_$(d):%.o=%.gcno)

# Append build targets
TGT_LIB		:= $(TGT_LIB) $(TARGET_$(d)) $(TARGET_X86_$(d))
CLEAN		:= $(CLEAN) CLEAN_$(d)


# Local rules
.PHONY: CLEAN_$(d)
CLEAN_$(d): d := $(d)
CLEAN_$(d):
		rm -f $(OBJS_$(d))
		rm -f $(X86_OBJS_$(d))
		rm -f $(DEPS_$(d))
		rm -f $(LST_$(d))
		rm -f $(TARGET_$(d))
		rm -f $(TARGET_X86_$(d))
		rm -f $(GCOV_$(d))


$(OBJS_$(d)):	CF_TGT := -I$(d)/include
$(X86_OBJS_$(d)): CF_TGT := -I$(d)/include

$(TARGET_$(d)):	$(OBJS_$(d))
		$(MAKE_DIRECTORY)
		$(AR) -r $@ $^

$(TARGET_X86_$(d)):	$(X86_OBJS_$(d))
		$(MAKE_DIRECTORY)
		$(AR_X86) -r $@ $^

$(ARM_OBJ_PATH_$(d))/%.o: %.c
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMP)

$(X86_OBJ_PATH_$(d))/%.o: %.c
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMP_X86)

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
