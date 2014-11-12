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

/*
 * Chybove kody
 */
#define LEX_ERR 1        // chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu)
#define SYN_ERR 2        // chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury programu)
#define SEM_PROG_ERR 3   // semanticka chyba - nedefinovana fce, promenna, pokud o redefinici fce/promenne, atd
#define SEM_TYPE_ERR 4   // semanticka chyba typove kompatibility v aritmetickych, retezcovych a relacnich vyrazech
                         // pripadne spatny pocet nebo typ parametru u volani fce
#define O_SEM_ERR 5      // ostatni semantice chyby
#define LOAD_NUM_ERR 6   // behova chyba pri nacitani ciselne hodnoty ze vstupu
#define VAR_ERR 7        // behova chyba pri praci s neinicializovanou promennou
#define DIV_ERR 8        // behova chyba deleni nulou
#define O_RUN_ERR 9      // ostatni behove chyby
#define INTER_ERR 99     // interni chyba interpretu, tedy neovlivnitelna vstupnim programem (napriklad chyba
                         // alokace pameti, chyba pri otevirani souboru s ridicim programem, spatne parametry 
                         // prikazove radky, atd)

 // toto cele asi do nejakeho hlavickoveho souboru?


int main (int argc, char** argv)
{
  FILE *f;

  if (argc != 2)
  {
    fprintf(stderr, "Spatny pocet parametru.\n");
    return INTER_ERR; 
  }
  
  if (!(f = fopen(argv[1], "r")))
  {
    printf("Soubor se nepodarilo otevrit.\n");
    return INTER_ERR;
  }   

   // Inicializujeme tabulku symbolu
   // Zavolame syntakticky analyzator
   // Kdyz se neco rozbije (chyba v prubehu prekladu), volame funkce:
     // uklid - uvolneni pameti, zavreni souboru
     // vratime error
   // jinak interpretujeme kod a nasledne po sobe zase uklidime a zavreme soubor

return 0;
}

