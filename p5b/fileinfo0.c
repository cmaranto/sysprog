#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    //get filename from arg 1
    char *filename = argv[1];

    //stat buffer
    struct stat _stat;

    //call stat
    stat(filename,&_stat);

    //print values
    printf("inode #:  %ld\n",_stat.st_ino);
    printf("permissions:  %i\n",_stat.st_mode);
    printf("user:  %i\n",_stat.st_uid);
    printf("group:  %i\n",_stat.st_gid);
    printf("size:  %ld\n",_stat.st_size);
    printf("filesystem blocks:  %ld\n",_stat.st_blksize);
    printf("last access time:  %ld\n",_stat.st_atim.tv_sec);
    printf("last modify time:  %ld\n",_stat.st_mtim.tv_sec);
    printf("status change time:  %ld\n",_stat.st_ctim.tv_sec);

    return 0;
}