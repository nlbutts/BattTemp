# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

# Build directories
X86_BUILD_PATH_$(d)	:=	$(d)/build/x86
X86_OBJ_PATH_$(d)	:=	$(X86_BUILD_PATH_$(d))/obj
X86_LIB_PATH_$(d) 	:= 	$(X86_BUILD_PATH_$(d))/lib
X86_BIN_PATH_$(d) 	:= 	$(X86_BUILD_PATH_$(d))/bin

ifeq ($(UNAME), Msys)
X86_TGT_NAME_$(d) 	:= 	$(X86_BIN_PATH_$(d))/ut.exe
else
X86_TGT_NAME_$(d) 	:=	$(X86_BIN_PATH_$(d))/ut
endif

# Gmock and GTest
GMOCK_DIR := $(d)/gmock-1.7.0
GTEST_DIR := $(GMOCK_DIR)/gtest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h

GMOCK_HEADERS := $(GMOCK_DIR)/include/gmock/*.h \
                 $(GMOCK_DIR)/include/gmock/internal/*.h \
                 $(GTEST_HEADERS)

GTEST_SRCS_ := $(shell $(FIND) $(GTEST_DIR)/src -name *.cc ! -name gtest-all.cc ! -name gtest_main.cc)
GMOCK_SRCS_ := $(shell $(FIND) $(GMOCK_DIR)/src -name *.cc ! -name gmock-all.cc)

GTEST_OBJS 	:= $(patsubst %.cc, %.o, $(GTEST_SRCS_:%=$(X86_OBJ_PATH_$(d))/%))
GMOCK_OBJS 	:= $(patsubst %.cc, %.o, $(GMOCK_SRCS_:%=$(X86_OBJ_PATH_$(d))/%))

# Local targets
TGTS_$(d)	:= $(X86_TGT_NAME_$(d))
CXXSRC_$(d) := $(shell $(FIND) $(d)/src -name "*.cpp")
OBJS_$(d)	:= \
	$(patsubst %.cpp, %.o, $(CXXSRC_$(d):%=$(X86_OBJ_PATH_$(d))/%))
DEPS_$(d)	:= $(OBJS_$(d):%.o=%.d) $(GTEST_OBJS:%.o=%.d) $(GMOCK_OBJS:%.o=%.d)
LST_$(d)	:= $(OBJS_$(d):%=%.lst) $(GTEST_OBJS:%=%.lst) $(GMOCK_OBJS:%=%.lst)
GCOV_$(d)	:= $(OBJS_$(d):%.o=%.gcda) $(OBJS_$(d):%.o=%.gcno)

# Append build targets
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
		rm -f $(GTEST_OBJS) $(GMOCK_OBJS)
		rm -f $(X86_LIB_PATH_$(d))/libgmock_main.a
		rm -f $(X86_LIB_PATH_$(d))/libgtest.a
		rm -f $(GCOV_$(d))
		rm -rf $(X86_BUILD_PATH_$(d))

$(X86_LIB_PATH_$(d))/libgtest.a: $(GTEST_OBJS)
		$(MAKE_DIRECTORY)
		$(AR_X86) -r $@ $^

$(X86_LIB_PATH_$(d))/libgmock_main.a: $(GMOCK_OBJS)
		$(MAKE_DIRECTORY)
		$(AR_X86) -r $@ $^

$(X86_OBJ_PATH_$(d))/%.o: CF_TGT := -I$(d)/include -IlibStringUI/include \
									-isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include \
									-I$(GTEST_DIR) -I$(GMOCK_DIR)
$(X86_OBJ_PATH_$(d))/%.o: %.cpp
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMPXX_X86)

$(X86_OBJ_PATH_$(d))/%.o: %.cc
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMPXX_X86)

$(X86_OBJ_PATH_$(d))/%.o: %.c
		$(MAKE_DIRECTORY)
		@echo CXX $<
		$(COMP_X86)

$(X86_TGT_NAME_$(d)):	LL_X86 := -Xlinker -lpthread
$(X86_TGT_NAME_$(d)): 	LF_TGT := $(LIBSTM32_X86_LF) \
									-LlibStringUI/build/x86/lib \
									-L$(X86_LIB_PATH_$(d))

$(X86_TGT_NAME_$(d)):	LL_TGT := $(LIBSTM32_X86_LL) -lgtest -lgmock_main -lStringUI
$(X86_TGT_NAME_$(d)):	$(OBJS_$(d)) $(LIBSTM32_X86_OBJS) \
									 libStringUI/build/x86/lib/libStringUI.a \
									 $(X86_LIB_PATH_$(d))/libgmock_main.a \
									 $(X86_LIB_PATH_$(d))/libgtest.a
		$(MAKE_DIRECTORY)
		@echo Linking x86
		$(COMPLINK_X86)

.PHONY: 	coverage
coverage:	#$(X86_TGT_COVERAGE_ut)
	python gcovr.py -r .. --xml -e "(projects/ut/*)|(Main.cpp)" > ../unit_test_coverage.xml

.PHONY: 	lcov
lcov: $(X86_TGT_NAME_$(d))
	$<
	mkdir -p ut_coverage
	lcov -q --base-directory . --directory . --capture --output-file ut_coverage/unit_test_coverage.info
	genhtml -q --output-directory ut_coverage ut_coverage/unit_test_coverage.info

.PHONY: 	ut
ut: $(X86_TGT_NAME_$(d))

.PHONY: 	run_ut
run_ut: $(X86_TGT_NAME_$(d))
	$<

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))