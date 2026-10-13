/*
 * vectstore.h
 * CPE-2600 Lab 7 - Vector Calculator cont.
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Storage layer: manages the array of up to MAX_VECTS vectors.
 * No console I/O belongs in this layer.
 */

#ifndef VECTSTORE_H
#define VECTSTORE_H

#include "vect.h"

/*
 * Adds a new vector to the store.
 * Returns 1 on success, -1 if out of memory.
 */
int store_add(vect v);

/*
 * Finds a vector by name.
 * Returns 1 on success, 0 if not found.
 */
int store_find(const char *name, vect *output);

/*
 * Get the vector at the given index.
 * Returns 1 on success, 0 if index is out of bounds or empty.
 */
int store_get(int index, vect *output);

/*
 * Frees memory used by the store and clears all vectors.
 */
void store_clear(void);

/*
 *  gets number of vectors that can be stored.
 */
int store_count(void);

/*
 * Loads vectors from a file.
 * Returns 1 on success, 0 on failure.
 */
int store_load(const char *filename, int *bad_lines, int *first_bad);

/*
 * Saves vectors to a file.
 * Returns 1 on success, 0 on failure.
 */
int store_save(const char *filename);

#endif // VECTSTORE_H