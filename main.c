/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:22:15 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "scanner.h"
#include "parser.h"
#include "ilist.h"
#include "stack.h"

int main(int argc, char *argv[])
{
  struct input in;
  in.line = 1;
  in.newline = false;

  if (argc != 2)
  {
    printErr("Spatny pocet parametru.\n");
    return EXIT_INTERNAL_ERROR;
  }

  if (!(in.file = fopen(argv[1], "r")))
  {
    printf("Soubor se nepodarilo otevrit.\n");
    return EXIT_INTERNAL_ERROR;
  }

  tListOfInstr ilist;
  listInit(&ilist);
  stack s;
  stackInit(&s);
  btree table;
  SymbolTableInit(&table);

  printDebug("=========== PARSER ============\n");
  int retValInterpret = EXIT_SUCCESS;
  int retVal = parser(&in, &table, &ilist, &s);

  printDebug("========== INTERPRET ==========\n");
  if (retVal == EXIT_SUCCESS) retValInterpret = interpret(&ilist);

  SymbolTableDispose(&table);
  listFree(&ilist, retVal);
  fclose(in.file);

  if (retValInterpret == EXIT_TYPE_ERROR) printErr("TYPE ERROR: You are trying to input boolean. Sorry I can't agree with that.\n");
  if (retValInterpret == EXIT_READ_STDIN_ERROR) printErr("READ STDIN ERROR: Your input is of a wrong data type.\n");
  if (retVal != EXIT_SUCCESS) return retVal;
  return retValInterpret;
}
