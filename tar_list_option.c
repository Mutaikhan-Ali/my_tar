#include "source.h"

void t_option(t_tar_info* tar)
{
    int fd_archieve = open_archieve(tar);
    
    if(fd_archieve == -1)
    {
        printf("my_tar: error: '%s': tar file not found\n", tar->tarFile_name);
        return;
    }
    // 1st file name
    
    get_entry_names_of_tar(tar, fd_archieve);
    print_name_list(tar->t_args);
    free_name_list(tar->t_args);
    
    if(tar->t_header != NULL)
    {
        free(tar->t_header);
    }
}