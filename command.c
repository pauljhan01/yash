#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include "job.h"
#include "command.h"

#define READ_END 0
#define WRITE_END 1


void executeCmd(struct Job *job){
    int in; int out; int status; int pipefd[2]; pid_t cpid; pid_t cpid1; int y = 0;
    if(pipe(pipefd)<0){
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if(cpid == 0){  
        while(job->lch->redir[y]){
            if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],"<")==0){
                y++;
                if((in = open(job->lch->redir[y],O_RDONLY,0777))<0){
                    perror(job->lch->redir[y]);
                }
                dup2(in,0);
            }
            if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],">")==0){
                y++;
                if((out = creat(job->lch->redir[y],0777))<=0){
                    perror(job->lch->redir[y]);
                }
                dup2(out, STDOUT_FILENO);
            }
            if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],"2>")==0){
                y++;
                if((out = creat(job->lch->redir[y],0777))<0){
                    perror(job->lch->redir[y]);
                }
                dup2(out,2);
                }
            y++;
        }
        if(job->pipe == 1){
            dup2(pipefd[WRITE_END],STDIN_FILENO);
            execvp(job->lch->parsed[0],job->lch->parsed);
            cpid1 = fork();
            if(cpid1 == 0){
                dup2(pipefd[READ_END], STDOUT_FILENO);
                close(pipefd[READ_END]);
                while(job->lch->redir[y]){
                    if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],"<")==0){
                        y++;
                        if((in = open(job->lch->redir[y],O_RDONLY,0777))<0){
                            perror(job->lch->redir[y]);
                        }
                        dup2(in,0);
                    }
                    if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],">")==0){
                        y++;
                        if((out = creat(job->lch->redir[y],0777))<=0){
                            perror(job->lch->redir[y]);
                        }
                        dup2(out, STDOUT_FILENO);
                    }
                    if(job->lch->redir[y]!=NULL && strcmp(job->lch->redir[y],"2>")==0){
                        y++;
                        if((out = creat(job->lch->redir[y],0777))<0){
                            perror(job->lch->redir[y]);
                        }
                        dup2(out,2);
                    }
                    y++;
                }
            }
            else{
                wait(&status);
            }
        }
        else{
            execvp(job->lch->parsed[0],job->lch->parsed);
            exit(0);
        }
    }
    else{
        wait(&status);
    }
}

Command *createChild(char *commandLine){
    char *com_copy; char *com_free; char *token; char *saveptr; 
    char *com_copy1; char *com_free1; char *token1; char *saveptr1;
    struct Command *cmd = malloc(sizeof(struct Command));

    int in; int out;

    cmd->pCount = 0;
    cmd->rCount = 0;

    com_copy = com_free = strdup(commandLine);
    com_copy1 = com_free1 = strdup(commandLine);
    //if statement dealing with input pipe must go first if right child
    //if statement dealing with output pipe must go first if left child 
    int x = 0;
    int y = 0;
    while(token = strtok_r(com_copy," ",&saveptr)){
        if((strcmp(token,"<")!=0) && (strcmp(token,">")!=0) && (strcmp(token,"2>")!=0) && (strcmp(token,"|")!=0)){
            x++;
        }
        else if((strcmp(token,"<")==0) || (strcmp(token,">")==0) || (strcmp(token,"2>")==0) || (strcmp(token,"|")==0)){
            y++;
            y++; //to account for file
            token = strtok_r(com_copy," ",&saveptr);
        }
        com_copy = NULL;
    }
    cmd->pCount = x;
    cmd->rCount = y;
    
    cmd->parsed = malloc((x+1)*sizeof(char*));
    cmd->redir = malloc((y+1)*sizeof(char*));

    cmd->parsed[x]=NULL; //to make both arrays null terminated
    cmd->redir[y]=NULL;

    x = 0;
    y = 0;
    com_copy = com_free;

    while(token1 = strtok_r(com_copy1," ",&saveptr1)){
        if(strcmp(token1,">")!=0 && strcmp(token1,"<")!=0 && strcmp(token1,"2>")!=0){
            cmd->parsed[x] = strdup(token1);
            x++;
            com_copy1 = NULL;
        }
        else{
            cmd->redir[y] = strdup(token1);
            y++;
            token1 = strtok_r(com_copy1," ",&saveptr1);
            cmd->redir[y] = strdup(token1);
            y++;
            com_copy1 = NULL;
        }
    }
    free(com_free);
    free(com_free1);
    return cmd;
}