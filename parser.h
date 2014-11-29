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

 #define SYNTAX_OK     0
 #define SYNTAX_ERROR  2
 #define SEMAN_ERROR   3
 #define LEX_ERROR     4
 #define INTER_ERROR   5
 #define dINT 1
 #define dREAL 2
 #dedine dSTR 3
 #define dBOOL 4

 //hlavicka syntaktickeho analyzatoru

int Parser (node *TS, tListOfInstr *instrList);
