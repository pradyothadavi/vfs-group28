#include <stdio.h>
#include <string.h>

#include "global.h"
#include "vfs_errorcodes.h"

/*
Function Name: i_makedir
Description: It creates a directory in the virtual file system. If the 
             intermediate directories does not exists then it creates them.
Parameters: It takes 2 parameters.
            1) Path of the directory to be created
            2) Name of the directory to be created
Return Type: int
*/
int i_makedir(char *cPtr_directoryPath,char *cPtr_directoryName){

    char *cPtr_token = NULL;
    char *cPtr_copyOfDirPath = NULL;
    char *cPtr_filePath = NULL;

    unsigned int ui_inodeNo = 0;
    unsigned int ui_dientryBlockNo = 0;
    unsigned int ui_noOfCharactersParsed = 0;
    unsigned int ui_dirPathLength = 0;
    unsigned int ui_tokenLength = 0;
    unsigned int ui_dirNameLength = 0;


    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *tempRootNode = NULL;
    struct nAryTreeNode *newNAryNode = NULL;

    struct fileDescriptor *s_newInode = NULL;

    struct binarySearchTree *newBSTNode = NULL;

    if( (NULL != (cPtr_token = strtok(cPtr_directoryName,"/")) ) &&
        (0 < strcmp(cPtr_token,cPtr_directoryName)) ) {
         printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_02);
         return FAILURE;
    }

    cPtr_copyOfDirPath = (char *)malloc(sizeof(char)*strlen(cPtr_directoryPath));
    strcpy(cPtr_copyOfDirPath,cPtr_directoryPath);
    ui_dirPathLength = strlen(cPtr_directoryPath);
    ui_dirNameLength = strlen(cPtr_directoryName);

    previousNode = currentNode = sPtr_rootNAryTree;    

    if( (0 == strcmp(cPtr_directoryPath,"/")) ){        
         currentNode = s_searchNAryTreeNode(currentNode,
                                            cPtr_directoryName,
                                            NONRECURSIVE);

         if( NULL == currentNode){
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              if( 0 == ui_inodeNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              if( 0 == ui_dientryBlockNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)*(ui_dirPathLength+1));
              memset((void *)cPtr_filePath,'\0',
                     sizeof(char)*(ui_dirPathLength+1));
              strcpy(cPtr_filePath,cPtr_directoryPath);
              *(cPtr_filePath + ui_dirPathLength + 1) = '\0';
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,
                                                  cPtr_filePath,'d',ui_inodeNo,
                                                  0,ui_dientryBlockNo);
       
              /*v_loadHashTable(ui_inodeNo,cPtr_directoryName,cPtr_filePath);*/
              currentNode = s_getNAryTreeNode();
              currentNode->s_inode = s_newInode;
              
              previousNode = sPtr_rootNAryTree;
              previousNode = s_insertNAryTreeNode(previousNode,currentNode);

              newBSTNode = s_getBSTNode(cPtr_directoryName,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);
                       
              if(NULL != cPtr_filePath){
                   free(cPtr_filePath);
              }
              
         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,
                                cPtr_directoryName)){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_03);
              return FAILURE;
         } /* End of inner if-else-if */
    } else {
         /* Tokenize the directory path */
         cPtr_token = strtok(cPtr_copyOfDirPath,"/");
         while(cPtr_token != NULL){
              previousNode = currentNode;
              currentNode = s_searchNAryTreeNode(currentNode,
                                                 cPtr_token,
                                                 NONRECURSIVE);
              if(NULL == currentNode){
                   currentNode = previousNode;
                   tempRootNode = previousNode;
                   while(cPtr_token != NULL){
                        if ( currentNode == sPtr_rootNAryTree ){
                             ui_noOfCharactersParsed = ui_noOfCharactersParsed+1; 
                        }
                        ui_tokenLength = strlen(cPtr_token);
                        /* Get a inode number from the free List */
                        ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
                        if( 0 == ui_inodeNo ){
                             printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                             return FAILURE;
                        }
                        /* Directory Entry Block Number */
                        ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
                        if( 0 == ui_dientryBlockNo ){
                             printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                             return FAILURE;
                        }
                        /* Path of the directory */
                        cPtr_filePath = (char *)malloc(sizeof(char)
                                                 *((ui_noOfCharactersParsed
                                                 +ui_tokenLength+3)) );
                        memset((void *)cPtr_filePath,'\0',
                       sizeof(char)*(ui_noOfCharactersParsed+ui_tokenLength+3));
                        strncpy(cPtr_filePath,
                                cPtr_directoryPath,
                                ui_noOfCharactersParsed);
                        if((*(cPtr_filePath+ui_noOfCharactersParsed-1) != '/')&&
                           (currentNode != sPtr_rootNAryTree) ){
                             strcpy(cPtr_filePath+ui_noOfCharactersParsed,"/");
                        }
                        /*Copy the details into the file descriptor structure */
                        s_newInode = s_createFileDescriptor(cPtr_token,
                                                            cPtr_filePath,
                                                            'd',ui_inodeNo,0,
                                                             ui_dientryBlockNo);
              
                        newNAryNode = s_getNAryTreeNode();
                        newNAryNode->s_inode = s_newInode;
                        previousNode = s_insertNAryTreeNode(currentNode,
                                                            newNAryNode);
                        currentNode = newNAryNode;
                       /*v_loadHashTable(ui_inodeNo,cPtr_token,cPtr_filePath);*/
                        /* Update Binary Search Tree */
                        strncpy(cPtr_filePath
                               +ui_noOfCharactersParsed,cPtr_token,ui_tokenLength);
                        strcpy(cPtr_filePath
                               +ui_noOfCharactersParsed+1+ui_tokenLength,"/");
                        *(cPtr_filePath 
                          + ui_noOfCharactersParsed+ui_tokenLength+3) = '\0';
                        newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
                        sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

                        ui_noOfCharactersParsed = ui_noOfCharactersParsed
                                                  + ui_tokenLength+1;
                        cPtr_token = strtok(NULL,"/");
                        if(NULL != cPtr_filePath){
              		         free(cPtr_filePath);
                        }
                   }/* End of Inner while */
                                        
              } else {
                   /* This variable helps in creating the file path when a new 
                      directory is found.
                   */
                   ui_tokenLength = strlen(cPtr_token);
                   ui_noOfCharactersParsed = ui_noOfCharactersParsed 
                                        + ui_tokenLength + 1;
                   cPtr_token = strtok(NULL,"/");
              }/* end of if(NULL == currentNode) else */
         }/* End of Outer while */
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,
                                            cPtr_directoryName,
                                            NONRECURSIVE);
         if(NULL == currentNode){
              currentNode = previousNode;
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              if( 0 == ui_inodeNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              if( 0 == ui_dientryBlockNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)
                                             *(ui_dirPathLength
                                             +ui_dirNameLength+3));
              memset((void *)cPtr_filePath,'\0',
                            sizeof(char)*(ui_dirPathLength+ui_dirNameLength+3));
              strncpy(cPtr_filePath,cPtr_directoryPath,ui_dirPathLength);
              if( *(cPtr_filePath+ui_dirPathLength-1) != '/'){
                   strcpy(cPtr_filePath+ui_dirPathLength,"/");
              }
              
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,
                                                  cPtr_filePath,
                                                  'd',ui_inodeNo,0,
                                                  ui_dientryBlockNo);
              newNAryNode = s_getNAryTreeNode();
              newNAryNode->s_inode = s_newInode;
              previousNode = s_insertNAryTreeNode(currentNode,newNAryNode);
              currentNode = newNAryNode;
              /*v_loadHashTable(ui_inodeNo,cPtr_directoryName,cPtr_filePath);*/
              if( *(cPtr_filePath+ui_dirPathLength) != '/'){
                   strncpy(cPtr_filePath+ui_dirPathLength,
                           cPtr_directoryName,
                           ui_dirNameLength);
                  strcpy(cPtr_filePath+ui_dirPathLength+ui_dirNameLength+1,"/");
              } else {
                   strncpy(cPtr_filePath+ui_dirPathLength+1,
                           cPtr_directoryName,
                           ui_dirNameLength);
                strcpy(cPtr_filePath+ui_dirPathLength+1+ui_dirNameLength+1,"/");
              }
              
              *(cPtr_filePath+ui_dirPathLength+ui_dirNameLength+3) = '\0';
              /* Update Binary Search Tree */
              newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);
              if(NULL != cPtr_filePath){
                   free(cPtr_filePath);
              }
         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,
                                cPtr_directoryName)){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_03);
              return FAILURE;
         } /* end of if(NULL == currentNode) else */         
    } /* end of outer if-else */
    if( NULL != cPtr_copyOfDirPath ){
         free(cPtr_copyOfDirPath);
    }
    return SUCCESS;
}

