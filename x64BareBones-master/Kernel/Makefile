include Makefile.inc

KERNEL=kernel.elf

SOURCES_COMMON=$(wildcard *.c) \
               $(wildcard Processes/*.c) \
               $(wildcard Semaphores/*.c) \
               $(wildcard Pipes/*.c)

SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

MEMORY_MANAGER ?= freelist

ifeq ($(MEMORY_MANAGER),buddy)
MM_SOURCE=MemoryManager/MemoryManagerBuddy.c
else
MM_SOURCE=MemoryManager/MemoryManagerFreeArray.c
endif

SOURCES=$(SOURCES_COMMON) $(MM_SOURCE)
OBJECTS=$(SOURCES:.c=.o)

all: $(KERNEL)

buddy:
	$(MAKE) MEMORY_MANAGER=buddy

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.elf *.bin Processes/*.o Semaphores/*.o Pipes/*.o MemoryManager/*.o

.PHONY: all clean buddy
