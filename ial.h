/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

// maximalni delka radky zpracovavaneho retezce
#define SIZE 255 

/*
 * Funkce vykonavajici radici algoritmus shell sort
 */
void shellSort(char *array, int n);

/*
 * compute jumps
 * - stanoveni hodnot pole charJump, ktere urcuji posuv vzorku
 */
void computeJumps(char *pattern, int patternLenght, int charJump[]);

/*
 * compute match jump
 * - funkce pro posunuti v ramci vzorku 
 * - (situace, kdy se nalezeny podretezec vyskytuje ve vzorku dvakrat)
 */
void computeMatchJump(char *pattern, int patternLenght, int matchJump[]);

/*
 * findSubString
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce, nebo by alespon mela..
 */
int findSubString(char *pattern, char *text,  int charJump[], int matchJump[]);

/*
 * min
 * - funkce vrati mensi ze dvou cisel
 */
int min(int a, int b);

/*
 * max
 * - funkce vrati mensi ze dvou cisel
 */
int max(int a, int b);
