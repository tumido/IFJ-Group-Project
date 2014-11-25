/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  10/17/2014 06:10:48 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "io.h"

void printErr(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  fprintf(stderr,"\x1B[31mError: \033[0m");
  vfprintf(stderr, msg, args);
  va_end(args);
  return;
}
