/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  30/12/2014 18:15:58 PM
 *         Autori:  Tomá Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "ilist.h"

void listInit(tListOfInstr *L)
// funkce inicializuje seznam instrukci
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}

void listFree(tListOfInstr *L, int retVal)
// funkce dealokuje seznam instrukci
{
  tListItem *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    // uvolnime celou polozku
    if (retVal != EXIT_SUCCESS && ptr->Instruction.instType == I_CLEAR)
    {
      if (ptr->Instruction.type == k_string) free(((string *)ptr->Instruction.addr1)->str);
      free(ptr->Instruction.addr1);
    }
    free(ptr);
  }
}

int listInsertLast(tListOfInstr *L, tInstr I)
// vlozi novou instruci na konec seznamu
{
  printDebug("Vkladam Instrukci\n");
  tListItem *newItem;
  if ((newItem = malloc(sizeof (tListItem))) == NULL) return EXIT_INTERNAL_ERROR;
  newItem->Instruction = I;
  newItem->nextItem = NULL;
  if (L->first == NULL)
     L->first = newItem;
  else
     L->last->nextItem=newItem;
  L->last=newItem;
  return EXIT_SUCCESS;
}

void listFirst(tListOfInstr *L)
// zaktivuje prvni instrukci
{
  L->active = L->first;
}

void listNext(tListOfInstr *L)
// aktivni instrukci se stane nasledujici instrukce
{
  if (L->active != NULL)
  L->active = L->active->nextItem;
}

void listGoto(tListOfInstr *L, void *gotoInstr)
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  L->active = (tListItem*) gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L)
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  return (void*) L->last;
}

tInstr *listGetData(tListOfInstr *L)
// vrati aktivni instrukci
{
  if (L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(L->active->Instruction);
}
