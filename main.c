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
#include "io.c"
#include "scanner.h"

int main(int argc, char *argv[])
{
  FILE *code;

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

/*
 * Zde bude volání funkce vypisujici tokeny na stdout (kontrola funkce scanneru).
 * Je treba vypisovat tokeny, dokud mi scanner nerekne konec.
 * Jsem z toho na prasky, nic nefunguje jak bych ocekaval, merlin ani eva se mnou
 * nemluvi, jdu se zaobirat aspon tim hloupym interpretem, s nim bude taky sposta
 * zabavy.
 *
 * Btw prikaz make na prelozeni celeho projektu mi dela neplechu, a nechci se v
 * Makefile hrabat, kdyz tomu poradne nerozumim.
 * Naincludovane "io.c" je samozrejme prasarna, ale jinak prekladac rve, ze nechape,
 * co sakra myslim volanim fce "printErr". Dekuji za pochopeni.
 */

  printToken(token);

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
