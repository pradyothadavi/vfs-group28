#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "vfs_errorcodes.h"
#include "nAryTree.h"
#include "block.h"
#include "fileOps.h"

/*
Function Name: 
Description:
Parameters: 
Return Type:
*/
int i_listfile (char *cPtr_filePath,char *cPtr_sysPath,int i_mode){

     char *cPtr_token = NULL;
     struct nAryTreeNode *currentNode = NULL;
     struct dataBlock s_block;
     FILE *fp = NULL;
     FILE *fpVfs = NULL;

     if( NULL == (fp=fopen(cPtr_sysPath,"r")) ){
         if(i_mode == TEXTFILE){
              printf("listfile FAILURE %s \n",ERR_VFS_LISTFILE_02);
         } else if(i_mode == REGULARFILE){
              printf("exportfile FAILURE %s \n",ERR_VFS_EXPORTFILE_02);
         }
         return FAILURE;
     } else {
         fclose(fp);
     }

     currentNode = sPtr_rootNAryTree;

     cPtr_token = strtok(cPtr_filePath,"/");
     while(cPtr_token != NULL){
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
         if( NULL == currentNode ){
              if(i_mode == TEXTFILE){
                   printf("listfile FAILURE %s \n",ERR_VFS_LISTFILE_01);
              } else if(i_mode == REGULARFILE){
                   printf("exportfile FAILURE %s \n",ERR_VFS_EXPORTFILE_01);
              }
              return FAILURE;
         }
         cPtr_token = strtok(NULL,"/");         
     }
     if((currentNode->s_inode->c_fileType[0] == 'r' && i_mode == REGULARFILE)||
         (currentNode->s_inode->c_fileType[0] == 't' && i_mode == TEXTFILE)){
         if( NULL == (fpVfs=fopen(cPtr_nameOfVfsMounted,"rb")) ){
              printf("listfile FAILURE UNKNOWN ERROR \n");
              return FAILURE;
         }
         fseek(fpVfs,currentNode->s_inode->ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET);
         memset((void *)&s_block,'\0',sizeof(struct dataBlock));
         fread(&s_block,sizeof(struct dataBlock),1,fpVfs);
         fclose(fpVfs);
         if( NULL == (fp=fopen(cPtr_sysPath,"w")) ){
              printf("listfile FAILURE %s \n",ERR_VFS_LISTFILE_02);
              return FAILURE;
         }
         fwrite(&s_block,sizeof(struct dataBlock),1,fp);
         fclose(fp);    
     } else {
         if(i_mode == TEXTFILE){
              printf("listfile FAILURE %s \n",ERR_VFS_LISTFILE_05);
         } else if(i_mode == REGULARFILE){
              printf("exportfile FAILURE %s \n",ERR_VFS_EXPORTFILE_05);
         }
         return FAILURE;
     }
	return SUCCESS;
}
