/*
 * payload_functions.h
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#ifndef PAYLOAD_FUNCTIONS_H_
#define PAYLOAD_FUNCTIONS_H_

#include "typedefs.h"

/**
 * A structure each container should have.
 * Such structure contains several methods to control how the payload can be managed.
 *
 * For example if you need to get a string represenrtation of a container, you should have a string representation
 * method of the payload as well. This structure contains all the methods you might need to achieve this.
 *
 * It's more flexible than to feed those methods inside the init() because here you don't have to alter the parameters of the method at all.
 * For example assume you have:
 *
 * ```
 * initList(destructor d, cloner c);
 * ```
 *
 * Then you need a ::buffer_string as well. With the current `initList()` you need to change every reference to:
 *
 * ```
 * initList(myDestructor, myCloner) //before
 * initList(myDestructor, myCloner, myBufferString) //after
 * ```
 *
 */
typedef struct {
	destructor destroy;
	cloner clone;
	buffer_string bufferString;
	orderer order;
	object_serializer serialize;
	object_deserializer deserialize;
} payload_functions;

/**
 * The default payload functions
 */
payload_functions getDefaultPayloadFunctions();

/**
 * functions tuned when the paylaod is actually an integer
 */
payload_functions getIntPayloadFunctions();

#endif /* PAYLOAD_FUNCTIONS_H_ */
