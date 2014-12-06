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

 #define dINT 6
 #define dREAL 7
 #define dSTR 8
 #define dBOOL 9

 //hlavicka syntaktickeho analyzatoru

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist);
int declList(FILE * source, btree * table, token * lex);
int declListNext(FILE * source, btree * table, token * lex);
int nextParam(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int param(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int body(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int function(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int parser(FILE * source, btree * table, tListOfInstr * ilist);
int state(FILE * source, btree * table, tListOfInstr * ilist, token * lex);
int statements (FILE * source, btree * table, tListOfInstr * ilist, token * lex);

