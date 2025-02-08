#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "deer-workers.h"
#include "global.h"
#include "vector.h"

extern uint8_t us_thread_ready;
extern uint8_t us_deers_trigger;

extern Vector vec_available_deers;

extern pthread_mutex_t mtx_deers_vector;

void *deer_worker(void *p) {
  int index = *(int *)p;

  us_thread_ready = 1;

  uint8_t is_home = 0;
  uint8_t is_waiting_for_santa = 0;

  time_t last_report = time(NULL);

  while (1) {
    double ellapsed = time(NULL) - last_report;

    int value = rand() % (DEER_COMMING_HOME_CHANCE + 1);

    if (is_home == 0 && DEER_COMMING_HOME_CHANCE == value)
      is_home = 1;

    if (is_home) {
      pthread_mutex_lock(&mtx_deers_vector);

      int size = vector_get_size(vec_available_deers);

      // Check if we are already getting help
      uint8_t are_we_in = 0;
      for (int i = 0; i < size; i++) {
        int idx = *(int *)vector_get_item_at(vec_available_deers, i);
        if (idx == index) {
          are_we_in = 1;
          break;
        }
      }

      if (is_waiting_for_santa && !are_we_in) {
        is_home = 0;
        is_waiting_for_santa = 0;
      } else if (!are_we_in) {
        printf("Deer [%i] is ready!\n", index);

        // We are not in. Lets insert ourself in
        const VectorItem self_idx = &index;
        vector_put_item(vec_available_deers, self_idx);

        is_waiting_for_santa = 1;

        // If we were the last one in, trigger santa
        int newSize = vector_get_size(vec_available_deers);
        if (MAX_DEERS == newSize)
          us_deers_trigger = 1;
      } else if (ellapsed > 5.0) {
        printf("Deer [%i] is waiting for santa!\n", index);
        last_report = time(NULL);
      }

      pthread_mutex_unlock(&mtx_deers_vector);
    }

    usleep(U_TO_MS(10));
  }

  return NULL;
}
