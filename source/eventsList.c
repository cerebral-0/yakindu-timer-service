/*
 * eventsList.c
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#include "eventsList.h"
#include "yTimerService.h"

static volatile eventItem_typedef pool[MAX_EVENTS];
extern yscttmr_raiseTimeEvent(const Yscttmr* handle, sc_eventid evid);
eventItem_typedef* eventsList = NULL;

static bool poolFirstAlloc = true;

eventItem_typedef* elGetBlockFromPool(eventItem_typedef* pool){
    if(poolFirstAlloc){
        memset(pool, 0, sizeof(eventItem_typedef)*MAX_EVENTS);
        poolFirstAlloc = false;
    }
    eventItem_typedef* retVal = (eventItem_typedef*)NULL;
    for(uint8_t itm = 0; itm < MAX_EVENTS; itm++){

        if(pool[itm].allocated == false){
            retVal = &(pool[itm]);
            pool[itm].allocated = true;
            break;
        }
    }
    return retVal;
}

void elReturnBlockToPool(eventItem_typedef* pool, eventItem_typedef* item){
    item->allocated = false;
    item->evid = 0;
    item->handle = 0;
    item->next = NULL;
    item->remainingTime_ms = 0;
    item->time_ms = 0;
}

void elAddItem(eventItem_typedef** eventsList, void* smHandle, sc_eventid evid, sc_integer time_ms, sc_boolean periodic){

    eventItem_typedef* tempEvent = elGetBlockFromPool(pool);

    if(tempEvent == NULL)
    {
        while(1);
    }

    tempEvent->evid = evid;
    tempEvent->handle = smHandle;
    tempEvent->periodic = periodic;
    tempEvent->remainingTime_ms = time_ms;
    tempEvent->time_ms = time_ms;
    tempEvent->next = (eventItem_typedef*)NULL;

    if(*eventsList == NULL){
        *eventsList = tempEvent;
        return;
    }

    eventItem_typedef* tempHead = *eventsList;

    while(tempHead->next){
        tempHead = tempHead->next;
    }
    tempHead->next = tempEvent;

    elSortByRemainingTime(eventsList);
}

void elRemoveByID(eventItem_typedef** eventsList, sc_eventid evid){
    if((*eventsList)->next == NULL){
    	if((*eventsList)->evid == evid){
    		elRemoveFromTop(eventsList);
			return;
    	} else return;
    }

    eventItem_typedef* eventPrev = *eventsList;
    eventItem_typedef* event = eventPrev->next;

    while(event->next && (event->evid != evid)){
        eventPrev = event;
        event = event->next;
    }
    if (event->evid == evid){
    	eventPrev->next = event->next;
		elReturnBlockToPool(pool, event);
    } else return;
}

void elRemoveFromTop(eventItem_typedef** eventsList){
    if(!(*eventsList)) return;
    elUpdateRemainigTime(eventsList, (*(*eventsList)).remainingTime_ms, false);
    eventItem_typedef* toDelete = *eventsList;
    *eventsList = (*eventsList)->next;
    elReturnBlockToPool(pool, toDelete);

}

void elUpdateRemainigTime(eventItem_typedef** eventsList, uint32_t time, bool include1st){
    eventItem_typedef* iter;
    if(include1st){
        iter = *eventsList;
    } else iter = (*eventsList)->next;
    if(!iter) return;
    while(iter->next){
        iter->remainingTime_ms -= time;
        if((iter->remainingTime_ms > iter->time_ms)||(iter->remainingTime_ms == 0)){
            //asm("nop"); //call event handler
        	yscttmr_raiseTimeEvent(iter->handle, iter->evid);
            elRemoveByID(eventsList, iter->evid);
        }
        iter = iter->next;
    }
    iter->remainingTime_ms -= time;
    if((iter->remainingTime_ms > iter->time_ms)||(iter->remainingTime_ms == 0)){
        //asm("nop"); //TODO: call event handler
    	yscttmr_raiseTimeEvent(iter->handle, iter->evid);
        elRemoveByID(eventsList, iter->evid);
    }
}

eventItem_typedef *mergeSort(eventItem_typedef *head);

void elSortByRemainingTime(eventItem_typedef** eventsList){
    *eventsList = mergeSort(*eventsList);
}

static eventItem_typedef* bisectList(eventItem_typedef* head)
{

    eventItem_typedef *fast = head, *slow = head, *prev = NULL;

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    if (prev != NULL)
    {
        prev->next = NULL;
    }
    return slow;
}

static eventItem_typedef* mergeList(eventItem_typedef *list1, eventItem_typedef *list2)
{
    eventItem_typedef dummy_head = { 0, NULL }, *tail = &dummy_head;

    while ( (list1 != NULL) && (list2 != NULL) )
    {
        eventItem_typedef **min = (list1->remainingTime_ms < list2->remainingTime_ms) ? &list1 : &list2;
        eventItem_typedef *next = (*min)->next;
        tail = tail->next = *min;
        *min = next;
    }
    tail->next = list1 ? list1 : list2;
    return dummy_head.next;
}

eventItem_typedef *mergeSort(eventItem_typedef *head)
{
    eventItem_typedef *list1 = head;
    if ( (list1 == NULL) || (list1->next == NULL) )
    {
        return list1;
    }
    eventItem_typedef *list2 = bisectList(list1);
    return mergeList( mergeSort(list1), mergeSort(list2) );
}
































