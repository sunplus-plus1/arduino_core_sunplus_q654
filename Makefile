TOP = .

CHIP     ?= Q645
CHIPDIRS ?=	sp645
FREERTOS ?= 0
OPENMAP  ?= 0

BIN 	= bin
TARGET 	= firmware

include ./make.cfg

ifeq ($(CHIP),Q645)
CCFLAGS += -DSP645
else
CCFLAGS += -DSP7021
OPENMAP  = 1
endif


LD_SRC = $(VARIANTS_PATH)/ldscript.ldi
LD_FILE = $(VARIANTS_PATH)/ldscript.ld

#INCLUDE PATH
#CMSIS header files
CCFLAGS += -I$(VARIANTS_PATH)
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/$(CHIPDIRS)/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/$(CHIPDIRS)/Source/gcc

ifeq ($(CHIP),Q645)
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_CM4/Include
else
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_ARMV5/Include
endif
#HAL header files
CCFLAGS += -I$(TOP)/system/drivers/$(CHIPDIRS)_hal_driver/inc
CCFLAGS += -I$(TOP)/system/$(CHIPDIRS)


#libraries srcwarpper header files for Arduino Framework
CCFLAGS += -I$(TOP)/system/drivers/$(CHIPDIRS)_hal_driver/src
CCFLAGS += -I$(TOP)/libraries/DriverWrapper/src
CCFLAGS += -I$(TOP)/libraries/DriverWrapper/src/sunplus
CCFLAGS += -I$(TOP)/libraries/SPI/src
CCFLAGS += -I$(TOP)/libraries/SPI/src/utility
CCFLAGS += -I$(TOP)/libraries/Wire/src
CCFLAGS += -I$(TOP)/libraries/Wire/src/utility

#Arduino header files for Arduino Framework
CCFLAGS += -I$(TOP)/cores/arduino
CCFLAGS += -I$(TOP)/cores/arduino/avr
CCFLAGS += -I$(TOP)/cores/arduino/sunplus
CCFLAGS += -I$(TOP)/cores/arduino/sunplus/hal
CCFLAGS += -I$(TOP)/cores/arduino/sunplus/LL
CCFLAGS += -I$(TOP)/cores/arduino/sunplus/LL
CCFLAGS += -I$(TOP)/variants/$(CHIPDIRS)_evb

# C Sources Files Directory
#DIRS += $(TOP)/system/$(CHIPDIRS)
DIRS += $(TOP)/libraries/DriverWrapper/src
DIRS += $(TOP)/libraries/DriverWrapper/src/HAL
DIRS += $(TOP)/libraries/DriverWrapper/src/sunplus
DIRS += $(TOP)/libraries/SPI/src
DIRS += $(TOP)/libraries/SPI/src/utility
DIRS += $(TOP)/libraries/Wire/src
DIRS += $(TOP)/libraries/Wire/src/utility
DIRS += $(TOP)/cores/arduino
DIRS += $(TOP)/cores/arduino/avr
DIRS += $(TOP)/cores/arduino/sunplus


###  OPENAMP ###
ifeq ($(OPENMAP),1)
#OpenAMP libmetal log on
CCFLAGS += -DDEFAULT_LOGGER_ON -DMETAL_INTERNAL
# virt_uart
CCFLAGS += -DVIRTIOCON -DENABLE_SERIALVIRTIO
#OpenAMP headers files
CCFLAGS += -I$(TOP)/cores/arduino/sunplus/OpenAMP
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/libmetal/lib/include
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/lib/include
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/apps/machine/a926
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/virt_driver

DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal/generic
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal/generic/a926
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/machine
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/proxy
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/remoteproc
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/rpmsg
DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/virtio
DIRS += $(TOP)/system/Middlewares/OpenAMP/virt_driver
endif

#include freertos files
sinclude makefile.freertos

# example
ifeq ($(FREERTOS),1)
CCFLAGS += -DFREERTOS
#freertos C++ example
CXXSOURCES += $(TOP)/libraries/examples/rtos/example.cpp
else
#arduino C++ example
DIRS += $(TOP)/libraries/examples/timer
#DIRS += $(TOP)/libraries/examples/i2c
#DIRS += $(TOP)/libraries/examples/exti
#DIRS = $(TOP)/cores/arduino/sunplus
#DIRS += $(TOP)/libraries/DriverWrapper/src
endif

CSOURCES += $(wildcard $(patsubst %,%/*.c, $(DIRS) ))
CXXSOURCES += $(wildcard $(patsubst %,%/*.cpp, $(DIRS) ))
ASOURCES += $(wildcard $(patsubst %,%/*.S, $(DIRS) ))


OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

all: $(OBJS)
	$P $(CPP) -P $(CFLAGS) $(LD_SRC) $(LD_FILE)
	$P $(GCC)  $(CFLAGS) $(CCFLAGS) $(LDFLAGS) $+ -o $(BIN)/$(TARGET) $(LDFLAGS_END)
	$(OBJDUMP) -d -S $(BIN)/$(TARGET) > $(BIN)/$(TARGET).dis
	$(OBJCOPY) -O binary -S $(BIN)/$(TARGET) $(BIN)/$(TARGET).bin
	@echo "firmware buid done !"

%.o: %.S
	$P mkdir -p $(BIN)
	$(Pecho) "  CC   $<"
	$P $(GCC) $(CFLAGS) $(CCFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.c
	$(Pecho) "  CC   $<"
	$P $(GCC) $(CFLAGS) $(CCFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp
	$(Pecho) "  CXX  $<"
	$P $(CXX) $(CXXFLAGS) $(CCFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	$P -rm -f $(OBJS) $(DEPS) rom.d $(LD_FILE) >/dev/null
	$P -rm -rf $(BIN) >/dev/null


p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)