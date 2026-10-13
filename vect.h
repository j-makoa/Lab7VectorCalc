/*
 * vect.h
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Vector type and math operations (inner layer).
 * No console I/O belongs in this layer.
 */

#ifndef VECT_H
#define VECT_H

#define VECT_NAME_LENGTH 16

typedef struct {
    char name[VECT_NAME_LENGTH];
    double x;
    double y;
    double z;
} vect;

// Vector math operations
vect vect_add(vect a, vect b);
vect vect_subtract(vect a, vect b);
vect vect_scale(vect a, double k);

// extra credit (decide if worth)
double vect_dot(vect a, vect b);
vect vect_cross(vect a, vect b);

#endif // VECT_H