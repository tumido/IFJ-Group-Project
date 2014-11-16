/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:31 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include <stdio.h>
#include <ctype.h>
#include "scanner.h"


#define DETAIL_LENGHT 8             //konstatna, kterou urcuji po kolika bytech bude provadena alokace pameti pro detail dokenu
#define ERROR 1
#define SUCCES 0
int test;

FILE *Code;

void load_Code (FILE *f)        //funkce pro nacteni ydrojoveho kodu
{
  Code = f;
}

int token_detail_init(tDetail *str)
{
   if ((str->detail = (char*) malloc (DETAIL_LENGHT)) == NULL)
    return ERROR;
   str->detail[0] = '\0';
   str->detail_lenght = 0;
   str->allocated_memory = DETAIL_LENGHT;
   return SUCCES;
}

void token_detail_free(tDetail *str)
{
    free(str->detail);
}

void token_detail_clean(tDetail *str)
{
    str->detail[0] = '\0'
    str->detail_lenght = 0;
}

int add_string_to_char(tDetail *str, char z)
{
    if(str->detail_lenght + 1 >= str->allocated_memory)
    {
        if((str->detail = (char*) realloc(str->detail, str->detail_lenght + DETAIL_LENGHT)) == NULL)            //realokace pameti
            return ERROR;
        str->allocated_memory = str->detail_lenght + DETAIL_LENGHT;
    }
    str->detail[str->detail_lenght] = z;                    //pridani znaku do retezce
    str->detail_lenght++;                                   //zvetseni velikosti retezce
    str->detail[str->detail_lenght] = '\0'                  //pridani znaku 0 na konec
}


void fill_Token()               //funkce, ktera naplni token
{

tDetail detail_tokenu;         //promena, do ktere budu ukladat jak se danny identifikator jmenuje
tState state = s_begin         //nastavim pocatecni stav automatu
bool read = true;              //bool pro zastaveni automatu
int z;                         //promenna pro nacitani pismen/znaku

token_detail_init(&detail_tokenu);

  while (read && (z = getc(Code)))      //start automatu
   {
      switch(state)
      {
         case s_begin:                                          //pocatecni stav automatu, samotny switch bude v ramci moznosti formou seznamu, za ucelem zprehledneni
         {
           if (isspace(z))                          state = s_begin;        //bile znaky preskocit
           else if (z == '{')                       state = s_coment;       //narazil jsem na "{" -> zacina komentar
           else if (isalpha(z) || z == '_')         state = s_id;           //pismeno nebo podtrzitko znaci zacatek identifikatoru nebo klicoveho slova
           else if (isdigit(z))                     state = s_integer;      //cele nebo desetinne cislo
           else if (z == 39)                        state = s_string;       //zacatek retezce, cislo 39 je ascii hodnota apostrofu
           else if (z == '+')                       state = s_add_num;      //scitani
           else if (z == '-')                       state = s_sub_num;      //odcitani
           else if (z == '*')                       state = s_mul_num;      //nasobeni
           else if (z == '/')                       state = s_mul_num;      //deleni
           else if (z == '<')                       state = s_less;         //je mensi
           else if (z == '>')                       state = s_bigger;       //je vetsi
           else if (z == '=')                       state = s_same;         //je rovno
           else if (z == ';')                       state = s_semicolon;    //strednik
           else
           {
              state = s_lex_err;                                            //nastala chyba
           }
           add_string_to_char(detail_tokenu, z);                            //ulozeni nacteneho znaku
           break;
         }

         case s_coment:                     //nacitam komentare, nic nedelam
         {
           if (z != '}');
           else
           {
             state = s_begin;               //konec komentare
           }
           break;
         }

         case s_id;
         {
           if (isalnum(z) || z == '_')          //dentifikator dale muze obahovat jakekoli cislo ci pismeno a znak "_"
           {
             add_string_to_char(z);             //zapisu dalsi znak a pokracuju
           }
           else
           {
             state = s_lex_end;
             ungetc(z);
           }
           break;
         }

         case s_integer:                        //nacitam cislo
         {
            if (isdigit(z))                     //pokracuje cislo
                add_string_to_char(z);
            if (z == 'E' || z == 'e')            //nacetl jsem exponent
            {
               add_string_to_char(z);
               state = s_real_exp;
            }
            if (z == '.')
            {
               add_string_to_char(z);
               state = s_real;
            }
            else
            {
               state = s_lex_end;
               ungetc(z);
            }
            break;
         }







      }
   }

}
