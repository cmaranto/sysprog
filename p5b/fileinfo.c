#include <sys/stat.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //get filename from arg1
    char *filename = argv[1];

    //stat buffer
    struct stat _stat;

    //call stat
    stat(filename,&_stat);

    //format permissions
    char pformat[10];
    const char *perms = "-rwxrwxrwx";
    int mask = 1;

    for(int i = 9; i >= 1; --i){
        if((_stat.st_mode & mask) > 0){
            pformat[i] = perms[i];
        }else{
            pformat[i] = '-';
        }
        mask *= 2;
    }

    if((_stat.st_mode & 0170000) == 0040000){
        pformat[0] = 'd';
    }else{
        pformat[0] = '-';
    }

    //print with formatting
    printf("inode #:  %ld\n",_stat.st_ino);
    printf("permissions:  %s\n",pformat);
    printf("user:  %s\n",getpwuid(_stat.st_uid)->pw_name);
    printf("group:  %s\n",getgrgid(_stat.st_gid)->gr_name);
    printf("size:  %ld\n",_stat.st_size);
    printf("filesystem blocks:  %ld\n",_stat.st_blksize);
    printf("last access time:  %s",ctime(&_stat.st_atim.tv_sec));
    printf("last modify time:  %s",ctime(&_stat.st_mtim.tv_sec));
    printf("status change time:  %s",ctime(&_stat.st_ctim.tv_sec));

    return 0;
}