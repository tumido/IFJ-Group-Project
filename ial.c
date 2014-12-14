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
 * Algoritmus shell sort
 * -----------------------------------------------------------------------------
 * - provadi shellovo razeni 'n' prvku v poli
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
 * Boyer-Mooreuv algoritmus
 * -----------------------------------------------------------------------------
 * - vyhledava prvni nalezeny podretezec v zadanem retezci
 * - vraci pozici podretezce (idexovano od 1) nebo -1 v pripade nenalezeni
 */ 
int bigger(int a, int b)
{
  if (a > b) return a;
  else if (a < b) return b;
  else return a; // jsou stejne, je jedno, co vratim, tak treba a
}

void computeJumps(int charJump[], char *pattern, int pLenght)
{
  for (int i = 0; i < SIZE; i++) charJump[i] = pLenght;
  for (int i = 0; i < pLenght - 1; i++) charJump[(int)pattern[i]] = pLenght - i - 1;
}

void computeMatchJump(int matchJump[], char *pattern, int pLenght)
{
  int pp = pLenght; // pattern possition
  int pp2 = pLenght + 1; 
  int backup[pLenght + 1]; // zalohovaci pole

  for (pp = 0; pp < pLenght + 1; pp++) // inicializace - vynulovani poli
  {
    backup[pp] = 0;
    matchJump[pp] = 0;
  }

  //hledame opakujici-se podretezce v patternu
  while (pp > 0)
  {
    backup[pp] = pp2;
    while ((pp2 <= pLenght) && (pattern[pp - 1] != pattern[pp2 - 1]))
    {
      
      if (matchJump[pp2] == 0) matchJump[pp2] = pp2 - pp;
      pp2 = backup[pp2];
    }

    pp--;
    pp2--;
  }

  pp2 = backup[0];

  for (pp = 0; pp <= pLenght; pp++)
  {
    if (matchJump[pp] == 0) matchJump[pp] = pp2;
    if (pp == pp2) pp2 = backup[pp2];
  }
}

int findSubString (char *pattern, char *text)
{
  int tLenght = strlen(text); // delka textu
  int pLenght = strlen(pattern); //delka patternu
  int charJump[SIZE], matchJump[pLenght + 1];
  int tp = 0; // text position
  int pp = pLenght - 1; // pattern position

  // volani pomocnych funkci
  computeJumps(charJump, pattern, pLenght);
  computeMatchJump(matchJump, pattern, pLenght);

  while (tp <= tLenght - pLenght) // dokud jsme v textu, hledame odzadu shodne znaky
  {
    while (text[tp + pp] == pattern[pp]) // dokud se znaky rovnaji
    {
      if (pp == 0) return tp + 1; // nalezeno, +1 jest korekce indexovani
      pp--; // jeste jsme neprohledali vse, pokracujeme
    }

    tp += bigger(charJump[(int) text[tp+pLenght - 1]], matchJump[pp + 1]); // vybereme vetsi posun
    pp = pLenght - 1; // presun na vychozi pozici v patternu
  }
  return -1; // nenalezeno
}

// !! syntaxe by mi mela overit, ze vzor && text nejsou prazdne retezce !!
