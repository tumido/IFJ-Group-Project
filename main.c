/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:22:15 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
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
  btree table;
  SymbolTableInit(&table);

  printDebug("=========== PARSER ============\n");
  int retVal = parser(&in, &table, &ilist);

  printDebug("========== INTERPRET ==========\n");
  if (retVal == EXIT_SUCCESS) interpret(&ilist);

  SymbolTableDispose(&table);
  listFree(&ilist, retVal);
  fclose(in.file);

  return retVal;
}
