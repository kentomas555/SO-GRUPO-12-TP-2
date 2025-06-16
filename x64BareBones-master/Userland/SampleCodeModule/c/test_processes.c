// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/test_util.h"

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[]) {

  printf("Process Test Started Running");
  NewLine();

  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;

  int16_t fds[2] = {0,1};
  char *argvAUX[] = {NULL};

  if (argc != 1){
     NewLine();
    printf("argc != 1");
    NewLine();
    return -1;

  }
   
  if ((max_processes = satoi(argv[0])) <= 0){
    NewLine();  
    printf("SATOI");
    NewLine();
    return -1;
  }
    

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = createNewProcess("endless_loop", endless_loop, argvAUX,LOWEST_PRIO, fds);

      if (p_rqs[rq].pid == -1) {
        printf("test_processes: ERROR creating process");
        NewLine();
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (killProcess(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR killing process");
                NewLine();
                return -1;
              }
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (blockProcess(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR blocking process");
                NewLine();
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (unblockProcess(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process");
            NewLine();
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
