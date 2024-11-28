# Project configuration
PROJECT ?= motor_controller
CC = arm-none-eabi-gcc.exe
LD = arm-none-eabi-gcc.exe
OBJCOPY = arm-none-eabi-objcopy.exe
# CC_PATH = /opt/gcc-arm-none-eabi-10.3-2021.10/arm-none-eabi
STD_PERIPH_LIBS ?= ./STM32F10x_StdPeriph_Lib_V3.6.0/

# Directories
SOURCES_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
STARTUP_DIR = startup
LIB_DIR = lib
3RD_LIB_DIR = 3rdparty

ST_FLASH ?= st-flash
ST_CLI ?= 'F:\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe'

# Source files
SOURCES = \
    $(SOURCES_DIR)/main.c \
    $(SOURCES_DIR)/stm32f10x_it.c \
    $(SOURCES_DIR)/system_stm32f10x.c \
    $(SOURCES_DIR)/app.c \
	$(SOURCES_DIR)/bsp/key.c \
    $(SOURCES_DIR)/bsp/tim2.c \
	$(SOURCES_DIR)/syscalls.c \
    $(SOURCES_DIR)/bsp/can1.c \
	$(SOURCES_DIR)/bsp/led.c \
    $(SOURCES_DIR)/bsp/usart1.c \
    $(SOURCES_DIR)/device/mf4010v2.c \
    $(SOURCES_DIR)/device/MPU6050/mpu6050.c \
    $(SOURCES_DIR)/device/MPU6050/mpuiic.c \
    $(SOURCES_DIR)/device/MPU6050/eMPL/inv_mpu.c  \
    $(SOURCES_DIR)/device/MPU6050/eMPL/inv_mpu_dmp_motion_driver.c \
    $(SOURCES_DIR)/mirco_lib/mirco_string.c \
	$(SOURCES_DIR)/mirco_lib/mirco_math.c \
    $(SOURCES_DIR)/mirco_lib/mirco_delay.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c \
	$(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c \
    $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c \
    $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c

# Include directories
INCLUDES = \
    -I$(INCLUDE_DIR) \
    -I$(3RD_LIB_DIR) \
    -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/ \
    -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/ \
    -I$(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/inc
# -I$(CC_PATH)/include

# Libraries
LIBS = -nostartfiles -static -Llib -lcjson -lpid -lc -lm -lnosys

# Compiler and linker flags
CFLAGS = \
    -g -Wall --specs=nano.specs -march=armv7-m -O0 \
    -mlittle-endian -mthumb -mcpu=cortex-m3 \
    -mfloat-abi=soft \
    -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER \
    $(INCLUDES)

LDFLAGS = \
    -Wl,--gc-sections $(LIBS) -T$(STARTUP_DIR)/stm32_flash.ld -u _printf_float

# Startup file
START_FILE = startup_stm32f10x_hd

# st-flash tool
ST_FLASH ?= st-flash

# Object files
OBJECTS = $(SOURCES:$(SOURCES_DIR)/%.c=$(BUILD_DIR)/%.o) \
          $(BUILD_DIR)/startup/$(START_FILE).o

# Quiet mode
QUIET ?= @

# LVGL_DIR_NAME ?= 3rdparty/lvgl
# LVGL_DIR ?= ${shell pwd}
# include $(LVGL_DIR)/lvgl.mk

# Default target
.PHONY: clean flash compile_commands static

# static:
# 	@echo "Compiling static libraries..."
# 	@make -C $(3RD_LIB_DIR)/mpu6050_driver clean
# 	@make -C $(3RD_LIB_DIR)/mpu6050_driver all
# 	@cp $(3RD_LIB_DIR)/mpu6050_driver/src/libmpu6050.a $(LIB_DIR)/libmpu6050.a

all: $(BUILD_DIR)/$(PROJECT).elf

# Compile target
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@echo "Linking $@..."
#   $(QUIET)mkdir -p $(BUILD_DIR)
	$(QUIET)$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@
	@echo "Generating HEX and BIN files..."
	$(QUIET)$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(PROJECT).hex
	$(QUIET)$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(PROJECT).bin

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.c
	@echo "Compiling $<..."
# $(QUIET)if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling the startup file
$(BUILD_DIR)/startup/$(START_FILE).o: $(STARTUP_DIR)/$(START_FILE).s
	@echo "Compiling $<..."
#   $(QUIET)if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

# Remove binary files

clean:
	@echo "Cleaning project..."
	$(QUIET)rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.hex $(BUILD_DIR)/*.bin

# Flash target

flash:
	openocd.exe -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(PROJECT).elf verify reset exit"

debug:
	openocd.exe -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(PROJECT).elf verify reset exit"
	@echo "Starting GDB server..."
	$(QUIET)arm-none-eabi-gdb -x $(STARTUP_DIR)/stm32_debug.gdb $(BUILD_DIR)/$(PROJECT).elf

# Generate compile_commands.json for clangd
compile_commands:
	@echo "Generating compile_commands.json..."
	$(QUIET)bear -- make all -j16
	$(QUIET)mv -f compile_commands.json $(BUILD_DIR)/compile_commands.json
