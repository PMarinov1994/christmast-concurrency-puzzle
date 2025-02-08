#include "vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define COOKIE 0x69

typedef struct {
  uint8_t cookie;
  VectorItem *data;
  int size;
  int capacity;
} Internal_Vector;

const Vector vector_create(const int capacity) {
  Internal_Vector *vector = calloc(capacity, sizeof(Internal_Vector));

  // TODO: This could be optimized to dinamically allocate the size, but its
  // fine for now.
  vector->cookie = COOKIE;
  vector->data = calloc(capacity, sizeof(VectorItem));
  vector->size = 0;
  vector->capacity = capacity;

  return (Vector)vector;
}

void vector_destroy(const Vector v) {
  Internal_Vector *vector = (Internal_Vector *)v;

  if (vector->cookie != COOKIE) {
    assert(0);
  }

  free(vector->data);
}

VectorItem vector_put_item(const Vector v, const VectorItem item) {
  Internal_Vector *vector = (Internal_Vector *)v;

  if (vector->cookie != COOKIE) {
    assert(0);
  }

  if (vector->size == vector->capacity) {
    // No space left
    return NULL;
  }

  vector->data[vector->size] = item;
  vector->size++;

  return item;
}

VectorItem vector_pop_item(const Vector v) {
  Internal_Vector *vector = (Internal_Vector *)v;

  if (vector->cookie != COOKIE) {
    assert(0);
  }

  if (!vector->size) {
    // No items to pop
    return NULL;
  }

  VectorItem item = vector->data[vector->size - 1];
  vector->size--;

  return item;
}

VectorItem vector_get_item_at(const Vector v, int index) {
  Internal_Vector *vector = (Internal_Vector *)v;

  if (vector->cookie != COOKIE) {
    assert(0);
  }

  if (index < 0 || index >= vector->size) {
    // Index out of bound
    return NULL;
  }

  return vector->data[index];
}

int vector_get_size(const Vector v) {
  Internal_Vector *vector = (Internal_Vector *)v;

  if (vector->cookie != COOKIE) {
    assert(0);
  }

  return vector->size;
}
