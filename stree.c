#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

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

int dir_iterate(char dirname[], int distance)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    if((dir_ptr = opendir( dirname )) == NULL)
    {
        fprintf(stderr,"ls1: cannot open $s\n",dirname);
    }
    else{
        print_file_details("file_path");

    }
   
  
  
 

    return 0;
}

int print_file_details(char * filename, struct stat *info_p,int space_count)
{
    for (size_t i = 0; i < space_count; i++)
    {
        printf("    ");
    }
    printf("|___ [");
    mode_to_letters(info_p->st_mode);
    print_user_name_by_uid(info_p->st_uid);
    print_group_by_gid(info_p->st_gid);
    printf("%81d]  ",(long)info_p->st_size);
    printf("%s\n",filename);
    return 0;
}
void mode_to_letters(int mode)
{
    char *str;
    strcpy(str,"__________ ");
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
int print_user_name_by_uid(uid_t uid)
{
    char * temp_name = getpwuid(uid)->pw_name;
    printf("%s   ",temp_name);
  
    return 0;
}

int print_group_by_gid(gid_t gid)
{
    char * temp_group= getgrgid(gid)->gr_name;
    printf("%s          ",temp_group);
  
    return 0;
}