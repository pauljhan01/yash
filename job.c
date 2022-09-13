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
        free(job->commandLine);
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

void changeStatus(void){
    while(fJob!=NULL){
        if(fJob->pgid==cpid){
            fJob->status = 1;
        }
        if(fJob->pgid==pid){
            fJob->status = 2;
            printf("[%d]");
        }
        fJob = fJob->next;
    }
}

struct Job *findCurJob(struct Job *firstJob){
    while(firstJob->next!=NULL){
        firstJob = firstJob->next;
    }
    return firstJob;
}

void printJobs(struct Job *firstJob){
    struct Job *tmp = firstJob;
    while(firstJob!=NULL){
        if(firstJob->status == 0){
            printf("[%d] - Running\t %s\n",firstJob->jobid,firstJob->commandLine);
        }
        firstJob = firstJob->next;
    }
    firstJob = tmp;
    while(firstJob!=NULL){
        if(firstJob->status == 1){
            printf("[%d] + Stopped\t %s\n",firstJob->jobid,firstJob->commandLine);
        }
        firstJob = firstJob->next;
    }
    
}

void handleBGJobs(char *commandLine, struct Job *firstJob){
    char *com_copy; char *com_free; char *token; char *saveptr;
    
    com_copy = com_free = strdup(commandLine);

    int x = 0;
    int y = 0;
    int jobNum = 0;

    while(token = strtok_r(com_copy," ",&saveptr)){
        if(atoi(token)>0 && atoi(token)<=20){
            jobNum = atoi(token);
        }
        com_copy = NULL;
    }
    struct Job *job = firstJob;
    if(jobNum==0){
        while(firstJob->next!=NULL){
            if(firstJob->status == 0 && firstJob->jobid > job->jobid){
                job = firstJob;
            }
            else{
                firstJob = firstJob->next;
            }
        }
        kill(job->pgid,SIGCONT);
    }
    else{
        while(firstJob->jobid != jobNum && firstJob->next != NULL){
            firstJob = firstJob->next;
        }
        job = firstJob;
        kill(job->pgid,SIGCONT);
    }
    free(com_free);
}

void handleFGJobs(char *commandLine, struct Job *firstJob){
    char *com_copy; char *com_free; char *token; char *saveptr;
    com_copy = com_free = strdup(commandLine);

    int x = 0;
    int y = 0;
    int jobNum = 0;

    while(token = strtok_r(com_copy," ",&saveptr)){
        if(atoi(token)>0 && atoi(token)<=20){
            jobNum = atoi(token);
        }
        com_copy = NULL;
    }

    struct Job *job = firstJob;
    if(jobNum==0){
        while(firstJob->next!=NULL){
            if(firstJob->status == 0 && firstJob->jobid > job->jobid){
                job = firstJob;
            }
            else{
                firstJob = firstJob->next;
            }
        }
        tcsetpgrp(0,job->pgid);
        kill(job->pgid,SIGCONT);
    }
    else{
        while(firstJob->jobid != jobNum && firstJob->next != NULL){
            firstJob = firstJob->next;
        }
        job = firstJob;
        tcsetpgrp(0,job->pgid);
        kill(job->pgid,SIGCONT);
    }
    free(com_free);
}

struct Job *createFirstJob(char *commandLine){
    char *token; char *token1; int background;
    struct Job *job = malloc(sizeof(struct Job));

    job->jobid = 1;
    job->prev = NULL;
    job->next = NULL;
    job->status = 0;
    int pipefd[2];
    job->pipe = 0;
    job->commandLine = commandLine;
    job->background = 0;

    int i = 0;
    int index = 0;
    char pip = '|';
    

    while(commandLine[index] && commandLine[index] != pip){
        index++;
        i++;
    }
    int back = 0;
    while(commandLine[back]){
        if(commandLine[back]=='&'){
            job->background = 1;
        }
        back++;
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
        executeCmd(job);
        free(token);
    }

    return job;

}

void createJob(char *commandLine, struct Job *firstJob){
    char *job_copy; char *job_free; char *token; char *saveptr; char *token1; int background;
    struct Job *job = malloc(sizeof(struct Job));
    //job_copy = job_free = strdup(commandLine);


    job->commandLine = commandLine;
    firstJob->next = job;
    job->prev = firstJob;
    job->jobid = firstJob->jobid+1;
    job->next = NULL;
    job->background = 0;
    job->pipe = 0;
    job->status = 1;
    int pipefd[2];

    int i = 0;
    int index = 0;
    char pip = '|';


    while(commandLine[index] && commandLine[index] != pip){
        index++;
        i++;
    }

    int back = 0;
    while(commandLine[back]){
        if(commandLine[back]=='&'){
            job->background = 1;
        }
        back++;
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
        executeCmd(job);
        free(token);
    }
    //job->lch = createChild(strndup(commandLine,i));
    //free(job_free);
}