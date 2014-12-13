/*
 * =====================================================================
 *          Verze:  1.EXIT_SUCCESS
 *      Vytvoreno:  11/12/2EXIT_SUCCESS14 EXIT_SUCCESS4:21:52 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "parser.h"

void generateInstruction(int instType, key type, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist)
// vlozi novou instrukci do seznamu instrukci

/*
 * KONVENCE INSTRUKCI
 * -----------------------------------------------------------------------------
 * - plati pro aritmeticke instrukce, instrukce porovnani a instrukce vestavenych funkci
 * - specialni instrukce maji specialni konvece uvedene vzdy pred instrukci samotnou
 *
 * - instrukce se dvema operandy:
 *   JMENO_INSTRUCE, TYP, OPERAND1, OPERAND2, VYSLEDEK
 *
 * - instrukce s jednim operandem:
 *   JMENO_INSTRUKCE, TYP, OPERAND1, NULL, VYSLEDEK
 */

{
   tInstr I;
   I.instType = instType;
   I.type = type;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(ilist, I);
   return;
}

int prTable [14][14] =
 //radek - vstupni token,
 //sloupec - znak na zasobniku
{  //        +     -    *     div    >     <    >=    <=     =    <>    (     )    i     $
/* + */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* - */  {Right, Right,Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* * */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* div */{Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* > */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* < */  {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* >= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* <= */ {Left,  Left, Left, Left, Right,Right,Right,Right,Right,Right,Left,Right,Left,Right,},
/* = */  {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
/* <> */ {Left,  Left, Left, Left, Left, Left, Left, Left, Right,Right,Left,Right,Left,Right,},
/* ( */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Straight,Left,Err,},
/* ) */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
/* i */  {Right, Right,Right,Right,Right,Right,Right,Right,Right,Right,Err,Right,Err,Right,},
/* $ */  {Left,  Left, Left, Left, Left, Left, Left, Left, Left, Left, Left,Err,Left,EE,},

};

// vraci index do tabulky
int retIndex (lexType typ)
{
  int j;
  switch (typ)
  {
    case (l_add): j=0; break;
    case (l_sub): j=1; break;
    case (l_mul): j=2; break;
    case (l_div): j=3; break;
    case (l_greater): j=4; break;
    case (l_less): j=5; break;
    case (l_gequal): j=6; break;
    case (l_lequal): j=7; break;
    case (l_equal): j=8; break;
    case (l_not): j=9; break;
    case (l_lparenth): j=10; break;
    case (l_rparenth): j=11; break;
    case (l_int):
    case (l_bool):
    case (l_real):
    case (l_str):
    case (l_id): j=12; break;
    case (l_eof): j=13; break;
    default: j=13;
  }

 return j;
}

/*   Syntakticka analyza
 * =====================================================================
 * - "zhora dolu", vyuziva rekurzivni sestup
 */

/*   Vyhodnoceni vyrazu
 * ---------------------------------------------------------------------
 * - bude namapovane na Juruv kod
 */
int evalExpression(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex, void * retVal, key retType)
{
  printDebug("Spoustim analyzu vyrazu\n");
  int result = EXIT_SUCCESS;
  int Ptoken=0;
  Stack s;
  sInit (&s);
    token NLex;
    if ((result = tokenInit(&NLex)) != EXIT_SUCCESS) return result;
  sData itemAct;
  sData itemTop;
  sData itemC;
  sData itemD;
  if (DataInit(&itemAct) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemTop) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemC) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  if (DataInit(&itemD) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
  itemAct.TypTok=l_eof;
  sPush (&s,itemAct);
  printDebug("Inicializuji zasobnik a vkladam zarazku\n");
  itemTop=sTop(&s);
  printDebug("Kontrola prvniho lexemu\n");
  if (nextLex==NULL)
   {
    printDebug("Byl obdrzen pouze jeden lexem\n");
    Ptoken=1;
   }
  else
    {
     printDebug("Obdrzel jsem dva lexemy\n");
     NLex.type=nextLex->type;
    }

  if (lex->type==l_id)
  {
    struct node *nd;
    printDebug("Prvni lexem je ID, jdu hledat ve strome\n");
    if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
    {
          return EXIT_SEMANTIC_ERROR;
    }
    // ulozim si ze stromu hodnoty
    if  (nd->type == k_int)
    {
      itemAct.TypVal=l_int;
      itemAct.vInt=*(long int*)nd->data;
      printDebug("Prvni lexem je int s hodnotou %d\n",itemAct.vInt);
    }
    else if  (nd->type == k_real)
    {
      itemAct.TypVal=l_real;
      itemAct.vDouble=*(double*)nd->data;
            printDebug("Prvni lexem je real s hodnotou %e\n",itemAct.vDouble);
    }
    else if  (nd->type == k_string)
    {
      printDebug("Prvni lexem je retezec\n");
      itemAct.TypVal=l_str;
      // string musim dodelat
    }
    else if  (nd->type == k_bool)
    {
      itemAct.TypVal=l_bool;
      itemAct.vBool=*(bool*)nd->data;
      printDebug("Prvni lexem je bool s hodnotou %d\n",itemAct.vBool);
    }
    else { return EXIT_SYNTAX_ERROR;}
  // __SymbolTableDispose(&nd);
   itemAct.TypTok = l_id;
  }
  else if (lex->type==l_key)
  {
      printDebug("je to klicove slovo, zkontroluji jestli je to true nebo false\n");
      if (*(key *)lex->data == k_true)
       {
          itemAct.TypVal=l_bool;
          itemAct.vBool=1;
          printDebug("Je to true\n");
       }
      else if(*(key *)lex->data == k_false)
      {
          printDebug("Je to false\n");
          itemAct.TypVal=l_bool;
          itemAct.vBool=0;
       }
      else return EXIT_SYNTAX_ERROR;
      itemAct.TypTok=l_id;
  }
   // jestli je token int,real,str ulozim si hodnotu a typ
  else if(lex->type == l_int || lex->type == l_real || lex->type == l_str)
  {
    if  (lex->type == l_int)
    {
      itemAct.TypVal=lex->type;
      itemAct.vInt=*(long int*)lex->data;
      printDebug("Prvni lexem je int s hodnotou %d\n",itemAct.vInt);
    }
    else if  (lex->type == l_real)
    {
     itemAct.TypVal=lex->type;
     itemAct.vDouble=*(double*)lex->data;
     printDebug("Prvni lexem je real s hodnotou %d\n",itemAct.vDouble);
    }
    else if  (lex->type == l_str)
    {
      printDebug("Prvni lexem je retezec\n");
      itemAct.TypVal=lex->type;
      // string musim dodelat
      //+++++++++++++++++++++++
    }
    itemAct.TypTok = l_id;
  }
  else
  {// pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
    printDebug("Prvni lexem neni promenna ani cislo\n");
    itemAct.TypTok=lex->type;
  }
  do
  {
    switch (prTable[retIndex(itemTop.TypTok)][retIndex(itemAct.TypTok)])
    {
       case Err:
           return EXIT_SYNTAX_ERROR;

       case Straight:
         sPush(&s,itemAct);
         itemTop=sTop(&s);
         if (Ptoken==1)
         {if ((result = fillToken(in,lex)) != EXIT_SUCCESS) return result;}
         else{lex->type=NLex.type;Ptoken=1;}

         if (lex->type==l_id)
         {
         struct node *nd;
         printDebug("Dalsi lexem je ID, jdu hledat ve strome\n");
         if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
         {
          return EXIT_SEMANTIC_ERROR;
         }
         // ulozim si ze stromu hodnoty
         if  (nd->type == k_int)
         {
          itemAct.TypVal=l_int;
          itemAct.vInt=*(long int*)nd->data;
          printDebug("Dalsi lexem je int s hodnotou %d\n",itemAct.vInt);
         }
         else if  (nd->type == k_real)
         {
          itemAct.TypVal=l_real;
          itemAct.vDouble=*(double*)nd->data;
          printDebug("Dalsi lexem je real s hodnotou %e\n",itemAct.vDouble);
          }
         else if  (nd->type == k_string)
         {
          printDebug("Dalsi lexem je retezec\n");
          itemAct.TypVal=l_str;
          // string musim dodelat
         }
         else if  (nd->type == k_bool)
         {
          itemAct.TypVal=l_bool;
          itemAct.vBool=*(bool*)nd->data;
          printDebug("Dalsi lexem je bool s hodnotou %d\n",itemAct.vBool);
         }
         else { return EXIT_SYNTAX_ERROR;}
         // __SymbolTableDispose(&nd);
         itemAct.TypTok = l_id;
         }
         // jestli je token int,real,str ulozim si hodnotu a typ
         else if(lex->type == l_int || lex->type == l_real || lex->type == l_str)
         {
         if  (lex->type == l_int)
         {
         itemAct.TypVal=lex->type;
         itemAct.vInt=*(long int*)lex->data;
         printDebug("Dalsi lexem je int s hodnotou %d\n",itemAct.vInt);
         }
         else if  (lex->type == l_real)
         {
         itemAct.TypVal=lex->type;
         itemAct.vDouble=*(double*)lex->data;
         printDebug("Dalsi lexem je real s hodnotou %d\n",itemAct.vDouble);
         }
         else if  (lex->type == l_str)
         {
         printDebug("Dalsi lexem je retezec\n");
         itemAct.TypVal=lex->type;
         // string musim dodelat
         //+++++++++++++++++++++++
         }
         itemAct.TypTok = l_id;
         }
         else
         {// pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
         printDebug("Dalsi lexem neni promenna ani cislo\n");
         itemAct.TypTok=lex->type;
         }
         break;

       case Left:
        printDebug("Uplatnuji Leve pravidlo\n");
        itemTop=sTop(&s);  // co je nahore?
        itemC.TypTok=l_left; // nastavim si <
        if (itemTop.TypTok==l_E)  // jestli je nahore E
        { // zamenime E za <E
         itemTop=sPop(&s);
         sPush (&s,itemC);
         sPush (&s,itemTop);
         printDebug("Zamena E za <E\n");
        }
        else // na vrcholu neni E, dame tam <
        {
         printDebug("Pridavam < do zasobniku\n");
         sPush (&s,itemC);
        }
        // nakonec vlozime token
        printDebug("Vkladam token do zasobniku\n");
        sPush (&s,itemAct);
        if (Ptoken==1)
        {if ((result = fillToken(in,lex)) != EXIT_SUCCESS) return result; }
        else{
            Ptoken=1;
            lex->type=NLex.type;
            }
        itemTop=sTop(&s);
         if (lex->type==l_id)
         {
         struct node *nd;
         printDebug("Dalsi lexem je ID, jdu hledat ve strome\n");
         if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
         {
          return EXIT_SEMANTIC_ERROR;
         }
         // ulozim si ze stromu hodnoty
         if  (nd->type == k_int)
         {
          itemAct.TypVal=l_int;
          itemAct.vInt=*(long int*)nd->data;
          printDebug("Dalsi lexem je int s hodnotou %d\n",itemAct.vInt);
         }
         else if  (nd->type == k_real)
         {
          itemAct.TypVal=l_real;
          itemAct.vDouble=*(double*)nd->data;
          printDebug("Dalsi lexem je real s hodnotou %e\n",itemAct.vDouble);
          }
         else if  (nd->type == k_string)
         {
          printDebug("Dalsi lexem je retezec\n");
          itemAct.TypVal=l_str;
          // string musim dodelat
         }
         else if  (nd->type == k_bool)
         {
          itemAct.TypVal=l_bool;
          itemAct.vBool=*(bool*)nd->data;
          printDebug("Dalsi lexem je bool s hodnotou %d\n",itemAct.vBool);
         }
         else { return EXIT_SYNTAX_ERROR;}
         // __SymbolTableDispose(&nd);
         itemAct.TypTok = l_id;
         }
         // jestli je token int,real,str ulozim si hodnotu a typ
         else if(lex->type == l_int || lex->type == l_real || lex->type == l_str)
         {
         if  (lex->type == l_int)
         {
         itemAct.TypVal=lex->type;
         itemAct.vInt=*(long int*)lex->data;
         printDebug("Dalsi lexem je int s hodnotou %d\n",itemAct.vInt);
         }
         else if  (lex->type == l_real)
         {
         itemAct.TypVal=lex->type;
         itemAct.vDouble=*(double*)lex->data;
         printDebug("Dalsi lexem je real s hodnotou %d\n",itemAct.vDouble);
         }
         else if  (lex->type == l_str)
         {
         printDebug("Dalsi lexem je retezec\n");
         itemAct.TypVal=lex->type;
         // string musim dodelat
         //+++++++++++++++++++++++
         }
         itemAct.TypTok = l_id;
         }
         else
         {// pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
         printDebug("Dalsi lexem neni promenna ani cislo\n");
         itemAct.TypTok=lex->type;
         }
      break;

     case Right:
        printDebug("Uplatnuji prave pravidlo\n");
        itemTop=sTop(&s); // kouknu se co je nahore
        switch (itemTop.TypTok)
        {   // nahore je i, redukujeme na E
          case l_id:
            itemD=sPop(&s); // popneme to
            sPop(&s);// popneme pryc <
            itemD.TypTok=l_E ; // zmenime i na E, data zustanou
            itemTop=sTop(&s);  // priradime na top to co je pred E
            sPush (&s,itemD);   // pushneme E
            printDebug("Redukuji <i na E\n");
            if (prTable[retIndex(itemTop.TypTok)][retIndex(itemAct.TypTok)]== EE)
            {
                printDebug("Je to prirazeni\n");
                if ((itemD.TypVal==l_int) && (retType== k_int) )
                {*(((long int*)itemD.data2))=itemD.vInt; generateInstruction(I_ASSIGN,k_int, itemD.data2, NULL,retVal, ilist);}
                else if ((itemD.TypVal==l_real) && (retType== k_real) )
                {*(((double*)itemD.data2))=itemD.vDouble; generateInstruction(I_ASSIGN,k_real, itemD.data2, NULL,retVal, ilist);}
                else if ((itemD.TypVal==l_str) && (retType== k_string) )
                printDebug("STRING MUSIM DODELAT\n");
                else if ((itemD.TypVal==l_bool) && (retType== k_bool) )
                {*(((bool*)itemD.data2))=itemD.vBool; generateInstruction(I_ASSIGN,k_bool, itemD.data2, NULL,retVal, ilist);}
                else return EXIT_TYPE_ERROR;
            }
            break;
          case l_rparenth:
            printDebug("Vrchol je ), vyraz byl v zavorce\n");
            sPop(&s); // odstranim )
            itemC=sPop(&s); // odstranim E
            sPop(&s); // odstranim (
            sPop (&s); // odstranim <
            itemTop=sTop(&s), // nahore je to co je pred E
            sPush(&s,itemC); // dam tam zpatky E
            break;
          // na vrchu je E
          case l_E:
             printDebug("Vrchol je E, kontroluji operaci\n");
            itemTop=sPop(&s); // popneme E
            itemD=sPop(&s); // popneme operaci
            itemC=sPop(&s); // popneme druhy E
            sPop(&s); // tady by melo byt <
            switch (itemD.TypTok)
            {
             case l_add:
                 printDebug("Operace je scitani\n");
                 if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                      *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int+int\n");
                     if (retType!= k_int) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_ADD,k_int, itemC.data2, itemTop.data,retVal, ilist);
                     printDebug("scitam %d a %d\n",*(long int*)itemC.data2,*(long int*)itemTop.data);
                     itemC.vInt =itemTop.vInt + itemC.vInt;
                     itemC.TypVal=l_int;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_real) )
                 {
                     printDebug("int+real\n");
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_ADD,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vInt + itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     printDebug("real+int\n");
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_ADD,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble + itemC.vInt;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     printDebug("real+real\n");
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_ADD,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble + itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                else if ((itemTop.TypVal==l_str) && (itemC.TypVal==l_str) )
                {
                     printDebug("string+string\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     // string dodelat
                     //+++++++++++++++++++
                     itemC.TypVal=l_str;
                }
                else return EXIT_SEMANTIC_ERROR;
                itemTop=sTop(&s);
                sPush(&s,itemC);
                break;
              case l_mul:
                 printDebug("Operace je nasobeni\n");
                 if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                      *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int*int\n");
                     if (retType!= k_int) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_MUL,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     printDebug("nasobim %d a %d\n",itemTop.vInt,itemC.vInt);
                     itemC.vInt =itemTop.vInt * itemC.vInt;
                     itemC.TypVal=l_int;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int*real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_MUL,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vInt * itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real*int\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_MUL,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble * itemC.vInt;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real*real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_MUL,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble * itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
             case l_div:
                 printDebug("Operace je deleni\n");
                 if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int/int\n");
                     if (retType!= k_int) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_DIV,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     printDebug("delim %d a %d\n",itemTop.vInt,itemC.vInt);
                     itemC.vDouble =itemC.vInt / itemTop.vInt;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("real/int\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_DIV,k_real,  itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemC.vInt / itemTop.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("int/real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_DIV,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemC.vDouble / itemTop.vInt;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real/real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_DIV,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble / itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
             case l_sub:
                 printDebug("Operace je odcitani\n");
                 if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int-int\n");
                     if (retType!= k_int) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_SUB,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     printDebug("Odcitam %d a %d\n",itemTop.vInt,itemC.vInt);
                     itemC.vInt =itemC.vInt - itemTop.vInt;
                     itemC.TypVal=l_int;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("real-int\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_SUB,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemC.vInt - itemTop.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("int-real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_SUB,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemC.vDouble - itemTop.vInt;
                     itemC.TypVal=l_real;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_int) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real-real\n");
                     if (retType!= k_real) return  EXIT_TYPE_ERROR;
                     generateInstruction(I_SUB,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     itemC.vDouble =itemTop.vDouble - itemC.vDouble;
                     itemC.TypVal=l_real;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
            case l_less:
                printDebug("Operace je mensi\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                      *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int<int\n");
                     generateInstruction(I_LESS,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt<itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                      *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real<real\n");
                     generateInstruction(I_LESS,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble<itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str<str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool<bool\n");
                     generateInstruction(I_LESS,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool<itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                break;
            case l_greater:
                printDebug("Operace je vetsi\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int>int\n");
                     generateInstruction(I_GREATER,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt>itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real>real\n");
                     generateInstruction(I_GREATER,k_real, itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble>itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str>str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool>bool\n");
                     generateInstruction(I_GREATER,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool>itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                break;
            case l_gequal:
                printDebug("Operace je vetsirovno\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int>=int\n");
                     generateInstruction(I_GREATER_EQUAL,k_int, itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt>=itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                    itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real>=real\n");
                     generateInstruction(I_GREATER_EQUAL,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble>=itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str>=str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool>=bool\n");
                     generateInstruction(I_GREATER_EQUAL,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool>=itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
               break;
            case l_lequal:
                printDebug("Operace je mensirovno\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                    *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int<=int\n");
                     generateInstruction(I_LESS_EQUAL,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt<=itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real=<real\n");
                     generateInstruction(I_LESS_EQUAL,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble<=itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str<str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool<bool\n");
                     generateInstruction(I_LESS_EQUAL,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool<=itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
            case l_equal:
                printDebug("Operace je rovna se\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int==int\n");
                     generateInstruction(I_EQUAL,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt==itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real==real\n");
                     generateInstruction(I_EQUAL,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble==itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str==str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool==bool\n");
                     generateInstruction(I_EQUAL,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool==itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
            case l_not:
                printDebug("Operace je nerovna se\n");
                if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     *(((long int*)itemC.data2))=itemC.vInt;
                       *(((long int*)itemTop.data))=itemTop.vInt;
                     printDebug("int<>int\n");
                     generateInstruction(I_NOT_EQUAL,k_int,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vInt!=itemD.vInt) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_real) && (itemC.TypVal==l_real) )
                 {
                     *(((double*)itemC.data2))=itemC.vDouble;
                       *(((double*)itemTop.data))=itemTop.vDouble;
                     printDebug("real<>real\n");
                     generateInstruction(I_NOT_EQUAL,k_real,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vDouble!=itemD.vDouble) itemC.vBool=true;
                     else itemC.vBool= false;
                     itemC.TypVal=l_bool;
                 }
                 else if ((itemTop.TypVal==l_int) && (itemC.TypVal==l_int) )
                 {
                     printDebug("str<>str\n");
                     // musim dodelat string
                 }
                 else if ((itemTop.TypVal==l_bool) && (itemC.TypVal==l_bool) )
                 {
                     *(((bool*)itemC.data2))=itemC.vBool;
                       *(((bool*)itemTop.data))=itemTop.vBool;
                     printDebug("bool<>bool\n");
                     generateInstruction(I_NOT_EQUAL,k_bool,itemC.data2, itemTop.data,retVal, ilist);
                     if (itemC.vBool!=itemD.vBool) itemC.vBool=true;
                     else itemC.vBool= false;
                 }
                 else return EXIT_SEMANTIC_ERROR;
                 itemTop=sTop(&s);
                 sPush(&s,itemC);
                 break;
             default: break;

         }// operace
          default: break;
        }// itemTop.TypTok
    }// switch prTable

  }while (prTable[retIndex(itemTop.TypTok)][retIndex(itemAct.TypTok)]!= EE);
    tokenFree(&NLex);
    itemTop=sTop(&s);
    if (itemTop.TypVal==l_int)
    {printDebug ("Vysledek je int = %d\n",itemTop.vInt); *(long int*)retVal=itemTop.vInt;}
    if (itemTop.TypVal==l_real)
    {printDebug ("Vysledek je real = %e\n",itemTop.vDouble); *(double*)retVal=itemTop.vDouble;}
    if (itemTop.TypVal==l_bool)
    {printDebug ("Vysledek je bool = %d\n",itemTop.vBool); *(bool*)retVal=itemTop.vBool;}



        printDebug("Vracim se z analyzy\n");
  return EXIT_SUCCESS;
}

/*   Blok deklarace promennych
 * ---------------------------------------------------------------------
 * - zpracovani bloku promenych
 * - pokud program nezacina klicovym slovem "var" je tato faze
 *   preskocena
 *
 *   <declareList> -> "var" <DeclareListContent>
 */
int declareList(struct input * in, btree * table, token * lex)
{
  printDebug("Deklarace promenych\n");
  if ((lex->type != l_key) || (*((key *)lex->data) != k_var))
    return EXIT_SUCCESS; // nezacinam "var" -> konec

  int result;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS) return result;
  if (lex->type != l_id)
  {
    if (lex->type == l_key) { printDebug("Blok je prazdny\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  return declareListContent(in, table, lex);
}

/*   Rekurzivni zpracovani promennych v deklaracni casti
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <declareListContent> -> "id" ":" "type" ";" <declareListContent>
 *   <declareListContent> -> eps
 */
int declareListContent(struct input * in, btree * table, token * lex)
{
  printDebug("Vytvarim promennou\n");
  int result;

  if (lex->type != l_id)
  {
    if (lex->type == l_key) { printDebug("Konec bloku promennych\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  if ( __SymbolTableSearch(table->local, ((string *)lex->data)->str) != NULL) return EXIT_NOT_DEFINED_ERROR;

  // zalohujeme si identifikator, zatim nevime, jaky je to typ
  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  struct node * nd; // vytvorime zaznam v tabulce symbolu pokud uz neexistuje v lokalni tabulce
  if ((__SymbolTableSearch(table->local, tmp) != NULL) || ((nd = SymbolTableCreateNode(tmp, *((key *)lex->data), NULL)) == NULL))
  {
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }
  SymbolTableInsert(table, nd); // vlozime symbol do lokalni tabulky (doresit redeklaraci)

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_endl ) {; return EXIT_SYNTAX_ERROR; }

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return declareListContent(in, table, lex);
}

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsList> -> "id" ":" "type" ";" <paramsList>
 *   <paramsList> -> "id" ":" "type"
 */
int paramsList(struct input * in, token * lex, struct funcParam ** param)
{
  printDebug("Vytvarim promennou\n");
  int result;

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id)
  {
    if (lex->type == l_rparenth) { printDebug("Konec bloku parametru\n"); return EXIT_SUCCESS;} // nedosel identifikator -> konec
    return EXIT_SYNTAX_ERROR;
  }

  // zalohujeme si identifikator, zatim nevime, jaky je to typ
  if ((*param = (struct funcParam *) malloc(sizeof(struct funcParam))) == NULL) return EXIT_INTERNAL_ERROR;
  strncpy((*param)->keyValue, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  (*param)->type = *(key *)lex->data;
  (*param)->next = NULL;
  printDebug("Vytvarim parametr funkce\n");

  // Nasleduje ";" nebo ")" podle toho se uvidi co dal
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  if (lex->type == l_rparenth) return EXIT_SUCCESS; // je to zavorka -> konec
  else if (lex->type != l_endl) return EXIT_SYNTAX_ERROR; // musi to byt strednik

  return paramsList(in, lex, &((*param)->next));
}

/*   Deklarace funkce
 * ---------------------------------------------------------------------
 * - zpracuje deklaraci funkce, ulozi jeji identifikator, a parametry
 *   <function> -> "function" "id" "(" <paramList> ")" ":" "typ" ";" <forward>
 *   <function> -> eps
 *   <forward> -> "forward" ";"
 *   <forward> -> <declareList> <body> ";" <function>
 */
int function(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Funkce\n");
  int result = EXIT_SUCCESS;
  struct node * nd = NULL, * new; // vytvorime zaznam v tabulce symbolu

  // Prvni token musi byt klicove slovo "function"
  if ((lex->type != l_key)  || (*(key *)lex->data != k_function ))
    { printDebug("Konec bloku deklarace funkci\n"); return EXIT_SUCCESS; }

  // Dalsi token musi byt ID, ktere si musim zatim zalohovat
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;
  if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) != NULL) &&
      ((nd->type != k_function) || ((funcData *)nd->data)->defined))
    // pokud uz je identifikator v tabulce a pritom to neni funkce (ktera jeste nebyla definovana) -> chyba
    return EXIT_SYNTAX_ERROR;

  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt oteviraci zavorka
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth ) { return EXIT_SYNTAX_ERROR; }

  //zpracuju parametry
  struct funcParam * firstParam = NULL;
  if ((result = paramsList(in, lex, &firstParam)) != EXIT_SUCCESS) return result;
  printDebug("Parametry funkce zpracovany\n");

  struct funcParam * paramDecl, * paramDef;
  // pokud byla uz dopredu deklarovana a ted je jeji definice, pak musi sedet parametry
  if (nd != NULL)
  {
    printDebug("Nalezen drive definovany prototyp\n");
    paramDecl = firstParam;
    paramDef = ((funcData *)nd->data)->param;
    while (paramDecl != NULL || paramDef != NULL) // zkontroluju obsah jednotlivych parametru
    {
      if ((paramDecl->type != paramDef->type) || (strcmp(paramDecl->keyValue, paramDef->keyValue) != EXIT_SUCCESS))
        break;
      paramDecl = paramDecl->next;
      paramDef = paramDef->next;
    }
    if (paramDecl != NULL && paramDef != NULL) result = EXIT_TYPE_ERROR; // nemaji stejny pocet parametru

    if (result != EXIT_SUCCESS) // prototyp neodpovida nove deklaraci -> smazat parametry a konec
    {
      FunctionParamsListDispose(firstParam);
      return result;
    }
    printDebug("Parametry sedi s prototypem\n");
  }

  // Dalsi token musi byt dvojtecka
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon ) { return EXIT_SYNTAX_ERROR; }

  // nasleduje token definujici typ navratove hodnoty
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if ((lex->type != l_key ) || // neni to klic nebo to neni ani jeden typ
      ((*(key *)lex->data != k_int) &&
      (*(key *)lex->data != k_bool) &&
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  if ((nd != NULL) && (*(key *)lex->data != ((funcData *)nd->data)->retType)) // navratova hodnota prototypu neodpovida deklaraci -> smazat parametry a konec
  {
    printDebug("Jiny navratovy typ\n");
    FunctionParamsListDispose(firstParam);
    return EXIT_TYPE_ERROR;
  }
  key type = *(key *)lex->data; // ulozit pro pozdejsi pouziti

  // Nasleduje ";"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;

  // "forward" || "var" || "begin"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key) return EXIT_SYNTAX_ERROR;

  bool defined = false;
  if (*(key *)lex->data != k_forward) defined = true;

  // jeden prototyp uz je a ja vytvarim druhy
  if (nd != NULL && defined == false)
  {
    printDebug("Nova deklarace prototypu funkce\n");
    FunctionParamsListDispose(firstParam);
    return EXIT_SEMANTIC_ERROR;
  }

  if ((nd = SymbolTableCreateFunctionNode(tmp, type, firstParam, defined)) == NULL)
  { // pokud selze vytvoreni uzlu funkce
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }
  SymbolTableInsert(table, nd); // vlozime symbol

  if (*(key *)lex->data == k_forward) // token je "forward", potrebuju ";"
  {
    printDebug("Dopredna deklarace\n");
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  else
  {
    printDebug("Definice funkce\n");
    // inicializuju ilist a TS funkce
    listInit(&(((funcData *)nd->data)->ilist));
    SymbolTableInitLocal(((funcData *)nd->data)->table, table);

    //vytvorim uzel pro navratovou hodnotu
    struct node * retNode;
    if ((retNode = SymbolTableCreateNode(tmp, type, NULL)) == NULL)
    { // pokud selze vytvoreni uzlu navratove hodnoty
      __SymbolTableDispose(&retNode);
      return EXIT_INTERNAL_ERROR;
    }
    SymbolTableInsert(((funcData *)nd->data)->table, retNode); // vlozime symbol
    while (firstParam != NULL)
    {
      if ((new = SymbolTableCreateNode(firstParam->keyValue, firstParam->type, NULL)) == NULL)
      {
        __SymbolTableDispose(&new);
        return EXIT_INTERNAL_ERROR;
      }
      SymbolTableInsert(((funcData *)nd->data)->table, new); // vlozime nove vytvoreny symbol bez hodnoty
      firstParam = firstParam->next;
    }
    // pokud selze definice funkce, uvolnuju jeji tabulku symbolu a koncim
    if (((result = declareList(in, ((funcData *)nd->data)->table, lex)) != EXIT_SUCCESS) ||
        ((result = body(in, ((funcData *)nd->data)->table, &(((funcData *)nd->data)->ilist), lex)) != EXIT_SUCCESS))
      return result;
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return function(in, table, ilist, lex);
}

/*   Rekurzivni zpracovani promennych v deklaraci funkce
 * ---------------------------------------------------------------------
 * - necha zpracovat deklaraci promenne, strednik a rekurzivne pokracuje
 * - zprostredkuje deklaraci promenne a jeji ulozeni do tabulky symbolu
 * - na vstupu mam token s identifikatorem
 *
 *   <paramsCall> -> "id" "," <paramsCall>
 *   <paramsCall> -> "id"
 */
int paramsCall(struct input * in, btree * table, tListOfInstr * ilist, token * lex, funcData * function, struct funcParam * param)
{
  printDebug("Zpracovavam parametry volani\n");
  int result;
  bool isOrd = false;

  if (param == NULL) { printDebug("Vsechny argumenty nacteny\n"); return EXIT_SUCCESS; } // mam spravny pocet parametru -> konec

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  struct node * nd, * paramNode;
  void * data;
  switch (lex->type)
  {
    case l_id: // prisel identifikator, musim ho najit (a musi byt stejneho typu) v tabulce a pouzit jeho hodnotu
      if ((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) return EXIT_NOT_DEFINED_ERROR;
      if (nd->type != param->type) return EXIT_TYPE_ERROR;

      data = nd->data;
    case l_int:
    case l_real:
    case l_str:
    case l_key: //pokud zadana hondota neodpovida typu parametru.. hnus fialovej, kdo to takhle nadeklaroval? ja...
      if (lex->type == l_key && *(key *)lex->data != k_true && *(key *)lex->type != k_false) return EXIT_SYNTAX_ERROR;
      if ((lex->type == l_int && param->type != k_int) ||
         ( lex->type == l_real && param->type != k_real) ||
         ( lex->type == l_str && param->type != k_string) ||
         ( lex->type == l_key && param->type != k_bool))
        return EXIT_TYPE_ERROR;
      isOrd = true;
      data = lex->data; // ulozim si data a hacknu lex->type, aby mi nedelal u stringu free
      lex->data = NULL;
      lex->type = l_int;
      break;
    default:
      return EXIT_SYNTAX_ERROR;
  }
  printDebug("Parametr volani nacten\n");

  if ((paramNode = SymbolTableSearch(function->table, param->keyValue)) == NULL)
  {
    printDebug("Parametr funkce nenalezen v lokalni tabulce, nemas delat hacky Coufale..\n");
    return EXIT_INTERNAL_ERROR;
  }
  generateInstruction(I_ASSIGN,param->type, data, NULL, paramNode->data, ilist);
  if (isOrd) generateInstruction(I_CLEAR, param->type, data, NULL, NULL, ilist);

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep && param->next != NULL) return EXIT_SYNTAX_ERROR; // neni carka, ale nemam vsechny parametry -> chyba

  return paramsCall(in, table, ilist,  lex, function, param->next);
}

/*   Volani funkce
 * ---------------------------------------------------------------------
 *   <callFunction> -> id "(" <paramsCall> ")"
 */
int callFunction(struct input * in, btree * table, tListOfInstr * ilist, token * lex, token * nextLex, struct node * retNode)
{
  int result = EXIT_SUCCESS;
  printDebug("Volani funkce\n");
  struct node * nd;
  if (lex->type == l_id) // uzivatelem definovana funkce
  {
    // musim overit, ze identifikator existuje a je definovana funkce
    if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) || (((funcData *)nd->data)->defined) == false)
      return EXIT_NOT_DEFINED_ERROR;
    if (nd->type != k_function)
      return EXIT_TYPE_ERROR;
    // nactu oteviraci zavorku
    if (nextLex != NULL)
    {
      if (nextLex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
    } else
    {
      if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
      if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
    }
    // zpracovani parametru
    if ((result = paramsCall(in, table, ilist, lex, (funcData *)nd->data, ((funcData *)nd->data)->param)) != EXIT_SUCCESS)
      return result;
    // nacteme pravou zavorku a konec
    if (lex->type != l_rparenth) return EXIT_SYNTAX_ERROR;
    generateInstruction(I_CALL_FUNCTION, k_function, &((funcData *)nd->data)->ilist, NULL, NULL, ilist); // vytvorime volani funkce (ma uz nactene parametry v tabulce)

    if (((nd = SymbolTableSearch(((funcData *)nd->data)->table, nd->keyValue)) == NULL))
      return EXIT_NOT_DEFINED_ERROR;
    generateInstruction(I_ASSIGN, retNode->type, nd->data, NULL, retNode->data, ilist); // prirazeni navratove hodnoty funkce do lhodonty
  }
  else if (lex->type == l_key) // vestavne funkce
  {
    switch (*(key *)lex->data)
    {
      case k_sort: result =  embededFuncSort(in, table, ilist, lex, retNode); break;
      case k_find: result = embededFuncFind(in, table, ilist, lex, retNode); break;
      case k_length: result = embededFuncLength(in, table, ilist, lex, retNode); break;
      case k_copy: result = embededFuncCopy(in, table, ilist, lex, retNode); break;
      default:
        return EXIT_SYNTAX_ERROR; break;
    }
  }
  else return EXIT_SYNTAX_ERROR;
  if (result != EXIT_SUCCESS || ((result = fillToken(in, lex)) != EXIT_SUCCESS)) return result;
  return result;
}

/*   Zpracovani prikazu programu
 * ---------------------------------------------------------------------
 * - zpracuju zde vsechna prirazeni, cykly, vestavne funkce, volani
 *   funkci, vetveni
 *
 *   <state> -> id := <evalExpression>
 *   <state> -> id := <callFunction>
 *   <state> -> if <evalExpression> then <body> ; else <body>
 *   <state> -> while <evalExpression> do <body>
 *   <state> -> write ( <type> )
 *   <state> -> readln ( <type> )
 */
int state(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Zpracovavam prikaz\n");
  switch (lex->type)
  {
    // <state> -> id := <evalExpression>
    // <state> -> id := <call>
    case (l_id): return embededAssign(in, table, ilist, lex);
    // <state> -> if <evalExpression> then <body> ; else <body>
    // <state> -> while <evalExpression> do <body>
    // <state> -> write ( <type> )
    // <state> -> readln ( <type> )
    case (l_key):
      switch (*(key *)lex->data)
      {
        case k_if: return embededIf(in, table, ilist, lex);
        case k_while: return embededWhile(in, table, ilist, lex);
        case k_write: return embededFuncWrite(in, table, ilist, lex);
        case k_readln: return embededFuncReadln(in, table, ilist, lex);
        default: return  EXIT_SYNTAX_ERROR;
      }
      break;
    default: return EXIT_SYNTAX_ERROR;
  }

  return EXIT_SUCCESS;
}

/*   Blok kodu
 * ---------------------------------------------------------------------
 * - zpracovavam jednotlive prikazy v bloku kodu programu
 * - rekurzivne zpracuje prikaz, ";" a znova dokud po prikazu nasleduje
 *   strednik
 *
 *   <statements> -> <state> ; <statements>
 *   <statements> -> <state>
 */
int statements (struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result;
  printDebug("Zacatek noveho prikazu\n");
  // nebyl to end, to jsme kontrolovali jeste v body
  // tzn ze tam bude nejaky prikaz
  // zavolame state
  result = state(in, table, ilist, lex);
  if (result != EXIT_SUCCESS) return result;

  // pokud dalsi token je strednik tzn ze bude
  // nasledovat dalsi prikaz
  if (lex->type == l_endl)
  {
    printDebug("Nasleduje prikaz\n");
    if ((result = fillToken(in, lex)) != EXIT_SUCCESS) return result;
    return statements(in, table, ilist, lex);
  }
  // pokud uz strednik nebude
  // za poslednim prikazem nema byt strednik
  // tzn ze se vratime do body
  else return result;
}

/*  Telo programu
 * ---------------------------------------------------------------------
 * - za tokenem "begin" nasleduje telo programu -> voleme statements a
 *   pak zkontrolujeme "end"
 * <body> -> <statements> end
 * <body> -> end
 */
int body(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Zacatek bloku kodu\n");
  int result = EXIT_SUCCESS;
  if ((result = fillToken(in, lex)) != EXIT_SUCCESS)
    return result;
  // naplneny token, pokud nic neselze
  // zkontroluje prazdne telo programu, kdy po "begin" nasleduje hned "end"
  if (lex->type == l_key && *(key *)lex->data == k_end )
  {
    printDebug("Prazdny blok kodu\n"); return result;
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  // a projde telo programu -> pokud cokoliv z toho selze, vraci error
  if ((result = statements(in, table, ilist, lex)) != EXIT_SUCCESS)
    return result;

  printDebug("Konec bloku kodu\n");
  if ((lex->type != l_key) || (*(key *)lex->data != k_end))
  { printDebug("Chybi end, mam typ: %d", *(key *)lex->data); return EXIT_SYNTAX_ERROR;} // za telem programu musi byt "end",

  return result;
}

/*   Parser
 * ---------------------------------------------------------------------
 * - volan hlavnim programem
 * - potrebuje znat zdrojovy soubor, tabulku symbolu a list instrukci
 * - program zacina deklaracnim liste promennych, deklaraci funkce a pak
 *   nasleduje samotne telo programu
 *
 * <program> -> <decList> <funciton> <body> <EOF>
 */
int parser(struct input * in, btree * table, tListOfInstr * ilist)
{
  printDebug("Spoustim Parser\n");
  token lex;
  int result = EXIT_SUCCESS;

  if ((tokenInit(&lex) == EXIT_SUCCESS) &&
      ((result = fillToken(in, &lex)) == EXIT_SUCCESS))
  {
    switch(lex.type)
    {
      case l_key: // pokud je to klicove slovo, var,function,begin (vyuzivam toho, ze C vyhodnocuje disjunkci zleva)
        if ((*(key *)lex.data != k_var) && (*(key *)lex.data != k_function) && (*(key *)lex.data != k_begin))
          { result = EXIT_SYNTAX_ERROR; break; }

        if (((result = declareList(in, table, &lex)) != EXIT_SUCCESS) || //Kontrola deklarace promennych (nemusi byt nic deklarovano)
            ((result = function(in, table, ilist, &lex)) != EXIT_SUCCESS) || // Kontrola funkci deklarace (nemusi byt nic deklarovano)
            ((result = body(in, table, ilist, &lex)) != EXIT_SUCCESS)) { break; } // Hlavni program begin

        if ((lex.type == l_enddot) ||
           ((result = fillToken(in, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_eof)) { result = EXIT_SYNTAX_ERROR; break; } // na konci programu musi byt . a pak EOF

          //generateInstruction(I_END, NULL, NULL, NULL); // instrukce?? + ilist
        break;
      case l_eof: // prazdny soubor
        result = EXIT_SUCCESS;
        break;
      default: // cokoliv jineho je chyba
        result = EXIT_SYNTAX_ERROR;
        break;
    }
  }

  if (result == EXIT_SEMANTIC_ERROR) printErr("SEMANTIC ERROR on line %d (excluding blank lines): Variable type conflict, use of an undeclared variable or conflict redeclarations.\n", in->line);
  if (result == EXIT_NOT_DEFINED_ERROR) printErr("NOT DEFINED (REDEFINED) ERROR on line %d (excluding blank lines): You are using undefined variable or trying to redefine something.\n", in->line);
  if (result == EXIT_TYPE_ERROR) printErr("TYPE ERROR on line %d (excluding blank lines): Some variable is of a wrong type. Or maybe you have used wrong amount or type of parameter of function.\n", in->line);
  if (result == EXIT_SYNTAX_ERROR) printErr("SYNTAX ERROR on line %d (excluding blank lines): Check your program once more, please.\n", in->line);
  if (result == EXIT_INTERNAL_ERROR) printErr("INTERNAL ERROR on line %d (excluding blank lines): Something really bad happend. This is not your fault.\n", in->line);
  tokenFree(&lex);
  return result;
}
