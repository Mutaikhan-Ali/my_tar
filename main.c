#include <stdio.h>
#include <stdlib.h>
#include "source.h"



void see_result(t_tar_info* tar)
{
 
    if(tar->tarFlags->v)
    {
        output_array(tar->tar_array->files, tar->tar_array->number_ofFiles);
    }

    if(tar->tarFlags->err == 1 || tar->tarFlags->f != 1)
    {
        printf("my_tar: this msg was outputed due to previous errors\n");
        free_everything(tar);
        return;
    }

    
  free_everything(tar);
}

int main(int ac,char**av)
{

  t_tar_info* tar = init_tar(ac, av);
  
  if (tar->tarFile_name == NULL) 
  {
      printf("wrong tar file name!\n");
      free_everything(tar);
      return -1;
  }

  get_to_see_result(tar);
  see_result(tar);

  return 0;
}
