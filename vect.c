/*
 * vect.c
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Vector math operations. Structs are passed and returned by value.
 */

#include "vect.h"

vect vect_add(vect a, vect b) 
{
    vect result = {0};
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vect vect_subtract(vect a, vect b)
{
    vect result = {0};
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vect vect_scale(vect a, double k)
{
    vect result = {0};
    result.x = a.x * k;
    result.y = a.y * k;
    result.z = a.z * k;
    return result;
}

/*
 * vect_dot: Computes the dot product of two vectors.
 */
double vect_dot(vect a, vect b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 * vect_cross: Computes the cross product of two vectors.
 */
vect vect_cross(vect a, vect b)
{
    vect result = {0};
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}