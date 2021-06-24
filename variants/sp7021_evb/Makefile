# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif
DD = dd status=none bs=1k of=$@ seek=

COMMON_DIR = common
LIB = lib
TESTAPI = testapi

CROSS = ../../../../crossgcc/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin/arm-none-eabi-
TOP = ../..
ifneq ($(CROSS),)
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
CPP = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
READELF = $(CROSS)readelf
endif

BIN = bin
TARGET = rom
LD_FILE = ldscript.ld
LD_SRC = script/rom.ldi
LDFLAGS = -T $(LD_FILE)
LDFLAGS_COM = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -L $(shell dirname `$(CC) -print-file-name=libc.a`) -lstdc++ -lm -lc -lgcc

CMSIS_HEADER += $(TOP)/system/drivers/CMSIS/CMSIS/Core_ARMV5/Include
CMSIS_DEVICE_HEADER += $(TOP)/system/drivers/CMSIS/Device/sunplus/sp7021/Include
HAL_DRIVER_HEADER += $(TOP)/system/drivers/sp7021_hal_driver/inc
#HAL_DRIVER_HEADER += $(TOP)/system/sp7021
ARDUNIO_HEADER += $(TOP)/cores/arduino
#ARDUNIO_HEADER += $(TOP)/variants/sp7021_evb
CMSIS_SRC = $(TOP)/system/drivers/CMSIS/Device/sunplus/sp7021/Source
HAL_DRIVER_SRC = $(TOP)/system/drivers/sp7021_hal_driver/src
ARDUNIO_SRC = $(TOP)/cores/arduino
WRAPPER_SRC = $(TOP)/libraries/DriverWrapper/src/sunplus
SPI_SRC = $(TOP)/libraries/SPI/src
TEST_SRC = $(TOP)/libraries/test
#INCLUDE_DIR += -I$(CMSIS_HEADER) -I$(HAL_DRIVER_HEADER) -I$(ARDUNIO_HEADER)

vpath %.c $(CMSIS_SRC):$(HAL_DRIVER_SRC):$(ARDUNIO_SRC):$(WRAPPER_SRC):$(SPI_SRC)
vpath %.cpp $(CMSIS_SRC):$(HAL_DRIVER_SRC):$(ARDUNIO_SRC):$(WRAPPER_SRC):$(SPI_SRC):$(TEST_SRC)


CFLAGS += -MMD -O1
CFLAGS += -nostdlib -fno-builtin
CFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CFLAGS += -fno-partial-inlining -fno-jump-tables
CFLAGS += -static
CFLAGS += -nodefaultlibs
CFLAGS += -ffunction-sections -fdata-sections 
CFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CFLAGS += -Iinclude -Iinclude/util -I$(TESTAPI)/qch -g 
CFLAGS += -I$(CMSIS_HEADER)
CFLAGS += -I$(CMSIS_DEVICE_HEADER)
CFLAGS += -I$(HAL_DRIVER_HEADER)
CFLAGS += -I$(ARDUNIO_HEADER)/sunplus
CFLAGS += -I$(ARDUNIO_HEADER)/sunplus/LL
CFLAGS += -I$(ARDUNIO_HEADER)
CFLAGS += -I$(TOP)/system/sp7021
CFLAGS += -I$(TOP)/variants/sp7021_evb
CFLAGS += -I$(TOP)/libraries/DriverWrapper/src/sunplus

CXXFLAGS += -MMD -O1
CXXFLAGS += -nostdlib -fno-builtin
CXXFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CXXFLAGS += -fno-partial-inlining -fno-jump-tables
CXXFLAGS += -static
CXXFLAGS += -nodefaultlibs
CXXFLAGS += -ffunction-sections -fdata-sections
CXXFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CXXFLAGS += -fno-use-cxa-atexit -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -w -std=gnu++14
CXXFLAGS += -I$(CMSIS_HEADER)
CXXFLAGS += -I$(CMSIS_DEVICE_HEADER)
CXXFLAGS += -I$(HAL_DRIVER_HEADER)
CXXFLAGS += -I$(ARDUNIO_HEADER)/sunplus
CXXFLAGS += -I$(ARDUNIO_HEADER)/sunplus/LL
CXXFLAGS += -I$(ARDUNIO_HEADER)
CXXFLAGS += -I$(TOP)/system/sp7021
CXXFLAGS += -I$(TOP)/variants/sp7021_evb
CFLAGS += -I$(ARDUINO_HEADER)
CFLAGS += -I$(ARDUINO_SUNPLUS_HEADER)
#-mthumb -mthumb-interwork

SPI_ALL := spi_all.bin
SPI_HEX := spi_all.hex

# Boot up
ASOURCES = start.S
CSOURCES = main.c

# common
CSOURCES += $(COMMON_DIR)/common.c

# mon
#CSOURCES += mon/monitor.c

# interrupt
CSOURCES += $(COMMON_DIR)/sp_interrupt.c
ASOURCES += $(COMMON_DIR)/vectors.S
#CSOURCES += sp7021_hal_irq_ctrl.c 
#CSOURCES += sp7021_hal_tim.c sp7021_hal.c sunplus_def.c
CSOURCES += sunplus_def.c wiring_time.c hooks.c board.c itoa.c $(ARDUNIO_SRC)/avr/dtostrf.c wiring_digital.c
CXXSOURCES += WInterrupts.cpp interrupt.cpp
CXXSOURCES += HardwareTimer.cpp new.cpp timer_test.cpp uart_test.cpp
CXXSOURCES += HardwareSerial.cpp WSerial.cpp Stream.cpp Print.cpp WString.cpp SPI.cpp


# HAL

CSOURCES += $(wildcard $(patsubst %,%/*.c,$(HAL_DRIVER_SRC) $(WRAPPER_SRC) $(SPI_SRC)/utility))

# ardunio C source
#CSOURCES += Marlin/arduino/wiring_analog.c Marlin/arduino/wiring_digital.c Marlin/arduino/wiring.c
#CXXSOURCES += $(ARDUINO_SRC)/WInterrupts.cpp $(WRAPPER_SRC)/interrupt.cpp
#CXXSOURCES += $(WRAPPER_SRC)/interrupt.cpp

# Marlin
CFLAGS += -fno-use-cxa-atexit -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -w -std=gnu++14
#CFLAGS += -IMarlin/include -IMarlin/arduino
#CFLAGS += -DF_CPU=16000000 -DARDUINO=105 -DMOTHERBOARD=33 -D__AVR_ATmega2560__ -DCPU_32_BIT
#CXXSOURCES += Marlin/motion_control.cpp Marlin/MarlinSerial.cpp Marlin/stepper.cpp Marlin/planner.cpp 
#CXXSOURCES += Marlin/temperature_ads1015.cpp Marlin/temperature.cpp
#CXXSOURCES += Marlin/ConfigurationStore.cpp Marlin/Marlin_main.cpp Marlin/arduino/main.cpp
#CFLAGS += -D__GNUC__

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

#$(LD_FILE): $(LD_SRC)
#	$P $(CPP) -P $(CFLAGS) -x c $< -o $@

$(BIN)/$(SPI_ALL): $(BIN)/$(TARGET).bin Makefile
	$(Pecho) "  PACK $@"
	$P bash ./script/add_uhdr.sh uboot_B $< $(BIN)/$(TARGET).img 0x200040 0x200040
	$P $(DD)64  if=prebuilt/xboot_nor.img
	$P $(DD)256 if=$(BIN)/$(TARGET).img
	$P ls -l $@

#testapi/qch/iop.o: testapi/qch/DQ8051.bin
%.o: %.S
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) -c -o $@ $<


%.o: %.c*
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) -c -o $@ $<

clean:
	$P -rm -f $(OBJS) $(DEPS) rom.d >/dev/null
	$P -cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL) $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null


p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)
