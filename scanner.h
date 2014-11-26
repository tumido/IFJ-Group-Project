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

/*   Typ pro lexemy
 * ---------------------------------------------------------------------
 * - urcuje typ lexemu, resp. tokenu
 */
typedef enum
{
  l_id, l_key, l_assign,          // identifikator   klicove slovo   :=
  l_colon, l_int,                 // :   integer
  l_real, l_str, l_bool,          // float   string   bool
  l_add, l_sub, l_mul, l_div,     // +   -   *   /
  l_less, l_lequal, l_greater,    // <   <=   >
  l_gequal, l_equal, l_not,       // >=  =   <>
  l_lparenth, l_rparenth,         // (   )
  l_lbracket, l_rbracket,         // [   ]
  l_endl, l_eof, l_reset = 99     // ;   EOF   chyba
} lexType;

/*   Struktura tokenu
 * ---------------------------------------------------------------------
 * - obsahuje typ lexemu a data
 */
typedef struct
{
  lexType type;
  char * data;
  unsigned int length;
  unsigned int allocatedMemory;
} token;


/*   Nacitani tokenu
 * ---------------------------------------------------------------------
 * - funkce zodpovedna za nacteni casti zdrojoveho textu a prevedeni jej
 *   na lexem (token)
 * - rozlisi spravny typ tokenu a pokud zadnemu neodpovida vrati
 *   EXIT_LEXICAL_ERROR
 */
int fillToken(FILE *Code, token * lex);

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

#endif
