/*
 * eventsList.c
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#include "../include/eventsList.h"
#include "../yTimerService.h"

eventItem_typedef pool[MAX_EVENTS];
eventItem_typedef* allocated[MAX_EVENTS];

eventItem_typedef* elInit(){
	return pool[0];
}

eventItem_typedef* elGetBlockFromPool(eventItem_typedef* pool){
	eventItem_typedef* retVal = (eventItem_typedef*)(void*)0;
	for(uint8_t itm = 0; itm < MAX_EVENTS; itm++){
		if(!pool[itm].allocated){
			retVal = pool[itm];
			pool[itm].allocated = true;
		}
	}
	return retVal;
}

void elReturnBlockToPool(eventItem_typedef* pool, eventItem_typedef* item){
	for(uint8_t itm = 0; itm < MAX_EVENTS; itm++){
		if(pool[itm] == item){
			pool[itm].allocated = false;
		}
	}
}

void elAddItem(eventItem_typedef* head, void* smHandle, sc_eventid evid, sc_integer time_ms, sc_boolean periodic){
	eventItem_typedef* tempEvent = elGetBlockFromPool(pool);
	tempEvent->evid = evid;
	tempEvent->handle = smHandle;
	tempEvent->periodic = periodic;
	tempEvent->remainingTime_ms = time_ms;
	tempEvent->time_ms = time_ms;
	tempEvent->next = (eventItem_typedef*)0;
	eventItem_typedef* tempHead = head;
	while(tempHead->next){
		tempHead = tempHead->next;
	}
	tempHead->next = tempEvent;
	elSortByRemainingTime(head);
}

void elRemoveByID(eventItem_typedef* head, sc_eventid evid){
	eventItem_typedef* tempHead = head;
	while(tempHead->next && (tempHead->evid != evid)){
		tempHead = tempHead->next;
	}



}
