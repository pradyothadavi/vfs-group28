#define ERR_VFS_CREATE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_CREATE_01 "DATA_FILE_ALREADY_EXISTS"
#define ERR_VFS_CREATE_02 "CANNOT_CREATE_DATAFILE"
#define ERR_VFS_CREATE_03 "NOT_ENOUGH_MEMORY"
#define ERR_VFS_CREATE_04 "INVALID_CHARACTER_IN_NAME"
#define ERR_VFS_CREATE_05 "INVALID_SIZE"  
#define ERR_VFS_CREATE_07 "VFS_LABEL_TOO_LARGE"

#define ERR_VFS_MOUNT_01 "DATA_FILE_NOT_FOUND"
#define ERR_VFS_MOUNT_02 "CANNOT_READ_FROM_FILE"
#define ERR_VFS_MOUNT_03 "INVALID_HEADER"
#define ERR_VFS_MOUNT_04 "VFS_ALREADY_MOUNTED" 
#define ERR_VFS_MOUNT_05 "VFS_INSUFFICIENT_ARGUMENTS"

#define ERR_VFS_UNMOUNT_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_UNMOUNT_01 "DATA_FILE_NOT_FOUND"
#define ERR_VFS_UNMOUNT_02 "ACCESS_DENIED"
#define ERR_VFS_UNMOUNT_03 "CANNOT_WRITE_TO_FILE"
#define ERR_VFS_UNMOUNT_04 "VFS_NOT_MOUNTED"

#define ERR_VFS_MAKEDIR_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_MAKEDIR_01 "FILESYSTEM_FULL"
#define ERR_VFS_MAKEDIR_02 "INVALID_CHARACTER_IN_DIRNAME"
#define ERR_VFS_MAKEDIR_03 "DIRECTORY_ALREADY_EXISTS"
#define ERR_VFS_MAKEDIR_05 "VFS_NOT_MOUNTED"

#define ERR_VFS_DELETEDIR_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_DELETEDIR_01 "CANNOT_FIND_SPECIFIED_PATH_OR_DIR"
#define ERR_VFS_DELETEDIR_02 "DIRECTORY_IS_NOT_EMPTY"
#define ERR_VFS_DELETEDIR_03 "INVALID_DIRNAME"
#define ERR_VFS_DELETEDIR_04 "VFS_NOT_MOUNTED" 

#define ERR_VFS_MOVEDIR_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_MOVEDIR_01 "CANNOT_FIND_SPECIFIED_SOURCEDIR"
#define ERR_VFS_MOVEDIR_02 "CANNOT_FIND_SPECIFIED_DESTINATIONDIR" /*(if specified dir is not there, it is not created in this case 
                                                                     unlike in add dir) */ 
#define ERR_VFS_MOVEDIR_03 "SOURCE_PATH_MISSING"
#define ERR_VFS_MOVEDIR_04 "DESTINATION_PATH_MISSING"
#define ERR_VFS_MOVEDIR_05 "DESTINATION_ALREADY_HAVE_SOURCE_DIR"
#define ERR_VFS_MOVEDIR_06 "CANNOT_MOVE_PARENT_TO_CHILD_DIR"
#define ERR_VFS_MOVEDIR_07 "DESTINATION_CANNOT_BE_FILE" 
#define ERR_VFS_MOVEDIR_08 "VFS_NOT_MOUNTED"

#define ERR_VFS_LISTDIR_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_LISTDIR_01 "CANNOT_FIND_SPECIFIED_PATH_OR_DIR"
#define ERR_VFS_LISTDIR_02 "INVALID_FLAG"
#define ERR_VFS_LISTDIR_03 "CANNOT_FIND_OUTPUTFILE"
#define ERR_VFS_LISTDIR_04 "VFS_NOT_MOUNTED"

#define ERR_VFS_ADDFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_ADDFILE_01 "UNABLE_TO_CREATE_NEWFILE"
#define ERR_VFS_ADDFILE_02 "INVALID_CHARACTER_IN_FILENAME"
#define ERR_VFS_ADDFILE_03 "FILE_ALREADY_EXISTS"
#define ERR_VFS_ADDFILE_04 "FILE_SYSTEM_FULL"   
#define ERR_VFS_ADDFILE_05 "CANNOT_OPEN_SPECIFIED_DATAFILE" 
#define ERR_VFS_ADDFILE_06 "FILE_TOO_LARGE"
#define ERR_VFS_ADDFILE_07 "VFS_NOT_MOUNTED"

#define ERR_VFS_LISTFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_LISTFILE_01 "SOURCE_FILE_PATH_NOT_FOUND"
#define ERR_VFS_LISTFILE_02 "OUTPUT_FILE_PATH_NOT_FOUND"
#define ERR_VFS_LISTFILE_03 "SOURCE_PATH_MISSING"
#define ERR_VFS_LISTFILE_04 "OUTPUT_PATH_MISSING"
#define ERR_VFS_LISTFILE_05 "NOT_A_TEXT_FILE"
#define ERR_VFS_LISTFILE_06 "VFS_NOT_MOUNTED"

#define ERR_VFS_UPDATEFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_UPDATEFILE_01 "SOURCE_FILE_NOT_FOUND"
#define ERR_VFS_UPDATEFILE_02 "DEST_FILE_NOT_FOUND"
#define ERR_VFS_UPDATEFILE_03 "INCORRECT_FILE_FORMAT"
#define ERR_VFS_UPDATEFILE_04 "FILE_TOO_LARGE"
#define ERR_VFS_UPDATEFILE_06 "VFS_NOT_MOUNTED"

#define ERR_VFS_REMOVEFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_REMOVEFILE_01 "CANNOT_FIND_SPECIFIED_PATH_OR_FILE"
#define ERR_VFS_REMOVEFILE_02 "VFS_NOT_MOUNTED" 

#define ERR_VFS_MOVEFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_MOVEFILE_01 "CANNOT_FIND_SPECIFIED_SOURCEFILE"
#define ERR_VFS_MOVEFILE_02 "CANNOT_FIND_SPECIFIED_DESTINATION_PATH" /*DESTINATION_FILE CHANGED TO DESTINATION_PATH */
#define ERR_VFS_MOVEFILE_05 "DESTINATION_CANNOT_BE_FILE"
#define ERR_VFS_MOVEFILE_06 "VFS_NOT_MOUNTED" 

#define ERR_VFS_COPYFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_COPYFILE_01 "CANNOT_FIND_SPECIFIED_SOURCEFILE"
#define ERR_VFS_COPYFILE_02 "CANNOT_FIND_SPECIFIED_DESTINATIONFILE"
#define ERR_VFS_COPYFILE_05 "CANNOT_COPY_DIR_TO_FILE"
#define ERR_VFS_COPYFILE_06 "FILE_SYSTEM_FULL"
#define ERR_VFS_COPYFILE_07 "VFS_NOT_MOUNTED"

#define ERR_VFS_EXPORTFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_EXPORTFILE_01 "CANNOT_FIND_SPECIFIED_SOURCEFILE"
#define ERR_VFS_EXPORTFILE_02 "CANNOT_FIND_SPECIFIED_OUTPUTFILE"
#define ERR_VFS_EXPORTFILE_03 "SOURCE_PATH_MISSING"
#define ERR_VFS_EXPORTFILE_04 "OUTPUT_PATH_MISSING"
#define ERR_VFS_EXPORTFILE_05 "OUTPUT_FILE_ALREADY_EXISTS"
#define ERR_VFS_EXPORTFILE_06 "CANNOT_EXPORT_DIR"
#define ERR_VFS_EXPORTFILE_07 "VFS_NOT_MOUNTED"

#define ERR_VFS_SEARCHFILE_00 "VFS_INSUFFICIENT_ARGUMENTS"
#define ERR_VFS_SEARCHFILE_01 "CANNOT_CREATE_SPECIFIED_OUTPUTFILE"
#define ERR_VFS_SEARCHFILE_03 "VFS_NOT_MOUNTED"
