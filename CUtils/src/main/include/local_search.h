/*
 * \file local_search.h
 *
 * Contains some abstract concepts about search algorithms
 *
 * A state is, in general, a <tt>void*</tt> value. Those values aren't marked as
 * **const** because maybe the function themselves alter some metadata of the state.
 * Since I don't want to add any constraints to the implementations, I create a general prototype.
 *
 *  Created on: Aug 1, 2017
 *      Author: koldar
 */

#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

#include "list.h"

/**
 * a type for any cost function
 *
 * A cost function accepts in input a particular state and it computes
 * the cost of reaching that state from where we are
 *
 * @param[in] startState where we have started from
 * @param[in] currentState the state we're currently into and that we need to evaluate
 * @return int the cost of passing from \c startState to \c currentState
 */
typedef int (*cost_function)(void* startState, void* currentState);
/**
 * a type for any heuristic function
 *
 * An heuristic function is a function estimating how much cost to go
 * from a particular state till the goal
 *
 * @param[in] currentState the state we're currently into and that we need to evaluate
 * @param[in] goalState the state we're aiming to go
 * @return the estimate of going from \c currentState till \c goalState
 */
typedef int (*heuristic_function)(void* currentState, void* goalState);

/**
 * A type for computing successors of a state
 *
 * Any kind of function statisfying this protocol will compute the successors of the given state
 *
 * @param[in] state the state whose successors we need to compute
 * @return a list fo successors of \c state
 */
typedef list* (*successor_function)(void* state);



#endif /* LOCAL_SEARCH_H_ */
