#include "source.h"



void set_flag(t_tar_info* tar,char argv)
{
  switch(argv)
  {
    case 'c' : tar->tarFlags->c = 1; break;
    case 'r' : tar->tarFlags->r = 1; break;
    case 't' : tar->tarFlags->t = 1; break;
    case 'u' : tar->tarFlags->u = 1; break;
    case 'x' : tar->tarFlags->x = 1; break;
    case 'v' : tar->tarFlags->v = 1; break;
    case 'f' : tar->tarFlags->f = 1; break;
    default  : printf("my_tar: error: unable option!\n");
    tar->tarFlags->err = 1;
    break;
  }


}

void set_flags(t_tar_info* tar, char* argv)
{
  int i = 0;
  
  if(argv[0] == '-')
  {
    i = 1;  // skip dash from parsing
  }
  
  for(;i < my_strlen(argv); i++)
  {
    set_flag(tar, argv[i]);
  }

}