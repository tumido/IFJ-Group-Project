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
 * - provadi shellovo razeni n prvku v poli
 * - implemetace podle slajdu prof. Hoznika
 * - zatim neotestovano
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
    for (i = step; n - 1; i--) // i-- jsem si domyslel, ten zapis ve slajdech je divny..
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
 *   - jestlize znak na akutali pozici ve vzorku je, porovnava nasledujici znaky
 *     zprava doleva
 *   - pro zabezpeceni rozeznani opakujicich se posloupnosti znaku ve vzoru slouzi
 *     tabulka 'dobrych' suffixu
 */

/*
 * badCharacters
 * - naplni pole badChars 'spatnymi' znaky
 */
void badCharacters(char *pattern, int patternLenght, int badChars[])
{
  int i;

  for (i = 0; i < SIZE; ++i) badChars[i] = patternLenght;
  for (i = 0; i < patternLenght - 1; ++i) badChars[pattern[i]] = patternLenght - i - 1;
}

/*
 * suffixes
 * - vypocte suffixy pro zadany vzor (pattern)
 */
void suffixes(char *pattern, int patternLenght, int *suffix)
{
  int f, g, i;

  suffix[patternLenght - 1] = patternLenght;
  g = patternLenght - 1;

  for (i = patternLenght - 2; i >= 0; --i)
  {
    if ((i > g) && (suffix[i + patternLenght - 1 - f]) < (i - g))
      suffix[i] = suffix[i + patternLenght - 1 - f];
    else
    {
      if (i < g) g = i;

      f = i;

      while ((g >= 0) && (pattern[g] == pattern[g + patternLenght - 1 - f])) --g;

      suffix[i] = f - g;
    }
  }
}

/*
 * goodSuffixes
 * - naplni pole spravnymi suffixy
 */
void goodSuffixes(char *pattern, int patternLenght, int goodSuffix[])
{
  int i = 0;
  int j = 0;
  int suffix[SIZE];

  suffixes(pattern, patternLenght, suffix);

  for (i = 0; i < patternLenght; ++i) goodSuffix[i] = patternLenght;

  for (i = patternLenght - 1; i >= 0; --i)
  {
    if (suffix[i] == i + 1)
    {
      for (; j < patternLenght - 1 - i; ++j)
      {
        if (goodSuffix[j] == patternLenght) goodSuffix[j] = patternLenght - 1 - i;
      }
    }
  }

  for (i = 0; i <= patternLenght - 2; ++i)
    goodSuffix[patternLenght - 1 - suffix[i]] = patternLenght - 1 - i;
}

/*
 * findSubString
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce, nebo by alespon mela..
 */
int findSubString(char *pattern, char *text)
{
  int rowLenght = strlen(text);
  int patternLenght = strlen(pattern);
  int i, j, count, goodSuffix[SIZE], badChars[SIZE];

  // vytvoreni tabulek
  goodSuffixes(pattern, patternLenght, goodSuffix);
  badCharacters(pattern, patternLenght, badChars);

  // hledani
  j = count = 0;
  while (j <= rowLenght - patternLenght)
  {
    for (i = patternLenght - 1; i >= 0 && pattern[i] == text[i + j]; --i);
    if (i < 0)
    {
      count++;
      j += goodSuffix[0];
    }
    else
    {
      if (goodSuffix[i] < (badChars[text[i + j]] - patternLenght + 1 + i))
        j += (badChars[text[i + j]] - patternLenght + 1 + i);
      else j += goodSuffix[i];
    }
  }
  return count + 1; // nutna korekce, pozice je pocitana od 1
  // jeste nutno vyresit vraceni 0 pri nenalezeni retezce, ale napret otestovat a ono se na to prijde..
}
