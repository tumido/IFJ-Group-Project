 /*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef PARSTACK_DEFINED
#define PARSTACK_DEFINED
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "strings.h"
#include "scanner.h"
#define MaxStackSize 100



typedef struct
{
    lexType TypVal; // tady si ulozim hodnotu a typ hodnoty
    lexType TypTok; // tady si ulozim typ lexemu
    long int vInt;
    double vDouble;
    bool vBool;
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
void DataFree(sData *str);
#endif
