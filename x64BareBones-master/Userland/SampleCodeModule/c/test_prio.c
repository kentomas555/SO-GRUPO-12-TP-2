// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include "../include/globalLib.h"
#include "../include/test_util.h"
#include "../include/syscall.h"

#define TOTAL_PROCESSES 5
#define TEST_PRIO_FD -1

int64_t prio[TOTAL_PROCESSES] = {LOWEST_PRIO, LOW_PRIO, AVERAGE_PRIO, HIGH_PRIO, HIGHEST_PRIO};

void test_prio() {

  int64_t pids[TOTAL_PROCESSES];
  int16_t fds[2] = {TEST_PRIO_FD,1};
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
  pids[i] = createNewProcess("endless_loop_print", endless_loop_print, argv, LOWEST_PRIO, fds);

  bussy_wait(WAIT);
  NewLine();
  printf("CHANGING PRIORITIES...");
  NewLine();

  for (i = 0; i < TOTAL_PROCESSES; i++)
  nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  NewLine();
  printf("BLOCKING...");
  NewLine();

  for (i = 0; i < TOTAL_PROCESSES; i++)
  blockProcess(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...");
  NewLine();

  for (i = 0; i < TOTAL_PROCESSES; i++)
  nice(pids[i], AVERAGE_PRIO);

  NewLine();
  printf("UNBLOCKING...");
  NewLine();

  for (i = 0; i < TOTAL_PROCESSES; i++)
  unblockProcess(pids[i]);

  bussy_wait(WAIT);
  NewLine();
  printf("KILLING...");
  NewLine();

  for (i = 0; i < TOTAL_PROCESSES; i++)
  killProcess(pids[i]);

  printProcesses(0, NULL);

  NewLine();
  printf("TEST PASSED SUCCESFULLY!");
  NewLine();
    printf("$");
    nextX(1);
    printf(">");
    nextX(1);
}
