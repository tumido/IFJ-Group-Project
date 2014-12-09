/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  12/09/2014 09:43:47 PM
 *          Autor:  Tomáš Coufal
 *          Login:  xcoufa09
 *        Projekt:  
 * =====================================================================
 */

#include "constructs.h"

/*   Vestavna funkce Write
 * ---------------------------------------------------------------------
 * - zpracovava neomezeny pocet argumentu
 * - kazdy argument vyhodnoti (zdali je to ID (platne, neni funkce) nebo
 *   hodnota) a zavola pro nej samostatnou write instrukci
 */
int embededFuncWrite(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result = EXIT_SUCCESS;
  struct node * loc;
  bool hadParam = false;
  printDebug("Write\n");
  // mam "write"
  // potrebuju "("
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju zpracovat n parametru (dokud mi je token id nebo hodnota volam)
  void * data;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  while (lex->type == l_id || lex->type == l_int || lex->type == l_real || lex->type == l_str ||
      (lex->type == l_key && (*(key *)lex->data == k_true || *(key *)lex->data == k_false)))
  {
    printDebug("Parametr funkce write\n");
    hadParam = true;
    if (lex->type == l_id)
    {
      if (((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL) || loc->type == k_function)
        return EXIT_NOT_DEFINED_ERROR;
      data = loc->data;
    }
    else data = lex->data;
     generateInstruction(I_WRITE, data, NULL, NULL, ilist);
    // nactu ","
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
    if (lex->type != l_sep) break;
    //pokracuju s dalsim identifikatorem
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  }
  // potrebuju ")"
  if (lex->type != l_rparenth || !hadParam)
  {
    return EXIT_SYNTAX_ERROR;
  }
  return result;
}

/*   Vestavna funkce readline
 * ---------------------------------------------------------------------
 * - vyhodnoti poskytnuty parametr a zapise do nej hodnotu ze vstupu
 *   resp. vytvori patricnou instrukci
 */
int embededFuncReadln(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result = EXIT_SUCCESS;
  struct node * loc;
  printDebug("Readln\n");
  // potrebuju "("
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju id
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_id) return EXIT_SYNTAX_ERROR;
  if ((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
    return EXIT_NOT_DEFINED_ERROR;
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_rparenth)
  {
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci
  generateInstruction(I_READ, NULL, NULL, loc, ilist);
  return result;
}

int embededFuncLength(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * loc)
{
  int result = EXIT_SUCCESS;
  struct node * what;
  printDebug("Length\n");
  // potrebuju "("
  string * data;
  if (loc->type != k_int) return EXIT_TYPE_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju id stringu
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type == k_string) return EXIT_TYPE_ERROR;
    data = what->data;
  }
  else if (lex->type == l_str)
  {
    data = lex->data;
    lex->data = NULL;
  }
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_rparenth)
  {
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci
  generateInstruction(I_ASSIGN, k_int, &data->length, NULL, loc->data, ilist);
  return result;
}
int embededFuncCopy(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  return EXIT_INTERNAL_ERROR;
}
int embededFuncFind(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  return EXIT_INTERNAL_ERROR;
}
int embededFuncSort(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  return EXIT_INTERNAL_ERROR;
}
/*   Prirazeni
 * ---------------------------------------------------------------------
 * - nacte si promennou do ktere se ma hodnota priradit a rozhodne,
 *   jestli bude volana funkce a nebo je to vyraz
 * - nasledne vola
 */
int embededAssign(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result = EXIT_SUCCESS;
  struct node * loc;
  printDebug("Prirazeni\n");
  if ((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL || loc->type == k_function)
    return EXIT_NOT_DEFINED_ERROR;

  // potrebuji ":="
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_assign) return EXIT_SYNTAX_ERROR;

  // nasleduje bud id -> volam <callFunction>, jinak <evalExpression>
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  // je to vestavna funkce s navratovou hodnotou volam
  if (lex->type == l_key && (*(key *)lex->data == k_sort ||
        *(key *)lex->data == k_find ||
        *(key *)lex->data == k_length ||
        *(key *)lex->data == k_copy)) result = callFunction(in, table, ilist, lex, NULL, );
  else if (lex->type == l_id) // mam identifikator, ale jeste nevim, jestli chci vyhodnocovat vyraz nebo volat vestavnou funkci
  {
    token tmp; // pokud bude dalsi token zavorka volam funkci
    tokenInit(&tmp);
    if ((result = fillToken(in,&tmp)) != EXIT_SUCCESS){ return result; }
    if (tmp.type == l_lparenth) result = callFunction(in, table, ilist, lex, &tmp, );
    else result = evalExpression(in, table, ilist, lex, &tmp, loc);
    tokenFree(&tmp);
  }
  else result = evalExpression(in, table, ilist, lex, NULL, loc); // jinak (je to hodnota, cokoliv) volan evalExpression

  if (result != EXIT_SUCCESS) return result;

  // generateInstruction();
  return result;
}

/*   Podminene vetveni
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo podminky
 * - JAK JSOU RESENE JUMPY??
 */
int embededIf(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result = EXIT_SUCCESS;
  printDebug("Vetveni\n");
  // vyhodnoceni podminky
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS) ||
      ((result = evalExpression(in, table, ilist, lex, NULL, )) != EXIT_SUCCESS))
    { return result; }
  // musim si nekde pamatovat navesti, jak to resi interpret ??
  // nasleduje "then"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key && *(key *)lex->data != k_then) return EXIT_SYNTAX_ERROR;
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) { return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) return EXIT_SYNTAX_ERROR;
  if  ((result = body(in, table, ilist, lex)) != EXIT_SUCCESS) { return result; }
  // pak ";"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_endl) return EXIT_SYNTAX_ERROR;
  // pak "else"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key && *(key *)lex->data != k_else) return EXIT_SYNTAX_ERROR;
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) { return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) return EXIT_SYNTAX_ERROR;
  if  ((result = body(in, table, ilist, lex)) != EXIT_SUCCESS) { return result; }
  return result;
}

/*   Cyklus while
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo cyklu
 * - JAK JSOU RESENE JUMPY??
 */
int embededWhile(struct input * in, btree * table, tListOfInstr * ilist, token * lex)
{
  int result = EXIT_SUCCESS;
  printDebug("Cyklus\n");
  // mam "while"
  // vyhodnoceni podminky
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS) ||
      ((result = evalExpression(in, table, ilist, lex, NULL,)) != EXIT_SUCCESS))
    { return result; }
  // potrebuju "do"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_key && *(key *)lex->data != k_do) return EXIT_SYNTAX_ERROR;
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) { return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) return EXIT_SYNTAX_ERROR;
  if  ((result = body(in, table, ilist, lex)) != EXIT_SUCCESS) { return result; }
  // jump back na podminku
  return result;
}
