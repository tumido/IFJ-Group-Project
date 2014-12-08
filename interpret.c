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
#include "ial.h" // kvuli funkci findSubstring a shellSort
#include "ilist.h"

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
 * - stop, read, write, if, then, if_end, jump, assign, substr, call_fuction,
 *   do_while
 */

/*
 * STOP
 * 
 * - I_STOP, NULL, NULL, NULL
 * - slouzi pro zastaveni provadeni cyklu
 */
      case I_STOP:
        break;

/*
 * READ
 *
 * - I_READ, cil, zdroj, NULL
 * - obsluhuje cteni ze stdin
 * - vola pomocnou funkci iRead
 */
      case I_READ: 
        iRead((operand1->data), (operand2->data));
        break;

/*
 * WRITE
 *
 * - I_WRITE, zdroj, NULL, NULL
 * - obsluhuje vypis na stdout
 * - vola pomocnou funkci iWrite
 */
      case I_WRITE:
        iWrite(operand1->data);
        break;

/*
 * IF
 *
 * - I_IF, NULL, NULL, NULL
 * - znaci zacatek podminky
 * - zpracovava vyraz v podmince
 */       
      case I_IF:
        while (tListOfInstr->active != I_THEN) // dokud je co vyhodnocovat
        {
          listNext(tListOfInstr *L); // posun na dalsi prvek seznamu
          // ...
        }
        
        // v tuto chvili mam skrze dalsi intrukce vyhodnocenou podminku,
        // tudiz musim zjistit jak dopadla a podle toho se chovat

        if(/* podminka pravdiva */)
        {
          // vykonavam dokud nenarazim na END_IF
        }
        else /* podminka nepravdiva */
        {
          // preskakuju dokud nenarazim na END_IF
        }
        break;

/*
 * THEN
 *
 * - I_THEN, NULL, NULL, expression_result
 * - znaci konec vyhodnocovani podminky a prechod na telo
 * - vraci vyhodnoceny vyraz podminky (ktery vsak zpracovava instrukce IF)
 */
      case I_THEN: // znaci konec vyhodnocovani podminky a prechod na telo
        if(!(operand1->type == T_BOOLEAN)) return EXIT_TYPE_ERROR;
        else
        {
          if(result->data == FALSE) return FALSE;
          else return TRUE;
        }
        break;

/*
 * IF_END
 *
 * - IF_END, NULL, NULL, NULL
 * - IF_END nic nedela, slouzi pro oznaceni konce tela podminky
 */
      case I_IF_END:

/*
 * JUMP
 *
 * - I_JUMP, NULL, NULL, NULL
 * - oznaceni, ze skaceme
 */
      case I_JUMP:
        break;

/*
 * LABEL
 *
 * - I_LABEL, NULL, NULL, NULL
 * - oznaceni navesti, kam skaceme
 */
      case I_LABEL:
        break;

/*
 * ASSIGN
 *
 * - I_ASSIGN cil, zdroj, NULL
 * - intrukce prirazeni, operator =
 * 
 * je treba doresit, jestli lze pri prirazeni realu do intu automaticky pretypovat, nebo radeji ne
 * kouknout se na to do zadani, pripadne prolezt forum..
 */
      case I_ASSIGN:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) operand1->data = operand2->data;
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL)) operand1->data = operand2->data;
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_BOOLEAN)) operand1->data = operand2->data;
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING)) operand1->data = operand2->data;
        {
          // tady to asi bude slozitejsi, nevim, jestli lze prostoduse priradit string do stringu..
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * SUBSTRING
 * - intrukce ulozi nekam podretezec v zadanem retezci
 */
      case I_SUBSTR: 
        break;

/*
 * CALL FUNCTION
 * - intrukce volani uzivatelskych funkci
 */
      case I_CALL_FUNCTION:
        break;

/*
 * RETURN
 *
 * - I_RETURN, NULL, NULL, NULL
 * - instrukce navratu z volane funkce
 */
      case I_RETURN:
        break;

/*
 * DO WHILE
 * - instrukce cyklu do-while
 */
      case I_DO_WHILE:
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
 * - I_MUL, operand1, operand2, vysledek
 * - vynasobi operand 1 a operand 2, vysledek ulozi do result
 */
      case I_MUL:
        if(!isIntOrReal()) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data * operand2->data;
        }
        break;

/*
 * DIVIDE
 *
 * - I_DIV, operand1, operand2, vysledek
 * - vydeli operand 1 a operand 2, vysledek ulozi do result
 */
      case I_DIV:
        if(!isIntOrReal()) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          if(operand2->data == 0) return EXIT_DIVISION_BY_ZERO_ERROR;
          else result->data = operand1->data / operand2->data;
        }
        break;

/*
 * ADDICTION
 *
 * - I_ADD, operand1, operand2, vysledek
 * - secte operand 1 a operand 2, vysledek ulozi do result
 */
      case I_ADD:
        if(!isIntOrReal()) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data + operand2->data;
        }
        break;

/*
 * SUBTITION
 *
 * - I_SUB, operand1, operand2, vysledek
 * - odecte od operandu1 operand2
 */
      case I_SUB:
        if(!isIntOrReal()) return EXIT_TYPE_ERROR;
        else
        {
          if(()operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data - operand2->data;
        }
        break;

/*
 * CONCATENATE
 *
 * - I_CON, operand1, operand2, vysledek
 * - konkatenace (zretezeni) oprandu 1 a operandu 2
 */
      case I_CON:
        if(!(operand1->type == T_STRING) && (operand2->type == T_STRING)) return EXIT_TYPE_ERROR;
        else
        {
          result->type = T_STRING;
          strcat(result->data, operand1->data);
          strcat(result->data, operand2->data);
        }
        break;

/*
 * INCREMENTATION
 *
 * - I_INC, operand1, NULL, vysledek
 * - inkrementuje operand 1, vysledek v result
 */
      case I_INC:
        if(!(operand1->type == T_INTEGER)) return EXIT_TYPE_ERROR;
        else 
        {
          result->type = T_INTEGER;
          result->data = (operand1->data)++;
        }
        break;

/*
 * DECREMENTATION
 *
 * - I_DEC, operand1, NULL, vysledek
 * - dekrementuje operand 1, vysledek v result
 */
      case I_DEC:
        if(!(operand1->type == T_INTEGER)) return EXIT_TYPE_ERROR;
        else
        {
          result->type = T_INTEGER;
          result->data = (operand1->data)--;
        }
        break;

/*
 * NEGATION
 *
 * - I_NEG, operand1, NULL, vysledek
 * - neguje operand 1, vysledek v result
 */
      case I_NEG:
        if(!(operand1->type == T_BOOLEAN)) return EXIT_TYPE_ERROR;
        else 
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == FALSE)? TRUE : FALSE);
        }
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
 * - I_LESS, operand1, operand2, vysledek
 * - operand 1 je mensi nez operand 2
 */
      case I_LESS:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data < operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data < operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) < 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * GREATER
 *
 * - I_GREATER, operand1, operand2, vysledek
 * - operand 1 je vetsi nez operand 2
 */
      case I_GREATER:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data > operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data > operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) > 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * LESS OR EQUAL
 *
 * - I_LESS_EQUAL, operand1, operand2, vysledek
 * - operand 1 je mensi nebo roven operandu 2
 */
      case I_LESS_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data <= operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_REAL))
        {
          result->type = T_REAL;
          result->data = ((operand1->data <= operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) <= 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * GREATER OR EQUAL
 *
 * - I_GREATER_EQUAL, operand1, operand2, vysledek
 * - operand 1 je vetsi nebo roven operandu 2
 */
      case I_GREATER_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data >= operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data >= operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) >= 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * EQUAL
 *
 * - I_EQUAL, operand1, operand2, vysledek
 * - operand 1 je roven operandu 2
 */
      case I_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_BOOLEAN))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) == 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * NOT EQUAL
 *
 * - I_NOT_EQUAL, operand1, operand2, vysledek
 * operand 1 neni roven (je ruzny od) operandu 2
 */
      case I_NOT_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_BOOLEAN))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) != 0)? TRUE : FALSE);
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * -----------------------------------------------------------------------------
 * Intrukce na vraceni datoveho typu
 * -----------------------------------------------------------------------------
 * tomu teda moc nerozumim..
 * je v ni dalsi switch a pripady pro kazdy typ, zatim nepisu
 * treba ani nebude potreba
 */
      case I_TYPE:
        break;

/*
 * -----------------------------------------------------------------------------
 * Instrukce implenetujici vestavene funkce
 * -----------------------------------------------------------------------------
 * - copy, lenght, find, sort
 */

/*
 * COPY
 * !!! - syntaxi intrukce musi doplnit KUBA, nechce se mi to snazit chapat
 * !!! - opravit indexovani (od 1)
 *
 * - copy(s : string; i : integer; n : integer) : string
 * - vrati podretezec zadaneho retezce 's'
 * - 'i' udava zacatek pozadovaneho podretezce (pocitano od 1)
 * - 'n' urcuje delku podretezce
 */
      case I_COPY:
        if(!((operand1->type == T_STRING) && (operand2->type == T_REAL))) return EXIT_TYPE_ERROR;
        else if(!(operand1->data)) return EXIT_NOT_INIT_ERROR;
        else
        {
          /*
           * budu vychaztet zhruba z neceho takoveho: fceCopy(char* textCopy, int POCATECNI_PISMENO, int DELKA_PODRETEZCE) - pro pozdejsi hromadne prepsani
           * vysledek bude PROZATIM ulozen v poli, ze ktereho si muzu vysledek ulozit kam budu chtit
           * jakmile se dohodne zpusob predani paramteru (respektive jak to v tom triadresnym kodu prijde), udelam i zpracovani parametru
           * prozatim budu uvazovat, ze uz mam dva integery, ktere mi urcuji podminky
           */
          int arrayLenght = strlen(textCopy);
          int arraySize = arrayLenght - POCATECNI_PISMENO - 2;                 // magicka konstanta zde vyrovnava deficit ypusobeny praci s indexy -- prechod mezi poctem pismen a poctem indexu, plati pro vsechny magicke konstanty
          char arrayCopy [arraySize];
          for(int i = 0; i <= arraySize; i++)            //inicializace vysledneho pole
            {arrayCopy[i] = '\0';}
          for(int i = 0; i <= DELKA_PODRETEZCE; i++)                        //prepsani stringu do charu a vyhoyeni vysledku
            arrayCopy[i] = textCopy[POCATECNI_PISMENO - 1];
            POCATECNI_PISMENO++;
            if(textCopy[i] == '\0')
              i = DELKA_PODRETEZCE;
          }
          /*
           * v tuto chvili by to melo byt odolne i na chyby kdz chci najit podretezec delsi nez samotny yakladni retezec (testovano u me na PC, ne na eve)
           * Ted uz jen staci ulozit vysledek na adresu urcenou ?triadresnym kodem?
           */
        }
        // vymyslet zpusob, jak v standardne 3 adresnem kodu predat krom instrukce
        // jeste 3 dalsi informace
        // co takle si cisla 'i' a 'n' dat dohromady a oddelit teckou - udelat z
        // nich vlastne real, ktery si pak rozkouskuju na dva integery a zahodim
        // tecku? :) Momentalne me nanapada nic rozumnejsiho..
        break;

/*
 * LENGHT
 *
 * - I_LENGHT, retezec, NULL, vysledek
 * - length(s : string) : integer
 * - vrati delku retezce zadaneho parametrem 's'
 */
      case I_LENGHT:
        if(!(operand1->type == T_STRING)) return EXIT_TYPE_ERROR;
        else if(!(operand1->data)) return EXIT_NOT_INIT_ERROR;
        else
        {
          result->type = T_INTEGER;
          result->data = strlen(operand1->data);
        }
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
        if(!((operand1->type == T_STRING) && (operand2->type == T_STRING))) return EXIT_TYPE_ERROR;
        else if((!(operand1->data)) || (!(operand2->data))) return EXIT_NOT_INIT_ERROR;
        else if(strlen(operand2->data) > strlen(operand1->data)) return EXIT_RUNTIME_ERROR;
        else
        {
          result->type = T_INTEGER;
          result->data = findSubtring();
        }
        break;

/*
 * SORT
 *
 * - I_SORT, retezec, NULL, vysledek
 * - sort(s : string) : string
 * - seradi znaky v retezci 's' tak, aby znak s nizsi ordinarni
 *   hodnotou vzdy predchazel znaku s vyssi ordinarni hodnotou
 * - vraci retezec obsahujici serazene znaky
 * - implementovano pomoci algoritmu shell sort
 */
      case I_SORT:
        // string je vlastne pole znaku, ze?
        // takze staci predat ukazatel na string a spocitat jeho delku, kvuli promenne 'n' v shellSortu

        if(!(operand1->type == T_STRING) return EXIT_TYPE_ERROR;
        else if(!(operand1->data)) return EXIT_NOT_INIT_ERROR;
        else
        {
          if(strlen(operand1->data) == 1) result->data = operand1->data; // retezec ma delku 1, neni co radit
          else
          {
            int n = strlen(operand1->data);

            result->type = T_STRING;
            result->data = (shellSort(*operand1, n));
          }
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
 * isIntOrReal
 * -----------------------------------------------------------------------------
 * - funkce na kontrolu typu integer a real v operandech 3AK
 * - vraci TRUE, jestlize se v operandech vyskytuji pouze typy int a real
 * - jinak vraci FALSE
 */
bool isIntOrReal(void)
{
  if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) return TRUE;
  else if((operand1->type == T_INTEGER) && (operand2->type == T_REAL)) return TRUE;
  else if((operand1->type == T_REAL) && (operand2->type == T_INTEGER)) return TRUE;
  else if((operand1->type == T_REAL) && (operand2->type == T_REAL)) return TRUE;
  else return FALSE;
}

/*
 * iWRITE
 * -----------------------------------------------------------------------------
 * - zapisuje na stdout
 */
int iWrite(operand1->data)
{
  switch(operand1->type)
  {
    case T_INTEGER:
      printf("%d\n", operand1->data);
      break;
    case T_REAL:
      printf("%f\n", operand1->data);
      break;
    case T_STRING:
      printf("%s\n", operand1->data);
      break;
    case T_BOOLEAN:
    default:
      return EXIT_TYPE_ERROR;
    break;
  }
}

/*
 * iREAD
 * -----------------------------------------------------------------------------
 * - cte ze stdin
 */
int iRead();
{
  // ... doplnit
  switch(source->type)
  {
    case T_INTEGER:
      break;
    case T_REAL:
      break;
    case T_STRING:
      break;
    case T_BOOLEAN:
    default:
      return EXIT_TYPE_ERROR;
    break;
  }
}

/*
 * TODO:
 * - kodit, makat, modlit se
 * - nejakym zpusobem vyzkouset praci se zasobnikem a frontou, nejlepe si to
 *   zkusit nejak nasimulovat..
 * - dokoncit zbyvajici instrukce
 */
