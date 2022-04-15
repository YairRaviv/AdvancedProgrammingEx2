#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>


int main(int argc, char * argv[])
{
    
     
    struct DIR *dir;
    struct dirent *dir_ant;
    char cwd_dir [PATH_MAX];

    if(getcwd(cwd_dir,sizeof(cwd_dir))!=NULL)
    {
      dir = opendir(cwd_dir);
      if(dir!=NULL)
      {

      }
    }

 

    return 0;
}

int dir_iterate(struct DIR * dir_ant)
{
    print_file_details("file_path");
    if()
  
 

    return 0;
}

int print_file_details(FILE * cur_file)
{
    
  
 
    return 0;
}
void mode_to_letters(int mode)
{
    char *str;
    strcpy(str,"__________");
    if(S_ISDIR(mode)){
        str[0] = 'd';
    }
    if(S_ISCHR(mode)){
        str[0] = 'c';
    }
    if(S_ISBLK(mode)){
        str[0] = 'b';
    }

    if(mode & S_IRUSR){
        str[1] = 'r';
    }
    if(mode & S_IWUSR){
        str[2] = 'w';
    }
    if(mode & S_IXUSR){
        str[3] = 'x';
    }

    if(mode & S_IRGRP){
        str[4] = 'r';
    }
    if(mode & S_IWGRP){
        str[5] = 'w';
    }
    if(mode & S_IXGRP){
        str[6] = 'x';
    }

    if(mode & S_IROTH){
        str[1] = 'r';
    }
    if(mode & S_IWOTH){
        str[2] = 'w';
    }
    if(mode & S_IXOTH){
        str[3] = 'x';
    }

    printf(str);
    return 0;
}