// Author: Wheeler Law
// Date: 4/13/14
// Description: Double-linked list implementation of dlList.h.

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

/// dll_create constructs an instance of an empty double-linked list.
/// @return instance of double-linked list (this is a pointer).
DlList_T dll_create( void ){
	DlList_T dlList=malloc(sizeof(struct dlListStruct));
	
	dlList->next=dll_create2(dlList,NULL,dlList,0,NULL);
	dlList->prev=NULL;
	dlList->cursor=dlList;
	dlList->top=dlList;
	dlList->data=NULL;
	dlList->index=-1;
	dlList->size=0;
	
	return dlList;
}	

/// dll_destroy clears and frees all storage associated with
/// the linked list data structure.
/// The function handles memory management of all data payloads.
/// @param lst the linked list to destroy.
/// @post lst is no longer usable; the function freed all its memory.
void dll_destroy( DlList_T lst ){
	if(!lst){
		return;
	}
	dll_destroy(lst->next);
	free(lst);
	lst=0;
}

/// dll_clear clears the list content, making the list empty.
/// dll_clear assumes all data payloads are dynamic and frees their storage.
/// @param lst the linked list to clear.
/// @post lst is now in an empty state.
void dll_clear( DlList_T lst ){
	if(!lst){
		return;
	}	
	lst->size=0;
	lst->cursor=lst;
	dll_destroy(lst->next);
	
	lst->next=dll_create2(lst,NULL,lst,0,NULL);
}

/// dll_move_to moves the cursor to the requested index, if index is valid.
/// @param lst the list whose cursor should move.
/// @param indx the 0-based index to which to move the cursor.
/// @return true if the cursor move was successful.
bool dll_move_to( DlList_T lst, int indx ){
	
	if(indx<0||indx>=dll_size(lst)){
		return false;
	}
	while(lst->cursor){
		if(lst->cursor->index==indx){
			break;
		}
		else if(lst->cursor->index>indx){
			lst->cursor=lst->cursor->prev;
		}else if(lst->cursor->index<indx){
			lst->cursor=lst->cursor->next;
		}
	}
	return true;

}

/// dll_has_next returns whether or not the cursor refers to a valid position.
/// @param lst the list to check.
/// @return non-zero if the cursor refers to a valid current position.
int dll_has_next( DlList_T lst ){
	if(!lst->cursor){
		return 0;
	}else{
		return 1;
	}
}

/// dll_next returns the current item and advance forward to next item.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the list to iterate over.
/// @return the data pointer of the current item.
/// @pre the cursor refers to a valid current position.
/// @post the cursor refers to the position after the current one, if any.
void * dll_next( DlList_T lst ){
	if((lst->cursor->index==lst->size)||(lst->cursor->index==-1)){
		fprintf(stderr,"Invalid cursor location.\n");
		exit(EXIT_FAILURE);
	}
	
	void* data=lst->cursor->data;
	lst->cursor=lst->cursor->next;
	return data;
}

/// dll_prev returns the current item and advance backward to previous item.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the list to iterate over.
/// @return the data pointer of the current item.
/// @pre the cursor refers to a valid current position.
/// @post the cursor refers to the position before the current one, if any.
void * dll_prev( DlList_T lst ){
	if((lst->cursor->index==lst->size)||(lst->cursor->index==-1)){
		fprintf(stderr,"Invalid cursor location.\n");
		exit(EXIT_FAILURE);
	}
	void* data=lst->cursor->data;
	lst->cursor=lst->cursor->prev;
	return data;
}

/// dll_size returns the size of the list.
/// @param lst the subject list.
/// @return the count of items in the list.
int dll_size( DlList_T lst ){
	return lst->size;
}

/// dll_append appends an item to the end of the list.
/// The function assumes ownership of the memory of the data payload.
/// @param lst the subject list.
/// @param data a pointer to the item to append.
/// @post cursor moves to refer to the appended item. the lst size grows by 1.
void dll_append( DlList_T lst, void *data ){
	while(lst->cursor->next->next){
		lst->cursor=lst->next;
	}
	
	DlList_T next=lst->cursor->next;
	lst->cursor->next=dll_create2(lst->cursor,next,lst,lst->cursor->index+1,data);
	lst->size++;
	dll_move_to(lst,lst->size-1);
	
	// Increase the index of the empty bottom element. 
	lst->cursor->next->index++;
	
	// Set the bottom element to point to the last data element.
	lst->cursor->next->prev=lst->cursor;
	
	return;
}

/// dll_insert_at inserts an item at the requested position, if index is valid.
/// The client is responsible for memory management of the item's storage.
/// The function assumes ownership of the memory of the data payload.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @param data a pointer to the item to append.
/// @pre indx must be in the range [0...dll_size(lst) ).
/// @post if successful, cursor moves to position of inserted item.
/// @post if successful, the lst size grows by 1.
void dll_insert_at( DlList_T lst, int indx, void *data ){
	if(!dll_move_to(lst,indx)){
		fprintf(stderr,"Invalid index at %s:%d.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	
	// The cursor is at the index at which the new node will go, so we must
	// go back one to change it.
	lst->cursor=lst->cursor->prev;
	
	lst->cursor->next=dll_create2(lst->cursor,lst->cursor->next,lst,indx,data);
	lst->size++;
	dll_move_to(lst,indx);
	
	// Set the bottom element to point to the last data element.
	lst->cursor->next->prev=lst->cursor;
	
	// Increase the indices of all the following nodes.
	DlList_T temp=lst->cursor->next;
	while(temp){
		temp->index++;
		temp=temp->next;
	}
}

/// dll_get returns a pointer to the item at index; item remains in list.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @return pointer to the item requested.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post cursor position does not change. lst size and content is unchanged.
void * dll_get( DlList_T lst, int indx ){

	if(indx<0||indx>=lst->size){
		fprintf(stderr,"Invalid index at %s:%d.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	DlList_T temp=lst->next;
	
	while(temp->index!=indx){
		temp=temp->next;
	}
	return temp->data;
}

/// dll_set replaces the pointer to the item with the data value.
/// The function transfers ownership of the old memory to the client.
/// The client is responsible for memory management of the old, returned data.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @param data a pointer to the item to set, replacing the existing entry.
/// @return pointer to the item that was replaced.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post data is the value of the list at the position of the index.
void * dll_set( DlList_T lst, int indx, void *data ){
	if(!dll_move_to(lst,indx)){
		fprintf(stderr,"Invalid index at %s:%d.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	
	void* oldData=lst->cursor->data;
	lst->cursor->data=data;
	return oldData;
	
}
	

/// dll_pop removes the item at the index and returns its data pointer.
/// The function transfers ownership of the old memory to the client.
/// The client is responsible for the memory management of the data removed.
/// The cursor moves to the next position, if present. Otherwise the cursor
/// moves to the previous position in the list, if present; Otherwise the
/// cursor is invalid since this function deleted the last line in the list.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @return pointer to the item removed.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post cursor moves to adjacent position or is invalid if list is now empty.
void * dll_pop( DlList_T lst, int indx ){
	if(!dll_move_to(lst,indx)){
		fprintf(stderr,"Invalid index at %s:%d.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	
	if(lst->size==lst->next->index){
		fprintf(stderr,"List is empty. Cannot pop.\n");
	}
	
	DlList_T toBePopped=lst->cursor;
	void* popData=toBePopped->data;
	
	lst->cursor=lst->cursor->prev;
	
	lst->cursor->next=lst->cursor->next->next;
	lst->cursor->next->prev=lst->cursor;
	
	free(toBePopped);
	
	lst->size--;
	
	DlList_T temp=lst->cursor->next;
	
	// Decrease the indices of all the following nodes.
	while(temp){
		temp->index--;
		temp=temp->next;
	}
	
	
	return popData;
}

/// dll_index returns a 0-based index of the data in the list or -1 if absent.
/// @param lst the subject list.
/// @param data a pointer to the item to find.
/// @return 0-based index of the data in the list or -1 if absent.
/// @post cursor position does not change. lst size and content is unchanged.
int dll_index( DlList_T lst, void *data ){
	DlList_T temp=lst->next;
	while(temp->index!=lst->size){
		if(temp->data==data){
			return temp->index;
		}else{
			temp=temp->next;
		}
	}
	
	return -1;
}

/// dll_empty checks for an empty list.
/// @param lst: the subject list.
/// @return true if the list is empty.
/// @post cursor position does not change. lst size and content is unchanged.
bool dll_empty( DlList_T lst ){
	if(lst->size){
		return true;
	}else{
		return false;
	}
}




