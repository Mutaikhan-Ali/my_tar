#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "source.h"

int create_dir(char* dir_name);

void extract_tar(t_tar_info* tar)
{
   // get extract permisions
   int fd_tar = open_archieve(tar);
   if(fd_tar == -1)
   {
       printf("my_tar: error: no such tar file to extract\n");
       tar->tarFlags->err = 1;
       return;
   }
   long offset = 0;
   struct stat st;
   stat(tar->tarFile_name, &st);
   long size = st.st_size;

   while (offset < size)
   {
           tar->t_header = get_header_value(fd_tar, &offset);
           char* file_name = tar->t_header->name;
           char* content;
           lseek(fd_tar, offset, SEEK_SET);
           
           if(tar->t_header->name[0] != '\0')
           {
               if(tar->tarFlags->v)
               {
                   printf("%s\n", file_name);
               }

               if(file_name[my_strlen(file_name) - 1] != '/')
               {
                   int fd_file = create_file(file_name);
                   content = get_content(fd_tar, offset);
                   write(fd_file, content, my_strlen(content));
                   free(content);
                   lseek(fd_tar, offset, SEEK_SET);
                   skip(&offset, &size, 1, fd_tar); // skip content
                   skip(&offset, &size, 0, fd_tar); // skip nulls   
               }
                
               else
               {
                   create_dir(file_name);
               }
           }
           free(tar->t_header);
   }
   
}

int create_dir(char* dir_name)
{
   const mode_t MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   int status = mkdir(dir_name, MODE);
   return status;
}