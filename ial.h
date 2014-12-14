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
// maximalni delka radky zpracovavaneho retezce
#define SIZE 255

/*
 * Funkce vykonavajici radici algoritmus shell sort
 */
void shellSort(char *array, int n);

/*
 * findSubString
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce, nebo by alespon mela..
 */
int findSubString(char *pattern, char *text);

/*
 * smaller
 * - funkce vrati mensi ze dvou cisel typu int
 */
int smaller(int a, int b);

/*
 * bigger
 * - funkce vrati mensi ze dvou cisel typu int
 */
int max(int a, int b);
int bigger(int a, int b);
#endif
