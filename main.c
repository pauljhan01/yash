#include <stdlib.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include "command.h"
#include "job.h"

static void sigint_handler(int signo){
    kill(-cpid,SIGINT);
}

static void sigtstp_handler(int signo){
    kill(-cpid,SIGTSTP);
}


int main(){
    char *commandLine; char *scommandLine;
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    commandLine = readline("# ");
    struct Job *firstJob = createFirstJob(commandLine);
    fJob = firstJob;
    struct Job *curJob = firstJob;
    while(scommandLine = readline("# ")){
        if(strncmp(scommandLine,"bg",2) == 0){
            handleBGJobs(scommandLine,firstJob);
        }
        if(strncmp(scommandLine,"fg",2) == 0){
            handleFGJobs(scommandLine,firstJob);
        }
        if(strncmp(scommandLine,"jobs",4) == 0){
            printJobs(firstJob);
        }
        else{ //segfaulting when only enter is inputted, lch and rch are null pointers
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
    
    return 0;
    /*
    free(commandLine);
    free(cmd);
    return 0;*/
}