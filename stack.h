/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

/*
 * Zasobnik
 * -----------------------------------------------------------------------------
 * implemetace pomoci linearniho seznamu
 */
typedef struct
{
  struct stackElem top; // ukazatel na vrchol zasobniku
  int numberOfResults; // pocet prvku v zasobniku
} stack;

/*
 * samotny prvek seznamu, bude obsahovat vsechnz mozne typy vysledku, nepouzite typy dostanou NULL
 * promena boolResult nahrazuje typ bool, nebot mi prijde jednodusi pracovat s integerem 1 TRUE nebo 0 FALSE
 * Roman se pta - proc? V cem je to jednodussi? Kdyztak bych bral jako 0 - TRUE a 1 - FALSE :)
 */
typedef struct stackPtr
{
  int iResult;
  float fResult;
  int boolResult;
  char cResult; // zde si presne nejsem jist, jestli by nebylo lepsi pouit pole
  struct stackPtr *next; // ukazatel na nasledujici prvek v zasobniku
} stackElem;

/*
 * Inicializace zasobniku, ocekavam ukazatel na dany zasobnik
 */
void stackInit(stack *S);

/*
 * PUSHnuti vysledku (podle typu) na zasobnik
 */
void stackPUSHInt(stack *S, int i);
void stackPUSHReal(stack *S, float f);
void stackPUSHBool(stack *S, int i);
void stackPUSHChar(stack *S, char *c); //opet, u charu si nejsem jist spravnosti

/*
 * POPnuti vysledku ze zasobnku, bude vracet celou strukturu
 */
void stackPOP(stack *S, stackPtr *result);

/*
 * Zjisti zdali je stack prazdny. Pokud je prazdny vraci 0 jinak 1
 */
int stackEmpty(stack *S);
