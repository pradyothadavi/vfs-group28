#include<stdio.h>
#include<string.h>

#include "global.h"
#include "vfs_errorcodes.h"

/*
Function Name: i_unmountvfs
Description: It stored all the information of the file system present in various 
             data structures back to the file system in the appropriate data bl-
             ocks of the file system.
Parameters: It takes the name of the file system to be unmounted from the main
            memory.
Return Type: int
            0 ---> If any occurs during the unmount stage
            1 ---> If the unmount stage is completed successfully
*/
int i_unmountvfs(char *vfsLabel){

				/* Declaration Section */

    FILE *fpVfs = NULL;

    struct mainHeader s_superBlock;

				/* End of declaration Section */

				/* Begining of Validation Section */
	
    /* Checking whether the file system to be unmounted is present in the main
       memory or not.
    */
    if( 0 == strcmp(vfsLabel,cPtr_nameOfVfsMounted) ){
         /* The file into which the information has to be written back is not 
            present in the desired folder (i.e. bin )
         */
         if( NULL == (fpVfs = fopen(vfsLabel,"rb")) ){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_01);
              return FAILURE;
         } else {
              if( NULL != fpVfs ){
                   fclose(fpVfs);
              } 
         } 
					/* End of Validation Section */
		
						/* Module Logic */

         /* Using binary search tree, for each directory present in the file sy-
            stem store the dientry information at the appropriate blocks in the
            in the file. It takes the help of the n-Ary Tree internally to store
            the dientry information of a directory.
         */
         v_traverseBST(sPtr_rootBST,UNMOUNT);
         /* The Inode information of all the files and directories present in 
            the file system are written back to the appropriate blocks.
         */
         v_traverseNAryTreeAux(sPtr_rootNAryTree,UNMOUNTVFS);
         /* The dientry of the root directory is stored, as the path of the root
            is not stored in the Binary Search Tree.
         */
         s_searchNAryTreeNode(sPtr_rootNAryTree,NULL,UNMOUNTVFS);
         /* Store the meta-data information of the file system into the super
            block of the file system.
         */ 
         if( NULL == (fpVfs = fopen(vfsLabel,"rb+")) ){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_01);
              return FAILURE;
         }
         fseek(fpVfs,0,SEEK_SET);
         if((fread(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
              printf("DEBUG: Error in reading superblock \n");   
         }
         strcpy(s_superBlock.c_fileSystemLabel,cPtr_nameOfVfsMounted);
         s_superBlock.ui_noOfUsedFileDescriptors = 
                                        s_superBlock.ui_noOfUsedFileDescriptors+
                                        s_superBlock.ui_maxNoOfFileDescriptors-
                         s_inodeBlockFreeList->sPtr_nextFreeBlock->ui_blockNo-1;
         s_superBlock.ui_endFreeBlockNoOfInodeArray = 
                           s_inodeBlockFreeList->sPtr_nextFreeBlock->ui_blockNo;
         s_superBlock.ui_endFreeBlockNoOfDataBlock =
                            s_dataBlockFreeList->sPtr_nextFreeBlock->ui_blockNo; 

         fseek(fpVfs,0,SEEK_SET);
         if((fwrite(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_03);
              return FAILURE;   
         } 
         if( NULL != fpVfs ){
              fclose(fpVfs);
         }
 
#if UNMOUNT_DEBUG
         v_displaySuperBlock(vfsLabel);
         v_traverseBST(sPtr_rootBST,UNMOUNTVERIFICATION);
         v_traverseNAryTreeAux(sPtr_rootNAryTree,UNMOUNTVFSVERIFICATION);
         s_searchNAryTreeNode(sPtr_rootNAryTree,NULL,UNMOUNTVFSVERIFICATION);
#endif
         
    } else {
         printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_04);
         return FAILURE;
    }

					/* End of Module Logic */
    return SUCCESS;
}

