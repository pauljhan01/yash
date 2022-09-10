#ifndef _JOB_H
#define _JOB_H

struct Job{
     int pgid;
     int pipe;
     char* status;
     struct Command *lch;
     struct Command *rch;
     struct Job *next;
};

struct Job *createJob(char *commandLine);

void freeJob(struct Job *job);

#endif