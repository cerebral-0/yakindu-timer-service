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
	elAddItem(&eventsList, handle, evid, time_ms, periodic);

	// if added event goes on top of events list we should update interrupt time and all items remaining times as well
	if(eventsList->evid == evid){
		if (eventsList->next){
			hwTimerStop();
			uint32_t tempTime = hwTimerGetTime();
			hwTimerSetTimer(time_ms);
			hwTimerStart();
			elUpdateRemainigTime(&eventsList, hwTimerGetTime());
		} else {
			hwTimerSetTimer(time_ms);
			hwTimerStart();
		}
	}
}

void ytsUnsetTimer(void* handle, const sc_eventid evid){
	hwTimerStop();
	if(eventsList->evid == evid){
		if(eventsList->next){
			uint32_t timerReduceVal = eventsList->remainingTime_ms - hwTimerGetTime();
			elUpdateRemainigTime(&eventsList, timerReduceVal);
			elRemoveFromTop(&eventsList);
			hwTimerSetTimer(eventsList->remainingTime_ms);
			hwTimerStart();
		} else {
			elRemoveFromTop(&eventsList);
		}
	}
	elRemoveByID(eventsList, evid);
	hwTimerStart();
}

void ytsTimerCallback(){
	hwTimerStop();
	elUpdateRemainigTime(&eventsList, eventsList->remainingTime_ms);
	if(eventsList->periodic){
		eventsList->remainingTime_ms = eventsList->time_ms;
		elSortByRemainingTime(&eventsList);

	} else {
		elRemoveFromTop(&eventsList);
	}
	hwTimerSetTimer(eventsList->remainingTime_ms);
	hwTimerStart();

}







