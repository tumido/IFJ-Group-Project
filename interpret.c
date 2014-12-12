/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:21:07 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "interpret.h"

int interpret(tListOfInstr * ilist)
{
  tListItem * listItem = ilist->first;
  tInstr * i;
  while(listItem != NULL)
  {
    printDebug("zpracovavam instrukci\n");
    i = &listItem->Instruction;
    switch(i->instType)
    {
      case I_READ:
        switch(i->type)
        {
          case k_int:
            scanf("%d", (int *)i->addr3);
            break;
          case k_real:
            scanf("%lf", (double *)i->addr3);
            break;
          case k_string:
            if (((string *)i->addr3)->str != NULL) free(((string *)i->addr3)->str);
            unsigned int size = 8, length = 0;
            char ch;
            if ((((string *)i->addr3)->str = malloc(sizeof(char) * size)) == NULL)
              return EXIT_INTERNAL_ERROR;
            while (EOF != (ch = getchar()) && ch != '\n')
            {
              ((string *)i->addr3)->str[length++] = ch;
              if (length == size)
              {
                if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, sizeof(char) * (size += 8))) == NULL)
                  return EXIT_INTERNAL_ERROR;
              }
            }
            ((string *)i->addr3)->str[length++] = '\0';
            if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, sizeof(char) * length)) == NULL)
               return EXIT_INTERNAL_ERROR;
            ((string *)i->addr3)->alloc = length;
            ((string *)i->addr3)->length = length;
            break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_WRITE:
        switch(i->type)
        {
          case k_int:
            printf("%d", *(int *)i->addr1);
          case k_real:
            printf("%g", *(double *)i->addr1);
          case k_string:
            printf("%s", ((string *)i->addr1)->str);
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_JUMP: // instrukce skoku, jen se nastavim na zadanou v adrese (ve volani si beru addr1 = ilist->last)
        listItem = i->addr1;
        break;
      case I_ASSIGN: // isntukce prirazeni, jedina komplikace je string (kopiruju retezec, musim realloc)
        switch(i->type)
        {
          case k_int:
            *(int *)i->addr3 = *(int *)i->addr1;
            break;
          case k_real:
            *(double *)i->addr3 = *(double *)i->addr1;
            break;
          case k_string:
            if (((string *)i->addr3)->alloc < ((string *)i->addr1)->alloc)
            {
              ((string *)i->addr3)->alloc = ((string *)i->addr1)->alloc;
              if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, sizeof(char) * ((string *)i->addr1)->alloc)) == NULL)
                return EXIT_INTERNAL_ERROR;
            }
            strncpy(((string *)i->addr3)->str, ((string *)i->addr1)->str, ((string *)i->addr3)->alloc);
            ((string *)i->addr3)->length = ((string *)i->addr1)->length;
            break;
          case k_bool:
            *(bool *)i->addr3 = *(bool *)i->addr1;
            break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_CALL_FUNCTION: // volani funkce (zaradim instrukce funkce jako nasledujici instrukci a za posledni instrukci fukce zaradim nasledujici instrukci (co by naseldovala v ilistu))
        ((tListOfInstr *)i->addr1)->last = listItem->nextItem;
        listItem = ((tListOfInstr *)i->addr1)->first;
        break;
      case I_CLEAR: // free nad docasnou hodnotou (u stringu jeste samotny string)
        if (i->type == k_string) free(((string *)i->addr1)->str);
        free(i->addr1);
        break;
      case I_MUL: // jak resit int * real?
        if (i->type == k_int) *(int *)i->addr3 = *(int *)i->addr1 * *(int *)i->addr2;
        else if (i->type == k_real) *(double *)i->addr3 = *(double *)i->addr1 * *(double *)i->addr2;
        else return EXIT_RUNTIME_ERROR;
        break;
      case I_DIV:
        if (i->type == k_int) *(int *)i->addr3 = *(int *)i->addr1 / *(int *)i->addr2;
        else if (i->type == k_real) *(double *)i->addr3 = *(double *)i->addr1 / *(double *)i->addr2;
        else return EXIT_RUNTIME_ERROR;
        break;
      case I_ADD:
        if (i->type == k_int) *(int *)i->addr3 = *(int *)i->addr1 +*(int *)i->addr2;
        else if (i->type == k_real) *(double *)i->addr3 = *(double *)i->addr1 + *(double *)i->addr2;
        else if (i->type == k_string)
        {
          if (((string *)i->addr3)->str != NULL) free(((string *)i->addr3)->str);
          if ((((string *)i->addr3)->str = malloc(sizeof(char) * (((string *)i->addr1)->alloc + ((string *)i->addr2)->alloc))) == NULL)
            return EXIT_INTERNAL_ERROR;
          strncpy(((string *)i->addr3)->str, ((string *)i->addr1)->str, ((string *)i->addr1)->length - 1);
          strncat(((string *)i->addr3)->str, ((string *)i->addr1)->str, ((string *)i->addr1)->length);
        }
        else return EXIT_RUNTIME_ERROR;
        break;
      case I_SUB:
        if(i->type == k_int) *(int *)i->addr3 = *(int *)i->addr1 - *(int *)i->addr2;
        else if(i->type == k_real) *(double *)i->addr3 = *(double *)i->addr1 - *(double *)i->addr2;
        else return EXIT_RUNTIME_ERROR;
        break;
//      case I_CON: // neni to nahodou totez v ADD ??

//      case I_INC:
//      case I_DEC:
//      case I_NEG: // v zadani jsem je nenasel, jsou tam teda?
      case I_LESS:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 < *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 < *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) < EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 < *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_GREATER:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 > *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 > *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) > EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 > *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_LESS_EQUAL:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 <= *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 <= *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) <= EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 <= *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_GREATER_EQUAL:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 >= *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 >= *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) >= EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 >= *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_EQUAL:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 == *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 == *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) == EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 == *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_NOT_EQUAL:
        switch(i->type)
        {
          case k_int: *(bool *)i->addr3 = *(int *)i->addr1 != *(int *)i->addr2; break;
          case k_real: *(bool *)i->addr3 = *(double *)i->addr1 != *(double *)i->addr2; break;
          case k_string: *(bool *)i->addr3 = (strcmp(((string *)i->addr1)->str, ((string *)i->addr2)->str) != EXIT_SUCCESS); break;
          case k_bool: *(bool *)i->addr3 = *(bool *)i->addr1 != *(bool *)i->addr2; break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_COPY:
//      case I_LENGHT: // neni potreba resi se assignem
      case I_FIND:
      case I_SORT:
        if (((string *)i->addr1)->length <= 1) return EXIT_INTERNAL_ERROR; //zkopirovat string
        else
        {
          //alloc na cilovy string
          if (((string *)i->addr3)->str != NULL) free(((string *)i->addr3)->str);
          if ((((string *)i->addr3)->str = malloc(sizeof(char) * ((string *)i->addr1)->alloc)) == NULL)
            return EXIT_INTERNAL_ERROR;

          // zkopiruju string a seradim ho
          ((string *)i->addr3)->length = ((string *)i->addr1)->length;
          strcpy(((string *)i->addr3)->str, ((string *)i->addr1)->str);
//          shellSort(((string *)i->addr3)->str, ((string *)i->addr3)->length);
        }
      default:
        return EXIT_INTERNAL_ERROR;
    }
    printDebug("nacitam dalsi instrukci\n");
    listItem = listItem->nextItem;
  }
  return EXIT_SUCCESS;
}
