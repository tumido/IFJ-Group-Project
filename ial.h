/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef IAL_INCLUDED
#define IAL_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"

#define SIZE 255 // maximalni delka radky zpracovavaneho retezce

/*
 * Funkce shellSort
 * -----------------------------------------------------------------------------
 * - provadi shellovo razeni 'n' prvku v poli
 */ 
void shellSort(char *array, int n);

/*
 * Algoritmus findSubString
 * -----------------------------------------------------------------------------
 * - vyhledava podretezec zadane delky 'n' v retezci 's'
 * - implementovano pomoci boyer-mooreova algoritmu:
 *   - prochazi text zleva doprava a porovnava vzor s textem zprava doleva
 *   - jestlize znak na aktualni pozici ve vzorku neni (pozna na zaklade tabulky
 *     charJump), preskoci na index vetsi o delku vzorku
 *   - jestlize znak na akutalni pozici ve vzorku je, porovnava nasledujici znaky
 *     zprava doleva
 *   - pro zabezpeceni rozeznani opakujicich se posloupnosti znaku ve vzoru slouzi
 *     tabulka matchJump
 */
 
/*
 * bigger
 * - pomocna funkce
 * - vraci vetsi ze dvou integeru
 * - pokud jsou hodnoty stejne, vraci prvni z nich
 */
int bigger(int a, int b);

/*
 * compute jumps
 * - pomocna funkce
 * - stanoveni hodnot pole charJump, ktere urcuje posuv vzorku
 */
void computeJumps(int charJump[], char *pattern, int pLenght);

/*
 * compute match jump
 * - pomocna funkce
 * - funkce pro posunuti v ramci vzorku
 * - (situace, kdy se nalezeny podretezec vyskytuje ve vzorku dvakrat)
 */
void computeMatchJump(int matchJump[], char *pattern, int pLenght);

/*
 * findSubString
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho vzorku (cislovano od 1)
 * - jestlize neni vzorek nalezen, vraci -1
 */
int findSubString (char *pattern, char *text);

#endif
