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
  s_bigger_or,          //13 operator >=
  s_same,              //14 operator =
  s_not_same,          //15 operator <>
  s_semicolon,         //16 strednik ;
  s_lex_end,           //17 konec tvorby tokenu
  s_lex_err,           //18 chyba, ktera vznikla pri lex analyze
  s_real_exp,          //19 pokracovani desetinneho cisla ..... 10e-2
  s_real_exp_all       //20 pokracovani desetinneho cisla ..... 10.41e-2

}tState;

typedef struct
{
  char * detail;            //detaily k identifikatoru
  int detailLenght;       //delka retezece (detailu)
  int allocatedMemory;    //velikost allokovane pameti

} tDetail;


void fillToken();                //predpokladam, ze budu dostavat adresu, kde bude prazdny token(formou struktury?) k vyplneni
int tokenDetailInit(tDetail *str);                    //inicializace detailu k ID
void tokenDetailFree(tDetail *str);                   //uvolneni pameti
void tokenDetailClean(tDetail *str);                  //promazani detailu
int addCharToString(tDetail *str, char z);           //prida znak do existujiciho retezce

#endif
