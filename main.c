#include <stdlib.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"
#include "job.h"



int main(){
    char *commandLine; char *scommandLine;

    commandLine = readline("# ");
    struct Job *firstJob = createFirstJob(commandLine);
    struct Job *curJob = firstJob;
    while(scommandLine = readline("# ")){
        createJob(scommandLine, curJob);
        curJob = findCurJob(firstJob);
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