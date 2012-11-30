#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

#include "global.h"
#include "fileSystemOps.h"
#include "freeList.h"
#include "nAryTree.h"
#include "vfs_errorcodes.h"

struct freeList *s_dataBlockFreeList = NULL; /* Data block freelist head */

struct freeList *s_inodeBlockFreeList = NULL; /* Inode block freelist head */

struct nAryTreeNode *sPtr_rootNAryTree = NULL; /* Root of n-Ary Tree */

struct binarySearchTree *sPtr_rootBST = NULL; /* Root of Binary Search Tree */

char *cPtr_nameOfVfsMounted = NULL; /* Holds the name of file system that is 
                                       currently loaded.
                                    */

/*
Function Name: i_mountvfs
Description: It loads the information of the file system into the following 
             data structures. 
             1) n-Ary Tree ---> Used to represent the entire file system
             2) Binary Search Tree ---> It keeps the full path information of 
                                        all the directories and files in the 
                                        file system.
             3) Hash Table ---> It keeps all the names of the files and directo-
                                -ries present in the file system.
Parameters: It takes the name of the file system to be loaded into the main mem-
            -ory.
Return Type: int
             0 ---> If any error occurs
             1 ---> If the file system is successfully loaded into the main mem-
                    -ory.
*/
int i_mountvfs(char *vfsLabel){   

				/* Declaration Section */

    struct mainHeader s_superBlock;
    struct mainHeader * sPtr_superBlock;

    FILE *fpVfs = NULL;

				/* End of declaration Section */


    /* The file system to be mounted is not created or not present in the expec-
        -ted folder (.i.e bin folder )
    */
    if((fpVfs = fopen(vfsLabel,"rb")) == NULL){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_01);
         return FAILURE;      
    }

    /* Unable to move to the current position where the super block information
       is present.
    */
    if((fseek(fpVfs,0,SEEK_SET)) == -1){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_02);
         return FAILURE;
    }

    /* If the super block contents are tampered, then the file system cannot be
       mounted.
    */ 
    if((fread(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_03);
         return FAILURE;
    }

    fclose(fpVfs);

				/* End of Validation Section */

					/* Module Logic */

    cPtr_nameOfVfsMounted = (char *)malloc(sizeof(char)*(VFSLABELLENGTH+1));
    memset((void *)cPtr_nameOfVfsMounted,'\0',sizeof(char)*(VFSLABELLENGTH+1));
    strcpy(cPtr_nameOfVfsMounted,vfsLabel);

    sPtr_superBlock = &s_superBlock;
    /* The data block free list will maintain the block numbers in the file sys-
       -tem that can be used to store information. Note that this list maintains 
       only the block numbers where the contents of file or directory in the fi-
       le system can be stored.
       Note: The value 2 is added because, during the creation stage of the file
             system, two of the data blocks were occupied by Root and lost+found
             directories to store their respective dientry information.
    */
    s_dataBlockFreeList = s_createFreeList(
                          s_superBlock.ui_startFreeBlockNoOfDataBlock,
                          s_superBlock.ui_endFreeBlockNoOfDataBlock);  
    /* The inode block free list will maintain the block numbers in the file sy-
       -stem that can be used to store file descriptor(Inode) information corre-
       -sponding to each file or directory created in the file system.
       Note: During creation stage of the file system, four Inode blocks were
             already occupied by standard input, output, root and lost+found
             directories. Also 0th block contains the meta-data information of 
             the file system and block number 1 is used as a reserve for future
             use. Hence VFS_ACTUAL_INODE_STARTBLOCK will have the block number 
             from where the information of the FD's of the files and directories
             created by user will be stored. 
    */
    s_inodeBlockFreeList = s_createFreeList(
                           s_superBlock.ui_startFreeBlockNoOfInodeArray,
                           s_superBlock.ui_endFreeBlockNoOfInodeArray);
    /* Load the file system into the n-Ary Tree */
    sPtr_rootNAryTree = s_loadFileSystem(VFS_ROOT_INODE,
                                         cPtr_nameOfVfsMounted,
                                         sPtr_superBlock);
    
    /* Load the Binary Search Tree */	
    v_traverseNAryTreeAux(sPtr_rootNAryTree,BST); 

					/* End of Module Logic */
	
    return SUCCESS;
}
