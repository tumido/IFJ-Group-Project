/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */


/*
 * Funkce shellSort
 * -----------------------------------------------------------------------------
 * - provadi shellovo razeni n prvku v poli
 * - implemetace podle slajdu prof. Hozníka
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
        tmp = a[j];
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
 * - implementovano pomoci boyer-mooreova algoritmu
 */ 
findSubtring();
