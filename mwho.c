#include <utmp.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //open who file
    FILE * fd = fopen("/var/run/utmp","rb");

    //buffer for who structs
    struct utmp _utmp;
    
    //read/print who structs into buffer
    while(fread(&_utmp,sizeof(struct utmp),1,fd)){
        //only print normal users
        if(_utmp.ut_type == USER_PROCESS){
            //format time
            time_t _time = _utmp.ut_tv.tv_sec;
            char *t = ctime(&_time);
            if (t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0';
            
            printf("%s\t%s\t%s (%s)\n",_utmp.ut_user,
                                        _utmp.ut_line,
                                        t,
                                        _utmp.ut_host);
        }


    }

    //close file
    fclose(fd);
    return 0;
}