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
        iRead(((tVarValue*) operand1->data), ((tVarValue*) operand2->data));
        return EXIT_SUCCESS;
        break;
      case I_WRITE: // vypis na stdout
        iWrite((tVarValue*) result->data);
        return EXIT_SUCCESS;
        break;
      case I_IF: // to bude dobre k rozpoznani zacatku IFu
        return EXIT_SUCCESS;
        break;
      case I_IF_END: // toto zas bude znacit konec IFu, nejak
      case I_JUMP: // skaceme
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

 /*
 * MULTIPLY
 * vynasobi operand 1 a operand 2, vysledek ulozi do acc
 */
      case I_MUL:
        if (!isIntOrReal) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data * operand2->data;
          return EXIT_SUCCESS;
        }
        break;

/*
 * DIVIDE
 * vydeli operand 1 a operand 2, vysledek ulozi do acc
 */
      case I_DIV:
        if (!isIntOrReal) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          if(operand2->data == 0) return EXIT_DIVISION_BY_ZERO_ERROR;
          else
          {
            result->data = operand1->data / operand2->data;
            return EXIT_SUCCESS;
          }
        }
        break;

/*
 * ADDICTION
 * secte operand 1 a operand 2, vysledek ulozi do acc
 */
      case I_ADD:
        if (!isIntOrReal) return EXIT_TYPE_ERROR;
        else
        {
          if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data + operand2->data;
          return EXIT_SUCCESS;
        }
        break;

/*
 * SUBTITION
 * odecte operand 1 a operand 2, vysledek ulozi do acc
 */
      case I_SUB:
        if (!isIntOrReal) return EXIT_TYPE_ERROR;
        else
        {
          if(()operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) result->type = T_INTEGER;
          else result->type = T_REAL; // real a real/int da real

          result->data = operand1->data - operand2->data;
          return EXIT_SUCCESS;
        }
        break;

/*
 * CONCATENATE
 * konkatenace (zretezeni) oprandu 1 a operandu 2
 */
      case I_CON:
        if(!(operand1->type == T_STRING) && (operand2->type == T_STRING)) return EXIT_TYPE_ERROR;
        else
        {
          result->type = T_STRING;
          strcat(result->data, operand1->data);
          strcat(result->data, operand2->data);
        }
        return EXIT_SUCCESS;
        break;

/*
 * INCREMENTATION
 * inkrementuje operand 2, vysledek v operandu 2
 */
      case I_INC: // inkrementace
        if(!(operand2->type == T_INTEGER)) return EXIT_TYPE_ERROR;
        else
        {
          operand2->data = (operand2->data)++;
          return EXIT_SUCCESS;
        }
        break;

/*
 * DECREMENTATION
 * inkrementuje operand 2, vysledek v operandu 2
 */
      case I_DEC: // inkrementace
        if(!(operand2->type == T_INTEGER)) return EXIT_TYPE_ERROR;
        else
        {
          operand2->data = (operand2->data)--;
          return EXIT_SUCCESS;
        }
        break;

/*
 * NEGATION
 * neguje operand 2, vysledek v operandu 2
 */
      case I_NEG: // negace, bude vubec potreba?
        if(!(operand2->type == T_BOOLEAN)) return EXIT_TYPE_ERROR;
        else
        {
          operand2->data = ((operand2->data == FALSE)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        break;

/*
 * Instrukce porovnani
 * -----------------------------------------------------------------------------
 * - less, greater, les_equal, greater_equal, equal, not_equal
 */

/*
 * LESS
 * operand 1 je mensi nez operand 2
 */
      case I_LESS:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data < operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data < operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) < 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * GREATER
 * operand 1 je vetsi nez operand 2
 */
      case I_GREATER:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data > operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data > operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) > 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * LESS OR EQUAL
 * operand 1 je mensi nebo roven operandu 2
 */
      case I_LESS_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data <= operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_REAL))
        {
          result->type = T_REAL;
          result->data = ((operand1->data <= operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) <= 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * GREATER OR EQUAL
 * operand 1 je vetsi nebo roven operandu 2
 */
      case I_GREATER_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data >= operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data >= operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) >= 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * EQUAL
 * operand 1 je roven operandu 2
 */
      case I_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_BOOLEAN))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data == operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) == 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * NOT EQUAL
 * operand 1 neni roven operandu 2
 */
      case I_NOT_EQUAL:
        if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_REAL) && (operand2->type == T_REAL))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_BOOLEAN) && (operand2->type == T_BOOLEAN))
        {
          result->type = T_BOOLEAN;
          result->data = ((operand1->data != operand2->data)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else if((operand1->type == T_STRING) && (operand2->type == T_STRING))
        {
          result->type = T_BOOLEAN;
          result->data = (((strcmp(operand1->data, operand2->data)) != 0)? TRUE : FALSE);
          return EXIT_SUCCESS;
        }
        else return EXIT_TYPE_ERROR;
        break;

/*
 * Intrukce na vraceni datoveho typu
 * -----------------------------------------------------------------------------
 * tomu teda moc nerozumim..
 * je v ni dalsi switch a pripady pro kazdy typ, zatim nepisu
 * treba ani nebude potreba
 */
      case I_TYPE:

        return EXIT_SUCCESS;
        break;

/*
 * Instrukce implenetujici vestavene funkce copy, lenght, find, sort
 * -----------------------------------------------------------------------------
 * copy(s : string; i : integer; n : integer) : string
 *   - vrati podretezec zadaneho retezce 's'
 *   - 'i' udava zacatek pozadovaneho podretezce (pocitano od 1)
 *   - 'n' urcuje delku podretezce
 *
 * length(s : string) : integer
 *   - vrati delku retezce zadaneho parametrem 's'
 *
 * find(s : string; search : string) : integer
 *   - vyhleda prvni vyskyt zadaneho podretezce 'search' v retezci 's'
 *     a vrati jeho pozici (pocitano od 1)
 *   - pokud neni podretezec nalezen, vraci 0
 *   - implementovano pomoci Boyer-Moorova algoritmu
 *
 * sort(s : string) : string
 *   - seradi znaky v retezci 's' tak, aby znak si nizsi ordinarni
 *     hodnotou vzdy predchazel znaku s vyssi ordinarni hodnotou
 *   - vraci retezec obsahujici serazene znaky
 *   - implementovano pomoci algoritmu shell sort
 */

/*
 * COPY
 */
      case I_COPY:
        if (!((operand1->type == T_STRING) && (operand2->type == T_REAL))) return EXIT_TYPE_ERROR;
        else if (!(operand1->data)) return EXIT_NOT_INIT_ERROR;
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
          return EXIT_SUCCESS;
        }
        // vymyslet zpusob, jak v standardne 3 adresnem kodu predat krom instrukce
        // jeste 3 dalsi informace
        // co takle si cisla 'i' a 'n' dat dohromady a oddelit teckou - udelat z
        // nich vlastne real, ktery si pak rozkouskuju na dva integery a zahodim
        // tecku? :) Momentalne me nanapada nic rozumnejsiho..
        break;

/*
 * LENGHT
 */
      case I_LENGHT:
        if (!(operand2->type == T_STRING)) return EXIT_TYPE_ERROR;
        else if(!(operand1->data)) return EXIT_NOT_INIT_ERROR;
        else
        {
          result->type = T_INTEGER;
          result->data = strlen(operand2->data);
          return EXIT_SUCCESS;
        }
        break;

/*
 * FIND
 */
      case I_FIND:
        if (!((operand1->type == T_STRING) && (operand2->type == T_STRING))) return EXIT_TYPE_ERROR;
        else if ((!(operand1->data)) || (!(operand2->data))) return EXIT_NOT_INIT_ERROR;
        else if (strlen(operand2->data) > strlen(operand1->data)) return EXIT_RUNTIME_ERROR;
        else
        {
          result->type = T_INTEGER;
          result->data = findSubtring();
          return EXIT_SUCCESS;
        }
        break;

/*
 * SORT
 */
      case I_SORT:
        // string je vlastne pole znaku, ze?
        // takze staci predat ukazatel na string a spocitat jeho delku, kvuli promenne 'n' v shellSortu

        if (!(operand2->type == T_STRING) return EXIT_TYPE_ERROR;
        else if (!(operand1->data)) return EXIT_NOT_INIT_ERROR;
        else
        {
          if (strlen(operand2->data) == 1) // retezec ma delku 1, neni co radit
          {
            result->data = operand2->data;
            return EXIT_SUCCESS;
          }
          else
          {
            int n = strlen(operand2->data);

            result->type = T_STRING;
            result->data = (shellSort(*operand2, n));
            return EXIT_SUCCESS;
          }
        }
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
  if((operand1->type == T_INTEGER) && (operand2->type == T_INTEGER)) return TRUE;
  else if((operand1->type == T_INTEGER) && (operand2->type == T_REAL)) return TRUE;
  else if((operand1->type == T_REAL) && (operand2->type == T_INTEGER)) return TRUE;
  else if((operand1->type == T_REAL) && (operand2->type == T_REAL)) return TRUE;
  else return FALSE;
}

/*
 * Funkce vykonava volani WRITE - zapisuje na stdout
 */
int iWrite(tVarValue *source)
{
  switch(source->type)
  {
    case T_INTEGER:
      printf("%d\n", source->integer);
      break;
    case T_REAL:
      printf("%f\n", source->real);
      break;
    case T_STRING:
      printf("%s\n", source->string);
      break;
    case T_BOOLEAN:
    default:
      return EXIT_TYPE_ERROR;
    break;
  }
}

/*
 * Funkce vykonava volani READ - cte ze stdin
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
 * - podivat se na funkci getSubString (Boyer-Mooreuv algoritmus), bylo by vhodne
 *   tu funkci konecne napsat
 */
