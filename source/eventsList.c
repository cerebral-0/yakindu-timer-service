/*
 * eventsList.c
 *
 *  Created on: Feb 22, 2017
 *      Author: azz
 */

#include "../include/eventsList.h"
#include "../yTimerService.h"

eventItem_typedef pool[MAX_EVENTS];
eventItem_typedef* eventsList = NULL;

eventItem_typedef* elInit(){
	return &(pool[0]);
}

eventItem_typedef* elGetBlockFromPool(eventItem_typedef* pool){
	eventItem_typedef* retVal = (eventItem_typedef*)(void*)0;
	for(uint8_t itm = 0; itm < MAX_EVENTS; itm++){
		if(!pool[itm].allocated){
			retVal = &(pool[itm]);
			pool[itm].allocated = true;
		}
	}
	return retVal;
}

void elReturnBlockToPool(eventItem_typedef* pool, eventItem_typedef* item){
	item->allocated = false;
	item->evid = 0;
	item->handle = 0;
	item->next = (void*)0;
	item->remainingTime_ms = 0;
	item->time_ms = 0;
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
	eventItem_typedef* event = head;
	while(event->next && (event->evid != evid)){
		event = event->next;
	}
	if(event){
		event->allocated = false;
	}
	if(event == head){
		head = head->next;
	}
}
void elRemoveFromTop(eventItem_typedef* list){
	eventItem_typedef* toDelete = list;
	list = list->next;
	elReturnBlockToPool(pool, toDelete);

}

static eventItem_typedef* bisectList(eventItem_typedef* head)
{
    /* The fast pointer moves twice as fast as the slow pointer. */
    /* The prev pointer points to the node preceding the slow pointer. */
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

//    printf("\nList1 = "); printList(list1);
//    printf("\nList2 = "); printList(list2);

    return mergeList( mergeSort(list1), mergeSort(list2) );
}
































