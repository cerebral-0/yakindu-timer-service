/*
 * hwTimerManagement.h
 *
 *  Created on: Feb 23, 2017
 *      Author: azz
 */

#ifndef INCLUDE_HWTIMERMANAGEMENT_H_
#define INCLUDE_HWTIMERMANAGEMENT_H_

void hwTimerStart();
void hwTimerStop();
void hwTimerSetTimer(uint32_t time);
uint32_t hwTimerGetTime();
void hwTimerInterruptCallback();

#endif /* INCLUDE_HWTIMERMANAGEMENT_H_ */
