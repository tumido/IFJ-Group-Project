/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  12/04/2014 02:10:01 PM
 *         Autori:  TomášCoufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef STRINGS_INCLUDED
#define STRINGS_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char * str;
  unsigned int length;
  unsigned int alloc;
} string;

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
  l_less, l_lequal, l_greater,    // <   <=  >
  l_gequal, l_equal, l_not,       // >=  =   <>
  l_lparenth, l_rparenth,         // (   )
  l_lbracket, l_rbracket,         // [   ]
  l_endl, l_eof, l_enddot,        // ;   EOF .
  l_sep, l_reset = 99             // ,   chyba
} lexType;
#endif
