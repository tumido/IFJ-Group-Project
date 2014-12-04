#include "io.h"

// typy jednotlivych instrukci
#define I_STOP    0
#define I_READ    1
#define I_WRITE   2
#define I_SETZERO 3
#define I_INC     4
#define I_DEC     5
#define I_NOT     6
#define I_LAB     7
#define I_IFGOTO  8
#define I_GOTO    9

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;

typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);

/*
 *  -----ZASOBNIK A FUNKCE PRO PRACI S NIM------
 * momentalnim umistenim si nejsem jist, muzu presunout jinam, treba i do samostatneho souboru (jak .h tak .c)
 */

 /*
  * Zasobnik bude formou linearniho seznamu
  *
  * vlastni seznam
  *  |
  *  |
  *  V
  */
typedef struct
{
  struct stackElem top;           // ukazatel na vrchol zasobniku
  int numberOfResults;            // pocet prvku v zasobniku
} stack;

/*
 * samotny prvek seznamu, bude obsahovat vsechnz mozne typy vysledku, nepouzite typy dostanou NULL
 * promena boolResult nahrazuje typ bool, nebot mi prijde jednodusi pracovat s integerem 1 TRUE nebo 0 FALSE
 */

typedef struct stackPtr
{
  int iResult;
  float fResult;
  int boolResult;
  char cResult;             // zde si presne nejsem jist, jestli by nebylo lepsi pouit pole
  struct stackPtr *next;    // ukazatel na nasledujici prvek v zasobniku
} stackElem;

/*
 * Inicializace zasobniku, ocekavam ukazatel na dany zasobnik
 */
void stackInit(stack *S);

/*
 * PUSHnuti vysledkù (podle typu) na zasobnik
 */
void stackPUSHInt(stack *S, int i);
void stackPUSHReal(stack *S, float f);
void stackPUSHBool(stack *S, int i);
void stackPUSHChar(stack *S, char *c);         //opet, u charu si nejsem jist spravnosti

/*
 * POPnuti vysledku ze zasobnku, bude vracet celou strukturu
 */

void stackPOP(stack *S, stackPtr *result);

/*
 * Zjisti zdali je stack prazdny. Pokud je prazdny vraci 0 jinak 1
 */

int stackEmpty(stack *S);
