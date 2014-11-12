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


// 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
// 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe struktury programu).
// 3 - sémantická chyba v programu – nedefinovaná funkce/promˇenná, pokus o redefinici
// funkce/promˇenné, atd.
// 4 - sémantická chyba typové kompatibility v aritmetických, ˇretˇezcových a relaˇcních
// výrazech, pˇríp. špatný poˇcet ˇci typ parametr°u u volání funkce.
// 5 - ostatní sémantické chyby.
// 6 - bˇehová chyba pˇri naˇcítání ˇcíselné hodnoty ze vstupu.
// 7 - bˇehová chyba pˇri práci s neinicializovanou promˇennou.
// 8 - bˇehová chyba dˇelení nulou.
// 9 - ostatní bˇehové chyby.
// 99 - interní chyba interpretu tj. neovlivnˇená vstupním programem (napˇr. chyba alokace
// pamˇeti, chyba pˇri otvírání souboru s ˇrídicím programem, špatné parametry
// pˇríkazové ˇrádky atd.).

/*
 * Chybove kody
 */
#define LEX_ERR 1
#define SYN_ERR 2
#define SEM_PROG_ERR 3
#define SEM_TYPE_ERR 4
#define O_SEM_ERR 5
#define LOAD_NUM_ERR 6
#define VAR_ERR 7
#define DIV_ERR 8
#define O_RUN_ERR 9
#define INTER_ERR 99
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

