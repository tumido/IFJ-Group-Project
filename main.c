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

int main(int argc, char *argv[])
{
  FILE * code;

  if (argc != 2)
  {
    printErr("Spatny pocet parametru.\n");
    return EXIT_INTERNAL_ERROR;
  }

  if (!(code = fopen(argv[1], "r")))
  {
    printf("Soubor se nepodarilo otevrit.\n");
    return EXIT_INTERNAL_ERROR;
  }

  token printToken;
  tokenInit(&printToken);
  int retVal;
  while ((retVal = fillToken(code, &printToken)) != EOF)
  {
    if (retVal != EXIT_INTERNAL_ERROR && retVal != EXIT_LEXICAL_ERROR)
    {
      keyWordCheck(&printToken);
      // dulezity je typ tokenu, obsah (retezec je ukladan jen u stringu, identifikatoru, klicoveho slova)
      printf("Token \"%s\"  of length %d (token of %d type)\n", printToken.data,  printToken.length,  printToken.type);
    }
    tokenClean(&printToken);
  }
  tokenFree(&printToken);

  // Inicializujeme tabulku symbolu
  // Inicializace seznamu instrukci
  // Zavolame syntakticky analyzator
  // Kdyz se neco rozbije (chyba v prubehu prekladu - at uz lex, syn, sem), volame funkce:
    // uklid - uvolneni pameti, zavreni souboru
    // vratime error code
  // Jinak interpretujeme kod a nasledne po sobe zase uklidime a zavreme soubor

  fclose(code);

  return 0;
}
