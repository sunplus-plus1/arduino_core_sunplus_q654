# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif
DD = dd status=none bs=1k of=$@ seek=

CROSS = $(ARDUINO_PATH)../../../../crossgcc/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin/arm-none-eabi-
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
CFLAGS += -std=gnu11
# Flags For C++ Only
CXXFLAGS += -std=gnu++14 -fno-use-cxa-atexit
CCFLAGS += -MMD -O1
CCFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CCFLAGS += -fno-partial-inlining -fno-jump-tables
CCFLAGS += -ffunction-sections -fdata-sections
CCFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CCFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums -w
#OpenAMP libmetal log on
CCFLAGS += -DDEFAULT_LOGGER_ON -DMETAL_INTERNAL
# virt_uart
CCFLAGS += -DVIRTIOCON -DENABLE_SERIALVIRTIO 
CCFLAGS += -DSP7021


#INCLUDE PATH
#CMSIS header files
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/CMSIS/Core_ARMV5/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/sp7021/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/sp7021/Source/gcc

#HAL header files
CCFLAGS += -I$(TOP)/system/drivers/sp7021_hal_driver/inc
CCFLAGS += -I$(TOP)/system/sp7021

#libraries srcwarpper header files for Arduino Framework
CCFLAGS += -I$(TOP)/system/drivers/sp7021_hal_driver/src
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
CCFLAGS += -I$(TOP)/cores/arduino/sunplus/OpenAMP
CCFLAGS += -I$(TOP)/variants/sp7021_evb
#OpenAMP headers files
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/libmetal/lib/include
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/lib/include
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/open-amp/apps/machine/a926
CCFLAGS += -I$(TOP)/system/Middlewares/OpenAMP/virt_driver

# C Sources Files Directory
#DIRS += $(TOP)/system/sp7021
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


#arduino C++ example
#DIRS += $(TOP)/libraries/examples/timer
#DIRS += $(TOP)/libraries/examples/i2c


CSOURCES += $(wildcard $(patsubst %,%/*.c, $(DIRS) ))
CXXSOURCES += $(wildcard $(patsubst %,%/*.cpp, $(DIRS) ))
ASOURCES += $(wildcard $(patsubst %,%/*.S, $(DIRS) ))

BIN = bin
TARGET = rom
LD_FILE = $(ARDUINO_PATH)ldscript.ld
LDFLAGS = -T $(LD_FILE)
LDFLAGS_COM = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -L $(shell dirname `$(CC) -print-file-name=libc.a`) -lstdc++ -lm -lc -lgcc

SPI_ALL := spi_all.bin
SPI_HEX := spi_all.hex

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
DEPS = $(OBJS:.o=.d)


.PHONY: clean all

all: $(BIN)/$(SPI_ALL) $(BIN)/$(TARGET).dis

$(BIN)/$(TARGET).bin: $(BIN)/$(TARGET)
	$P $(OBJCOPY) -O binary -S $< $@


$(BIN)/$(TARGET).dis: $(BIN)/$(TARGET)
	$P $(OBJDUMP) -d -S $< > $@

$(BIN)/$(TARGET): $(OBJS) $(LD_FILE)
	$(Pecho) "  LD   $@"
	$P mkdir -p $(BIN)
	$P $(LD) $(OBJS) -o $(BIN)/$(TARGET) -Map $(BIN)/$(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)
	$P $(SIZE) -B $(BIN)/$(TARGET)



#$(LD_FILE): $(LD_SRC)
#	$P $(CPP) -P $(CFLAGS) -x c $< -o $@

$(BIN)/$(SPI_ALL): $(BIN)/$(TARGET).bin Makefile
	$(Pecho) "  PACK $@"
	$P bash $(ARDUINO_PATH)./script/add_uhdr.sh uboot_B $< $(FIRMWARE_TOP)$(BIN)/$(TARGET).img 0x200040 0x200040 $(ARDUINO_PATH)
	$P $(DD)64  if=$(ARDUINO_PATH)prebuilt/xboot_nor.img
	$P $(DD)256 if=$(BIN)/$(TARGET).img
	$P ls -l $@


%.o: %.S
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $<


%.o: %.c
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $<

%.o: %.cpp
	$(Pecho) "  CXX  $<"
	$P $(CXX) $(CXXFLAGS) $(CCFLAGS) -c -o $@ $<

clean:
	$P -rm -f $(OBJS) $(DEPS) rom.d >/dev/null
	$P -cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL) $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null


p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)
