/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  10/17/2014 06:10:50 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef IO_INCLUDED
#define IO_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define EXIT_SUCCESS 0
#define EXIT_LEXICAL_ERROR 1
#define EXIT_PARSER_ERROR 2
#define EXIT_NOT_DEFINED_ERROR 3
#define EXIT_TYPE_ERROR 4
#define EXIT_SEMANTIC_ERROR 5
#define EXIT_READ_STDIN_ERROR 6
#define EXIT_NOT_INIT_ERROR 7
#define EXIT_DIVISION_BY_ZERO_ERROR 8
#define EXIT_RUNTIME_ERROR 9
#define EXIT_INTERNAL_ERROR 99

void printErr(const char * msg, ...);

#endif
