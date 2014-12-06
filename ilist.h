/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  30/12/2014 18:15:58 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef ILIST_INCLUDED
#define ILIST_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "io.h"

// typy jednotlivych instrukci
#define I_STOP    0
#define I_READ    1
#define I_WRITE   2
#define I_SETZERO 3
#define I_INC     4
#define I_DEC     5
#define I_NOT     6
#define I_LAB     7
#define I_IFGOTO  8
#define I_GOTO    9

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;

typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
int listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);

/*
 *  -----ZASOBNIK A FUNKCE PRO PRACI S NIM------
 * momentalnim umistenim si nejsem jist, muzu presunout jinam, treba i do samostatneho souboru (jak .h tak .c)
 */

 /*
  * Zasobnik bude formou linearniho seznamu
  *
  * vlastni seznam
  *  |
  *  |
  *  V
  */

/*
 * samotny prvek seznamu, bude obsahovat vsechnz mozne typy vysledku, nepouzite typy dostanou NULL
 * promena boolResult nahrazuje typ bool, nebot mi prijde jednodusi pracovat s integerem 1 TRUE nebo 0 FALSE
 */

typedef struct stackPtr
{
  int iResult;
  double fResult;
  int boolResult;
  char cResult;             // zde si presne nejsem jist, jestli by nebylo lepsi pouit pole
  struct stackPtr * next;    // ukazatel na nasledujici prvek v zasobniku
} stackElem;

typedef struct
{
  stackElem * top;           // ukazatel na vrchol zasobniku
  int numberOfResults;            // pocet prvku v zasobniku
} stack;
/*
 * Inicializace zasobniku, ocekavam ukazatel na dany zasobnik
 */
void stackInit(stack *S);

/*
 * PUSHnuti vysledkù (podle typu) na zasobnik
 */
int stackPUSHInt(stack *S, int i);
int stackPUSHReal(stack *S, double f);
int stackPUSHBool(stack *S, int i);
int stackPUSHChar(stack *S, char *c);         //opet, u charu si nejsem jist spravnosti

/*
 * POPnuti vysledku ze zasobnku, bude vracet celou strukturu
 */

int stackPOP(stack *S, struct stackPtr *result);

/*
 * Zjisti zdali je stack prazdny. Pokud je prazdny vraci 0 jinak 1
 */

int stackEmpty(stack *S);
#endif
