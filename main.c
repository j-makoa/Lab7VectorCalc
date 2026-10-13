/*
 * main.c
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Handles the -h option, then hands off to the user interface.
 * Compile: make
 */
#include <stdio.h>
#include <string.h>
#include "ui.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        ui_help();
        return 0;
    }

    if (argc > 1) {
        fprintf(stderr, "Unknown option, Try: %s or -h\n", argv[0]);
        return 1;
    }

    ui_run();
    return 0;
}