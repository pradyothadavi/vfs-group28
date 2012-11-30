#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"nAryTree.h"
#include"global.h"
#include"vfs_errorcodes.h"
#include "binarySearchTree.h"
#include "hashTable.h"

int i_deletedir(char *P1){

	struct nAryTreeNode *temp=sPtr_rootNAryTree;
	struct nAryTreeNode *prev= NULL;
	struct binarySearchTree *node=NULL;
	char *result=NULL;
	char *P2=(char *)malloc(sizeof(char)*strlen(P1));

if(sPtr_rootNAryTree==NULL||sPtr_rootNAryTree->leftChild==NULL)
	{
	return 0;
	}	
	strcpy(P2,P1);

	if(*P1==0)
	{
	printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_00);

	return 0;
	}

	if(temp->leftChild->rightSibling==NULL){
/*		printf("there are no directories to be deleted");*/
		printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_01);

		return 0;
	} else {
		result=strtok(P1,"/");

		while(result!=NULL){

			prev=temp;
			temp=temp->leftChild;
			if(temp==NULL)
			{
/*			printf("invalid path");*/
			printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_01);

			return 0;
			}
			while((temp->rightSibling!=NULL)&&(!((strcmp(temp->s_inode->cptr_fileName,result)==0)&&(strcmp(temp->s_inode->c_fileType,"d")==0)))){
				prev=temp;
				temp=temp->rightSibling;
			}
			if(!((strcmp(temp->s_inode->cptr_fileName,result)==0)&&
                            (strcmp(temp->s_inode->c_fileType,"d")==0))){
				printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_01);

				return 0;
			}
			/*call strtok and retrieve the next substring;*/
			result=strtok(NULL,"/");
		}


		if((temp->leftChild==NULL)&&(temp!=sPtr_rootNAryTree)){

			if(temp==temp->parent->leftChild){
				temp->parent->leftChild=temp->rightSibling;
			
			} 
			else  {
				prev->rightSibling = temp->rightSibling;
			}

			v_delete(temp->s_inode->ui_inodeNo,temp->s_inode->cptr_fileName);

			node=s_searchBSTNode(sPtr_rootBST,P2+1);
			if(node!=NULL)
			{
			v_deleteBSTNode(sPtr_rootBST,node);
			}

			s_inodeBlockFreeList=s_addFreeBlock(s_inodeBlockFreeList,temp->s_inode->ui_inodeNo);
			s_dataBlockFreeList=s_addFreeBlock(s_dataBlockFreeList,temp->s_inode->ui_locationDataBlockNo[0]);
		free(temp);

		} else {
			printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_02);
			return 0;

		}
	}
        


    return 1;
}


