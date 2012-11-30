#include<stdio.h>
#include<string.h>
#include"nAryTree.h"
#include"global.h"
#include"vfs_errorcodes.h"
#include "directoryOps.h"

int i_movedir(char *P1,char *P2){
	/*request for root of n ary tree and store it in temp and temp1*/
	/*P1=source dir path P2=dest dir path*/
	/*move to source dir*/
	/*assign the links to dest dir*/

	struct nAryTreeNode *temp=NULL;
	struct nAryTreeNode *tempprob=NULL;
	struct binarySearchTree *node=NULL;
	struct nAryTreeNode *templf=sPtr_rootNAryTree->leftChild;
	struct nAryTreeNode *temp1=NULL;
	struct nAryTreeNode *temp2=NULL;
	struct nAryTreeNode *temp4=NULL;
	struct nAryTreeNode *prev1=NULL;
	struct nAryTreeNode *prev=NULL;
	char *P3,*result,*result1,*P4,*res,*res1;
	int flags=0,flagd=0;
	
	
if(sPtr_rootNAryTree==NULL||sPtr_rootNAryTree->leftChild==NULL)
	{
	return 0;
	}
	
	sPtr_rootNAryTree->leftChild=templf->rightSibling;
	templf->rightSibling=NULL;
	templf->parent=NULL;
	
		if(*P1==0||*P2==0)
		{
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_00);
		templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
		return 0;
		}

	temp=(struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
	temp->s_inode=(struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);
	temp=sPtr_rootNAryTree;
	temp1=sPtr_rootNAryTree;
	P4=(char *)malloc(sizeof(char)*strlen(P1));
	strcpy(P4,P2);
		if(temp->leftChild==NULL)/*if N ary tree is empty*/
		{
/*		printf("there are no directories to be moved");*/
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_01);
		templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
		return 0;
		}



		else
		{
		result=strtok(P1,"/");
			while(result!=NULL)/*points to the directory to be moved(source)*/
			{
			prev=temp;
			temp=temp->leftChild;
			if(temp==NULL)
			{
/*			printf("dir does not exist");*/
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_01);
				templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
				return 0;

			}
				while((temp->rightSibling!=NULL)&&(!((strcmp(temp->s_inode->cptr_fileName,result)==0)&&(strcmp(temp->s_inode->c_fileType,"d")==0))))
				{
				prev=temp;
				temp=temp->rightSibling;
				}
				if(!((strcmp(temp->s_inode->cptr_fileName,result)==0)&&(strcmp(temp->s_inode->c_fileType,"d")==0)))
				{
				printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_01);
				templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
				return 0;
				}
				/*call strtok and retrieve the next substring;*/
			res=result;
			result=strtok(NULL,"/");
			}
		result1=strtok(P2,"/");
			while(result1!=NULL)/*points to the directory to which it is to be moved(destination)*/
			{
			prev1=temp1;
			temp1=temp1->leftChild;
			if(temp1==NULL)
			{
/*			printf("dir does not exist");*/
			
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_02);
			templf->rightSibling=sPtr_rootNAryTree->leftChild;
			sPtr_rootNAryTree->leftChild=templf;
			templf->parent=sPtr_rootNAryTree;
			return 0;

			}

				while((temp1->rightSibling!=NULL)&&(!((strcmp(temp1->s_inode->cptr_fileName,result1)==0)&&(strcmp(temp1->s_inode->c_fileType,"d")==0))))
				{
				prev1=temp1;
				temp1=temp1->rightSibling;
				}

				if(!((strcmp(temp1->s_inode->cptr_fileName,result1)==0)&&(strcmp(temp1->s_inode->c_fileType,"d")==0)))
				{
if(strcmp(temp1->s_inode->c_fileType,"d")==0)
			{
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_02);
			}
			else
			{
			if(strcmp(temp1->s_inode->cptr_fileName,result1)==0)
			{
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_07);
			}
			else
			{
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_02);			
			}
			}

				/*printf("destination dir does not exist");*/
				templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
		return 0;
				}
			/*call strtok and retrieve the next substring;*/
			res1=result1;
			result1=strtok(NULL,"/");
			}

		}

		if(temp1==temp)
		{
		/*printf("a directory cannot be moved to itself");*/
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_06);
		templf->rightSibling=sPtr_rootNAryTree->leftChild;
			sPtr_rootNAryTree->leftChild=templf;
			templf->parent=sPtr_rootNAryTree;
		return 0;
		}

		else
		{
			if(temp==sPtr_rootNAryTree)
			{
/*			printf("the root directory cannot be moved");	*/
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_06);
			templf->rightSibling=sPtr_rootNAryTree->leftChild;
			sPtr_rootNAryTree->leftChild=templf;
			templf->parent=sPtr_rootNAryTree;
			return 0;
			}
			else
			{
				if(temp->leftChild!=NULL)
				{
				temp4=temp1;
				while((temp4!=temp)&&(temp4!=sPtr_rootNAryTree))
				{
				temp4=temp4->parent;
				}
				if(temp4==temp)
				{
				/*printf("a directory cannot be moved to its subdirectory");*/
				printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_06);
				templf->rightSibling=sPtr_rootNAryTree->leftChild;
				sPtr_rootNAryTree->leftChild=templf;
				templf->parent=sPtr_rootNAryTree;
				return 0;
				}
				}
				if(temp1->leftChild!=NULL)/*if*/
				{
				tempprob=temp1->leftChild;
					while((tempprob->rightSibling!=NULL)&&(!((strcmp(tempprob->s_inode->cptr_fileName,res)==0)&&(strcmp(tempprob->s_inode->c_fileType,"d")==0))))
					{
					tempprob=tempprob->rightSibling;
					}
					if(strcmp(tempprob->s_inode->cptr_fileName,res)==0)
					{
					/*printf("there is a directory with same name in destination");*/
					printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_05);
					templf->rightSibling=sPtr_rootNAryTree->leftChild;
					sPtr_rootNAryTree->leftChild=templf;
					templf->parent=sPtr_rootNAryTree;
					return 0;
					}
				}
			}
		}


			if(temp->parent->leftChild==temp)
			{
			temp->parent->leftChild=temp->rightSibling;
			}
			else
			{
			prev->rightSibling=temp->rightSibling;
			}
			if(temp1->leftChild==NULL)                                                                                                              
			{
			temp1->leftChild=temp;
			}
			else
			{
			temp2=temp1->leftChild;
				while(temp2->rightSibling!=NULL)
				{
				temp2=temp2->rightSibling;
				}
			temp2->rightSibling=temp;
			}
		temp->parent=temp1;
		temp->rightSibling=NULL;
		if(strcmp(P4,"/")!=0)
		{
		strcat(P4,"/");
		}
		strcat(P4,res);		
		v_changePaths(temp,UPDATEFILEPATH);



	templf->rightSibling=sPtr_rootNAryTree->leftChild;
	sPtr_rootNAryTree->leftChild=templf;
	templf->parent=sPtr_rootNAryTree;
    return 1;
}

void v_changePaths(struct nAryTreeNode *ptrToANode,int i_mode){

    struct nAryTreeNode *temp = NULL;
    struct nAryTreeNode *temp1 =NULL;
    struct binarySearchTree *node = NULL;

    temp = ptrToANode;
if((i_mode==UPDATEFILEPATH)&&(temp!=NULL))
	{
	temp1 =(struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
	temp1->s_inode =(struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);
	node=(struct binarySearchTree *)malloc(sizeof(struct binarySearchTree)*1);
/*	v_delete(temp->s_inode->ui_inodeNo,temp->s_inode->cptr_fileName);*/
	strcpy(temp1->s_inode->cptr_filePath,temp->s_inode->cptr_filePath);
	strcat(temp1->s_inode->cptr_filePath,temp->s_inode->cptr_fileName);
	node=s_searchBSTNode(sPtr_rootBST,temp1->s_inode->cptr_filePath+1);
	 v_deleteBSTNode(sPtr_rootBST,node);
	if(temp->parent!=sPtr_rootNAryTree)
	{
	strcpy(temp->s_inode->cptr_filePath,temp->parent->s_inode->cptr_filePath);
	strcat(temp->s_inode->cptr_filePath,temp->parent->s_inode->cptr_fileName);
	}
	else
	{
	strcpy(temp->s_inode->cptr_filePath,"/");
	}
	strcpy(temp1->s_inode->cptr_filePath,temp->s_inode->cptr_filePath);
     if( *(temp1->s_inode->cptr_filePath 
           + (strlen(temp1->s_inode->cptr_filePath)-1)) != '/' &&
         (strcmp(temp1->s_inode->cptr_filePath,"/")!=0) )
/*	if((*(temp1->s_inode->cptr_filePath[strlen(temp1->s_inode->cptr_filePath)-1)])!='/')&&(strcmp(temp1->s_inode->cptr_filePath,"/")!=0)) */
				{
				strcat(temp1->s_inode->cptr_filePath,"/");
				}
	strcat(temp1->s_inode->cptr_filePath,temp->s_inode->cptr_fileName);
/*	v_loadHashTable(temp->s_inode->ui_inodeNo,temp->s_inode->cptr_fileName,temp->s_inode->cptr_filePath);
	printList();*/
	node=s_getBSTNode(temp1->s_inode->cptr_filePath,temp->s_inode->ui_inodeNo);
	sPtr_rootBST=s_insertBSTNode(sPtr_rootBST,node);
     if( *(temp->s_inode->cptr_filePath 
           + (strlen(temp->s_inode->cptr_filePath)-1)) != '/' &&
         (strcmp(temp->s_inode->cptr_filePath,"/")!=0) )
/*	if((*(temp->s_inode->cptr_filePath[strlen(temp->s_inode->cptr_filePath)-1)])!='/')&&(strcmp(temp->s_inode->cptr_filePath,"/")!=0))*/
				{
				strcat(temp->s_inode->cptr_filePath,"/");
				}
	strcpy(node->cArr_filePath,temp1->s_inode->cptr_filePath+1);
	}
    if(NULL == temp){
         return;
    } else {
         v_changePaths(temp->leftChild,i_mode);
         v_changePaths(temp->rightSibling,i_mode);
    }
}
