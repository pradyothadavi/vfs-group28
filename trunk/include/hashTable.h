#ifndef _HASH_TABLE_
#define _HASH_TABLE_

/*
Group No: 028
Version No: 1.0
*/

#include"global.h"
                        
struct chain{
    char c_fileName[VFS_NAMELEN];
    char c_filePath[VFS_FILEPATHLEN];
    int i_inodeNo;
    struct chain *next;
};



void v_initializeHashTable();

void v_loadHashTable(unsigned int inode,char *,char *);

int i_calculateIndex(char *);

struct chain* searchHashTable(char*,char*);

void v_delete(int,char*);

struct chain *addToList(struct chain *,char *fileArray);

void v_populateDataStructures(struct nAryTreeNode *);

#endif


