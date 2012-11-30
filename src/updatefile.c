#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "global.h"
#include "vfs_errorcodes.h"

int i_updatefile(char *cPtr_filePath,char *cPtr_sysPath){

    char *cPtr_token = NULL;
    char c;
    unsigned int ui_size = 0;
    unsigned int ui_updatePos = 0;
    struct nAryTreeNode *currentNode = NULL;
    struct dataBlock s_buffer;
    FILE *fp = NULL;
    FILE *fpVfs = NULL;

    if(NULL == (fp=fopen(cPtr_sysPath,"r")) ){
         printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_02);
         return FAILURE;
    } else {
         fseek(fp,0,SEEK_END);
         ui_size = ftell(fp);
         fclose(fp);
         if( VFS_BLOCKSIZE < ui_size ){
              printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_04);
              return FAILURE;
         }
    }

    currentNode = sPtr_rootNAryTree;
    cPtr_token = strtok(cPtr_filePath,"/");
    while( NULL != cPtr_token ){
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
         if( NULL == currentNode ){
              printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_01);
              return FAILURE;
         }
         cPtr_token = strtok(NULL,"/");
    }

    if( NULL == (fpVfs=fopen(cPtr_nameOfVfsMounted,"rb")) ){
         printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_06);
         return FAILURE;
    }   
    fseek(fpVfs,currentNode->s_inode->ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET);
    ui_updatePos = 0;
    while( (c=fgetc(fpVfs)) != '\0' ){
         ui_updatePos++;
    }
    fclose(fpVfs);   

    if(NULL == (fp=fopen(cPtr_sysPath,"r")) ){
         printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_02);
         return FAILURE;
    }
    memset((void *)&s_buffer,'\0',sizeof(struct dataBlock));
    fread(&s_buffer,sizeof(struct dataBlock),1,fp);
    fclose(fp); 
    if( NULL == (fpVfs=fopen(cPtr_nameOfVfsMounted,"rb")) ){
         printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_06);
         return FAILURE;
    }   
    fseek(fpVfs,
    ((currentNode->s_inode->ui_locationDataBlockNo[0]*VFS_BLOCKSIZE)),
    SEEK_SET);
    printf("Location Block %u \n",currentNode->s_inode->ui_locationDataBlockNo[0]);
    printf("Position %ld \n",ftell(fpVfs));
    ui_updatePos = 0;
    while(s_buffer.ui_dataBlock[ui_updatePos] != '\0'){
         printf("%c",s_buffer.ui_dataBlock[ui_updatePos]);
         ui_updatePos++;
    }
    fwrite(&s_buffer,sizeof(struct dataBlock),1,fpVfs);
    printf("Position %ld \n",ftell(fpVfs));
    fclose(fpVfs);
    return SUCCESS;
}
