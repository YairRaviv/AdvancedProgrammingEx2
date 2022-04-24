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

void mode_to_letters(int mode);
int print_user_name_by_uid(uid_t uid);
int print_group_by_gid(gid_t gid);
int check_is_hidden(const char *pathname );
char *PATH;
int ARGC = 0;

static int              /* Callback function called by ftw() */
dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if(check_is_hidden(pathname)== 1)return 0;
    if(strcmp(pathname,".")==0)
    {
        printf(".\n");
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
    printf("%ld]  ",(long)sbuf->st_size);
    printf("%s\n" , filename);
    return 0;                                   /* Tell nftw() to continue */
}
int main(int argc, char *argv[])
{
    int flags = 0;
    if (argc < 2) 
    {
        if (nftw(".", dirTree, 10, flags) == -1) 
        {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
        else
        {
            exit(EXIT_SUCCESS);
        }
    }
    else
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
        }
        else
        {
            printf("directory doesn't exist!\n");
            exit(EXIT_FAILURE);
        }
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
    if((pw_ptr = getpwuid(uid) ) == NULL)
    {
        printf("%d",uid);
        return 1;
    }
    else{
        printf("%s   ",pw_ptr->pw_name);
    }
    return 0;
}

int print_group_by_gid(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    if((grp_ptr = getgrgid(gid) ) == NULL)
    {
        printf("%d",gid);
        return 1;
    }
    else{
        printf("%s          ",grp_ptr->gr_name);
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
















































// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <string.h>
// #include <time.h>
// #include <limits.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <pwd.h>
// #include <grp.h>
// #include <dirent.h>
// #include <sys/stat.h>
 
// size_t dir_count =0;
// size_t files_count =0;
// int print_file_details(char * filename, struct stat *info_p,int space_count);
// int dir_iterate(char *dirname, int space_count);
// void dostat(char *filename ,int space_count);
// void mode_to_letters(int mode);
// int print_user_name_by_uid(uid_t uid);
// int print_group_by_gid(gid_t gid);
// int main(int argc, char * argv[])
// {
//     char dir_name[PATH_MAX];
//     if(argc > 1)
//     {
//         //dir_name = argv[1]; 
//     }
//     else
//     {
//         if(getcwd(dir_name,sizeof(dir_name))!=NULL)
//         {
//             int spaces = 0;
//             int ans = dir_iterate(dir_name,spaces);
//         }
//         else
//         {
//             return -1;
//         }
//     }
//     //printf("\n%zu directories, %zu files\n", dir_count - 1, files_count);
//     return 0;
// }

// int dir_iterate(char *dirname, int space_count)
// {
//     //NOTE - we should convert dirname to be the full path of this file.
//     DIR *dir_ptr;
//     struct dirent *direntp;
//     struct stat *temp_stat;
//     char DirName[PATH_MAX];
//     getcwd(DirName,sizeof(DirName));
//     //NOTE - we should check if the current file/directory is an hidden file/the original directory of the execution.
//     if(strcmp(dirname , DirName)!=0 && strcmp(dirname , "..")!=0 && strcmp(dirname , ".")!=0 && strcmp(dirname , ".git")!=0)
//     {
//         dostat(dirname,space_count);
//     }
//     if((dir_ptr = opendir( dirname )) == NULL)
//     {
//         return 0;
//     }
//     dir_count++;

//     //NOTE - this while loop are iterate over hidden files/directories such as - "." , ".." , ".git" etc.
//     while ((direntp= readdir(dir_ptr)) != NULL)
//     {
//         if(direntp->d_type!=4)
//         {
//             files_count++;
//         }
//         dir_iterate(direntp->d_name,space_count+1);
//     }
//     return 0;
// }

// void dostat(char *filename,int space_count)
// {
//     struct stat info;
//     if( (stat(filename,&info))== -1)
//     {
//         perror(filename);
//     }
//     else
//     {
//         //printf("%s\n" , filename);
//         print_file_details(filename,&info,1);
//     }
// }

// int print_file_details(char * filename, struct stat *info_p,int space_count)
// {
//     for (size_t i = 0; i < space_count; i++)
//     {
//         printf("    ");
//     }
//     printf("|___ [");
//     mode_to_letters(info_p->st_mode);
//     print_user_name_by_uid(info_p->st_uid);
//     print_group_by_gid(info_p->st_gid);
//     printf("%ld]  ",(long)info_p->st_size);
//     printf("%s\n",filename);
//     return 0;
// }
// void mode_to_letters(int mode)
// {
//     char *str;
//     strcpy(str,"---------- ");
//     if(S_ISDIR(mode)){
//         str[0] = 'd';
//     }
//     if(S_ISCHR(mode)){
//         str[0] = 'c';
//     }
//     if(S_ISBLK(mode)){
//         str[0] = 'b';
//     }

//     if(mode & S_IRUSR){
//         str[1] = 'r';
//     }
//     if(mode & S_IWUSR){
//         str[2] = 'w';
//     }
//     if(mode & S_IXUSR){
//         str[3] = 'x';
//     }

//     if(mode & S_IRGRP){
//         str[4] = 'r';
//     }
//     if(mode & S_IWGRP){
//         str[5] = 'w';
//     }
//     if(mode & S_IXGRP){
//         str[6] = 'x';
//     }

//     if(mode & S_IROTH){
//         str[1] = 'r';
//     }
//     if(mode & S_IWOTH){
//         str[2] = 'w';
//     }
//     if(mode & S_IXOTH){
//         str[3] = 'x';
//     }

//     printf("%s",str);
// }
// int print_user_name_by_uid(uid_t uid)
// {
//     struct passwd *getpwuid(), *pw_ptr;
//     if((pw_ptr = getpwuid(uid) ) == NULL)
//     {
//         printf("%d",uid);
//         return 1;
//     }
//     else{
//         printf("%s   ",pw_ptr->pw_name);
//     }
//     return 0;
// }

// int print_group_by_gid(gid_t gid)
// {
//     struct group *getgrgid(), *grp_ptr;
//     if((grp_ptr = getgrgid(gid) ) == NULL)
//     {
//         printf("%d",gid);
//         return 1;
//     }
//     else{
//         printf("%s          ",grp_ptr->gr_name);
//     }
//     return 0;
// }