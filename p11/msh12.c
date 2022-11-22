 /* msh10.c JM a mini shell with command line */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#define CMD_LENGTH 100
#define TOKENS 10

typedef struct {
    int redout; // 0 for no redirection; 1 for redirection
    int redin; // same convention
    char* outfile;
    char* infile;
} redir_t;

int command_stoa(char **command, char *comamand_text, char *divider);
void mcd(const char* dir);
char** getopt_r(int token_count, char* command[], redir_t* redir);
void execute(int token_count, char **command);

int main(void) {
    //ignore signals in main
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);

    size_t line_size = CMD_LENGTH;
    char *input_line = (char *) malloc(CMD_LENGTH + 1);    
    char *command[TOKENS];
    char *pcommands[TOKENS];
    int cmd_pid, cmd_status, execstatus;
    const char *exit_str = "exit";
    const char *cd_str = "cd";

    
    int token_count = 0;
    int command_count = 0;

    while(1) {
        printf("|msh> ");
        getline(&input_line, &line_size, stdin);
        command_count = command_stoa(pcommands,input_line,"|");    

        if(strncmp(input_line,exit_str,strlen(exit_str)) == 0){
            //handle exit command
            exit(0);
        }else if(strncmp(input_line,cd_str,strlen(cd_str)) == 0){
            mcd(pcommands[1]);
            continue;
        }

        cmd_pid = fork();
        if (cmd_pid == 0) {
            if(command_count - 1 == 0){
                token_count = command_stoa(command, pcommands[0], " ");
                execute(token_count, command); 
                continue;
            }
            
            int the_pipe[2];
            int fd, pid;
            pipe(the_pipe);

            pid = fork();
            if(pid > 0){
                close(the_pipe[1]);
                dup2(the_pipe[0],0);
                close(the_pipe[0]);
                token_count = command_stoa(command, pcommands[1], " ");
                execute(token_count, command); 
            }else{
                close(the_pipe[0]);
                dup2(the_pipe[1],1);
                close(the_pipe[1]);
                token_count = command_stoa(command, pcommands[0], " ");
                execute(token_count, command); 
            }       
        }
        else {
            wait(&cmd_status);
        }
    }
    free(input_line);
}

void execute(int token_count, char **command){
    //restore signal in child
    signal(SIGINT,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);

    redir_t redir;
    redir.redin = 0;
    redir.redout = 0;
    char **red_command = getopt_r(token_count,command, &redir);

    //redirect
    if(redir.redin){
        close(0);
        open(redir.infile,O_RDONLY);
    }
    if(redir.redout){
        close(1);
        open(redir.outfile,O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
    }   

    if(execvp(command[0], red_command) == -1){
        //handle invalid command
        exit(-1);
    } 
}

int command_stoa(char **command, char *command_text, char *divider) {
    char* token =strtok(command_text, divider);
    int token_index = 0;
    while(token != NULL) {
        command[token_index] = token;
        token = strtok(NULL, divider);
        token_index++;
    }

    char *last_token = command[token_index -1];
    int length = strlen(last_token);
    if(last_token[length-1] == '\n'){
        last_token[length -1] = 0;
    }
    command[token_index] = 0;
    return token_index;
}

void mcd(const char *dir){
    chdir(dir);
}

char** getopt_r(int token_count, char* command[], redir_t* redir) {
    char** red_command = (char**) malloc((token_count+1)*sizeof(char**));
    red_command[0] = command[0];
    int index = 1, cindex = 1;
    while(command[index] != 0) {
        if (strcmp(command[index], ">") == 0) {
            redir->redout = 1;
            redir->outfile = command[index + 1];
            index++;
        }
        else if (strcmp(command[index],"<") == 0) {
            redir->redin = 1;
            redir->infile = command[index + 1];
            index++;
        }
        else {
            red_command[cindex] = command[index];
            cindex++;
        }
        index++;
    }
    red_command[cindex] = (char*) 0;
    return red_command;
}
