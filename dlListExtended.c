// Author: Wheeler Law
// Date: 4/16/14
// Description: Extended functionality of the double-linked list implementation.

#include <stdlib.h>
#include <stdio.h>

typedef struct dlListStruct{
	struct dlListStruct* next;
	struct dlListStruct* prev;
	struct dlListStruct* cursor;
	struct dlListStruct* top;
	void* data;
	int index;
	signed int size;
	
}* DlList_T;

#define _DLL_IMPL_
#include "dlList.h"
#include "dlListExtended.h"


/** dll_cursor_pos Returns the index of the node at which the cursor is
 ** pointing to.
 ** @param lst: the subject list. 
 ** @return 0-based index of the index of the cursor.
 ** @post The cursor does not change position, data is not changed.
 **/
int dll_cursor_pos(DlList_T lst){
	return lst->cursor->index;
}

DlList_T dll_create2(DlList_T prev, DlList_T next, DlList_T top, int index, void* data){
	DlList_T newNode=malloc(sizeof(struct dlListStruct));
	
	newNode->next=next;
	newNode->prev=prev;
	newNode->cursor=NULL;
	newNode->top=top;
	newNode->data=data;
	newNode->index=index;
	
	return newNode;
	
}
	