#include <string.h>
#include "source.h"

void parse_args(t_tar_info* tar,char* argv,int arg_index,int argc)
{
    // if it is a flag
    if(argv[0] == '-' || arg_index == 1)
    {
      parse_flags(tar,argv);
    }

    // if it is tar file or entry file

    else if(argv) 
    {
    
            if(tar->tarFile_name == NULL)
            {
                parse_name(tar, argv);
            }
            
            else
            {
                if(tar->tar_array->number_ofFiles == 0)
                {
                    tar->tar_array->number_ofFiles = argc - arg_index;
                    init_array_OfFiles(tar, tar->tar_array->number_ofFiles);
                    init_arr_value(tar, argv,tar->tar_array->array_index);
                }
                else
                {
                    init_arr_value(tar, argv, tar->tar_array->array_index);
                }
                
            }
    }
        
}

void parse_name(t_tar_info* tar,char* argv)
{
    if((tar->tarFile_name == NULL) && (my_strstr(argv, ".tar") != NULL))
    {
        // allocate memory for copying name value
        tar->tarFile_name  =(char*) malloc(sizeof(char) * (my_strlen(argv) + 2) );
        my_strcpy(tar->tarFile_name,argv);
    } else tar->tarFile_name = NULL;
}


void parse_flags(t_tar_info*tar, char*argv)
{
  //setting flags 
  if(argv[0] == '-' && (my_strlen(argv) == 2))
  {
    set_flag(tar,argv[1]);
  }

  else
  {
    set_flags(tar,argv);
  }

}






        
