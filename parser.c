z�� /*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
 #include <stdio.h>
 #include "scanner.h"
 #include "btree.h"
 #include "parser.h"
 #include "scanner.h"
 #include "io.h"
 #include "ilist.h"



node *NODE;      // globalni promena obsahujici tabulku symbolu
tListofInstr *list; // GP seznam isntrukci
token Token;        // zde bude ulozen aktualni token






// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
  source = f;
}

// generuje jedinecne nazvy identifikatoru
// nazev se sklada ze znaku $ nasledovanym cislem
// postupne se tu generuji prirozena cisla a do nazvu promenne se ukladaji
// v reverzovanem poradi - na funkcnost to nema vliv, ale je jednodussi implementace
int counterVar = 1;
void generateVariable(token *var)
{
  tokenDetailClean(var);
  addCharToString(var, '$');
  int i;
  i = counterVar;
  while (i != 0)
  {
    addCharToString(var, (char)(i % 10 + '0'));
    i = i / 10;
  }
  counterVar ++;
}

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   if(listInsertLast(list, I))
    return INTER_ERROR;
    return 0;
}


//=====================================================
//      Deklaracni funkce
//      Pravidlo <DecList> -> var id : typ ; <DecList>
//      Tato funkce kontroluje gramatiku deklarace
//       (Mela by je ukladat do binarniho stromu)
//=====================================================
int declList()
{
  int result;
  int i;
  token Zaloh; // zaloha tokenu
  // Pokud je to klicove slovo var nebo pak uz jen klicove slovo
  // kdyz budeme volat znova tuto funkci
  if (strcmp("var",Token.data)==0 || Token.lexType== l_id)
  {
    // Pokud tuto funkci prochazim poprve udela se i tento krok
    if(Token.lexType!=l_id)
    {
     // Pozadame o dalsi token, vime ze to musi byt identifikator
     // Pokud to neni identifikator l_id, vratim chybu
     if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
     if (token.lexType != l_id) return SYNTAX_ERROR;
    }// jestli byla volana rekurzivne znova tato funkce
     // tak identifikator uz bude token tak nesmime nacitat znova
    // zalohujeme si token, zatim nevime jakej je to typ
    if (i=tokenDetailInit(&Zaloh) != EXIT_SUCCESS);
    return INTER_ERROR;
    //+++++++++++++++++++++++++++++++++
    // tady musim prohledat strom jestli nedeklaruji 2x stejny jmeno id
    //++++++++++++++++++++++++++++++++++++++++
    Zaloh=Token;

    // Dalsi token musi byt ":"
    if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR){ tokenFree(Zaloh); return LEX_ERROR;}
    if (Token.lexType != l_colon) { tokenFree(Zaloh); return SYNTAX_ERROR;}

    // Dalsi token musi byt datovy typ
    if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR){ tokenFree(Zaloh); return LEX_ERROR;}

     switch{ (Token.lexType)
      // pokud je to integer, real atd.
      // ++++++++++++++++++++++++++++++++++++++++++
       // Tady bych mel vlozit ted ID+TYP do stromu
       //++++++++++++++++++++++++++++++++++++++++++
       case l_int:
           // nechapu to volani tohoto stromu
           // SymbolTableInsert( &NODE, )

            tokenFree(Zaloh);
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
     // Pozadam o dalsi token ktery musi byt ':'
     if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
     if (Token.lexType != l_endl) return SYNTAX_ERROR;

     // pozadam uz jen o dalsi token a zavolam znova dec.list
     if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
     return result=declList();
    }
    else return SYNTAX_OK;
  }


// ==================================================================
// deklarace funkci
// <function> -> ID ( <param>): typ ; <forward> <declList> <body>; <function>
// fce kontroluje gramatiku podle deklaraci funkci
// (musi se doplnit ukladani funkci ID parametry atd.)
// ===================================================================
function()
{
    int result;
    token Zaloh;

    if (strcmp("function",Token.data)==0)
    {

      // Dalsi token musi byt ID, ktere si musim zatim zalohovat
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
      if (Token.lexType != l_id) return SYNTAX_ERROR;
     //zalohujeme si token
      if (i=tokenDetailInit(&Zaloh) != EXIT_SUCCESS);
      return INTER_ERROR;
      Zaloh=Token;

      // dalsi token musi byt "("
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      if (Token.lexType!= l_lparenth)  return SYNTAX_ERROR;

      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      // dalsi token by mel byt parametr
       result = Param();
       if (result != SYNTAX_OK) return;

      // dalsi token musi byt ")" a pak ":"
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      if (Token.lexType!= l_colon)  return SYNTAX_ERROR;

      // Dalsi token musi byt datovy typ
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR){ tokenFree(Zaloh); return LEX_ERROR;}

      switch{ (Token.lexType)
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
      if (Token.lexType!= l_endl)  return SYNTAX_ERROR;

      // Ted je vice moznosti co mi muze dojit
      // 1. forward 2. var (deklarace lokalnich promennych)
      // begin  (telo funkce)
      // forward ani deklarace nemusi byt

      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      // nejdrive zkontrolujeme jestli to neni forward a pak var
      // protoze to predchazi begin
      //  <forward> -> ; <function>
      if (strcmp("forward",Token.data)==0)
      {
         // jestli to byl forward, tak poslu dalsi token kterej musi byt ;
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (Token.lexType!= l_endl)  return SYNTAX_ERROR;
         // musim ulozit ze je to pouze prototyp boolean nastavit true napr.
         //+++++++++++++++++++++ DOPLNIM
         // zavolam rekurzivne fci pro dalsi deklarace fci
         result = function();
      }
      else if (strcmp("var",Token.data)==0)
      {
        result=decList();
        if (result != SYNTAX_OK) return result;
      }
      // v jinym pripade by se melo jednat o begin
      // stejne to zkontrolujeme radsi
      if (strcmp("begin",Token.data)!=0)
      {
          result=body();
          if (result != SYNTAX_OK) return result;
        //++++++++++++++++ NEVIM JESTLI VOLAT DALSI TOKEN NEBO UZ BUDE ZAVOLAN
          // funkce musi koncit ;
        if (Token.lexType!= l_endl)  return SYNTAX_ERROR;
      }
      else return SEM_ERROR;
      // zavolame si o dalsi token a zavolame rekurzivne funkci
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      return function();
    }

  return SYNTAX_OK;
}
//===================================
// <Param> -> ID: typ <NextParam>
// Funkce kontroluje parametry funkci
// Pokud zadny parametr nebyl nepokracuje
// jinak vola funkci NextParam
// ====================================
int Param()
{
  int result;
   // zkontrolujeme jestli tam vubec nejaky parametr je
   if (Token.lexType== l_rparenth ) return SYNTAX_OK;

   // dalsi token musi byt ID
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.lexType!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.lexType!= l_colon)  return SYNTAX_ERROR;

   // Dalsi token musi byt typ
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    switch{ (Token.lexType)
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
   return result = NextParam;

}
//=======================================================================
// dalsi parametr <NextParam> -> ; ID: TYP <Nextparam>
// zkontroluje jestli tam je strednik jestli ano, tak bude dalsi parametr
// jinak podobna funkci Param
//========================================================================
int NextParam()
{
  int result;
  // zkontroluji jestli tam je strednik jestli neni tak asi zadny dalsi param
  if (Token.lexType != l_endl ) return SYNTAX_OK;

   // dalsi token musi byt ID
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.lexType!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (Token.lexType!= l_colon)  return SYNTAX_ERROR;

   // Dalsi token musi byt typ
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    switch{ (Token.lexType)
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
   return result = NextParam;

}

//================================================
// Tady budou vsechny cykly
// read, write
// volani funkci
// expresions
//=================================================
int state()
{
  int result;

  tData *varInfo;
  token zaloh;

  switch (Token.lexType)
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
    if (Token.lexType!=l_assign) return SYNTAX_ERROR;

    // dalsi token je budto expression nebo id funkce
    if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
    // pokud je to id
    // vestavene funkce by bylo asi nejlepsi hned vlozit do stromu
    // +++++++++++++++++++++++++++++
    // zatim to neresim, pak doplnim
    if (Token.lexType==l_id || Token.lexType==l_key )
    {
       // vim ze dalsi token musi byt zavorka
       if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
       if (Token.lexType!= l_lparenth) return SYNTAX_ERROR;

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
     if (strcmp("if",Token.data)==0)
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
     else if (strcmp ("while",Token.data)==0)
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
      if (strcmp(Token.data, "do")!= 0) return SYNTAX_ERROR;

      // dalsi token musi byt slozeny prikaz, zavolame begin
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      result = body();
      if (result != SYNTAX_OK) return result;
      // za end musi byt ;
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      if (token.lexType!=l_endl) return SYNTAX_ERROR;

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

      return SYNTAX_OK;

     }
     else if (strcmp ("readln",Token.data)==0)
     {
         // vim ze dalsi token je zavorka (
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (Token.lexType != l_lparenth) return SYNTAX_ERROR;

         // nacteme dalsi token
         // vime ze podle pravidla tady musi byt typ
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         // to musim jeste nejdriv dopsat ten type..
         //return type();
     }
     else if (strcmp ("write",Token.data)==0)
     {
         // vim ze dalsi token je zavorka (
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (Token.lexType != l_lparenth) return SYNTAX_ERROR;

         // nacteme dalsi token
         // vime ze podle pravidla tady musi byt <term>
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         // to musim jeste nejdriv dopsat ten type..
         //return term();
     }
     else return SYNTAX_ERROR;
     break;
     default break;

  }


}


//===============================================
// <statements> -> <state> ; <statements>
// <statements> -> <state>
//================================================

int statements ()
{
  int result;
  // nebyl to end, to jsme kontrolovali jeste v body
  // tzn ze tam bude nejaky prikaz
  // zavolame state
  result= state();
  if (result != SYNTAX_OK) return result;

  // pokud dalsi token je strednik tzn ze bude
  // nasledovat dalsi prikaz
  if (Token.lexType== l_endl)
    return statements();
  // pokud uz strednik nebude
  // za poslednim prikazem nema byt strednik
  // tzn ze se vratime do body
  else return result;
}

//===============================================
// <body> -> <statements> end
//================================================
int body()
{
 int result;
  // zacatek byl begin
  // nacteme dalsi token a zkontrolujeme jestli neni end
  if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
  if (strcmp("end",Token.data)==0) return SYNTAX_OK;

  result = statements();

  // pokud za poslednim prikazem nebude end je to chyba
  if (strcmp("end",Token.data)!=0) return SYNTAX_ERROR;

return result;

}



//====================================================
// Program zacina deklaraci, def. funkci nebo begin
// Zavolam deklaracni list pak function dec. list
// Nakonec bychom meli narazit na Begin -> zac.programu
// <program> -> <decList> <funciton> <body> . <EOF>
// ====================================================
int program()
{
 int result;

 switch(Token.lexType)
 {
   // pokud je to klicove slovo, var,function,begin
   case l_key:
     result = declList(); //Kontrola deklarace promennych (nemusi byt nic deklarovano)
     if (result != SYNTAX_OK) return result;

     result=function(); // Kontrola funkci deklarace (nemusi byt nic deklarovano)
     if (result != SYNTAX_OK) return result;

     result = body(); // Hlavni program begin
     if (result != SYNTAX_OK) return result;


     //ZDE MUSIM JESTE TESTOVAT KONEC SOUBORU
     // BUDE DOPLNENO
     // + GENERACE INSTRUKCE KONEC PROGRAMU
     break;

    default:
        return SYNTAX_ERROR;
      break;
   }
 return result;
}



// hlavicka parseru
// -- parametry dostane strom a instrukcni list
int Parser(node *TS, tListOfInstr *instrList,){

 int result;
 int i;

 if (i=tokenInit (Token)==EXIT_INTERNAL_ERROR)
  return INTER_ERROR;

 NODE=TS;
 list = instrList;

 result=fillToken (source,Token);

 if (result==EXIT_LEXICAL_ERROR)return LEX_ERROR;

 else if (result == EXIT_INTERNAL_ERROR) return INTER_ERROR;
 else

 result=program ();
 tokenFree(Token);

 return result;
}
