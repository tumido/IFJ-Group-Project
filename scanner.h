/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:21 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef SCANNER_INCLUDED
#define SCANNER_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "io.h"

#define DETAIL_LENGHT 8             //konstatna, kterou urcuji po kolika bytech bude provadena alokace pameti pro detail dokenu

typedef enum            //seznam vsech stavu automatu
{
  s_begin,             // 0 pocatecni stav automatu
  s_coment,            // 1 komentare
  s_id,                // 2 identifikator
  s_integer,           // 3 cislo
  s_real,              // 4 desetinne cislo
  s_string,            // 5 retezec
  s_add_num,           // 6 operator +
  s_sub_num,           // 7 operator -
  s_mul_num,           // 8 operator *
  s_div_num,           // 9 operator /, vysledek je vzdy typu real!
  s_less,              //10 operator <
  s_bigger,            //11 operator >
  s_less_or,           //12 operator <=
  s_bigger_or,         //13 operator >=
  s_same,              //14 operator =
  s_not_same,          //15 operator <>
  s_semicolon,         //16 strednik ;
  s_lex_end,           //17 konec tvorby tokenu
  s_lex_err,           //18 chyba, ktera vznikla pri lex analyze
  s_real_exp,          //19 pokracovani desetinneho cisla ..... 10e-2
  s_real_exp_all       //20 pokracovani desetinneho cisla ..... 10.41e-2
} tState;

typedef enum           //Pomoci tohoto enumerate budu ukladat informaci o lexemu do tokenu ---zdali se jedna o indetifikator,integer,etc
{
  l_id,                 // 0 nacetl jsem identifikator
  l_keyWord,            // 1 nacetl jsem klicove slovo
  l_integer,            // 2 nacetl jsem integer
  l_real,               // 3 nacetl jsem real
  l_string,             // 4 nacetl jsem retezcovy literal
  l_bool,               // 5 nacetl jsem boolovsky literal
  l_add,                // 6 nacetl jsem scitani
  l_sub,                // 7 nacetl jsem odcitani
  l_mul,                // 8 nacetl jsem nasobeni
  l_div,                // 9 nacetl jsem deleni
  l_less,               //10 nacetl jsem mensi
  l_lessOr,             //11 nacetl jsem mensi rovno
  l_bigger,             //12 nacetl jsem vetsi
  l_biggerOr,           //13 nacetl jsem vetsi rovno
  l_same,               //14 nacetl jsem rovna se
  l_notSame,            //15 nacetl jsem nerovna se
  l_semicolon,          //16 nacetl jsem strednik
} lexType;

typedef struct            //Samotny token, jeho struktura
{
  lexType tokenMain;      //Hlavni cast tokenu, yde ukladam, co jsem vlastne nacetl za typ lexemu
  char * detail;          //detaily k lexemu
  int detailLenght;       //delka retezece (detailu)
  int allocatedMemory;    //velikost allokovane pameti

} token;


void fillToken(FILE *Code);                //predpokladam, ze budu dostavat adresu, kde bude prazdny token(formou struktury?) k vyplneni
int tokenDetailInit(token *str);                    //inicializace detailu k ID
void tokenDetailFree(token *str);                   //uvolneni pameti
void tokenDetailClean(token *str);                  //promazani detailu
int addCharToString(token *str, char z);           //prida znak do existujiciho retezce
int keyWordCheck(token *str);

#endif
