#ifndef __VECTOR_H
#define __VECTOR_H

typedef void *Vector;
typedef void *VectorItem;

// Creates a vector
const Vector vector_create(const int capacity);

// Cleans up the vector
void vector_destroy(const Vector v);

// Insert item at the end of the vector.
// Returns the inserted item on success, or NULL if the vector is unchanged.
VectorItem vector_put_item(const Vector v, const VectorItem item);

// Removes the last item of the vector.
// Returns the removed item or NULL if the vector is unchanged.
VectorItem vector_pop_item(const Vector v);

// Retuns the item at index or NULL if not found
VectorItem vector_get_item_at(const Vector v, int index);

// Returns the current element count
int vector_get_size(const Vector v);

#endif // __VECTOR_H
