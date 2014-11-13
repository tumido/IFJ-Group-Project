/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef BTREE_INCLUDED
#define BTREE_INCLUDED
#include <stdlib.h>
#include <stdio.h>

/*  Struktura pro binarni strom
 * --------------------------------------------------------------------
 * - (??)
 */
typedef int btree;

/*   Vytvoreni tabulky symbolu
 * ---------------------------------------------------------------------
 * - vytvori zakladni strukturu tabulky pomoci binarniho vyhledavaciho
 *   stromu
 * - vraci ukazatel na strukturu btree
 */
btree * SymbolTableInit();

/*   Vlozeni noveho prvku do tabulky symbolu
 * ---------------------------------------------------------------------
 * - zapoji novy prvek do tabulky symbolu na spravne misto
 * - vstupnimy parametry funkce jsou tabulka symbolu a prvek, ktery ma
 *   byt vlozen
 */
void SymbolTableInsert(btree * table, ...);

/*   Zruseni tabulky symbolu
 * ---------------------------------------------------------------------
 * - zrusi tabulku symbolu (nerekurzivne z duvodu rychlosti)
 */
void SymbolTableDispose(btree * table);

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - prohleda tabulku, nejspis bude rozhodovat jestli pre/in/postorder
 * - vraci index do tabulky (??)
 */
int SymbolTableSearch(btree * table);

#endif
