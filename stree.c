#define _XOPEN_SOURCE 600 /* Get nftw() */
#define __USE_XOPEN_EXTENDED 1
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
#include <ftw.h>
#include <errno.h>


int dirs_counter=0;
int files_counter =0;
void mode_to_letters(int mode);
int print_user_name_by_uid(uid_t uid);
int print_group_by_gid(gid_t gid);
int check_is_hidden(const char *pathname );
int print_name(const char* name,int type, int mode);
char *PATH;
int ARGC = 0;

static int              /* Callback function called by ftw() */
dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if(check_is_hidden(pathname)== 1)return 0;
    if(strcmp(pathname,".")==0)
    {
        printf("\033[0;34m");
        printf(".\n");
        printf("\033[0m");
        return 0;
    }
    else if (ARGC > 1 && strcmp(pathname,PATH)==0)
    {
        printf("%s\n" , PATH);
        return 0;
    }
    char *filename;
    int length = strlen(pathname);
    int start = 0;
    for(int i = length-1;i>=0;i--)
    {
        if(pathname[i] == '/')
        {
            start = i+1;
            break;
        }
    }
         filename = (pathname+start);
    
    if(filename[0] == '.')return 0;
    if (type == FTW_NS) 
    {                  /* Could not stat() file */
        printf("?");
    }
    for(int i=0;i<(4*(ftwb->level-1));i++)
    {
        if(i%4 == 0){printf("│");}
        else {printf(" ");}
    }
    printf("├── [");
    mode_to_letters(sbuf->st_mode);
    print_user_name_by_uid(sbuf->st_uid);
    print_group_by_gid(sbuf->st_gid);
    printf("%15ld]  ",(long)sbuf->st_size);
    print_name(filename,type,sbuf->st_mode);
    if (type == FTW_F)
        files_counter++;
    if (type == FTW_D && strcmp(".", filename) != 0)
        dirs_counter++;
    return 0;                                   /* Tell nftw() to continue */
}
int main(int argc, char *argv[])
{
    int flags = 0;
    char * dirs;
    char * files; 
    if (argc < 2) 
    {
        if (nftw(".", dirTree, 10, flags) == -1) 
        {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
        else
        {
                        if (dirs_counter == 1)
                dirs = "directory";
            else
                dirs = "directories";
            if (files_counter == 1)
                files = "file";
            else
                files = "files";
            printf("\n%d %s, %d %s\n", dirs_counter, dirs, files_counter, files);
            exit(EXIT_SUCCESS);
        }
    }
    else if ( argc ==2)
    {
        ARGC = argc;
        PATH = argv[1];
        DIR* dir = opendir(argv[1]);
        if(dir)
        {
            if (nftw(argv[1], dirTree, 10, flags) == -1) 
            {

                perror("nftw");
                exit(EXIT_FAILURE);
            }
            else{
                                 if (dirs_counter == 1)
                dirs = "directory";
            else
                dirs = "directories";
            if (files_counter == 1)
                files = "file";
            else
                files = "files";
            printf("\n%d %s, %d %s\n", dirs_counter, dirs, files_counter, files);
            exit(EXIT_SUCCESS);
            }
        }
        else
        {
            printf("directory doesn't exist!\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        printf("too many arguments for this function\n");
        return -1;
    }
    exit(EXIT_SUCCESS);
}
void mode_to_letters(int mode)
{

    char str[11] = "---------- ";
    if(S_ISDIR(mode))
    {
        str[0] = 'd';
    }
    if(S_ISCHR(mode))
    {
        str[0] = 'c';
    }
    if(S_ISBLK(mode))
    {
        str[0] = 'b';
    }

    if(mode & S_IRUSR)
    {
        str[1] = 'r';
    }
    if(mode & S_IWUSR)
    {
        str[2] = 'w';
    }
    if(mode & S_IXUSR)
    {
        str[3] = 'x';
    }
    if(mode & S_IRGRP)
    {
        str[4] = 'r';
    }
    if(mode & S_IWGRP)
    {
        str[5] = 'w';
    }
    if(mode & S_IXGRP)
    {
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
    printf("%s",str);
}
int print_user_name_by_uid(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        printf("%d", uid);
        return 1;
    }
    else
    {
        printf("%s\t ", pw_ptr->pw_name);
    }
    return 0;
}

int print_group_by_gid(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        printf("%d", gid);
        return 1;
    }
    else
    {
        printf("%s ", grp_ptr->gr_name);
    }
    return 0;
}
int check_is_hidden(const char *pathname )
{
    int length = strlen(pathname);
    int start = 0;
    for(int i = 1;i<length-1;i++)
    {

        if((pathname[i] == '/') && pathname[i+1] == '.')
        {
            return 1;
        }
    }
    return 0;   
}
int print_name(const char* name,int type, int mode)
{
         if (type == FTW_D)
            {printf("\e[1;94m");
            }
        else if (type == FTW_F)
           {
               if(strstr(name, ".zip"))
               {printf("\e[1;91m");}
               else if((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH))
               {printf("\e[1;92m");}
             }
             printf("%s\n" , name);
               printf("\033[0m");

}
