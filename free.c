#include "source.h"

void free_string_array(char** files, int l)
{
    for(int i = 0; i < l; i++)
    {
        if(files[i] != NULL)
        {
            free(files[i]);
        }
        
    }
    free(files);
}

void free_everything(t_tar_info* tar)
{ 
    if (tar->tarFile_name != NULL)  free(tar->tarFile_name);
    
    free(tar->tarFlags);
    if(tar->tar_array->number_ofFiles > 0)
    {
        free_string_array(tar->tar_array->files, tar->tar_array->number_ofFiles);
    }
    
    free(tar->tar_array); 
    free(tar);
}
void free_name_list(t_argument* head)
{
    if(head != NULL)
    {
        if(head->next)
        {
            free_name_list(head->next);
        }
        free(head);
    }
}