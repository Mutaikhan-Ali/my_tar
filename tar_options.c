#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>



#include "source.h"


char* get_content(int fd_archieve, int offset)
{
    int size = 0; 
    size = get_file_content_size(fd_archieve,offset);
    lseek(fd_archieve,  offset, SEEK_SET); 
    char* content = malloc (size * sizeof(char)+1);
    char c;
    int i = 0;
    while (read(fd_archieve, &c, 1) > 0)
    {
        if(c == '\0')
        {
            break;
        }
         content[i] = c;
         i++;
    }
    
    content[size] = '\0';

    return content;
}

int get_file_content_size (int fd_archieve, int offset)
{
    int size = 0;
    char c;

    lseek(fd_archieve,  offset, SEEK_SET); 
    
    while (read(fd_archieve, &c, 1) > 0)
    {
        if(c == '\0')
        {
            break;
        }
        size += 1;
    }

    lseek(fd_archieve, offset, SEEK_SET); // update
    return size;
}




int create_file(char* file_name)
{
    const mode_t MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    int fd = open(file_name, flags, MODE);
    return fd;
}

int do_next(t_header* header, char* file_name)
{
    long timeOfheader = octal_to_long(my_atoi(header->mtime));
    struct stat st;
    stat(file_name, &st);
    
    if(st.st_mtime > timeOfheader)
    {
        return 1;
    }
    return 0;
}

long getforfile_size(char* file)
{
    struct stat st;
    lstat(file, &st);
    return st.st_size;
}

char* get_content_file(char* file)
{
    int fd = open(file, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    char content[st.st_size + 1];
    read(fd, content, st.st_size);
    content[st.st_size] = '\0';
    char c;
    int i = 0;

    while (read(fd, &c, 1) > 0)
    {
        if(c == '\0')
        {
            break;
        }

         content[i] = c;
         i++;
    }
    
    char*res = content;
    return  res;
}
int is_exist(t_tar_info* tar, char* file_name)
{
    for(int i = 0;i < tar->tar_array->number_ofFiles;i++)
    {
        if(strcmp(tar->tar_array->files[i], file_name) != 0)
        {
            return i;
        }
    }
    return -1;
}

t_argument* get_all_names(t_tar_info* tar)
{
    int fd = open_archieve(tar);

    if(fd == -1)
    {
        printf("my_tar: error: no such tar file\n");
        tar->tarFlags->err = 1;
        return NULL;
    }

    long offset = 0;
    struct stat st;
    stat(tar->tarFile_name, &st);
    long size = st.st_size;
    tar->t_args = NULL;
    
    tar->t_header = get_header_value(fd, &offset); // done with header
    add_name_node(&tar->t_args, tar->t_header->name); // add name of header to the linked list
    free(tar->t_header);

    while(offset < size)
    {
        tar->t_header = NULL;
        lseek(fd,  offset, SEEK_SET); // kraten k 512
        
        skip(&offset, &size, 1, fd); // skip content
        
        skip(&offset, &size, 0, fd); // skip nulls
        lseek(fd, offset, SEEK_SET); //last index of null
        tar->t_header = get_header_value(fd, &offset); // header value
        
        
        if(tar->t_header->name[0] != '\0')
        {
            add_name_node(&tar->t_args, tar->t_header->name);
        }
  
        else if(tar->t_header->name[0] == '\0')
        {
            free(tar->t_header);
            break;
        }
        
        free(tar->t_header);
        
    }


  return tar->t_args;
}





void fill_metadata (char* filename, t_header* header)
{
    struct stat st;
    lstat(filename, &st);
    
    if(header->name[0] == '\0')
    {
        return;
    }
    
    st.st_mode =  my_atoi(header->mode);
    st.st_uid = octal_to_long(my_atoi(header->uid)); 
    st.st_gid = octal_to_long(my_atoi(header->gid));
    if(header->linkname[0] != '\0')
    {
        symlink(header->linkname, header->name);
    }
}


char* write_name(int fd_archieve, t_argument** t_args)
{
    char file[100];
    char c;
    int bytes_read = 0;
    int i = 0;

    while((bytes_read = read(fd_archieve, &c, 1)) > 0)
    {
        if(c == '\0')
        {
            break;
        }
        file[i] = c;
        i++;
    }
    file[i] = '\0';

    if(i != 0)
    {
        add_name_node(t_args, file);
        char* res = file;
        return res;
    }

    return NULL;
}





long get_number_of_nulls(long char_index)
{
    long res = 512;
    
    while(char_index > res)
    {
        res += 512;
    }
    
    return res;
}




int write_archieve(t_tar_info* tar, char* name)
{
    int res = 0;
    fill_header(tar, name);
    
    if(tar->t_header == NULL)
    {
        tar->tarFlags->err = 1;
        res++;
    }
    return res;
}

