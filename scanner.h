/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:21 PM
 *         Autori:  TomášCoufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
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
#include "strings.h"

#define DETAIL_LENGHT 8 // relikt Kubovy spravy pameti, je treba predelat

/*   Typ pro stavovy automat
 * --------------------------------------------------------------------
 * - obsahuje vsechny stavy stavoveho automatu
 */
typedef enum
{
  s_begin, s_comment, s_id,
  s_colon, s_less, s_greater,
  s_string, s_string_escape, s_string_escape_check, s_string_check,
  s_integer, s_real,
  s_real_exp, s_real_exp_all, s_real_exp_ok
} tState;


/*   Struktura tokenu
 * ---------------------------------------------------------------------
 * - obsahuje typ lexemu a data
 */
typedef struct
{
  lexType type;
  void * data;
} token;


struct input
{
  FILE * file;
  unsigned int line;
};

/*   Nacitani tokenu
 * ---------------------------------------------------------------------
 * - funkce zodpovedna za nacteni casti zdrojoveho textu a prevedeni jej
 *   na lexem (token)
 * - rozlisi spravny typ tokenu a pokud zadnemu neodpovida vrati
 *   EXIT_LEXICAL_ERROR
 */
int fillToken(struct input * in, token * lex);

/*   Inicializace tokenu
 * ---------------------------------------------------------------------
 * - inicializuje token: hodnoty jeho parametru a prostor pro jeho data
 */
int tokenInit(token * str);

/*   vraceni pameti allokovane tokenem
 * ---------------------------------------------------------------------
 */
void tokenFree(token *str);

/*   Pridat charu do retezce
 * ---------------------------------------------------------------------
 * - prida znak na konec retezce (data) v tokenu a zvisi pocitadlo delky
 *   retezce s tim, ze soucasne misto inicializuje \0 hodnotou
 */
int addCharToString(token *str, char z);

/*   Kontrola klicovych slov
 * ---------------------------------------------------------------------
 * - kontroluje token, pokud je to identifikator, zda-li neni klicovym
 *   slovem, pokud ano, zmeni typ tokenu na l_key
 */
int keyWordCheck(token *str);

/*   Vycisteni tokenu
 * ---------------------------------------------------------------------
 * - provede vymazani dat tokenu - vymazani retezce
 */
void tokenClean(token *str);

/*   Prevod na integer
 * ---------------------------------------------------------------------
 * - pokud bzl nacten integer je treba prevest string ve kterem je
 *   ulozeny na integer
 * - provdede vymenu stringu za integer v lex->data a string dealokuje
 */
int strToInt(token * lex);

/*   Prevod na double
 * ---------------------------------------------------------------------
 * - analogicka funkce k prevodu na integer
 */
int strToDouble(token * lex);

#endif
