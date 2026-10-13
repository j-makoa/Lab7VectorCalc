/*
 * ui.c
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Reads a line with fgets, splits it into tokens with strtok, then
 * decides what to do based on the number and kind of tokens.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "vectstore.h"
#include "vect.h"

#define LINE_LEN 256
#define DELIMS " ,\t\n"

/* helper methods for the loop */

// Checks if a string represents a number and stores it in *value if true. Returns 1 if true, 0 otherwise.
static int is_number(const char *str, double *value)
{
    char *endptr;
    *value = strtod(str, &endptr);
    if (*endptr != '\0' || str == endptr) {
        return 0;
    }
    return 1;
}

// Prints a vector in the format "name = x y z"
static void print_vect(vect v)
{
    printf("%s = %g %g %g\n", v.name, v.x, v.y, v.z);
}

/* helper methods for commands */

// Saves a vector to the store and prints it
static void save_and_print(vect v, const char *name)
{
    strncpy(v.name, name, VECT_NAME_LENGTH - 1);
    v.name[VECT_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    print_vect(v);
    if (store_add(v) != 1) {
        printf("Error: Vector storage is full. Cannot save vector '%s'.\n", name);
    }
}

// Handles the "list" command by printing all stored vectors.
static void handle_list(void)
{
    vect v;
    for (int i = 0; i < MAX_VECTS; i++) {
        if (store_get(i, &v) == 1) {
            print_vect(v);
        }
    }
}

// Handles the "display" command by printing a specific vector.
static void handle_display(const char *name)
{
    vect v = {0};
    if (store_find(name, &v) == 1) {
        print_vect(v);
    }
    else {
        printf("Vector '%s' not found.\n", name);
    }
}

/*
* varname = x y z (n = 5)
* varname = x y   (n = 4, z = 0)
*/
static void handle_assign(const char *name, const char *xs, const char *ys, const char *zs)
{
    vect v = {0};

    if (!is_number(xs, &v.x)) {
        printf("Invalid x value: %s\n", xs);
        return;
    }
    if (!is_number(ys, &v.y)) {
        printf("Invalid y value: %s\n", ys);
        return;
    }
    if (zs != NULL && !is_number(zs, &v.z)) {
        printf("Invalid z value: %s\n", zs);
        return;
    }
    save_and_print(v, name);
}

/*
* evalutate: lhs op rhs where op is +, -, * (dot product), x (cross product)
*/
static int do_operation(const char *lhs, const char *op, const char *rhs, vect *result)
{
    vect a, b;
    double k;

    if (strcmp(op, "*") == 0) 
    {
        if (is_number(rhs, &k) && store_find(lhs, &a) == 1)
        {
            *result = vect_scale(a, k);
            return 1;
        }
        if (is_number(lhs, &k) && store_find(rhs, &b) == 1)
        {
            *result = vect_scale(b, k);
            return 1;
        }
        printf("Scalar multiplication requires one vector and one number.\n");
        return 0;
    }
    if (strcmp(op, "+") != 0 && strcmp(op, "-") != 0)
    {
        printf("Unknown operator: %s\n", op);
        return 0;
    }
    if (store_find(lhs, &a) != 1)
    {
        printf("Vector '%s' not found.\n", lhs);
        return 0;
    }
    if (store_find(rhs, &b) != 1)
    {
        printf("Vector '%s' not found.\n", rhs);
        return 0;
    }
    if (strcmp(op, "+") == 0) 
    {
        *result = vect_add(a, b);
    } 
    else 
    {
        *result = vect_subtract(a, b);
    }
    return 1;
}

static int handle_line(const char *t1, const char *t2, const char *t3, const char *t4, const char *t5, int n)
{
    if (n == 0) 
    {
        return 0;
    }

    if (n == 1) {
        if(strcmp(t1, "quit") == 0)
        {
            return 1;
        }
        else if(strcmp(t1, "clear") == 0)
        {
            store_clear();
        }
        else if(strcmp(t1, "list") == 0)
        {
            handle_list();
        }
        else {
            handle_display(t1);
        }
        return 0;
    }
        if (strcmp(t2, "=") == 0) 
        {
        vect result;
        if (n == 5 && (strcmp(t4, "+") == 0 || strcmp(t4, "-") == 0 || strcmp(t4, "*") == 0)) 
        {
            if (do_operation(t3, t4, t5, &result) == 1) // c = a + b
            {   
                save_and_print(result, t1);
            }
        } 
        else if (n >= 4) 
        {
            handle_assign(t1, t3, t4, t5); // a = 1 2 3 or a = 1 2
        } 
        else 
        {
            printf("Invalid assignment.\n");
        }
        return 0;
    }

    if (n == 3) 
    {
        vect result;
        if (do_operation(t1, t2, t3, &result) == 1) // a + b -> ans
        {
            save_and_print(result, "ans");
        }
        return 0;
    }

    printf("Unrecognized command.\n");
    return 0;
}

/* public methods */

void ui_run(void)
{
    char line[LINE_LEN];

    while (1) {
        printf("myvectorcalc> ");
        fflush(stdout);

        if (fgets(line, sizeof line, stdin) == NULL)
        {
            printf("\n"); // end of line
            break;
        }

        char *t1 = strtok(line, DELIMS);
        char *t2 = strtok(NULL, DELIMS);
        char *t3 = strtok(NULL, DELIMS);
        char *t4 = strtok(NULL, DELIMS);
        char *t5 = strtok(NULL, DELIMS);
        char *extra = strtok(NULL, DELIMS);

        if (extra != NULL) {
            printf("Extra input detected: %s\n", extra);
            continue;
        }

        int n = (t1 != NULL) + (t2 != NULL) + (t3 != NULL) + (t4 != NULL) + (t5 != NULL);
        if (handle_line(t1, t2, t3, t4, t5, n))
        {
            break;
        }
    }
}

void ui_help(void)
{
    printf("Usage: <command> [arguments]\n");
    printf("Commands:\n");
    printf("  a = 1 2 3        create or replace vector a (z = 0 if left out)\n");
    printf("  a                display vector a\n");
    printf("  a + b            add two vectors (result saved as ans)\n");
    printf("  a - b            subtract two vectors (result saved as ans)\n");
    printf("  a * 2            scalar multiply, also works as 2 * a (saved as ans)\n");
    printf("  c = a + b        do +, -, or * and save the result as c\n");
    printf("  list             show all stored vectors\n");
    printf("  clear            delete all stored vectors\n");
    printf("  quit             exit the program\n");
}