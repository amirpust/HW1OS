//
// Created by jonat on 10/04/2020.
//

#ifndef HW1_JOBSLIST_H
#define HW1_JOBSLIST_H

#include "Commands.h"


class JobsList {
public:
    class JobEntry {
        Command* cmd;
        bool isStopped;
        time_t startTime;
        time_t stopTime;
        int jobId;
    };

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs() {};
    ~JobsList() = default;
    void addJob(Command* cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};


#endif //HW1_JOBSLIST_H
