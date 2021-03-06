/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:31 PM
 *         Autori:  Tomas Coufal, Roman Halik, Yurij Hladyuk, Jakub Jochlik
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */
#include "scanner.h"

/*   Inicializace tokenu
 * ---------------------------------------------------------------------
 * - inicializuje token: hodnoty jeho parametru a prostor pro jeho data
 */
int tokenInit(token * lex)
{
  if (((lex->data = (string *) malloc(sizeof(string))) == NULL) ||
     ((((string *) lex->data)->str = malloc(sizeof(char) * DETAIL_LENGHT)) == NULL ))
    return EXIT_INTERNAL_ERROR;
  lex->type = l_reset;
  ((string *) lex->data)->str[0] = '\0';
  ((string *) lex->data)->length = 0;
  ((string *) lex->data)->alloc = DETAIL_LENGHT;
  return EXIT_SUCCESS;
}

/*   vraceni pameti allokovane tokenem
 * ---------------------------------------------------------------------
 */
void tokenFree(token *str)
{
  if (str->type != l_int && str->type != l_real && str->type != l_key)
    free(((string *) str->data)->str);
  free(str->data);
}

/*   Vycisteni tokenu
 * ---------------------------------------------------------------------
 * - provede vymazani dat tokenu - vymazani retezce
 */
void tokenClean(token *str)
{
  tokenFree(str);
  tokenInit(str);
  str->type = l_reset;
}

/*   Pridat charu do retezce
 * ---------------------------------------------------------------------
 * - prida znak na konec retezce (data) v tokenu a zvisi pocitadlo delky
 *   retezce s tim, ze soucasne misto inicializuje \0 hodnotou
 */
int addCharToString(token *lex, char z)
{
  if(((string *) lex->data)->length + 1 >= ((string *) lex->data)->alloc)
  {
    if((((string *) lex->data)->str = (char *) realloc(((string *) lex->data)->str, ((string *) lex->data)->length + sizeof(char) * DETAIL_LENGHT)) == NULL)
      return EXIT_INTERNAL_ERROR;
    ((string *) lex->data)->alloc = ((string *) lex->data)->length + DETAIL_LENGHT;
  }
  ((string *) lex->data)->str[((string *) lex->data)->length] = z;
  ((string *) lex->data)->length++;
  ((string *) lex->data)->str[((string *) lex->data)->length] = '\0';
  return EXIT_SUCCESS;
}

/*   Prevod na integer
 * ---------------------------------------------------------------------
 * - pokud bzl nacten integer je treba prevest string ve kterem je
 *   ulozeny na integer
 * - provdede vymenu stringu za integer v lex->data a string dealokuje
 */
int strToInt(token * lex)
{
  string * oldData = lex->data;
  char * tail = oldData->str;
  if ((lex->data = (long int *) malloc (sizeof(long int))) == NULL )
    return EXIT_INTERNAL_ERROR;
  *(((long int *) lex->data)) = strtol(oldData->str, &tail, BASE);
  free(oldData->str);
  free(oldData);
  return EXIT_SUCCESS;
}

/*   Prevod na double
 * ---------------------------------------------------------------------
 * - analogicka funkce k prevodu na integer
 */
int strToDouble(token * lex)
{
  string * oldData = lex->data;
  char * tail = oldData->str;
  if ((lex->data= (double *) malloc (sizeof(double))) == NULL )
    return EXIT_INTERNAL_ERROR;
  *(((double *) lex->data)) = strtod(oldData->str, &tail);
  free(oldData->str);
  free(oldData);
  return EXIT_SUCCESS;
}

char * lower(char * str)
{
  int counter = 0;

  while (str[counter])
  {
    str[counter] = tolower(str[counter]);
    counter++;
  }
  return str;
}


/*   Kontrola klicovych slov
 * ---------------------------------------------------------------------
 * - kontroluje token, pokud je to identifikator, zda-li neni klicovym
 *   slovem, pokud ano, zmeni typ tokenu na l_key
 */
int keyWordCheck(token * lex)
{
  const int KEYWORDS_COUNT =  22;
  const char * KEYWORDS[] ={ "begin", "boolean", "do", "else", "end", "false", "find", "forward", "function", "if", "integer", "readln", "real", "sort", "string", "then", "true", "var", "while", "write", "copy", "length" };
  for ( key i = k_begin; i < KEYWORDS_COUNT; i++)
  {
    if (strcmp(KEYWORDS[i], ((string *) lex->data)->str) == EXIT_SUCCESS)
    {
      lex->type = l_key;
      free(((string *)lex->data)->str);
      free(lex->data);
      if ((lex->data = (key *) malloc(sizeof(key))) == NULL )
        return EXIT_INTERNAL_ERROR;
      *((key *) lex->data) = i;
      return EXIT_SUCCESS;
    }
  }
  return EXIT_SUCCESS;
}

/*   Nacitani tokenu
 * ---------------------------------------------------------------------
 * - funkce zodpovedna za nacteni casti zdrojoveho textu a prevedeni jej
 *   na lexem (token)
 * - rozlisi spravny typ tokenu a pokud zadnemu neodpovida vrati
 *   EXIT_LEXICAL_ERROR
 */
int fillToken(struct input * in, token * lex)
{
  tState state = s_begin;
  bool read = true;
  int z;
  char * expected = "valid symbol";
  int retVal = EXIT_SUCCESS;
  tokenClean(lex);

  while (read)
  {
    z = fgetc(in->file);
    switch(state)
    {
      case s_begin: // zakladni stav automatu, za zaklade pismene rozhodne co by to mohlo byt za lexem
        if (isspace(z)) {}
        else if (isalpha(z) || z == '_') { state = s_id; lex->type = l_id; addCharToString(lex, z); }
        else if (isdigit(z)) { state = s_integer; lex->type = l_int; addCharToString(lex, z); }
        else
        {
          switch (z)
          {
            case EOF: // jediny spravny EOF je pri zacatku lexemu
              lex->type = l_eof; read = false;
            case '{': state = s_comment; break;
            case '\'': state = s_string; break;
            case '.': read = false; lex->type = l_enddot; break;
            case ',': read = false; lex->type = l_sep; break;
            case '+': read = false; lex->type = l_add; break;
            case '-': read = false; lex->type = l_sub; break;
            case '*': read = false; lex->type = l_mul; break;
            case '/': read = false; lex->type = l_div; break;
            case '=': read = false; lex->type = l_equal; break;
            case ';': read = false; lex->type = l_endl; break;
            case '<': state = s_less; break;
            case '>': state = s_greater; break;
            case ':': state = s_colon; break;
            case '(': read = false; lex->type = l_lparenth; break;
            case ')': read = false; lex->type = l_rparenth; break;
            default: read = false; retVal = EXIT_LEXICAL_ERROR;
          }
        }
        break;
      case s_comment: // jsem v komentari
        if (z == '}') state = s_begin;
        else if (z == EOF) { retVal = EXIT_LEXICAL_ERROR; read = false; ungetc(z, in->file); expected = "\"}\""; }
        break;
      case s_id: // nacitam identifikator
        if (isalnum(z) || z == '_') { addCharToString(lex, z); }
        else { read = false; ungetc(z, in->file); }
        break;
      case s_colon: // je to dvojtecka nebo prirazeni
        if ( z == '=') { lex->type = l_assign; }
        else { lex->type = l_colon; ungetc(z, in->file); }
        read = false;
        break;
      case s_less: // rozhoduji < <= <>
        if ( z == '=') { lex->type = l_lequal; }
        else if ( z == '>') { lex->type = l_not; }
        else { lex->type = l_less; ungetc(z, in->file); }
        read = false;
        break;
      case s_greater: // rozhoduji > >=
        if ( z == '=') { lex->type = l_gequal; }
        else { lex->type = l_greater; ungetc(z, in->file); }
        read = false;
        break;
      case s_string: // zpracovani retezce
        if (z == '\'') { state = s_string_check; }
        else if (z == EOF) {read = false; retVal = EXIT_LEXICAL_ERROR; }
        else { addCharToString(lex, z); }
        break;
      case s_string_check: // je to escape sekvence nebo konec?
        if (z == '\'') { state = s_string; addCharToString(lex, z); }
        else if (z =='#' ){ state = s_string_escape; }
        else if (z == EOF) {read = false; retVal = EXIT_LEXICAL_ERROR; }
        else { lex->type = l_str; read = false; ungetc(z, in->file); }
        break;
      case s_string_escape: // u escape sekvence potrebuj alespon jedno cislo
        if (isdigit(z)) { addCharToString(lex, z - '0'); state = s_string_escape_check; }
        else { read = false; retVal = EXIT_LEXICAL_ERROR; expected = "0..9"; }
        break;
      case s_string_escape_check: // zpracovani escape sekvence s ord. hodnotou vetsi nez 9
        if (isdigit(z)) { ((string *) lex->data)->str[((string *) lex->data)->length - 1] = ((string *) lex->data)->str[((string *) lex->data)->length - 1] * 10 + z - '0'; }
        else if (z == '\'') { state = s_string; }
        else { read = false; retVal = EXIT_LEXICAL_ERROR; expected = "0..9 or \' "; }
        break;
      case s_integer: // nacitani integeru
        if (isdigit(z)) { addCharToString(lex, z); }
        else if (z == 'E' || z == 'e') { addCharToString(lex, z); state = s_real_exp; }
        else if (z == '.') { addCharToString(lex, z); state = s_real; }
        else { lex->type = l_int; read = false; ungetc(z, in->file); }
        break;
      case s_real: // integer skoncil teckou, je to tedy realne cislo
        if (isdigit(z)) { addCharToString(lex, z); lex->type = l_real; state = s_real_ok; }
        else { read = false; ungetc(z, in->file); retVal = EXIT_LEXICAL_ERROR; expected = "0..9, real number should not end with . [dot]"; }
        break;
      case s_real_ok: // integer skoncil teckou, je to tedy realne cislo
        if (isdigit(z)) { addCharToString(lex, z); }
        else if(z == 'E' || z == 'e') { addCharToString(lex, z); state = s_real_exp; }
        else { lex->type = l_real; read = false; ungetc(z, in->file); }
        break;
      case s_real_exp: // dostal jsem se k exponentu
        if (isdigit(z)) { addCharToString(lex, z); state = s_real_exp_all; }
        else if (z == '+' || z == '-') { addCharToString(lex, z); state = s_real_exp_ok; }
        else { ungetc(z, in->file); read = false; retVal = EXIT_LEXICAL_ERROR; expected = "0..9 or \"+\" or \"-\""; }
        break;
      case s_real_exp_ok: // je specifikovane znamenko, potrebuji cislici
        if (isdigit(z)) { addCharToString(lex, z); state = s_real_exp_all; }
        else { ungetc(z, in->file); read = false; retVal = EXIT_LEXICAL_ERROR; expected = "0..9";}
        break;
      case s_real_exp_all: // nacitam cislice exponentu
        if (isdigit(z)) { addCharToString(lex, z); }
        else { lex->type = l_real; read = false; ungetc(z, in->file); }
        break;
      default: read = false; retVal = EXIT_INTERNAL_ERROR;
    }
    if (z == '\n' && in->newline == false) { (in->line)++; in->newline = true; }
    else if (in->newline) in->newline = false;
  }
  // doslo-li k problemu behem nacitani vypisu hlaseni
  if (lex->type == l_id) ((string *)lex->data)->str = lower(((string *)lex->data)->str);

  if (retVal == EXIT_LEXICAL_ERROR) { printErr("LEXICAL ERROR on line %d (excluding blank lines): What did you mean by \"%s%c\" ? Expected %s.\n", in->line, ((string *) lex->data)->str, z, expected);}
  if (lex->type == l_int) { strToInt(lex); }
  else if (lex->type == l_real) { strToDouble(lex); }
  else if (lex->type == l_id) { keyWordCheck(lex); }
  return retVal;
}
