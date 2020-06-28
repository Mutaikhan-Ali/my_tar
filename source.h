#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sys/sysmacros.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define BLOCK_SIZE 512
#define REGTYPE '0'
#define AREGTYPE '\0'
#define LNKTYPE '1'
#define SYMTYPE '2'
#define CHRTYPE '3'
#define BLKTYPE '4'
#define DIRTYPE '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'
#define OCTAL_BASE 8


typedef struct s_tar_flags
{
  int c;
  int r;
  int t;
  int u;
  int x;
  int v;
  int f;
  int err;
} t_tar_flags;

typedef struct s_array
{
  char** files;
  int number_ofFiles;
  int array_index;
} t_array;


typedef struct posix_header
{                              /* byte offset */
  char name[100]; // done                     /*   0 */
  char mode[8];   // done                    /* 100 */
  char uid[8];    // done                    /* 108 */
  char gid[8];    // done                    /* 116 */
  char size[12];  // done                      /* 124 */
  char mtime[12]; // done                        /* 136 */
  char chksum[8];      // done                          /* 148 */
  char typeflag;   //  done                        /* 156 */
  char linkname[100]; // done              /* 157 */
  char magic[6];    // done                       /* 257 */ ** ustar
  char version[2];   //done           /* 263 */ // 
  char uname[32];      //done         /* 265 */
  char gname[32];       // done        /* 297 */
  char devmajor[8];    // done         /* 329 */
  char devminor[8];  // done          /* 337 */
  char prefix[155];    // done         /* 345 */
   int value;    // done                          /* 500 */
}t_header;

typedef struct s_argument
{
    char filename[100];
    struct s_argument* next;
}t_argument;

typedef struct s_node
{
    t_header* value;
    struct s_node* next;
}t_node;
typedef struct s_tar_info
{
  char* tarFile_name; 
  t_tar_flags* tarFlags;
  t_array* tar_array; 
  t_header* t_header; 
  t_argument* t_args;
  t_node* t_node;
  char block[512]; // copies all char values, used for checksum 
}t_tar_info;


int get_file_content_size (int fd_archieve, int offset);
char* get_content(int fd_archieve, int offset);
t_tar_info* init_tar(int ac,char** av);
void get_to_see_result(t_tar_info* tar);
void init_array_OfFiles(t_tar_info*tar, int l);
void init_arr_value(t_tar_info*tar,char*argv, int i);
void parse_args(t_tar_info*tar,char* argv,int arg_index,int argc);
void parse_flags(t_tar_info* tar, char* argv);
void parse_name(t_tar_info* tar,char* argv);
void set_flag(t_tar_info* tar,char argv);
void set_flags(t_tar_info* tar,char* argv);
void init_flags(t_tar_info*tar);
void free_string_array(char**files,int l);
void output_array(char**files,int l);
void free_everything(t_tar_info* tar);
void check_options_forValid(t_tar_info* tar);
void parse_options(t_tar_info* tar);
void print_help();
int open_archieve(t_tar_info* tar);
t_header* init_tar_header();
char* write_name(int fd_archieve, t_argument** t_args);
int write_archieve(t_tar_info* tar, char* name);
char * transfrom_to_other_base(char * res, long num, int size, int base);
char get_typeflag(int mode);
char* get_CHMOD(char* filename);
char* itoa(long num, long base);
int file_exists(char * filename);
int write_file_content(int fd_archive, char *filename);
int get_file_size(int fd);
char* get_link_name(char* name);
char* get_user_owner_name(char* file_name);
char* get_group_owner_name(char* file_name);
t_header* fill_header(t_tar_info* tar, char* file_name);
int calculate_check_sum(t_tar_info* tar);
void close_tar(int fd, t_tar_info* tar);
void set_link_name(t_tar_info* tar, char* file_name);
long get_numberOfNULLS(t_tar_info* tar);
void get_major_and_minor(t_tar_info* tar, char* file_name);
void close_file(int fd_arhcieve, t_tar_info* tar);
long get_numberofNULLs_for_file(t_tar_info* tar);
int get_index_of_last_exist_file(t_tar_info* tar);
char* get_the_last_exist_file_name(t_tar_info* tar);
void do_multiple_create_option(t_tar_info* tar);
void create_single_tar(t_tar_info* tar);
long get_last_char_index(int fd_archieve);
void r_append(t_tar_info* tar);
void full_write_into_tar(int fd_archieve, t_tar_info* tar);
void t_option(t_tar_info* tar);
void skip(long* offset, long* size, int flag, int fd_archieve);
void add_name_node(t_argument** head, char* value);
void reverse_name_list(t_argument** head);
void print_name_list(t_argument* header);
void check_equal(t_tar_info* tar, long* offset, int fd);
void free_name_list(t_argument* head);
void extract_header(t_tar_info* tar);
t_header* get_header_value(int fd_archieve, long* offset);
void fill_metadata (char* filename, t_header* header);
long octal_to_long(long octalNumber);
long my_atoi(char* str);
void u_opt(t_tar_info* tar);
t_header* fill_header(t_tar_info* tar, char* file_name);
void *my_memset(void *b, int c, int len);
char *my_strcat(char *dest, const char *src);
char* my_strncpy(char* pszDest, const char* pszSource, unsigned int n);
char *my_strcpy(char *dest, char *src);
int my_strlen(char* c);
int my_strcmp(const char *s1, const char *s2);
char* my_strstr(char *s1, char *s2);
int is_directory(char* file_name);
char*  make_path(char* parent, char* son);
void write_dir_files(char* dir_name, int fd_archieve, t_tar_info* tar);
long get_number_of_nulls(long char_index);
int is_exist(t_tar_info* tar, char* file_name);
t_argument* get_all_names(t_tar_info* tar);
char* get_content_file(char* file);
int do_next(t_header* header, char* file_name);
long getforfile_size(char* file);
int is_directory(char*);
long get_the_first_char_from_tar_archieve(int fd_tar, long offset); // not null of the last file
long get_the_last_char_from_tar_archieve(int fd_tar); // not null of the last file
void write_tar_for_file(int fd_archieve, char* file_name, t_tar_info* tar);
t_argument* get_entry_names_of_tar(t_tar_info* tar, int fd_archieve);
void fill_similar_files(t_tar_info* tar, t_argument** exists);
void modify_tar_array(t_tar_info** tar, t_argument* exists);
long get_the_length_of_linked_list(t_argument* list);
long get_last_modificaton_time(char* file_name);
int check_for_time(char* prev, char* newer);
t_argument* get_only_updated(t_tar_info** tar, int fd_archieve, t_argument* exists, t_argument** updateds, long* offset);
void get_updated_files(t_tar_info* tar, int fd_archieve, t_argument* exists, t_argument** updateds);
long get_file_size_by_name(char* name);
int create_file(char* file_name);
void extract_tar(t_tar_info* tar);
int is_exist_element(t_argument** list, char* target);