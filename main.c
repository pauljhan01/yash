#include <stdlib.h>
#include <readline/readline.h>
#include "command.h"
#include "job.h"

int main(){
    char *commandLine = "ls";
    //commandLine = readline("# ");
    struct Job *job = createJob(commandLine);
    freeJob(job);
    return 0;
    /*
    free(commandLine);
    free(cmd);
    return 0;*/
}