/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
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
  return;
}

/*
 * PUSH Integer
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 * - ostatni prvky vynulluje (to mi uplne smysl nedava, ale mozna jsem jen hloupej :)
 */

/*
 * PUSH Char
 * -----------------------------------------------------------------------------
 * - vlozi na zasobnik prvek s hodntou integer
 */
int stackPUSH(stack * S, btree * table)
{
  stackElem * push;
  if (((push = malloc(sizeof(stackElem))) == NULL) || ((push->table = malloc(sizeof(btree))) == NULL))
    return EXIT_INTERNAL_ERROR;

  SymbolTableCopy(table, push->table);
  push->next = S->top;
  S->top = push;
  return EXIT_SUCCESS;
}

/*
 * POP
 * -----------------------------------------------------------------------------
 * - vraci celou strukturu
 */
int stackPOP(stack *S, btree * table, char * key)
{
  if (S->top == NULL)
    return EXIT_INTERNAL_ERROR;

  SymbolTableRestore(table, S->top->table, key);
  stackElem * tmp = S->top;
  S->top = S->top->next;

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
  return S->top == NULL;
}
