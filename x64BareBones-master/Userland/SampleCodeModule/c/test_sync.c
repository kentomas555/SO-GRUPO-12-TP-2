#include <stdint.h>

#include "../include/test_util.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  //Declare YIELD
  yield();
  aux += inc;
  *p = aux;
}

uint64_t myProcessInc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  printf("Entered to Process Inc");
  NewLine();

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  char aux[20];
  int mySemaphore = semInit(SEM_ID,1);
        itoaBase(mySemaphore, aux,10);
      printf(aux);
      NewLine();
  if (use_sem)
    if (!mySemaphore) {
      printf("test_sync: ERROR opening semaphore\n");
      NewLine();

      itoaBase(getpid(), aux,10);
      printf(aux);
      NewLine();

      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      semWait(mySemaphore);
    slowInc(&global, inc);
    if (use_sem)
      semPost(mySemaphore);
  }

  printf("Exiting process Inc");
  NewLine();
  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  int16_t fd[] = {0, 1};
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;
  printf("******Creating processes*****");
  NewLine();
  
  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = createNewProcess("My Process Inc", myProcessInc, argvDec, LOW_PRIO, fd);
    pids[i + TOTAL_PAIR_PROCESSES - 1] = createNewProcess("My Process Inc", myProcessInc, argvInc, LOW_PRIO, fd);
      char aux[20];
    itoaBase(pids[i], aux,10);
    printf(aux);
    NewLine();
    itoaBase(pids[i + TOTAL_PAIR_PROCESSES - 1], aux,10);
    printf(aux);
    NewLine();
  }

  printf("*****Processes created*****");
  NewLine();

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {

    waitPID(pids[0]);
    waitPID(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  NewLine();
  printf("Destroying sem");
  semDestroy(SEM_ID);
  NewLine();
  NewLine();

  printf("Final value: ");
  nextX(13);
  char aux[20];
  itoaBase(global, aux,10);
  printf(aux);
  NewLine();
  NewLine();
  return 0;
}
