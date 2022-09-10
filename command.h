#ifndef _COMMAND_H
#define _COMMAND_H

typedef struct Command{
    char **parsed;
    char **redir;
    int pCount;
    int rCount;
} Command;

Command *createChild(char *commandLine, struct Job *job);

void executeCmd(struct Job *job);
//?
#endif