#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

#include "hashTable.h"
#include "global.h"

struct chain *hashTableBucket[NOOFBUCKETS];

/*
Function Name: v_initializeHashTable
Description: It initializes the hash table with the required parameters
Parameters: It does not take any parameters
Return Type: It returns void
*/
void v_initializeHashTable(){
		int j;
		
		for(j=0;j<=NOOFBUCKETS;j++){
		hashTableBucket[j]=(struct chain *)malloc(sizeof(struct chain)*1);
                memset((void *)hashTableBucket[j]->c_fileName,'\0',VFS_NAMELEN);
                memset((void *)hashTableBucket[j]->c_filePath,'\0',VFS_FILEPATHLEN);
		hashTableBucket[j]->i_inodeNo=0;
		hashTableBucket[j]->next=NULL;
		}
}

/*
Function Name: v_loadHashTable
Description: It loads the hash table with all the files in the file system using
             the hash function.
Parameters: It takes inode number and fileName as input
Return Type: It return's void
*/
void v_loadHashTable(unsigned int iNode,char *fileArray,char *filePath){

	int index=0;
        int compare_value=0;
	struct chain *newNode;
        struct chain *temp;
	index=i_calculateIndex(fileArray);
	compare_value=strcmp(fileArray,"/");
        if(compare_value==0)
        	return;

	if(hashTableBucket[index]->next==NULL){
	
		newNode=(struct chain *)malloc(sizeof(struct chain));
		strcpy(newNode->c_fileName,fileArray);
		strcpy(newNode->c_filePath,filePath);
		newNode->i_inodeNo=iNode;
		newNode->next=NULL;	
		hashTableBucket[index]->next=newNode;
	}
	else if (hashTableBucket[index]->next!=NULL){
		
		newNode=(struct chain*)malloc(sizeof(struct chain));
		strcpy(newNode->c_fileName,fileArray);
		strcpy(newNode->c_filePath,filePath);
		newNode->i_inodeNo=iNode;
                newNode->next=NULL;
		
		temp=hashTableBucket[index]->next;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=newNode;
			
	}
    

}

int i_calculateIndex(char *fileArray){
			
			char firstChar;
			int index=0;
			
			firstChar=*fileArray;
			firstChar=tolower(firstChar);
			index=toascii(firstChar);
			
			if(index >=97 && index <= 122){
				
					index=index-97;
			}
			else
				index=26;
			return index;
				
}

/*
Function Name: v_delete
Description: It deletes a fileName from hashTable base on unique iNode number and fileName
Parameters: It takes fileName and inode number as parameter
Return Type: It does not return anything
*/


void v_delete(int iNode,char *sendName){

	int index=0;
	int j=0;
	int i=0;
	char fileArray[VFS_NAMELEN];
	
	struct chain *start=NULL;
        struct chain *previous=NULL;
	strcpy(fileArray,sendName);
	index=i_calculateIndex(fileArray);
        previous = hashTableBucket[index];
        start = previous->next;
	if(hashTableBucket[index]->next==NULL){
		
		return;
	}

	

	else{
		
		while(start!=NULL){
			
			
			j=strcmp(start->c_fileName,fileArray);
			if(j==0 && start->i_inodeNo==iNode){
				if(NULL == start->next){
					
					previous->next=NULL;
                                        start->next = NULL;
                                        if( NULL != start ){
					     free(start);
                                        }
					break;
					
				}
				else{
				
				previous->next=start->next;
				start->next = NULL;
                                        if( NULL != start ){
					     free(start);
                                        }
				break;
				}
			}
			else{
				previous = start;
				start=start->next;
				
			}
		}
	
	
	}
}

void printList(){

	struct chain *p;
	int i;
	printf("File Names:\n");
	for(i=0;i<=26;i++)
	{
		p=hashTableBucket[i];
		while(p!=NULL){	
			printf(" %d \t %s and inode %d \t",i,p->c_fileName,p->i_inodeNo);
			p=p->next;
		}
		printf("\n");
	}

}


void v_populateDataStructures(struct nAryTreeNode *root){

    struct nAryTreeNode *temp = NULL;

    temp = root;

    if(NULL == temp ){
         return;
    } else {
         v_populateDataStructures(temp->leftChild);

         v_loadHashTable(temp->s_inode->ui_inodeNo,temp->s_inode->cptr_fileName,temp->s_inode->cptr_filePath);
        
         v_populateDataStructures(temp->rightSibling);

    }
	
}
