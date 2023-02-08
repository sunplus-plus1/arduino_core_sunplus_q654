TOP = .

CHIP     ?= Q645
CHIPDIRS ?= sp645
FREERTOS ?= 0
OPENAMP  ?= 0
LVGL     ?= 0
CMSISDSP ?= 0
CM_BACKTRACE ?= 1

#for cunit test
CUNIT ?= 0

BIN 	= bin
TARGET 	= firmware

include ./make.cfg

#for test.
ifeq ($(CHIP),SP7350)
FREERTOS = 1
else ifeq ($(CHIP),Q628)
OPENAMP  = 1
CM_BACKTRACE = 0
endif

LD_SRC = $(VARIANTS_PATH)/ldscript.ldi
LD_FILE = $(VARIANTS_PATH)/ldscript.ld

#INCLUDE PATH
#CMSIS header files
CCFLAGS += -I$(VARIANTS_PATH)
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/$(CHIPDIRS)/Include
CCFLAGS += -I$(TOP)/system/drivers/CMSIS/Device/sunplus/$(CHIPDIRS)/Source/gcc

ifneq ($(CHIP),Q628)
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
CCFLAGS += -I$(TOP)/libraries/IWatchdog/src

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
DIRS += $(TOP)/libraries/IWatchdog/src

DIRS += $(TOP)/cores/arduino
DIRS += $(TOP)/cores/arduino/avr
DIRS += $(TOP)/cores/arduino/sunplus

ifeq ($(CUNIT),1)
CCFLAGS += -DCUNIT_TEST
CCFLAGS += -I$(TOP)/CUnit/Cunit/Headers
CCFLAGS += -I$(TOP)/CUnit
DIRS += $(TOP)/CUnit/Cunit/Sources
DIRS += $(TOP)/CUnit/Cunit/Sources/Basic
DIRS += $(TOP)/CUnit/Cunit/Sources/Framework
DIRS += $(TOP)/CUnit
endif

# cm_backtrace
ifeq ($(CM_BACKTRACE),1)
DIRS += $(TOP)/libraries/CmBacktrace/cm_backtrace
CCFLAGS += -I$(TOP)/libraries/CmBacktrace/cm_backtrace
endif

sinclude makefile.freertos
sinclude makefile.openamp
sinclude makefile.lvgl
sinclude makefile.cmsisdsp

# example
ifeq ($(FREERTOS),1)
CCFLAGS += -DFREERTOS
#freertos C++ example
CXXSOURCES += $(TOP)/libraries/examples/rtos/example.cpp
else
#arduino C++ example
ifeq ($(LVGL),1)
DIRS += $(TOP)/libraries/examples/lvgl
else ifeq ($(OPENAMP),1)
DIRS += $(TOP)/libraries/examples/VirtIOSerial
else
DIRS += $(TOP)/libraries/examples/timer
endif
#DIRS += $(TOP)/libraries/examples/uart
#DIRS += $(TOP)/libraries/examples/spi
#DIRS += $(TOP)/libraries/examples/i2c
#DIRS += $(TOP)/libraries/examples/gpio
#DIRS += $(TOP)/libraries/examples/pwm
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
