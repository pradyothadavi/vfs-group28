#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileOps.h"
#include "global.h"
#include "nAryTree.h"
#include "binarySearchTree.h"
#include "vfs_errorcodes.h"
#include "block.h"

int i_addfile(char *cPtr_destinationPath,char *cPtr_fileName,char *cPtr_sysPath){

    char *cPtr_copyOfDestPath = NULL;
    char *cPtr_token = NULL;
    char *cPtr_dirPath = NULL;
    char *cPtr_filePath = NULL;
    char *cPtr_copyOfSysPath = NULL;
    char fileType;
    char *original=NULL;

    unsigned int ui_inodeNo = 0;
    unsigned int ui_dataBlockNo = 0;
    unsigned int ui_size = 0;
    unsigned int ui_noOfCharactersParsed = 0;

    int i = 0;

    FILE *fp = NULL;
    FILE *fpVfs = NULL;

    struct fileDescriptor *s_newInode = NULL;
    struct nAryTreeNode *newFileNode = NULL;
    struct nAryTreeNode *nAryRoot = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *tempNode = NULL;
    struct dataBlock s_block;
    struct binarySearchTree *bstTempNode = NULL;

    /* Validations */

    original=(char *)malloc(sizeof(char)*strlen(cPtr_fileName));
    strcpy(original,cPtr_fileName);

    cPtr_token = strtok(cPtr_fileName,"/");
    if( cPtr_token != NULL && ( 0 != strcmp(cPtr_token,cPtr_fileName)) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_02);
         return FAILURE;
    }
    if( NULL != original){
         free(original);
    }
    if( NULL == (fpVfs = fopen(cPtr_nameOfVfsMounted,"rb")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_07);
         return FAILURE;
    } else {
         fclose(fpVfs);
    }

    if( NULL == (fp = fopen(cPtr_sysPath,"r")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_05);
         return FAILURE;
    } else {
         fseek(fp,0,SEEK_END);
         ui_size = ftell(fp);
         fclose(fp);
         if( VFS_BLOCKSIZE < ui_size ){     
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_06);
              return FAILURE;
         }
    }

    nAryRoot = sPtr_rootNAryTree;
    currentNode = nAryRoot;
    cPtr_copyOfDestPath = (char *)malloc(sizeof(char)*strlen(cPtr_destinationPath));
    strcpy(cPtr_copyOfDestPath,cPtr_destinationPath);
    cPtr_copyOfSysPath = (char *)malloc(sizeof(char)*strlen(cPtr_sysPath));
    strcpy(cPtr_copyOfSysPath,cPtr_sysPath);

    cPtr_token = strtok(cPtr_copyOfSysPath,"/.");
    while (cPtr_token != NULL){
         if( 0 == strcmp(cPtr_token,"txt") ){
              fileType = 't';
         } else {
              fileType = 'r';
         }
         cPtr_token = strtok(NULL,"/.");
    }

    if( NULL != cPtr_copyOfSysPath){
         free(cPtr_copyOfSysPath);
    }
    if( 0 == strcmp(cPtr_destinationPath,"/") ){
         currentNode = s_searchNAryTreeNode(nAryRoot,cPtr_fileName,NONRECURSIVE);
         if( ( currentNode != NULL ) && (0 == strcmp(currentNode->s_inode->cptr_fileName,cPtr_fileName)) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_03);
              return FAILURE;
         }
         ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
         ui_dataBlockNo = i_getFreeBlock(s_dataBlockFreeList);
         if( (0 == ui_inodeNo) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_01);
              return FAILURE;
         }
         if( (0 == ui_dataBlockNo) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_04);
              return FAILURE;
         }
         cPtr_filePath = (char *)malloc(sizeof(char)*((strlen(cPtr_destinationPath)+strlen(cPtr_fileName)+3)));
         memset((void *)cPtr_filePath,'\0',sizeof(char)*((strlen(cPtr_destinationPath)+strlen(cPtr_fileName)+3)));
         strncpy(cPtr_filePath,
                 cPtr_destinationPath,
                 (strlen(cPtr_destinationPath)));
         if(*(cPtr_filePath+strlen(cPtr_destinationPath)) != '/'){
              *(cPtr_filePath+strlen(cPtr_destinationPath)) = '/';
              *(cPtr_filePath+strlen(cPtr_destinationPath)+1) = '\0';
         }
         printf("Path %s \n",cPtr_filePath); 
         s_newInode = s_createFileDescriptor(cPtr_fileName,
                                             cPtr_filePath,
                                             fileType,ui_inodeNo,0,ui_dataBlockNo);
         newFileNode = s_getNAryTreeNode();
         newFileNode->s_inode = s_newInode;
         nAryRoot = s_insertNAryTreeNode(nAryRoot,newFileNode);
         strncpy(cPtr_filePath+strlen(cPtr_destinationPath)+1,cPtr_fileName,(strlen(cPtr_fileName)));
       *(cPtr_filePath+strlen(cPtr_destinationPath)+strlen(cPtr_fileName)+1)='\0';       
         bstTempNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
         sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,bstTempNode);
         if( NULL != cPtr_filePath ){
              free(cPtr_filePath);
         }
    } else {
         cPtr_token = strtok(cPtr_copyOfDestPath,"/");
         while( cPtr_token != NULL ){
              previousNode = currentNode;
              currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
              if( NULL == currentNode ){
                   do{
                        cPtr_dirPath = (char *)malloc(sizeof(char)*(ui_noOfCharactersParsed+1));
                        strncpy(cPtr_dirPath,cPtr_destinationPath,ui_noOfCharactersParsed);
                        *(cPtr_dirPath + (ui_noOfCharactersParsed+1)) = '\0';
                        i_makedir(cPtr_dirPath,cPtr_token);
                        if( NULL != cPtr_dirPath ){
                             free(cPtr_dirPath);
                        }
                        cPtr_dirPath = NULL;
                        ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
                        cPtr_token = strtok(NULL,"/");
                   } while ( cPtr_token != NULL );
                   /* Get the current Node to the newly formed directory */
              } else {
                   ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
                   cPtr_token = strtok(NULL,"/");
              }
         }
         cPtr_copyOfDestPath = (char *)malloc(sizeof(char)*strlen(cPtr_destinationPath));
         strcpy(cPtr_copyOfDestPath,cPtr_destinationPath);
         previousNode = currentNode = sPtr_rootNAryTree;
         cPtr_token = strtok(cPtr_copyOfDestPath,"/");
         while( cPtr_token != NULL ){
              previousNode = currentNode;
              currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
              cPtr_token = strtok(NULL,"/");
         }
         tempNode = s_searchNAryTreeNode(currentNode,cPtr_fileName,NONRECURSIVE);
         if( (tempNode != NULL) && (0 == strcmp(tempNode->s_inode->cptr_fileName,cPtr_fileName)) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_03);
              return FAILURE;
         }
         ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
         ui_dataBlockNo = i_getFreeBlock(s_dataBlockFreeList);
         if( (0 == ui_inodeNo) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_01);
              return FAILURE;
         }
         if( (0 == ui_dataBlockNo) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_04);
              return FAILURE;
         }
         cPtr_filePath = (char *)malloc(sizeof(char)
                                *((strlen(cPtr_destinationPath)+
                                   strlen(cPtr_fileName)+3)));
         memset((void *)cPtr_filePath,'\0',sizeof(char)*((strlen(cPtr_destinationPath)+strlen(cPtr_fileName)+3)));
         strncpy(cPtr_filePath,
                 cPtr_destinationPath,
                 (strlen(cPtr_destinationPath)));
         if(*(cPtr_filePath+strlen(cPtr_destinationPath)) != '/'){
              *(cPtr_filePath+strlen(cPtr_destinationPath)) = '/';
              *(cPtr_filePath+strlen(cPtr_destinationPath)+1) = '\0';
         }
         s_newInode = s_createFileDescriptor(cPtr_fileName,cPtr_filePath,fileType,ui_inodeNo,0,ui_dataBlockNo);
         newFileNode = s_getNAryTreeNode();
         newFileNode->s_inode = s_newInode;
         nAryRoot = s_insertNAryTreeNode(currentNode,newFileNode);
         strncpy(cPtr_filePath+strlen(cPtr_destinationPath)+1,cPtr_fileName,(strlen(cPtr_fileName)));
       *(cPtr_filePath+strlen(cPtr_destinationPath)+strlen(cPtr_fileName)+1)='\0';       
         bstTempNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
         sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,bstTempNode);
         if( NULL != cPtr_filePath ){
              free(cPtr_filePath);
         }
    }

    /* Store the data into the file System */
    if( NULL == (fpVfs = fopen(cPtr_nameOfVfsMounted,"rb+")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_07);
         return FAILURE;
    }
    if( NULL == (fp = fopen(cPtr_sysPath,"r")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_05);
         return FAILURE;
    }
    fseek(fpVfs,ui_dataBlockNo*VFS_BLOCKSIZE,SEEK_SET);
    memset((void *)&s_block,0,sizeof(struct dataBlock));
    fread(&s_block,sizeof(struct dataBlock),1,fp);
    fclose(fp);
    fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs);
    fclose(fpVfs);
    if( NULL != cPtr_copyOfDestPath ){
         free(cPtr_copyOfDestPath);
    }

    return SUCCESS;
}
