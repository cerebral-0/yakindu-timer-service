/*
 * yTimerService.c
 *
 *  Created on: Feb 23, 2017
 *      Author: azz
 */

#include "yTimerService.h"
#include "eventsList.h"
#include "hwTimerManagement.h"
#include "Yscttmr.h"
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
			if(tempTime) elUpdateRemainigTime(&eventsList, tempTime, false);
		} else {
			hwTimerSetTimer(time_ms);
			hwTimerStart();
		}
	}
}

void ytsUnsetTimer(void* handle, const sc_eventid evid){
	if(!eventsList) return;
	hwTimerStop();
	if(eventsList->evid == evid){
		if(eventsList->next){
			uint32_t timerReduceVal = eventsList->remainingTime_ms - hwTimerGetTime();
			elUpdateRemainigTime(&eventsList, timerReduceVal, false);
			elRemoveFromTop(&eventsList);
			hwTimerSetTimer(eventsList->remainingTime_ms);
			hwTimerStart();
		} else {
			elRemoveFromTop(&eventsList);
		}
	} else {
		elRemoveByID(&eventsList, evid);
		hwTimerStart();
	}
}
//extern yscttmr_raiseTimeEvent(const Yscttmr* handle, sc_eventid evid);

void ytsTimerCallback(){
	if(!eventsList) return;
	if(eventsList->next){
		hwTimerStop();
		yscttmr_raiseTimeEvent(eventsList->handle, eventsList->evid);

		uint32_t timeToDecrease = eventsList->remainingTime_ms;
		elUpdateRemainigTime(&eventsList, eventsList->remainingTime_ms, true);
		if(eventsList->periodic){
			eventsList->remainingTime_ms = eventsList->time_ms;
			elSortByRemainingTime(&eventsList);
		} else {
			elRemoveFromTop(&eventsList);
		}
		hwTimerSetTimer(eventsList->remainingTime_ms);
		hwTimerStart();

	} else {
		yscttmr_raiseTimeEvent(eventsList->handle, eventsList->evid);
		if(eventsList->periodic){
			eventsList->remainingTime_ms = eventsList->time_ms;
			hwTimerSetTimer(eventsList->remainingTime_ms);
			return;
		} else {
			hwTimerStop();
			elRemoveFromTop(&eventsList);
		}
	}
}




