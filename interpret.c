/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "io.h"

/*
 * Klasifikace datového typu
 * -----------------------------------------------------------------------------
 * - integer, real, boolean, string, null
 */
int returnValue()
{
  switch(source->type)
  {
    case T_INTEGER: 
    break;
    case T_REAL: 
    break;
    case T_BOOLEAN: 
    break;
    case T_STRING: 
    break;
    case T_NULL;
    break;
  }
  return INT_SUCCESS;
}

/*
 * Interpretace konkretni instrukce
 * -----------------------------------------------------------------------------
 * - specialni instrukce
 * - aritmeticke instrukce
 * - instrukce porovnani
 * - instrukce navraceni datoveho typu
 * - instrukce vestavenych funkci
 */
int instruction()
{    
  switch (I->instType)
  {

/*
 * Specialni instrukce
 * -----------------------------------------------------------------------------
 * - stop, read, write, if, if_end, jump, assign, substr, call_fuction, do_while
 */ 
    case I_STOP: // pro zastaveni provadeni cyklu
    break;
    case I_READ: // cteni ze stdin
    break;
    case I_WRITE: // vypis na stdout
    break;    
    case I_IF: // to bude dobre k rozpoznani zacatku IFu
    break;
    case I_IF_END: // toto zas bude znacit konec IFu, nejak
    break;
    case I_JUMP: // skaceme
    break;
    case I_LABEl: // navesti, kam skaceme
    break;
    case I_ASSIGN: // intrukce prirazeni, operator =
    break;
    case I_SUBSTR: // intrukce ulozi nekam podretezec v zadanem retezci
    break;
    case I_CALL_FUNCTION: // instrukce volani uzivatelskych funkci
    break;
    case I_RETURN; // instrukce navratu z volane funkce
    break;
    case I_DO_WHILE: // instrukce cyklu do-while
    break;

/*
 * Aritmeticke instrukce
 * -----------------------------------------------------------------------------
 * - mul, div, add, sub, con, inc, dec, neg
 */
    case I_MUL: // nasobeni
    break;
    case I_DIV: // deleni
    break;
    case I_ADD: // scitani
    break;
    case I_SUB: // odcitani
    break;
    case I_CON: // konktarenace, zretezeni
    break;
    case I_INC: // inkrementace
    break;
    case I_DEC; // dekrementace
    break;
    case I_NEG; // negace, bude vubec potreba?
    break;

/*
 * Instrukce porovnani
 * -----------------------------------------------------------------------------
 * - cmp, less, greater, les_equal, greater_equal, equal, not_equal
 */
    case I_CMP; // porovnavani
    break;
    case I_LESS; // je mensi nez
    break;
    case I_GREATER; // je vetsi nez
    break;
    case I_LESS_EQUAL: // je mensi nebo rovno
    break;
    case I_GREATER_EQUAL; // je vetsi nebo rovno
    break;
    case I_EQUAL; // rovnaji se
    break;
    case I_NOT_EQUAL: // nerovnaji se
    break;

/*
 * Intrukce na vraceni datoveho typu
 * -----------------------------------------------------------------------------
 * tomu teda moc nerozumim..
 * je v ni dalsi switch a pripady pro kazdy ty, zatim nepisu
 */
    case I_TYPE: 
    break;

/*
 * Instrukce implenetujici vestavene funkce
 * -----------------------------------------------------------------------------
 * copy(s : string; i : integer; n : integer) : string
 *   - vrati podretezec zadaneho retezce 's'
 *   - 'i' udava zacatek pozadovaneho podretezce (pocitano od 1)
 *   - 'n' urcuje delku podretezce
 * length(s : string) : integer
 *   - vrati delku retezce zadaneho parametrem 's'
 * find(s : string; search : string) : integer
 *   - vyhleda prvni vyskyt zadaneho podretezce 'search' v retezci 's' 
 *     a vrati jeho pozici (pocitano od 1)
 *   - pokud neni podretezec nalezen, vraci 0
 * sort(s : string) : string
 *   - seradi znaky v retezci 's' tak, aby znak si nizsi ordinarni
 *     hodnout vzdy predchazel znaku s vyssi ordinarni hodnotou
 *   - vraci retezec obsahujici serazene znaky  
 */
    case I_COPY: // implemetuje vestavenou funkci copy
    break;
    case I_LENGHT: // Implementuje vestavenou funkci lenght
    break;
    case I_FIND: // implementuje vestavenou funkci find
    break;
    case I_SORT: // implementuje vestavenou funkci sort
    break;
  }
  return 0; 
}

/*
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 * -----------------------------------------------------------------------------
 */
int interpreter()
{
  // ...
}

/* Zajimava myslenka jest ukladat triadresny kod jako vektor 
 * (tedy asi jednorozmerne pole)
 * pak si zrejme prochazim jeden vektor po druhem, a zpracovavam jeho prvky
 * naimplementovanymi instrukcemi
 * pricemz se pokazde posunu dal o jednu instrukci = vektor!!
 *
 * jeste se tam na neco pouziva zasobnik, a sice na to, ze si do nej ukladam
 * mezivysledky operaci. Sikovna to vecicka! :) Je to vlastne metodika, jak 
 * zpracovavat slozite programove kontrukce.
 * Takze bude potreba si naimplementovat zasobnik - IAL? A nejakym zpusobem
 * ho narvat do intepretu a pak hlavne odzkouset - musi umet vkladat, cist,
 * mazat a mozna jeste neco.
 * Takze spousta srandy prede mnou..
 * 
 * TODO:
 * Nastudovat rozhrani interpetu, system vykonavani instrukci (tabulka, seznam?)
 * System predavani 3AK - no asi ten vektor ne? Davat to jako string je blbost
 * ma entou, bych si to zas musel rozkouskovavat a stejne bych to pak ukladal 
 * nejakych lokalnich promennych nebo do pole. 
 * Mozna ale bych si to z toho pole vzdycky mohl rozkouskovat do promennych, 
 * ale mozna je to taky pekna zbytecnost. Uvidime, jak se s tim bude pracovat.
 */