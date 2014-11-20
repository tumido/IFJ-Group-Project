/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:31 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "scanner.h"

int tokenDetailInit(tDetail *str)
{
  if ((str->detail = (char*) malloc (DETAIL_LENGHT)) == NULL)
    return EXIT_INTERNAL_ERROR;
  str->detail[0] = '\0';
  str->detailLenght = 0;
  str->allocatedMemory = DETAIL_LENGHT;
  return EXIT_SUCCESS;
}

void tokenDetailFree(tDetail *str)
{
  free(str->detail);
}

void tokenDetailClean(tDetail *str)
{
  str->detail[0] = '\0';
  str->detailLenght = 0;
}

int addCharToString(tDetail *str, char z)
{
  if(str->detailLenght + 1 >= str->allocatedMemory)
  {
    if((str->detail = (char*) realloc(str->detail, str->detailLenght + DETAIL_LENGHT)) == NULL)            //realokace pameti
      return EXIT_INTERNAL_ERROR;
    str->allocatedMemory = str->detailLenght + DETAIL_LENGHT;
  }
  str->detail[str->detailLenght] = z;                    //pridani znaku do retezce
  str->detailLenght++;                                   //zvetseni velikosti retezce
  str->detail[str->detailLenght] = '\0';                 //pridani znaku 0 na konec
  return EXIT_SUCCESS;
}


void fillToken()               //funkce, ktera naplni token
{
  tDetail detailTokenu;         //promena, do ktere budu ukladat jak se danny identifikator jmenuje
  tState state = s_begin;        //nastavim pocatecni stav automatu
  bool read = true;              //bool pro zastaveni automatu
  int z;                         //promenna pro nacitani pismen/znaku

  tokenDetailInit(&detailTokenu);

  while (read && (z = getc(Code)))      //start automatu
  {
    switch(state)
    {
      case s_begin:                                          //pocatecni stav automatu, samotny switch bude v ramci moznosti formou seznamu, za ucelem zprehledneni
        if (isspace(z))                          state = s_begin;        //bile znaky preskocit
        else if (z == '{')                       state = s_coment;       //narazil jsem na "{" -> zacina komentar
        else if (isalpha(z) || z == '_')         state = s_id;           //pismeno nebo podtrzitko znaci zacatek identifikatoru nebo klicoveho slova
        else if (isdigit(z))                     state = s_integer;      //cele nebo desetinne cislo
        else if (z == '\'')                        state = s_string;       //zacatek retezce, cislo 39 je ascii hodnota apostrofu
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
        addCharToString(&detailTokenu, z);                            //ulozeni nacteneho znaku
        break;
      case s_coment:                     //nacitam komentare, nic nedelam
        if (z != '}');
        else
        {
          state = s_begin;               //konec komentare
        }
        break;
      case s_id:
        if (isalnum(z) || z == '_')          //dentifikator dale muze obahovat jakekoli cislo ci pismeno a znak "_"
        {
          addCharToString(&detailTokenu, z);             //zapisu dalsi znak a pokracuju
        }
        else
        {
          state = s_lex_end;
          ungetc(z);
        }
        break;
      case s_integer:                        //nacitam cislo
        if (isdigit(z))                     //pokracuje cislo
            addCharToString(&detailTokenu, z);
        if (z == 'E' || z == 'e')            //nacetl jsem exponent
        {
            addCharToString(&detailTokenu, z);
           state = s_real_exp;
        }
        if (z == '.')
        {
            addCharToString(&detailTokenu, z);
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
