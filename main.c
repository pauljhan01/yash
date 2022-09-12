#include <stdlib.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include "command.h"
#include "job.h"

static void sig_handler(int signo){
    switch(signo){
        case SIGINT:
            kill(-cpid,SIGINT);
        case SIGTSTP:
            kill(-cpid,SIGTSTP);
    }
}

int main(){
    char *commandLine; char *scommandLine;
    commandLine = readline("# ");
    signal(SIGINT,sig_handler);
    struct Job *firstJob = createFirstJob(commandLine);
    struct Job *curJob = firstJob;
    while(scommandLine = readline("# ")){
        if(strncmp(scommandLine,"bg",2) == 0){
            printf("bg\n");
        }
        if(strncmp(scommandLine,"fg",2) == 0){

        }
        if(strncmp(scommandLine,"jobs",4) == 0){

        }
        else{
            createJob(scommandLine, curJob);
            curJob = findCurJob(firstJob);
        }
        
    }
    freeJob(firstJob);

    /*commandLine = readline("# ");
    struct Job *firstJob = createFirstJob(commandLine);

    struct Job *job;
    
    scommandLine = readline("# ");
    createJob(commandLine, firstJob);
    freeJob(firstJob);
    free(scommandLine);*/
    free(scommandLine);
    free(commandLine);
    
    return 0;
    /*
    free(commandLine);
    free(cmd);
    return 0;*/
}