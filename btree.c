/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:22:15 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "btree.h"


void SymbolTableInit(btree * table)
{
  table->root = table->last = NULL;
}

struct node * SymbolTableCreateNode(key type, char * key)
{
  struct node * nd;
  if ((nd = malloc(sizeof(struct node))) == NULL) return NULL;

  nd->rightNode = nd->leftNode = NULL;
  strncpy(nd->keyValue, key, BUFSIZE);
  key = NULL;
  switch (type)
  {
    case k_int:
      nd->data = (int *) malloc(sizeof(int));
      break;
    case k_real:
      nd->data = (double *) malloc(sizeof(double));
      break;
    case k_string:
      nd->data = (string *) malloc(sizeof(string));
      break;
    default:
      free(nd);
      return NULL;
      break;
  }
  printErr("node\n");
  return nd;
}


/*   Vlozeni noveho prvku do tabulky symbolu
 * ---------------------------------------------------------------------
 * - zapoji novy prvek do tabulky symbolu na spravne misto
 * - vstupnimy parametry funkce jsou tabulka symbolu a prvek, ktery ma
 *   byt vlozen
 */
int SymbolTableInsert(btree * table, struct node * insert)
{
  return __SymbolTableInsert(&table->root, insert);
}

int __SymbolTableInsert(struct node ** leaf, struct node * insert)
{
  if (*leaf == NULL) { *leaf = insert; return EXIT_SUCCESS; }
  else if (strcmp(insert->keyValue, (*leaf)->keyValue) == EXIT_SUCCESS)
    { (*leaf)->data = insert->data; return EXIT_SUCCESS; }
  else if (strcmp(insert->keyValue, (*leaf)->keyValue) > EXIT_SUCCESS)
    return __SymbolTableInsert(&(*leaf)->rightNode, insert);
  else
    return __SymbolTableInsert(&(*leaf)->leftNode, insert);

}

/*   Zruseni tabulky symbolu
 * ---------------------------------------------------------------------
 * - zrusi tabulku symbolu (nerekurzivne z duvodu rychlosti) -> upravit
 */
int SymbolTableDispose(btree * table)
{
  __SymbolTableDispose(&table->root);
  table->root = table->last = NULL;
  return EXIT_SUCCESS;
}

int __SymbolTableDispose(struct node ** leaf)
{
  if (*leaf != NULL)
  {
    __SymbolTableDispose(&((*leaf)->leftNode));
    __SymbolTableDispose(&((*leaf)->rightNode));

    if ((*leaf)->type == l_str) free(((string * )(*leaf)->data)->str);
    free((*leaf)->data);
    free(*leaf);
  }
  return EXIT_SUCCESS;
}

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */
struct node * SymbolTableSearch(btree * table, char * key)
{
  return __SymbolTableSearch(table->root, key);
}

struct node * __SymbolTableSearch(struct node * leaf, char * key)
{
  if (leaf != NULL)
  {
    if (strcmp(key, leaf->keyValue) == EXIT_SUCCESS) { return leaf; }
    else if (strcmp(key, leaf->keyValue) < EXIT_SUCCESS) { return __SymbolTableSearch(leaf->leftNode, key); }
    else {return __SymbolTableSearch(leaf->rightNode, key); }
  }
  return NULL;
}
