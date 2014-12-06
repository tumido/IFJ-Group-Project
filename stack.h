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
/*
 * Zasobnik
 * -----------------------------------------------------------------------------
 * implemetace pomoci linearniho seznamu
 */

/*
 * samotny prvek seznamu, bude obsahovat vsechnz mozne typy vysledku, nepouzite typy dostanou NULL
 * promena boolResult nahrazuje typ bool, nebot mi prijde jednodusi pracovat s integerem 1 TRUE nebo 0 FALSE
 * Roman se pta - proc? V cem je to jednodussi? Kdyztak bych bral jako 0 - TRUE a 1 - FALSE :)
 */
typedef struct stackPtr
{
  int iResult;
  float fResult;
  int boolResult;
  char cResult; // zde si presne nejsem jist, jestli by nebylo lepsi pouit pole
  struct stackPtr *next; // ukazatel na nasledujici prvek v zasobniku
} stackElem;

typedef struct
{
  stackElem * top; // ukazatel na vrchol zasobniku
  int numberOfResults; // pocet prvku v zasobniku
} stack;

/*
 * Inicializace zasobniku, ocekavam ukazatel na dany zasobnik
 */
void stackInit(stack *S);

/*
 * PUSHnuti vysledku (podle typu) na zasobnik
 */
int stackPUSHInt(stack *S, int i);
int stackPUSHReal(stack *S, double f);
int stackPUSHBool(stack *S, int i);
int stackPUSHChar(stack *S, char *c); //opet, u charu si nejsem jist spravnosti

/*
 * POPnuti vysledku ze zasobnku, bude vracet celou strukturu
 */
int stackPOP(stack *S, struct stackPtr * result);

/*
 * Zjisti zdali je stack prazdny. Pokud je prazdny vraci 0 jinak 1
 */
int stackEmpty(stack *S);
#endif
