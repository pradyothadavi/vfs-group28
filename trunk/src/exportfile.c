#include "global.h"
#include "fileOps.h"

int i_exportfile(char *P1,char *P2){

     int i_retVal = 0;
     i_retVal = i_listfile(P1,P2,REGULARFILE);
     if( SUCCESS == i_retVal ){
         return SUCCESS;
     } else {
         return FAILURE;
     }
}
