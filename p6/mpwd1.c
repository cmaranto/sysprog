#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //dirent pointer
    struct dirent *entry;

    unsigned long curr_ino,parent_ino;
    DIR *dir = NULL;

    do{
        //open curr dir
        dir = opendir(".");
        //get curr ino
        while((entry = readdir(dir)) != NULL){
            if(strcmp(entry->d_name,".") == 0){
                curr_ino = entry->d_ino;
                break;
            }
        }
        closedir(dir);

        //open parent
        chdir("..");
        dir = opendir(".");
        //get parent ino
        //print child name
        while((entry = readdir(dir)) != NULL){
            if(strcmp(entry->d_name,".") == 0){
                parent_ino = entry->d_ino;
            }else if(entry->d_ino == curr_ino){
                if(strcmp(entry->d_name,"..") != 0)
                    printf("%s ",entry->d_name);
            }
        }
        closedir(dir);
    }while(curr_ino != parent_ino);
    printf("\n");

    return 0;
}