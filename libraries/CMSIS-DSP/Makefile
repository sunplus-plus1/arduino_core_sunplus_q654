TOP = ../..

CHIP     ?= Q645

include $(TOP)/make.cfg

# Compilation flags
CFLAGS += \
 -Wsign-compare \
 -Wdouble-promotion \
 -Ofast -ffast-math \
 -DNDEBUG \
 -Wall -Wextra  -Werror \
 -fshort-enums \
 -mfloat-abi=hard

ifneq ($(CHIP),Q628)
CFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_CM4/Include
else
CFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_ARMV5/Include
endif

# Path to CMSIS_DSP
CMSIS_DSP := .

# Sources
SRCS := $(CMSIS_DSP)/Source/BasicMathFunctions/BasicMathFunctions.c \
 $(CMSIS_DSP)/Source/CommonTables/CommonTables.c \
 $(CMSIS_DSP)/Source/InterpolationFunctions/InterpolationFunctions.c \
 $(CMSIS_DSP)/Source/BayesFunctions/BayesFunctions.c \
 $(CMSIS_DSP)/Source/MatrixFunctions/MatrixFunctions.c \
 $(CMSIS_DSP)/Source/ComplexMathFunctions/ComplexMathFunctions.c \
 $(CMSIS_DSP)/Source/QuaternionMathFunctions/QuaternionMathFunctions.c \
 $(CMSIS_DSP)/Source/ControllerFunctions/ControllerFunctions.c \
 $(CMSIS_DSP)/Source/SVMFunctions/SVMFunctions.c \
 $(CMSIS_DSP)/Source/DistanceFunctions/DistanceFunctions.c \
 $(CMSIS_DSP)/Source/StatisticsFunctions/StatisticsFunctions.c \
 $(CMSIS_DSP)/Source/FastMathFunctions/FastMathFunctions.c \
 $(CMSIS_DSP)/Source/SupportFunctions/SupportFunctions.c \
 $(CMSIS_DSP)/Source/FilteringFunctions/FilteringFunctions.c \
 $(CMSIS_DSP)/Source/TransformFunctions/TransformFunctions.c

# Includes
DSP_INCLUDES := $(CMSIS_DSP)/Include \
  $(CMSIS_DSP)/PrivateInclude

# If Neon and Cortex-A
#DSP_INCLUDES += $(CMSIS_DSP)/ComputeLibrary/Include
#SRCS += $(CMSIS_DSP)/ComputeLibrary/Source/arm_cl_tables.c

# Compilation flags for include folders
INC_FLAGS := $(addprefix -I,$(DSP_INCLUDES))
CFLAGS += $(INC_FLAGS)

# Output folder for build products
BUILDDIR := ./lib

OBJECTS := $(SRCS:%=$(BUILDDIR)/%.o)

# Build rules
$(BUILDDIR)/libCMSISDSP.a: $(OBJECTS)
	$(AR) -rc $@ $(OBJECTS)


$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
