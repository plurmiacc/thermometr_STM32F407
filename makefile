TOOLCHAIN_ROOT =

TARGET = thermometr_TestTask.elf
SRC_DIR = ./Core/Src
INC_DIR = ./Core/Inc

CC = $(TOOLCHAIN_ROOT)arm-none-eabi-gcc
DB = $(TOOLCHAIN_ROOT)arm-none-eabi-gdb

SRC_FILES = $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*/*.c)
ASM_FILES += ./Core/Startup/startup_stm32f407iehx.s
LD_SCRIPT = ./STM32F407IEHX_FLASH.ld

INCLUDES   = -I$(INC_DIR)
INCLUDES   += -I./Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCLUDES   += -I./Drivers/CMSIS/Include

CFLAGS  = -g -O0 -Wall -Wextra -Warray-bounds -Wno-unused-parameter
CFLAGS += -mcpu=cortex-m4 -mthumb -mlittle-endian -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F407xx
CFLAGS += $(INCLUDES)

LFLAGS = -T$(LD_SCRIPT) --specs=nosys.specs -Wl,-Map="thermometr_TestTask.map" -Wl,--gc-sections -static --specs=nano.specs -Wl,--start-group -lc -lm -Wl,--end-group

CXX_OBJS = $(SRC_FILES:.c=.o)
ASM_OBJS = $(ASM_FILES:.s=.o)
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)

.PHONY: clean

all: $(TARGET)

# Compile
$(CXX_OBJS): %.o: %.c
$(ASM_OBJS): %.o: %.s
$(ALL_OBJS):
	@echo "[CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
%.elf: $(ALL_OBJS)
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $(ALL_OBJS) -o $@

# Clean
clean:
	@rm -f $(ALL_OBJS) $(TARGET)
