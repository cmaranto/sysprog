 /* msh7.c JM a mini shell */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define CMD_LENGTH 100
#define TOKENS 10

void command_stoa(char **command, char *comamand_text);
void mcd(const char* dir);

int main(void) {
    //ignore signals in main
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);

    size_t line_size = CMD_LENGTH;
    char *input_line = (char *) malloc(CMD_LENGTH + 1);    
    char *command[TOKENS];
    int cmd_pid, cmd_status, execstatus;
    const char *exit_str = "exit";
    const char *cd_str = "cd";

    while(1) {
        printf("|msh> ");
        getline(&input_line, &line_size, stdin);

        if(strncmp(input_line,exit_str,strlen(exit_str)) == 0){
            //handle exit command
            exit(0);
        }else if(strncmp(input_line,cd_str,strlen(cd_str)) == 0){
            command_stoa(command, input_line);
            mcd(command[1]);
            continue;
        }

        cmd_pid = fork();
        if (cmd_pid == 0) {
            //restore signal in child
            signal(SIGINT,SIG_DFL);
            signal(SIGQUIT,SIG_DFL);
            command_stoa(command, input_line);
            if(execvp(command[0], command) == -1){
                //handle invalid command
                exit(-1);
            }            
        }
        else {
            wait(&cmd_status);
        }
    }
    free(input_line);
}

void command_stoa(char **command, char *command_text) {
    char* token =strtok(command_text, " ");
    int token_index = 0;
    while(token != NULL) {
        command[token_index] = token;
        token = strtok(NULL, " ");
        token_index++;
    }

    char *last_token = command[token_index -1];
    int length = strlen(last_token);
    last_token[length -1] = 0;
    command[token_index] = 0;
}

void mcd(const char *dir){
    chdir(dir);
}
