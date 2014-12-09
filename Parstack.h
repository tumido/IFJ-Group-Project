 /*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "scanner.h"
#define MaxStackSize 100



typedef struct
{
    token lexdata; // tady si ulozim hodnotu a typ hodnoty
    lexType TypTok; // tady si ulozim typ lexemu

}sData;


typedef struct
{
    sData StackData[MaxStackSize];
    int top;
}Stack;

// inicializace zasobniku
void sInit (Stack *s);

// vlozi hodnotu na vrchol zasobniku
void sPush (Stack *s, sData item);

// odstrani prvek z vrcholu zasobniku
// a vrati jeho hodnotu
sData sPop (Stack *s);
// vrati co je na vrchu
sData sTop (Stack *s);

// zkontroluje jeslti je zasobnik prazdny
// true pokud je prazdny
int sEmpty(Stack *s);

int DataInit(sData *lex);
