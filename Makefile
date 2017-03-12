PROJECT_NAME=mfsw

BUILDDIR = build

DEVICE = inc/STM32F4xx
CORE = inc/CMSIS
PERIPH = inc/STM32F4xx_StdPeriph_Driver
DISCOVERY = inc
USB = inc/usb

#SOURCES += $(DISCOVERY)/src/stm32f4_discovery.c

SOURCES += \
	$(PERIPH)/src/stm32f4xx_gpio.c \
	$(PERIPH)/src/stm32f4xx_i2c.c \
	$(PERIPH)/src/stm32f4xx_rcc.c \
	$(PERIPH)/src/stm32f4xx_can.c \
	$(PERIPH)/src/stm32f4xx_spi.c \
	$(PERIPH)/src/stm32f4xx_exti.c \
	$(PERIPH)/src/stm32f4xx_syscfg.c \
	$(PERIPH)/src/misc.c

SOURCES += startup_stm32f4xx.S
#SOURCES += stm32f4xx_it.c
SOURCES += system_stm32f4xx.c

SOURCES += \
	src/main.c \
	src/can.c \
	src/syscalls.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

DEFINES += USE_STDPERIPH_DRIVER
#DEFINES += __TARGET_FPU_VFP

INCLUDES += $(DEVICE)
INCLUDES += $(CORE)
INCLUDES += $(PERIPH)/inc
INCLUDES += $(DISCOVERY)
INCLUDES += $(USB)/inc
INCLUDES += .

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb


CFLAGS += -mthumb -mcpu=cortex-m4 # архитектура и система комманд
CFLAGS += -std=gnu99              # стандарт языка С
CFLAGS += -Wall -pedantic         # Выводить все предупреждения
CFLAGS += -Os                     # Оптимизация
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=soft
CFLAGS += -ggdb                   # Генерировать отладочную информацию для gdb
CFLAGS += -fno-builtin

CFLAGS += $(addprefix -I, $(INCLUDES))
CFLAGS += $(addprefix -D, $(DEFINES))

LDSCRIPT = stm32_flash.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -nostdlib

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: $(BIN)
	st-flash write $(BIN) 0x8000000

debug: $(ELF)
	$(GDB) -tui $(ELF)

all: $(HEX) $(BIN)

.PHONY: clean
clean:
	rm -rf build
