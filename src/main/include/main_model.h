/**
 * @file
 *
 * Contains all the global variables inside the project
 *
 * @date Oct 11, 2017
 * @author koldar
 */

#ifndef MAIN_MODEL_H_
#define MAIN_MODEL_H_

#include "model.h"

//TODO rename the file in global_variables.h

/**
 * @defgroup globalVariables Global Variables
 * @brief a collection of all the global variables used in the project
 * @{
 */

/**
 * A structure containing the whole model of application
 *
 * This global variable represents the struct ct_model the framework use to store all its data.
 *
 */
extern struct ct_model* ct_model;

///@}

#endif /* MAIN_MODEL_H_ */
