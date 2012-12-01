/*
Version No: 1.0
Application Name: Virtual File System
*/

/*
    The file contains the code which is the entry point of the application.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "main.h"
#include "vfs_errorcodes.h"
#include "nAryTree.h"
#include "binarySearchTree.h"
#include "fileOps.h"

unsigned int ui_mountFlag = 0;

/*
Function Name: main
Description: It is the entry point of the application, which calls different 
             modules based on the command.
Parameters: It takes 2 parameters from the command line
            1) No.of.command line arguments
            2) A file path to a text file, which has all the commands to be 
               executed.
Return Type: It return a integer type, to the operating system
             0 --> If programs exits successfully
             Greater than 0 --> On abnormal termination
*/
int main( int argc, char *argv[] ){
    
    FILE *scriptfp;
    
    char linebuffer[BUFSIZE];
    char command[CMDSIZE], par1[PARSIZE], par2[PARSIZE], par3[PARSIZE];
    char *token;

    if( argc != 2 ){
         fprintf(stderr,"Usage: vfsdriver <scriptfile>\n");
	 return(1);
    }

    if( (scriptfp=fopen(argv[1],"r")) == NULL ){
         fprintf(stderr,"Unable to open script file: %s\n", argv[1]);
	 return(2);
    }

    while( fgets(linebuffer, sizeof(linebuffer), scriptfp) != NULL ){
	 /* This output is for debugging... do not uncomment in final version */
#if MAIN_DEBUG
	 printf("==================================================\n");
	 printf("Processing: %s", linebuffer);
#endif

	 /* Remove the extra newline character in the end of line */
	 linebuffer[ strlen(linebuffer)-1 ] = '\0';
		
	 /* Get the command and the parameters using tokenizer */
	 strcpy( command, (token = strtok(linebuffer, " ")) == NULL ? "" : token );
		
	 strcpy( par1, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par2, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par3, (token = strtok(NULL, " ")) == NULL ? "" : token );
#if MAIN_DEBUG
      printf("Command:%s:p1:%s:p2:%s:p3:%s\n",command, par1, par2, par3);
      printf("==================================================\n");
#endif

	 processcommand( command, par1, par2, par3 );
    }

    fclose(scriptfp);

    v_freeResources();

    return 0;
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void v_freeResources(){

    if( NULL != sPtr_rootBST ){
         v_deleteBST(sPtr_rootBST);
         sPtr_rootBST = NULL;
    }
    if( NULL != sPtr_rootNAryTree ){
         v_deleteNAryTreeNode(sPtr_rootNAryTree);
         sPtr_rootNAryTree = NULL;
    }
    if( NULL != s_dataBlockFreeList ){
         v_deleteList(s_dataBlockFreeList);
         s_dataBlockFreeList = NULL;
    }    
    if( NULL != s_inodeBlockFreeList ){
         v_deleteList(s_inodeBlockFreeList);
         s_inodeBlockFreeList = NULL;
    }
    if( NULL != cPtr_nameOfVfsMounted){
         free(cPtr_nameOfVfsMounted);
         cPtr_nameOfVfsMounted = NULL;
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void createvfs ( char *P1, int P2 ){
 
    int i_retVal = 0;

    i_retVal = i_createvfs(P1,P2);
    if( SUCCESS == i_retVal ){
         printf("createvfs_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void mountvfs ( char *P1 ){

    int i_retVal = 0;

    i_retVal = i_mountvfs(P1);
    if( SUCCESS == i_retVal ){
         ui_mountFlag = 1;
         printf("mountvfs_SUCCESS \n");
#if MAIN_DEBUG
         v_traverseNAryTree(sPtr_rootNAryTree,INODENUM);
         v_traverseBST(sPtr_rootBST,FILEPATH);
#endif
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void unmountvfs ( char *P1 ){

    int i_retVal = 0;

    i_retVal = i_unmountvfs(P1);
    if( SUCCESS == i_retVal ){
         ui_mountFlag = 0;
         printf("unmountvfs_SUCCESS \n");
         v_freeResources();
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void makedir ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_makedir(P1,P2);
    if( SUCCESS == i_retVal ){
#if MAIN_DEBUG
         v_traverseNAryTree(sPtr_rootNAryTree,FILEPATH);
#endif
         printf("makedir_SUCCESS \n");
#if MAIN_DEBUG
         v_traverseBST(sPtr_rootBST,FILEPATH);
#endif
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void deletedir ( char *P1 ){	

    int i_retVal = 0;

    i_retVal = i_deletedir(P1);
    if( SUCCESS == i_retVal ) {
         printf("deletedir_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void movedir ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_movedir(P1,P2);
    if( SUCCESS == i_retVal ) {
         printf("movedir_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void listdir ( char *P1, int P2, char *P3 ){

    int i_retVal = 0;

    i_retVal = i_listdir(P1,P2,P3);
    if( SUCCESS == i_retVal ) {
         printf("listdir_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void addfile ( char *P1, char *P2, char *P3 ){

    int i_retVal = 0;

    i_retVal = i_addfile(P1,P2,P3);
    if( SUCCESS == i_retVal ) {
#if MAIN_DEBUG
         v_traverseNAryTree(sPtr_rootNAryTree,FILEPATH);
#endif
         printf("addfile_SUCCESS \n");
#if MAIN_DEBUG
         v_traverseBST(sPtr_rootBST,FILEPATH);
#endif
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void listfile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_listfile(P1,P2,TEXTFILE);
    if( SUCCESS == i_retVal ) {
         printf("listfile_SUCCESS \n");
    }	
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void updatefile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_updatefile(P1,P2);
    if( SUCCESS == i_retVal ) {
         printf("updatefile_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void removefile ( char *P1 ){

    int i_retVal = 0;

    i_retVal = i_removefile(P1);
    if( SUCCESS == i_retVal ) {
         printf("removefile_SUCCESS \n");
    }	

}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void movefile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_movefile(P1,P2);
    if( SUCCESS == i_retVal ) {
         printf("movefile_SUCCESS \n");
    }	
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void copyfile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_copyfile(P1,P2);
    if( SUCCESS == i_retVal ) {
         printf("copyfile_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void exportfile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_exportfile(P1,P2);
    if( SUCCESS == i_retVal ) {
         printf("exportfile_SUCCESS \n");
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void searchfile ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_searchfile(P1,P2);
    
    printf("searchfile_SUCCESS %d\n",i_retVal);
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void processcommand( char *command, char *P1, char *P2, char *P3 ){
	
    if( strcmp(command, "createvfs") == 0 ){
	 int size = atoi(P2);
         if( (0 == strcmp(P1,"")) || 0 == P2 ){
              printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_00);
         } else {
              createvfs (P1,size);
         }
    }
    else if( strcmp(command, "mountvfs") == 0 ){
         if( (0 == strcmp(P1,"")) ){
              printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_05);
         } else {
              if( 1 == ui_mountFlag ){
                   printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_04);
              } else {
                   mountvfs (P1);
              }
         }
    }
    else if( strcmp(command, "unmountvfs") == 0 ){
         if( (0 == strcmp(P1,"")) ){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_04);
              } else {
                   unmountvfs (P1);
              }
         }
    }
    else if( strcmp(command, "makedir") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_05);
              } else {
                   makedir (P1,P2);
              }
         } 
    }
    else if( strcmp(command, "deletedir") == 0 ){
         if( 0 == strcmp(P1,"") ){
              printf("deletedir_FAILURE %s \n",ERR_VFS_DELETEDIR_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("deletedir_FAILURE %s \n",ERR_VFS_DELETEDIR_04);
              } else {
                   deletedir (P1);
              }
         }
    }
    else if( strcmp(command, "movedir") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("movedir_FAILURE %s \n",ERR_VFS_MOVEDIR_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("movedir_FAILURE %s \n",ERR_VFS_MOVEDIR_08);
              } else {
                   movedir (P1,P2);
              }
         }
    }
    else if( strcmp(command, "listdir") == 0 ){
         int flag = atoi(P2);
         if( 0 == strcmp(P1,"") || 0 == strcmp(P3,"") ){
              printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_04);
              } else {
                   listdir (P1,flag,P3);
              }
         }
    }
    else if( strcmp(command, "addfile") == 0 ){
         if( 0 == strcmp(P1,"") || 0 == strcmp(P2,"") || 0 == strcmp(P3,"") ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_00);
              } else {
                   addfile (P1,P2,P3);
              }
         }
    }
    else if( strcmp(command, "listfile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("listfile_FAILURE %s \n",ERR_VFS_LISTFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("listfile_FAILURE %s \n",ERR_VFS_LISTFILE_06);
              } else {
                   listfile (P1,P2);
              }
         }
    }
    else if( strcmp(command, "updatefile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("updatefile_FAILURE %s \n",ERR_VFS_UPDATEFILE_06);
              } else {
                   updatefile (P1,P2);
              }
         }
    }
    else if( strcmp(command, "removefile") == 0 ){
         if( 0 == strcmp(P1,"") ){
              printf("removefile_FAILURE %s \n",ERR_VFS_REMOVEFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("removefile_FAILURE %s \n",ERR_VFS_REMOVEFILE_00);
              } else {
                   removefile (P1);
              }
         }
    }
    else if( strcmp(command, "movefile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("movefile_FAILURE %s \n",ERR_VFS_MOVEFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("movefile_FAILURE %s \n",ERR_VFS_MOVEFILE_06);
              } else {
                   movefile (P1,P2);
              }
         }
    }
    else if( strcmp(command, "copyfile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("copyfile_FAILURE %s \n",ERR_VFS_COPYFILE_07);
              } else {
                   copyfile (P1,P2);
              }
         }
    }
    else if( strcmp(command, "exportfile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("exportfile_FAILURE %s \n",ERR_VFS_EXPORTFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("exportfile_FAILURE %s \n",ERR_VFS_EXPORTFILE_07);
              } else {
                   exportfile (P1,P2);
              }
         }
    }
    else if( strcmp(command, "searchfile") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("searchfile_FAILURE %s \n",ERR_VFS_SEARCHFILE_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("searchfile_FAILURE %s \n",ERR_VFS_SEARCHFILE_03);
              } else {
                   searchfile (P1,P2);
              }
         }
    }
    else
	 printf("Ignoring invalid command %s\n", command);
}
