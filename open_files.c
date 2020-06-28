#include "source.h"

int open_archieve(t_tar_info* tar)  // returns file descriptor
{
    const int create = tar->tarFlags->c;
    const int r_append = tar->tarFlags->r;
    const int t_option = tar->tarFlags->t;
    const int extract = tar->tarFlags->x;
    const int u_opt = tar->tarFlags->u;
    int fd = -1;
    int flags;
    const mode_t MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if(create)
    {
        flags = O_CREAT | O_WRONLY | O_TRUNC;
        fd = open(tar->tarFile_name, flags, MODE);
    }

    else if(r_append)
    {
        fd = open(tar->tarFile_name, O_RDWR);
    }
    
    else if(t_option)
    {
        fd = open(tar->tarFile_name, O_RDONLY);
    }
    
    else if(extract)
    {
        fd = open(tar->tarFile_name, O_RDONLY);
    }
    else if(u_opt)
    {
        fd = open(tar->tarFile_name, O_RDWR);
    }

    return fd;
}
