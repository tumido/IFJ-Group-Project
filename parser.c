/*
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
 #include "str.h"



node *NODE;      // globalni promena obsahujici tabulku symbolu
tListofInstr *list; // GP seznam isntrukci
token Token;        // zde bude ulozen aktualni token






// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
  source = f;
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
  if (strcmp("var",Token.data.str)==0 || Token.lexType== l_id)
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
    if (i=tokenInit(Zaloh) == EXIT_INTERNAL_ERROR);
    return INTER_ERROR;
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
     // Pozadam o dalsi token ktery musi byt ':'
     if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
     if (Token.lexType != l_endl) return SYNTAX_ERROR;

     // pozadam uz jen o dalsi token a zavolam znova dec.list
     if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
     return declList();
    }
    else return SYNTAX_OK;
  }


// ==================================================================
// deklarace funkci
// <function> -> ID ( <param>): typ ; <forward> <declList> <body> end;
// fce kontroluje gramatiku podle deklaraci funkci
// (musi se doplnit ukladani funkci ID parametry atd.)
// ===================================================================
funDecList()
{
    int result;
    token Zaloh;

    if (strcmp("function",Token.data.str)==0)
    {

      // Dalsi token musi byt ID, ktere si musim zatim zalohovat
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR) return LEX_ERROR;
      if (token.lexType != l_id) return SYNTAX_ERROR;

       tokenInit(Zaloh);
       Zaloh=Token;

      // dalsi token musi byt "("
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      if (result = fillToken(source,Token)!= l_lparenth)  return SYNTAX_ERROR;

      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      // dalsi token by mel byt parametr
       result = Param();
       if (result != SYNTAX_OK) return;

      // dalsi token musi byt ")" a pak ":"
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      if (result = fillToken(source,Token)!= l_colon)  return SYNTAX_ERROR;

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
      if (result = fillToken(source,Token)!= l_endl)  return SYNTAX_ERROR;

      // Ted je vice moznosti co mi muze dojit
      // 1. forward 2. var (deklarace lokalnich promennych)
      // begin  (telo funkce)
      // forward ani deklarace nemusi byt

      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      // nejdrive zkontrolujeme jestli to neni forward a pak var
      // protoze to predchazi begin
      //  <forward> -> ; <funDecList>
      if (strcmp("forward",Token.data.str)==0)
      {
         // jestli to byl forward, tak poslu dalsi token kterej musi byt ;
         if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
         if (result = fillToken(source,Token)!= l_endl)  return SYNTAX_ERROR;
         // musim ulozit ze je to pouze prototyp boolean nastavit true napr.
         //+++++++++++++++++++++ DOPLNIM
         // zavolam rekurzivne fci pro dalsi deklarace fci
         result = funDecList();
      }
      else if (strcmp("var",Token.data.str)==0)
      {
        result=decList();
        if (result != SYNTAX_OK) return result;
      }
      // v jinym pripade by se melo jednat o begin
      // stejne to zkontrolujeme radsi
      if (strcmp("begin",Token.data.str)!=0)
      {
          //+++++++++++++++++++++++++++++++++
          // zde pridam begin funkci
          //+++++++++++++++++++++++++++++++++
          // funkce musi koncit ;
        if (result = fillToken(source,Token)!= l_endl)  return SYNTAX_ERROR;
      }
      else return SEM_ERROR;
      // zavolame si o dalsi token a zavolame rekurzivne funkci
      if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
      return funDecList();
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
   if (result = fillToken(source,Token)!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (result = fillToken(source,Token)!= l_colon)  return SYNTAX_ERROR;

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
// dalsi parametr <NextParam> -> ; <Nextparam>
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
   if (result = fillToken(source,Token)!= l_id)  return SYNTAX_ERROR;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //  meli bychom si ulozit k te funkci parametry ale tady jeste nevime typ
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // dalsi token je :
   if (result=fillToken (source,Token) == EXIT_LEXICAL_ERROR)  return LEX_ERROR;
   if (result = fillToken(source,Token)!= l_colon)  return SYNTAX_ERROR;

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



//====================================================
// Program zacina deklaraci, def. funkci nebo begin
// Zavolam deklaracni list pak function dec. list
// Nakonec bychom meli narazit na Begin -> zac.programu
// <program> -> <decList> <funDecList> <MainStList>
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

     result=funDecList(); // Kontrola funkci deklarace (nemusi byt nic deklarovano)
     if (result != SYNTAX_OK) return result;

     result = mainStList(); // Hlavni program begin
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

 NODE=TREE;
 list = instrList;

 result=fillToken (source,Token);

 if (result==EXIT_LEXICAL_ERROR)
    result = LEX_ERROR;
 else if (result == EXIT_INTERNAL_ERROR)
  result = INTER_ERROR;
 else
  result=program ();

 tokenFree(Token);
 return result;
}
