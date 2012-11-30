#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

#include "hashTable.h"
#include "fileOps.h"
#include "vfs_errorcodes.h"
#include "global.h"


/*
Function Name: searchHashTable
Description: It searches the hashTable based on the fileName only
Parameters: It takes fileName as parameter
Return Type: It returns a linked list of files with the name to be searched
*/

int i_searchfile(char *p1,char *p2){
	FILE *f;
	char fileArray[VFS_NAMELEN];
        int index = 0;
	int i=1,j=0,count=0;
	struct chain *p=NULL;
	struct chain *head=NULL;
        strcpy(fileArray,p1);
	index=i_calculateIndex(fileArray);
	p=hashTableBucket[index]->next;
	
	if(p==NULL){
		count=0;
	}
	else{
	
	
	
	f=fopen(p2,"w+");
	if(f==NULL){
		
		return count;

	}
	else{
	
		if(hashTableBucket[index]->next==NULL){
			return count;	
		
		
		}
		else{
		
		
			while(p!=NULL){
				for(j=0;j<strlen(fileArray);j++){
					if(fileArray[j]!=p->c_fileName[j]){
						i=0;
						break;
					}
       					else{
						i=1;
					} 
				}
				if(i==1){
					
					
					head=addToList(head,p->c_filePath);
					p=p->next;
				
				}
				else
					p=p->next;
			}
		
	
		}
	}	
	
	while(head!=NULL){
			fprintf(f,"%s\n",head->c_filePath);
			head=head->next;
			count++;

		}
	}
		/*if(NULL!=f){
			fclose(f);
		}*/
	return count;	
}
/*
Function Name: addToList
Description: It adds the file to the linked list that has matched the prefix
Parameters: It takes head of linked list and fileName as input
Return Type: It returns a linked list of files with the prefix to be searched to search module
*/

struct chain *addToList(struct chain *head,char *fileArray){
	struct chain *newNode;
	newNode=(struct chain*)malloc(sizeof(struct chain)*1);
        memset((void *)newNode->c_fileName,'\0',VFS_NAMELEN);
        memset((void *)newNode->c_filePath,'\0',VFS_FILEPATHLEN);
	
	strcpy(newNode->c_filePath,fileArray);
	
        newNode->i_inodeNo = 0;
	newNode->next = NULL;
	if(head==NULL){	
			head=newNode;
			
					
	}
	else{

		newNode->next=head;
		head=newNode;
		
	}
	return head;
}
