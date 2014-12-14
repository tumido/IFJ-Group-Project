/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef STACK_INCLUDED
#define STACK_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "btree.h"
/*
 * Zasobnik
 * -----------------------------------------------------------------------------
 * implemetace pomoci linearniho seznamu
 */
typedef struct stackPtr
{
  btree * table;
  struct stackPtr *next; // ukazatel na nasledujici prvek v zasobniku
} stackElem;

typedef struct
{
  stackElem * top; // ukazatel na vrchol zasobniku
} stack;

/*
 * Inicializace zasobniku, ocekavam ukazatel na dany zasobnik
 */
void stackInit(stack *S);

/*
 * PUSHnuti vysledku (podle typu) na zasobnik
 */
int stackPUSH(stack * s, btree *table);

/*
 * POPnuti vysledku ze zasobnku, bude vracet celou strukturu
 */
int stackPOP(stack *S, btree * table, char * key);

/*
 * Zjisti zdali je stack prazdny. Pokud je prazdny vraci 0 jinak 1
 */
int stackEmpty(stack *S);
#endif
