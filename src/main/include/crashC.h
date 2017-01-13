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

typedef unsigned int SectionId;

#define MAX_SECTION_DEPTH 10

/**
 * Represents in what level of the hierarchy we're currently in
 *
 * \includedot hierarchy01.dot
 *
 * For example if we're in "when 2" the index should be "1", because:
 * \li test cases contain everything, so their index is 0;
 * \li test cases contain whens, so whens have indexes 1;
 * \li whens contain then, so thens have indexes 2;
 */
static SectionId hierarchyId;

/**
 * Represent which sector of the tests we're currently analyzing
 *
 * the index of the array represents the depth of the test hierarchy.
 *
 * \includedot hierarchy01.dot
 *
 * For example "test cases" have hierarchy id "0", "whens" have id "1" whilst "thens" have id "2".
 * If we're running "then 1", then the array would be {0, 2, 0, ...} because we're running test case 1 (with id 0),
 * when 3 (id 2) and then 1 (id 0).
 *
 * \attention
 * the array has a meaning up until ::hierarchyId
 */
static SectionId currentTestHierarchyState[MAX_SECTION_DEPTH];

/**
 * Represents how many sub section
 *
 * \def a \b container is named block ("{...}") containing some code and optionally some sub sections. For example,
 * ::TESTCASE is a container
 *
 * \def a sub section is a named block ("{...}") inside a container. It can also be a container. For example ::WHEN is sub
 * section but it is also a container. THEN is only a container.
 *
 * We can run only one container of the same hierarchy at a time. If we enter in a container at the hierarchy
 * index "i", we need to get the number of subsections inside the container at index "i+1". By
 * default you can't insert in a container somethin in the hierarchy at index different than "i+1".
 *
 * \includedot hierarchy01.dot
 *
 * For example in the figure if we're in test case 1 (the hierarchy id of "test case" is 0) the value inside "1" should be
 * 3 because inside test case 1 there are 3 "when" clauses. As soon as we go to "test case 2", the cell should be set to 1.
 *
 * \attention
 * the array has a meaning up until ::hierarchyId
 */
static int subSectionNumber[MAX_SECTION_DEPTH];

/**
 * Tells you if we have already computed the number of subsections in the current container
 *
 * \includedot hierarchy01.dot
 *
 * Suppose you've just entered in a test case (eg. test case 1). As soon as you enter, you don't know how many "when"
 * clauses you will have to scan. So in your first execution you need to do 2 things:
 * \li run the first "when" you encounter;
 * \li compute the number of "when" the container have;
 *
 * This variable tells (depending on ::hierarchyId) whether or not we have computed the number
 * of subsection of the container. For example as soon as we enter in "test case 1" the cell indexed by "0" (the hierarchy id
 * of the test cases) will be set to false. After the first loop, the cell will have the value "true".
 *
 * \attention
 * the array has a meaning up until ::hierarchyId
 */
static bool isSubSectionNumberSet[MAX_SECTION_DEPTH];

/**
 * Name of the section we're currently in
 *
 * \def a section is either a container or a subsection
 *
 * \attention
 * the array has a meaning up until ::hierarchyId
 */
static char* currentContainerName[MAX_SECTION_DEPTH];

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

#define SECTIONCONTAINER(id, hierarchyId, total) \
		currentContainerName[hierarchyId] = strdup( #id ); \
		isSubSectionNumberSet[hierarchyId] = false; \
		subSectionNumber[hierarchyId] = 0; \
		for(SectionId i = 0; )

		currentContainerVariable = strdup( #id ); \
		totalNumberOfSubSectionInCurrentContainer[]



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
