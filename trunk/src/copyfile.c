#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fileDescriptor.h"
#include "global.h"
#include "vfs_errorcodes.h"
#include "block.h"

int i_copyfile(char *cPtr_srcFilePath,char *cPtr_destFilePath){

     char *cPtr_token = NULL;
     char *cPtr_copyOfDestFilePath = NULL;
     char *cPtr_fileName = NULL;
     char *cPtr_filePath = NULL;
     char c_fileType;

     unsigned int ui_inodeNo = 0;
     unsigned int ui_dataBlockNo = 0;
     unsigned int ui_destFilePathLength = 0;
     unsigned int ui_lastTokenLength = 0;
     unsigned int ui_copyPos = 0;

     struct fileDescriptor *s_newInode = NULL;
     struct nAryTreeNode *newFileNode = NULL;
     struct nAryTreeNode *currentNode = NULL;
     struct nAryTreeNode *previousNode = NULL;
     struct dataBlock s_block;
     struct binarySearchTree *bstTempNode = NULL;
  
     FILE *fpVfs = NULL;

     currentNode = sPtr_rootNAryTree;
     cPtr_token = strtok(cPtr_srcFilePath,"/");
     while( NULL != cPtr_token ){
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
         if(NULL == currentNode){
              printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_01);
              return FAILURE;
         }
         cPtr_token = strtok(NULL,"/");
     }
   
     if( currentNode->s_inode->c_fileType[0] == 'd' ){
         printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_05);
         return FAILURE;         
     } else {
         c_fileType = currentNode->s_inode->c_fileType[0];
         ui_copyPos = currentNode->s_inode->ui_locationDataBlockNo[0];
     }
     ui_destFilePathLength = strlen(cPtr_destFilePath);
     cPtr_copyOfDestFilePath = (char *)malloc(sizeof(char)*(ui_destFilePathLength+1));
     strcpy(cPtr_copyOfDestFilePath,cPtr_destFilePath);
     *(cPtr_copyOfDestFilePath+ui_destFilePathLength) = '\0';
     currentNode = sPtr_rootNAryTree;
     cPtr_token = strtok(cPtr_destFilePath,"/");
     while( NULL != cPtr_token ){
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
         if(NULL == currentNode && NULL == previousNode ){
              printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_02);
              return FAILURE;
         }
         ui_lastTokenLength = strlen(cPtr_token);
         cPtr_token = strtok(NULL,"/");
     }
     currentNode = previousNode;
     cPtr_fileName = (char *)malloc(sizeof(char)*(ui_lastTokenLength+1));
     strncpy(cPtr_fileName,
             (cPtr_copyOfDestFilePath+ui_destFilePathLength-ui_lastTokenLength),
             ui_lastTokenLength);
     *(cPtr_fileName+ui_lastTokenLength) = '\0';

     ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
     ui_dataBlockNo = i_getFreeBlock(s_dataBlockFreeList);
     if( (0 == ui_inodeNo) ){
         printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_06);
         return FAILURE;
     }
     if( (0 == ui_dataBlockNo) ){
         printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_06);
         return FAILURE;
     }
     cPtr_filePath = (char *)malloc(sizeof(char)*(ui_destFilePathLength+1));
     strncpy(cPtr_filePath,cPtr_copyOfDestFilePath,ui_destFilePathLength);
     *(cPtr_filePath+ui_destFilePathLength) = '\0';
     s_newInode = s_createFileDescriptor(cPtr_fileName,
                                         cPtr_filePath,
                                         c_fileType,
                                         ui_inodeNo,0,ui_dataBlockNo);
     newFileNode = s_getNAryTreeNode();
     newFileNode->s_inode = s_newInode;
     previousNode = currentNode;
     currentNode = s_insertNAryTreeNode(currentNode,newFileNode);
 
     bstTempNode = s_getBSTNode(cPtr_copyOfDestFilePath,ui_inodeNo);
     sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,bstTempNode);
     if(NULL != cPtr_filePath){
         free(cPtr_filePath);
     }
     if(NULL != cPtr_fileName){
         free(cPtr_fileName);
     }

     if(NULL == (fpVfs=fopen(cPtr_nameOfVfsMounted,"rb+"))){
         printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_07);
         return FAILURE;
     }
     fseek(fpVfs,ui_copyPos*VFS_BLOCKSIZE,SEEK_SET);
     fread(&s_block,sizeof(struct dataBlock),1,fpVfs);
     fseek(fpVfs,ui_dataBlockNo*VFS_BLOCKSIZE,SEEK_SET);
     fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs);
     fclose(fpVfs);
 
     return SUCCESS;
}
