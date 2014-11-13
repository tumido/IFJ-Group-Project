/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:24:23 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

/*
 * Hlavickove soubory
 */
// #include ...
#include <stdio.h>
#include <stdlib.h>
#include "io.h"


int main (int argc, char** argv)
{
  FILE *f;

  if (argc != 2)
  {
    printErr("Spatny pocet parametru.");
    return EXIT_INTERNAL_ERROR;
  }
  
  if (!(f = fopen(argv[1], "r")))
  {
    printf("Soubor se nepodarilo otevrit.\n");
    return EXIT_INTERNAL_ERROR;
  }   

   // Inicializujeme tabulku symbolu
   // Zavolame syntakticky analyzator
   // Kdyz se neco rozbije (chyba v prubehu prekladu), volame funkce:
     // uklid - uvolneni pameti, zavreni souboru
     // vratime error
   // jinak interpretujeme kod a nasledne po sobe zase uklidime a zavreme soubor

return 0;
}

