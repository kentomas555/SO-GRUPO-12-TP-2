
all:  bootloader kernel userland bin image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

bin:
	cd Userland; objcopy -O binary 0000-sampleCodeModule.elf 0000-sampleCodeModule.bin
	cd Kernel; objcopy -O binary kernel.elf kernel.bin

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
