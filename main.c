#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "global.h"
#include "vector.h"

#include "deer-workers.h"
#include "elf-workers.h"
#include "santa-worker.h"

uint8_t us_thread_ready = 0;

Vector vec_available_deers;
Vector vec_elfs_needing_help;

pthread_mutex_t mtx_deers_vector;
pthread_mutex_t mtx_elfs_vector;

uint8_t us_deers_trigger = 0;
uint8_t us_elfs_trigger = 0;

void spin_one(pthread_t *idx, void *func) {
  *idx = 0;

  us_thread_ready = 0;

  // TODO: Error handling
  pthread_create(idx, NULL, func, NULL);
}

void spin_many(pthread_t *idx, void *func, int count) {
  memset(idx, 0, sizeof(pthread_t) * count);

  for (int i = 0; i < count; i++) {
    us_thread_ready = 0;

    // TODO: Error handling
    pthread_create(&idx[i], NULL, func, &i);

    // Wait for tread the signal that its ready
    while (!us_thread_ready) {
    }
  }
}

int main() {
  printf("Hello, Cmake!\n");

  vec_available_deers = vector_create(MAX_DEERS);
  vec_elfs_needing_help = vector_create(MAX_ELFS_TO_HANDLE);

  pthread_mutex_init(&mtx_elfs_vector, NULL);
  pthread_mutex_init(&mtx_deers_vector, NULL);

  pthread_t elf_thread_idx[ELF_THREAD_COUNT];
  spin_many(elf_thread_idx, elfs_worker, ELF_THREAD_COUNT);

  pthread_t deer_thread_idx[DEER_THREAD_COUNT];
  spin_many(deer_thread_idx, deer_worker, DEER_THREAD_COUNT);

  pthread_t santa_thread_idx;
  spin_one(&santa_thread_idx, santa_worker);

  while (1) {
    // NOTE: yeal the current thread
    usleep(U_TO_MS(10));
  }

  return 0;
}
