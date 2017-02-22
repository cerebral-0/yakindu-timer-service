/*
 * eventsList.h
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#ifndef INCLUDE_EVENTSLIST_H_
#define INCLUDE_EVENTSLIST_H_

#include "sc_types.h"

typedef struct tsEventItem;

typedef struct tsEventItem {
	void* handle;
	void* evid;
	uint32_t time_ms;
	uint32_t remainingTime_ms;
	bool periodic;
	bool allocated;
	eventItem_typedef* next;
} eventItem_typedef;

extern eventItem_typedef* pool;

eventItem_typedef* elInit();
void elAddItem(eventItem_typedef* head, void* smHandle, sc_eventid evid, sc_integer time_ms, sc_boolean periodic);
void elRemoveByID(eventItem_typedef* head, sc_eventid evid);
void elSortByRemainingTime(eventItem_typedef* head);

eventItem_typedef* elGetBlockFromPool(eventItem_typedef* pool);
void elReturnBlockToPool(eventItem_typedef* pool, eventItem_typedef* item);

#endif /* INCLUDE_EVENTSLIST_H_ */
