#
#  =====================================================================
#           Verze:  1.0
#       Vytvoreno:  10/17/2014 06:46:48 PM
#          Autors:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
#           Login:  xcoufa09, xhalik01, xhlady00, xjochl00
#         Projekt:  IFJ
#  =====================================================================
#

#
#  =====================================================================
#                               Makefile
#  =====================================================================
#

#    Deklarace promenych
#  ---------------------------------------------------------------------
CC = gcc
CPP = g++
CFLAGS = -std=c99 -pedantic -Wall -Werror

#    Pravidla pro jednotlive programy
#  ---------------------------------------------------------------------
all: ifj14

ifj14: ifj14.c io.o
	$(CC) $(CFLAGS) $^ -o $@

#    Ostatni objekty
#  ---------------------------------------------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

#    Doplnkove funkce
#  ---------------------------------------------------------------------
clean:
	rm -vf *.o *.a *.so ifj14

zip:
	zip ifj14.zip *.c *.h Makefile
