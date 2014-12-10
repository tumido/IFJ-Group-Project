IFJ project
===========

Overview
--------
This project aims to create Pascal compiler and interpreter. Well not all funcionality of Pascal - just a subset of it's features.
For detail informations about this project see https://www.fit.vutbr.cz/study/courses/IFJ/public/project/ .

GIT
---
Soucasny stav = Pull Req + vlastni Fork
Navrh zjednoduseni = na punk, Kuba style (muzete smazat forky -> nastavit git origin na moje repo a normalne commit do nej, pripadne merge)
muzeme k tomu udelat nejakou videokonferenci / vncviewer (?)

Konvence
--------
aneb to, na cem jsme se dohodli a sere se na to:

- odsazovani dvema mezerama
- komentare nad kazdou funkci (viz. io.c) v jednotnem formatu a s popisem parametru a vlastnosti (co funkce dela?)
- pojmenovavani funkci a promennych - anglicky: randomFunction() (ne Cammel_Back() ani nic dalsiho)
- ukazka

```
/*   Funkce uplne na houby
 * -----------------------------------------------------
 * - ma parametry, ale nic nedela, to je co?
 */
void ThisIsFunction(int parameterOne, ...)
{
  for(int i = 0; i < 10; i++)
  {
    ...
  }
  return; // vraci houby, fakt!
}
```

Progress
--------

- Kuba napsal lexikalni analyzu, ted ho bude cekat interpret
- Tom nenapsal to co mel, vysral se na to, do zitra to dopise, jinak bude lyncovani + za odmenu semanticka analyza, revize Kubova kodu (jsou to spagety)
- Roman se pusti do psani ukazkovych programu, testsuite + upravi main, aby kod nacetl a vypsal tokeny
- Jura napise syntaktickou analyzu

Konzultace
----------
Bylo by fajn, domluvit si konzultaci a projit s nekym povolanym to. co uz mame a jestli je to OK (tak za tyden, to uz by mela byt vetsina veci funkcni)

Dalsi schuzka
-------------
Patek?
