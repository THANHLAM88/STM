PROGRAM = uart
 
ENTRY_FILE = startup
 
ENTRY_CFILE = $(ENTRY_FILE).c
ENTRY_OFILE = $(ENTRY_FILE).o
 
RCC_CFILE = ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
RCC_OFILE = stm32f10x_rcc.o
 
GPIO_CFILE = ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
GPIO_OFILE = stm32f10x_gpio.o
 
USART_CFILE = ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
USART_OFILE = stm32f10x_usart.o
 
ALL_OFILES = main.o $(ENTRY_OFILE) $(RCC_OFILE) $(GPIO_OFILE) $(USART_OFILE)
 
TARGET_BIN = $(PROGRAM).bin
TARGET_ELF = $(PROGRAM).elf
 
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy
 
LKR_SCRIPT = $(PROGRAM).ld
 
FAMILY = STM32F10X_MD
 
DEFINES = -D$(FAMILY)\
          -DUSE_STDPERIPH_DRIVER
           
INCLUDES = -I../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/ \
           -I../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/CoreSupport/ \
           -I../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/inc/ \
           -I../STM32F10x_StdPeriph_Lib_V3.5.0/Project/STM32F10x_StdPeriph_Template
 
CFLAGS  = -c -march=armv7-m -mcpu=cortex-m3 -mthumb \
          -fno-common -nostdlib -fno-builtin -ffreestanding \
          -Wall -O0 -g  \
          $(DEFINES)
           
LFLAGS  = -nostartfiles -T$(LKR_SCRIPT)
CPFLAGS = -Obinary
 
.PHONY: all clean write
 
all: $(TARGET_BIN)
 
$(TARGET_BIN): $(TARGET_ELF)
    $(CP) $(CPFLAGS) $< $@
     
$(TARGET_ELF): $(ALL_OFILES)
    $(LD) $(LFLAGS) $(ALL_OFILES) -o $@ 
     
main.o: main.c
    $(CC) $(INCLUDES) $(CFLAGS) main.c -o $@
 
$(ENTRY_OFILE): $(ENTRY_CFILE)
    $(CC) $(INCLUDES) $(CFLAGS) $(ENTRY_CFILE) -o $@
     
$(RCC_OFILE): $(RCC_CFILE)
    $(CC) $(INCLUDES) $(CFLAGS) $(RCC_CFILE) -o $@
 
$(GPIO_OFILE): $(GPIO_CFILE)
    $(CC) $(INCLUDES) $(CFLAGS) $(GPIO_CFILE) -o $@
 
$(USART_OFILE): $(USART_CFILE)
    $(CC) $(INCLUDES) $(CFLAGS) $(USART_CFILE) -o $@
     
clean:
    rm -rf *.o *.elf *.bin
 
write: 
    ./write_bin.sh openocd.cfg $(TARGET_ELF)