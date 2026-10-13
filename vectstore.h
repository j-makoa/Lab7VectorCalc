/*
 * vectstore.h
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Storage layer: manages the array of up to MAX_VECTS vectors.
 * No console I/O belongs in this layer.
 */

#ifndef VECTSTORE_H
#define VECTSTORE_H

#include "vect.h"

#define MAX_VECTS 10

/*
 * Adds a new vector to the store.
 * Returns 1 on success, 0 if the store is full.
 */
int store_add(vect v);

/*
 * Finds a vector by name.
 * Returns 1 on success, 0 if not found.
 */
int store_find(const char *name, vect *output);

/*
 * Gets a vector by index.
 * Returns 1 on success, 0 if the index is out of range or empty.
 */
int store_get(int index, vect *output);

/*
 * Clears all vectors from the storage.
 */
void store_clear(void);

#endif // VECTSTORE_H