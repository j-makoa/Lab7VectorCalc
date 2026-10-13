/*
 * main.c
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * Handles the -h option, then hands off to the user interface.
 * Compile: make
 */

#define _POSIX_C_SOURCE 200809L // for getline

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "ui.h"
#include "vectstore.h"

void on_sigint(int sig)
{
    (void)sig; // unused parameter
}

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-h") == 0) 
    {
        ui_help();
        return 0;
    }

    if (argc > 1) 
    {
        fprintf(stderr, "Usage: %s [-h]\n", argv[0]);
        return 1;
    }

    atexit(store_clear); // Ensure store is cleared on exit

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    ui_run();
    return 0;
}