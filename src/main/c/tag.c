/*
 * tag.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "tag.h"

void destroyTag(Tag* tag) {
	free(tag->name);
	free(tag);
}


