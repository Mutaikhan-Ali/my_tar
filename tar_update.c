#include "source.h"

void u_opt(t_tar_info* tar)
{  
        // there i open tar file
    int fd = open_archieve(tar);

    if(fd == -1) // then check for valid
    {
        printf("my_tar: error: no such tar file\n");
        tar->tarFlags->err = 1;
        return;
    }

    get_entry_names_of_tar(tar, fd);
    t_argument* exists = NULL;
    fill_similar_files(tar, &exists);

    // if there are no corresponding entries
    if(exists == NULL)
    {
        tar->t_header = init_tar_header();
        r_append(tar);
        free_name_list(tar->t_args);
        return; // exit as we dont need anythin else to do this u option
    }

    else
    {
   
        t_argument* updateds = NULL;
        get_updated_files(tar, fd, exists, &updateds);

        if(updateds != NULL)
        {
            tar->t_header = init_tar_header();
            long tar_file_size = get_file_size_by_name(tar->tarFile_name);
            
            if(tar_file_size != -1)
            {
                t_argument* visiteds = NULL;
                t_argument* traverse = updateds;
                while (traverse != NULL)
                {
                    if(!is_exist_element(&visiteds, traverse->filename))
                    {
                            printf("updating %s\n", traverse->filename);
                            long last_char_index = get_the_last_char_from_tar_archieve(fd);  // get the last not null char
                            long to_close = get_number_of_nulls(last_char_index); // i get the last char which closes tar archieve
                            lseek(fd, to_close, SEEK_SET);    
                            write_tar_for_file(fd, traverse->filename, tar);
                            close_file(fd, tar);
                            add_name_node(&visiteds, traverse->filename);
                    }   
                    my_memset(tar->t_header, 0, BLOCK_SIZE);
                    traverse  = traverse->next;
                }
                free_name_list(visiteds);
            }    
            free_name_list(updateds);
            free(tar->t_header);
        }

        long exists_len = get_the_length_of_linked_list(exists);

        if(exists_len != tar->tar_array->number_ofFiles)
        {
            // as we know, that there are files that dont exist in tar archieve also files should be updated
            // so we modify it, just files dont exist in tar archeive stay in array
            lseek(fd, 0, SEEK_SET);
            modify_tar_array(&tar, exists);
            tar->t_header = init_tar_header();  // init structure again
            r_append(tar);
        } 
        
    }
    // free proccesses
    free_name_list(tar->t_args);
    free_name_list(exists);
}


void get_updated_files(t_tar_info* tar, int fd_archieve, t_argument* exists, t_argument** updateds)
{
    struct stat st;
    
    if(stat(tar->tarFile_name, &st) != -1)
    {
            long offset = 0;
            long size = st.st_size;
            while (offset < size)
            {
                tar->t_header = get_header_value(fd_archieve, &offset);
                get_only_updated(&tar, fd_archieve, exists, updateds, &offset);
                lseek(fd_archieve,  offset, SEEK_SET); // set ptr to content 
                if(tar->t_header->name[my_strlen(tar->t_header->name) - 1] != '/')
                {
                    skip(&offset, &size, 1, fd_archieve); // skip content
                    skip(&offset, &size, 0, fd_archieve); // skip nulls   
                }
            
                lseek(fd_archieve, offset, SEEK_SET); 
                free(tar->t_header);
            }
    }
}

t_argument* get_only_updated(t_tar_info** tar, int fd_archieve, t_argument* exists, t_argument** updateds, long* offset)
{
        t_argument* traverse = exists;

        while (traverse != NULL)
        {
            if(my_strcmp((*tar)->t_header->name, traverse->filename) == 0)
            {
                long prev_time = octal_to_long(my_atoi((*tar)->t_header->mtime)); 
                long new_time = get_last_modificaton_time(traverse->filename);
                
                if(prev_time < new_time)
                {
                    add_name_node(updateds, traverse->filename);
                    break;
                }
            }

            traverse = traverse->next; // move to next        
        }

        return *updateds;
}

void fill_similar_files(t_tar_info* tar, t_argument** exists)
{
    for(int i = 0; i < tar->tar_array->number_ofFiles;i++)
  {
      t_argument* ptr = tar->t_args; // move to next value of tar->t_args, which stores names
      
      while (ptr != NULL)
      {
        if( my_strcmp(tar->tar_array->files[i], ptr->filename) == 0) // if there we have similar files between tar file and terminal
        {
            add_name_node(exists, ptr->filename); // add it to exists
        }
        ptr = ptr->next;    
      }
  }
}
