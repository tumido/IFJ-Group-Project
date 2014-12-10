/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

/*
 * -----------------------------------------------------------------------------
 * NOVY ZPUSOB SYNTAXE INSTRUKCI
 * -----------------------------------------------------------------------------
 * INSTRUKCE, typ (k_int || k_string || k_bool || k_real || (k_function)), op1, op2, cilovyOperand
 */

#include "interpret.h"

/*
 * Interpretace konkretni instrukce
 * -----------------------------------------------------------------------------
 * - specialni instrukce
 * - aritmeticke instrukce
 * - instrukce porovnani
 * - instrukce vestavenych funkci
 *
 * - u instrukci pracujicimi s pouze jednim datovym I->instTypem netreba tento datovy
 *   typ znat, predpokladam osetreni parserem, ze posila pouze platny datovy typ
 */
int instruction(tListOfInstr *instrList)
{
  while(1)
  {
    tInstr * I = listGetData(instrList);

    switch (I->instType)
    {

/*
 * -----------------------------------------------------------------------------
 * Specialni instrukce
 * -----------------------------------------------------------------------------
 * - read, write, jump, assign, call_fuction, clear
 */

/*
 * READ
 *
 * - I_READ, typ, zdroj, NULL, cil
 * - obsluhuje cteni ze stdin
 * - vola pomocnou funkci iRead
 */
      case I_READ:
        iRead(I);
        break;

/*
 * WRITE
 *
 * - I_WRITE, typ, zdroj, NULL, NULL
 * - obsluhuje vypis na stdout
 * - vola pomocnou funkci iWrite
 */
      case I_WRITE:
        iWrite(I);        //predavam promennou I, do ktere jsem na zacatku teto funkce ulozil aktivni instrukci
        break;

/*
 * JUMP
 *
 * - I_JUMP, NULL, I->addr1, I->addr2, NULL
 * - operand 1 rozhoduje o skoku, operand 2 je adresa instrukce, na kterou skaceme  
 */
      case I_JUMP:
          if(((*(bool*))I->addr1) == TRUE) return EXIT_SUCCESS;
          else
          {
             // jump na adresu danou operandem 2
          }
        break;

/*
 * ASSIGN
 *
 * - I_ASSIGN, typ, zdroj, NULL, cil
 * - intrukce prirazeni, operator =
 * - do cile (I->addr3) priradi hodnotu operandu 1
 */
      case I_ASSIGN:
        if(type == k_int) (*(int*)result) = (*(int*)operand1);
        else if(type == k_real) (*(double*)result) = (*(double*)operand1);
        else if(type == k_bool) (*(bool*)result) = (*(bool*)operand1);
        else if(type == k_string)
        {
          if (((string *)result)->alloc < ((string *)operand1)->alloc)
          {
            ((string *)result)->alloc = ((string *)operand1)->alloc;
            if ((((string *)result)->str = realloc(sizeof(char) * ((string *)operand1)->alloc)) == NULL)
              return EXIT_INTERNAL_ERROR;
          }
          strncpy(((string *)result)->str, ((string *)operand1)->str, ((string *)result)->alloc);
          ((string *)result)->length = ((string *)operand1)->length;
        }
        break;

/*
 * CALL FUNCTION
 * - intrukce volani uzivatelskych funkci
 */
      case I_CALL_FUNCTION:
        break;

/*
 * CLEAR
 *
 * - I_CLEAR, typ, I->addr1, NULL, NULL
 * - uvolni obsah operandu 1
 */
      case I_CLEAR:
        if(type == t_string)
          free(((string *)operand1)->str);
        free(operand1);
        break;

/*
 * -----------------------------------------------------------------------------
 * Aritmeticke instrukce
 * -----------------------------------------------------------------------------
 * - mul, div, add, sub, con, inc, dec, neg
 */

 /*
 * MULTIPLY
 *
 * - I_MUL, typ, I->addr1, I->addr2, vysledek
 * - vynasobi operand 1 a operand 2, vysledek ulozi do I->addr3
 */
      case I_MUL:
        if(I->instType == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) * (*(int*)I->addr2);
        else if(I->instType == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) * (*(double*)I->addr2);
        else return EXIT_RUNTIME_ERROR;
        break;

/*
 * DIVIDE
 *
 * - I_DIV, typ, I->addr1, I->addr2, vysledek
 * - vydeli operand 1 a operand 2, vysledek ulozi do I->addr3
 * - pozor, vysledek bude vzdy REAL!
 */
      case I_DIV:
        if(((*(int*)I->addr2) == 0) || ((*(double*)I->addr2) == 0.0)) return EXIT_DIVISION_BY_ZERO_ERROR; // nulou delit neumime
        else
        {
          if(I->instType == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) / (*(int*)I->addr2);
          else if(I->instType == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) / (*(double*)I->addr2);
          else return EXIT_RUNTIME_ERROR;
        }
        break;

/*
 * ADDICTION
 *
 * - I_ADD, typ, I->addr1, I->addr2, vysledek
 * - secte operand 1 a operand 2, vysledek ulozi do I->addr3
 */
      case I_ADD:
          if(I->instType == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) + (*(int*)I->addr2);
          else if(I->instType == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) + (*(double*)I->addr2);
          else return EXIT_RUNTIME_ERROR;
        break;

/*
 * SUBTITION
 *
 * - I_SUB, typ, I->addr1, I->addr2, vysledek
 * - odecte od operandu1 I->addr2
 */
      case I_SUB:
          if(I->instType == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) - (*(int*)I->addr2);
          else if(I->instType == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) - (*(double*)I->addr2);
          else return EXIT_RUNTIME_ERROR;
        break;

/*
 * CONCATENATE
 *
 * - I_CON, NULL, I->addr1, I->addr2, vysledek
 * - konkatenace (zretezeni) oprandu 1 a operandu 2
 */
      case I_CON:
          strcat((*(char*)I->addr3), (*(char*)I->addr1); // tady opravdu nevim, jestli je to dobre/asi ROMAN..,,,,,,,,ja bych to udelal stejne / KUBA
          strcat((*(char*)I->addr3), (*(char*)I->addr2);
        break;

/*
 * INCREMENTATION
 *
 * - I_INC, NULL, I->addr1, NULL, vysledek
 * - inkrementuje operand 1, vysledek v I->addr3
 */
      case I_INC:
          (*(int*)I->addr3) = ++(*(int*)I->addr1);
        break;

/*
 * DECREMENTATION
 *
 * - I_DEC, NULL, I->addr1, NULL, vysledek
 * - dekrementuje operand 1, vysledek v I->addr3
 */
      case I_DEC:
          (*(int*)I->addr3) = --(*(int*)I->addr1);
        break;

/*
 * NEGATION
 *
 * - I_NEG, typ, I->addr1, NULL, vysledek
 * - neguje operand 1, vysledek v I->addr3
 */
      case I_NEG:
          if(I->instType == k_bool)(*(bool*)I->addr3) = (((*(bool*)I->addr1) == FALSE)? TRUE : FALSE);
          else if(I->instType == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) - 2 * (*(int*)I->addr1);  //negace integeru by Kuba, muze byt?
        break;

/*
 * -----------------------------------------------------------------------------
 * Instrukce porovnani
 * -----------------------------------------------------------------------------
 * - less, greater, les_equal, greater_equal, equal, not_equal
 */

/*
 * LESS
 *
 * - I_LESS, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je mensi nez operand 2
 */
      case I_LESS:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) < (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) < (*(double*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) < 0)? TRUE : FALSE);
        break;

/*
 * GREATER
 *
 * - I_GREATER, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je vetsi nez operand 2
 */
      case I_GREATER:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) > (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) > (*(double*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) > 0)? TRUE : FALSE);
        break;


/*
 * LESS OR EQUAL
 *
 * - I_LESS_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je mensi nebo roven operandu 2
 */
      case I_LESS_EQUAL:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) <= (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) <= (*(double*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) <= 0)? TRUE : FALSE);
        break;


/*
 * GREATER OR EQUAL
 *
 * - I_GREATER_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je vetsi nebo roven operandu 2
 */
      case I_GREATER_EQUAL:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) >= (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) >= (*(double*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) >= 0)? TRUE : FALSE);
        break;


/*
 * EQUAL
 *
 * - I_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je roven operandu 2
 */
      case I_EQUAL:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) == (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) == (*(double*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_bool) (*(bool*)I->addr3) = (((*(bool*)I->addr1) == (*(bool*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) == 0)? TRUE : FALSE);
        break;

/*
 * NOT EQUAL
 *
 * - I_NOT_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 neni roven (je ruzny od) operandu 2
 */
      case I_NOT_EQUAL:
        if(I->instType == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) != (*(int*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) != (*(double*)I->addr2))? TRUE : FALSE);
        else if(I->instType == k_bool) (*(bool*)I->addr3) = (((*(bool*)I->addr1) != (*(bool*)I->addr2))? TRUE : FALSE);
        else (I->instType == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) != 0)? TRUE : FALSE);
        break;

/*
 * -----------------------------------------------------------------------------
 * Instrukce implenetujici vestavene funkce
 * -----------------------------------------------------------------------------
 * - copy, lenght, find, sort
 */

/*
 * COPY
 *
 * I_COPY, NULL, I->addr1, I->addr2, I->addr3
 * I->addr1 - retezec ze ktereho budu kopirovat
 * I->addr2 - struktura o dvou integerech
 * I->addr3 - misto kam nahraji vysledny podretezec
 *
 * - copy(s : string; i : integer; n : integer) : string
 * - vrati podretezec zadaneho retezce 's'
 * - 'i' udava zacatek pozadovaneho podretezce (pocitano od 1)
 * - 'n' urcuje delku podretezce
 */
      case I_COPY:
        int start = L->addr2->start;                    //promene start a length dostanu decodovanim druheho operandu, urcuji, ktera cast retezce bude zkopirovana
        int length = L->addr2->length;
        int arrayLenght = strlen(textCopy);             //zjistim si, jak dlouhy retezec mi prisel
        int arraySize = arrayLenght - length - 1;       // promena, ktera yjisti, jak dlouhe pole budu potrebovat, urcene podle delky ocekavaneho podretezce,magicka konstanta zde vyrovnava deficit ypusobeny praci s indexy -- prechod mezi poctem pismen a poctem indexu
        char arrayCopy [arraySize];
        for(int i = 0; i <= arraySize; i++)             //inicializace vysledneho pole
          {arrayCopy[i] = '\0';}
        for(int i = 0; i <= length; i++)                //prepsani stringu do charu a vyhoyeni vysledku
          {
            arrayCopy[i] = textCopy[start];
            start++;
            if(textCopy[i] == '\0')
              i = length;
          }
        //vysledek je momentalne ulozen v arrayCopy. Ted bych ho jen prekopiroval na I->addr3, kde by bylo nachystane pole (tohle je zatim jediny zpusob predani vysledku, ktery me naoadl)
        /*
         * v tuto chvili by to melo byt odolne i na chyby kdz chci najit podretezec delsi nez samotny yakladni retezec (testovano u me na PC, ne na eve)
         * Ted uz jen staci ulozit vysledek na adresu urcenou ?triadresnym kodem?
         * na 100% je potreba tuto funkci otestovat, promene jsem prepisoval na dvakrat, nejsem is jist ze jsou prepsane spravne
         */
        break;

/*
 * LENGHT
 *
 * - I_LENGHT, NULL, retezec, NULL, vysledek
 * - length(s : string) : integer
 * - vrati delku retezce zadaneho parametrem 's'
 */
      case I_LENGHT:
         (*(int*)I->addr3) = strlen((*(char*)I->addr1));
        break;

/*
 * FIND
 *
 * - I_FIND, zadany_retezec, hledany_podretezec, vysledek
 * - find(s : string; search : string) : integer
 * - vyhleda prvni vyskyt zadaneho podretezce 'search' v retezci 's' a vrati jeho pozici (pocitano od 1)
 * - pokud neni podretezec nalezen, vraci 0
 * - implementovano pomoci Boyer-Moorova algoritmu
 */
      case I_FIND:
          (*(char*)I->addr3) = findSubtring();
        break;

/*
 * SORT
 *
 * - I_SORT, NULL, retezec, NULL, vysledek
 * - sort(s : string) : string
 * - seradi znaky v retezci 's' tak, aby znak s nizsi ordinarni
 *   hodnotou vzdy predchazel znaku s vyssi ordinarni hodnotou
 * - vraci retezec obsahujici serazene znaky
 * - implementovano pomoci algoritmu shell sort
 */
      case I_SORT:
        // string je vlastne pole znaku, ze?
        // takze staci predat ukazatel na string a spocitat jeho delku, kvuli promenne 'n' v shellSortu

        if(strlen(*(int*)I->addr1) == 1) (*(char*)I->addr3) = (*(char*)I->addr1); // retezec ma delku 1, neni co radit
        else
        {
          int n = strlen(*(char*)I->addr1);

          (*(char*)I->addr3) = (shellSort((*(char*)I->addr1), n));
        }
        break;
    }
    return EXIT_SUCCESS;
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
 * iWRITE
 * -----------------------------------------------------------------------------
 * - zapisuje na stdout
 *
 * - má se nějak vypisovat i bool? třeba jen jako 0 a 1??
 */
int iWrite(tListOfInstr *L)
{
  switch(I->instType)
    case k_int:
      printf("%d", (*(int*))I->addr1);
      break;

    case k_real:
      printf("%g", (*(double*))I->addr1);
      break;

    case k_string:
      printf("%c", (*(char*))I->addr1);
      break;

  return EXIT_SUCCESS;
}

/*
 * iREAD
 * -----------------------------------------------------------------------------
 * - cte ze stdin
 * - musi jeste umet preskakovat mezery a tabulatory az do konce radku/vstupu
 */
int iRead(tListOfInstr *L);
{
  switch(I->instType)
    case k_int:
      int num = 0;
      I->addr3 = scanf("%d" &num);
      break;

    case k_real:
      double num = 0.0;
      I->addr3 = scanf("%lf" &num);
      break;

    case k_string:
      char ch[];
      char tmp[256];
      I->addr3 = scanf("%c" &ch);

      while(ch = getchar() != (EOF || '\n'))
      {
        strcat(tmp, ch);
      }
      strcpy(I->addr3, tmp); 
      break;

  return EXIT_SUCCESS;
}

/*
 * TODO:
 * - kodit, makat, modlit se
 * - nejakym zpusobem vyzkouset praci se zasobnikem a frontou, nejlepe si to
 *   zkusit nejak nasimulovat..
 * - dokoncit zbyvajici instrukce
 *
 * - metodu BMA mame implmentovat dle varianty algoritmu popsane v ramci predmetu IAL
 *   takze cele prepsat do nejakeho prasackeho na dve veci reseni...
 *   - tezky nezdar, v opore je v algoritmu chyba, podle me.. tak jsem na rozpacich,
 *     jestli se to snazit implementovat s chybou, a pak ji nejak opravovat, nebo jak..
 */
