/*
 * sections.h
 *
 *  Created on: Jan 13, 2017
 *      Author: koldar
 */

#ifndef SECTIONS_H_
#define SECTIONS_H_

#include <stdbool.h>

typedef int HierarchyId;

typedef struct Section {
	/**
	 * Represent what is this section
	 *
	 * "test case", "when", "then"?
	 */
	HierarchyId hierarchyId;
	/**
	 * The id of this section related to its sibling
	 *
	 * For example this is the third "when" clauses inside the second test case
	 */
	SectionId siblingId;
	const char* name;
	bool isSubSectionNumberSet;
	int subSectionNumber;
	SectionId subSectionToAnalyze;
	struct Section* currentSubSection;
	struct Section* container;
	/**
	 * Pointer to the first sub section inside this section.
	 *
	 * ::Section::hierarchyId of the sub section will be greater than this id by 1.
	 * NULL if there is no sub section
	 */
	struct Section* firstSubSection;
	/**
	 * Pointer to the next sibling
	 *
	 * The sibling has the same ::Section::hierarchyId. Note that this is a list. NULL if there
	 * is no next sibling
	 */
	struct Section* nextSubling;
} Section;

typedef Section SectionTree;

Section* initSection(Section* container, HierarchyId hierarchyId, SectionId sectionId);

Section* destroySection(Section* section);

SectionTree* initSectionTree();

Section* addChildToSection(Section* section, const char* name);


#endif /* SECTIONS_H_ */
