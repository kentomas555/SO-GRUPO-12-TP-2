// #include <stdint.h>

// #include "../include/test_util.h"

// #define SEM_ID "sem"
// #define TOTAL_PAIR_PROCESSES 2

// int64_t global; // shared memory

// void slowInc(int64_t *p, int64_t inc) {
//   uint64_t aux = *p;
//   //Declare YIELD
//   yield();
//   aux += inc;
//   *p = aux;
// }

// uint64_t myProcessInc(uint64_t argc, char *argv[]) {
//   uint64_t n;
//   int8_t inc;
//   int8_t use_sem;

//   printf("Entered to function from Sync Test");
//   NewLine();

//   if (argc != 3)
//     return -1;

//   if ((n = satoi(argv[0])) <= 0)
//     return -1;
//   if ((inc = satoi(argv[1])) == 0)
//     return -1;
//   if ((use_sem = satoi(argv[2])) < 0)
//     return -1;

//   int mySemaphore = semInit(1);
//   if (use_sem)
//     if (!mySemaphore) {
//       printf("test_sync: ERROR opening semaphore\n");
//       return -1;
//     }

//   uint64_t i;
//   for (i = 0; i < n; i++) {
//     if (use_sem)
//       semWait(mySemaphore);
//     slowInc(&global, inc);
//     if (use_sem)
//       semPost(mySemaphore);
//   }

//   if (use_sem)
//     semDestroy(mySemaphore);

//   return 0;
// }

// uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
//   int16_t fd[] = {0, 1};
//   uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

//   if (argc != 2)
//     return -1;

//   char *argvDec[] = {argv[0], "-1", argv[1], NULL};
//   char *argvInc[] = {argv[0], "1", argv[1], NULL};

//   global = 0;

//   uint64_t i;
//   for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//     pids[i] = createNewProcess("My Process Inc", myProcessInc, argvDec, LOW_PRIO, fd);
//     pids[i + TOTAL_PAIR_PROCESSES] = createNewProcess("My Process Inc", myProcessInc, argvInc, LOW_PRIO, fd);
//   }

//   for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//     semWait(pids[i]);
//     semWait(pids[i + TOTAL_PAIR_PROCESSES]);
//   }

//   printf("Final value: ");
//   nextX(13);
//   char aux[20];
//   itoaBase(global, aux,10);
//   printf(aux);
//   NewLine();
//   NewLine();
//   return 0;
// }
