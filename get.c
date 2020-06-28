#include <sys/stat.h>
#include "source.h"



long get_file_size_by_name(char* name)
{
    struct stat st;
    if(stat(name, &st) != -1)
    {
        return st.st_size;
    }
    return -1;
}

long get_last_modificaton_time(char* file_name)
{   
    struct stat st;
    long m_time = 0;

    if(stat(file_name, &st) > -1)
    {
        m_time = st.st_mtime;
        return m_time;
    }
    
    else
    {
        printf("my_tar: %s we can not stat it\n", file_name);
        return -1;
    }
}

long get_the_length_of_linked_list(t_argument* list)
{
    // if list is empty
    if(list == NULL)
    {
        return 0;
    }
    else
    {
        long len = 0;
        t_argument* traverse = list;
        while (traverse != NULL)
        {
            len += 1;
            traverse = traverse->next;
        }
        return len;
    }  
}

int get_index_of_last_exist_file(t_tar_info* tar)
{
    int index = 0;
    for(int i = 0; i < tar->tar_array->number_ofFiles; i++)
    {
        if(file_exists(tar->tar_array->files[i]))
        {
            index = i;
        }
    }
    return index;
}

char* get_the_last_exist_file_name(t_tar_info* tar)
{
    int target_index = get_index_of_last_exist_file(tar);
    char* value = NULL;
    for(int i = 0; i < tar->tar_array->number_ofFiles; i++)
    {
        if(target_index == i)
        {
            int len = my_strlen(tar->tar_array->files[i]);
            value = (char*) malloc(sizeof(char) * (len + 1));
            my_strcpy(value, tar->tar_array->files[i]);
            break;
        }    
    }
    return value;
}


int get_file_size(int fd){
    int res;
    int reader  = 0;
    while((reader = read(fd, NULL, BLOCK_SIZE) > 0)){
        res += reader;
    }
    if(res % BLOCK_SIZE != 0){
        res += BLOCK_SIZE - (res % BLOCK_SIZE);
    }
    return res;
}

int get_NumberOfFiles(char *path, int all)
{
    DIR* dir;
    struct dirent* entry;
    int size = 0;

    dir = opendir(path);

    while((entry = readdir(dir)) != NULL)
    {
        // skip hidden files
        if(!all && entry->d_name[0] == '.')
        {
            continue;
        }
        size += 1;
    }
    
    closedir(dir);

    return size;
}

long get_the_last_char_from_tar_archieve(int fd_tar) // not null of the last file
{
   struct stat st;
   fstat(fd_tar, &st);
   long size = st.st_size - 1; // get the tar file's size
   lseek(fd_tar, size, SEEK_SET); // set ptr to the last char of tar file
   char c;
   read(fd_tar, &c, 1); // read one by one
   long offset = size; // assign to offset

   
   while(c == '\0')
   {
           read(fd_tar, &c, 1); // read one by one
           offset -= 1; // move to previous
           lseek(fd_tar, offset, SEEK_SET);
   }

   return offset + 2;
   
}

long get_the_first_char_from_tar_archieve(int fd_tar, long offset) // not null of the last file
{
   char c;
   lseek(fd_tar, offset - 2, SEEK_SET);
   read(fd_tar, &c, 1); // read one by one
   
   while(c != '\0')
   {
           read(fd_tar, &c, 1); // read one by one
           offset -= 1; // move to previous
           lseek(fd_tar, offset, SEEK_SET);
   }

   return offset + 2;
   
}

t_argument* get_entry_names_of_tar(t_tar_info* tar, int fd_archieve)
{
    struct stat st;
    long size;
    long offset = 0;
    tar->t_args = NULL;
    fstat(fd_archieve, &st);
    size = st.st_size;
    // get 1st filename
    lseek(fd_archieve, offset, SEEK_SET);
    char* name = write_name(fd_archieve, &tar->t_args);
    lseek(fd_archieve, offset, SEEK_SET); 
    offset += 512; // skip header
    
    // others file name
    while (offset < size)
    {
        lseek(fd_archieve,  offset, SEEK_SET);
        
        if(name[strlen(name) - 1] != '/')
        {
            skip(&offset, &size, 1, fd_archieve); // skip content
            skip(&offset, &size, 0, fd_archieve); // skip nulls   
        }
    
        name = write_name(fd_archieve, &tar->t_args);
        lseek(fd_archieve, offset, SEEK_SET); 
        offset += 512; // skip header
    }
    return tar->t_args;
}


t_header* get_header_value(int fd_archieve, long* offset)
{   
    t_header* header = init_tar_header();
    lseek(fd_archieve, *offset, SEEK_SET); // start point
    *offset += read(fd_archieve, header->name, 100); // get name
    *offset += read(fd_archieve, header->mode, 8);
    *offset += read(fd_archieve, header->uid, 8);
    *offset += read(fd_archieve, header->gid, 8);
    *offset += read(fd_archieve, header->size, 12);
    *offset += read(fd_archieve, header->mtime, 12);
    *offset += read(fd_archieve, header->chksum, 8);
    *offset += read(fd_archieve, &header->typeflag, 1);
    *offset += read(fd_archieve, header->linkname, 100);
    *offset += read(fd_archieve, header->magic, 6);
    *offset += read(fd_archieve, header->version, 2);
    *offset += read(fd_archieve, header->uname, 32);
    *offset += read(fd_archieve, header->gname, 32);
    *offset += read(fd_archieve, header->devmajor, 8);
    *offset += read(fd_archieve, header->devminor, 8);
    *offset += 167;
    return header;
}