#ifndef _COMMAND_H
#define _COMMAND_H

#include "job.h"



typedef struct Command{
    char **parsed;
    char **redir;
    int pCount;
    int rCount;

} Command;

Command *createChild(char *commandLine);

void executeCmd(struct Job *Job);

void executePipeCmd(struct Job *job);

#endif