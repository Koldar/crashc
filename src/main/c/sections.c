/*
 * sections.c
 *
 *  Created on: Jan 14, 2017
 *      Author: koldar
 */

#include "sections.h"

SectionTree* initSectionTree() {
	return initSection(0, 0);
}

Section* initSection(HierarchyId hierarchyId, SectionId sectionId);

Section* destroySection(Section* section);
