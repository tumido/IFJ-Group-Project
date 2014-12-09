/*
 * =====================================================================
 *          Verze:  1.EXIT_SUCCESS
 *      Vytvoreno:  11/12/2EXIT_SUCCESS14 EXIT_SUCCESS4:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
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
 * - specialni instrukce maji specianli konvece uvedene vzdy pred instrukci samotnou
 *   - na prani muzu doplnit i sem, pro lepsi prehlednost
 *
 * - instrukce se dvema operandy:
 * JMENO_INSTRUCE, OPERAND1, OPERAND2, VYSLEDEK
 *
 * - instrukce s jednim operandem:
 * JMENO_INSTRUKCE, OPERAND1, NULL, VYSLEDEK
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
/* $ */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Err,Left,Err,},

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

/*   Syntakticka analyza
 * =====================================================================
 * - "zhora dolu", vyuziva rekurzivni sestup
 */

/*   Vyhodnoceni vyrazu
 * ---------------------------------------------------------------------
 * - bude namapovane na Juruv kod
 */
int evalExpression(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex, struct node * retNode)
{
  printDebug("Vyhodnoceni vyrazu << NAMAPOVAT NA JURUV KOD\n");
  int result;
  Stack s;
  sInit (&s);
    token NLex;
    if (tokenInit(&NLex) != EXIT_SUCCESS) return result ;

  sData itemAct;
  sData itemTop;
  sData itemC;
  sData itemD;
  if (DataInit(&itemAct) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemTop) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemC) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemD) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  itemAct.TypTok=l_eof;
  sPush (&s,itemAct);
  printDebug("Inicializuji zasobnik a vlozim zarazku\n");
  itemTop=sTop(&s);
  struct node *nd;
  printDebug("Kontrola prvniho lexemu\n");
  if (nextLex==NULL)
   {
    NLex.data=NULL;
   }
  else
    {NLex.data=nextLex->data; NLex.type=nextLex->type;}

  if (lex->type==l_id)
  {
    printDebug("Prvni lex je ID, jdu hledat ve stromu\n");
    if ( (nd=SymbolTableSearch (table,((string *) lex->type)->str))==NULL)
    return EXIT_SEMANTIC_ERROR;
    // ulozim si ze stromu hodnoty
    //itemAct.lexdata.data = nd->data; // ulozime si hodnotu
    //itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
   itemAct.TypTok = l_id;
  }
   //printDebug("tady jeste jsu\n");
   // jestli je token int,real,str ulozim si hodnotu a typ
  else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
  {
    printDebug("Prvni lex je int,real,str, bool\n");
    //itemAct.lexdata.data = nextLex->data;
    //itemAct.lexdata.type = nextLex->type;
    itemAct.TypTok = l_id;
  }
  else
  {// pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
    printDebug("Prvni lex neni promenna ani cislo\n");
    itemAct.TypTok=lex->type;
  }
      int x=2;
      int f=prTable[retIndex(itemTop.TypTok)][retIndex(itemAct.TypTok)];
      printf ("%d %d\n",itemTop.TypTok,itemAct.TypTok);
      printf ("%d %d\n",retIndex(itemTop.TypTok),retIndex(itemAct.TypTok));
      printf ("%d\n",f);
  do
  {
    switch (prTable[retIndex(itemTop.TypTok)][retIndex(itemAct.TypTok)])
     case Left:
       itemTop=sTop(&s);  // co je nahore?
       itemC.TypTok=l_left; // nastavim si <
       if (itemTop.TypTok==l_E)  // jestli je nahore E
       { // zamenime E za <E
        itemTop=sPop(&s);
        sPush (&s,itemC);
        sPush (&s,itemTop);
       }
       else // na vrcholu neni E, dame tam <
       {
        sPush (&s,itemC);
       }
       // nakonec vlozime token
       sPush (&s,itemAct);
       if (NLex.data==NULL)
        {if ((result = fillToken(in,lex)) != EXIT_SUCCESS) return result;}
       else{lex->data=NLex.data;lex->type=NLex.type; NLex.data=NULL;}

       itemTop=sTop(&s);
       if (lex->type==l_id)
       {
        printDebug("dalsi lex je ID, jdu hledat ve stromu\n");
        if ( (nd=SymbolTableSearch (table,((string *) lex->type)->str))==NULL)
        return EXIT_SEMANTIC_ERROR;
        // ulozim si ze stromu hodnoty
        //itemAct.lexdata.data = nd->data; // ulozime si hodnotu
        //itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
        itemAct.TypTok = l_id;
        }
        //printDebug("tady jeste jsu\n");
        // jestli je token int,real,str ulozim si hodnotu a typ
       else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
       {
        printDebug("dalsi lex je int,real,str, bool\n");
        //itemAct.lexdata.data = nextLex->data;
        //itemAct.lexdata.type = nextLex->type;
        itemAct.TypTok = l_id;
        }
       else
       {// pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
        printDebug("dalsi lex neni promenna ani cislo\n");
        itemAct.TypTok=lex->type;
       }
      break;
    x--;
  }while (itemTop.TypTok != l_eof && itemAct.TypTok!=l_eof);


  return EXIT_INTERNAL_ERROR;
}

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

  if ( SymbolTableSearch(table, ((string *)lex->data)->str) != NULL) return EXIT_SYNTAX_ERROR;

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
int function(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Funkce\n");
  int result = EXIT_SUCCESS;
  struct node * nd = NULL; // vytvorime zaznam v tabulce symbolu

  // Prvni token musi byt klicove slovo "function"
  if ((lex->type != l_key)  || (*(key *)lex->data != k_function ))
    { printDebug("Konec bloku deklarace funkci\n"); return EXIT_SUCCESS; }

  // Dalsi token musi byt ID, ktere si musim zatim zalohovat
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;
  if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) != NULL) &&
      ((nd->type != k_function) || ((funcData *)nd->data)->defined))
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
    paramDef = ((funcData *)nd->data)->param;
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

  if ((nd != NULL) && (*(key *)lex->data != ((funcData *)nd->data)->retType)) // navratova hodnota prototypu neodpovida deklaraci -> smazat parametry a konec
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
  __SymbolTableInsert(&table->global, nd); // vlozime symbol

  if (*(key *)lex->data == k_forward) // token je "forward", potrebuju ";"
  {
    printDebug("Dopredna deklarace\n");
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
    if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;
  }
  else
  {
    printDebug("Definice funkce\n");
    // inicializuju ilist a TS funkce
    listInit(&(((funcData *)nd->data)->ilist));
    SymbolTableInitLocal(((funcData *)nd->data)->table, table);
    // pokud selze definice funkce, uvolnuju jeji tabulku symbolu a koncim
    if (((result = declareList(in, ((funcData *)nd->data)->table, lex)) != EXIT_SUCCESS) ||
        ((result = body(in, table, &(((funcData *)nd->data)->ilist), lex)) != EXIT_SUCCESS))
      return result;
  }
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return function(in, table, ilist, lex);
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
int paramsCall(struct input * in, btree * table, token * lex, funcData * function, struct funcParam * param)
{
  printDebug("Zpracovavam parametry volani\n");
  int result;

  if (param == NULL) { printDebug("Vsechny argumenty nacteny\n"); return EXIT_SUCCESS; } // mam spravny pocet parametru -> konec

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  struct node * nd, * new;
  void * data = lex->data;
  switch (lex->type)
  {
    case l_id: // prisel identifikator, musim ho najit (a musi byt stejneho typu) v tabulce a pouzit jeho hodnotu
      if ((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) return EXIT_NOT_DEFINED_ERROR;
      if (nd->type != param->type) return EXIT_TYPE_ERROR;

      data = nd->data;
    case l_int:
    case l_real:
    case l_str:
      if ((new = SymbolTableCreateNode(param->keyValue, param->type, data)) == NULL)
      {
        __SymbolTableDispose(&new);
        return EXIT_INTERNAL_ERROR;
      }
      SymbolTableInsert(function->table, new); // vlozime nove vytvoreny symbol se zkopirovanou hodnotou odkazovaneho
      break;
    default:
      return EXIT_SYNTAX_ERROR;
  }
  printDebug("Parametr volani nacten\n");

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep) return EXIT_SYNTAX_ERROR; // neni carka, ale nemam vsechny parametry -> chyba

  return paramsCall(in, table, lex, function, param->next);
}

/*   Volani funkce
 * ---------------------------------------------------------------------
 *   <callFunction> -> id "(" <paramsCall> ")"
 */
int callFunction(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex)
{
  int result = EXIT_SUCCESS;
  printDebug("Volani funkce\n");
  struct node * nd;
  if (lex->type == l_id)
  {
    // musim overit, ze identifikator existuje a je definovana funkce
    if ((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if ((nd->type != k_function) || (((funcData *)nd->data)->defined) == false)
      return EXIT_TYPE_ERROR;
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
    if ((result = paramsCall(in, table, lex, (funcData *)nd->data, ((funcData *)nd->data)->param)) != EXIT_SUCCESS)
      return result;
    // nacteme pravou zavorku a konec
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
    if (lex->type != l_rparenth) return EXIT_SYNTAX_ERROR;
    return EXIT_SUCCESS;
  }
  else if (lex->type == l_key)
  {
    switch (*(key *)lex->data)
    {
      case k_sort: return embededFuncSort(in, table, ilist, lex, nd);
      case k_find: return embededFuncFind(in, table, ilist, lex, nd);
      case k_length: return embededFuncLength(in, table, ilist, lex, nd);
      case k_copy: return embededFuncCopy(in, table, ilist, lex, nd);
      default:
        return EXIT_SYNTAX_ERROR;
    }
  }
  return EXIT_SYNTAX_ERROR;
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
int state(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Zpracovavam prikaz\n");
  switch (lex->type)
  {
    // <state> -> id := <evalExpression>
    // <state> -> id := <call>
    case (l_id): return embededAssign(in, table, ilist, lex);
    // <state> -> if <evalExpression> then <body> ; else <body>
    // <state> -> while <evalExpression> do <body>
    // <state> -> write ( <type> )
    // <state> -> readln ( <type> )
    case (l_key):
      switch (*(key *)lex->data)
      {
        case k_if: return embededIf(in, table, ilist, lex);
        case k_while: return embededWhile(in, table, ilist, lex);
        case k_write: return embededFuncWrite(in, table, ilist, lex);
        case k_readln: return embededFuncReadln(in, table, ilist, lex);
        default: return  EXIT_SYNTAX_ERROR;
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
int statements (struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result;
  printDebug("Zacatek noveho prikazu\n");
  // nebyl to end, to jsme kontrolovali jeste v body
  // tzn ze tam bude nejaky prikaz
  // zavolame state
  result = state(in, table, ilist, lex);
  if (result != EXIT_SUCCESS) return result;

  // pokud dalsi token je strednik tzn ze bude
  // nasledovat dalsi prikaz
  if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
  if (lex->type == l_endl)
  {
    printDebug("Nasleduje prikaz\n");
    if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
    return statements(in, table, ilist, lex);
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
int body(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Zacatek bloku kodu\n");
  int result = EXIT_SUCCESS;
  if ((result = fillToken(in, lex)) != EXIT_SUCCESS)
    return result;
  // naplneny token, pokud nic neselze
  // zkontroluje prazdne telo programu, kdy po "begin" nasleduje hned "end"
  if (lex->type == l_key && *(key *)lex->data == k_end )
    { printDebug("Prazdny blok kodu\n"); return result; }
  // a projde telo programu -> pokud cokoliv z toho selze, vraci error
  if ((result = statements(in, table, ilist, lex)) != EXIT_SUCCESS)
    return result;

  printDebug("Konec bloku kodu\n");
  if ((lex->type != l_key) || (*(key *)lex->data != k_end))
    return EXIT_SYNTAX_ERROR; // za telem programu musi byt "end",

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
int parser(struct input * in, btree * table, tListOfInstr * ilist)
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
            ((result = function(in, table, ilist, &lex)) != EXIT_SUCCESS) || // Kontrola funkci deklarace (nemusi byt nic deklarovano)
            ((result = fillToken(in, &lex)) != EXIT_SUCCESS) || // nactu dalsi token (posun kvuli deklaracim funkci)
            ((result = body(in, table, ilist, &lex)) != EXIT_SUCCESS)) { break; } // Hlavni program begin

        if (((result = fillToken(in, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_enddot) ||
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

  if (result == EXIT_SEMANTIC_ERROR) printErr("SEMANTIC ERROR on line %d: Variable type conflict, use of an undeclared variable or conflict redeclarations.\n", in->line);
  if (result == EXIT_SYNTAX_ERROR) printErr("SYNTAX ERROR on line %d: Check your program once more, please.\n", in->line);
  if (result == EXIT_INTERNAL_ERROR) printErr("INTERNAL ERROR on line %d: Something really bad happend. This is not your fault.\n", in->line);
  tokenFree(&lex);
  return result;
}
