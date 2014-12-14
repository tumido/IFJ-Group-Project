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
//#include "io.h"
#define SIZE 255

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
 * findSubString
 * -----------------------------------------------------------------------------
 * - hlavni ridici funkce celeho algoritmu, spousti pomocne funkce
 * - vraci index prvniho vyskytu zadaneho podretezce
 * - v pripade nenalezeni, vraci -1
 */
int findSubString(char *pattern, char *text)
{
  int patternLenght = strlen(pattern);
  int textLenght = strlen(text);

 // computeJumps(pattern, patternLenght, textLenght, charJump);

  int i = 0;
  int a = patternLenght;
  int b = patternLenght;
  int lenghtFind = 0;
  int f = 0;
  int g = 0;
  int count = 0;
  int shift = 0;

  while(i < textLenght)
  { 

/*
  - skocit o delku patternu
  - zkontrolovat ostatni znaky patternu
    - shoda, posunu o pozici od prvni shody
      - zkontroluji ostatni znaky s Ackovymi vsemy
         - shoda - koncim a vracim index
         - neshoda - skacu o tolik, kolikaty byl shodny prvek
    - neshoda, posunu se o pattern od minula
 */

    // uz na zacatku jsem skocil o pattern (inicializaci) - rovnou porovnam prvky
    for(f = 0; f < patternLenght; f++)
    {
      printf("\n---- porovnavam v patternu\n");
      printf("text: %c\n", text[a-1]);
      printf("vzor: %c\n", pattern[b-1-f]);
       
      if(text[a-1] == pattern[b-1-f])
      {
        printf("shoda\n");
        // shoda f-teho prvku vzorku, musim posunout o f a proverit ostatni
        // bezva, ted musim projit ty ostatni, a pokud si nejsou rovny, tak shiftuji o delku patternu
        shift = a; // ulozim acko, abych vedel, odkud mam pokracovat
        // projdu ostatni znaky a ukladam si pocet o neuspesnych nalezenych
          // lepe receno pri prvnim spatnem rovnou koncim a posouvam o shift + patternlenght-poradi spatneho
        // no a pokud je vsechno dobre, vracim
        
        for(g = 0; g < patternLenght; g++)
        {
          printf("-- porovnavam zbyvajici pismena posunuteho patternu\n");
          printf("text: %c\n", text[a-1]);
          printf("vzor: %c\n", pattern[b-1]);

          if (text[a-1] == pattern[b-1]) // nasel jsem shodu, kontroluji prechozi znaky
          {
            printf("............... nalezena shoda ....................\n");

            a--;
            b--;
            lenghtFind++; 

            if(lenghtFind == patternLenght) return a + 1; // nalezeno
          }
          else
          {
            printf("nenalezeno, shiftuju\n");
            a = shift + patternLenght - g - 1;
          }
        } 
      }
      else 
      {
        printf("neshoda\n");
        count++; // pocitam si pocet neshod v prvnim porovnavani

        if(count == patternLenght)
        {
          a = a + patternLenght;
          printf(" -> posun o pattern\n");
          count = 0; // a pocitame od znovu..
        } 
      }
    }
    i++; 
  }
  return -1; // nenalezeno
}
