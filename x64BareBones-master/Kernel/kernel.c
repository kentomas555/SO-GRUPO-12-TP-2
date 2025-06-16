// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kernel.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const stackStartAddress = (void*)0x600000;


static MemoryManagerADT kernelMM;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

MemoryManagerADT getKernelMem() {
	return kernelMM;
}

static void idleKernel(){
    while(1){
		_hlt();
    }
}

int main()
{	

	_cli();
	setTickFreq(1000);
	load_idt();
	
	//sound(800, 10);

	createMemoryManager(); // empieza en 0x700000
	startStack(stackStartAddress); // empieza en 0x600000
	startScheduler(); // empieza en 0x700000 (usa el memory manager, alloc)
	// initializeSemaphores(); // en que parte de memoria inician?

	int16_t fds[2] = {STDIN,STDOUT};
	char * args[] = {NULL};
	onCreateProcess("idle", (mainFunc)idleKernel, args, LOWEST_PRIORITY, fds);
	onCreateProcess("shell", sampleCodeModuleAddress, args, HIGHEST_PRIORITY, fds);
	_sti();

	while(1);
	return 0;
}