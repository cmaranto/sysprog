#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //dirent pointer
    struct dirent *entry;

    unsigned long curr_ino,parent_ino;
    DIR *dir = NULL;

    for(;;){
        //open curr dir
        dir = opendir(".");
        //get curr/parent ino
        while((entry = readdir(dir)) != NULL){
            if(strcmp(entry->d_name,".") == 0){
                curr_ino = entry->d_ino;
            }else if(strcmp(entry->d_name,"..") == 0){
                parent_ino = entry->d_ino;
            }
        }
        closedir(dir);

        //in root?
        if(curr_ino == parent_ino){
            printf("/\n");
            break;
        }

        //open parent
        chdir("..");
        dir = opendir(".");
        //print child name
        while((entry = readdir(dir)) != NULL){
            if(entry->d_ino == curr_ino){
                printf("%s ",entry->d_name);
                break;
            }
        }
        closedir(dir);
    }
    return 0;
}