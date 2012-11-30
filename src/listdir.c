#include <stdio.h>
#include <string.h>

#include "global.h"
#include "directoryOps.h"
#include "vfs_errorcodes.h"

int i_listdir(char *cPtr_directoryPath,int flag,char *cPtr_sysPath){

    char *cPtr_token = NULL;
    char *cPtr_copyFilePath = NULL;
    int i_retVal = 0;
    FILE *fp = NULL;

    struct nAryTreeNode *currentNode = NULL;
    struct s_directoryPath *dirList = NULL;
    struct s_directoryPath *nodeToBeDeleted = NULL;

    if( flag < 0 || flag> 1 ){
         printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_02);
         return FAILURE;
    }

    if( NULL == (fp=fopen(cPtr_sysPath,"r")) ){
         printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_03);
         return FAILURE;
    } else {
         fclose(fp);
         fp=fopen(cPtr_sysPath,"w");
         fclose(fp);
    }

    currentNode = sPtr_rootNAryTree;
    if( 0 == strcmp(cPtr_directoryPath,"/") ){
         currentNode = sPtr_rootNAryTree;
    } else {
         cPtr_token = strtok(cPtr_directoryPath,"/");
         while( NULL != cPtr_token ){
              currentNode = s_searchNAryTreeNode(currentNode,
                                                 cPtr_token,
                                                 NONRECURSIVE);
              if( NULL == currentNode ){
                   printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_01);
                   return FAILURE;
              }
              cPtr_token = strtok(NULL,"/");
         }
    }/* end of if ( 0 == strcmp(cPtr_directoryPath,"/") ) else part */

    if( 0 == flag ){
         i_retVal = i_printDirectoryContents(currentNode,cPtr_sysPath);
         if( FAILURE == i_retVal ){
              return FAILURE;
         } 
    } else if ( 1 == flag ){ /* end of if(0 == flag ) */
         do{
              i_retVal = i_printDirectoryContents(currentNode,cPtr_sysPath);
              if( FAILURE == i_retVal ){
                   return FAILURE;
              }
              dirList = s_storeDirectoryPath(dirList,currentNode);
              nodeToBeDeleted = dirList;
              if( NULL != dirList ){
                   cPtr_copyFilePath = (char *)malloc(sizeof(char)*
                                                 strlen(dirList->c_filePath));
                   memset((void *)cPtr_copyFilePath,'\0',
                                                  strlen(dirList->c_filePath));
                   strcpy(cPtr_copyFilePath,dirList->c_filePath);
                   dirList = dirList->sPtr_nextBlock;
                   if(NULL != nodeToBeDeleted ){ 
                        nodeToBeDeleted->sPtr_nextBlock = NULL;
                        free(nodeToBeDeleted);
                   }
                   cPtr_token = strtok(cPtr_copyFilePath,"/");
                   while( NULL != cPtr_token ){
                        currentNode = s_searchNAryTreeNode(currentNode,
                                                           cPtr_token,
                                                           NONRECURSIVE);
                        cPtr_token = strtok(NULL,"/");
                   }/* end of while(NULL != cPtr_token ) */
                   if(NULL != cPtr_copyFilePath){
                        free(cPtr_copyFilePath);
                   }
              }/* end of if(NULL != dirList) */
         } while(dirList != NULL);
         i_retVal = i_printDirectoryContents(currentNode,cPtr_sysPath);
         if( FAILURE == i_retVal ){
              return FAILURE;
         }         
    }
    return SUCCESS;
}

/* 
Function Name:
Description:
Parameters:
Return Type:
*/
int i_printDirectoryContents(struct nAryTreeNode *sPtr_directory,
                             char *cPtr_sysPath){

    unsigned int ui_count = 0;
    FILE *fp = NULL;
    struct nAryTreeNode *currentNode = NULL;

    currentNode = sPtr_directory;

    if( NULL == (fp=fopen(cPtr_sysPath,"a")) ){
         printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_03);
         return FAILURE;
    }
         
    fprintf(fp,"%s%s : \n",currentNode->s_inode->cptr_filePath,
                                           currentNode->s_inode->cptr_fileName);
    if( NULL != currentNode->leftChild ){
         fprintf(fp,"%s ",currentNode->leftChild->s_inode->cptr_fileName);
         ui_count++;
         currentNode = currentNode->leftChild;
         while( NULL != currentNode->rightSibling){
              fprintf(fp,"%s ",currentNode->rightSibling->s_inode->cptr_fileName);
              ui_count++;
              currentNode = currentNode->rightSibling;
              if( 3 == ui_count ){
                   fprintf(fp,"\n");
              }
         }/* end of while(NULL != currentNode->leftChild) */
    }/* end of if(NULL != currentNode->leftChild) */
    if( NULL != fp ){
         fclose(fp);
    }

    return SUCCESS;
}

/* 
Function Name:
Description:
Parameters:
Return Type:
*/
struct s_directoryPath *s_storeDirectoryPath(struct s_directoryPath *dirList,
                                             struct nAryTreeNode *currentNode){

    struct nAryTreeNode *tempNode = NULL;
    struct s_directoryPath *current = NULL;
    struct s_directoryPath *dirNode = NULL;

    tempNode = currentNode;

    if( NULL == tempNode || NULL == tempNode->leftChild ){
         return dirList;
    } 

    if( NULL != tempNode->leftChild ){
         tempNode = tempNode->leftChild;
         if( tempNode->s_inode->c_fileType[0] == 'd' ){
              dirNode=s_getDirectoryPathNode(tempNode->s_inode->cptr_filePath);
              if( NULL == dirList ){
                   dirList = dirNode;
                   current = dirList;
              } else {
                   current = dirList;
                   while( NULL != current->sPtr_nextBlock ){
                        current = current->sPtr_nextBlock;
                   }
                   current = dirNode;
              }/* end of if(NULL == dirList) */
         }/* end of if( tempNode->s_inode->c_fileType[0] == 'd' ) */
         while( NULL != tempNode->rightSibling ){
              tempNode = tempNode->rightSibling;
              if( tempNode->s_inode->c_fileType[0] == 'd' ){
                   dirNode = s_getDirectoryPathNode(
                                              tempNode->s_inode->cptr_filePath);
                   if( current != NULL ){
                        current->sPtr_nextBlock = dirNode;
                        current = current->sPtr_nextBlock;
                   }
              }/* end of if( tempNode->s_inode->c_fileType[0] == 'd' ) */
         }/* end of while(NULL != temp->rightSibling) */
         current = NULL;/* Make current to NULL as it is pointing to some node*/
    }/* end of if( NULL != tempNode->leftChild) */

    return dirList;
}

/* 
Function Name:
Description:
Parameters:
Return Type:
*/
struct s_directoryPath *s_getDirectoryPathNode(char *cPtr_directoryPath){

    struct s_directoryPath *temp = NULL;

    temp = (struct s_directoryPath *)malloc(sizeof(struct s_directoryPath)*1);

    strcpy(temp->c_filePath,cPtr_directoryPath);
    temp->sPtr_nextBlock = NULL;

    return temp;
}
