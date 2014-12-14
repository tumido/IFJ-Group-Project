/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  30/12/2014 18:15:58 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef ILIST_INCLUDED
#define ILIST_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "strings.h"
#define I_CLEAR 9

typedef struct
{
  int instType;  // typ instrukce
  key type;
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

/*
 * pro funkci COPY je potreba aby operand2 (addr2) byla struktura o dvou prvcich
 * moje otazka na TOMA, muzuju pouzit tento zpusob?
 */
typedef struct copy
{
  int start;
  int legth;
}tCopy;

void listInit(tListOfInstr *L); // funkce inicializuje seznam instrukci
void listFree(tListOfInstr *L, int retVal); // funkce dealokuje seznam instrukci
int listInsertLast(tListOfInstr *L, tInstr I); // vlozi novou instruci na konec seznamu
void listFirst(tListOfInstr *L); // zaktivuje prvni instrukci
void listNext(tListOfInstr *L); // aktivni instrukci se stane nasledujici instrukce
void listGoto(tListOfInstr *L, void *gotoInstr); // nastavime aktivni instrukci podle zadaneho ukazatele
void *listGetPointerLast(tListOfInstr *L); // vrati ukazatel na posledni instrukci
tInstr *listGetData(tListOfInstr *L); // vrati aktivni instrukci
#endif
