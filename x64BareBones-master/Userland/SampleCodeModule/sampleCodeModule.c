/* sampleCodeModule.c */
#include <stdarg.h>
#include <stdint.h>

#include <globalLib.h>
#include <userShell.h>

static void idleFunction(){
    while(1){
        hlt();
    }
}

int main() {

    //int16_t fds[2] = {0,1};
    // createNewProcess("IDLE", *idleFunction, NULL, AVERAGE_PRIORITY, fds); //IDLE
    //createNewProcess("SHELL", *startShell, NULL, AVERAGE_PRIORITY, fds); //SHELL
    startShell();
	return 0xDEADBEEF;
}
