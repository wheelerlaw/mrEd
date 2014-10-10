// Author: Wheeler Law <wpl3499@rit.edu>
// Date: 4/14/14
// Description: This program tests the implementation of the double-linked list

#define _GNU_SOURCE

#include "dlList.h"
#include "dlListExtended.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	int data[100];
	int numData=0;
	
	DlList_T lst=0;
	
	char** args=NULL;
	
	char* buff=NULL;
	size_t len=0;
	size_t read;
	
	printf("Enter a command: (Use 'help' to see a list of commands)\n");
	printf("> ");
	while((read=getline(&buff,&len,stdin))!=-1){
	
		if(read==1){
			printf("> ");
			continue;
		}
		
		// Change newline into NUL.
		char* ptr;
		if((ptr=strchr(buff,'\n'))){
			*ptr='\0';
			read--;
		}
		
		char* tok=0;
		tok=strtok(buff," ");
		int i=0;
		while(tok){
			args=realloc(args,sizeof(char*)*(i+1));
			args[i]=malloc(strlen(tok)+1);
			strcpy(args[i],tok);
			tok=strtok(NULL," ");
			i++;
		}
		
		if(!strcmp(args[0],"append")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(i!=2){
				printf("Arguments for append are: append <data>\n");
			}else{
				data[numData]=atoi(args[1]);
				numData++;
				dll_append(lst,&data[numData-1]);
			}
			
		}else if(!strcmp(args[0],"create")){
			if(lst){
				printf("The list has already been created.\n");
			}else{
				lst=dll_create();
			}
			
		}else if(!strcmp(args[0],"destroy")){
			dll_destroy(lst);
			lst=0;
			printf("The list has been cleared and freed.\n");
			
		}else if(!strcmp(args[0],"clear")){
			if(!lst){
				printf("The list has not been created yet and cannot be cleared.\n");
			}else{
				dll_clear(lst);
			}
			
		}else if(!strcmp(args[0],"moveto")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(i!=2){
				printf("Arguments for moveto are: moveto <index>\n");
			}else{
				int index=atoi(args[1]);
				if(dll_move_to(lst,index)){
					printf("Move was successful.\n");
				}else{
					printf("Move was unsuccessful.\n");
				}
			}
			
		}else if(!strcmp(args[0],"hasnext")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else{
				if(dll_has_next(lst)){
					printf("Position is valid.\n");
				}else{
					printf("Position is not valid\n");
				}
			}
			
		}else if(!strcmp(args[0],"next")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(!dll_size(lst)){
				printf("Can't move. The list is empty\n");
			}else{
				int* data=(int*)dll_next(lst);
				printf("The data in the current element is: %d\n",*data);
			}
			
		}else if(!strcmp(args[0],"prev")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(!dll_size(lst)){
				printf("Can't move. The list is empty\n");
			}else{
				int* data=(int*)dll_prev(lst);
				printf("The data in the current element is: %d\n",*data);
			}
			
		}else if(!strcmp(args[0],"size")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else{
				printf("The size of the list is: %d\n",dll_size(lst));
			}
			
		}else if(!strcmp(args[0],"insertat")){
			int index=atoi(args[1]);
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(i!=3){
				printf("The arguments for insertat are: insertat <index> <data>\n");
			}else if(index>=dll_size(lst)){
				printf("The index is out of range.\n");
			}else{
				data[numData]=atoi(args[2]);
				numData++;
				dll_insert_at(lst,index,&data[numData-1]);
				
				int* data=(int*)dll_get(lst,index);
				printf("The data at index '%d' is '%d'.\n",index,*data);
			}
			
		}else if(!strcmp(args[0],"get")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(i!=2){
				printf("Arguments for get are: get <index>\n");
			}else{
				int index=atoi(args[1]);
				if(dll_move_to(lst,index)){
					int* data=(int*)dll_get(lst,index);
					printf("The data at index '%d' is '%d'.\n",index,*data);
				}else{
					printf("Invalid index: %d\n",index);
				}
			}
			
		}else if(!strcmp(args[0],"set")){
			if(!lst){
				printf("The list has not been created yet.\n");
				
			}else if(i!=3){
				printf("The arguments for set are: set <index> <data>\n");	
			}else{			
				int index=atoi(args[1]);
				if(dll_move_to(lst,index)){
					data[numData]=atoi(args[2]);
					int* oldData=(int*)dll_set(lst,index,&data[numData]);
					*oldData=data[numData];
					dll_set(lst,index,oldData);
					printf("The data at index '%d' is '%d'.\n",index,*(int*)dll_get(lst,index));
					data[numData]=0;
				}else{
					printf("Invalid index: %d\n",index);
				}
			}
			
		}else if(!strcmp(args[0],"pop")){
			if(!lst){
				printf("The list has not been created yet.\n");
				
			}else if(i!=2){
				printf("The arguments for pop are: pop <index>\n");	
			}else{			
				int index=atoi(args[1]);
				if(dll_move_to(lst,index)){
					int* oldData=(int*)dll_pop(lst,index);
					*oldData=0;
					printf("The item at index %d has been removed.\n",index);
				}else{
					printf("Invalid index: %d\n",index);
				}
			}
			
		}else if(!strcmp(args[0],"index")){
			if(!lst){
				printf("The list has not been created yet.\n");
				
			}else if(i!=2){
				printf("The arguments for index are: index <data>\n");
				
			}else{		
				int data1=atoi(args[1]);
				int index;
				if((index=dll_index(lst,memchr(data,data1,100)))!=-1){
					printf("The index of '%d' is at '%d'\n",data1,index);
				}else{
					printf("The data was not found\n");
				}
			}
		}else if(!strcmp(args[0],"empty")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else{
				if(dll_empty(lst)){
					printf("List is empty\n");
				}else{
					printf("List is not empty\n");
				}
			}
			
		}else if(!strcmp(args[0],"cursorpos")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else{
				int index=dll_cursor_pos(lst);
				printf("Cursor position: %d\n",index);
			}
		
		}else if(!strcmp(args[0],"print")){
			if(!lst){
				printf("The list has not been created yet.\n");
			}else if(!dll_size(lst)){
				printf("The list is empty.\n");
			}else{
			
				
				int size=dll_size(lst);
				int cursorpos=dll_cursor_pos(lst);
				printf("Indices followed by data:\n");
				
				for(int i=0;i<size;i++){
					if(i==cursorpos){
						printf("-> ");
					}else{
						printf("   ");
					}
					printf("%d: %d\n",i,*(int*)dll_get(lst,i));
				}
				
				
				
				/* Old code. Made obsolete with the above.
				printf("0");
				for(int i=1;i<size;i++){
					printf("-%d",i);
				}
				
				printf("\n");
				
				
				for(int i=0;i<cursorpos*2;i++){
					printf(" ");
				}
				printf("|\n");
				
				printf("%d",*(int*)dll_get(lst,0));
				for(int i=1;i<size;i++){
					printf("-%d",*(int*)dll_get(lst,i));
				}
				printf("\n");
				*/
			}	
			
		}else if(!strcmp(args[0],"help")){
			printf("The following commands can be executed:\n\n");
			printf("create\n");
			printf("destroy\n");
			printf("clear\n");
			printf("moveto <index>\n");
			printf("hasnext\n");
			printf("next\n");
			printf("prev\n");
			printf("size\n");
			printf("append <data>\n");
			printf("insertat <index> <data>\n");
			printf("get <index>\n");
			printf("set <index> <data>\n");
			printf("pop <index>\n");
			printf("index <data>\n");
			printf("empty\n");
			printf("cursorpos");
			printf("print");
			printf("'exit' and 'quit' are the same.\n");
			
		}else if(!strcmp(args[0],"quit")||!strcmp(args[0],"exit")){
			for(int j=0;j<i;j++){
				free(args[j]);
			}
			free(args);
			args=0;
			break;
		}else{
			printf("Command not recognized. Type 'help' for list of commands.\n");
		}
		
		fflush(stdout);
		for(int j=0;j<i;j++){
			free(args[j]);
		}
		free(args);
		args=0;
		
		printf("> ");
	}
	
	free(buff);
	
}

