/*
 * yTimerService.h
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#ifndef YTIMERSERVICE_H_
#define YTIMERSERVICE_H_

#include "sc_types.h"

#define MAX_EVENTS 50

void ytsSetTimer(void* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic);
void ytsUnsetTimer(void* handle, const sc_eventid evid);
extern void ytsRaiseTimeEvent(const void* handle, const sc_eventid evid);

#endif /* YTIMERSERVICE_H_ */
