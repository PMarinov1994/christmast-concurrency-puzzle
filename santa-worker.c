#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "santa-worker.h"
#include "vector.h"

extern uint8_t us_thread_ready;

extern Vector vec_available_deers;
extern Vector vec_elfs_needing_help;

extern uint8_t us_deers_trigger;
extern uint8_t us_elfs_trigger;

extern pthread_mutex_t mtx_deers_vector;
extern pthread_mutex_t mtx_elfs_vector;

void *santa_worker(void *p) {
  us_thread_ready = 1;

  time_t last_report = time(NULL);

  while (1) {
    double ellapsed = time(NULL) - last_report;

    pthread_mutex_lock(&mtx_deers_vector);
    pthread_mutex_lock(&mtx_elfs_vector);

    if (us_deers_trigger) {
      printf("=== Going out with the deers! ===\n");

      int count = vector_get_size(vec_available_deers);
      for (int i = 0; i < count; i++)
        vector_pop_item(vec_available_deers);

      assert(vector_get_size(vec_available_deers) == 0);

      us_deers_trigger = 0;

    } else if (us_elfs_trigger) {
      printf("Elfs need help!\n");

      int count = vector_get_size(vec_elfs_needing_help);
      for (int i = 0; i < count; i++) {
        int elf_idx = *(int *)vector_pop_item(vec_elfs_needing_help);
        printf("Helping elf %d\n", elf_idx);
      }

      assert(vector_get_size(vec_elfs_needing_help) == 0);

      us_elfs_trigger = 0;

    } else if (ellapsed > 1.0) {
      printf("Santa is sleeping...\n");
      last_report = time(NULL);
    }

    pthread_mutex_unlock(&mtx_deers_vector);
    pthread_mutex_unlock(&mtx_elfs_vector);

    usleep(U_TO_MS(10));
  }

  return NULL;
}
