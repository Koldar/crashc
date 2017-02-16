///**
// * \file List.h
// *
// * A fat list is slightly more memory wasteful object containing metadata regarding the list
// *
// * A fat list is just a ::forward_list with additional metdata inside it. At a cost of an increase memory consumption
// * (though neglible), some operation (count and tail) are O(1) time.
// * A fat list may not be appropriate if have lots of small lists or if the operation enhanced are not used by you
// *
// *  Created on: Dec 22, 2016
// *      Author: koldar
// */
//
//#ifndef FORWARDLIST_H_
//#define FORWARDLIST_H_
//
//#include <stdbool.h>
//#include <stdlib.h>
//
//#include "forwardList.h"
//
///**
// * A structure representing the list
// */
//typedef struct fat_forward_list {
//	forward_list* list;
//	forward_cell* tail;
//	int size;
//} fat_forward_list;
//
//typedef forward_cell forward_list;
//
///**
// * Creates a new empty list
// */
//fat_forward_list* initFatForwardList();
//
///**
// * remove from the memory every element of the list
// *
// * \attention the pointed elements in the list won't be removed from the memory at all
// *
// * @param[in] list the list to delete
// */
//void destroyFatForwardList(fat_forward_list** list);
///**
// * like ::destroyForwardList(ForwardList*) but it will dete also the pointers
// *
// * @param[inout] list the list to destroy
// * @param[in] d a function that accept an element of the list and removes it from the memory
// */
//void destroyFatForwardListWithElements(fat_forward_list** list, destructor d);
//
///**
// * Adds an element to the head of the list
// *
// * @param[inout] list the list invovled
// * @param[in] pointer the variable to add to the list
// */
//void addHeadInFatForwardList(fat_forward_list** list, const void* pointer);
//
///**
// * Adds an element to the tail of the list
// *
// * @param[inout] list the list invovled
// * @param[in] pointer the variable to add to the list
// */
//void addTailInFatForwardList(fat_forward_list** list, const void* pointer);
//
///**
// * Find an element inside the list
// *
// * \note the list will be scanned in \b linear time
// *
// * @param[in] list the list involved
// * @param[in] c a function accepting an element of the list and return \c true if the element is the one we're looking for.
// * 	See ::Comparator for further information about the prototype
// * @return
// * 	\li the element searched;
// * 	\li null otherwise
// */
//void* findInFatForwardList(fat_forward_list** list, comparator c);
//
///**
// * like ::find but it removes the element from the list once found
// *
// * \note
// * if the element is not found, the function does nothing. If the element is found, the element is reovmed only from the list
// * \b not from the memory
// *
// * \post
// * 	\li the list does not contain the element anymore, however the element remains in the memory
// *
// * \todo make it accept ForwardList**
// *
// * @param[inout] list the list involved
// * @param[in] c the comparator used to search the list. See ::Comparator for further information about the prototype
// * @return true if we have removed something, false otherwise
// */
//bool removeFindInFatForwardList(fat_forward_list** list, comparator c);
//
///**
// * like ::removeFind but it removes the element also from the memory
// *
// * \post
// * 	\li the element in the list is deallocated from the memory
// *
// * \todo make it accept ForwardList**
// *
// * @param[in] list the list involved
// * @param[in] c the comparator used to search the list.See ::Comparator for further information about the prototype
// * @param[in] d the destructor to use to remove the pointer from memory
// * @return true if we have removed something, false otherwise
// */
//bool removeFindWithElementInFatForwardList(fat_forward_list** list, comparator c, destructor d);
//
///**
// * like ::removeFind, but it always return the head of the list
// *
// * \post
// * 	\li after the operation, \c list is automatically updated to refer to the element next to the head of the list, o <b>no next operation needs to be performed!</b>;
// *
// * @param[inout] list whose head has to be removed. The variable pointed will be changed. The double pointer is needed in order to
// * 					actually change the list from the caller, since a list by default refer to the head
// * @return
// * 	\li the head of the list;
// * 	\li NULL if the list is actually empty;
// */
//void* popHeadFromFatForwardList(fat_forward_list** list);
//
///**
// * like ::popHeadFromList, but it doesn't remove the head though
// *
// * @param[in] list whose head has to be removed
// * @return
// * 	\li the head of the list;
// * 	\li NULL if the list is actually empty;
// */
//void* peekHeadFromFatForwardList(const fat_forward_list** list);
//
///**
// * Function used to check if a list is empty or not
// *
// * @param[in] list the list involved
// * @return
// * 	\li true if list is empty;
// * 	\li false otherwise
// */
//bool isFatForwardListEmpty(const fat_forward_list** list);
//
///**
// * get the number of elements inside list
// *
// * @param[in] list the list involved
// * @return number of elements inside \c list
// */
//int getFatForwardListSize(const fat_forward_list** list);
//
///**
// * Get the last element of the list
// *
// * @param[in] list the list involved
// * @return a ::ForwardList structure representing the last element of the \c list; NULL if the list is actually empty
// */
//void* getTailInFatForwardList(const fat_forward_list** list);
//
///**
// * Appends \c src list to the tail of \c dest
// *
// * \post
// * 	\li \c src is not changed whatsoever
// * 	\li \c dest contains all the elements of \c src (by \b reference);
// *
// * \attention
// * If you need to free \c src, be sure not to delete the elements as well
// *
// * @param[in] dest the list whose size will be increased;
// * @param[in] src the list to append to \c dest
// */
//void appendFatForwardListToTail(fat_forward_list** dest, const fat_forward_list** src);
//
///**
// * Removes all the elements in the list
// *
// * \note
// * the elements in the list are simply remove from the list itself, but they remain in the memory
// *
// * \post
// * 	\li list size is 0;
// *
// * @param[inout] toClear the list to remove
// */
//void clearFatForwardList(fat_forward_list** toClear);
//
///**
// * like ::clearForwardList but it also removes the elements from the memory
// *
// * \note
// * the elements in the list are simply remove from the list itself, but they remain in the memory
// *
// * \post
// * 	\li list size is 0;
// *
// * @param[inout] toClear the list to remove
// * @param[in] d a function used to remove the elements of the list from the memory
// */
//void clearFatForwardListWithElements(fat_forward_list** toClear, destructor d);
//
//#endif /* FORWARDLIST_H_ */
