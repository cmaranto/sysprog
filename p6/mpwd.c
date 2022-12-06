#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void print_path_to_current_directory();

int main(int argc, char *argv[]) {
    print_path_to_current_directory();
    printf("\n");
    return 0;
}

int has_children = 0;

void print_path_to_current_directory(){
    struct dirent *entry;
    unsigned long curr_ino,parent_ino;
    const char *curr_name;

    //name of curr dir
    DIR *dir = opendir(".");

    //get curr ino and name
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") == 0){
            curr_ino = entry->d_ino;
            break;
        }
    }
    closedir(dir);
    //chdir to parent
    chdir("..");
    dir = opendir(".");
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") == 0){
            parent_ino = entry->d_ino;            
        }else if(entry->d_ino == curr_ino){
            curr_name = entry->d_name;
        }
    }
    
    //in root?
    if(curr_ino == parent_ino){
        //print root name and return
        if(!has_children){
            printf("/");
        }
        return;
    }else{
        ++has_children;
        //recursive call        
        print_path_to_current_directory();
        //print / name of curr dir
        printf("/%s",curr_name);
    }
}