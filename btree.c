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


/*   Vytvoreni tabulky symbolu
 * ---------------------------------------------------------------------
 * - lze provest jednoduchym, lec ucinnym
 *   struct node * root = NULL;
 */
void SymbolTableInit(btree * table)
{
  table->root = NULL;
}

/*   Vytvoreni uzlu z dat
 * ---------------------------------------------------------------------
 * - z poskytnutych dat vytvorim uzel pro tabulku symbolu
 * - vytvatim uzly pro promenne a pro funkce
 */
struct node * SymbolTableCreateNode(char * name, key type)
{
  struct node * nd;
  if ((nd = malloc(sizeof(struct node))) == NULL) return NULL;

  nd->type = type;
  nd->rightNode = nd->leftNode = NULL;
  strncpy(nd->keyValue, name, BUFSIZE);
  name = NULL;
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
  if (nd->data == NULL) {free(nd); return NULL;}
  printErr("Novy uzel promenne vytvoren\n");
  return nd;
}

struct node * SymbolTableCreateFunctionNode(char * name, key type, struct funcParam * param, unsigned int count, bool defined)
{
  struct node * nd;
  if ((nd = malloc(sizeof(struct node))) == NULL) return NULL;

  nd->type = k_function;
  nd->rightNode = nd->leftNode = NULL;
  strncpy(nd->keyValue, name, BUFSIZE);
  name = NULL;

  if ((nd->data = (funcData *) malloc(sizeof(funcData))) == NULL) return NULL;

  ((funcData *)nd->data)->defined = defined;
  ((funcData *)nd->data)->numberOfParams = count;
  ((funcData *)nd->data)->param = param;
  ((funcData *)nd->data)->retVal = type;
  ((funcData *)nd->data)->localTable = NULL;

  printErr("Novy uzel pro funkci vytvoren\n");
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
  table->root = NULL;
  return EXIT_SUCCESS;
}

int __SymbolTableDispose(struct node ** leaf)
{
  if (*leaf != NULL)
  {
    __SymbolTableDispose(&((*leaf)->leftNode));
    __SymbolTableDispose(&((*leaf)->rightNode));

    if ((*leaf)->type == k_string) free(((string * )(*leaf)->data)->str);

    if ((*leaf)->type == k_function)
    {
      struct funcParam * param;
      for (unsigned int i = 0; i < ((funcData *)(*leaf)->data)->numberOfParams; i++)
      {
        printErr("Mazu Parametr\n");
        param = ((funcData *)(*leaf)->data)->param;
        ((funcData *)(*leaf)->data)->param = ((funcData *)(*leaf)->data)->param->next;
        free(param);
      }
    }

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
