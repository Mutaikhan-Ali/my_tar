#include "source.h"


void init_flags(t_tar_info* tar)
{
    // set flags as 0 as default, cause c gives random negative gib number
    tar->tarFlags->c = 0;
    tar->tarFlags->r = 0;
    tar->tarFlags->t = 0;
    tar->tarFlags->u = 0;
    tar->tarFlags->x = 0;
    tar->tarFlags->err = 0;
    tar->tarFlags->f = 0;
    tar->tarFlags->v = 0;
    tar->tar_array->number_ofFiles = 0;
    tar->tar_array->array_index = 0;
}

t_tar_info* init_tar(int ac,char** av)
{
    // allocate enogh memory for pointers
    t_tar_info* tar = (t_tar_info*)malloc(sizeof(t_tar_info));
    tar->tarFlags = (t_tar_flags*)malloc(sizeof(t_tar_flags));
    tar->tar_array = (t_array*)malloc(sizeof(t_array));
    init_flags(tar);

    my_memset(tar->block, 0, BLOCK_SIZE);
    tar->tarFile_name = NULL;  // set the name of tar file as null
    int arg_index = 1; // skip the null ptr

    while(arg_index < ac) // parse arguments till the end
    {
        parse_args(tar, av[arg_index], arg_index,ac);
        arg_index++;  // move to next
    
    }

    if(!tar->tarFlags->x)
    {
        if(!tar->tarFlags->u)
        {
             tar->t_header = init_tar_header();
        }
    }
    
    return tar;
}

void init_array_OfFiles(t_tar_info*tar, int l)
{
    tar->tar_array->files = (char**) malloc(sizeof(char*) * (l + 1));
}
void init_arr_value(t_tar_info* tar, char*argv, int i)
{  
    tar->tar_array->files[i] = malloc(sizeof(char) * (my_strlen(argv) + 2));
    my_strcpy(tar->tar_array->files[i], argv); // copy value
    tar->tar_array->array_index += 1; // move to next
}

t_header* init_tar_header()
{
      t_header* header = malloc(BLOCK_SIZE);
      
      if(header != NULL)
      {
          my_memset(header, 0, BLOCK_SIZE);
      }
      return header;
}