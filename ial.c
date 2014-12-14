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

/**
 * Zjisteni posuvu vzorku na zaklade posledniho vyskytu pismene - do pole jumpArray[]
 * ulozi index posledniho vyskytu pismene
 *
 * @param jumpArray ukazatel na pole, kam se ulozi vypoctena hodnota posunu
 * @param patCont ukazatel na obsah hledaneho retezce
 * @param patLen delka hledaneho retezce
 */
static void charJump(int jumpArray[], char *patCont, int patLen) {
    int i;

       for (i = 0; i < SIZE; i++) {
           jumpArray[i] = patLen;
       }
       for (i = 0; i < patLen - 1; i++) {
           jumpArray[(int) patCont[i]] = patLen - i - 1;
       }
}

/**
 * Vypocteni posuvu vzorku, kdy bereme v uvahu i opakujici se podretezce v patternu
 *
 * @param matchJumpArray ukazatel na pole, kam se ulozi vypoctena hodnota posunu
 * @param patCont ukazatel na obsah hledaneho retezce
 * @param patLen delka hledaneho retezce
 */
static void matchJump(int matchJumpArray[], char *patCont, int patLen) {
    int patPos,patPos2;
    int backupArray[patLen+1];

    //inicializujeme pole
    for (patPos = 0; patPos < patLen+1; patPos++) {
        backupArray[patPos] = 0;
        matchJumpArray[patPos] = 0;
    }

    patPos = patLen;
    patPos2 = patLen+1;

    backupArray[patPos] = patPos2;

    //hledame opakovane podretezce v patternu
    while (patPos > 0) {
        while ((patPos2 <= patLen) && (patCont[patPos-1] != patCont[patPos2-1])) {
            if (matchJumpArray[patPos2] == 0) matchJumpArray[patPos2] = patPos2-patPos;
            patPos2 = backupArray[patPos2];
        }
        patPos--;
        patPos2--;
        backupArray[patPos] = patPos2;
    }

    patPos2 = backupArray[0];

    for (patPos = 0; patPos <= patLen; patPos++) {
        if (matchJumpArray[patPos] == 0) matchJumpArray[patPos] = patPos2;
        if (patPos == patPos2) patPos2 = backupArray[patPos2];
    }
}

/**
 * Vyhleda vzorek v retezci pomoci tzv. Boyer-Moorova vyhledavaciho algoritmu.
 *
 * @param  source   vstupni retezec, ve kterem se bude vyhledavat
 * @param  pattern  vyhledavany vzorek
 * @return          vraci pozici prvniho prvku prvniho nalezeneho vzorku ve
 *                  vstupnim retezci nebo -1 pokud dany vzorek nenalezne
 */
int boyerMoore (char *pattern, char *source)
{
    int result = -1;
    int jumpArray[SIZE], matchJumpArray[pattern->length+1];

    if (lenghtString(pattern) == 0) {  //pokud je pattern prazdny retezec
        return 0;
    }

    //ulozime si delku a ukazatel na obsah source do promennych
    int srcLen = strlen(source);
    char *srcCont = valueString(source); // toto predelat, navod dole

    //totez pro pattern
    int patLen = strlen(pattern);
    char *patCont = valueString(pattern); // toto predelat, navod dole

    //inicializujeme si pole pro posuv patternu
    charJump(jumpArray, patCont, patLen);
    matchJump(matchJumpArray, patCont, patLen);

    if ((srcLen < patLen) || (patLen <= 0) || (srcLen <= 0)) return result;
    else {
        int srcPos = 0;  //source position
        int patPos = patLen-1;  //pattern position

        while (srcPos <= srcLen - patLen) {  //dokud budeme v source, tak...
            while (srcCont[srcPos+patPos] == patCont[patPos]) {  //hledame odzadu stejne znaky
                if (patPos == 0) return srcPos;
                patPos--;
            }
            //posuneme pattern vuci textu dale podle vysledku - vybereme vetsi mozny posun
            srcPos += ((jumpArray[(int) srcCont[srcPos+patLen-1]] > matchJumpArray[patPos+1]) ? jumpArray[(int) srcCont[srcPos+patLen-1]] : matchJumpArray[patPos+1]);

            patPos = patLen-1;  //presuneme se na konec pattern
        }
    }

    return result;
}

/**
 * Vraci vlastni hodnotu stringu, abstrakce nad strukturou string
 *
 * @param string ukazatel na string
 * @return ukazatel na retezec charu

char *valueString(TString *string) {
    if (string == NULL || string->content == NULL) {
        return "";
    }

    return string->content;
}
 */
