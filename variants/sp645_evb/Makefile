

# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif

CROSS = $(ARDUINO_PATH)../../../../crossgcc/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-
TOP = $(ARDUINO_PATH)../..

ifneq ($(CROSS),)
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
CPP = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
READELF = $(CROSS)readelf
SIZE = $(CROSS)size
endif

# Flags For C Only

#LD_FILE = $(ARDUINO_PATH)/sp645_cm4.ld

LDFLAGS = -T $(LD_FILE) -specs=nano.specs --specs=rdimon.specs  -lc -lrdimon -lm -lgcc -lstdc++ 
LDFLAGS += -Xlinker -Map=$(BIN)/$(TARGET).map

CFLAGS += -nostartfiles -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 

ifeq ($(DEBUG), 1)
    CFLAGS += -ggdb3 -Og
else
    CFLAGS += -O3
endif
CFLAGS += -fstrict-aliasing -Wstrict-aliasing -Wno-error=address-of-packed-member

COMMON_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb 

CXXFLAGS = $(COMMON_FLAGS) -c -Os -w -std=gnu++14 -ffunction-sections -fdata-sections -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -MMD

#OpenAMP libmetal log on
CCFLAGS += -DDEFAULT_LOGGER_ON -DMETAL_INTERNAL
# virt_uart
#CCFLAGS += -DVIRTIOCON -DENABLE_SERIALVIRTIO 

CCFLAGS += -DSP645

#INCLUDE PATH
#CMSIS header files
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_CM4/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/sp645/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/sp645/Source/gcc

#HAL header files
CCFLAGS += -I$(TOP)/system/drivers/sp645_hal_driver/inc
CCFLAGS += -I$(TOP)/system/sp645


#libraries srcwarpper header files for Arduino Framework
CCFLAGS += -I$(TOP)/system/drivers/sp645_hal_driver/src
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
#CCFLAGS += -I$(TOP)/cores/arduino/sunplus/OpenAMP
CCFLAGS += -I$(TOP)/variants/sp645_evb
#OpenAMP headers files
#CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP
#CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/libmetal/lib/include
#CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/lib/include
#CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/apps/machine/a926
#CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/virt_driver

# C Sources Files Directory
#DIRS += $(TOP)/system/sp645
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

#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal/generic
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/libmetal/generic/a926
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/machine
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/proxy
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/remoteproc
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/rpmsg
#DIRS += $(TOP)/cores/arduino/sunplus/OpenAMP/open-amp/virtio
#DIRS += $(TOP)/system/Middlewares/OpenAMP/virt_driver


#arduino C++ example
DIRS += $(TOP)/libraries/examples/timer
#DIRS += $(TOP)/libraries/examples/i2c
#DIRS += $(TOP)/libraries/examples/exti

#DIRS = $(TOP)/cores/arduino/sunplus
#DIRS += $(TOP)/libraries/DriverWrapper/src

CSOURCES += $(wildcard $(patsubst %,%/*.c, $(DIRS) ))
CXXSOURCES += $(wildcard $(patsubst %,%/*.cpp, $(DIRS) ))
ASOURCES += $(wildcard $(patsubst %,%/*.S, $(DIRS) ))


BIN = bin
TARGET = m4
LD_FILE = $(ARDUINO_PATH)ldscript.ld

LDFLAGS_COM = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -L $(shell dirname `$(CC) -print-file-name=libc.a`) -lstdc++ -lm -lc -lgcc

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

all: $(OBJS)

#	$(CPP) -P $(CFLAGS) $(LD_SRC) $(LD_FILE)
	$(CC) -ffunction-sections -fdata-sections $(CFLAGS) $(CCFLAGS) $(LDFLAGS) $+ -o $(BIN)/$(TARGET)
	$(OBJDUMP) -d -S $(BIN)/$(TARGET) > $(BIN)/$(TARGET).dis
	$(OBJCOPY) -O binary -S $(BIN)/$(TARGET) $(BIN)/$(TARGET).bin
%.o: %.S
	$P mkdir -p $(BIN)
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) $(CPPFLAGS) -MMD -c -o $@ $<


%.o: %.c
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) $(CPPFLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(Pecho) "  CXX  $<"
	$P $(CXX) $(CXXFLAGS) $(CCFLAGS) $(CPPFLAGS) -MMD -c -o $@ $<

clean:
	$P -rm -f $(OBJS) $(DEPS) rom.d >/dev/null
	$P -cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL) $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null


p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)
