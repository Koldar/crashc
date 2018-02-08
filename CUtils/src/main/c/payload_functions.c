/*
 * payload_functions.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */


#include "payload_functions.h"
#include "defaultFunctions.h"

payload_functions getDefaultPayloadFunctions() {
	return (payload_functions){
		defaultPayloadDestructor,
		defaultPayloadCloner,
		defaultBufferString,
		defaultOrderer,
		defaultSerializer,
		defaultDeserializer
	};
}

payload_functions getIntPayloadFunctions() {
	return (payload_functions){
		payloadDestructorNOP,
		defaultPayloadCloner,
		getBufferStringOfInteger,
		integerValueOrderer,
		serializerInt,
		deserializerInt
	};
}
