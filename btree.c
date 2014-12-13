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
 *   struct node * global = NULL;
 */
void SymbolTableInit(btree * table)
{
  table->global = table->local = NULL;
}
void SymbolTableInitLocal(btree * table, btree * global)
{
  table->global = global->local;
  table->local = NULL;
}

/*   Vytvoreni uzlu z dat
 * ---------------------------------------------------------------------
 * - z poskytnutych dat vytvorim uzel pro tabulku symbolu
 * - vytvatim uzly pro promenne a pro funkce
 */
struct node * SymbolTableCreateNode(char * name, key type, void * data)
{
  struct node * nd;
  if ((nd = malloc(sizeof(struct node))) == NULL) return NULL;

  nd->type = type;
  nd->defined = false;
  nd->rightNode = nd->leftNode = NULL;
  strncpy(nd->keyValue, name, BUFSIZE);
  name = NULL;
  switch (type)
  {
    case k_int:
      nd->data = (int *) malloc(sizeof(int));
      if (data != NULL) *((int *)nd->data) = *(int *)data;
      break;
    case k_bool:
      nd->data = (bool *) malloc(sizeof(bool));
      if (data != NULL) *((bool *)nd->data) = *(bool *)data;
      break;
    case k_real:
      nd->data = (double *) malloc(sizeof(double));
      if (data != NULL) *((double *)nd->data) = *(double *)data;
      break;
    case k_string:
      nd->data = (string *) malloc(sizeof(string));
      if (data != NULL) *((string *)nd->data) = *(string *)data; // kopirovani stringu ??
      break;
    default:
      free(nd);
      return NULL;
      break;
  }
  if (nd->data == NULL) {free(nd); return NULL;}
  printDebug("Novy uzel promenne vytvoren\n");
  return nd;
}

struct node * SymbolTableCreateFunctionNode(char * name, key type, struct funcParam * param, bool defined)
{
  struct node * nd;
  if ((nd = malloc(sizeof(struct node))) == NULL) return NULL;

  nd->type = k_function;
  nd->rightNode = nd->leftNode = NULL;
  strncpy(nd->keyValue, name, BUFSIZE);
  name = NULL;

  if ((nd->data = (funcData *) malloc(sizeof(funcData))) == NULL) return NULL;

  ((funcData *)nd->data)->defined = defined;
  ((funcData *)nd->data)->param = param;
  ((funcData *)nd->data)->retVal = NULL;
  ((funcData *)nd->data)->retType = type;
  if ((((funcData *)nd->data)->table = (btree *) malloc(sizeof(btree))) == NULL) return NULL;

  SymbolTableInit(((funcData *)nd->data)->table);
  listInit(&((funcData *)nd->data)->ilist);

  printDebug("Novy uzel pro funkci vytvoren\n");
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
  return __SymbolTableInsert(&table->local, insert);
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
 * - zrusi lokalni tabulku symbolu (nerekurzivne z duvodu rychlosti) -> upravit
 */
int SymbolTableDispose(btree * table)
{
  __SymbolTableDispose(&table->local);
  table->local = NULL;
  return EXIT_SUCCESS;
}

int __SymbolTableDispose(struct node ** leaf)
{
  if (*leaf != NULL)
  {
    __SymbolTableDispose(&((*leaf)->leftNode));
    __SymbolTableDispose(&((*leaf)->rightNode));

    if ((*leaf)->type == k_function)
    {
      SymbolTableDispose(((funcData *)(*leaf)->data)->table);
      FunctionParamsListDispose(((funcData *)(*leaf)->data)->param);
      free(((funcData *)(*leaf)->data)->table);
      listFree(&((funcData *)(*leaf)->data)->ilist, EXIT_SUCCESS);
    }
    if ((*leaf)->type == k_string) free(((string *)(*leaf)->data)->str);
    free((*leaf)->data);
    free(*leaf);
  }
  return EXIT_SUCCESS;
}

int FunctionParamsListDispose(struct funcParam * paramList)
{
  struct funcParam * param;
  while (paramList != NULL)
  {
    printDebug("Mazu Parametr\n");
    param = paramList;
    paramList = paramList->next;
    free(param);
  }
  return EXIT_SUCCESS;
}

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */

struct node * SymbolTableSearch(btree * table, char * key)
{
  struct node * found = __SymbolTableSearch(table->local, key);
  if (found == NULL) {printDebug("Prohledavam globani tabulku\n"); return __SymbolTableSearch(table->global, key);}
  return found;
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
