#include "source.h"

void output_array(char**files,int l)
{
    for(int i = 0;i<l;i++)
    {
        if(files[i] != NULL)
        {
            if(file_exists(files[i]) && !is_directory(files[i]))
            {
                printf("%s\n",files[i]);
            }
        }
        
    }
}

void print_name_list(t_argument* header)
{
    reverse_name_list(&header);
    t_argument* curr = header;
    while(curr != NULL)
    {
        printf("%s\n", curr->filename);
        curr = curr->next;
    }
    reverse_name_list(&header);
    
}