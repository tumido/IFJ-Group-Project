/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "stack.h"
#include "io.h"

/*
 * stack inicialization
 * -----------------------------------------------------------------------------
 * - inicializace zasobniku
 * - vynulluje prvek na vrcholu zasobniku
 * - prohlasi citac vysledku za prazdny
 */
void stackInit(stack *S)
{
  S->top = NULL;
  S->numberOfResults = 0;
  return;
}

/*
 * PUSH Integer
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 * - ostatni prvky vynulluje (to mi uplne smysl nedava, ale mozna jsem jen hloupej :)
 */
void stackPUSHInt(stack *S, int i)
{
  struct stackPtr *push;
  if (!(push = malloc(sizeof(stackElem)))) return EXIT_INTERNAL_ERROR;
  else 
  {
    push->iResult = i;
    push->fResult = NULL;
    push->boolResult = NULL;
    push->cResult = NULL;
    push->next = S->top;
    S->top = push;
    return;
  }
}

/*
 * PUSH Real
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou real
 */
void stackPUSHReal(stack *S, float f)
{
  struct stackPtr *push;
  if (!(push = malloc(sizeof(stackElem)))) return EXIT_INTERNAL_ERROR;
  else
  {
    push->iResult = NULL;
    push->fResult = f;
    push->boolResult = NULL;
    push->cResult = NULL;
    push->next = S->top;
    S->top = push;
    return;
  }
}

/*
 * PUSH Boolean
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 */
void stackPUSHBool(stack *S, int i)
{
  struct stackPtr *push;
  if (!(push = malloc(sizeof(stackElem)))) return EXIT_INTERNAL_ERROR;
  else
  {
    push->iResult = NULL;
    push->fResult = NULL;
    push->boolResult = i;
    push->cResult = NULL;
    push->next = S->top;
    S->top = push;
    return;
  }
}

/*
 * PUSH Char
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 */
void stackPUSHChar(stack *S, char *c) // opet, nejsem si jist, jak to ma vypadat
{
  struct stackPtr *push;
  if (!(push = malloc(sizeof(stackElem)))) return EXIT_INTERNAL_ERROR;
  else
  {
    push->iResult = NULL;
    push->fResult = NULL;
    push->boolResult = NULL;
    push->cResult = *c;
    push->next = S->top;
    S->top = push;
    return;
  }
}

/*
 * POP
 * -----------------------------------------------------------------------------
 * - vraci celou strukturu
 */
void stackPOP(stack *S, stackPtr *result)
{
  if (!S->top) return EXIT_INTERNAL_ERROR; // pokud je zasobnik prazdny => error
  else
  {
    struct stackPtr *tmp;
    tmp = S->top;
    S->top = S->top->next;
    *result = tmp;
    free(tmp);
    return;
  }
}

/*
 * Stack Empty
 * -----------------------------------------------------------------------------
 * - funkce testuje, zda je zasobnik prazdny
 */
int stackEmpty(stack *S)
{
  return S->numberOfResults == 0 ? TRUE : FALSE;
}
