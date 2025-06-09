#include "../include/syscall.h"
#include "../include/test_util.h"
#include "../include/globalLib.h"
#include <stdlib.h>


#define MAX_BLOCKS 128
#define BLOCK_SIZE 0x1000

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

static void memset(void * address, uint32_t c, uint32_t length){
  uint8_t character = ( uint8_t ) c;
	char * dst = ( char *) address;
	while ( length-- )
		dst[length] = character;
}

char aux[20];

void breakpoint(){
  return;
}

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1){
    printf("Failed argcs");
    NewLine();
    bussy_wait(100000000);
    return -1;
  }


  if ((max_memory = satoi(argv[0])) <= 0){
    printf("Failed max mem");
    NewLine();
    bussy_wait(100000000);
    return -1;
  }


  while (1) {
    rq = 0;
    total = 0;

    breakpoint();

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = allocMemoryUser(mm_rqs[rq].size);
      uint64_t block = getCurrentBlock();
      itoaBase(block, aux, 10);
      NewLine();
      printf("BLOCK: ");
      NewLine();
      printf(aux);
      NewLine();      
      printf("ALLOC SUCCESS");
      NewLine();
      itoaBase((uint64_t)mm_rqs[rq].address, aux,16);
      printf(aux);
      NewLine();
      bussy_wait(100000000);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("Failed Memory Test");
          NewLine();
          itoaBase((uint64_t)mm_rqs[i].address, aux,16);
          bussy_wait(100000000);
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address){
          freeMemoryUser(mm_rqs[rq-i].address);
          NewLine();
          uint64_t block = getCurrentBlock();
      itoaBase(block, aux, 10);
      NewLine();
      printf("BLOCK: ");
      NewLine();
      printf(aux);
      NewLine(); 
          printf("FREE");
          NewLine();
          itoaBase((uint64_t)mm_rqs[rq-i].address, aux,16);
          printf(aux);
          NewLine();
          bussy_wait(100000000);
      }
  }
}
