#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //dirent pointer
    struct dirent *entry;

    //get current directory
    DIR* dir = opendir(".");

    //loop all entries
    unsigned long my_ino;
    while((entry = readdir(dir)) != NULL){
        //find current directory
        if(strcmp(entry->d_name,".") == 0){
            my_ino = entry->d_ino;
            break;
        }
    }
    closedir(dir);

    //open parent dir
    dir = opendir("..");
    while((entry = readdir(dir)) != NULL){
        //find curr dir ino
        if(entry->d_ino == my_ino){
            printf("%s\n",entry->d_name);
            break;
        }
    }
    closedir(dir);

    return 0;
}