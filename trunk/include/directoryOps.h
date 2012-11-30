#ifndef _DIRECTORY_OPS_
#define _DIRECTORY_OPS_

#define DIROPS_DEBUG 0

#include "fileDescriptor.h"

struct s_directoryPath{
    char c_filePath[VFS_FILEPATHLEN];
    struct s_directoryPath *sPtr_nextBlock;
};

int i_makedir(char *,char *);

int i_deletedir(char *);

int i_movedir(char *,char *);

int i_listdir(char *,int,char *);

int i_printDirectoryContents(struct nAryTreeNode *,char *);

struct s_directoryPath *s_storeDirectoryPath(struct s_directoryPath *,struct nAryTreeNode *);

struct s_directoryPath *s_getDirectoryPathNode(char *);

void v_changePaths(struct nAryTreeNode *,int);

#endif
