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
void shellSort(int *array, int n);

/*
 * badCharacters
 * - pomocna funkce BM algormitmu
 * - naplni pole badChars 'spatnymi' znaky, tedy takovymi, ktere se nevyskytuji
 *   ve hledanem podretezci
 */
void computeJumps(char *pattern, int patternLenght, int charJump[]);

/*
 * suffixes
 * - pomocna funkce BM algormitmu
 * - vypocte suffixy pro zadany vzor (pattern)
 */
void suffixes(char *pattern, int patternLenght, int *suffix);

/*
 * goodSuffixes
 * - pomocna funkce BM algormitmu
 * - naplni pole spravnymi suffixy
 */
void goodSuffixes(char *pattern, int patternLenght, int goodSuffix[]);

/*
 * findSubString
 * - hlavni ridici funkce celeho Boyer-Mooreova algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce, nebo by alespon mela..
 */
int findSubString(char *pattern, char *text);
