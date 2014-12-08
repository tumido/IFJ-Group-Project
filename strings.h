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

#define BUFSIZE 256
#define BASE 10

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
  l_endl, l_eof, l_enddot,        // ;   EOF .
  l_sep, l_reset = 99             // ,   chyba
} lexType;

typedef enum
{
  k_begin, k_bool, k_do,
  k_else, k_end, k_false,
  k_find, k_forward, k_function,
  k_if, k_int, k_readln,
  k_real, k_sort, k_string,
  k_then, k_true, k_var,
  k_while, k_write
} key;

#endif
