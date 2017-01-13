/*
 * crashC.h
 *
 *  Created on: Jan 4, 2017
 *      Author: koldar
 */

#ifndef CRASHC_H_
#define CRASHC_H_




#define STR(x) #x
#define ARGSTR(x) STR(x)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static char* currentTestCase = NULL;
static char* currentWhen = NULL;
static char* currentThen = NULL;

static int whenCasesInCurrentTest = 0;
static int whenCaseUnderAnalysis = 0;
static bool totalNumberOfWhenCasesInCurrentTestCase = false;

/*#define SECTOR(id, varToAssign) \
	END_LABEL_ ## id: ; \
	START_LABEL_ ## id: varToAssign = strdup( #id );

#define TESTCASE(id) SECTOR(id, currentTestCase)
#define WHEN(id) SECTOR(id, currentWhen)
#define THEN(id) SECTOR(id, currentThen)*/


/**
	- devo ciclare sullo stesso test case in modo da eseguire tutte le combinazioni di sottosezioni (dirette ed indirette) all'interno della sezione.
	- il ciclo for mi serve perché così posso eseguire istruzioni alla fine di ogni iterazione;
	- all'inizio fisso
 */
#define TESTCASE(id) \
	currentTestCase = strdup( #id ); \
	totalNumberOfWhenCasesInCurrentTestCase = false; \
	whenCasesInCurrentTest = 0; \
	for (int i=0; (whenCaseUnderAnalysis = -1)&&((!totalNumberOfWhenCasesInCurrentTestCase) || (i<whenCasesInCurrentTest));totalNumberOfWhenCasesInCurrentTestCase = true,i++)

/**
	- se devo ancora calcolare il numero totale di sotto sezioni, incremento il numero di sottosezioni fino ad ora individuate
	- se devo ancora calcolare il numero totale di sotto sezioni e questa è la prima che incontro la eseguo perché nel primo ciclo dovrò eseguire la prima
		(mentre le altre verranno evitate)
	- se conosco il numero totali di sotto sezioni eseguo soltanto la n°esima: whenCaseUnderAnalysis dice qual'è il numero della sottosezione
*/
#define WHEN(id) \
	whenCaseUnderAnalysis++; \
	if (!totalNumberOfWhenCasesInCurrentTestCase) { \
		whenCasesInCurrentTest++; \
	} \
	if ((i == whenCaseUnderAnalysis))


//i when devono contenere un qualcosa che permette ci capire quanti sono
//il ciclo for mi permette di eseguire istruzioni alla fine del "{" e del "}"

int main(int argc, const char* argv[]) {

	TESTCASE(testcase) {
		printf("before when1\n");
		WHEN(when1) {
			printf("when1\n");
		}
		printf("when1 -> when2\n");
		WHEN(when2) {
			printf("when2\n");
		}
		printf("when2 -> when3\n");
		WHEN(when3) {
			printf("when3\n");
		}
		printf("after when3\n");
	}

	return 0;
}

#endif /* CRASHC_H_ */
