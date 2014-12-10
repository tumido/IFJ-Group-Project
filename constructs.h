/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  12/09/2014 09:44:50 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef CONSTRUCTS_INCLUDED
#define CONSTRUCTS_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "scanner.h"
#include "strings.h"
#include "ilist.h"
#include "btree.h"
#include "io.h"


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

/*   Vestavna funkce Length
 * ---------------------------------------------------------------------
 * - jen wrapper nad funkci assign, ktere preda delku retezce a cilovou
 *   prommennou (kontroluje jeji typ -> int)
 */
int embededFuncLength(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * retNode);
int embededFuncCopy(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * retNode);
int embededFuncFind(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * retNode);
int embededFuncSort(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * retNode);

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
#endif
