 /*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

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



