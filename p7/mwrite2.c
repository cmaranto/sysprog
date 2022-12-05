#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define HEADERSIZE 500

int user_check(const char *username, const char *dev){
        //open who file
    FILE * fd = fopen("/var/run/utmp","rb");

    //buffer for who structs
    struct utmp _utmp;
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        if(strcmp(dev,_utmp.ut_line) == 0 &&
            strcmp(username, _utmp.ut_user) == 0)
        return 0;
    }

    //close file
    fclose(fd);
    return 1;
}

int hostname_lookup(const char *dev, char *hostname){
    //open who file
    FILE * fd = fopen("/var/run/utmp","rb");

    //buffer for who structs
    struct utmp _utmp;

    //remove /dev/ from string
    char new_dev[strlen(dev)];
    for(int i = 5; i < strlen(dev)+1; ++i){        
        new_dev[i-5] = dev[i]; 
    }
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        printf("%s\n",_utmp.ut_host);
        if(strcmp(new_dev,_utmp.ut_line) == 0){
            strcpy(hostname,_utmp.ut_host);
            return 0;
        }
    }

    //close file
    fclose(fd);
    return 1;
}


int main(int argc, char *argv[]) {
    if(argc != 3)return -1;

    char dev[11];
    char hostname[UT_HOSTSIZE];

    sprintf(dev,"/dev/%s",argv[2]);


    if(user_check(argv[1],argv[2]) == 0){
        char header[HEADERSIZE];
        time_t t = time(NULL);
        char *time_str = ctime(&t);
        char *name = getenv("LOGNAME");
        char *term = ttyname(STDIN_FILENO);
        char hostname[UT_HOSTSIZE];
        
        // sprintf(header,"Message from %s@%s on %s at %s ...",
        //         getenv("LOGNAME"),hostname,ttyname(STDIN_FILENO),time_str);
        
        printf("%s\n",name);
        printf("%s\n",term);
        hostname_lookup(term,hostname);
        printf("%s\n",hostname);
        printf("%s\n",time_str);


        int fd = open(dev,O_WRONLY);
        char buf;
        for(;;){
            read(STDIN_FILENO,(void*)&buf,1);
            write(fd,(void*)&buf,1);
        }
    }else{
        printf("write: %s is not logged in on %s\n",argv[1],argv[2]);
    }



    return 0;
}