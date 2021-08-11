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
SYSTEM_SP7021_SRC += $(TOP)/system/sp7021
ARDUNIO_HEADER += $(TOP)/cores/arduino
#ARDUNIO_HEADER += $(TOP)/variants/sp7021_evb
CMSIS_SRC = $(TOP)/system/drivers/CMSIS/Device/sunplus/sp7021/Source/gcc
HAL_DRIVER_SRC = $(TOP)/system/drivers/sp7021_hal_driver/src
ARDUNIO_SRC = $(TOP)/cores/arduino
WRAPPER_SRC = $(TOP)/libraries/DriverWrapper/src/sunplus
SPI_SRC = $(TOP)/libraries/SPI/src
I2C_SRC = $(TOP)/libraries/Wire/src
TEST_SRC = $(TOP)/libraries/test
EXAMPLE_TIMER_SRC = $(TOP)/libraries/examples/timer
#INCLUDE_DIR += -I$(CMSIS_HEADER) -I$(HAL_DRIVER_HEADER) -I$(ARDUNIO_HEADER)

vpath %.c $(CMSIS_SRC):$(HAL_DRIVER_SRC):$(ARDUNIO_SRC):$(WRAPPER_SRC)
vpath %.cpp $(CMSIS_SRC):$(HAL_DRIVER_SRC):$(ARDUNIO_SRC):$(WRAPPER_SRC):$(SPI_SRC):$(I2C_SRC):$(TEST_SRC):$(EXAMPLE_TIMER_SRC)
vpath %.S $(CMSIS_SRC)
# flags for c only
CFLAGS += -std=gnu11

# flags for c++ only
CXXFLAGS += -std=gnu++14 -fno-use-cxa-atexit

# common flags for c & c++
CCFLAGS += -MMD -O1
#CCFLAGS += -nostdlib -fno-builtin
CCFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CCFLAGS += -fno-partial-inlining -fno-jump-tables
#CCFLAGS += -static
#CCFLAGS += -nodefaultlibs
CCFLAGS += -ffunction-sections -fdata-sections
CCFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CCFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums -w
CCFLAGS += -I$(CMSIS_HEADER)
CCFLAGS += -I$(CMSIS_DEVICE_HEADER)
CCFLAGS += -I$(HAL_DRIVER_HEADER)
CCFLAGS += -I$(ARDUNIO_HEADER)/sunplus
CCFLAGS += -I$(ARDUNIO_HEADER)/sunplus/LL
CCFLAGS += -I$(ARDUNIO_HEADER)/sunplus/OpenAMP
CCFLAGS += -I$(ARDUNIO_HEADER)
CCFLAGS += -I$(SYSTEM_SP7021_SRC)
CCFLAGS += -I$(TOP)/variants/sp7021_evb -I$(TOP)/variants/sp7021_evb/include
CCFLAGS += -I$(TOP)/libraries/DriverWrapper/src/sunplus
CCFLAGS += -I$(I2C_SRC)
CCFLAGS += -I$(ARDUINO_HEADER)
CCFLAGS += -I$(ARDUINO_SUNPLUS_HEADER)

SPI_ALL := spi_all.bin
SPI_HEX := spi_all.hex


LDFLAGS_COM += -L $(SYSTEM_SP7021_SRC) 
# Boot up
ASOURCES = startup_sp7021_arm926.S
ASOURCES += vectors.S
CSOURCES += sunplus_def.c wiring_time.c hooks.c board.c itoa.c $(ARDUNIO_SRC)/avr/dtostrf.c wiring_digital.c
CSOURCES += $(TOP)/libraries/DriverWrapper/src/syscall.c
CXXSOURCES += WInterrupts.cpp interrupt.cpp
CXXSOURCES += HardwareTimer.cpp new.cpp 
CXXSOURCES += HardwareSerial.cpp WSerial.cpp Stream.cpp Print.cpp WString.cpp SPI.cpp variant.cpp main.cpp timer_interrupt.cpp VirtIOSerial.cpp  Wire.cpp


# HAL
CSOURCES += $(wildcard $(patsubst %,%/*.c,$(HAL_DRIVER_SRC) $(WRAPPER_SRC) $(SPI_SRC)/utility $(I2C_SRC)/utility $(SYSTEM_SP7021_SRC)))

# OpenAMP
MW_DIR = $(TOP)/system/Middlewares
OPENAMP_SRC = $(MW_DIR)/OpenAMP/open-amp
LIBMETAL_SRC = $(MW_DIR)/OpenAMP/libmetal/lib
CCFLAGS += -I$(LIBMETAL_SRC)/include \
           -I$(OPENAMP_SRC)/lib/include \
           -I$(OPENAMP_SRC)/apps/machine/a926 \
           -I$(ARDUNIO_HEADER)/sunplus/OpenAMP \
           -I$(MW_DIR)/OpenAMP \
           -I$(MW_DIR)/OpenAMP/virt_driver
			
# open-amp
CSOURCES += $(OPENAMP_SRC)/lib/virtio/virtio.c \
            $(OPENAMP_SRC)/lib/virtio/virtqueue.c \
            $(OPENAMP_SRC)/lib/rpmsg/rpmsg.c \
            $(OPENAMP_SRC)/lib/rpmsg/rpmsg_virtio.c \
            $(OPENAMP_SRC)/lib/remoteproc/remoteproc.c \
            $(OPENAMP_SRC)/lib/remoteproc/remoteproc_virtio.c \
            $(OPENAMP_SRC)/lib/remoteproc/rsc_table_parser.c \
            $(OPENAMP_SRC)/lib/remoteproc/elf_loader.c \
            $(OPENAMP_SRC)/lib/proxy/rpmsg_retarget.c \
            $(OPENAMP_SRC)/apps/machine/a926/a926_rproc.c \
            $(OPENAMP_SRC)/apps/machine/a926/platform_info.c \
            $(OPENAMP_SRC)/apps/machine/a926/rsc_table.c \
#			$(OPENAMP_SRC)/apps/examples/echo/rpmsg-echo.c \
#			$(OPENAMP_SRC)/apps/examples/rpc_demo/rpc_demo.c \

# libmetal
CCFLAGS += -DDEFAULT_LOGGER_ON -DMETAL_INTERNAL
CSOURCES += $(LIBMETAL_SRC)/dma.c \
            $(LIBMETAL_SRC)/device.c \
            $(LIBMETAL_SRC)/init.c \
            $(LIBMETAL_SRC)/io.c \
            $(LIBMETAL_SRC)/irq.c \
            $(LIBMETAL_SRC)/log.c \
            $(LIBMETAL_SRC)/shmem.c \
            $(LIBMETAL_SRC)/shmem-provider.c \
            $(LIBMETAL_SRC)/softirq.c \
            $(LIBMETAL_SRC)/version.c \
            $(LIBMETAL_SRC)/system/generic/condition.c \
            $(LIBMETAL_SRC)/system/generic/device.c \
            $(LIBMETAL_SRC)/system/generic/init.c \
            $(LIBMETAL_SRC)/system/generic/io.c \
            $(LIBMETAL_SRC)/system/generic/irq.c \
            $(LIBMETAL_SRC)/system/generic/time.c \
            $(LIBMETAL_SRC)/system/generic/a926/sys.c \

# virt_uart
CCFLAGS += -DVIRTIOCON -DENABLE_SERIALVIRTIO -I$(MW_DIR)/OpenAMP/virt_driver
CSOURCES += $(MW_DIR)/OpenAMP/virt_driver/virtio_buffer.c \
           	$(MW_DIR)/OpenAMP/virt_driver/virt_uart.c \
#            $(MW_DIR)/OpenAMP/virt_uart/virt_uart_demo.c

# VirtIOSerial
#CXXSOURCES += $(MW_DIR)/OpenAMP/virt_uart/VirtIOSerial.cpp \
#1              $(MW_DIR)/OpenAMP/virt_uart/virt_uart_demo2.cpp


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
#	$P $(LD) $(OBJS) i2c_mas.o -o $(BIN)/$(TARGET) -Map $(BIN)/$(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)

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
