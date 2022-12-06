#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    char dev[11];

    sprintf(dev,"/dev/%s",argv[2]);

    int fd = open(dev,O_WRONLY);

    char buf;
    while(read(STDIN_FILENO,(void*)&buf,1) == 1){
        write(fd,(void*)&buf,1);
    }
    write(fd,"EOF\n",4);

    return 0;
}