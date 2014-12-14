#
#  =====================================================================
#           Verze:  1.0
#       Vytvoreno:  10/17/2014 06:46:48 PM
#          Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
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
CC = clang #gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror -g
ARCHIVE = xcoufa09
LIBS = io.o scanner.o ilist.o parser.o btree.o parstack.o constructs.o interpret.o stack.o ial.o

#    Pravidla pro jednotlive programy
#  ---------------------------------------------------------------------
all: ifj14

ifj14: main.c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

#    Ostatni objekty
#  ---------------------------------------------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

#    Doplnkove funkce
#  ---------------------------------------------------------------------
clean:
	rm -vf *.o *.a *.so *.zip ifj14

zip:
	zip $(ARCHIVE).zip *.c *.h rozdeleni Makefile
