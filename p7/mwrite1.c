#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <string.h>

int user_check(const char *username, const char *dev){
        //open who file
    FILE * fd = fopen("/var/run/utmp","rb");

    //buffer for who structs
    struct utmp _utmp;
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        if(strcmp(dev,&_utmp.ut_line[0]) == 0 &&
        strcmp(username,&_utmp.ut_user[0]) == 0)
        return 0;
    }

    //close file
    fclose(fd);
    return 1;
}


int main(int argc, char *argv[]) {
    if(argc != 3)return -1;

    char dev[11];

    sprintf(dev,"/dev/%s",argv[2]);

    if(user_check(argv[1],argv[2]) == 0){
        int fd = open(dev,O_WRONLY);
        char buf;
        while(read(STDIN_FILENO,(void*)&buf,1) == 1){
            write(fd,(void*)&buf,1);
        }
        write(fd,"EOF\n",4);
    }else{
        printf("write: %s is not logged in on %s\n",argv[1],argv[2]);
    }



    return 0;
}