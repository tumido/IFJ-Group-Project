/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:11 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include <stdio.h>
#include "scanner.h"
#include "btree.h"
#include "scanner.h"
#include "io.h"
#include "ilist.h"
//#include "interpret.h"
//#include "ParStack.h"

#define dINT 6
#define dREAL 7
#define dSTR 8
#define dBOOL 9


#define Right 20
#define Left 21
#define Staight 22
#define Err 23

// precedencni tabulka
//int Table [14][14] =
// radek - vstupni token,
// sloupec - znak na zasobniku
//{  //        +     -    *     div    >     <    >=    <=     =    <>    (     )    i     $
//* + */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* - */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* * */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* div */{Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* > */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* < */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* >= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* <= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
//* = */  {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
//* <> */ {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
//* ( */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Straight,Left,Err,},
//* ) */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
//* i */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
//* $ */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Err,Left,Err,},

//};

 //hlavicka syntaktickeho analyzatoru

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist);


/*   Syntakticka analyza
 * =====================================================================
 * - "zhora dolu", vyuziva rekurzivni sestup
 */


/*   Vyhodnoceni vyrazu
 * ---------------------------------------------------------------------
 * - bude namapovane na Juruv kod
 */
int evalExpression(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex);

/*   Blok deklarace promennych
 * ---------------------------------------------------------------------
 * - zpracovani bloku promenych
 * - pokud program nezacina klicovym slovem "var" je tato faze
 *   preskocena
 *
 *   <declareList> -> "var" <DeclareListContent>
 */
int declareList(struct input * in, btree * table, token * lex);

/*   Rekurzivni zpracovani promennych v deklaracni casti
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <declareListContent> -> "id" ":" "type" ";" <declareListContent>
 *   <declareListContent> -> eps
 */
int declareListContent(struct input * in, btree * table, token * lex);

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsList> -> "id" ":" "type" ";" <paramsList>
 *   <paramsList> -> "id" ":" "type"
 */
int paramsList(struct input * in, token * lex, struct funcParam ** param);

/*   Deklarace funkce
 * ---------------------------------------------------------------------
 * - zpracuje deklaraci funkce, ulozi jeji identifikator, a parametry
 *   <function> -> "function" "id" "(" <paramList> ")" ":" "typ" ";" <forward>
 *   <function> -> eps
 *   <forward> -> "forward" ";"
 *   <forward> -> <declareList> <body> ";" <function>
 */
int function(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsCall> -> "id" "," <paramsCall>
 *   <paramsCall> -> "id"
 */
int paramsCall(struct input * in, btree * table, token * lex, funcData * function, struct funcParam * param);

/*   Volani funkce
 * ---------------------------------------------------------------------
 *   <callFunction> -> id "(" <paramsCall> ")"
 */
int callFunction(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex);

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
int state(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Blok kodu
 * ---------------------------------------------------------------------
 * - zpracovavam jednotlive prikazy v bloku kodu programu
 * - rekurzivne zpracuje prikaz, ";" a znova dokud po prikazu nasleduje
 *   strednik
 *
 *   <statements> -> <state> ; <statements>
 *   <statements> -> <state>
 */
int statements (struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*  Telo programu
 * ---------------------------------------------------------------------
 * - za tokenem "begin" nasleduje telo programu -> voleme statements a
 *   pak zkontrolujeme "end"
 * <body> -> <statements> end
 * <body> -> end
 */
int body(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Parser
 * ---------------------------------------------------------------------
 * - volan hlavnim programem
 * - potrebuje znat zdrojovy soubor, tabulku symbolu a list instrukci
 * - program zacina deklaracnim liste promennych, deklaraci funkce a pak
 *   nasleduje samotne telo programu
 *
 * <program> -> <decList> <funciton> <body> <EOF>
 */
int parser(struct input * in, btree * table, tListOfInstr * ilist);

/*   Zpracovani vestavnych funkci a prikazu
 * =====================================================================
 */

/*   Vestavna funkce Write
 * ---------------------------------------------------------------------
 * - zpracovava neomezeny pocet argumentu
 * - kazdy argument vyhodnoti (zdali je to ID (platne, neni funkce) nebo
 *   hodnota) a zavola pro nej samostatnou write instrukci
 */
int embededFuncWrite(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Vestavna funkce readline
 * ---------------------------------------------------------------------
 * - vyhodnoti poskytnuty parametr a zapise do nej hodnotu ze vstupu
 *   resp. vytvori patricnou instrukci
 */
int embededFuncReadln(struct input * in, btree * table, tListOfInstr * ilist, token * lex);
int embededFuncLength(struct input * in, btree * table, tListOfInstr * ilist, token * lex);
int embededFuncCopy(struct input * in, btree * table, tListOfInstr * ilist, token * lex);
int embededFuncFind(struct input * in, btree * table, tListOfInstr * ilist, token * lex);
int embededFuncSort(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Prirazeni
 * ---------------------------------------------------------------------
 * - nacte si promennou do ktere se ma hodnota priradit a rozhodne,
 *   jestli bude volana funkce a nebo je to vyraz
 * - nasledne vola
 */
int embededAssign(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Podminene vetveni
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo podminky
 * - JAK JSOU RESENE JUMPY??
 */
int embededIf(struct input * in, btree * table, tListOfInstr * ilist, token * lex);

/*   Cyklus while
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo cyklu
 * - JAK JSOU RESENE JUMPY??
 */
int embededWhile(struct input * in, btree * table, tListOfInstr * ilist, token * lex);
