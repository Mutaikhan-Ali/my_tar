#include "source.h"

void write_dir_files(char* dir_name, int fd_archieve, t_tar_info* tar)
{
    DIR* dir;
    dir = opendir(dir_name);

    if(dir == NULL)
    {
        tar->tarFlags->err = 1;
        return;
    }

    struct dirent* entry;
    char* parent_path = dir_name;

    while ((entry = readdir(dir)) != NULL)
    {
        if(entry->d_name[0] == '.')
        {
            continue;
        }
        char* path;
        path = make_path(dir_name, entry->d_name);
        
        if(tar->tarFlags->v)
        {
            printf("%s\n", path);
        }
        // write the header
        my_memset(tar->t_header, 0, BLOCK_SIZE);
        write_archieve(tar,  path);
        write(fd_archieve, tar->t_header,BLOCK_SIZE);
        
        // if we found subdir we call function recursievly
        if(entry->d_type & DT_DIR) 
        {
            my_memset(tar->t_header, 0, BLOCK_SIZE); // make empty t_header 
            write_dir_files(path, fd_archieve, tar);  // do recursion
        }

        else
        {
            write_file_content(fd_archieve, path);
            close_file(fd_archieve, tar);
            memset(tar->t_header, 0, BLOCK_SIZE);
        }
        
        free(path);
    }

    closedir(dir);

}