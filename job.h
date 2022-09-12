#ifndef _JOB_H
#define _JOB_H


int cpid, cpid1, pid;
struct Job *fJob;
struct Job{
     int pgid;
     int jobid;
     char* commandLine;
     int pipe; //1 if pipe, 0 if not
     int status; //0 if running, 1 if stopped
     struct Command *lch;
     struct Command *rch;
     struct Job *next;
     struct Job *prev;
};

struct Job *findCurJob(struct Job *firstJob);

void handleBGJobs(char *commandLine, struct Job *firstJob);

void handleFGJobs(char *commandLine, struct Job *firstJob);

void printJobs(struct Job *firstJob);

void createJob(char *commandLine, struct Job *job);

struct Job *createFirstJob(char *commandLine);

void freeJob(struct Job *job);

#endif