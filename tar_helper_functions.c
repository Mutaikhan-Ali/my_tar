#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include "source.h"


void skip(long* offset, long* size, int flag, int fd_archieve)
{
    while (*offset < *size)
    {
        
        char c;
        read(fd_archieve, &c, 1);
        lseek(fd_archieve,  *offset, SEEK_SET);
        int f;
        if(flag == 1)
        {
            f = c != '\0';
        }
        else
        {
            f = c == '\0';
        }

        if(f)
        {
            *offset += 1;
            lseek(fd_archieve,  *offset, SEEK_SET);
        }
        
        else
        {
            lseek(fd_archieve,  *offset, SEEK_SET);
            break;
        }   
    }
    
}

long get_last_char_index(int fd_archieve)  // return last index of null char
{
    struct stat st;
    fstat(fd_archieve, &st);
    long tar_file_size = st.st_size; // get the size of tar file
    char c; // tmp value

    while(1)
    {
        tar_file_size -= 1;
        lseek(fd_archieve, tar_file_size, SEEK_SET);
        read(fd_archieve, &c, 1);
        // if there we meet another char
        if(c != '\0')
        {
            break;
        }
    }
    return tar_file_size;
}

t_header* fill_header(t_tar_info* tar, char* file_name)
{
    struct stat st;
    int st_status = stat(file_name, &st); 
    
    if(st_status < 0)
    {
        tar->tarFlags->err = 1;
        printf("my_tar: error opening file\n");
        return NULL;
    }

    
    { 
        my_strncpy (tar->t_header->name, file_name, 100);
        my_strncpy(tar->block, file_name, 100);
        my_strcat(tar->t_header->mode, get_CHMOD(tar->t_header->name));
        my_strcat(tar->block, get_CHMOD(tar->t_header->name));
        transfrom_to_other_base(tar->t_header->uid, st.st_uid, 6, OCTAL_BASE); // iMPORTANT 
        my_strcat(tar->block, tar->t_header->uid);
        transfrom_to_other_base(tar->t_header->gid, st.st_gid, 6, OCTAL_BASE);
        my_strcat(tar->block, tar->t_header->gid);
        if(is_directory(file_name))
        {
                transfrom_to_other_base(tar->t_header->size, 0, 10, OCTAL_BASE);
        }
        else
        {
            transfrom_to_other_base(tar->t_header->size, st.st_size, 10, OCTAL_BASE);
        }
        
        
        
        my_strcat(tar->block, tar->t_header->size);
        transfrom_to_other_base(tar->t_header->mtime, st.st_mtime, 10, OCTAL_BASE);
        my_strcat(tar->block, tar->t_header->mtime);
        set_link_name(tar, file_name);
        tar->t_header->typeflag = get_typeflag(st.st_mode);
        tar->block[156] = tar->t_header->typeflag;
        my_strcpy(tar->t_header->magic,"ustar  ");
        my_strcat(tar->block, tar->t_header->magic);
        my_memset(tar->t_header->version,' ',1);
        my_strcat(tar->block, tar->t_header->version);
        get_user_owner_name(file_name);
        my_strcpy(tar->t_header->uname, get_user_owner_name(file_name));
        my_strcat(tar->block, tar->t_header->uname);
        my_strcpy(tar->t_header->gname, get_group_owner_name(file_name));
        my_strcat(tar->block, tar->t_header->uname);
        get_major_and_minor(tar, file_name);
        calculate_check_sum(tar);

        return tar->t_header;
    }
}

long get_numberOfNULLS(t_tar_info* tar)
{
    struct stat st;
    if(stat(tar->tarFile_name, &st) < 0)
    {
        printf("failed while stat tar file name");
        return -1;
    }   
    long num_null = 10240; // set as default
    while(num_null < st.st_size)
    {
        num_null += 10240;
    }

    return num_null - st.st_size;
}

long get_numberofNULLs_for_file(t_tar_info* tar)
{
    struct stat st;
    if(stat(tar->tarFile_name, &st) < 0)
    {
        printf("failed while stat file\n");
        return -1;
    }   
    long num_null = 512; // set as default
    while(num_null < st.st_size)
    {
        num_null += 512;
    }
    return num_null - st.st_size;
}
void close_file(int fd_arhcieve, t_tar_info* tar)
{
    struct stat st;
    int status = stat(tar->tarFile_name, &st);
    
    if(status < 0)
    {
        printf("failde to stat file\n");
        return;
    }

    long number_ofNull = get_numberofNULLs_for_file(tar);
    char end_of_file[number_ofNull];

    my_memset(end_of_file, 0, number_ofNull);
    write(fd_arhcieve, end_of_file, sizeof(end_of_file));
}

void close_tar(int fd, t_tar_info* tar)
{
    struct stat st;
    if(stat(tar->tarFile_name, &st) < 0)
    {
        return;
    }
    long number_ofNull = get_numberOfNULLS(tar);
    char end[number_ofNull];
    my_memset(end, 0, number_ofNull);
    write(fd, end, sizeof(end));
    
}


void get_major_and_minor(t_tar_info* tar, char* file_name)
{
    struct stat st;
    int status = stat(file_name, &st);
    
    if(status < 0)
    {
        printf("failed to stat file\n");
        return;
    }

    else
    {
        if(st.st_rdev != 0)
        {
            transfrom_to_other_base(tar->t_header->devmajor, major(st.st_rdev), 6, OCTAL_BASE);
            my_strcat(tar->block, tar->t_header->devmajor);
            transfrom_to_other_base(tar->t_header->devminor, minor(st.st_rdev), 6, OCTAL_BASE);
            my_strcat(tar->block, tar->t_header->devminor);
        } 
    }
    
    

}

char* get_CHMOD(char* filename)
{
    int fd = open(filename, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    int usr = 0, grp = 0, oth = 0;
    if (st.st_mode & S_IRUSR) usr += 4;
    if (st.st_mode & S_IWUSR) usr += 2;
    if (st.st_mode & S_IXUSR) usr += 1;
    if (st.st_mode & S_IRGRP) grp += 4;
    if (st.st_mode & S_IWGRP) grp += 2;
    if (st.st_mode & S_IXGRP) grp += 1;
    if (st.st_mode & S_IROTH) oth += 4;
    if (st.st_mode & S_IWOTH) oth += 2;
    if (st.st_mode & S_IXOTH) oth += 1;
    char fmod[8];
    my_strcpy(fmod, "0000");
    fmod[4] = (char)usr + '0';
    fmod[5] = (char)grp + '0';
    fmod[6] = (char)oth + '0';
    fmod[7] = '\0';
    char* return_value = fmod;
    return return_value;
}

char* get_link_name(char* name)
{
    char link_name_buffer[100];
    int status = readlink(name, link_name_buffer, 100);
    
    // if file doesnt have a link
    if(status < 0)
    {
        return NULL;
    }
    
    link_name_buffer[status] = '\0'; // make it null terminated
    char* res = link_name_buffer; // point to it
    return res;
}

void set_link_name(t_tar_info* tar, char* file_name)
{
    char* link = get_link_name(file_name);
    if(link != NULL)
    {
        my_strncpy(tar->t_header->linkname, link, 100); // just in case i use strncpy
        my_strcat(tar->block,link);
    }
}

char* get_user_owner_name(char* file_name)
{
    struct stat st;
    stat(file_name, &st);
    struct passwd* pw = getpwuid(st.st_uid);
    if(pw != NULL)
    {
        return pw->pw_name;
    }
    else
    {
        printf("my_tar: error with struct passwd\n");
        return NULL;
    }
}

char* get_group_owner_name(char* file_name)
{
    struct stat st;
    lstat(file_name, &st);
    struct group* gr = getgrgid(st.st_gid);
    if(gr != NULL)
    {
        return gr->gr_name;
    }
    else
    {
        printf("error with struct group\n");
        return NULL;
    }
}



int calculate_check_sum(t_tar_info* tar)
{
    int sum = 0;
    for(int i = 0;i < 512; i++)
    {
       sum += (unsigned char)tar->block[i];
    }
    tar->t_header->chksum[7] = 32;
    tar->t_header->chksum[6] = '\0';
    sum += 8 * 32;
    sum -= 32;
    transfrom_to_other_base(tar->t_header->chksum, sum, 5, OCTAL_BASE);
    return sum;
}

char get_typeflag(int mode)
{
    switch (mode  & S_IFMT){
        case S_IFREG:
            return REGTYPE;
        case S_IFLNK:
            return SYMTYPE;
        case S_IFCHR:
            return CHRTYPE;
        case S_IFBLK:
            return BLKTYPE;
        case S_IFDIR:
            return DIRTYPE;
        case S_IFIFO:
            return FIFOTYPE;
        default:
            return REGTYPE;

    }
}




