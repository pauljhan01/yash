#include <stdlib.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"
#include "job.h"



int main(){
    char *commandLine = "ls | wc";
    //commandLine = readline("# ");
    struct Job *job = createJob(commandLine);
    freeJob(job);
    return 0;
    /*
    free(commandLine);
    free(cmd);
    return 0;*/
}