/*
 * a_star.c
 *
 *  Created on: Aug 1, 2017
 *      Author: koldar
 */

#include "macros.h"
#include <stdlib.h>
#include <limits.h>
#include "a_star.h"

astar_support* initAStarSupport(int maxOpenListSize, int maxCloseListSize, cost_function cost, heuristic_function heuristic, successor_function successors, destructor stateDestructor) {
	astar_support* retVal = malloc(sizeof(astar_support));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	//FIXME continue here
	retVal->openQueue = initQueue(heuristic, true, stateDestructor);
	retVal->closeList = initList();
	retVal->costComputer = cost;
	retVal->heuristicComputer = heuristic;
	retVal->successorsComputer = successors;

	return retVal;
}

void destroyAStarSupport(astar_support* support) {

}

/**
 * Run a search with A\*
 *
 * @param[in] startState the start state where we need to start our search
 * @param[in] goalState the state we want to reach
 * @return
 * 	A sequence of
 */
path* runSearchWithAStar(astar_support* support, void* startState, void* goalState) {
	void* cameFrom = NULL;

	clearList(support->closeList);
	clearQueue(support->openQueue);

	addItemInQueue(support->openQueue, startState);

	while (!isQueueEmpty(support->openQueue)) {
		void* currentState = popItemFromQueue(support->openQueue, void*);
		addTailInList(support->closeList, currentState);

		list* successors = support->successorsComputer(currentState);
		VARIABLE_ITERATE_ON_LIST(successors, prevCell, cell, childState, void) {
			int cost = support->costComputer(currentState, NULL) + support->costComputer(currentState, childState);


			bool isChildInOpen = containsItemInQueue(support->openQueue, childState);
			//if (isChildInOpen && cost <

			removeElementInListCell(successors, &prevCell, cell);
		}

	}

	int goalScore = INT_MAX;


}

