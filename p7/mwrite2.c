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
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        if(strcmp(dev,_utmp.ut_line) == 0){
            if(strcmp(_utmp.ut_host,"127.0.0.1") != 0){
                strcpy(hostname,_utmp.ut_host);
                return 0;
            }
        }
    }

    //close file
    fclose(fd);
    return 1;
}

void formatCTime(char *ctimestr, char *formatted){
    int i;
    for(i = 11; i < 16; ++i){        
        formatted[i-11] = ctimestr[i]; 
    }
    formatted[i-11] = 0;
}

void formatTerm(char *termstr, char *formatted){
    int i;
    for(i = 5; i < strlen(termstr); ++i){        
        formatted[i-5] = termstr[i]; 
    }
    formatted[i-5]=0;
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
        char ftime_str[6];
        formatCTime(time_str,ftime_str);

        char *name = getenv("LOGNAME");
        char *term = ttyname(STDIN_FILENO);
        char fterm[strlen(term)];
        formatTerm(term,fterm);

        
        char hostname[UT_HOSTSIZE];
        if(hostname_lookup(fterm,hostname) != 0){
            gethostname(hostname,UT_HOSTSIZE);
        }
        
        sprintf(header,"Message from %s@%s on %s at %s ...\n",
                 name,hostname,fterm,ftime_str);


        int fd = open(dev,O_WRONLY);

        //write header
        write(fd,(void*)header,strlen(header));

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