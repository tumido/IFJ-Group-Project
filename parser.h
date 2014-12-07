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
int declareList(FILE * source, btree * table, token * lex);
int declareListContent(FILE * source, btree * table, token * lex);
int paramsList(FILE * source, token * lex, unsigned int * count, struct funcParam ** param);
int body(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int function(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int parser(FILE * source, btree * table, tListOfInstr * ilist);
int state(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int statements (FILE * source, btree * table, tListOfInstr * ilist, token * lex);

