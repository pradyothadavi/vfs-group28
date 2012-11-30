#include"main.h"
#include"nAryTree.h"
#include"fileDescriptor.h"
#include"block.h"
#include"global.h"
#include<string.h>
#include "limits.h"
#include "fileSystemOps.h"
#include"vfs_errorcodes.h"
#include "hashTable.h"

/*
Function Name: removefile
Description: It traverses the binary search Tree from the root with the entire path passed as parameter to get the file descriptor
Parameters : It takes a single parameter, which is the absolute path of the file to be removed
Return Type: It returns void
*/

int i_removefile ( char *P1 ){
	/*
		1. Search the nary with the entire path to get the file descriptor
		2. Delete that node from BST
		3. Search the nary with the given iNode number from file descriptor and remove that node from nAry tree
		4. Delete the given iNode number and delete that node from hashTable
		5. Add the iNode number to the iNode number free list
		6. Add the data block number to the dataBlock number free list

	*/
	
	struct nAryTreeNode *nary= NULL;
	struct binarySearchTree *sPtr_nodeToBeDeleted=NULL;
	char *sendName;
	char *token;
	struct fileDescriptor *fd;
	char *ndir= NULL;
	static int i_noOfCharacters=0;

        ndir = (char *)malloc(sizeof(char)*strlen(P1)+1);
        strncpy(ndir,P1,strlen(P1));
        *(ndir+strlen(P1)) = '\0';
	sendName=NULL;	
	nary=sPtr_rootNAryTree;
	token=strtok(ndir,"/");
	
	while(token!=NULL)
             {  
                 
             i_noOfCharacters=strlen(token)+1;
             nary=s_searchNAryTreeNode(nary,token,NONRECURSIVE);

              if(NULL==nary)
                 {   
                     printf("removefile_FAILURE %s \n",ERR_VFS_REMOVEFILE_01);
			return FAILURE;
                                     
                 }
                 
               token=strtok(NULL,"/");
            }
	if( NULL != ndir){
        	free(ndir);
        }
	fd=nary->s_inode;
	sendName=(char*)malloc(sizeof(char)*VFS_NAMELEN);
	strcpy(sendName,fd->cptr_fileName);
	
	sPtr_nodeToBeDeleted=s_searchBSTNode(sPtr_rootBST,P1+1);
	v_deleteNAryTreeNode(nary);
	v_delete(fd->ui_inodeNo,sendName); /*deleting from hash table */
	v_deleteBSTNode(sPtr_rootBST,sPtr_nodeToBeDeleted);/* deleting from BST */
	s_inodeBlockFreeList=s_addFreeBlock(s_inodeBlockFreeList,fd->ui_inodeNo); /*add the inode number*/
	s_dataBlockFreeList=s_addFreeBlock(s_dataBlockFreeList,fd->ui_locationDataBlockNo[0]);/*add the data number*/

	return SUCCESS;
}






