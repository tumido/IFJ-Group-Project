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
#include "interpret.h"

// datove typy
#define T_INTEGER 0
#define T_REAL 1
#define T_BOOLEAN 2
#define T_STRING 3

// typy jednotlivych instrukci
#define I_STOP 4 
#define I_READ 4
#define I_WRITE 6 
#define I_IF 7 
#define I_IF_END 8 
#define I_JUMP 9 
#define I_LABEl 10 
#define I_ASSIGN 11 
#define I_SUBSTR 12 
#define I_CALL_FUNCTION 13
#define I_RETURN 14
#define I_DO_WHILE 15
#define I_MUL 16
#define I_DIV 17 
#define I_ADD 18 
#define I_SUB 29 
#define I_CON 20 
#define I_INC 21 
#define I_DEC 22 
#define I_NEG 23 
#define I_CMP 24 
#define I_LESS 25 
#define I_GREATER 26
#define I_LESS_EQUAL 27
#define I_GREATER_EQUAL 28
#define I_EQUAL 29
#define I_NOT_EQUAL 30
#define I_TYPE 31
#define I_COPY 32 
#define I_SORT 33 
// Toto cele pak do nejakeho hlavickoveho souboru, asi do ilist.h (instruction list)

/*
 * Klasifikace datového typu
 * -----------------------------------------------------------------------------
 * - integer, real, boolean, string
 */
int returnValue(tVarValue *destination, tVarValue *source)
{
  switch(source->type)
  {
    case T_INTEGER: 
      (*destination)->type = T_INTEGER;
      (*destination)->integer = source->integer;
      break;
    case T_REAL: 
      (*destination)->type = T_REAL;
      (*destination)->rea = source->real;
      break;
    case T_BOOLEAN: 
      (*destination)->type = T_BOOLEAN;
      (*destination)->boolean = source->boolean;
      break;
    case T_STRING: 
      (*destination)->type = T_STRING;
      (*destination)->string = source->string;
      break;
  }
  return EXIT_SUCCESS;
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
int instruction(tSymbolTable *ST, tListOfInstr *instrList)
{ 
  while(1);
  {
    I = listGetData(instrList);

    switch (I->instType)
    {

/*
 * Specialni instrukce
 * -----------------------------------------------------------------------------
 * - stop, read, write, if, if_end, jump, assign, substr, call_fuction, do_while
 */ 
      case I_STOP: // pro zastaveni provadeni cyklu
        return EXIT_SUCCESS;
        break;
      case I_READ: // cteni ze stdin
        // pomocna funkce iRead();
        return EXIT_SUCCESS;
        break;
      case I_WRITE: // vypis na stdout
        // pomocna funkce iWrite();
        return EXIT_SUCCESS;
        break;
      case I_IF: // to bude dobre k rozpoznani zacatku IFu
        return EXIT_SUCCESS;
        break;
      case I_IF_END: // toto zas bude znacit konec IFu, nejak
        return EXIT_SUCCESS;
        break;
      case I_JUMP: // skaceme
        return EXIT_SUCCESS;
        break;
      case I_LABEL: // navesti, kam skaceme
        return EXIT_SUCCESS;
        break;
      case I_ASSIGN: // intrukce prirazeni, operator =
        return EXIT_SUCCESS;
        break;
      case I_SUBSTR: // intrukce ulozi nekam podretezec v zadanem retezci
        return EXIT_SUCCESS;
        break;
      case I_CALL_FUNCTION: // instrukce volani uzivatelskych funkci
        return EXIT_SUCCESS;
        break;
      case I_RETURN: // instrukce navratu z volane funkce
        return EXIT_SUCCESS;
        break;
      case I_DO_WHILE: // instrukce cyklu do-while
        return EXIT_SUCCESS;
        break;

/*
 * Aritmeticke instrukce
 * -----------------------------------------------------------------------------
 * - mul, div, add, sub, con, inc, dec, neg
 */
      case I_MUL: // nasobeni
      // takto nejak by mohla vypadat kazda instrukce
        if (!isIntOrReal) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1)->type == T_INTEGER) && ((operand2)->type == T_INTEGER) ((accumulator)->type == T_INTEGER)
          else ((accumulator)->type == T_REAL) // real a real/int da real

          accumulator->data = operand1->data * operand2->data; 
          return EXIT_SUCCESS; 
        }
        break;
      case I_DIV: // deleni
        return EXIT_SUCCESS;
        break;
      case I_ADD: // scitani
        return EXIT_SUCCESS;
        break;
      case I_SUB: // odcitani
        return EXIT_SUCCESS;
        break;
      case I_CON: // konktarenace, zretezeni
        return EXIT_SUCCESS;
        break;
      case I_INC: // inkrementace
        return EXIT_SUCCESS;
        break;
      case I_DEC: // dekrementace
        return EXIT_SUCCESS;
        break;
      case I_NEG: // negace, bude vubec potreba?
        return EXIT_SUCCESS;
        break;

/*
 * Instrukce porovnani
 * -----------------------------------------------------------------------------
 * - cmp, less, greater, les_equal, greater_equal, equal, not_equal
 */
      case I_CMP: // porovnavani
        return EXIT_SUCCESS;
        break;
      case I_LESS: // je mensi nez
        return EXIT_SUCCESS;
        break;
      case I_GREATER: // je vetsi nez
        return EXIT_SUCCESS;
        break;
      case I_LESS_EQUAL: // je mensi nebo rovno
        return EXIT_SUCCESS;       
        break;
      case I_GREATER_EQUAL: // je vetsi nebo rovno
        return EXIT_SUCCESS;
        break;
      case I_EQUAL: // rovnaji se
        return EXIT_SUCCESS;
        break;
      case I_NOT_EQUAL: // nerovnaji se
        return EXIT_SUCCESS;
        break;

/*
 * Intrukce na vraceni datoveho typu
 * -----------------------------------------------------------------------------
 * tomu teda moc nerozumim..
 * je v ni dalsi switch a pripady pro kazdy ty, zatim nepisu
 */
      case I_TYPE: 
        return EXIT_SUCCESS;
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
        return EXIT_SUCCESS;
        break;
      case I_LENGHT: // Implementuje vestavenou funkci lenght
        return EXIT_SUCCESS;
        break;
      case I_FIND: // implementuje vestavenou funkci find
        return EXIT_SUCCESS;
        break;
      case I_SORT: // implementuje vestavenou funkci sort
        return EXIT_SUCCESS;
        break;
    }
  }  
}   

/*
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 * -----------------------------------------------------------------------------
 */
int interpret(tInstList *)
{
  // ...
}

/*
 * Funkce na kontrolu typu integer a real v operandech 3AK
 * -----------------------------------------------------------------------------
 * - vraci TRUE, jestlize se v operandech vyskytuji pouze typy int a real
 * - jinak vraci FALSE
 */
bool isIntOrReal(void)
{
  if((operand1)->type == T_INTEGER) && ((operand2)->type == T_INTEGER) return TRUE;
  else if((operand1)->type == T_INTEGER) && ((operand2)->type == T_REAL) return TRUE;
  else if((operand1)->type == T_REAL) && ((operand2)->type == T_INTEGER) return TRUE;
  else if((operand1)->type == T_REAL) && ((operand2)->type == T_REAL) return TRUE;
  else return FALSE;  
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
 * na entou, bych si to zas musel rozkouskovavat a stejne bych to pak ukladal 
 * nejakych lokalnich promennych nebo do pole. 
 *
 * Je treba vymyslet, jakym zpusobem dostanu 3AK, a jak ho teda budu zpracovavat.
 * Stale jsem neprisel na to, zda-li je treba dostat veskery kod najednou, nebo 
 * staci vykonavat jednu instrukci po druhe a tudit v kazdem jednom okamziku mi 
 * staci znat jen jeden konkretni 3AK, ktery by se timpadem dal rvat do obycejne
 * struktury, ze ktere bych si ho vzal. No ale kam s vysledky?
 * 
 * Na skakani si udelat pole, do ktereho si budu znacit, kam skakat?
 */
