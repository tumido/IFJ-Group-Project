/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:22:15 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
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
  nd->funcData = NULL;
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

  if ((nd->funcData = (funcData *) malloc(sizeof(funcData))) == NULL) return NULL;
  switch (type)
  {
    case k_int: if ((nd->data = malloc(sizeof(int))) == NULL) return NULL; break;
    case k_real: if ((nd->data = malloc(sizeof(double))) == NULL) return NULL; break;
    case k_string: if ((nd->data = malloc(sizeof(string))) == NULL) return NULL; break;
    case k_bool: if ((nd->data = malloc(sizeof(bool))) == NULL) return NULL; break;
    default: return NULL;
  }

  ((funcData *)nd->funcData)->defined = defined;
  ((funcData *)nd->funcData)->param = param;
  ((funcData *)nd->funcData)->retType = type;
  if ((((funcData *)nd->funcData)->table = (btree *) malloc(sizeof(btree))) == NULL) return NULL;

  SymbolTableInit(((funcData *)nd->funcData)->table);
  listInit(&((funcData *)nd->funcData)->ilist);

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
      SymbolTableDispose(((funcData *)(*leaf)->funcData)->table);
      FunctionParamsListDispose(((funcData *)(*leaf)->funcData)->param);
      free(((funcData *)(*leaf)->funcData)->table);
      listFree(&((funcData *)(*leaf)->funcData)->ilist, EXIT_SUCCESS);
      free((*leaf)->funcData);
    }
    else if ((*leaf)->type == k_string) free(((string *)(*leaf)->data)->str);
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


int SymbolTableCopy(btree * tableOriginal, btree * tableNew)
{
  tableNew->global = tableOriginal->global;
  tableNew->local = __SymbolTableCopy(tableOriginal->local);
  return EXIT_SUCCESS;
}

struct node * __SymbolTableCopy(struct node * nd)
{
  if(nd != NULL)
  {
    struct node * nodeNew;
    if ((nodeNew = SymbolTableCreateNode(nd->keyValue, nd->type, nd->data)) == NULL)
      return NULL;

    nodeNew->leftNode = __SymbolTableCopy(nd->leftNode);
    nodeNew->rightNode = __SymbolTableCopy(nd->rightNode);
    return nodeNew;
  }
  return NULL;
}
int SymbolTableRestore(btree * tableOriginal, btree * tableNew, char * key)
{
  tableNew->global = tableOriginal->global;
  __SymbolTableRestore(tableOriginal->local, tableNew->local, key);
  return EXIT_SUCCESS;
}

int __SymbolTableRestore(struct node * ndO, struct node * nd, char * key)
{
  if(ndO != NULL || nd != NULL)
  {
    if (strcmp(ndO->keyValue, key) != EXIT_SUCCESS)
    {
      switch (ndO->type)
      {
        case k_int:
          *(int *)ndO->data = *(int *)nd->data;
          break;
        case k_real:
          *(double *)ndO->data = *(double *)nd->data;
          break;
        case k_bool:
          *(bool *)ndO->data = *(bool *)nd->data;
          break;
        case k_string:
          ((string *)ndO->data)->str = ((string *)nd->data)->str;
          ((string *)ndO->data)->alloc = ((string *)nd->data)->alloc;
          ((string *)ndO->data)->length = ((string *)nd->data)->length;
          break;
        default:
          return EXIT_INTERNAL_ERROR;
      }
    } else printDebug("Navratova hodnota ponechana\n");

    __SymbolTableRestore(ndO->leftNode, nd->leftNode, key);
    __SymbolTableRestore(ndO->rightNode, nd->rightNode, key);
    return EXIT_SUCCESS;
  }
  return EXIT_INTERNAL_ERROR;
}
