/*
 * yTimerService.c
 *
 *  Created on: Feb 23, 2017
 *      Author: azz
 */

#include "yTimerService.h"
#include "include/eventsList.h"
#include "include/hwTimerManagement.h"

extern eventItem_typedef* eventsList;

void ytsSetTimer(void* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic){
	if(!eventsList) eventsList = elInit();

	elAddItem(eventsList, handle, evid, time_ms, periodic);

	// if added event goes on top of events list we should update interrupt time and all items remaining times as well
	if(eventsList->evid == evid){
		hwTimerStop();
		uint32_t tempTime = hwTimerGetTime();
		hwTimerSetTimer(time_ms);
		hwTimerStart();
		for(uint8_t itm = 1; eventsList[itm].next; itm++){
			eventsList[itm].remainingTime_ms -= tempTime;
		}
	}
}

void ytsUnsetTimer(void* handle, const sc_eventid evid){
	if(eventsList[0].evid == evid){
		hwTimerStop();
		if(eventsList[0].next){
			hwTimerSetTimer(eventsList[0].next->remainingTime_ms);
			hwTimerStart();
		}
	}
	elRemoveByID(eventsList, evid);
}

void ytsTimerCallback(){
	hwTimerStop();
	uint32_t tempTime = hwTimerGetTime();
	for(uint8_t itm = 1; eventsList[itm].next; itm++){
		eventsList[itm].remainingTime_ms -= tempTime;
	}
	if(eventsList[0].periodic){
		eventsList[0].remainingTime_ms = eventsList[0].time_ms;
		elSortByRemainingTime(eventsList);
	} else {
		elRemoveFromTop(eventsList);
	}
}







