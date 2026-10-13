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

static int set_name(vect *v, const char *name)
{
    double tmp;
    if (is_number(name, &tmp)) 
    {
        printf("Error: Name cannot be a number: %s\n", name);
        return 0; // Name cannot be a number
    }
    if (strlen(name) >= sizeof(v->name)) 
    {
        printf("Error: Name too long: %s\n", name);
        return 0; // Name too long
    }
    strcpy(v->name, name);
    return 1; // Name set successfully
}

static int is_operator(const char *str)
{
    return (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "*") == 0);
}

static void store_and_print(vect v, const char *name)
{
    if (!set_name(&v, name)) 
    {
        return; // Invalid name
    }
    int saved = (store_add(v) == 1);
    print_vect(v);
    if (!saved) 
    {
        printf("Error: Vector storage is full. Cannot save vector '%s'.\n", name);
    }
}

/* helper methods for commands */

// Handles the "list" command by printing all stored vectors.
static void handle_list(void)
{
    int n = store_count();
    if (n == 0) 
    {
        printf("No vectors stored.\n");
        return;
    }
    for (int i = 0; i < n; i++) 
    {
        vect v;
        if (store_get(i, &v) == 1) 
        {
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
    store_and_print(v, name);
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

static void handle_load(const char *filename)
{
    int bad_lines = 0;
    int first_bad = -1;
    int loaded = store_load(filename, &bad_lines, &first_bad);
    if (loaded == -1)
    {
        printf("Error: Could not open file '%s'.\n", filename);
    } 
    else if (loaded == -2) 
    {
        printf("Error: Out of memory while loading vectors from '%s'.\n", filename);
    } 
    else 
    {
        printf("Loaded %d vectors from '%s'.\n", loaded, filename);
        if (bad_lines > 0) 
        {
            printf("Warning: %d bad lines encountered. First bad line: %d\n", bad_lines, first_bad);
        }
    }
}

static void handle_save(const char *filename)
{
    int saved = store_save(filename);
    if (saved < 0) 
    {
        printf("Error: Could not open file '%s' for writing.\n", filename);
    } 
    else 
    {
        printf("Saved %d vectors to '%s'.\n", saved, filename);
    }
}

static void handle_fill(const char *count_str)
{
    double d;
    if (!is_number(count_str, &d) || d < 1 || d != (int)d) 
    {
        printf("Error: Invalid count for fill: %s\n", count_str);
        return;
    }

    int n = (int)d;
    for (int i = 1; i <= n; i++) 
    {
        vect v = {0};
        snprintf(v.name, sizeof(v.name), "f%d", i);
        v.x = i;
        v.y = i * 2;
        v.z = i * 3;
        if (store_add(v) != 1) 
        {
            printf("Error: Out of memory while filling vectors.\n");
            return;
        }
    }
    printf("Filled %d vectors.\n", n);
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
            return 0;
        }
        else if(strcmp(t1, "list") == 0)
        {
            handle_list();
            return 0;
        }
        else if (strcmp(t1, "load") == 0 || strcmp(t1, "save") == 0 || strcmp(t1, "fill") == 0)
        {
            printf("Usage: %s <%s>\n", t1, strcmp(t1, "fill") == 0 ? "count" : "filename");
            return 0;
        }
        handle_display(t1);
        return 0;
        }

    if (n == 2)
    {
        if (strcmp(t1, "load") == 0) 
        {
            handle_load(t2);
            return 0;
        }
        if (strcmp(t1, "save") == 0) 
        {
            handle_save(t2);
            return 0;
        }
        if (strcmp(t1, "fill") == 0) 
        {
            handle_fill(t2);
            return 0;
        }
    }

    if (n >= 4 && strcmp(t2, "=") == 0)
    {
        if (n == 5 && is_operator(t4))
        {
            vect result;
            if (do_operation(t3, t4, t5, &result)) // c = a + b
            {   
                store_and_print(result, t1);
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
            store_and_print(result, "ans");
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