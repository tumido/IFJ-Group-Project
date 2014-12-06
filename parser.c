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

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(ilist, I);
   return;
}


//=====================================================
//      Deklaracni funkce
//      Pravidlo <decList> -> var id : typ ; <DecListNext>
//      Tato funkce kontroluje gramatiku deklarace
//       (Mela by je ukladat do binarniho stromu)
//=====================================================
int declList(FILE * source, btree * table, token * lex)
{
  printErr("declList\n");
  int result;
  // Pokud je to klicove slovo var
  // pokud ne, nejedna se o deklaraci tak se vratime zpet, kde nas volali
  if ((lex->type != l_key) || (*((key *)lex->data) != k_var))
      return EXIT_SUCCESS;

  // Pozadame o dalsi token, vime ze to musi byt identifikator
  // Pokud to neni identifikator l_id, vratim chybu
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;

  if ( SymbolTableSearch(table, ((string *)lex->data)->str) != NULL) return EXIT_SYNTAX_ERROR;

  // zalohujeme si token, zatim nevime jakej je to typ
  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS){ return result; }

  struct node * nd;
  if ((nd = SymbolTableCreateNode(*((key *)lex->data), tmp)) == NULL)
  {
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }

  SymbolTableInsert(table, nd);
  // Pozadam o dalsi token ktery musi byt ';'
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;

  // pozadam uz jen o dalsi token a zavolam decListNext
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  return declListNext(source, table, lex);
}
// pravidlo <decListNext> -> id: typ; <decListNext>
// <decListNext> -> eps
int declListNext(FILE * source, btree * table, token * lex)
{
  printErr("declListNext\n");
  int result;
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_id) return EXIT_SUCCESS;

  if ( SymbolTableSearch(table, ((string *)lex->data)->str) == NULL) return EXIT_SYNTAX_ERROR;

  // zalohujeme si token, zatim nevime jakej je to typ
  token tmp; // zaloha tokenu
  if ((result = tokenInit(&tmp)) != EXIT_SUCCESS) return result;
  tmp.type = lex->type;
  tmp.data = lex->data;
  lex->data = NULL;

  // Dalsi token musi byt ":"
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS){ tokenFree(&tmp); return result; }
  if (lex->type != l_colon) { tokenFree(&tmp); return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS){ tokenFree(&tmp); return result; }

  struct node * nd;
  if ((nd = SymbolTableCreateNode(*((key *)lex->data), ((string *)tmp.data)->str)) == NULL)
  {
    __SymbolTableDispose(&nd);
    tokenFree(&tmp);
    return EXIT_INTERNAL_ERROR;
  }

  switch (lex->type)
  {
    case l_int:
    case l_real:
    case l_str:
      nd->data = lex->data;
      break;
    default:
      __SymbolTableDispose(&nd);
      tokenFree(&tmp);
      return EXIT_INTERNAL_ERROR;
  }

  tokenFree(&tmp); // uz jej nepotrebuju
  SymbolTableInsert(table, nd);
  // Pozadam o dalsi token ktery musi byt ';'
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;

  // pozadam uz jen o dalsi token a zavolam decListNext
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  return declListNext(source, table, lex);
}


// ==================================================================
// deklarace funkci
// <function> -> ID ( <param>): typ ; <forward> <declList> <body>; <function>
// fce kontroluje gramatiku podle deklaraci funkci
// (musi se doplnit ukladani funkci ID parametry atd.)
// ===================================================================
int function(FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
/*  int result;

  if (strcmp("function", ((string *)lex->data)->str) != EXIT_SUCCESS)
    return EXIT_SUCCESS;

  // Dalsi token musi byt ID, ktere si musim zatim zalohovat
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;
  //zalohujeme si token
  if (tokenInit(&Zaloh) != EXIT_SUCCESS)
    return EXIT_INTERNAL_ERROR;
  Zaloh=Token;

  // dalsi token musi byt "("
  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;

  if ((result = fillToken(source,lex)) != EXIT_SUCCESS) return result;
  // dalsi token by mel byt parametr
  if ((result = param()) != EXIT_SUCCESS) return result;

  // dalsi token musi byt ")" a pak ":"
  if (Token.type!= l_rparenth) return SYNTAX_ERROR;
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
  if (Token.type!= l_colon)  return SYNTAX_ERROR;

  // Dalsi token musi byt datovy typ
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR){ tokenFree(Zaloh); return LEX_ERROR;}

  switch (Token.type)
  {
  // pokud je to integer, real atd.
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Tady bych mel vlozit ted ID+TYP+?parametry? do stromu +
  // pak se jeste zjisti jestli je to prototyp nebo ne
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    case l_int: tokenFree(Zaloh);
      break;
    case l_real: tokenFree(Zaloh);
      break;
    case l_str: tokenFree(Zaloh);
      break;
    case l_bool: tokenFree(Zaloh);
      break;
    default:
      tokenFree(Zaloh);
      return SYNTAX_ERROR;
      break;
  }
  // dalsi co by mi melo prijit je strednik
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
  if (Token.type!= l_endl)  return SYNTAX_ERROR;

  // Ted je vice moznosti co mi muze dojit
  // 1. forward 2. var (deklarace lokalnich promennych)
  // begin  (telo funkce)
  // forward ani deklarace nemusi byt

  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
  // nejdrive zkontrolujeme jestli to neni forward a pak var
  // protoze to predchazi begin
  //  <forward> -> ; <function>
  if (strcmp("forward", ((string * )lex->data)->str) == EXIT_SUCCESS)
  {
      // jestli to byl forward, tak poslu dalsi token kterej musi byt ;
      if ((result = fillToken(source,lex)) != EXIT_SUCCESS)  return result;
      if (lex->type != l_endl) return ;


  }
  else if (strcmp("var",Token.data)==EXIT_SUCCESS)
  {
    result=declList();
    if (result != EXIT_SUCCESS) return result;
  }
  // v jinym pripade by se melo jednat o begin
  // stejne to zkontrolujeme radsi
  if (strcmp("begin",Token.data) == EXIT_SUCCESS)
  {
      result=body();
      if (result != EXIT_SUCCESS) return result;
    //++++++++++++++++ NEVIM JESTLI VOLAT DALSI TOKEN NEBO UZ BUDE ZAVOLAN
      // funkce musi koncit ;
    if (Token.type!= l_endl)  return SYNTAX_ERROR;
  }
  else return SEM_ERROR;
  // zavolame si o dalsi token a zavolame rekurzivne funkci
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
  return function(); */ printErr("Funkce\n"); return EXIT_SUCCESS;
}
//===================================
// <Param> -> ID: typ <NextParam>
// Funkce kontroluje parametry funkci
// Pokud zadny parametr nebyl nepokracuje
// jinak vola funkci NextParam
// ====================================
int param(FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
/*  int result;
   // zkontrolujeme jestli tam vubec nejaky parametr je
   if (Token.type== l_rparenth ) return EXIT_SUCCESS;

   // dalsi token musi byt ID
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.type!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.type!= l_colon)  return SYNTAX_ERROR;

   // Dalsi token musi byt typ
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    switch{ (Token.type)
      // pokud je to integer, real atd.
      // ++++++++++++++++++++++++++++++++++++++++++++++
      // meli bychom ted k te funkci ulozit parametry
      // zatim vynecham pak doplnime
       case l_int:
         break;
       case l_real:
         break;
       case l_str:
         break;
       case l_bool:
         break;
       default:
         return SYNTAX_ERROR;
         break;
      }

  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   return result = NextParam; */ return EXIT_SUCCESS;

}
//=======================================================================
// dalsi parametr <NextParam> -> ; ID: TYP <Nextparam>
// zkontroluje jestli tam je strednik jestli ano, tak bude dalsi parametr
// jinak podobna funkci Param
//========================================================================
int nextParam(FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
/*  int result;
  // zkontroluji jestli tam je strednik jestli neni tak asi zadny dalsi param
  if (Token.type != l_endl ) return EXIT_SUCCESS;

   // dalsi token musi byt ID
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.type!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.type!= l_colon)  return SYNTAX_ERROR;

   // Dalsi token musi byt typ
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    switch{ (Token.type)
      // pokud je to integer, real atd.
      // ++++++++++++++++++++++++++++++++++++++++++++
      // meli bychom ted k te funkci ulozit parametry
      // zatim vynecham pak doplnime
       case l_int:
         break;
       case l_real:
         break;
       case l_str:
         break;
       case l_bool:
         break;
       default:
         return SYNTAX_ERROR;
         break;
      }
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   return result = NextParam; */ return EXIT_SUCCESS;

}

//================================================
// Tady budou vsechny cykly
// read, write
// volani funkci
// expresions
//<state> -> id := <expression>
//<state> -> id := id (<term>)
//<state> -> if <expression> then<body> ; else <body> ;
//<state> -> while <expression> do <body> ;
//<state> -> readln (<type>)
//=================================================
int state(FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
/*  int result;

  tData *varInfo;
  token zaloh;

  switch (Token.type)
  {
    // pokud token je id
    // <stat> -> id := <expression>
    // <stat> -> id := <term>
    case (l_id):
    // musim si ulozit data, kam se bude nacitat
    // ++++++++++++ doplnit
    // ++++++++++++ prohledat strom, jestli identifikator neexistuje
    // ++++++++++++ tak je to sem. chyba

    // dalsi znak musi byt :=
    if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    if (Token.type!=l_assign) return SYNTAX_ERROR;

    // dalsi token je budto expression nebo id funkce
    if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    // pokud je to id
    // vestavene funkce by bylo asi nejlepsi hned vlozit do stromu
    // +++++++++++++++++++++++++++++
    // zatim to neresim, pak doplnim
    if (Token.type==l_id || Token.type==l_key )
    {
       // vim ze dalsi token musi byt zavorka
       if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
       if (Token.type!= l_lparenth) return SYNTAX_ERROR;

       if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
       // ++++++++++++++++++++++++
       // ted ocekavame TERM, funkci dopisu pak ji sem dam

    }
    else
    {
    // ted co?? Zdola nahoru SA zkontrolovat expression ?
    // pak generovat kod?
    // +++++++++++++ musime doplnit

    }
    return result;
    break;

    // nacetly jsme klicove slovo
    // tzn ze by to melo byt cyklus while nebo if
    // nebo readln nebo write
    case (l_key):
    // pokud se jedna o cyklus if
     if (strcmp("if",Token.data)==EXIT_SUCCESS)
     { //++++++++++++++++++++++++++++++++++++
       // dalsi musi byt expression
       // tady to musim doplnit, ukladani do tab.symbolu
       // generovat vyraz
       // vysledek vyrazu si musim ulozit
       //  ++++++++++++++++++++++++++++++

       // zkontrolovat dalsi token musi byt then
       // nactu dalsi token
       // ted zavolam body
       // tady se musi resit skoky atd.
       // KURVAAAAA boli me uz hlava
       // generovat a srat se se skokama
       //dalsi token else
       //++++++++++++++++++++++++++++++++++++++++++
     }

     //<state> -> while <expression> do <body> ;
     else if (strcmp ("while",((string * )Token.data)->str )==EXIT_SUCCESS)
     {

      // dalsi token co nas ceka je vyraz
      // to dame pak zpracovat SA zdola nahoru
      // nejdriv nagenerujeme instrukci navesti

      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      // vygenerujeme instrukci navesti
      if(generateInstruction(I_LAB, NULL, NULL, NULL)) return INTER_ERROR;
      // ted potrebujeme ulozit adresu za WHILE
      void *addrLab1;
      addrLab1 = listGetPointerLast (list) ; // vraci nam ukazatel na posledni instukci

      // potrebujeme unikatni nazev promenne
      if ((result=tokenDetailInit(zaloh))!= EXIT_SUCCESS) return INTER_ERROR;
      if(generateVariable(&zaloh))   return INTER_ERROR;
      //++++++++++++++++++++++++++++++
      // tady bych si mel ulozit promennou
      // varInfo= tady bych si mel ulozit adresu s daty
      tokenDetailFree(&zaloh); // uvolnim nazev
      //
      // ++++++++++++++++++++++++++++++++++++
      //ted tu musi byt uz cteni EXPRESSION
      // Zdola nahoru

      //  Musime si vygenerovat instrukci pod.skoku a ulozit si adresu
      void *addrIfNotGoto;

      if(generateInstruction(I_IFNOTGOTO, (void*) varInfo, NULL, NULL))return INTER_ERROR;
      addrIfNotGoto = listGetPointerLast (list);

      //nevim jestli nacitat dalsi token nebo ho dostanu od READEXPRESSION funkce
      // kazdopadne dalsi token musi byt DO
      if (strcmp(Token.data, "do")!= EXIT_SUCCESS) return SYNTAX_ERROR;

      // dalsi token musi byt slozeny prikaz, zavolame begin
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      result = body();
      if (result != EXIT_SUCCESS) return result;

      // je potreba ted instrukce skoku
      // skoci za while nad adressu Lab1
      if (generateInstuction (I_GOTO,NULL,NULL, (void*)addrLab1)) return INTER_ERROR;

      //musime si udelat navesti kam to ma skocit kdyz podminka neplati
      if(generateInstruction(I_LAB, NULL, NULL, NULL)) return INTER_ERROR;

      void *addrLab2;
      addrLab2= listGetPointerLast(list); // ukazatel je nyni za posledni instrukci

      // zname adresu 2 navesti
      // nastavime aktualni instrukci

      listGoto(list,addrIfNotGoto);
      tInstr *data;
      data=listGetData(list);
      data->addr3= addrOfLab2;

      return EXIT_SUCCESS;

     }
     else if (strcmp ("readln",Token.data)==EXIT_SUCCESS)
     {
         // vim ze dalsi token je zavorka (
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (Token.type != l_lparenth) return SYNTAX_ERROR;

         // nacteme dalsi token
         // vime ze podle pravidla tady musi byt typ
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         // to musim jeste nejdriv dopsat ten type..
         //return type();
     }
     else if (strcmp ("write",Token.data)==EXIT_SUCCESS)
     {
         // vim ze dalsi token je zavorka (
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (Token.type != l_lparenth) return SYNTAX_ERROR;

         // nacteme dalsi token
         // vime ze podle pravidla tady musi byt <term>
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         // to musim jeste nejdriv dopsat ten type..
         //return term();
     }
     else return SYNTAX_ERROR;
     break;
     default break;

  } */ printErr("Statement\n"); return EXIT_SUCCESS;


}


//===============================================
// <statements> -> <state> ; <statements>
// <statements> -> <state>
//================================================

int statements (FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
  int result;
  printErr("Statements\n");
  // nebyl to end, to jsme kontrolovali jeste v body
  // tzn ze tam bude nejaky prikaz
  // zavolame state
  result = state(source, table, ilist, lex);
  if (result != EXIT_SUCCESS) return result;

  // pokud dalsi token je strednik tzn ze bude
  // nasledovat dalsi prikaz
  if (lex->type == l_endl)
    return statements(source, table, ilist, lex);
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
int body(FILE * source, btree * table, tListOfInstr * ilist, token * lex)
{
  printErr("Body\n");
  int result;
  if (((result = fillToken(source, lex)) != EXIT_SUCCESS) ||
      // naplneny token, pokud nic neselze
   //   (strcmp("end", ((string *)lex->data)->str) == EXIT_SUCCESS) ||
      // zkontroluje prazdne telo programu, kdy po "begin" nasleduje hned "end"
      ((result = statements(source, table, ilist, lex)) != EXIT_SUCCESS))
      // a projde telo programu -> pokud cokoliv z toho selze, vraci error
    return result;

  if (strcmp("end", ((string *)lex->data)->str) != EXIT_SUCCESS)
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
int parser(FILE * source, btree * table, tListOfInstr * ilist)
{
  printErr("parser\n");
  token lex;
  int result = EXIT_SUCCESS;

  if ((tokenInit(&lex) != EXIT_INTERNAL_ERROR) &&
      ((result = fillToken(source, &lex)) != EXIT_INTERNAL_ERROR) &&
      (result != EXIT_LEXICAL_ERROR))
  {
    switch(lex.type)
    {
      case l_key: // pokud je to klicove slovo, var,function,begin (vyuzivam toho, ze C vyhodnocuje disjunkci zleva)
        if (((result = declList(source, table, &lex)) != EXIT_SUCCESS) || //Kontrola deklarace promennych (nemusi byt nic deklarovano)
            ((result = function(source, table, ilist, &lex)) != EXIT_SUCCESS) || // Kontrola funkci deklarace (nemusi byt nic deklarovano)
            ((result = body(source, table, ilist, &lex)) != EXIT_SUCCESS)) { break; } // Hlavni program begin

        if (((result = fillToken(source, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_enddot) ||
           ((result = fillToken(source, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_eof)) { result = EXIT_SYNTAX_ERROR; break; } // na konci programu musi byt . a pak EOF

          //generateInstruction(I_END, NULL, NULL, NULL); // instrukce?? + ilist
        break;
      case l_eof: // prazdny soubor
        break;
      default: // cokoliv jineho je chyba
        result = EXIT_SYNTAX_ERROR;
        break;
    }
  }

  tokenFree(&lex);
  return result;
}
