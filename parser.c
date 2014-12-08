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

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3, tListOfInstr * ilist)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(ilist, I);
   return;
}
/*// vraci index do tabulky
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
    case (l_mul):
    case (l_add):
    case (l_div):
    case (l_sub):
    case (l_bool):
    case (l_id): j=12; break;
    case (l_eof): j=13; break;
    default j=13;
  }

 return j;
}

// teto funkci se předá soubor, tabulka, token a ??struktura node, kam ulozim vypocitany vyraz??
int expression(FILE *in->file, btree *table, token *lex,node *data)
{
 sData itemAct; // tady bude aktualni token
 sData itemTop; // tady bude token co je navrcholu zasobniku
 sData itemC;   // pomocne tokeny
 sData itemD;
 sData itemSign; // tady uchovam operator
 if (tokenInit(&itemA.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemB.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemC.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemD.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 if (tokenInit(&itemE.lexdata) == EXIT_INTERNAL_ERROR) return EXIT_INTERNAL_ERROR;
 itemAct.TypTok=l_eof;  // ulozim si typ EOF, slouzi pouze jako zarazka v zasobniku
 Stack s;             // inicializuji si zasobnik
 sInit (&s);
 sPush (&s,itemAct);      // ulozim item s EOF

 int result=EXIT_SUCCESS;

 struct node *nd;
 // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
 // do itemAct.lexdata.data
 // nevim jak to udelat, zatim necham jednoduchy prirazeni
 //++++++++++++++++++++++++++++++
 if (lex->type=l_id)
 {
  if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
  return EXIT_SEMANTIC_ERROR;
  // ulozim si ze stromu hodnoty
  itemAct.lexdata.data = nd->data; // ulozime si hodnotu
  itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
  itemAct.TypTop = l_id;
 }
 // jestli je token int,real,str ulozim si hodnotu a typ
 else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
 {
  itemAct.lexdata.data = lex->data;
  itemAct.lexdata.type = lex->type;
  itemAct.TypTop = l_id;
 }
 else
 // pokud to neni id ani zadny cislo tak si prsote ulozime typ tokenu
  itemAct.TypTok=lex->type;

 itemTop=sTop(&s); // kouknu se, co je nahore v zasobniku
 do
 {
  switch (Table[retIndex(itemTop.TypTok),retIndex(itemAct.TypTok)]):
  {
  // pokud je to =
  case Staight:
    sPush (&s,itemAct);  // dam na zasobnik token a vezmu dalsi
    itemTop= sTop (&s);  // iTop = vrhcol zasobniku
    // do iAct si nactu dalsi token
    if (result=fillTOken (in->file,lex) == EXIT_LEXICAL_ERROR) return EXIT_LEXICAL_ERROR;
    // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
    // do itemAct.data
    // nevim jak to udelat, zatim necham jednoduchy prirazeni
    //++++++++++++++++++++++++++++++
    if (lex->type=l_id)
    {
     if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
     return EXIT_SEMANTIC_ERROR;
     // ulozim si ze stromu hodnoty
     itemAct.lexdata.data = nd->data; // ulozime si hodnotu
     itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
     itemAct.TypTop = l_id;
    }
    // jestli je token int,real,str ulozim si hodnotu a typ
     else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
     {
      itemAct.lexdata.data = lex->data;
      itemAct.lexdata.type = lex->type;
      itemAct.TypTop = l_id;
     }
     else
     itemAct.TypTok=lex->type;
  break;

  // pokud je to <
  // koukneme se co je na vrhcolu zasobniku
  // jestli je nahore ], tak pred nej dam [
  // jestli tam neni tak jenom dam [
  case Left:
    itemC= sTop (&s); // koukneme se co je nahore
    itemTop.TypTok= l_lbracket; // nastavim < ([)
    // jestli je nahore E (])
    if (itemC.TypTok == l_rbracket)
    {
     // zamenime E (]) za <E ([])
     itemC=sPop(&s); // popneme E (])
     sPush (&s,itemTop);
     sPush (&s,itemC);
    }
    // na vrcholu neni E (])
    // vlozime < ([)
    else
    {
     sPush(&s,itemTop);
    }
    // nakonec vlozime token na vrchol
    sPush (&s,itemAct);
    //nactu dalsi token a itemTop bude vrchol zasobniku
    if (result=fillTOken (in->file,lex) == EXIT_LEXICAL_ERROR) return EXIT_LEXICAL_ERROR;
    itemTop=sTop(&s);
    // pokud je nacteny token ID tak si musim najit jeho data a ulozit je
    // do itemA.data
    // nevim jak to udelat, zatim necham jednoduchy prirazeni
    //++++++++++++++++++++++++++++++
    if (lex->type=l_id)
    {
     if ( (nd=SymbolTableSearch (table,((string *)lex->data)->str))==NULL)
     return EXIT_SEMANTIC_ERROR;
     // ulozim si ze stromu hodnoty
     itemAct.lexdata.data = nd->data; // ulozime si hodnotu
     itemAct.lexdata.type = nd->type; // a typ (int,boolean,real,str)
     itemAct.TypTop = l_id;
    }
    // jestli je token int,real,str ulozim si hodnotu a typ
    else if(lex->type == l_int || lex->type == l_real || lex->type == l_str || lex->type==l_bool)
    {
     itemAct.lexdata.data = lex->data;
     itemAct.lexdata.type = lex->type;
     itemAct.TypTop = l_id;
    }
    else
    itemAct.TypTok=lex->type;
  break;

  // pokud je to >
  case Right:
    itemD=sTop(&s); // kouknu se co je nahore

    switch (itemD.TypTok)
    {
     // pokud je nahore ID
     // redukuji na E->i a ulozim si hodnotu
     case l_id:
      itemD=sPop(&s); // id,int,real popneme
      itemC=sPop(&s); // vime ze pred id je < ([)
      if(itemC.TypTok==l_lbracket)
      {
       // v i by mela byt data, kdyby se nahodou stalo
       // ze data by byla prazdna
       // jestli zadny nema je to semanticka chyba
       if (itemD.lexdata.data== NULL)
       {
         return EXIT_SEMANTIC_ERROR;
       }
       // zredukovali jsme <i na E (])
       itemD.TypTok=l_rbracket;
       itemTop=sTop(&s); // itemB bude hodnota, co je pred E (])
       sPush (&s,itemD);// pushneme E (])
       }
       break; // itemA je aktualni token porad a itemTop ma hodnotu pred E (])
       // vrchol je E (])
      case l_rbracket:
       // kdyz dojde k teto situaci tak vime, ze v zasobniku
       // pred E musi byt nejaka operace
       itemD=sPop(&s); // Popneme E (])
       itemSign=sPop(&s); // popneme operaci, (mela by byt operace)
       if (itemSign.TypTok >=l_add && itemSign.TypTok <= l_not)
       {
        itemC=sPop(&s);// popneme druhy E (])
        itemTop=sPop(&s);
        // pred E by melo byt < ([)
        // zkontrolujeme jestli tam bylo <E
        if ((itemTop.TypTok == l_lbracket) && (itemC.TypTok==l_rbracket) )
        {
         // mel bych si vygenerovat unikatni nazev pro vysledek
         //++++++++++++++++++++
         switch(itemSign.TypTok)
         {
            // operace scitani
            case l_add:
             // int + int = int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int + real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real + int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real+real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             // real
             // str+str str (konkatenace)
             else if ((itemD.lexdata.lexType==l_str) && (itemC.lexdata.lexType==l_str) )
             //str
             else return EXIT_SEMANTIC_ERROR;
             //if (generateInstruction (I_ADD,.....))
             // +++++++
             // nastavit vrchol zasobniku
            break;
            // operace odcitani
            case l_sub:
             // int - int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int - real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real - int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real+real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             else return EXIT_SEMANTIC_ERROR;
             //if(generateInstruction(I_SUB,....
             //++++++++++++++++++++++++++++
            break;
            // nasobeni
            case l_mul:
             // int*int = int
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // int * real = real
             else if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             // real * int = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_int) )
             // typ real
             //real*real=real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             else return EXIT_SEMANTIC_ERROR;
             //if(generateInstruction(I_MUL,....
             //++++++++++++++++++++++++++++
            break;
            // deleni
            case l_div:
             // int/int = real
             if ((itemD.lexdata.lexType==l_int) && (itemC.lexdata.lexType==l_int) )
             // musim nastavit vysledek na typ int
             // real / real = real
             else if ((itemD.lexdata.lexType==l_real) && (itemC.lexdata.lexType==l_real) )
             // na typ real
             else return EXIT_SEMANTIC_ERROR;
             // if(generateInstruction(I_DIV
             // ********************************
            break;
            case l_greater:
             //if(generateInstruction(I_GREATER
            break;
            case l_less:
             //if(generateInstruction(I_LESS
            case l_gequal:
             //if(generateInstruction(I_GREATEQ,
            case l_lequal:
             //if(generateInstruction(I_LESSEQ,
            case l_equal:
             //if(generateInstruction(I_EQ ..
            case l_not:
             //if(generateInstruction(I_NOTEQ
            default:
             return EXIT_SEMANTIC_ERROR;
           }
        }
      }
      break;
      // pokud je tam zavorka
      case l_rparenth:
      //++++++++++++++
      break;
      default: return EXIT_SEMANTIC_ERROR; break;
   }
   break;
   case Err:
   //++++++++++++++++
   break;
  }
 }while (itemTop.TypTok != l_eof && itemAct.TypTok!= l_eof) ;

 if (itemTop.TypTok == l_eof && itemAct.TypTop== l_eof)
 result = EXIT_SUCCESS;

 return result;
}
*/


/*   Syntakticka analyza
 * =====================================================================
 * - "zhora dolu", vyuziva rekurzivni sestup
 */

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

  if ( SymbolTableSearch(table, ((string *)lex->data)->str) != NULL) return EXIT_SYNTAX_ERROR;

  // zalohujeme si identifikator, zatim nevime, jaky je to typ
  char tmp[BUFSIZE];
  strncpy(tmp, ((string *)lex->data)->str, BUFSIZE);

  // Dalsi token musi byt ":"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_colon) { return EXIT_SYNTAX_ERROR; }

  // Dalsi token musi byt datovy typ
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key) { return EXIT_SYNTAX_ERROR; }

  struct node * nd; // vytvorime zaznam v tabulce symbolu
  if ((nd = SymbolTableCreateNode(tmp, *((key *)lex->data), NULL)) == NULL)
  {
    __SymbolTableDispose(&nd);
    return EXIT_INTERNAL_ERROR;
  }
  SymbolTableInsert(table, nd); // vlozime symbol

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
  struct node * nd = NULL; // vytvorime zaznam v tabulce symbolu

  // Prvni token musi byt klicove slovo "function"
  if ((lex->type != l_key)  || (*(key *)lex->data != k_function ))
    return EXIT_SUCCESS;

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
    if (paramDecl != NULL && paramDef != NULL) result = EXIT_SEMANTIC_ERROR; // nemaji stejny pocet parametru

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
      (*(key *)lex->data != k_real) &&
      (*(key *)lex->data != k_string)))
    return EXIT_SYNTAX_ERROR;

  if ((nd != NULL) && (*(key *)lex->data != ((funcData *)nd->data)->retVal)) // navratova hodnota prototypu neodpovida deklaraci -> smazat parametry a konec
  {
    printDebug("Jiny navratovy typ\n");
    FunctionParamsListDispose(firstParam);
    return EXIT_SEMANTIC_ERROR;
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
    if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;
  }
  else
  {
    printDebug("Definice funkce\n");
    listInit(&(((funcData *)nd->data)->ilist));
    SymbolTableInitLocal(((funcData *)nd->data)->table, table);
    if (((result = declareList(in, ((funcData *)nd->data)->table, lex)) != EXIT_SUCCESS) ||
        ((result = body(in, table, &(((funcData *)nd->data)->ilist), lex)) != EXIT_SUCCESS))
      { SymbolTableDispose(((funcData *)nd->data)->table); return result; }
  }
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
int paramsCall(struct input * in, btree * table, token * lex, funcData * function, struct funcParam * param)
{
  printDebug("Zpracovavam parametry volani\n");
  int result;

  if (param == NULL) { printDebug("Vsechny argumenty nacteny\n"); return EXIT_SUCCESS; } // mam spravny pocet parametru -> konec

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }

  struct node * nd, * new;
  switch (lex->type)
  {
    case l_id: // prisel identifikator, musim ho najit (a musi byt stejneho typu) v tabulce a pouzit jeho hodnotu
      if (((nd = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) || (nd->type != param->type))
        return EXIT_SEMANTIC_ERROR;
      if ((new = SymbolTableCreateNode(param->keyValue, nd->type, nd->data)) == NULL)
      {
        __SymbolTableDispose(&new);
        return EXIT_INTERNAL_ERROR;
      }
      SymbolTableInsert(function->table, new); // vlozime nove vytvoreny symbol se zkopirovanou hodnotou odkazovaneho
      break;
    case l_int:
    case l_real:
    case l_str:
    default:
      return EXIT_SYNTAX_ERROR;
  }
  printDebug("Parametr volani nacten\n");

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep) return EXIT_SYNTAX_ERROR; // neni carka, ale nemam vsechny parametry -> chyba

  return paramsCall(in, table, lex, function, param->next);
}
/*   Zpracovani prikazu programu
 * ---------------------------------------------------------------------
 * - zpracuju zde vsechna prirazeni, cykly, vestavne funkce, volani
 *   funkci, vetveni
 *
 *   <state> -> id := <expression>
 *   <state> -> id := <call>
 *   <state> -> if <expression> then <body> ; else <body>
 *   <state> -> while <expression> do <body>
 *   <state> -> write ( <type> )
 *   <state> -> readln ( <type> )
 */
int state(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  printDebug("Zpracovavam prikaz\n");
  int result = EXIT_SUCCESS;

  return EXIT_SYNTAX_ERROR;
  struct node * loc = NULL;
  switch (lex->type)
  {
    // <state> -> id := <expression>
    // <state> -> id := <call>
    case (l_id):
      if ((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
        return EXIT_SEMANTIC_ERROR;

      // potrebuji ":="
      if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
      if (lex->type != l_assign) return EXIT_SYNTAX_ERROR;

      // bud nasleduje identifikator nebo expression
      //  evalExpression();
      break;
    // <state> -> if <expression> then <body> ; else <body>
    // <state> -> while <expression> do <body>
    // <state> -> write ( <type> )
    // <state> -> readln ( <type> )
    case (l_key):
      switch (*(key *)lex->data)
      {
        case k_if:
          // vyhodnoceni podminky
        //  if (((result = fillToken(in,lex)) != EXIT_SUCCESS) ||
        //      ((result = evalExpression()) != EXIT_SUCCESS))
        //  { return result; }
        //  

        case k_while:
        case k_write:
        case k_readln:
        default:
          result = EXIT_SYNTAX_ERROR;
          break;
      }
      break;
    default:
      result = EXIT_SYNTAX_ERROR;
      break;
  }
  return result;
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
    return statements(in, table, ilist, lex);
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
  printDebug("Zacatek hlavniho kodu programu\n");
  int result;
  if (((result = fillToken(in, lex)) != EXIT_SUCCESS) ||
      // naplneny token, pokud nic neselze
   //   (strcmp("end", ((string *)lex->data)->str) == EXIT_SUCCESS) ||
      // zkontroluje prazdne telo programu, kdy po "begin" nasleduje hned "end"
      ((result = statements(in, table, ilist, lex)) != EXIT_SUCCESS))
      // a projde telo programu -> pokud cokoliv z toho selze, vraci error
    return result;
  printDebug("Konec Programu");
  if ((lex->type != l_key) || (*(key *)lex->data != k_end))
    return EXIT_SYNTAX_ERROR; // za telem programu musi byt "end",

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

        if (((result = fillToken(in, &lex)) != EXIT_SUCCESS) ||
           (lex.type == l_enddot) ||
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

  if (result == EXIT_SEMANTIC_ERROR) printErr("SEMANTIC ERROR on line %d: Check your program once more, please, wrong type assignment or use of undeclared variable.\n", in->line);
  if (result == EXIT_SYNTAX_ERROR) printErr("SYNTAX ERROR on line %d: Check your program once more, please.\n", in->line);
  if (result == EXIT_INTERNAL_ERROR) printErr("INTERNAL ERROR on line %d: Something really bad happend. This is not your fault.\n", in->line);
  tokenFree(&lex);
  return result;
}
