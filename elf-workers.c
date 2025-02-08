#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "elf-workers.h"
#include "global.h"
#include "vector.h"

extern uint8_t us_thread_ready;
extern uint8_t us_elfs_trigger;

extern Vector vec_elfs_needing_help;

extern pthread_mutex_t mtx_elfs_vector;

void *elfs_worker(void *p) {
  int index = *(int *)p;

  us_thread_ready = 1;

  uint8_t us_need_help = 0;
  uint8_t us_getting_help = 0;

  time_t last_report = time(NULL);

  while (1) {
    double ellapsed = time(NULL) - last_report;

    int value = rand() % (ELF_NEEDS_HELP_CHANCE + 1);

    if (us_need_help == 0 && ELF_NEEDS_HELP_CHANCE == value)
      us_need_help = 1;

    if (us_need_help) {
      pthread_mutex_lock(&mtx_elfs_vector);

      int size = vector_get_size(vec_elfs_needing_help);

      // Check if we are already getting help
      uint8_t us_are_we_in = 0;
      for (int i = 0; i < size; i++) {
        int idx = *(int *)vector_get_item_at(vec_elfs_needing_help, i);
        if (idx == index) {
          us_are_we_in = 1;
          break;
        }
      }

      if (us_getting_help && !us_are_we_in) {
        // Are we solved?
        us_need_help = 0;
        us_getting_help = 0;
      } else if (MAX_ELFS_TO_HANDLE > size && !us_are_we_in) {
        printf("Elf [%i] requesting help!\n", index);

        // We are not in. Lets insert ourself in
        const VectorItem self_idx = &index;
        vector_put_item(vec_elfs_needing_help, self_idx);

        us_getting_help = 1;

        // If we were the last one in, trigger santa
        int newSize = vector_get_size(vec_elfs_needing_help);
        if (MAX_ELFS_TO_HANDLE == newSize)
          us_elfs_trigger = 1;
      } else if (!us_are_we_in && ellapsed > 5.0) {
        printf("Elf [%i] needs help, but no space!\n", index);
        last_report = time(NULL);
      }

      pthread_mutex_unlock(&mtx_elfs_vector);
    }

    usleep(U_TO_MS(10));
  }

  return NULL;
}
