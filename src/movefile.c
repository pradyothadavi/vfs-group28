#include<stdio.h>
#include<string.h>
#include"nAryTree.h"
#include"global.h"
#include"vfs_errorcodes.h"
#include"binarySearchTree.h"
int i_movefile(char *P1,char *P2){

/*request for root of n ary tree and store it in temp and temp1*/
	/*P1=source dir path P2=dest dir path*/
	/*move to source dir*/
	/*assign the links to dest dir*/
	struct nAryTreeNode *temp=NULL;
	struct nAryTreeNode *tempprob=NULL;
	struct nAryTreeNode *tempprob1=NULL;
	struct nAryTreeNode *templf=sPtr_rootNAryTree->leftChild;
	struct nAryTreeNode *temp1=NULL;
	struct nAryTreeNode *prev1=NULL;
	struct nAryTreeNode *prev=NULL;
	struct nAryTreeNode *p=NULL;
	struct binarySearchTree *node=NULL;
	char *P3,*result,*result1,*P4,*res,*res1,*valid;

	
	if(sPtr_rootNAryTree==NULL||sPtr_rootNAryTree->leftChild==NULL)
	{
	return 0;
	}
	sPtr_rootNAryTree->leftChild=templf->rightSibling;
	templf->rightSibling=NULL;
	templf->parent=NULL;
	
		if(*P1==0||*P2==0)
		{
		printf("movefile_FAILURE %s\n",ERR_VFS_MOVEDIR_00);
		templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
		return 0;
		}

	temp=(struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
	temp->s_inode=(struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);
	p=(struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
	p->s_inode=(struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);
	temp=sPtr_rootNAryTree;
	temp1=sPtr_rootNAryTree;
	P4=(char *)malloc(sizeof(char)*strlen(P1));
	strcpy(P4,P2);
		if(temp->leftChild==NULL)/*if N ary tree is empty*/
		{
/*		printf("there are no directories to be moved");*/
		printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_01);
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
			printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_01);
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
				
				/*call strtok and retrieve the next substring;*/
			res=result;
			result=strtok(NULL,"/");
			}
if(temp!=sPtr_rootNAryTree)
{
temp=temp->parent;
}
		result1=strtok(P2,"/");
		if(result1==NULL)
		{
		templf->rightSibling=sPtr_rootNAryTree->leftChild;
		sPtr_rootNAryTree->leftChild=templf;
		templf->parent=sPtr_rootNAryTree;
		return 0;
		}
			while(result1!=NULL)/*points to the directory to which it is to be moved(destination)*/
			{
			prev1=temp1;
			temp1=temp1->leftChild;
			if(temp1==NULL)
			{
			break;
			}

				while((temp1->rightSibling!=NULL)&&(!((strcmp(temp1->s_inode->cptr_fileName,result1)==0)&&(strcmp(temp1->s_inode->c_fileType,"d")==0))))
				{
				prev1=temp1;
				temp1=temp1->rightSibling;
				}
				
			/*call strtok and retrieve the next substring;*/
			res1=result1;
			result1=strtok(NULL,"/");
			}
if(temp1==NULL)
{
res1=result1;
result1=strtok(NULL,"/");
if(result1==NULL)
{
temp1=prev1;
}
else
{
/*			printf("dir does not exist");*/
			printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_02);
			templf->rightSibling=sPtr_rootNAryTree->leftChild;
			sPtr_rootNAryTree->leftChild=templf;
			templf->parent=sPtr_rootNAryTree;
			return 0;

}
}
else
{
temp1=temp1->parent;
}
		}
			if(temp==sPtr_rootNAryTree&&res==NULL)
			{
/*			printf("the root directory cannot be moved");*/
			printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_01);
			templf->rightSibling=sPtr_rootNAryTree->leftChild;
			sPtr_rootNAryTree->leftChild=templf;
			templf->parent=sPtr_rootNAryTree;
			return 0;
			}
			else
			{
				if(temp->leftChild!=NULL)
				{
				prev=temp;
				tempprob=temp->leftChild;
				while((tempprob->rightSibling!=NULL)&&(!((strcmp(tempprob->s_inode->cptr_fileName,res)==0)&&(strcmp(tempprob->s_inode->c_fileType,"d")!=0))))
				{
				prev=tempprob;
				tempprob=tempprob->rightSibling;
				}
				if(!((strcmp(tempprob->s_inode->cptr_fileName,res)==0)&&(strcmp(tempprob->s_inode->c_fileType,"d")!=0)))
				{
/*				printf("source file not found");*/
				printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_01);
				templf->rightSibling=sPtr_rootNAryTree->leftChild;
				sPtr_rootNAryTree->leftChild=templf;
				templf->parent=sPtr_rootNAryTree;
				return 0;

				}

				}
				if(temp1->leftChild!=NULL)/*if*/
				{
				tempprob1=temp1->leftChild;

				while((tempprob1->rightSibling!=NULL)&&(!((strcmp(tempprob1->s_inode->cptr_fileName,res1)==0)&&(strcmp(tempprob1->s_inode->c_fileType,"r")==0))))
				{
				tempprob1=tempprob1->rightSibling;
				}

				if(!((strcmp(tempprob1->s_inode->cptr_fileName,res1)==0)&&(strcmp(tempprob1->s_inode->c_fileType,"d")!=0)))
				{
				strcpy(p->s_inode->cptr_fileName,res1);
				strcpy(p->s_inode->c_fileType,tempprob->s_inode->c_fileType);

				p->s_inode->ui_inodeNo=tempprob->s_inode->ui_inodeNo;
				p->s_inode->ui_locationDataBlockNo[0]=tempprob->s_inode->ui_locationDataBlockNo[0];
				p->parent=temp1;
				if(p->parent==sPtr_rootNAryTree)
				{
				strcpy(p->s_inode->cptr_filePath,"/");
				}
				else
				{
				strcpy(p->s_inode->cptr_filePath,temp1->s_inode->cptr_filePath);
				strcat(p->s_inode->cptr_filePath,temp1->s_inode->cptr_fileName);
				}
				if( *(p->s_inode->cptr_filePath + (strlen(p->s_inode->cptr_filePath)-1)) != '/'&&(strcmp(p->s_inode->cptr_filePath,"/")!=0) )
/*				if((p->s_inode->cptr_filePath[strlen(temp1->s_inode->cptr_filePath-1)]!="/")&&(strcmp(p->s_inode->cptr_filePath,"/")!=0))*/
				{
				strcat(p->s_inode->cptr_filePath,"/");
				}
				tempprob1->rightSibling=p;
				p->leftChild=NULL;
				p->rightSibling=NULL;
				node=s_getBSTNode(P4+1,p->s_inode->ui_inodeNo);
				sPtr_rootBST=s_insertBSTNode(sPtr_rootBST,node);
				v_loadHashTable(p->s_inode->ui_inodeNo,p->s_inode->cptr_fileName,p->s_inode->cptr_filePath);

				}
				else
				{
				s_inodeBlockFreeList=s_addFreeBlock(s_inodeBlockFreeList,tempprob1->s_inode->ui_inodeNo);
				s_dataBlockFreeList=s_addFreeBlock(s_dataBlockFreeList,tempprob1->s_inode->ui_locationDataBlockNo[0]);
				tempprob1->s_inode->ui_inodeNo=tempprob->s_inode->ui_inodeNo;
				tempprob1->s_inode->ui_locationDataBlockNo[0]=tempprob->s_inode->ui_locationDataBlockNo[0];
				}
				}
				else
				{
				strcpy(p->s_inode->cptr_fileName,res1);
				strcpy(p->s_inode->c_fileType,tempprob->s_inode->c_fileType);

				p->s_inode->ui_inodeNo=tempprob->s_inode->ui_inodeNo;
				p->s_inode->ui_locationDataBlockNo[0]=tempprob->s_inode->ui_locationDataBlockNo[0];
				if( *(p->s_inode->cptr_filePath + (strlen(p->s_inode->cptr_filePath)-1)) != '/'&&(strcmp(p->s_inode->cptr_filePath,"/")!=0) )
				/*if(p->s_inode->cptr_filePath[strlen(p->s_inode->cptr_filePath-1)]!="/");*/
				{
				strcat(p->s_inode->cptr_filePath,"/");
				}
				node=s_getBSTNode(P4+1,p->s_inode->ui_inodeNo);
				sPtr_rootBST=s_insertBSTNode(sPtr_rootBST,node);
				v_loadHashTable(p->s_inode->ui_inodeNo,p->s_inode->cptr_fileName,p->s_inode->cptr_filePath);

				p->parent=temp1;
				if(p->parent==sPtr_rootNAryTree)
				{
				strcpy(p->s_inode->cptr_filePath,"/");
				}
				else
				{
				strcpy(p->s_inode->cptr_filePath,temp1->s_inode->cptr_filePath);
				strcat(p->s_inode->cptr_filePath,temp1->s_inode->cptr_fileName);
				}
				if( *(p->s_inode->cptr_filePath + (strlen(p->s_inode->cptr_filePath)-1)) != '/'&&(strcmp(p->s_inode->cptr_filePath,"/")!=0) )
				/*if((p->s_inode->cptr_filePath[strlen(temp1->s_inode->cptr_filePath-1)]!="/")&&(strcmp(p->s_inode->cptr_filePath,"/")!=0))*/
				{
				strcat(p->s_inode->cptr_filePath,"/");
				}
				v_loadHashTable(p->s_inode->ui_inodeNo,p->s_inode->cptr_fileName,p->s_inode->cptr_filePath);
				
				temp1->leftChild=p;
				p->leftChild=NULL;
				p->rightSibling=NULL;
				}
				
			}
if(tempprob->parent->leftChild==tempprob)
			{
			tempprob->parent->leftChild=tempprob->rightSibling;
			}
			else
			{
			prev->rightSibling=tempprob->rightSibling;
			}
	free(tempprob);
	templf->rightSibling=sPtr_rootNAryTree->leftChild;
	sPtr_rootNAryTree->leftChild=templf;
	templf->parent=sPtr_rootNAryTree;

     return 1;
}
