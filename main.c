/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:24:23 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

int main (int argc, char** argv)
{
  FILE *f;

  if (argc != 2)
  {
    fprintf(stderr, "Spatny pocet parametru.\n");
    return 99; // 99 je kod pro interni chybu, asi bude fajn si to nekde vsechno sepsat do struktury?;
  }
  
  if (!(f = fopen(argv[1], "r")))
  {
    printf("Soubor se nepodarilo otevrit.\n");
    return 99;
  }   

return 0;
}

