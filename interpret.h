/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  10/17/2014 06:10:50 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */


/*
 * Klasifikace datového typu
 */
int returnValue(tVarValue *destination, tVarValue *source);

/*
 * Interpretace konkretni instrukce
 */
int instruction(tInst *);

/*
 * Hlavni ridici funkce intepretu, spousti a ridi cinnost pomocnych funkci
 */
int interpret(tInstList *);

/*
 * Funkce na kontrolu typu integer a real v operandech 3AK
 */
bool isIntOrReal(void);