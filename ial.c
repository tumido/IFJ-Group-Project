/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "ial.h"
#include "io.h"

/*
 * Funkce shellSort
 * -----------------------------------------------------------------------------
 * - provadi shellovo razeni 'n' prvku v poli
 * - implemetace podle slajdu prof. Hoznika
 * - zatim neotestovano
 * - strana 162
 * 
 * - serazene znaky budu rovnou vypisovat na stdout, nebo je nekam ulozim, ha?
 */ 
void shellSort(char *array, int n)
{
  int step, i, j;
  char tmp;
  step = n / 2; // prvni krok je polovina delky pole
  
  while (step > 0) // cykli, pokud je krok vetsi/roven 1
  {
    for (i = step; i < n - 1; i++)
    {
      j = i - step + 1;

      while ((j >= 1) && (array[j] > array[j + step])) // bubble pruchod
      {
        tmp = array[j];
        array[j] = array[j + step];
        array[j + step] = tmp;
        j = j - step; // snizeni indexu o krok
      }
    }
    step = step / 2; // puleni kroku
  }
}

/*
 * Funkce findSubString
 * -----------------------------------------------------------------------------
 * - vyhledava podretezec zadane delky 'n' v retezci 's'
 * - implementovano pomoci boyer-mooreova algoritmu:
 *   - prochazi text zleva doprava a porovnava vzor s textem zprava doleva
 *   - jestlize znak na aktualni pozici ve vzorku neni (pozna na zaklade tabulky
 *     spatnych znaku), preskoci na index vetsi o delku vzorku
 *   - jestlize znak na akutalni pozici ve vzorku je, porovnava nasledujici znaky
 *     zprava doleva
 *   - pro zabezpeceni rozeznani opakujicich se posloupnosti znaku ve vzoru slouzi
 *     'compute march jumps'
 */

int charJump[];

/*
 * compute jumps
 * - stanoveni hodnot pole charJump, ktere urcuji posuv vzorku
 */
void computeJumps(char *pattern, int patternLenght, int charJump[])
{
  int i;

  for (i = 0; i < SIZE; ++i) charJump[i] = patternLenght;
  for (i = 0; i < patternLenght - 1; ++i) charJump[pattern[i]] = patternLenght - 1 - i;
}

/*
 * compute match jump
 * - funkce pro posunuti v ramci vzorku
 * - (situace, kdy se nalezeny podretezec vyskytuje ve vzorku dvakrat)
 */
void computeMatchJump(char *pattern, int patternLenght, int matchJump[])
{
  int k, q, qq;
  int m = patternLenght;
  int backup[];

  for(k = 0; k < m; k++)
  {
    matchJump[k] = 2 * m - k;
    k = m;
    q = m + 1;

    while (k > 0)
    {
      backup[k] = q;

      while ((q < m) && (pattern[k] != pattern[q]))
      {
        matchJump[q] = min(matchJump[q], m - k);
        q = backup[q];
      }

      k--;
      q--;
    }

    for (k = 0; k < q; k++) matchJump[k] = min(matchJump[k], m + q - k);

    qq = backup[q];

    while (q < m)
    {
      while (q < qq)
      {
        matchJump[q] = min(matchJump[q], qq - q + m); // funkce vraci mensi ze dvou
        q = q + 1;
      }
      qq = backup[qq];
    }
  }
}

/*
 * findSubString
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce, nebo by alespon mela..
 */
int findSubString(char *pattern, char *text,  int charJump[], int matchJump[])
{
  int j, k; // j - index to textu, k - index do vzorku
  int patternLenght = strlen(pattern);
  
  j = patternLenght;
  k = patternLenght;

  while ((j < SIZE) && (k > 0))
  {
    if(text[j] == pattern[k])
    {
      j--;
      k--;
    }
    else
    {
      j = j + max(charJump[text[j]], matchJump[k]); // vybere vetsi = vyhodnejsi posun
      k = patternLenght;
    }
  }

  if (k == 0) return (j + 1); // nasla se shoda
  else return (SIZE + 1); // nenasla se shoda
}

/*
 * min
 * - funkce vrati mensi ze dvou cisel
 * - ano, je to hloupy nazev
 */
int min(int a, int b)
{
  if(a < b) return a;
  else if (a > b) return b;
  else return a; // pripad kdy a = b, je tedy uplne jedno, co vratime, tak treba 'a'
}

/*
 * max
 * - funkce vrati mensi ze dvou cisel
 * - ano, je to hloupy nazev
 */
int max(int a, int b)
{
  if(a < b) return b;
  else if (a > b) return a;
  else return a; // pripad kdy a = b, je tedy uplne jedno, co vratime, tak treba 'a'
}

/*
 * - oba dva algoritmy jsou treba otestovat, nejlepe nekym povolanym..
 */
