/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
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
        iRead(&I);
        break;

/*
 * WRITE
 *
 * - I_WRITE, typ, zdroj, NULL, NULL
 * - obsluhuje vypis na stdout
 * - vola pomocnou funkci iWrite
 */
      case I_WRITE:
        iWrite(&I);        //predavam promennou I, do ktere jsem na zacatku teto funkce ulozil aktivni instrukci
        break;

/*
 * JUMP
 *
 * - I_JUMP, NULL, I->addr1, I->addr2, NULL
 * - operand 1 rozhoduje o skoku, operand 2 je adresa instrukce, na kterou skaceme
 * - zkusim aspon yhruba nacrtnout, jak by to melo vypadat
 */
      case I_JUMP:
        if(*(bool*)I->addr1 == true) return EXIT_SUCCESS;
        if(I->instType == I_IF)
        {
          while(I->instType != I_THEN)/*dokud nenarazim na then tak vyhodnoucji podminku*/
          {
            listNext(&I);
            instruction(&I);
          } /*prislo mi I_THEN, takze budu skakat podle vysledku podminky*/
          if(/*podminka byla pravdiva*/)
          {
            while(I->instType != I_ELSE)  /* nacitam instrukce dokud neni ELSE */
            {
              listNext(&I);             /* posunu aktivitu dal */
              instruction(&I);          /* vykonam instrukci */
            }
            while(I->instType != I_END_IF){listNext(&I);}   /* prisel else takze preskakuju dokud neni konec ifu */
            listNext(&I);         /* prisel end if a jdu dal */
            instruction(&I);
          }
          else /* podminka nebyla pravdiva*/
          {
            while(I->instType != I_ELSE){listNext(&I);}     /*preskakuju cast bloku, ktera se nema porvadet*/
            while(I->instType != I_END_IF)
            {
              listNext(&I);             /* posunu aktivitu dal */
              instruction(&I);          /* vykonam instrukci */
            }
          }/* konec ELSE*/
        }/* konec IF*/
        else
        {
          listGoto(&I, I->addr2);           //nastavim aktivitu na instrukci urcenou adresou v addr2
          instruction(/*aktualni instrukce*/);
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
        if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1);
        else if(I->type == k_real) (*(double*)I->addr3) = (*(double*)I->addr1);
        else if(I->type == k_bool) (*(bool*)I->addr3) = (*(bool*)I->addr1);
        else if(I->type == k_string)
        {
          if (((string *)I->addr3)->alloc < ((string *)I->addr1)->alloc)
          {
            ((string *)I->addr3)->alloc = ((string *)I->addr1)->alloc;
            if (((string *)I->addr3)->str = realloc((sizeof(char) * ((string *)I->addr1)->alloc)) == NULL)
              return EXIT_INTERNAL_ERROR;
          }
          strncpy(((string *)I->addr3)->str, ((string *)I->addr1)->str, ((string *)I->addr3)->alloc);
          ((string *)I->addr3)->length = ((string *)I->addr1)->length;
        }
        break;

/*
 * CALL FUNCTION
 * - intrukce volani uzivatelskych funkci
 */
      case I_CALL_FUNCTION:
        stack intstack;
        stackInit(&intstack);                       //inicializace zasobniku
        break;

/*
 * CLEAR
 *
 * - I_CLEAR, typ, I->addr1, NULL, NULL
 * - uvolni obsah operandu 1
 */
      case I_CLEAR:
        if(I->type == k_string)
          free(((string *)I->addr1)->str);
        free(I->addr1);
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
        if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) * (*(int*)I->addr2);
        else if(I->type == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) * (*(double*)I->addr2);
        else return EXIT_RUNTIME_ERROR; // kde se to tu vzalo a proc to tu je? neresi to parser?
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
          if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) / (*(int*)I->addr2);
          else if(I->type == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) / (*(double*)I->addr2);
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
        if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) + (*(int*)I->addr2);
        else if(type == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) + (*(double*)I->addr2);
        else return EXIT_RUNTIME_ERROR;
        break;

/*
 * SUBTITION
 *
 * - I_SUB, typ, I->addr1, I->addr2, vysledek
 * - odecte od operandu1 I->addr2
 */
      case I_SUB:
        if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) - (*(int*)I->addr2);
        else if(I->type == k_real) (*(double*)I->addr3) = (*(double*)I->addr1) - (*(double*)I->addr2);
        else return EXIT_RUNTIME_ERROR;
        break;

/*
 * CONCATENATE
 *
 * - I_CON, NULL, I->addr1, I->addr2, vysledek
 * - konkatenace (zretezeni) oprandu 1 a operandu 2
 */
      case I_CON:
        strcat((*(char*)I->addr3), (*(char*)I->addr1)); // tady opravdu nevim, jestli je to dobre/asi ROMAN..,,,,,,,,ja bych to udelal stejne / KUBA
        strcat((*(char*)I->addr3), (*(char*)I->addr2));
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
        if(I->type == k_bool)(*(bool*)I->addr3) = (((*(bool*)I->addr1) == false)? true : false);
        else if(I->type == k_int) (*(int*)I->addr3) = (*(int*)I->addr1) - 2 * (*(int*)I->addr1);  //negace integeru by Kuba, muze byt?
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
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) < (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) < (*(double*)I->addr2))? true : false);
        else if(I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) < 0)? true : false);
        break;

/*
 * GREATER
 *
 * - I_GREATER, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je vetsi nez operand 2
 */
      case I_GREATER:
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) > (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) > (*(double*)I->addr2))? true : false);
        else if (I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) > 0)? true : false);
        break;


/*
 * LESS OR EQUAL
 *
 * - I_LESS_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je mensi nebo roven operandu 2
 */
      case I_LESS_EQUAL:
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) <= (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) <= (*(double*)I->addr2))? true : false);
        else if(I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) <= 0)? true : false);
        break;


/*
 * GREATER OR EQUAL
 *
 * - I_GREATER_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je vetsi nebo roven operandu 2
 */
      case I_GREATER_EQUAL:
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) >= (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) >= (*(double*)I->addr2))? true : false);
        else if(I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) >= 0)? true : false);
        break;


/*
 * EQUAL
 *
 * - I_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 je roven operandu 2
 */
      case I_EQUAL:
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) == (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) == (*(double*)I->addr2))? true : false);
        else if(I->type == k_bool) (*(bool*)I->addr3) = (((*(bool*)I->addr1) == (*(bool*)I->addr2))? true : false);
        else if(I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) == 0)? true : false);
        break;

/*
 * NOT EQUAL
 *
 * - I_NOT_EQUAL, typ, I->addr1, I->addr2, vysledek
 * - operand 1 neni roven (je ruzny od) operandu 2
 */
      case I_NOT_EQUAL:
        if(I->type == k_int) (*(bool*)I->addr3) = (((*(int*)I->addr1) != (*(int*)I->addr2))? true : false);
        else if(I->type == k_real) (*(bool*)I->addr3) = (((*(double*)I->addr1) != (*(double*)I->addr2))? true : false);
        else if(I->type == k_bool) (*(bool*)I->addr3) = (((*(bool*)I->addr1) != (*(bool*)I->addr2))? true : false);
        else if(I->type == k_string) (*(bool*)I->addr3) = (((strcmp((*(char*)I->addr1), (*(char*)I->addr2))) != 0)? true : false);
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
        int start = I->addr2->start;                    //promene start a length dostanu decodovanim druheho operandu, urcuji, ktera cast retezce bude zkopirovana
        int length = I->addr2->length;
        int arrayLenght = strlen((string *)I->addr1);             //zjistim si, jak dlouhy retezec mi prisel
        int arraySize = arrayLenght - length - 1;       // promena, ktera yjisti, jak dlouhe pole budu potrebovat, urcene podle delky ocekavaneho podretezce,magicka konstanta zde vyrovnava deficit ypusobeny praci s indexy -- prechod mezi poctem pismen a poctem indexu
        char arrayCopy [arraySize];
        for(int i = 0; i <= arraySize; i++)             //inicializace vysledneho pole
          {arrayCopy[i] = '\0';}
        for(int j = 0; j <= length; j++)                //prepsani stringu do charu a vyhoyeni vysledku
          {
            arrayCopy[j] = I->addr1[start];
            start++;
            if(I->addr1[j] == '\0')
              j = length;
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
        (*(int*)I->addr3) = ((string *)I->addr2)->length;
        break;

/*
 * FIND
 *
 * - I_FIND, zadany_retezec, hledany_podretezec, vysledek
 * - find(s : string; search : string) : integer
 * - vyhleda prvni vyskyt zadaneho podretezce 'search' v retezci 's' a vrati jeho pozici (pocitano od 1)
 * - pokud neni podretezec nalezen, vraci -1
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
 * -----------------------------------------------------------------------------
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 * -----------------------------------------------------------------------------
 * - jak to tak vypada, nebude vubec potreba..
 */
int interpret(tListOfInstr *)
{
  // ...
}

/*
 * iWRITE
 * -----------------------------------------------------------------------------
 * - vypisuje hodnotu addr1
 */
int iWrite(tListOfInstr *I)
{
  switch(I->instType)
  {
    case k_int:
      printf("%d", (*(int*))I->addr1);
      break;

    case k_real:
      printf("%g", (*(double*))I->addr1);
      break;

    case k_string:
      printf("%s", ((string *)I->addr1)->str);
      break;
  }

  return EXIT_SUCCESS;
}

/*
 * iREAD
 * -----------------------------------------------------------------------------
 * - cte ze stdin
 * - musi jeste umet preskakovat mezery a tabulatory az do konce radku/vstupu
 */
int iRead(tInstr *I)
{
  switch(I->instType)
  {
    case k_int:
      int num = 0;
      I->addr3 = scanf("%d", &num);
      break;

    case k_real:
      double numb = 0.0;
      I->addr3 = scanf("%lf", &numb);
      break;

    case k_string:
      char ch[];
      char tmp[256];
      I->addr3 = scanf("%c", &ch);

      while(ch = getchar() != (EOF || '\n'))
        strcat(tmp, ch);
      strcpy(I->addr3, tmp);
      break;
  }
  return EXIT_SUCCESS;
}

/*
 * TODO:
 * - kodit, makat, modlit se
 * - dokoncit zbyvajici instrukce
 * - odzkouset instrukce vestavenych funkci
 */
