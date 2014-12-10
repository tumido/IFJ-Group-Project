/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  10/17/2014 06:10:50 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
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
#include "ial.h" // kvuli funkci findSubstring a shellSort
#include "ilist.h"

// datove typy
#define T_INTEGER 0
#define T_REAL 1
#define T_BOOLEAN 2
#define T_STRING 3

// typy jednotlivych instrukci
#define I_STOP 4
#define I_READ 5
#define I_WRITE 6
#define I_IF 7
#define I_IF_END 8
#define I_JUMP 9
#define I_LABEl 10
#define I_ASSIGN 11
#define I_SUBSTR 12
#define I_CALL_FUNCTION 13
#define I_RETURN 14
#define I_DO_WHILE 15
#define I_MUL 16
#define I_DIV 17
#define I_ADD 18
#define I_SUB 29
#define I_CON 20
#define I_INC 21
#define I_DEC 22
#define I_NEG 23
#define I_LESS 24
#define I_GREATER 25
#define I_LESS_EQUAL 26
#define I_GREATER_EQUAL 27
#define I_EQUAL 28
#define I_NOT_EQUAL 29
#define I_TYPE 30
#define I_COPY 31
#define I_SORT 32
#define I_CLEAN 33


/*
 * Klasifikace datového typu
 */
int returnValue(tVarValue *destination, tVarValue *source);

/*
 * Interpretace konkretni instrukce
 */
int instruction(tSymbolTable *ST, tListOfInstr *instrList);

/*
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 */
int interpret(tInstList *);

/*
 * Funkce na kontrolu typu integer a real v operandech 3AK
 */
bool isIntOrReal(void);

/*
 * Funkce vykonava volani WRITE - zapisuje na stdout
 */
int iWrite(tVarValue *source);

/*
 * Funkce vykonava volani READ - cte ze stdin
 */
int iRead(tListOfInstr *L);

/*
 *Funkce vypise na stdout data ulozena na pozici instruction->addr1
 */
int iWrite(tListOfInstr *L);
#endif
