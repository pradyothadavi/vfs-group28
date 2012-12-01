#ifndef _FILE_OPS_
#define _FILE_OPS_

#define FILEOPS_DEBUG 0

#define TEXTFILE 101
#define REGULARFILE 102

int i_addfile(char *,char *,char *);

int i_listfile(char *,char *,int);

int i_updatefile(char *,char *);

int i_removefile(char *);

int i_movefile(char *,char *);

int i_copyfile(char *,char *);

int i_exportfile(char *,char *);

int i_searchfile(char *,char *);

#endif
