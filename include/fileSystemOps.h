#ifndef _FILE_SYSTEM_OPS_
#define _FILE_SYSTEM_OPS_
/*
Group No : 28
Version No : 1.0
*/

#include "global.h"

#define CREATE_DEBUG 0

#define MOUNT_DEBUG 0

#define UNMOUNT_DEBUG 0

#define VFSLABELLENGTH 30

struct mainHeader{
    char c_fileSystemLabel[VFS_NAMELEN];
    unsigned int ui_uniqueNo;
    unsigned int ui_maxNoOfFileDescriptors;
    unsigned int ui_totalBlocks;
    unsigned int ui_startBlockNoInode;
    unsigned int ui_endBlockNoInode;
    unsigned int ui_startBlockNoDataBlock;
    unsigned int ui_endBlockNoDataBlock;
    unsigned int ui_noOfUsedFileDescriptors;
    unsigned int ui_startFreeBlockNoOfInodeArray;
    unsigned int ui_endFreeBlockNoOfInodeArray;
    unsigned int ui_startFreeBlockNoOfDataBlock;
    unsigned int ui_endFreeBlockNoOfDataBlock;
};

struct directoryEntry{
    unsigned int ui_inodeNo;
    char c_fileName[VFS_NAMELEN];
};

int i_createvfs(char *,unsigned int);

void v_displaySuperBlock(char *);

int i_mountvfs(char *);

int i_unmountvfs(char *);

#endif


