/*
 * =====================================================================
 *          Verze:  1.EXIT_SUCCESS
 *      Vytvoreno:  11/12/2EXIT_SUCCESS14 EXIT_SUCCESS4:21:52 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "parser.h"

void generateInstruction(int instType, key type, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist)
// vlozi novou instrukci do seznamu instrukci

/*
 * KONVENCE INSTRUKCI
 * -----------------------------------------------------------------------------
 * - plati pro aritmeticke instrukce, instrukce porovnani a instrukce vestavenych funkci
 * - specialni instrukce maji specialni konvece uvedene vzdy pred instrukci samotnou
 *
 * - instrukce se dvema operandy:
 *   JMENO_INSTRUCE, TYP, OPERAND1, OPERAND2, VYSLEDEK
 *
 * - instrukce s jednim operandem:
 *   JMENO_INSTRUKCE, TYP, OPERAND1, NULL, VYSLEDEK
 */

{
   tInstr I;
   I.instType = instType;
   I.type = type;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(ilist, I);
   return;
}

int prTable [14][14] =
 //radek - vstupni token,
 //sloupec - znak na zasobniku
{  //        +     -    *     div    >     <    >=    <=     =    <>    (     )    i     $
/* + */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* - */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* * */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* div */{Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* > */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* < */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* >= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* <= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* = */  {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
/* <> */ {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
/* ( */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Straight,Left,Err,},
/* ) */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
/* i */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
/* $ */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Err,Left,EE,},

};

// vraci index do tabulky
int retIndex (lexType typ)
{
  int j;
  switch (typ)
  {
    case (l_add): j=0; break;
    case (l_sub): j=1; break;
    case (l_mul): j=2; break;
    case (l_div): j=3; break;
    case (l_greater): j=4; break;
    case (l_less): j=5; break;
    case (l_gequal): j=6; break;
    case (l_lequal): j=7; break;
    case (l_equal): j=8; break;
    case (l_not): j=9; break;
    case (l_lparenth): j=10; break;
    case (l_rparenth): j=11; break;
    case (l_int):
    case (l_bool):
    case (l_real):
    case (l_str):
    case (l_id): j=12; break;
    case (l_eof): j=13; break;
    default: j=13;
  }

 return j;
}

int convertType(sData * item1, sData * item2)
{
  printDebug("Konvertuji na real ");
  void * data;
  if (item1->typeVal != l_real)
  {
    printDebug("item1\n");
    data = item1->data;
    if ((item1->data = malloc(sizeof(double))) == NULL)
      return EXIT_INTERNAL_ERROR;
    *((double *)item1->data) = (double) *(int *)data;
    item1->typeVal = l_real;
    item1->typeKey = k_real;
  }
  if (item2->typeVal != l_real)
  {
    printDebug("item2\n");
    data = item2->data;
    if ((item2->data = malloc(sizeof(double))) == NULL)
      return EXIT_INTERNAL_ERROR;
    *((double *)item2->data) = (double) *(int *)data;
    item2->typeVal = l_real;
    item2->typeKey = k_real;
  }
  return EXIT_SUCCESS;
}

void * createSemiResult(lexType typeA, lexType typeB, key * retType, lexType * retTypeL, lexType op)
{
  void * data;
  if (op == l_div)
  {
    if ((typeA != l_int || typeA != l_real) && (typeB != l_int || typeB != l_real))
      return NULL;
    if ((data = malloc(sizeof(double))) == NULL)
      return NULL;
    *retType = k_real;
    *retTypeL = l_real;
    return data;
  }
  if ((op == l_not || op == l_equal || op == l_less || op == l_lequal || op == l_greater || op == l_gequal) && (typeA == typeB))
  {
    if ((data = malloc(sizeof(bool))) == NULL)
      return NULL;
    *retType = k_bool;
    *retTypeL = l_bool;
    return data;
  }

  if (typeA == l_real || typeB == l_real)
  {
    if ((data = malloc(sizeof(double))) == NULL)
      return NULL;
    *retType = k_real;
    *retTypeL = l_real;
    return data;
  }

  if (typeA == l_int && typeB == l_int)
  {
    if ((data = malloc(sizeof(int))) == NULL)
      return NULL;
    *retType = k_int;
    *retTypeL = l_int;
    return data;
  }

  if (typeA == l_bool && typeB == l_bool)
  {
    if ((data = malloc(sizeof(bool))) == NULL)
      return NULL;
    *retType = k_bool;
    *retTypeL = l_bool;
    return data;
  } else
    return NULL;
  return data;
}

/*   Vyhodnoceni tokenu
 * ---------------------------------------------------------------------
 * - vyhodnoti token a ulozi jeho data do sData itemu
 */
int defineToken(token * lex, sData * item, btree * table)
{
  printDebug("Identifikuji token");
  struct node * nd;
  switch (lex->type)
  {
    case l_int:
    case l_real:
    case l_str:
      printDebug("Token je ordinalni hodnota\n");
      item->typeVal = item ->type = lex->type;
      item->data = lex->data;
      lex->data = NULL;
      item->typeKey = (lex->type ==  l_int) ? k_int : (lex->type == l_str) ? k_string : (lex->type == l_real) ? k_real : k_else;
      break;
    case l_id:
      if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) || nd->defined == false)
        return EXIT_NOT_DEFINED_ERROR;
      printDebug("Token je id typu %d\n", nd->type);
      switch(nd->type)
      {
        case k_int: item->type = item->typeVal = l_int; break;
        case k_string: item->type = item->typeVal = l_str; break;
        case k_real: item->type = item->typeVal = l_real; break;
        case k_bool: item->type = item->typeVal = l_bool; break;
        default: return EXIT_INTERNAL_ERROR;
      }
      item->data = nd->data;
      item->typeKey = nd->type;
      break;
    case l_key:
      if (*(key *)lex->data == k_true || *(key *)lex->data == k_false)
      {
      printDebug("Token je bool\n");
        item->type = item->typeVal = l_bool;
        if ((item->data = malloc(sizeof(bool))) == NULL)
          return EXIT_INTERNAL_ERROR;
        *(bool *)item->data = (*(key *)lex->data == k_true);
        lex->data = NULL;
        break;
      }
      item->type = lex->type;
      item->typeKey = k_bool;
      printDebug("Token je key\n");
      break;
    case l_lparenth:
      item->type = lex->type;
      printDebug("Token je (\n");
      break;
    default:
      item->type = lex->type;
      printDebug("Token je neznamy\n");
      break;
  }
  return EXIT_SUCCESS;
}

/*   Vyhodnoceni vyrazu
 * ---------------------------------------------------------------------
 * - bude namapovane na Juruv kod
 */
int evalExpression(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex, void * retVal, key retType)
{
  printDebug("Spoustim analyzu vyrazu\n");
  int result = EXIT_SUCCESS;
  Stack s;
  sInit (&s);
  sData itemAct, itemTop, itemC, itemD;
  if (DataInit(&itemAct) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemTop) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemC) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemD) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  itemAct.type=l_eof;
  sPush (&s,itemAct);
  printDebug("Inicializuji zasobnik a vkladam zarazku\n");
  printDebug("Zpracovavam prvni lexem\n");
  printDebug("Zkontroluji, jestli mam dva lexemy nebo jeden\n");
  if ((result = defineToken(lex,&itemAct,table)) != EXIT_SUCCESS) return result;
  itemTop=sTop(&s);
  do
  {
    printDebug("itemTop.type = %d; itemAct.type = %d\n", itemTop.type, itemAct.type);
    switch (prTable[retIndex(itemTop.type)][retIndex(itemAct.type)])
    {
      case Err:
        return EXIT_SYNTAX_ERROR;
      case Straight:
        printDebug("Pouziva pravidlo =\n");
        sPush(&s,itemAct);
        itemTop = sTop(&s);
        if (nextLex != NULL)
        {
          if ((result = defineToken(nextLex,&itemAct,table)) != EXIT_SUCCESS) return result;
          tokenFree(nextLex);
          nextLex = NULL;
        } else
        {
          if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
          if ((result = defineToken(lex,&itemAct,table)) != EXIT_SUCCESS) return result;
        }
        break;
      case Left:
        printDebug("Pouzivam leve pravidlo\n");
        itemTop = sTop(&s);
        itemC.type  = l_left;
        if (itemTop.type == l_E)
        {
          itemTop = sPop(&s);
          sPush(&s, itemC);
          sPush(&s, itemTop);
          printDebug("Zamena \"E\" za \"<E\"\n");
        } else
        {
          sPush(&s, itemC);
          printDebug("Pridavam \"<\"\n");
        }
        sPush(&s, itemAct);
        itemTop=sTop(&s);
        if (nextLex != NULL)
        {
          if ((result = defineToken(nextLex,&itemAct,table)) != EXIT_SUCCESS) return result;
          tokenFree(nextLex);
          nextLex = NULL;
        } else
        {
          if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
          if ((result = defineToken(lex,&itemAct,table)) != EXIT_SUCCESS) return result;
        }
        break;
      case Right:
        printDebug("Pouzivam prave pravidlo\n");
        itemTop = sTop(&s);
        switch (itemTop.type)
        {
          case l_id:
          case l_str:
          case l_bool:
          case l_int:
          case l_real:
            itemD = sPop(&s);
            itemC = sPop(&s);
            if (itemC.type != l_left) return EXIT_INTERNAL_ERROR;
            itemD.type = l_E;
            itemTop = sTop(&s);
            sPush(&s, itemD);
            printDebug("Redukuji \"<i\" na \"E\"\n");
            if (prTable[retIndex(itemTop.type)][retIndex(itemAct.type)]== EE)
            {
                printDebug("Je to prirazeni\n");
                printErr("%d k %d\n", retType, itemD.typeVal);
                if (((itemD.typeVal==l_int) && (retType== k_int)) || ((itemD.typeVal==l_real) && (retType== k_real)) ||
                    ((itemD.typeVal==l_str) && (retType== k_string)) || ((itemD.typeVal==l_bool) && (retType== k_bool)))
                  generateInstruction(I_ASSIGN, retType, itemD.data, NULL, retVal, ilist);
                else return EXIT_TYPE_ERROR;
            }
            break;
          case l_rparenth:
            printDebug("Na vrcholu je \")\"\n");
            itemD = sPop(&s);
            if (itemD.type != l_rparenth) return EXIT_INTERNAL_ERROR;
            itemC = sPop(&s);
            itemD = sPop(&s);
            if (itemD.type != l_lparenth) return EXIT_INTERNAL_ERROR;
            itemD = sPop(&s);
            if (itemD.type != l_left) return EXIT_INTERNAL_ERROR;
            itemTop = sTop(&s);
            sPush(&s, itemC);
            break;
          case l_E:
            printDebug("Na vrcholu je E, kontroluji operaci\n");
            itemTop = sPop(&s);
            itemD = sPop(&s);
            itemC = sPop(&s);
            sPop(&s);
            if (itemC.type != l_E) return EXIT_INTERNAL_ERROR;
            key semiresultType;
            lexType semiresultTypeL;
            void * semiresult = createSemiResult(itemC.typeVal, itemTop.typeVal, &semiresultType, &semiresultTypeL, itemD.type);
            if (semiresult == NULL) return EXIT_INTERNAL_ERROR;
            switch (itemD.type)
            {
              case l_add:
                printDebug("Scitani\n");
                if (!(((itemTop.typeVal==l_int) && (itemC.typeVal == l_int)) || ((itemTop.typeVal==l_real) && (itemC.typeVal== l_real)) ||
                    ((itemTop.typeVal==l_str) && (itemC.typeVal== l_str)) || ((itemTop.typeVal==l_int) && (itemC.typeVal == l_real)) ||
                    ((itemTop.typeVal==l_real) && (itemC.typeVal == l_int))))
                  return EXIT_TYPE_ERROR;
                if (itemTop.typeVal != itemC.typeVal)
                {
                  result = convertType(&itemTop, &itemC);
                  if (result != EXIT_SUCCESS) return result;
                }
                generateInstruction(I_ADD, semiresultType, itemC.data, itemTop.data, semiresult, ilist);
                break;
              case l_mul:
                printDebug("Nasobeni\n");
                if (!(((itemTop.typeVal==l_int) && (itemC.typeVal == l_int)) || ((itemTop.typeVal==l_real) && (itemC.typeVal== l_real)) ||
                      ((itemTop.typeVal==l_int) && (itemC.typeVal == l_real)) ||
                    ((itemTop.typeVal==l_real) && (itemC.typeVal == l_int))))
                  return EXIT_TYPE_ERROR;
                if (itemTop.typeVal != itemC.typeVal)
                {
                  result = convertType(&itemTop, &itemC);
                  if (result != EXIT_SUCCESS) return result;
                }
                generateInstruction(I_MUL, semiresultType, itemTop.data, itemC.data, semiresult, ilist);
                break;
              case l_div:
                printDebug("Deleni\n");
                if (!(((itemTop.typeVal==l_int) && (itemC.typeVal == l_int)) || ((itemTop.typeVal==l_real) && (itemC.typeVal== l_real)) ||
                      ((itemTop.typeVal==l_int) && (itemC.typeVal == l_real)) ||
                    ((itemTop.typeVal==l_real) && (itemC.typeVal == l_int))))
                  return EXIT_TYPE_ERROR;
                if (itemTop.typeVal != itemC.typeVal)
                {
                  result = convertType(&itemTop, &itemC);
                  if (result != EXIT_SUCCESS) return result;
                }
                generateInstruction(I_DIV, semiresultType, itemC.data, itemTop.data, semiresult, ilist);
                break;
              case l_sub:
                printDebug("Odcitani\n");
                if (!(((itemTop.typeVal==l_int) && (itemC.typeVal == l_int)) || ((itemTop.typeVal==l_real) && (itemC.typeVal== l_real)) ||
                      ((itemTop.typeVal==l_int) && (itemC.typeVal == l_real)) ||
                    ((itemTop.typeVal==l_real) && (itemC.typeVal == l_int))))
                  return EXIT_TYPE_ERROR;
                if (itemTop.typeVal != itemC.typeVal)
                {
                  result = convertType(&itemTop, &itemC);
                  if (result != EXIT_SUCCESS) return result;
                }
                generateInstruction(I_SUB, semiresultType, itemC.data, itemTop.data, semiresult, ilist);
                break;
              case l_less:
                printDebug("Mensi nez\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_LESS, itemTop.typeKey, itemTop.data, itemC.data, semiresult, ilist);
                break;
              case l_greater:
                printDebug("Vetsi nez\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_GREATER, itemTop.typeKey, itemTop.data, itemC.data, semiresult, ilist);
                break;
              case l_gequal:
                printDebug("Vetsi nebo rovno\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_GREATER_EQUAL, itemTop.typeKey, itemTop.data, itemC.data, semiresult, ilist);
                break;
              case l_lequal:
                printDebug("Mensi nebo rovno\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_LESS_EQUAL, itemTop.typeKey, itemTop.data, itemC.data, semiresult, ilist);
                break;
              case l_equal:
                printDebug("Rovna se\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_EQUAL, itemTop.typeKey, itemC.data, itemTop.data, semiresult, ilist);
                break;
              case l_not:
                printDebug("Nerovna se\n");
                if (itemTop.typeVal != itemC.typeVal)
                  return EXIT_TYPE_ERROR;
                generateInstruction(I_NOT_EQUAL, itemTop.typeKey, itemC.data, itemTop.data, semiresult, ilist);
                break;
              default:
                break;
            }
            generateInstruction(I_CLEAR, k_int, semiresult, NULL, NULL, ilist);
            itemC.data = semiresult;
            itemC.typeVal = semiresultTypeL;
            itemC.typeKey = semiresultType;
            itemTop=sTop(&s);
            sPush(&s,itemC);
            break;
          default:
            break;
        }
    }
  }while (prTable[retIndex(itemTop.type)][retIndex(itemAct.type)]!= EE);
  itemTop=sTop(&s);

  if (!(((itemTop.typeVal == l_int) && (retType == k_int)) ||
      ((itemTop.typeVal == l_str) && (retType == k_string)) ||
      ((itemTop.typeVal == l_real) && (retType == k_real)) ||
      ((itemTop.typeVal == l_bool) && (retType == k_bool))))
    return EXIT_TYPE_ERROR;
  generateInstruction(I_ASSIGN, retType, itemTop.data, NULL, retVal, ilist);
  printDebug("Vracim se z analyzy\n");
  return EXIT_SUCCESS;
}


/*   Syntakticka analyza
 * =====================================================================
 * - "zhora dolu", vyuziva rekurzivni sestup
 */

/*   Blok deklarace promennych
 * ---------------------------------------------------------------------
 * - zpracovani bloku promenych
 * - pokud program nezacina klicovym slovem "var" je tato faze
 *   preskocena
 *
 *   <declareList> -> "var" <DeclareListContent>
 */
int declareList(struct input * in, btree * table, token * lex)
{
  printDebug("Deklarace promenych\n");
  if ((lex->type != l_key) || (*((key *)lex->data) != k_var))
    return EXIT_SUCCESS; // nezacinam "var" -> konec

  int result;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_id)
  {
    if (lex->type == l_key) { printDebug("Blok je prazdny\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  return declareListContent(in, table, lex);
}

/*   Rekurzivni zpracovani promennych v deklaracni casti
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <declareListContent> -> "id" ":" "type" ";" <declareListContent>
 *   <declareListContent> -> eps
 */
int declareListContent(struct input * in, btree * table, token * lex)
{
  printDebug("Vytvarim promennou\n");
  int result;

  if (lex->type != l_id)
  {
    if (lex->type == l_key) { printDebug("Konec bloku promennych\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  if ( __SymbolTableSearch(table->local, ((string *)lex->data)->str) != NULL) return EXIT_NOT_DEFINED_ERROR;

  // zalohujeme si identifikator, zatim nevime, jaky je to typ
  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  struct node * nd; // vytvorime zaznam v tabulce symbolu pokud uz neexistuje v lokalni tabulce
  if ((__SymbolTableSearch(table->local, tmp) != NULL) || ((nd = SymbolTableCreateNode(tmp, *((key *)lex->data), NULL)) == NULL))
  {
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }
  SymbolTableInsert(table, nd); // vlozime symbol do lokalni tabulky (doresit redeklaraci)

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_endl ) {; return EXIT_SYNTAX_ERROR; }

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return declareListContent(in, table, lex);
}

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsList> -> "id" ":" "type" ";" <paramsList>
 *   <paramsList> -> "id" ":" "type"
 */
int paramsList(struct input * in, token * lex, struct funcParam ** param)
{
  printDebug("Vytvarim promennou\n");
  int result;

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id)
  {
    if (lex->type == l_rparenth) { printDebug("Konec bloku parametru\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  // zalohujeme si identifikator, zatim nevime, jaky je to typ
  if ((*param = (struct funcParam *) malloc(sizeof(struct funcParam))) == NULL) return EXIT_INTERNAL_ERROR;
  strncpy((*param)->keyValue, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  (*param)->type = *(key *)lex->data;
  (*param)->next = NULL;
  printDebug("Vytvarim parametr funkce\n");

  // Nasleduje ";" nebo ")" podle toho se uvidi co dal
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  if (lex->type == l_rparenth) return EXIT_SUCCESS; // je to zavorka -> konec
  else if (lex->type != l_endl) return EXIT_SYNTAX_ERROR; // musi to byt strednik

  return paramsList(in, lex, &((*param)->next));
}

/*   Deklarace funkce
 * ---------------------------------------------------------------------
 * - zpracuje deklaraci funkce, ulozi jeji identifikator, a parametry
 *   <function> -> "function" "id" "(" <paramList> ")" ":" "typ" ";" <forward>
 *   <function> -> eps
 *   <forward> -> "forward" ";"
 *   <forward> -> <declareList> <body> ";" <function>
 */
int function(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s)
{
  printDebug("Funkce\n");
  int result = EXIT_SUCCESS;
  struct node * nd = NULL, * new; // vytvorime zaznam v tabulce symbolu

  // Prvni token musi byt klicove slovo "function"
  if ((lex->type != l_key)  || (*(key *)lex->data != k_function ))
    { printDebug("Konec bloku deklarace funkci\n"); return EXIT_SUCCESS; }

  // Dalsi token musi byt ID, ktere si musim zatim zalohovat
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;
  if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) != NULL) &&
      ((nd->type != k_function) || ((funcData *)nd->funcData)->defined))
    // pokud uz je identifikator v tabulce a pritom to neni funkce (ktera jeste nebyla definovana) -> chyba
    return EXIT_SYNTAX_ERROR;

  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt oteviraci zavorka
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth ) { return EXIT_SYNTAX_ERROR; }

  //zpracuju parametry
  struct funcParam * firstParam = NULL;
  if ((result = paramsList(in, lex, &firstParam)) != EXIT_SUCCESS) return result;
  printDebug("Parametry funkce zpracovany\n");

  struct funcParam * paramDecl, * paramDef;
  // pokud byla uz dopredu deklarovana a ted je jeji definice, pak musi sedet parametry
  if (nd != NULL)
  {
    printDebug("Nalezen drive definovany prototyp\n");
    paramDecl = firstParam;
    paramDef = ((funcData *)nd->funcData)->param;
    while (paramDecl != NULL || paramDef != NULL) // zkontroluju obsah jednotlivych parametru
    {
      if ((paramDecl->type != paramDef->type) || (strcmp(paramDecl->keyValue, paramDef->keyValue) != EXIT_SUCCESS))
        break;
      paramDecl = paramDecl->next;
      paramDef = paramDef->next;
    }
    if (paramDecl != NULL && paramDef != NULL) result = EXIT_TYPE_ERROR; // nemaji stejny pocet parametru

    if (result != EXIT_SUCCESS) // prototyp neodpovida nove deklaraci -> smazat parametry a konec
    {
      FunctionParamsListDispose(firstParam);
      return result;
    }
    printDebug("Parametry sedi s prototypem\n");
  }

  // Dalsi token musi byt dvojtecka
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon ) { return EXIT_SYNTAX_ERROR; }

  // nasleduje token definujici typ navratove hodnoty
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  if ((nd != NULL) && (*(key *)lex->data != ((funcData *)nd->funcData)->retType)) // navratova hodnota prototypu neodpovida deklaraci -> smazat parametry a konec
  {
    printDebug("Jiny navratovy typ\n");
    FunctionParamsListDispose(firstParam);
    return EXIT_TYPE_ERROR;
  }
  key type = *(key *)lex->data; // ulozit pro pozdejsi pouziti

  // Nasleduje ";"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;

  // "forward" || "var" || "begin"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key) return EXIT_SYNTAX_ERROR;

  bool defined = false;
  if (*(key *)lex->data != k_forward) defined = true;

  // jeden prototyp uz je a ja vytvarim druhy
  if (nd != NULL && defined == false)
  {
    printDebug("Nova deklarace prototypu funkce\n");
    FunctionParamsListDispose(firstParam);
    return EXIT_SEMANTIC_ERROR;
  }

  if ((nd = SymbolTableCreateFunctionNode(tmp, type, firstParam, defined)) == NULL)
  { // pokud selze vytvoreni uzlu funkce
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }
  SymbolTableInsert(table, nd); // vlozime symbol
  ((funcData *)nd->data)->retType = type;

  if (*(key *)lex->data == k_forward) // token je "forward", potrebuju ";"
  {
    printDebug("Dopredna deklarace\n");
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  else
  {
    printDebug("Definice funkce\n");
    // inicializuju ilist a TS funkce
    listInit(&(((funcData *)nd->funcData)->ilist));
    SymbolTableInitLocal(((funcData *)nd->funcData)->table, table);

    printDebug("Plnim tabulku symbolu parametry\n");
    while (firstParam != NULL)
    {
      if ((new = SymbolTableCreateNode(firstParam->keyValue, firstParam->type, NULL)) == NULL)
      {
        __SymbolTableDispose(&new);
        return EXIT_INTERNAL_ERROR;
      }
      SymbolTableInsert(((funcData *)nd->funcData)->table, new); // vlozime nove vytvoreny symbol bez hodnoty
      new->defined = true;
      firstParam = firstParam->next;
    }
    printDebug("Deklarace vlastnich promennych a telo funkce\n");
    // pokud selze definice funkce, uvolnuju jeji tabulku symbolu a koncim
    if (((result = declareList(in, ((funcData *)nd->funcData)->table, lex)) != EXIT_SUCCESS) ||
        ((result = body(in, ((funcData *)nd->funcData)->table, &(((funcData *)nd->funcData)->ilist), lex, s, nd)) != EXIT_SUCCESS))
      return result;
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return function(in, table, ilist, lex, s);
}

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsCall> -> "id" "," <paramsCall>
 *   <paramsCall> -> "id"
 */
int paramsCall(struct input * in, btree * table, tListOfInstr * ilist, token * lex, funcData * function, struct funcParam * param)
{
  printDebug("Zpracovavam parametry volani\n");
  int result;
  bool isOrd = false;

  if (param == NULL) { printDebug("Vsechny argumenty nacteny\n"); return EXIT_SUCCESS; } // mam spravny pocet parametru -> konec

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  struct node * nd, * paramNode;
  void * data;
  switch (lex->type)
  {
    case l_id: // prisel identifikator, musim ho najit (a musi byt stejneho typu) v tabulce a pouzit jeho hodnotu
      if ((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) return EXIT_NOT_DEFINED_ERROR;
      if (nd->type != param->type) return EXIT_TYPE_ERROR;

      data = nd->data;
      break;
    case l_int:
    case l_real:
    case l_str:
    case l_key: //pokud zadana hondota neodpovida typu parametru.. hnus fialovej, kdo to takhle nadeklaroval? ja...
      if (lex->type == l_key && *(key *)lex->data != k_true && *(key *)lex->type != k_false) return EXIT_SYNTAX_ERROR;
      if ((lex->type == l_int && param->type != k_int) ||
         ( lex->type == l_real && param->type != k_real) ||
         ( lex->type == l_str && param->type != k_string) ||
         ( lex->type == l_key && param->type != k_bool))
        return EXIT_TYPE_ERROR;
      isOrd = true;
      data = lex->data; // ulozim si data a hacknu lex->type, aby mi nedelal u stringu free
      lex->data = NULL;
      lex->type = l_int;
      break;
    default:
      return EXIT_SYNTAX_ERROR;
  }
  printDebug("Parametr volani nacten\n");

  if ((paramNode = SymbolTableSearch(function->table, param->keyValue)) == NULL)
  {
    printDebug("Parametr funkce nenalezen v lokalni tabulce, nemas delat hacky Coufale..\n");
    return EXIT_INTERNAL_ERROR;
  }
  if (lex->type == l_id) printErr("%d", *(int *)lex->data);
  generateInstruction(I_ASSIGN,param->type, data, NULL, paramNode->data, ilist);
  paramNode->defined = true;
  if (isOrd) generateInstruction(I_CLEAR, param->type, data, NULL, NULL, ilist);

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep && param->next != NULL) return EXIT_SYNTAX_ERROR; // neni carka, ale nemam vsechny parametry -> chyba

  return paramsCall(in, table, ilist,  lex, function, param->next);
}

/*   Volani funkce
 * ---------------------------------------------------------------------
 *   <callFunction> -> id "(" <paramsCall> ")"
 */
int callFunction(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex, struct node * retNode, stack * s)
{
  int result = EXIT_SUCCESS;
  printDebug("Volani funkce\n");
  struct node * nd;
  if (lex->type == l_id) // uzivatelem definovana funkce
  {
    // musim overit, ze identifikator existuje a je definovana funkce
    if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) || (((funcData *)nd->funcData)->defined) == false)
      return EXIT_NOT_DEFINED_ERROR;
    if (nd->type != k_function)
      return EXIT_TYPE_ERROR;
    generateInstruction(I_TABLE_BACKUP, k_var, ((funcData *)nd->funcData)->table, NULL, s, ilist);
    // nactu oteviraci zavorku
    if (nextLex != NULL)
    {
      if (nextLex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
    } else
    {
      if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
      if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
    }
    // zpracovani parametru
    if ((result = paramsCall(in, table, ilist, lex, (funcData *)nd->funcData, ((funcData *)nd->funcData)->param)) != EXIT_SUCCESS)
      return result;
    // nacteme pravou zavorku a konec
    if (lex->type != l_rparenth) return EXIT_SYNTAX_ERROR;
    generateInstruction(I_CALL_FUNCTION, k_function, &((funcData *)nd->funcData)->ilist, NULL, NULL, ilist); // vytvorime volani funkce (ma uz nactene parametry v tabulce)

    generateInstruction(I_ASSIGN, retNode->type, nd->data, NULL, retNode->data, ilist); // prirazeni navratove hodnoty funkce do lhodonty
    generateInstruction(I_TABLE_RESTORE, k_var, ((funcData *)nd->funcData)->table, nd->keyValue, s, ilist);
  }
  else if (lex->type == l_key) // vestavne funkce
  {
    switch (*(key *)lex->data)
    {
      case k_sort: result =  embededFuncSort(in, table, ilist, lex, retNode); break;
      case k_find: result = embededFuncFind(in, table, ilist, lex, retNode); break;
      case k_length: result = embededFuncLength(in, table, ilist, lex, retNode); break;
      case k_copy: result = embededFuncCopy(in, table, ilist, lex, retNode); break;
      default:
        return EXIT_SYNTAX_ERROR; break;
    }
  }
  else return EXIT_SYNTAX_ERROR;
  if (result != EXIT_SUCCESS || ((result = fillToken(in, lex)) != EXIT_SUCCESS)) return result;
  return result;
}

/*   Zpracovani prikazu programu
 * ---------------------------------------------------------------------
 * - zpracuju zde vsechna prirazeni, cykly, vestavne funkce, volani
 *   funkci, vetveni
 *
 *   <state> -> id := <evalExpression>
 *   <state> -> id := <callFunction>
 *   <state> -> if <evalExpression> then <body> ; else <body>
 *   <state> -> while <evalExpression> do <body>
 *   <state> -> write ( <type> )
 *   <state> -> readln ( <type> )
 */
int state(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s, struct node * inFunc)
{
  printDebug("Zpracovavam prikaz\n");
  switch (lex->type)
  {
    // <state> -> id := <evalExpression>
    // <state> -> id := <call>
    case (l_id): return embededAssign(in, table, ilist, lex, s, inFunc);
    // <state> -> if <evalExpression> then <body> ; else <body>
    // <state> -> while <evalExpression> do <body>
    // <state> -> write ( <type> )
    // <state> -> readln ( <type> )
    case (l_key):
      switch (*(key *)lex->data)
      {
        case k_if: return embededIf(in, table, ilist, lex, s, inFunc);
        case k_while: return embededWhile(in, table, ilist, lex, s, inFunc);
        case k_write: return embededFuncWrite(in, table, ilist, lex);
        case k_readln: return embededFuncReadln(in, table, ilist, lex);
        default: printDebug("%d", *(key *)lex->data); return  EXIT_SYNTAX_ERROR;
      }
      break;
    default: return EXIT_SYNTAX_ERROR;
  }

  return EXIT_SUCCESS;
}

/*   Blok kodu
 * ---------------------------------------------------------------------
 * - zpracovavam jednotlive prikazy v bloku kodu programu
 * - rekurzivne zpracuje prikaz, ";" a znova dokud po prikazu nasleduje
 *   strednik
 *
 *   <statements> -> <state> ; <statements>
 *   <statements> -> <state>
 */
int statements (struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s, struct node * inFunc)
{
  int result;
  printDebug("Zacatek noveho prikazu\n");
  // nebyl to end, to jsme kontrolovali jeste v body
  // tzn ze tam bude nejaky prikaz
  // zavolame state
  result = state(in, table, ilist, lex, s, inFunc);
  if (result != EXIT_SUCCESS) return result;

  // pokud dalsi token je strednik tzn ze bude
  // nasledovat dalsi prikaz
  if (lex->type == l_endl)
  {
    printDebug("Nasleduje prikaz\n");
    if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
    return statements(in, table, ilist, lex, s, inFunc);
  }
  // pokud uz strednik nebude
  // za poslednim prikazem nema byt strednik
  // tzn ze se vratime do body
  else return result;
}

/*  Telo programu
 * ---------------------------------------------------------------------
 * - za tokenem "begin" nasleduje telo programu -> voleme statements a
 *   pak zkontrolujeme "end"
 * <body> -> <statements> end
 * <body> -> end
 */
int body(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s, struct node * inFunc)
{
  printDebug("Zacatek bloku kodu\n");
  int result = EXIT_SUCCESS;
  if ((result = fillToken(in, lex)) != EXIT_SUCCESS)
    return result;
  // naplneny token, pokud nic neselze
  // zkontroluje prazdne telo programu, kdy po "begin" nasleduje hned "end"
  if (lex->type == l_key && *(key *)lex->data == k_end )
  {
    printDebug("Prazdny blok kodu\n"); return result;
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  // a projde telo programu -> pokud cokoliv z toho selze, vraci error
  if ((result = statements(in, table, ilist, lex, s, inFunc)) != EXIT_SUCCESS)
    return result;

  printDebug("Konec bloku kodu\n");
  if ((lex->type != l_key) || (*(key *)lex->data != k_end))
  { printDebug("Chybi end, mam typ: %d", *(key *)lex->data); return EXIT_SYNTAX_ERROR;} // za telem programu musi byt "end",

  return result;
}

/*   Parser
 * ---------------------------------------------------------------------
 * - volan hlavnim programem
 * - potrebuje znat zdrojovy soubor, tabulku symbolu a list instrukci
 * - program zacina deklaracnim liste promennych, deklaraci funkce a pak
 *   nasleduje samotne telo programu
 *
 * <program> -> <decList> <funciton> <body> <EOF>
 */
int parser(struct input * in, btree * table, tListOfInstr * ilist, stack * s)
{
  printDebug("Spoustim Parser\n");
  token lex;
  int result = EXIT_SUCCESS;

  if ((tokenInit(&lex) == EXIT_SUCCESS) &&
      ((result = fillToken(in, &lex)) == EXIT_SUCCESS))
  {
    switch(lex.type)
    {
      case l_key: // pokud je to klicove slovo, var,function,begin (vyuzivam toho, ze C vyhodnocuje disjunkci zleva)
        if ((*(key *)lex.data != k_var) && (*(key *)lex.data != k_function) && (*(key *)lex.data != k_begin))
          { result = EXIT_SYNTAX_ERROR; break; }

        if (((result = declareList(in, table, &lex)) != EXIT_SUCCESS) || //Kontrola deklarace promennych (nemusi byt nic deklarovano)
            ((result = function(in, table, ilist, &lex, s)) != EXIT_SUCCESS) || // Kontrola funkci deklarace (nemusi byt nic deklarovano)
            ((result = body(in, table, ilist, &lex, s, NULL)) != EXIT_SUCCESS)) { break; } // Hlavni program begin

        if ((lex.type == l_enddot) ||
           ((result = fillToken(in, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_eof)) { result = EXIT_SYNTAX_ERROR; break; } // na konci programu musi byt . a pak EOF

          //generateInstruction(I_END, NULL, NULL, NULL); // instrukce?? + ilist
        break;
      case l_eof: // prazdny soubor
        result = EXIT_SUCCESS;
        break;
      default: // cokoliv jineho je chyba
        result = EXIT_SYNTAX_ERROR;
        break;
    }
  }

  if (result == EXIT_SEMANTIC_ERROR) printErr("SEMANTIC ERROR on line %d (excluding blank lines): Variable type conflict, use of an undeclared variable or conflict redeclarations.\n", in->line);
  if (result == EXIT_NOT_DEFINED_ERROR) printErr("NOT DEFINED (REDEFINED) ERROR on line %d (excluding blank lines): You are using undefined variable or trying to redefine something.\n", in->line);
  if (result == EXIT_TYPE_ERROR) printErr("TYPE ERROR on line %d (excluding blank lines): Some variable is of a wrong type. Or maybe you have used wrong amount or type of parameter of function.\n", in->line);
  if (result == EXIT_SYNTAX_ERROR) printErr("SYNTAX ERROR on line %d (excluding blank lines): Check your program once more, please.\n", in->line);
  if (result == EXIT_INTERNAL_ERROR) printErr("INTERNAL ERROR on line %d (excluding blank lines): Something really bad happend. This is not your fault.\n", in->line);
  tokenFree(&lex);
  return result;
}
