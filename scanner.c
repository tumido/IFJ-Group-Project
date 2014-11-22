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

int tokenDetailInit(token *str)
{
  if ((str->detail = (char*) malloc (DETAIL_LENGHT)) == NULL)
    return EXIT_INTERNAL_ERROR;
  str->detail[0] = '\0';
  str->detailLenght = 0;
  str->allocatedMemory = DETAIL_LENGHT;
  return EXIT_SUCCESS;
}

void tokenDetailFree(token *str)
{
  free(str->detail);
}

void tokenDetailClean(token *str)
{
  str->detail[0] = '\0';
  str->detailLenght = 0;
}

int addCharToString(token *str, char z)
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

/* Funkce pro kontrolu klicovych slov, jako parametr predavam ukazatel na token (strukturu)
 * Pomoci strmp() a vetveni if else se rozhodnu, zdali se jedna o identifikator ci klicove slovo
 * a vysledek zapisu do tokenu
 */
int keyWordCheck(token *str)
{
  int result;
  if((strcmp(str->detail, "begin")) == 0)             str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "boolean")) == 0)      str->tokenMain = l_bool;
  else if((strcmp(str->detail, "do")) == 0)           str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "else")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "end")) == 0)          str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "false")) == 0)        str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "find")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "forward")) == 0)      str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "function")) == 0)     str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "if")) == 0)           str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "integer")) == 0)      str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "readln")) == 0)       str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "real")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "sort")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "string")) == 0)       str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "then")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "true")) == 0)         str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "var")) == 0)          str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "while")) == 0)        str->tokenMain = l_keyWord;
  else if((strcmp(str->detail, "write")) == 0)        str->tokenMain = l_keyWord;
  else                                                // nejedna se o klicove slovo
  {
    str->tokenMain = l_id;
  }
  return 0;
}

void fillToken(FILE *Code)               //funkce, ktera naplni token
{
  token detailTokenu;         //promena, do ktere budu ukladat jak se danny identifikator jmenuje
  tState state = s_begin;        //nastavim pocatecni stav automatu
  bool read = true;              //bool pro zastaveni automatu
  int z;                         //promenna pro nacitani pismen/znaku
  tokenDetailInit(&detailTokenu);

  while (read && (z = getc(*Code)))      //start automatu
  {
    switch(state)
    {
      case s_begin:                                          //pocatecni stav automatu, samotny switch bude v ramci moznosti formou seznamu, za ucelem zprehledneni
        if (isspace(z))                          state = s_begin;        //bile znaky preskocit
        else if (z == '{')                       state = s_coment;       //narazil jsem na "{" -> zacina komentar
        else if (isalpha(z) || z == '_')         state = s_id;           //pismeno nebo podtrzitko znaci zacatek identifikatoru nebo klicoveho slova
        else if (isdigit(z))                     state = s_integer;      //cele nebo desetinne cislo
        else if (z == '\'')                      state = s_string;       //zacatek retezce
        else if (z == '+')
        {
          state = s_add_num;                                             //scitani
          detailTokenu->tokenMain = l_add;
        }
        else if (z == '-')
        {
          state = s_sub_num;                                             //odcitani
          detailTokenu->tokenMain = l_sub;
        }
        else if (z == '*')
        {
          state = s_mul_num;                                             //nasobeni
          detailTokenu->tokenMain = l_mul;
        }
        else if (z == '/')
        {
          state = s_div_num;                                             //deleni
          detailTokenu->tokenMain = l_div;
        }
        else if (z == '<')                       state = s_less;         //je mensi
        else if (z == '>')                       state = s_bigger;       //je vetsi
        else if (z == '=')
        {
          state = s_same;                                                //je rovno
          detailTokenu->tokenMain = l_same;
        }
        else if (z == ';')
        {
          state = s_semicolon;                                           //strednik
          detailTokenu->tokenMain = l_semicolon;
        }
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
          keyWordCheck(&detailTokenu);
          state = s_lex_end;
          ungetc(z);
        }
        break;
      case s_integer:                        //nacitam cislo
        if (isdigit(z))                     //pokracuje cislo
            addCharToString(&detailTokenu, z);
        else if (z == 'E' || z == 'e')            //nacetl jsem exponent
        {
          addCharToString(&detailTokenu, z);
          state = s_real_exp;
        }
        else if (z == '.')
        {
          addCharToString(&detailTokenu, z);
          state = s_real;
        }
        else
        {
          detailTokenu->tokenMain = l_integer;
          state = s_lex_end;
          ungetc(z);
        }
        break;
      case s_real:                               //yatim mam nacteno nejake cislo a tecku
        if (isdigit(z))
        {
          addCharToString(&detailTokenu, z);
        }
        else
        {
          state = s_lex_err;           // koncim analyzu po znamenku '.' musi nasledovat cislice ..... nejsem si jist, zdali musim provadet ungetc, nebot analyza konci
        }
        break;
      case s_real_ok:                            //zde uz mam nactene cislo tecku i desetinnou cast, cekam na konec cisla
        if (isdigit(z))
        {
          addCharToString(&detailTokenu, z);
        }
        else if(z == 'E' || z == 'e')
        {
          addCharToString(&detailTokenu, z);
          state = s_real_exp;
        }
        else
        {
          detailTokenu->tokenMain = l_real;
          state = s_lex_end;
          ungetc(z, Code);
        }
        break;
      case s_real_exp:     //zde mam posledni nactene e (exponent) tudiz pokud prijde neco jine nez cislice nebo znamenka + nebo - vracim chybu a ukoncim automat
        if (isdigit(z))
        {
          addCharToString(&detailTokenu, z);
          state = s_real_exp_all;
        }
        else if (z == '+' || z == '-')
        {
          addCharToString(&detailTokenu, z);
          state = s_real_exp_ok;
        }
        else
        {
          state = s_lex_err;
        }
        break;
      case s_real_exp_ok:                    //mam nacteny exponent i pripadne znamenko, cekam pouze cislici
        if(isdigit(z))
        {
          addCharToString(&detailTokenu, z);
          state = s_real_exp_all;
        }
        else
        {
          state = s_lex_err;
        }
        break;
      case s_real_exp_all:                   //cislo je spravne, pouze ho doctu do konce
        if (isdigit(z))
        {
          addCharToString(&detailTokenu, z);
        }
        else
        {
          detailTokenu->tokenMain = l_real;
          state = s_lex_end;
          ungetc(z, Code);
        }
        break;
      case s_string:                         //nacitam retezec ohraniceny apostrofy
        addCharToString(&detailTokenu, z);
        if (z == '\'')
        {
          state = s_string_check;
        }

        break;
      case s_string_check:                   //nacetl jsem jeden apostrof, pokud nactu druhy, jsem stale v retezci, pokud ne, retezec konci. Zaroven dalsi apostrof neukladam
        if(z == '\'')
        {
          state = s_string;
        }
        else
        {
          detailTokenu->tokenMain = l_string;
          state = s_lex_end;
          ungetc(z, Code);
        }
        break;
      case s_less:           //nacetl jsem < .... je mensi
        addCharToString(&detailTokenu, z);
        if (z == '=')    //jeste muze prijit = nebo >
        {
          detailTokenu->tokenMain = l_lessOr;
          state = s_less_or;
        }
        else if(z == '>')
        {
          state = s_not_same;
          detailTokenu->tokenMain = l_notSame:
        }
        else
        {
          detailTokenu->tokenMain = l_less;
          state = s_lex_end;
          ungetc(z, Code);
        }
        break;
      case s_bigger:
        addCharToString(&detailTokenu, z);
        if (z == '=')
        {
          detailTokenu->tokenMain = l_biggerOr;
          state = s_bigger_or;
        }
        else
        {
          detailTokenu->tokenMain = l_bigger;
          state = s_lex_end;
          ungetc(z, Code);
        }
        break;
      case s_add_num:                                //nevetvici se/konecne stavy automatu
      case s_sub_num:
      case s_mul_num:
      case s_div_num:
      case s_semicolon:
      case s_same:
      case s_not_same:
      case s_less_or:
      case s_bigger_or:
        state = s_lex_end;
        ungetc(z, Code);
        break;
      case s_lex_end:                            //automat konci
        ungetc(z, Code);
        read = false;
        break;
      case s_lex_err:                    //chyba pri lexikalni analyze
        return EXIT_LEXICAL_ERROR;
    }
  }
}
