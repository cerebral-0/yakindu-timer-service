/*
 * eventsList.h
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#ifndef INCLUDE_EVENTSLIST_H_
#define INCLUDE_EVENTSLIST_H_

#include "sc_types.h"

#define NULL (void*)0

//typedef struct tsEventItem;

typedef struct tsEventItem {
	void* handle;
	void* evid;
	uint32_t time_ms;
	uint32_t remainingTime_ms;
	bool periodic;
	bool allocated;
	struct tsEventItem* next;
} eventItem_typedef;

//extern eventItem_typedef pool[MAX_EVENTS];

void elAddItem(eventItem_typedef** eventsList, void* smHandle, sc_eventid evid, sc_integer time_ms, sc_boolean periodic);
void elRemoveByID(eventItem_typedef** eventsList, sc_eventid evid);
void elRemoveFromTop(eventItem_typedef** eventsList);
void elSortByRemainingTime(eventItem_typedef** eventsList);
void elUpdateRemainigTime(eventItem_typedef** eventsList, uint32_t time, bool include1st);

static eventItem_typedef* elGetBlockFromPool(eventItem_typedef* pool);
static void elReturnBlockToPool(eventItem_typedef* pool, eventItem_typedef* item);



#endif /* INCLUDE_EVENTSLIST_H_ */
