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
    i = &listItem->Instruction;
    printDebug("zpracovavam instrukci typu %d\n", i->instType);
    switch(i->instType)
    {
      case I_READ:
        switch(i->type)
        {
          unsigned int size, length;
          char ch;
          case k_int:
            if ( scanf("%d", (int *)i->addr3) != 1) return EXIT_READ_STDIN_ERROR;
            break;
          case k_real:
            if (scanf("%lf", (double *)i->addr3) != 1) return EXIT_READ_STDIN_ERROR;
            break;
          case k_string:
            size = 8; length = 0;
            if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, sizeof(char) * size)) == NULL)
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
            ((string *)i->addr3)->str[length] = '\0';
            if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, sizeof(char) * length)) == NULL)
               return EXIT_INTERNAL_ERROR;
            ((string *)i->addr3)->alloc = length;
            ((string *)i->addr3)->length = length;
            break;
          case k_bool:
            return EXIT_TYPE_ERROR;
          default:
            printDebug("%d", i->type); return EXIT_READ_STDIN_ERROR;
        }
        printDebug("Nacteno ze vstupu.\n");
        break;
      case I_WRITE:
        switch(i->type)
        {
          case k_int:
            printf("%d", *(int *)i->addr1); break;
          case k_real:
            printf("%g", *(double *)i->addr1); break;
          case k_string:
            printf("%s", ((string *)i->addr1)->str); break;
          default:
            return EXIT_TYPE_ERROR;
        }
        break;
      case I_JUMP: // instrukce skoku, jen se nastavim na zadanou v adrese (ve volani si beru addr1 = ilist->last)
        if (i->addr1 == NULL || !(*(bool *)i->addr1)) listItem = *(tListItem **)i->addr2;
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
        ((tListOfInstr *)i->addr1)->last->nextItem = listItem->nextItem;
        listItem->nextItem = ((tListOfInstr *)i->addr1)->first;
        break;
      case I_CLEAR: // free nad docasnou hodnotou (u stringu jeste samotny string) -> provede se az pri uklidu na konci, kvuli jumpum...
        //if (i->type == k_string) free(((string *)i->addr1)->str);
        //free(i->addr1);
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
        if (((string *)i->addr1)->length < (*((struct srange *)i->addr2)->length + *((struct srange *)i->addr2)->start)) return EXIT_RUNTIME_ERROR;
        if ((((string *)i->addr3)->str = realloc(((string *)i->addr3)->str, *((struct srange *)i->addr2)->length * sizeof(char))) == NULL)
          return EXIT_INTERNAL_ERROR;
        strncpy(&((string *)i->addr3)->str[*((struct srange *)i->addr2)->start], ((string *)i->addr1)->str, *((struct srange *)i->addr2)->length);
        ((string *)i->addr3)->length = *((struct srange *)i->addr2)->length;
        break;
      case I_FIND:
        if(!strlen(((string *)i->addr2)->str) || !strlen(((string *)i->addr1)->str) || 
          ((strlen(((string *)i->addr1)->str)) < (strlen(((string *)i->addr2)->str)))) return EXIT_RUNTIME_ERROR;
        *(int *)i->addr3 = findSubString(((string *)i->addr2)->str, ((string *)i->addr1)->str);
        break;
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
          shellSort(((string *)i->addr3)->str, ((string *)i->addr3)->length);
        }
        break;
      case I_TABLE_BACKUP:
        stackPUSH((stack *)i->addr3, (btree *)i->addr1);
        break;
      case I_TABLE_RESTORE:
        stackPOP((stack *)i->addr3, (btree *)i->addr1, (char *)i->addr2);
        break;
      default:
        return EXIT_INTERNAL_ERROR;
    }
    listItem = listItem->nextItem;
  }
  return EXIT_SUCCESS;
}
