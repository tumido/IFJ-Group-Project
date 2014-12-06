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

void listFree(tListOfInstr *L)
// funkce dealokuje seznam instrukci
{
  tListItem *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    // uvolnime celou polozku
    free(ptr);
  }
}

int listInsertLast(tListOfInstr *L, tInstr I)
// vlozi novou instruci na konec seznamu
{
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




/*
 *----------------Cast pro praci se zasobnikem----------------
 */

void stackInit(stack * S)
{
  S->top = NULL;
  S->numberOfResults = 0;
  return;
}

int stackPUSHInt(stack *S, int i)
{
  struct stackPtr *push;
  if ((push = malloc(sizeof(stackElem))) == NULL)
    return EXIT_INTERNAL_ERROR;
  push->iResult = i;
  push->fResult = 0;
  push->boolResult = 0;
  push->cResult = 0;
  push->next = S->top;
  S->top = push;
  return EXIT_SUCCESS;
}

int stackPUSHReal(stack *S, double f)
{
  struct stackPtr *push;
  if ((push = malloc(sizeof(stackElem))) == NULL)
    return EXIT_INTERNAL_ERROR;
  push->iResult = 0;
  push->fResult = f;
  push->boolResult = false;
  push->cResult = '\0';
  push->next = S->top;
  S->top = push;
  return EXIT_SUCCESS;
}

int stackPUSHBool(stack *S, int i)
{
  struct stackPtr *push;
  if ((push = malloc(sizeof(stackElem))) == NULL)
    return EXIT_INTERNAL_ERROR;
  push->iResult = 0;
  push->fResult = 0;
  push->boolResult = i;
  push->cResult = '\0';
  push->next = S->top;
  S->top = push;
  return EXIT_SUCCESS;
}

int stackPUSHChar(stack *S, char *c)                // opet, nejsem si jist, jak to ma vypadat
{
  struct stackPtr *push;
  if ((push = malloc(sizeof(stackElem))) == NULL)
    return EXIT_INTERNAL_ERROR;
  push->iResult = 0;
  push->fResult = 0;
  push->boolResult = false;
  push->cResult = *c;
  push->next = S->top;
  S->top = push;
  return EXIT_SUCCESS;
}

int stackPOP(stack *S, struct stackPtr *result)
{
  if (S->top == NULL)
    return EXIT_INTERNAL_ERROR;
  struct stackPtr *tmp;
  tmp = S->top;
  S->top = S->top->next;
  result = tmp;
  free(tmp);
  return EXIT_SUCCESS;
}

int stackEmpty(stack *S)
{
  return S->numberOfResults == 0 ? 0 : 1;
}
