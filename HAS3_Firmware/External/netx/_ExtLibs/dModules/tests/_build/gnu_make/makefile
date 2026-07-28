#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = your

#--- Inputs ----#
PROJECT_HOME_DIR = .

ifeq "$(CPPUTEST_HOME)" ""
$(error The environment variable CPPUTEST_HOME is not set. \
Set it to where cpputest is installed)
endif

# --- SRC_FILES and SRC_DIRS ---
# Production code files are compiled and put into
# a library to link with the test runner.
#
# Test code of the same name overrides
# production code at link time.
#
# SRC_FILES specifies individual production
# code files.
#
# SRC_DIRS specifies directories containing
# production code C and CPP files.
#
# SRC_FILES += add source files
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/circularBuffer_byte.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/crc8bit.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/ddio.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/d_uart.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/g0comm.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/netx90_ddio.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/netx90_duart.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/stm32g0_ddio.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/stm32g0_duart.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/stm32g4_ddio.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/stm32g4_duart.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/cli/cli.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/cli/CliCommandHandlers/cliG0.c
SRC_FILES += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/util/monitorBootupStatus.c
SRC_DIRS += src

# --- TEST_SRC_FILES and TEST_SRC_DIRS ---
# Test files are always included in the build.
# Production code is pulled into the build unless
# it is overriden by code of the same name in the
# test code.
#
# TEST_SRC_FILES specifies individual test files to build.
# TEST_SRC_DIRS, builds everything in the directory

# TEST_SRC_DIRS += tests/srctest
TEST_SRC_DIRS += tests/helpers
TEST_SRC_DIRS += tests/srctest/

#	tests/example-fff \
#	tests/fff \

# --- MOCKS_SRC_DIRS ---
# MOCKS_SRC_DIRS specifies a directories where you can put your
# mocks, stubs and fakes.  You can also just put them
# in TEST_SRC_DIRS
MOCKS_SRC_DIRS +=  tests/mocks

# Turn on CppUMock
CPPUTEST_USE_EXTENSIONS = Y

# INCLUDE_DIRS are searched in order after the included file's
# containing directory
INCLUDE_DIRS += $(CPPUTEST_HOME)/include
INCLUDE_DIRS += $(CPPUTEST_HOME)/include/Platforms/Gcc
INCLUDE_DIRS += inc
INCLUDE_DIRS += tests/helpers/inc
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/G0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/G4BB/Drivers/CMSIS/Device/ST/STM32G4xx/Include
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules/inc
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G0Modules
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/NetX90/Components/CMSIS/Device/Hilscher/netx/Include
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G4DC/Drivers/CMSIS/Include
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/NetX90/Components/dematic/Include
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/cli
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/cli/CliCommandHandlers
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/logging
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/G4DC/Core/Inc
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/io
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/DataStore
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/G4_int
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/util
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/shared
INCLUDE_DIRS += /builds/dematic/pd-scs/xmutilshared
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/shared/dcanParams
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/shared/dematic-utilities
INCLUDE_DIRS += /builds/dematic/pd-scs/xmc/XMC/Daughtercard/Shared/RTOS
# --- CPPUTEST_OBJS_DIR ---
# CPPUTEST_OBJS_DIR lets you control where the
# build artifact (.o and .d) files are stored.
#
# If you have to use "../" to get to your source path
# the makefile will put the .o and .d files in surprising
# places.
#
# To make up for each level of "../"in the source path,
# add place holder subdirectories to CPPUTEST_OBJS_DIR
# each.
# e.g. if you have "../../src", set to "test-objs/1/2"
#
# This is kind of a kludge, but it causes the
# .o and .d files to be put under objs.
CPPUTEST_OBJS_DIR = test-obj

CPPUTEST_LIB_DIR = test-lib

# You may have to tweak these compiler flags
#    CPPUTEST_WARNINGFLAGS - apply to C and C++
#    CPPUTEST_CFLAGS - apply to C files only
#    CPPUTEST_CXXFLAGS - apply to C++ files only
#    CPPUTEST_CPPFLAGS - apply to C and C++ Pre-Processor
#
# If you get an error like this
#     TestPlugin.h:93:59: error: 'override' keyword is incompatible
#        with C++98 [-Werror,-Wc++98-compat] ...
# The compiler is basically telling you how to fix the
# build problem.  You would add this flag setting
#     CPPUTEST_CXXFLAGS += -Wno-c++14-compat

CPPUTEST_CFLAGS += -D__TDD_CPPUTEST__ -DSTM32G071xx -DSTM32G474xx -DCI_GIT_COMMIT_HASH_U32=0xAAAAAAAA -D_DEBUG -D_CONSOLE -DBUILD_CLI

CPPUTEST_CPPFLAGS += -D__TDD_CPPUTEST__ -DSTM32G071xx -DSTM32G474xx -DCI_GIT_COMMIT_HASH_U32=0xAAAAAAAA -DBUILD_CLI

# Some flags to quiet clang
ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang"), 1)
CPPUTEST_WARNINGFLAGS += -Wno-unknown-warning-option
CPPUTEST_WARNINGFLAGS += -Wno-covered-switch-default
CPPUTEST_WARNINGFLAGS += -Wno-reserved-id-macro
CPPUTEST_WARNINGFLAGS += -Wno-keyword-macro
CPPUTEST_WARNINGFLAGS += -Wno-documentation
CPPUTEST_WARNINGFLAGS += -Wno-missing-noreturn
endif

CPPUTEST_WARNINGFLAGS += -Wall
#CPPUTEST_WARNINGFLAGS += -Werror
CPPUTEST_WARNINGFLAGS += -Wfatal-errors
CPPUTEST_WARNINGFLAGS += -Wswitch-default
CPPUTEST_WARNINGFLAGS += -Wno-format-nonliteral
CPPUTEST_WARNINGFLAGS += -Wno-sign-conversion
CPPUTEST_WARNINGFLAGS += -Wno-pedantic
CPPUTEST_WARNINGFLAGS += -Wno-shadow
CPPUTEST_WARNINGFLAGS += -Wno-missing-field-initializers
CPPUTEST_WARNINGFLAGS += -Wno-unused-parameter
CPPUTEST_WARNINGFLAGS += -Wno-int-to-pointer-cast
CPPUTEST_WARNINGFLAGS += -Wno-switch-default
CPPUTEST_WARNINGFLAGS += -Wno-unused-function
CPPUTEST_WARNINGFLAGS += -Wno-unknown-pragmas


CPPUTEST_CFLAGS += -pedantic
CPPUTEST_CFLAGS += -Wno-missing-prototypes
CPPUTEST_CFLAGS += -Wno-strict-prototypes
CPPUTEST_CXXFLAGS += -Wno-c++14-compat
CPPUTEST_CXXFLAGS += --std=c++11
CPPUTEST_CXXFLAGS += -Wno-c++98-compat-pedantic
CPPUTEST_CXXFLAGS += -Wno-c++98-compat

# Coloroze output
CPPUTEST_EXE_FLAGS += -c

# --- LD_LIBRARIES -- Additional needed libraries can be added here.
# commented out example specifies math library
#LD_LIBRARIES += -lm

# Look at $(CPPUTEST_HOME)/build/MakefileWorker.mk for more controls
include $(CPPUTEST_HOME)/build/MakefileWorker.mk