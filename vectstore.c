/*
 * vectstore.c
 * CPE-2600 Lab 7 - Vector Calculator cont.
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Vector storage and management (middle layer).
 */

#include <string.h>
#include "vectstore.h"

#define START_CAPACITY 4
#define CSV_LINE_LEN 256

static vect *store = NULL; // Pointer to the dynamic array of vectors
static int count = 0; // Current number of vectors in the store
static int capacity = 0; // Current capacity of the store

/* helper methods */

static int find_index(const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(store[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

static int grow_if_full(void)
{
    if (count < capacity) {
        return 0; // No need to grow
    }
    int new_capacity = (capacity == 0) ? START_CAPACITY : capacity * 2;
    vect *bigger = realloc(store, new_capacity * sizeof(vect));
    if (bigger == NULL) {
        return -1; // Out of memory
    }
    store = bigger;
    capacity = new_capacity;
    return 0; // Successfully grown
}

static char *trim(char *s)
{
    while (*s == ' ' || *s == '\t')
    { 
        s++;
    }
    char *end = s + strlen(s);
    while (end > s && (*(end - 1) == ' ' || *(end - 1) == '\t')) 
    {
        end--;
    }
    *end = '\0';
    return s;
}

static int parse_double(char *field, double *output)
{
    char *endptr;
    field = trim(field);
    *output = strtod(field, &endptr);
    return endptr != field && *endptr == '\0'; // Return 1 if valid double, 0 otherwise
}

static int parse_csv_line(const char *line, vect *v)
{
    char *fields[4];
    int n = 0;

    char *p = line;
    fields[n++] = p;
    while (*p != '\0')
    {
        if (*p == ',') 
        {
            *p = '\0';
            if (n == 4) 
            {
                return 0; // Too many fields
            }
            fields[n++] = p + 1;
        }
        p++;
    }
    if (n != 4) 
    {
        return 0; // Not enough fields
    }

    char *name = trim(fields[0]);
    double dummy;
    if (name[0] == '\0' || strlen(name) >= sizeof(v->name) || strchr(name, ' ') != NULL)
    {
        return 0; // Invalid name
    }
    if (parse_double(name, &dummy)) 
    {
        return 0; // Name cannot be a number
    }
    vect temp = {0};
    strncpy(temp.name, name);
    if (!parse_double(fields[1], &temp.x) || !parse_double(fields[2], &temp.y) || !parse_double(fields[3], &temp.z)) 
    {
        return 0; // Invalid number
    }
    *v = temp;
    return 1;
}

/* public methods */

int store_add(vect v) 
{
    int index = find_index(v.name);
    if (index >= 0) 
    {
        store[index] = v; // Overwrite existing vector
        return 1;
    }
    if (grow_if_full() != 0) 
    {
        return -1; // Out of memory
    }
    store[count] = v; // Add new vector
    count++;
    return 1;
}

int store_find(const char *name, vect *output)
{
    int index = find_index(name);
    if (index >= 0) {
        *output = store[index];
        return 1;
    }
    return 0; // Not found
}

int store_get(int index, vect *output)
{
    if (index < 0 || index >= count) {
        return 0; // Invalid index
    }
    *output = store[index];
    return 1;
}

void store_clear(void)
{
    free(store);
    store = NULL;
    count = 0;
    capacity = 0;
}

int store_count(void)
{
    return count;
}

int store_load(const char *filename, int *bad_lines, int *first_bad)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // Failed to open file
    }

    char line[CSV_LINE_LEN];
    int loaded = 0;
    int line_number = 0;
    *bad_lines = 0;
    *first_bad = -1;

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        line_number++;
        if (strchr(line, '\n') == NULL && !feof(file)) 
        {
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF); // Discard the rest of the line
            if (*bad_lines == 0) 
            {
                *first_bad = line_number;
            }
            (*bad_lines)++;
            continue; // Line too long
        }

        line[strcspn(line, "\r\n")] = 0; // Remove newline characters

        char *start = line;
        if (line_number == 1 && strncmp(start, "\xEF\xBF\xBB", 3) == 0) 
        {
            start += 3; // Skip BOM
        }

        if (trim(start)[0] == '\0') 
        {
            continue; // Skip empty lines
        }
        vect v;
        if (!parse_csv_line(start, &v)) 
        {
            if (*bad_lines == 0) 
            {
                *first_bad = line_number;
            }
            (*bad_lines)++;
            continue; // Invalid line
        }
        if (store_add(v) != 1) 
        {
            fclose(file);
            return -2; // Out of memory
        }
        loaded++;
    }

    fclose(file);
    return 1; // Success
}

int store_save(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return -1; // Failed to open file
    }
    
    if (fclose(file) != 0) {
        return -1; // Failed to close file
    }
    return count; // Return number of vectors saved
}