/*
 * vectstore.c
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Vector storage and management (middle layer).
 */

#include <string.h>
#include "vectstore.h"

static vect vectors[MAX_VECTS];

int store_add(vect v) 
{
    for (int i = 0; i < MAX_VECTS; i++) {
        if (strcmp(vectors[i].name, v.name) == 0)
        {
            vectors[i] = v;
            return 1;
        }
    }
    for (int i = 0; i < MAX_VECTS; i++) {
        if (vectors[i].name[0] == '\0') {
            vectors[i] = v;
            return 1;
        }
    }
    return 0; // Storage is full
}

int store_find(const char *name, vect *output)
{
    for (int i = 0; i < MAX_VECTS; i++) {
        if (strcmp(vectors[i].name, name) == 0) {
            *output = vectors[i];
            return 1;
        }
    }
    return 0; // Not found
}

int store_get(int index, vect *output)
{
    if (index < 0 || index >= MAX_VECTS) {
        return 0; // Invalid index
    }
    if (vectors[index].name[0] == '\0') {
        return 0; // Empty slot
    }
    *output = vectors[index];
    return 1;
}

void store_clear(void)
{
    for (int i = 0; i < MAX_VECTS; i++) {
        vectors[i].name[0] = '\0';
    }
}