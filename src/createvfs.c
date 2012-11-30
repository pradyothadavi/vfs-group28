#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "global.h"
#include "fileSystemOps.h"
#include "block.h"
#include "vfs_errorcodes.h"

/*
Function Name: i_createvfs
Description: It creates a file system with the specified name and specified 
             size.
Parameters: It takes two parameters
            1)vfsLabel, which is the name of the VFS
            2)size, which is the size of the VFS ( in KB )
Return Type: int
             0 ---> If any error occurs
             1 ---> If file system is created successfully
*/
int i_createvfs(char *vfsLabel,unsigned int ui_size){

    char *cPtr_token = NULL;
    int i = 0;
    FILE *fpVfs = NULL;
    struct mainHeader s_superBlock;
    struct fileDescriptor s_inode;
    struct dataBlock s_block;
    struct directoryEntry s_dientry;

    /* Name of a virtual file system is said to be invalid, if the name has the 
       character '/' in it.
    */
    if( ( NULL != ( cPtr_token = strtok(vfsLabel,"/") ) ) && 
        ( 0 < strcmp(cPtr_token,vfsLabel) ) ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_04);
         return FAILURE;
    }

    /* Virtual file system of size 1024Kb cannot be created. */
    if( ui_size <= 0 || ui_size > 1024 ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_05);
         return FAILURE;
    }

    /* Name of the virtual file system cannot exceed 30 characters. */
    if( VFSLABELLENGTH < strlen(vfsLabel) ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_07);
         return FAILURE;
    }

    /* A virtual file system is already exists with the same name. */
    if( NULL != (fpVfs = fopen(vfsLabel,"rb")) ){
         fclose(fpVfs);
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_01);
         return FAILURE;
    }

    /* Cannot create a virtual file system. */
    if((fpVfs = fopen(vfsLabel,"wb+")) == NULL){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_02);
         return FAILURE;
    }

    ui_size = ui_size * 1024; /* Convert the file size into bytes */
    s_superBlock.ui_totalBlocks = ui_size/VFS_BLOCKSIZE; /* Block size is 1Kb */

    for(i = 0; i < s_superBlock.ui_totalBlocks; i++){
         memset((void *)&s_block,'\0',VFS_BLOCKSIZE);
         fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs);
    }

    /* Super block is the first block in the virtual file system, which has the 
       meta-data of the entire file system. The maximum file size that can be 
       stored in this file system is 16Kb (VFS_DIRECT_DATABLOCK = 16 ). When a 
       file system is created, by default four directories are formed. They are
       1) 0 ----> Standard Input
       2) 1 ----> Standard Output
       3) / ----> Root
       4) lost+found ----> When the file system crashes, the data goes into this 
                           from where it can be recoverd.

       The super block information is stored at Block No 0 in the file system.
    */
    strcpy(s_superBlock.c_fileSystemLabel,vfsLabel);
    s_superBlock.ui_uniqueNo = VFS_UNIQUE_FILE_SYSTEM_NO;

    s_superBlock.ui_maxNoOfFileDescriptors = 
                              s_superBlock.ui_totalBlocks/VFS_DIRECT_DATABLOCK;
    s_superBlock.ui_startBlockNoInode = VFS_INODE_STARTBLOCK;
    s_superBlock.ui_endBlockNoInode = 
              VFS_INODE_STARTBLOCK + s_superBlock.ui_maxNoOfFileDescriptors - 2;

    s_superBlock.ui_startBlockNoDataBlock = s_superBlock.ui_maxNoOfFileDescriptors;
    s_superBlock.ui_endBlockNoDataBlock = s_superBlock.ui_totalBlocks;

    s_superBlock.ui_noOfUsedFileDescriptors = 4;       
    s_superBlock.ui_startFreeBlockNoOfInodeArray = VFS_ACTUAL_INODE_STARTBLOCK;
    s_superBlock.ui_endFreeBlockNoOfInodeArray = s_superBlock.ui_endBlockNoInode;

    s_superBlock.ui_startFreeBlockNoOfDataBlock = 
                                      s_superBlock.ui_startBlockNoDataBlock + 2;
    s_superBlock.ui_endFreeBlockNoOfDataBlock = 
                                            s_superBlock.ui_endBlockNoDataBlock;

    if((fseek(fpVfs,0,SEEK_SET))==-1){
         printf("ERROR: Cannot Travese To The First Data Block \n");
         printf("STATUS: Program Terminated \n");
         exit(1);     
    }

    if( (fwrite(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1 ){
         printf("ERROR: Could not write the superblock \n");
         printf("STATUS: Program Terminated \n");
         exit(1);
    }


    /* Populate the root directory information. The variable ui_locationData-
       -BlockNo hold the block numbers of the dientry table for directories and
       in the case of regular files, it holds the block numbers where the actual
       data of the file is stored.
    */
    memset((void *)&s_inode,0,sizeof(struct fileDescriptor));
    strcpy(s_inode.cptr_fileName,"/");
    strcpy(s_inode.cptr_filePath,"/");
    s_inode.c_fileType[0]='d';
    s_inode.c_fileType[1]='\0';
    s_inode.ui_inodeNo = 3;
    s_inode.ui_fileSize = 0;
    s_inode.ui_locationDataBlockNo[0] = s_superBlock.ui_startBlockNoDataBlock;

    if((fseek(fpVfs,
              VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+2*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if( (fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR:Could not write the / information to the data block \n");
    }

    if((fseek(fpVfs,
              s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }

    memset((void *)&s_block,0,VFS_BLOCKSIZE);
    if( (fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing A Data Block \n");
    }
    if((fseek(fpVfs,
              s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }
    s_dientry.ui_inodeNo = 4;
    strcpy(s_dientry.c_fileName,"lost+found");
    if( (fwrite(&s_dientry,sizeof(struct directoryEntry),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing Directory Entry \n");
    }
    s_inode.ui_fileSize = s_inode.ui_fileSize + sizeof(struct directoryEntry);

    if((fseek(fpVfs,
              VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+2*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if( (fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR:Could not write the / information to the data block \n");
    }

    /* Populate the information of lost+found directory. */
    memset((void *)&s_inode,0,sizeof(struct fileDescriptor));
    strcpy(s_inode.cptr_fileName,"lost+found");
    strcpy(s_inode.cptr_filePath,"/lost+found");
    s_inode.c_fileType[0]='d';
    s_inode.c_fileType[1]='\0';
    s_inode.ui_inodeNo = 4;
    s_inode.ui_fileSize = 0;
    s_inode.ui_locationDataBlockNo[0] = s_superBlock.ui_startBlockNoDataBlock+1;

    if((fseek(fpVfs,
              VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+3*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if( (fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR:Could not write the lost+found directory information to the data block \n");
    }

    if((fseek(fpVfs,
              s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,
              SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }

    memset((void *)&s_block,0,VFS_BLOCKSIZE);
    if( (fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing A Data Block \n");
    }

    if(NULL != fpVfs){
         fclose(fpVfs);
    }

#if CREATE_DEBUG
    v_displaySuperBlock(vfsLabel);
#endif

    return SUCCESS;
}

/*
Function Name: v_displaySuperBlock
Description: It displays the super block information about a particular file 
             system on the console.
Parameters: It takes the name of the file system whose information is to be 
            displayed. 
Return Type: void
*/
void v_displaySuperBlock(char *cPtr_fileSysName){

		/* Declaration Section */
    int i = 0;
        
    FILE *fpVfs = NULL;

    struct mainHeader s_testSuperBlock;

		/* End of declaration Section */

		/* Begining of Module Logic */

    if( NULL == (fpVfs = fopen(cPtr_fileSysName,"rb")) ){
         printf("ERROR:Cannot Open The File System \n");
         return;
    }
    fseek(fpVfs,0,SEEK_SET);
    if((fread(&s_testSuperBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("DEBUG: Error in reading superblock \n");   
    }
    if(NULL != fpVfs){
         fclose(fpVfs);
    }

    printf("DEBUG: Details of File System \n");
    printf("       FILE SYSTEM NAME: ");
    i = 0;
    while(s_testSuperBlock.c_fileSystemLabel[i] != '\0'){
         printf("%c",s_testSuperBlock.c_fileSystemLabel[i]);
         i++;
    }       
    printf("\n");
    printf("       FILE SYSTEM ID: %d \n",s_testSuperBlock.ui_uniqueNo);
    printf("       MAX FILE DESCRIPTORS: %d \n",s_testSuperBlock.ui_maxNoOfFileDescriptors);
    printf("       TOTAL DATA BLOCKS: %d \n",s_testSuperBlock.ui_totalBlocks);
    printf("       INODE START BLOCK NO: %d \n",s_testSuperBlock.ui_startBlockNoInode);
    printf("       INODE END BLOCK NO: %d \n",s_testSuperBlock.ui_endBlockNoInode);
    printf("       DATA START BLOCK NO: %d \n",s_testSuperBlock.ui_startBlockNoDataBlock);
    printf("       DATA END BLOCK NO: %d \n",s_testSuperBlock.ui_endBlockNoDataBlock);
    printf("       USED FILE DESCRIPTORS: %d \n",s_testSuperBlock.ui_noOfUsedFileDescriptors);
    printf("       FREE INODE START BLOCK NO: %d \n",s_testSuperBlock.ui_startFreeBlockNoOfInodeArray);
    printf("       FREE INODE END BLOCK NO: %d \n",s_testSuperBlock.ui_endFreeBlockNoOfInodeArray);
    printf("       FREE DATA BLOCK START NO: %d \n",s_testSuperBlock.ui_startFreeBlockNoOfDataBlock);
    printf("       FREE DATA BLOCK END NO: %d \n",s_testSuperBlock.ui_endFreeBlockNoOfDataBlock);

}
