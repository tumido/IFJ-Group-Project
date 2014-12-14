/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  10/17/2014 06:10:50 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef INTERPRET_INCLUDED
#define INTERPRET_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "strings.h"
#include "io.h"
#include "ial.h"
#include "ilist.h"
#include "stack.h"

// typy jednotlivych instrukci
#define I_READ 4
#define I_WRITE 5
#define I_JUMP 6
#define I_ASSIGN 7
#define I_CALL_FUNCTION 8
#define I_CLEAR 9
#define I_MUL 10
#define I_DIV 11
#define I_ADD 12
#define I_SUB 13
#define I_CON 14
#define I_INC 15
#define I_DEC 16
#define I_NEG 17
#define I_LESS 18
#define I_GREATER 19
#define I_LESS_EQUAL 20
#define I_GREATER_EQUAL 21
#define I_EQUAL 22
#define I_NOT_EQUAL 23
#define I_COPY 24
#define I_LENGHT 25
#define I_FIND 26
#define I_SORT 27
#define I_TABLE_BACKUP 28
#define I_TABLE_RESTORE 29
/*
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 */
int interpret(tListOfInstr *  I);
#endif
