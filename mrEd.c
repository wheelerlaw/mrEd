// Author: Wheeler Law
// Date: 4/14/14
// Description: mrEd: a simple, line by like file editor.

#define _GNU_SOURCE

#include "dlList.h"
#include "dlListExtended.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void appendLine(char** command, DlList_T lst);
void printLine(char** command, DlList_T lst);
void printNextLine(char** command, DlList_T lst);
void printPrevLine(char** command, DlList_T lst);
void printLastLine(char** command, DlList_T lst);
void printLineN(char** command, DlList_T lst);
void deleteLine(char** command, DlList_T lst);
void insertLine(char** command, DlList_T lst);
void printLineNum(char** command, DlList_T lst);
void printLastLineNum(char** command, DlList_T lst);
void printAllLines(char** command, DlList_T lst);
void write(char** command, DlList_T lst);
void quit(char** command, DlList_T lst);
void forceQuit(char** command, DlList_T lst);

int main(int argc, char** argv){
	if(argc!=1&&argc!=2){
		printf("Usage: mrEd <filename>\n");
		exit(EXIT_FAILURE);
	}
	
	FILE* fp;
	DlList_T lst=dll_create();
	if(argc==2){
		if(!(fp=fopen(argv[1],"a+"))){
			int errsv=errno;
			if(errsv==13){
				fprintf(stderr,"could not read file %s\n",__FILE__);
				exit(EXIT_FAILURE);
			}else{
				fprintf(stderr,"Error: %d\n",errsv);
				exit(EXIT_FAILURE);
			}
		}
		
		char* buff=0;
		size_t len=0;
		size_t read;
		size_t count=-1;
		
		while((read=getline(&buff,&len,fp))!=-1){
			// Change newline into NUL.
			char* ptr;
			if((ptr=strchr(buff,'\n'))){
				*ptr='\0';
				read--;
			}
			
			char* line=malloc(sizeof(char)*(read+1));
			strcpy(line,buff);
			
			/* Thought the lines had to be editable, so I needed
			 * a way to store a line in an editable way. What
			 * better way to implement this by using a linked list
			 * of linked lists, with the nested list containing
			 * nodes of the individual characters, and the outer
			 * list containing the nodes of the inner lists. 
			 * Not needed anymore. 
			
			DlList_T line=dll_create();
			
			for(int i=0;i<read;i++){
				char* store=malloc(sizeof(char));
				*store=buff[i];
				dll_append(line,store);
			}
			*/
			
			dll_append(lst,line);
			count++;
		}
		free(buff);
		count++;
		
		if(count==-1){
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		
		fclose(fp);
	}else{
		printf("no file supplied\n");
	}
	
	/* This was to print out the list of lists. Not needed anymore.
	for(int i=0;i<dll_size(lst);i++){
		DlList_T line=dll_get(lst,i);
		int size=dll_size(line);
		for(int j=0;j<size;j++){
			printf("%c",*(char*)dll_get(line,j));
		}
		printf("\n");
	}
	*/
	
	/*
	for(int i=0;i<dll_size(lst);i++){
		printf("%s\n",(char*)dll_get(lst,i));
	}
	*/
	
	char** command=0;
	char* buff=0;
	size_t len=0;
	size_t read;
	
	struct option{
		char ch;
		void (*process)(char**, DlList_T);
	};
	
	struct option optionTable[]={
		{'a',appendLine},{'.',printLine},{'+',printNextLine},
		{'-',printPrevLine},{'$',printLastLine},{'N',printLineN},
		{'d',deleteLine},{'i',insertLine},{'p',printAllLines},
		{'w',write},{'q',quit},{'Q',forceQuit}
	};
	
	int optionTableSize=sizeof(optionTable)/sizeof(struct option);
	
	while((read=getline(&buff,&len,stdin))!=-1){
		char* ptr;
		if((ptr=strchr(buff,'\n'))){
			*ptr='\0';
			read--;
		}
		
		char* tok=0;
		tok=strtok(buff," ");
		int i=0;
		while(tok){
			command=realloc(command,sizeof(char*)*(i+1));
			command[i]=malloc(strlen(tok)+1);
			strcpy(command[i],tok);
			tok=strtok(NULL," ");
			i++;
		}
		if(read){
			char* ptr=0;
			strtol(command[0],&ptr,10);
			if(*ptr=='\0'){
				optionTable[5].process(command,lst);
			}
			for(int i=0;i<optionTableSize;i++){
				if(optionTable[i].ch==command[0][0]){
					optionTable[i].process(command,lst);
				}
			}
		}else{
			printNextLine(command,lst);
		}
		
		for(int j=0;j<i;j++){
			free(command[j]);
		}
		free(command);
		command=0;
	}
	free(buff);
	free(command);
	command=0;
	
	dll_destroy(lst);
	
}

void appendLine(char** command, DlList_T lst){
	char* buff=0;
	size_t len=0;
	size_t read;
	size_t count=0;
	
	
	while((read=getline(&buff,&len,stdin))!=-1){
		char* ptr;
		if((ptr=strchr(buff,'\n'))){
			*ptr='\0';
			read--;
		}
		
		if(!strcmp(".",buff)){
			break;
		}
		
		char* line=malloc(sizeof(char)*(read+1));
		strcpy(line,buff);
		
		dll_append(lst,line);
		count++;
	}
	free(buff);
}

void printLine(char** command, DlList_T lst){
	if(command[0][1]=='='){
		printLineNum(command,lst);
	}else{
		int index=dll_cursor_pos(lst);
		if(index<0||index>=dll_size(lst)){
			printf("?\n");
		}else{
			printf("%s\n",(char*)dll_get(lst,index));
		}
	}
}

void printNextLine(char** command, DlList_T lst){
	int index=dll_cursor_pos(lst);
	if(index>=dll_size(lst)-1){
		printf("?\n");
		return;
	}
	dll_move_to(lst,index+1);
	printf("%s\n",(char*)dll_get(lst,index+1));
}

void printPrevLine(char** command, DlList_T lst){
	int index=dll_cursor_pos(lst);
	if(index<=0){
		printf("?\n");
		return;
	}
	dll_move_to(lst,index-1);
	printf("%s\n",(char*)dll_get(lst,index-1));
}

void printLastLine(char** command, DlList_T lst){
	if(command[0][1]=='='){
		printLineNum(command,lst);
	}else{
		printf("%s\n",(char*)dll_get(lst,dll_size(lst)-1));
	}
}

void printLineN(char** command, DlList_T lst){
	int line=atoi(command[0]);
	if(line<1||line>dll_size(lst)){
		printf("?\n");
		return;
	}
	dll_move_to(lst,line-1);
	printf("%s\n",(char*)dll_get(lst,line-1));
}

void deleteLine(char** command, DlList_T lst){
	int index=dll_cursor_pos(lst);
	if(index<0||index>=dll_size(lst)){
		printf("?\n");
		return;
	}
	free(dll_pop(lst,index));
}

void insertLine(char** command, DlList_T lst){
	char* buff=0;
	size_t len=0;
	size_t read;
	size_t count=0;
	
	int index=dll_cursor_pos(lst);
	
	while((read=getline(&buff,&len,stdin))!=-1){
		char* ptr;
		if((ptr=strchr(buff,'\n'))){
			*ptr='\0';
			read--;
		}
		
		if(!strcmp(".",buff)){
			break;
		}
		
		char* line=malloc(sizeof(char)*(read+1));
		strcpy(line,buff);
		
		dll_insert_at(lst,index++,line);
		count++;
	}
	free(buff);
}

void printLineNum(char** command, DlList_T lst){
	printf("%d\n",dll_cursor_pos(lst)+1);
}

void printLastLineNum(char** command, DlList_T lst){
	printf("%d\n",dll_size(lst));
}

void printAllLines(char** command, DlList_T lst){
	if(!dll_size(lst)){
		printf("?\n");
		return;
	}
	for(int i=0;i<dll_size(lst);i++){
		dll_move_to(lst,i);
		printf("%s\n",(char*)dll_get(lst,i));
	}
}

void write(char** command, DlList_T lst){
	FILE* fp=0;
	if(!(fp=fopen(command[1],"a+"))){
		int errsv=errno;
		if(errsv==13){
			fprintf(stderr,"could not read file %s\n",__FILE__);
			exit(EXIT_FAILURE);
		}else{
			fprintf(stderr,"Error: %d\n",errsv);
			exit(EXIT_FAILURE);
		}
	}
	
	for(int i=0;i<dll_size(lst);i++){
		fprintf(fp,"%s\n",(char*)dll_get(lst,i));
	}
}

void quit(char** command, DlList_T lst){
	//TODO: quit but warn
	printf("Quit but warn\n");
}

void forceQuit(char** command, DlList_T lst){
	printf("Bye\n");
	exit(EXIT_SUCCESS);
}
