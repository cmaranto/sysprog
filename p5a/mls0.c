#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//same as NULL
#define EMPTY (void *)0

int main(int argc, char *argv[]) {
    //dirent pointer
    struct dirent *entry;

    //get current directory
    DIR* dir = opendir(".");

    //loop all entries
    while((entry = readdir(dir)) != EMPTY){
        //only print visible entries
        if(entry->d_name[0] != '.'){
            write(1,entry->d_name,strlen(entry->d_name));
            write(1,"\n",1);
        }
    }

    return 0;
}