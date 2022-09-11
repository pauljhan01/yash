#ifndef _JOB_H
#define _JOB_H

struct Job{
     int pgid;
     int jobid;
     int pipe; //1 if pipe, 0 if not
     char* status;
     struct Command *lch;
     struct Command *rch;
     struct Job *next;
};

int findBGToken(char *commandLine);

int findFGToken(char *commandLine);

struct Job *findCurJob(struct Job *firstJob);

void createJob(char *commandLine, struct Job *job);

struct Job *createFirstJob(char *commandLine);

void freeJob(struct Job *job);

#endif