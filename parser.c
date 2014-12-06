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
    case (l_mul):
    case (l_add):
    case (l_div):
    case (l_sub):
    case (l_bool):
    case (l_id): j=12; break;
    case (l_eof): j=13; break;
    default j=13;
  }

 return j;
}


int expression(FILE *source, btree *table, token *lex)
{
 sData itemAct; // tady bude aktualni token
 sData itemTop; // tady bude token co je navrcholu zasobniku
 sData itemC;   // pomocne tokeny
 sData itemD;
 sData itemSign; // tady uchovam operator
 if (tokenInit(&itemA.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemB.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemC.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemD.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemE.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 itemAct.TypTok=l_eof;  // ulozim si typ EOF, slouzi pouze jako zarazka v zasobniku
 Stack s;             // inicializuji si zasobnik
 sInit (&s);
 sPush (&s,itemAct);      // ulozim item s EOF

 int result=EXIT_SUCCESS;

 struct node *nd;
 // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
 // do itemAct.lexdata.data
 // nevim jak to udelat, zatim necham jednoduchy prirazeni
 //++++++++++++++++++++++++++++++
 if (lex->type=l_id)
 {
  if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
  return EXIT_SEMANTIC_ERROR;
  // ulozim si ze stromu hodnoty
  itemAct.lexdata.data = nd->data; // ulozime si hodnotu
  itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
  itemAct.TypTop = l_id;
 }
 // jestli je token int,real,str ulozim si hodnotu a typ
 else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
 {
  itemAct.lexdata.data = lex->data;
  itemAct.lexdata.type = lex->type;
  itemAct.TypTop = l_id;
 }
 else
 // pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
  itemAct.TypTok=lex->type;

 itemTop=sTop(&s); // kouknu se, co je nahore v zasobniku
 do
 {
  switch (Table[retIndex(itemTop.TypTok),retIndex(itemAct.TypTok)]):
  {
  // pokud je to =
  case Staight:
    sPush (&s,itemAct);  // dam na zasobnik token a vezmu dalsi
    itemTop= sTop (&s);  // iTop = vrhcol zasobniku
    // do iAct si nactu dalsi token
    if (result=fillTOken (source,lex) == EXIT_LEXICAL_ERROR) return EXIT_LEXICAL_ERROR;
    // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
    // do itemAct.data
    // nevim jak to udelat, zatim necham jednoduchy prirazeni
    //++++++++++++++++++++++++++++++
    if (lex->type=l_id)
    {
     if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
     return EXIT_SEMANTIC_ERROR;
     // ulozim si ze stromu hodnoty
     itemAct.lexdata.data = nd->data; // ulozime si hodnotu
     itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
     itemAct.TypTop = l_id;
    }
    // jestli je token int,real,str ulozim si hodnotu a typ
     else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
     {
      itemAct.lexdata.data = lex->data;
      itemAct.lexdata.type = lex->type;
      itemAct.TypTop = l_id;
     }
     else
     itemAct.TypTok=lex->type;
  break;

  // pokud je to <
  // koukneme se co je na vrhcolu zasobniku
  // jestli je nahore ], tak pred nej dam [
  // jestli tam neni tak jenom dam [
  case Left:
    itemC= sTop (&s); // koukneme se co je nahore
    itemTop.TypTok= l_lbracket; // nastavim < ([)
    // jestli je nahore E (])
    if (itemC.TypTok == l_rbracket)
    {
     // zamenime E (]) za <E ([])
     itemC=sPop(&s); // popneme E (])
     sPush (&s,itemTop);
     sPush (&s,itemC);
    }
    // na vrcholu neni E (])
    // vlozime < ([)
    else
    {
     sPush(&s,itemTop);
    }
    // nakonec vlozime token na vrchol
    sPush (&s,itemAct);
    //nactu dalsi token a itemTop bude vrchol zasobniku
    if (result=fillTOken (source,lex) == EXIT_LEXICAL_ERROR) return EXIT_LEXICAL_ERROR;
    itemTop=sTop(&s);
    // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
    // do itemA.data
    // nevim jak to udelat, zatim necham jednoduchy prirazeni
    //++++++++++++++++++++++++++++++
    if (lex->type=l_id)
    {
     if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
     return EXIT_SEMANTIC_ERROR;
     // ulozim si ze stromu hodnoty
     itemAct.lexdata.data = nd->data; // ulozime si hodnotu
     itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
     itemAct.TypTop = l_id;
    }
    // jestli je token int,real,str ulozim si hodnotu a typ
    else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
    {
     itemAct.lexdata.data = lex->data;
     itemAct.lexdata.type = lex->type;
     itemAct.TypTop = l_id;
    }
    else
    itemAct.TypTok=lex->type;
  break;

  // pokud je to >
  case Right:
    itemD=sTop(&s); // kouknu se co je nahore

    switch (itemD.TypTok)
    {
     // pokud je nahore ID
     // redukuji na E->i a ulozim si hodnotu
     case l_id:
      itemD=sPop(&s); // id,int,real popneme
      itemC=sPop(&s); // vime ze pred id je < ([)
      if(itemC.TypTok==l_lbracket)
      {
       // v i by mela byt data, kdyby se nahodou stalo
       // ze data by byla prazdna
       // jestli zadny nema je to semanticka chyba
       if (itemD.lexdata.data== NULL)
       {
         return EXIT_SEMANTIC_ERROR;
       }
       // zredukovali jsme <i na E (])
       itemD.TypTok=l_rbracket;
       itemTop=sTop(&s); // itemB bude hodnota, co je pred E (])
       sPush (&s,itemD);// pushneme E (])
       }
       break; // itemA je aktualni token porad a itemTop ma hodnotu pred E (])
       // vrchol je E (])
      case l_rbracket:
       // kdyz dojde k teto situaci tak vime, ze v zasobniku
       // pred E musi byt nejaka operace
       itemD=sPop(&s); // Popneme E (])
       itemSign=sPop(&s); // popneme operaci, (mela by byt operace)
       if (itemSign.TypTok >=l_add && itemSign.TypTok <= l_not)
       {
        itemC=sPop(&s);// popneme druhy E (])
        itemTop=sPop(&s);
        // pred E by melo byt < ([)
        // zkontrolujeme jestli tam bylo <E
        if ((itemTop.TypTok == l_lbracket) && (itemC.TypTok==l_rbracket) )
        {
         // mel bych si vygenerovat unikatni nazev pro vysledek
         //++++++++++++++++++++
         switch(itemSign.TypTok)
         {
            // operace scitani
            case l_add:
             // int + int = int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int + real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real + int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real+real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             // real
             // str+str str (konkatenace)
             else if ((itemD.lexdata.lexType==l_str) && (itemC.lexdata.lexType==l_str) )
             //str
             else return EXIT_SEMANTIC_ERROR;
             //if (generateInstruction (I_ADD,.....))
             // +++++++
             // nastavit vrchol zasobniku
            break;
            // operace odcitani
            case l_sub:
             // int - int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int - real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real - int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real+real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             else return EXIT_SEMANTIC_ERROR;
             //if(generateInstruction(I_SUB,....
             //++++++++++++++++++++++++++++
            break;
            // nasobeni
            case l_mul:
             // int*int = int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int * real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real * int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real*real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             else return EXIT_SEMANTIC_ERROR;
             //if(generateInstruction(I_MUL,....
             //++++++++++++++++++++++++++++
            break;
            // deleni
            case l_div:
             // int/int = real
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // real / real = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             else return EXIT_SEMANTIC_ERROR;
             // if(generateInstruction(I_DIV
             //********************************
            break;
            case l_greater:
             //if(generateInstruction(I_GREATER
            break;
            case l_less:
             //if(generateInstruction(I_LESS
            case l_gequal:
             //if(generateInstruction(I_GREATEQ,
            case l_lequal:
             //if(generateInstruction(I_LESSEQ,
            case l_equal:
             //if(generateInstruction(I_EQ ..
            case l_not:
             //if(generateInstruction(I_NOTEQ
            default:
             return EXIT_SEMANTIC_ERROR;
           }
        }
      }
      break;
      // pokud je tam zavorka
      case l_rparenth:
      //++++++++++++++
      break;
      default: return EXIT_SEMANTIC_ERROR; break;
   }
   break;
   case Err:
   //++++++++++++++++
   break;
  }
 }while (itemTop.TypTok != l_eof && itemAct.TypTok!= l_eof) ;

 if (itemTop.TypTok == l_eof && itemAct.TypTop== l_eof)
 result = EXIT_SUCCESS;

 return result;
}





//=====================================================
//      Deklaracni funkce
//      Pravidlo <decList> -> var <DecListNext>
//      Tato funkce kontroluje gramatiku deklarace
//       (Mela by je ukladat do binarniho stromu)
//=====================================================
int declList(FILE * source, btree * table, token * lex)
{
  printErr("declList\n");
  // Pokud je to klicove slovo var
  // pokud ne, nejedna se o deklaraci tak se vratime zpet, kde nas volali
  if ((lex->type != l_key) || (*((key *)lex->data) != k_var))
      return EXIT_SUCCESS;

  return declListNext(source, table, lex);
}
// pravidlo <decListNext> -> id: typ; <decListNext>
// <decListNext> -> eps
int declListNext(FILE * source, btree * table, token * lex)
{
  printErr("declListNext\n");
  int result;
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

        printErr("skorokonec");
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

  if ((tokenInit(&lex) == EXIT_SUCCESS) &&
      ((result = fillToken(source, &lex)) == EXIT_SUCCESS))
  {
    switch(lex.type)
    {
      case l_key: // pokud je to klicove slovo, var,function,begin (vyuzivam toho, ze C vyhodnocuje disjunkci zleva)
        if ((*(key *)lex.data != k_var) && (*(key *)lex.data != k_function) && (*(key *)lex.data != k_begin))
          { result = EXIT_SYNTAX_ERROR; break; }

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

  if (result == EXIT_SYNTAX_ERROR) printErr("Syntax error found. Check your program once more, please.\n");
  tokenFree(&lex);
  return result;
}
