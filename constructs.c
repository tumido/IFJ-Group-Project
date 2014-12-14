/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  12/09/2014 09:43:47 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
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
  bool hadParam = false, isOrd = false;
  printDebug("Write\n");
  // mam "write"
  // potrebuju "("
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju zpracovat n parametru (dokud mi je token id nebo hodnota volam)
  void * data; key type;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  while (lex->type == l_id || lex->type == l_int || lex->type == l_real || lex->type == l_str ||
      (lex->type == l_key && (*(key *)lex->data == k_true || *(key *)lex->data == k_false)))
  {
    printDebug("Parametr funkce write\n");
    hadParam = true;
    isOrd = false;
    if (lex->type == l_id)
    {
      if ((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
        return EXIT_NOT_DEFINED_ERROR;
      data = loc->data;
      type = loc->type;
    }
    else
    {
      data = lex->data;
      switch(lex->type)
      { case l_int: type = k_int; break; case l_real: type = k_real; break; case l_str: type = k_string; break; default: return EXIT_INTERNAL_ERROR;}
      lex->type = l_int;
      isOrd = true;
      lex->data = NULL;
    }
    generateInstruction(I_WRITE, type, data, NULL, NULL, ilist);
    if (isOrd) generateInstruction(I_CLEAR, type, data, NULL, NULL, ilist);
    // nactu ","
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){if (isOrd) free(data); return result; }
    if (lex->type != l_sep) break;
    //pokracuju s dalsim identifikatorem
    if ((result = fillToken(in,lex)) != EXIT_SUCCESS){if (isOrd) free(data);  return result; }
  }
  // potrebuju ")"
  if (lex->type != l_rparenth || !hadParam) return EXIT_SYNTAX_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
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
  generateInstruction(I_READ, loc->type, NULL, NULL, loc->data, ilist);
  loc->defined = true;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  return result;
}

int embededFuncLength(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * loc)
{
  int result = EXIT_SUCCESS;
  struct node * what;
  bool isOrd = false;
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
    if (what->type != k_string) return EXIT_TYPE_ERROR;
    data = what->data;
  }
  else if (lex->type == l_str)
  {
    data = lex->data;
    lex->type = l_int;
    lex->data = NULL;
    isOrd = true;
  }
  else return EXIT_TYPE_ERROR;
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_rparenth)
  {
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci (pokud jsem si hral s ordinalni hodnotou, musim ji uklidit)
  generateInstruction(I_ASSIGN, k_int, &data->length, NULL, loc->data, ilist);
  if (isOrd) generateInstruction(I_CLEAR , k_string, data, NULL, NULL, ilist);
  return result;
}
int embededFuncCopy(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * loc)
{
  int result = EXIT_SUCCESS;
  struct node * what;
  bool isOrd1 = false, isOrd2 = false, isOrd3 = false;
  printDebug("Copy\n");
  // potrebuju "("
  string * data;
  if (loc->type != k_string) return EXIT_TYPE_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju id stringu
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_string) return EXIT_TYPE_ERROR;
    data = what->data;
  }
  else if (lex->type == l_str) // nebo string
  {
    data = lex->data;
    lex->type = l_int;
    lex->data = NULL;
    isOrd1 = true;
  }
  else return EXIT_TYPE_ERROR;
  // nactu ","
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep) return EXIT_SYNTAX_ERROR;
  // potrebuju zadat rozsah (id nebo int)
  struct srange * range;
  if ((range = malloc (sizeof(struct srange))) == NULL) return EXIT_INTERNAL_ERROR;

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_int) return EXIT_TYPE_ERROR;
    range->start = (int *)what->data;
  }
  else if (lex->type == l_int) // nebo int
  {
    range->start = (int *)lex->data;
    lex->data = NULL;
    isOrd2 = true;
  }
  else return EXIT_TYPE_ERROR;
  // nactu ","
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep) return EXIT_SYNTAX_ERROR;
  // a posledni -> delku
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_int) return EXIT_TYPE_ERROR;
    range->length = (int *)what->data;
  }
  else if (lex->type == l_int) // nebo int
  {
    range->length = (int *)lex->data;
    lex->data = NULL;
    isOrd3 = true;
  }
  else return EXIT_TYPE_ERROR;
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_rparenth)
  {
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci (pokud jsem si hral s ordinalni hodnotou, musim ji uklidit)
  generateInstruction(I_COPY, k_string, data, range, loc->data, ilist);
  if (isOrd1) generateInstruction(I_CLEAR , k_string, data, NULL, NULL, ilist);
  if (isOrd2) generateInstruction(I_CLEAR , k_int, range->start, NULL, NULL, ilist);
  if (isOrd3) generateInstruction(I_CLEAR , k_int, range->length, NULL, NULL, ilist);
  generateInstruction(I_CLEAR , k_int, range, NULL, NULL, ilist);
  return result;
}

int embededFuncFind(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * loc)
{
  int result = EXIT_SUCCESS;
  struct node * what;
  bool isOrd1 = false, isOrd2 = false;
  printDebug("Find\n");
  // potrebuju "("
  string * data, * substr;
  if (loc->type != k_int) return EXIT_TYPE_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju id stringu
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_string) return EXIT_TYPE_ERROR;
    data = what->data;
  }
  else if (lex->type == l_str) // nebo string
  {
    data = lex->data;
    lex->type = l_int;
    lex->data = NULL;
    isOrd1 = true;
  }
  else return EXIT_TYPE_ERROR;
  // nactu ","
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_sep) return EXIT_SYNTAX_ERROR;
  // potrebuju substring, ktery se bude hledat

  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_string) return EXIT_TYPE_ERROR;
    substr = what->data;
  }
  else if (lex->type == l_str) // nebo int
  {
    substr = lex->data;
    lex->type = l_int;
    lex->data = NULL;
    isOrd2 = true;
  }
  else return EXIT_TYPE_ERROR;
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_rparenth)
  {
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci (pokud jsem si hral s ordinalni hodnotou, musim ji uklidit)
  generateInstruction(I_FIND, k_string, data, substr, loc->data, ilist);
  loc->defined = true;
  if (isOrd1) generateInstruction(I_CLEAR , k_string, data, NULL, NULL, ilist);
  if (isOrd2) generateInstruction(I_CLEAR , k_string, substr, NULL, NULL, ilist);
  return result;
}
int embededFuncSort(struct input * in, btree * table, tListOfInstr * ilist, token * lex, struct node * loc)
{
  int result = EXIT_SUCCESS;
  struct node * what;
  bool isOrd = false;
  printDebug("Sort\n");
  // potrebuju "("
  string * data;
  if (loc->type != k_string) return EXIT_TYPE_ERROR;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type != l_lparenth) return EXIT_SYNTAX_ERROR;
  // potrebuju id stringu
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){ return result; }
  if (lex->type == l_id)
  {
    if ((what = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
      return EXIT_NOT_DEFINED_ERROR;
    if (what->type != k_string) return EXIT_TYPE_ERROR;
    data = what->data;
  }
  else if (lex->type == l_str) // nebo string
  {
    data = lex->data;
    lex->type = l_int;
    lex->data = NULL;
    isOrd = true;
  }
  else return EXIT_TYPE_ERROR;
  // potrebuju ")"
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){if (isOrd) free(data); return result; }
  if (lex->type != l_rparenth)
  {
    if (isOrd) free(data);
    if (lex->type == l_sep) return EXIT_TYPE_ERROR;
    return EXIT_SYNTAX_ERROR;
  }
  // volam instrukci (pokud jsem si hral s ordinalni hodnotou, musim ji uklidit)
  generateInstruction(I_SORT, k_string, data, NULL, loc->data, ilist);
  loc->defined = true;
  if (isOrd) generateInstruction(I_CLEAR , k_string, data, NULL, NULL, ilist);
  return result;
}
/*   Prirazeni
 * ---------------------------------------------------------------------
 * - nacte si promennou do ktere se ma hodnota priradit a rozhodne,
 *   jestli bude volana funkce a nebo je to vyraz
 * - nasledne vola
 */
int embededAssign(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s)
{
  int result = EXIT_SUCCESS;
  struct node * loc;
  printDebug("Prirazeni\n");
  if ((loc = SymbolTableSearch(table, ((string *)lex->data)->str)) == NULL)
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
        *(key *)lex->data == k_copy))
  {
    if (loc->type == k_function) return EXIT_NOT_DEFINED_ERROR;
    result = callFunction(in, table, ilist, lex, NULL, loc, s);
  }
  else if (lex->type == l_id) // mam identifikator, ale jeste nevim, jestli chci vyhodnocovat vyraz nebo volat vestavnou funkci
  {
    token tmp; // pokud bude dalsi token zavorka volam funkci
    tokenInit(&tmp);
    if ((result = fillToken(in,&tmp)) != EXIT_SUCCESS){ return result; }
    if (tmp.type == l_lparenth) result = callFunction(in, table, ilist, lex, &tmp, loc, s);
    else result = evalExpression(in, table, ilist, lex, &tmp, loc->data, loc->type);
  }
  else result = evalExpression(in, table, ilist, lex, NULL, loc->data, loc->type); // jinak (je to hodnota, cokoliv) volan evalExpression
  loc->defined = true;
  return result;
}

/*   Podminene vetveni
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo podminky
 * - JAK JSOU RESENE JUMPY??
 */
int embededIf(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s)
{
  int result = EXIT_SUCCESS;
  printDebug("Vetveni\n");
  bool * condition;
  tListItem ** sign, ** sign2;
  if ((condition = malloc(sizeof(bool))) == NULL) return EXIT_INTERNAL_ERROR;
  if ((sign = malloc(sizeof(tListItem *))) == NULL) return EXIT_INTERNAL_ERROR;
  if ((sign2 = malloc(sizeof(tListItem *))) == NULL) return EXIT_INTERNAL_ERROR;
  // vyhodnoceni podminky
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS) ||
      ((result = evalExpression(in, table, ilist, lex, NULL, condition, k_bool)) != EXIT_SUCCESS))
    {free(condition); free(sign); free(sign2); return result; }
  // provedu podmineny jump (pokud je podminka nepravda, skacu)
  generateInstruction(I_JUMP, k_bool, condition, sign, NULL, ilist);
  // nasleduje "then"
  if (lex->type != l_key || *(key *)lex->data != k_then) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) {free(condition); free(sign); free(sign2);  return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  if  ((result = body(in, table, ilist, lex, s)) != EXIT_SUCCESS) {free(condition); free(sign); free(sign2);  return result; }
  // pak "else"
  generateInstruction(I_JUMP, k_bool, NULL, sign2, NULL, ilist);
  *sign = ilist->last;
  if ((result = fillToken(in,lex)) != EXIT_SUCCESS){free(condition); free(sign); free(sign2);  return result; }
  if (lex->type != l_key || *(key *)lex->data != k_else) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) {free(condition); free(sign); free(sign2);  return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  if  ((result = body(in, table, ilist, lex, s)) != EXIT_SUCCESS) {free(condition); free(sign); free(sign2);  return result; }
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) {free(condition); free(sign); free(sign2);  return result; }
  *sign2 = ilist->last;
  generateInstruction(I_CLEAR, k_bool, condition, NULL, NULL, ilist);
  generateInstruction(I_CLEAR, k_int, sign, NULL, NULL, ilist);
  generateInstruction(I_CLEAR, k_int, sign2, NULL, NULL, ilist);
  return result;
}

/*   Cyklus while
 * ---------------------------------------------------------------------
 * - zpracuje patricne tokeny a necha probehnout telo cyklu
 * - JAK JSOU RESENE JUMPY??
 */
int embededWhile(struct input * in, btree * table, tListOfInstr * ilist, token * lex, stack * s)
{
  int result = EXIT_SUCCESS;
  printDebug("Cyklus\n");
  bool * condition;
  tListItem ** sign, ** sign2;
  if ((condition = malloc(sizeof(bool))) == NULL) return EXIT_INTERNAL_ERROR;
  if ((sign = malloc(sizeof(tListItem *))) == NULL) return EXIT_INTERNAL_ERROR;
  if ((sign2 = malloc(sizeof(tListItem *))) == NULL) return EXIT_INTERNAL_ERROR;
  // mam "while"
  // vyhodnoceni podminky
  *sign2 = ilist->last;
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS) ||
      ((result = evalExpression(in, table, ilist, lex, NULL, condition, k_bool)) != EXIT_SUCCESS))
    {free(condition); free(sign); free(sign2);  return result; }
  generateInstruction(I_JUMP, k_bool, condition, sign, NULL, ilist);
  // potrebuju "do"
  if (lex->type != l_key && *(key *)lex->data != k_do) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  //nasledovat musi begin
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) {free(condition); free(sign); free(sign2);  return result; }
  if (lex->type != l_key || *(key *)lex->data != k_begin) {free(condition); free(sign); free(sign2); return EXIT_SYNTAX_ERROR;}
  if  ((result = body(in, table, ilist, lex, s)) != EXIT_SUCCESS) {free(condition); free(sign); free(sign2);  return result; }
  // jump back na podminku
  generateInstruction(I_JUMP, k_bool, NULL, sign2, NULL, ilist);
  *sign = ilist->last; // sem skoci pokud while neplati
  if (((result = fillToken(in,lex)) != EXIT_SUCCESS)) {free(condition); free(sign); free(sign2);  return result; }
  generateInstruction(I_CLEAR, k_bool, condition, NULL, NULL, ilist);
  generateInstruction(I_CLEAR, k_int, sign, NULL, NULL, ilist);
  generateInstruction(I_CLEAR, k_int, sign2, NULL, NULL, ilist);
  return result;
}
