#include <utmp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    //open who file
    FILE * fd = fopen("/var/run/utmp","rb");

    //buffer for who structs
    struct utmp _utmp;
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        printf("ut_user: %s\n",_utmp.ut_user);
        printf("ut_line: %s\n",_utmp.ut_line);
        printf("ut_host: %s\n",_utmp.ut_host);
        printf("ut_tv_sec: %i\n",_utmp.ut_tv.tv_sec);
        
    }

    //close file
    fclose(fd);
    return 0;
}