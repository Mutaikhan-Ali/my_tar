#include "source.h"


void get_to_see_result(t_tar_info* tar)
{
    check_options_forValid(tar);
    
    if(tar->tarFlags->err == 1) // smth went wrong
    {
        return;
    }

    if(tar->tarFlags->c)
    {
        if(tar->tar_array->number_ofFiles < 2)
        {
            create_single_tar(tar);
            return;
        }

        else
        {
            do_multiple_create_option(tar);
            return;
        }
    }

    else if(tar->tarFlags->r)
    {
            int fd_archieve;
            fd_archieve = open_archieve(tar);
            if(fd_archieve == -1)
            {
                printf("my_tar: no such tar file found\n");
                tar->tarFlags->err = 1;
                return;
            }
            if(!tar->tarFlags->f)
            {
                printf("my_tar: error: refusing to read archive contents from terminal (missing -f option?)\n");
                tar->tarFlags->err = 1;
                return;
            }           
            r_append(tar);
    }

    else if(tar->tarFlags->t)
    {
        if(!tar->tarFlags->f)
        {
            printf("my_tar: error: refusing to read archive contents from terminal (missing -f option?)\n");
            tar->tarFlags->err = 1;
            return;
        }
        t_option(tar);
    }

    else if(tar->tarFlags->x)
    {
        if(!tar->tarFlags->f)
        {
            printf("my_tar: error: refusing to read archive contents from terminal (missing -f option?)\n");
            tar->tarFlags->err = 1;
            return;
        }
       extract_tar(tar);
    }

    else if(tar->tarFlags->u)
    {
        if(!tar->tarFlags->f)
        {
            printf("my_tar: error: refusing to read archive contents from terminal (missing -f option?)\n");
            tar->tarFlags->err = 1;
            return;
        }
       u_opt(tar);
    }

}
