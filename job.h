#ifndef _JOB_H
#define _JOB_H

struct Job{
     int pgid;
     int pipe; //1 if pipe, 0 if not
     char* status;
     struct Command *lch;
     struct Command *rch;
     struct Job *next;
};

struct Job *createJob(char *commandLine);

void freeJob(struct Job *job);

#endif