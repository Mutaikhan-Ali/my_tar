#include <stdlib.h>
#include "source.h"

int is_exist_element(t_argument** list, char* target)
{
    if(*list == NULL)
    {
        add_name_node(list, target);
        return 0;
    }

    else
    {
        t_argument* traverse = *list;
        while (traverse != NULL)
        {
            if(my_strcmp(traverse->filename, target) == 0)
            {
                return 1;
            }
            traverse = traverse->next;
        }
        return 0;
    }
}

int check_for_time(char* prev, char* new)
{
  
    long prev_time = get_last_modificaton_time(prev);
    long new_time =  get_last_modificaton_time(new);

    return new_time > prev_time;
}

void modify_tar_array(t_tar_info** tar, t_argument* exists)
{
       for(int i = 0; i < (*tar)->tar_array->number_ofFiles;i++)
    {
        t_argument* curr = exists;
        
        while (curr != NULL)
        {
            if(my_strcmp((*tar)->tar_array->files[i], curr->filename) == 0)
            {
               free((*tar)->tar_array->files[i]);
               (*tar)->tar_array->files[i] = NULL;
               break;
            }
            curr = curr->next;
        }
    }

    long new_size = (*tar)->tar_array->number_ofFiles - get_the_length_of_linked_list(exists);
    long new_index = 0;
    char** new_array = (char**) malloc(sizeof(char*) * (new_size + 1));

    for(int i = 0;i < (*tar)->tar_array->number_ofFiles;i++)
    {
        if((*tar)->tar_array->files[i] != NULL)
        {
            new_array[new_index] = malloc(sizeof(char) * (my_strlen((*tar)->tar_array->files[i]) + 2));
            strcpy(new_array[new_index], (*tar)->tar_array->files[i]); // copy value
            new_index += 1;
        }
    }

    free_string_array((*tar)->tar_array->files, (*tar)->tar_array->number_ofFiles);
    free((*tar)->tar_array);
    // init everythin again
    (*tar)->tar_array = (t_array*)malloc(sizeof(t_array));
    (*tar)->tar_array->files = (char**)malloc(sizeof(char*) * (new_size + 1));
    for(int i = 0;i < new_size;i++)
    {
        (*tar)->tar_array->files[i] = (char*)malloc(sizeof(char) *(my_strlen(new_array[i]) + 2));
        strcpy((*tar)->tar_array->files[i], new_array[i]);
    }
    
    (*tar)->tar_array->number_ofFiles = new_size;
    free_string_array(new_array, new_size);
}

char*  make_path(char* parent, char* son)
{
    if(parent == NULL)
    {
        printf("error with parent path\n");
        return NULL;
    }

    char* path = malloc(sizeof(char) * (strlen(parent) + strlen(son) + 2));
    strcpy(path, parent);
    if(parent[strlen(path) - 1] == '/' )
    {
        strcat(path, son);
    }
    else
    {
        strcat(path, "/");
        strcat(path, son);
    }
    return path;
}

int is_directory(char* file_name)
{
    struct stat st;
    lstat(file_name, &st);
    return S_ISDIR(st.st_mode);
}
long my_pow(int num, int base)
{
    int res = num;
    
    for(int i = 1;i<base;i++)
    {
        num = num * res;   
    }
    return num;
}

long octal_to_long(long octalNumber)
{
    int decimalNumber = 0, i = 0;

    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber % 10) * my_pow(8, i);
        i++;
        octalNumber /= 10;
    }

    return decimalNumber;
}

long my_atoi(char* str) 
{ 
    long res = 0; 
   
    for (int i = 0; str[i] != '\0'; i++)
    {
        res = res * 10 + str[i] - '0';
    } 
         
    return res; 
}

void add_name_node(t_argument** head, char* value)
{
    t_argument* tmp = *head;
    t_argument* node = (malloc(sizeof(t_argument)));
    my_strcpy(node->filename, value);
    node->next = *head; 
    *head = node;
}


void reverse_name_list(t_argument** head)
{
    t_argument* prev = NULL;
    t_argument* curr = *head;
    t_argument* next = NULL;

    while(curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    *head = prev;
}


void print_help()
{
    printf("Try 'tar --help' or 'tar --usage' for more information.\n");
}

void check_options_forValid(t_tar_info* tar)
{
    int option = tar->tarFlags->c + tar->tarFlags->r + tar->tarFlags->x
    +tar->tarFlags->u + tar->tarFlags->t;
    

    if(option > 1)
    {
        printf("You may not specify more than one ctrux, --delete or  --test-label option\n");
        print_help();
        tar->tarFlags->err = 1;
        return;
    }

    else if(option < 1)
    {
        tar->tarFlags->err = 1;   
        printf("You must specify one of the '-ctrux', '--delete' or '--test-label' options\n");
        print_help();
        return;
    }
    
    if(tar->tar_array->number_ofFiles == 0 && (!tar->tarFlags->t  && !tar->tarFlags->x))
    {
        tar->tarFlags->err = 1;
        printf("my_tar: Cowardly refusing to create an empty archive\n");
    }
}

char *transfrom_to_other_base(char * res, long num, int size, int base)
{
    const char symbols[] = "0123456789ABCDEF";
    my_memset(res, '0', size);
    res[size] = '\0';
    while(num > 0)
    {
        res[size--] = symbols[num % base];
        num /= base;
    }
    return res;
}


int write_file_content(int fd_archive, char *filename)
{
    int res = 0;
    int fd_read = open(filename, O_RDONLY);
    int bytes = 0;
    char c;

    while((bytes = read(fd_read, &c, 1))>0)
    {
        write(fd_archive, &c, 1);
    }

    close(fd_read);
    return res;
}

int file_exists(char* filename)
{
    int fd = open(filename, O_RDONLY);
    
    if(fd == -1)
    {
         return 0;
    }
    close(fd);
    return 1;
}
