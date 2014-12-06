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

/*
 * PUSH Real
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou real
 */
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

/*
 * PUSH Boolean
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 */
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

/*
 * PUSH Char
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 */
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

/*
 * POP
 * -----------------------------------------------------------------------------
 * - vraci celou strukturu
 */
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

/*
 * Stack Empty
 * -----------------------------------------------------------------------------
 * - funkce testuje, zda je zasobnik prazdny
 */
int stackEmpty(stack *S)
{
  return S->numberOfResults == 0 ? true : false;
}

