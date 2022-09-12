#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include "command.h"
#include "job.h"

void freeJob(struct Job *job){
    struct Job *tmp;
    while(job->next!=NULL){
        tmp = job;
        if(job->pipe == 0){
            for(int i = 0;i<job->lch->pCount;i++){
                free(job->lch->parsed[i]);
            }
            free(job->lch->parsed);
            if(job->lch->redir!=NULL){
                for(int i = 0;i<job->lch->rCount;i++){ 
                    free(job->lch->redir[i]);
                }
                free(job->lch->redir);
            }
            free(job->lch);
        }
        if(job->pipe == 1){
            for(int i = 0;i<job->lch->pCount;i++){
                free(job->lch->parsed[i]);
            }
            free(job->lch->parsed);
            if(job->lch->redir!=NULL){
                for(int i = 0;i<job->lch->rCount;i++){ 
                    free(job->lch->redir[i]);
                }
                free(job->lch->redir);
            }
            free(job->lch);
            if(job->rch!=NULL){
                for(int i = 0;i<job->rch->pCount;i++){
                    free(job->rch->parsed[i]);
                }
                free(job->rch->parsed);
                if(job->rch->redir!=NULL){
                    for(int i = 0;i<job->rch->rCount;i++){
                        free(job->rch->redir[i]);
                    }
                    free(job->rch->redir);
                }
                free(job->rch);
            }
        }
        job = job->next;
        free(tmp);
    }
    
    
    /*if(job->status!=NULL){
        free(job->status);
    }*/

    /*for(int i = 0;i<cmd->pCount;i++){
        free(cmd->parsed[i]);
    }
    free(cmd->parsed);
    for(int i = 0;i<cmd->rCount;i++){
        free(cmd->redir[i]);
    }
    free(cmd->redir);*/
}

struct Job *findCurJob(struct Job *firstJob){
    while(firstJob->next!=NULL){
        firstJob = firstJob->next;
    }
    return firstJob;
}



struct Job *createFirstJob(char *commandLine){
    char *token; char *token1; int background;
    struct Job *job = malloc(sizeof(struct Job));

    job->jobid = 1;
    job->next = NULL;

    int pipefd[2];

    int i = 0;
    int index = 0;
    char pip = '|';
    

    while(commandLine[index] && commandLine[index] != pip){
        index++;
        i++;
    }
    while(commandLine[index]){
        if(commandLine[index]=='&'){
            background = 1;
        }
    }

    if(commandLine[index]==pip){
        job->pipe = 1;
        token = strndup(commandLine,i);
        commandLine = commandLine + i + 2;
        token1 = strdup(commandLine);
        job->lch = createChild(token);
        job->rch = createChild(token1);
        executePipeCmd(job);
        free(token);
        free(token1);
    }
    else{
        job->pipe = 0;
        token = strdup(commandLine);
        job->lch = createChild(token);
        executeCmd(job->lch);
        free(token);
    }

    return job;

}

void createJob(char *commandLine, struct Job *prevJob){
    char *job_copy; char *job_free; char *token; char *saveptr; char *token1;
    struct Job *job = malloc(sizeof(struct Job));
    //job_copy = job_free = strdup(commandLine);

    prevJob->next = job;
    job->jobid = prevJob->jobid+1;
    job->next = NULL;

    int pipefd[2];

    int i = 0;
    int index = 0;
    char pip = '|';


    while(commandLine[index] && commandLine[index] != pip){
        index++;
        i++;
    }

    if(commandLine[index]==pip){
        job->pipe = 1;
        token = strndup(commandLine,i);
        commandLine = commandLine + i + 2;
        token1 = strdup(commandLine);
        job->lch = createChild(token);
        job->rch = createChild(token1);
        executePipeCmd(job);
        free(token);
        free(token1);
    }
    else{
        job->pipe = 0;
        token = strdup(commandLine);
        job->lch = createChild(token);
        executeCmd(job->lch);
        free(token);
    }
    //job->lch = createChild(strndup(commandLine,i));
    //free(job_free);
}