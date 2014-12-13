 /*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "parstack.h"

void sInit (Stack *s)
{
    s->top=0;
}

void sPush (Stack *s, sData item)
{
    if (s-> top == MaxStackSize)
    printf ("Chyba! Preteceni zasobniku\n");
    else
    {
      s->top++;
      s->StackData[s->top] = item  ;
    }
}

sData sPop (Stack *s)
{
    return (s->StackData[s->top--]);
}
sData sTop (Stack *s)
{
    return (s->StackData [s->top]);
}

int sEmpty(Stack *s)
{
    return (s->top == 0);
}

int DataInit(sData *lex)
{
  if (((lex->data = (string *) malloc(sizeof(string))) == NULL) ||
     ((((string *) lex->data)->str = malloc(sizeof(char) * DETAIL_LENGHT)) == NULL ))
    return EXIT_INTERNAL_ERROR;
  ((string *) lex->data)->str[0] = '\0';
  ((string *) lex->data)->length = 0;
  ((string *) lex->data)->alloc = DETAIL_LENGHT;

  if (((lex->data2 = (string *) malloc(sizeof(string))) == NULL) ||
     ((((string *) lex->data2)->str = malloc(sizeof(char) * DETAIL_LENGHT)) == NULL ))
    return EXIT_INTERNAL_ERROR;
  ((string *) lex->data2)->str[0] = '\0';
  ((string *) lex->data2)->length = 0;
  ((string *) lex->data2)->alloc = DETAIL_LENGHT;
  return EXIT_SUCCESS;
}

/*void DataFree(sData *str)
{
  if (str->lexdata.type != l_int && str->lexdata.type != l_real && str->lexdata.type != l_key)
    free(((string *)str->data)->str);
  free(str->lexdata.data);
}
*/
