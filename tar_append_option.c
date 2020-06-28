#include "source.h"


void fill_nulls(int size, int* fd_archieve)
{
       for(int i = 0; i < size; i++)
       {
           char c = '\0';
           write(*fd_archieve, &c, 1);
       }
}


int is_file_exists(char* file_name)
{
   int fd = open(file_name, O_RDONLY);
   if(fd == -1)
   {
       return 0;
   }
   close(fd);
   return 1;
}

long get_tar_size(int fd_archieve)
{
   struct stat st;
   fstat(fd_archieve, &st);
   return st.st_size;
}

/*
So, at first i need to get index of last not null character
then i need to get the last index of null by skipping the contetn of file, we go to up
then when we have index of first char and last of content 
we just get the size of file 
than count nulls to be closed, then we sum it to the last index, than set file ptr to this point and we start writing
other files
*/

void write_tar_for_file(int fd_archieve, char* file_name, t_tar_info* tar)
{       if(file_name != NULL)
        {
            if(file_exists(file_name) && !is_directory(file_name))
                {           
                    write_archieve(tar, file_name);
                    write(fd_archieve, tar->t_header,BLOCK_SIZE);
                    write_file_content(fd_archieve, file_name);
                    close_file(fd_archieve, tar);   
                }

            else if(is_directory(file_name))
                { 
                    // add '/' 
                    char path[strlen(file_name) + 2];
                    strcpy(path, file_name);
                    strcat(path, "/");
                    if(tar->tarFlags->v) 
                    {
                        printf("%s\n", path);
                    }
                    write_archieve(tar, path);
                    write(fd_archieve, tar->t_header,BLOCK_SIZE);
                    write_dir_files(file_name, fd_archieve, tar);
                }      
            else
                {
                    printf("my_tar: %s: we can't stat it: no such file or directory\n", file_name);
                    tar->tarFlags->err = 1;
                }
        }
        
}

void r_append(t_tar_info* tar)
{
   // there we open file and get the fd of tar file
   int fd_archieve = open_archieve(tar);
   
   if(fd_archieve == -1)
   {
       printf("my_tar:'%s' : can not open tar file\n", tar->tarFile_name);
       tar->tarFlags->err = 1;
       return;
   }

   for(int i = 0;i<tar->tar_array->number_ofFiles;i++)
    {
            long last_char_index = get_the_last_char_from_tar_archieve(fd_archieve);  // get the last not null char
            long to_close = get_number_of_nulls(last_char_index); // i get the last char which closes tar archieve
            lseek(fd_archieve, to_close, SEEK_SET);    
            write_tar_for_file(fd_archieve, tar->tar_array->files[i], tar);
            close_file(fd_archieve, tar);
            my_memset(tar->t_header, 0, BLOCK_SIZE);
    }
    free(tar->t_header);
    close_tar(fd_archieve, tar);
}