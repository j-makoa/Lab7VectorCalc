/*
 * ui.h
 * CPE-2600 Lab 5 - Vector Calculator
 * Date: 9/29/2026
 * Author: Jim Luzano-Belfield
 *
 * User interface layer: all console input and output lives here.
 */

#ifndef UI_H
#define UI_H

// run the main user interface loop until quit
void ui_run(void);

// print help message for the user interface (with -h)
void ui_help(void);

#endif // UI_H