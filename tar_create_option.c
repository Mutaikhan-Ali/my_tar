#include<string.h>
#include"source.h"

void create_single_tar(t_tar_info* tar)
{
        int fd_archieve;
        fd_archieve = open_archieve(tar);

        if(is_directory(tar->tar_array->files[0]))
                {
                    
                    char dir_path[strlen(tar->tar_array->files[0]) + 2];
                    strcpy(dir_path, tar->tar_array->files[0]);
                    strcat(dir_path, "/");
                    if(tar->tarFlags->v)
                    {
                        printf("%s\n", dir_path);
                    }
                    write_archieve(tar, dir_path);
                    write(fd_archieve, tar->t_header,BLOCK_SIZE);
                    my_memset(tar->t_header, 0, BLOCK_SIZE);
                    write_dir_files(tar->tar_array->files[0], fd_archieve, tar);
                }
                   

        else if(file_exists(tar->tar_array->files[0]))
        {           
            write_archieve(tar, tar->tar_array->files[0]); // add values to structure
            write(fd_archieve, tar->t_header,BLOCK_SIZE); // write structrute to file
            write_file_content(fd_archieve, tar->tar_array->files[0]);                
        }
        
        else
        {
            printf("my_tar: %s: we can't stat it: no such file or directory\n", tar->tar_array->files[0]);
            tar->tarFlags->err = 1;
        }
        free(tar->t_header);
        close_tar(fd_archieve, tar);
}

void full_write_into_tar(int fd_archieve, t_tar_info* tar)
{
            //char* last_file = get_the_last_exist_file_name(tar);
            
            for(int i = 0;i < tar->tar_array->number_ofFiles;i++)
            {    
                write_tar_for_file(fd_archieve, tar->tar_array->files[i], tar);
                // reset
                my_memset(tar->t_header, 0, BLOCK_SIZE);
            }

            //free(last_file);
            close_tar(fd_archieve, tar);
}

void do_multiple_create_option(t_tar_info* tar)
{
    if(tar->tar_array->number_ofFiles < 2)
    {
        create_single_tar(tar);
        return;
    }

    else
    {
        int fd_archieve;
        fd_archieve = open_archieve(tar);
        full_write_into_tar(fd_archieve, tar);
        free(tar->t_header);
        return;    
    }   

     

}