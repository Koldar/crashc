/*
 * a_star.h
 *
 *  Created on: Aug 1, 2017
 *      Author: koldar
 */

#ifndef A_STAR_H_
#define A_STAR_H_

#include "local_search.h"
#include "list.h"
#include "queue.h"

typedef list path;

typedef struct {
	queue* openQueue;
	int maxOpenListSize;
	list* closeList;
	int maxCloseListSize;
	cost_function costComputer;
	heuristic_function heuristicComputer;
	successor_function successorsComputer;
} astar_support;


astar_support* initAStarSupport(int maxOpenListSize, int maxCloseListSize, cost_function cost, heuristic_function heuristic, successor_function successors, destructor stateDestructor);

void destroyAStarSupport(astar_support* support);

/**
 * Run a search with A\*
 *
 * @param[in] startState the start state where we need to start our search
 * @param[in] goalState the state we want to reach
 * @return
 * 	A sequence of
 */
path* runSearchWithAStar(astar_support* support, void* startState, void* goalState);


#endif /* A_STAR_H_ */
