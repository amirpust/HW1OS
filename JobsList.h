//
// Created by jonat on 10/04/2020.
//

#ifndef HW1_JOBSLIST_H
#define HW1_JOBSLIST_H

#include "Commands.h"
#include <exception>
#include <ctime>

using std::cout;
using std::endl;


class JobsList {
public:
    class JobEntry {
        Command* cmd;
        bool stopped;
        time_t startTime;
        time_t stopTime;
        int jobId;
        pid_t pid;
    public:
        JobEntry(Command* _cmd, bool _stopped, int _jobId, pid_t p) :
            cmd(_cmd), stopped(_stopped), jobId(_jobId),pid(p){
            startTime = time(nullptr);
        }
        ~JobEntry() {
            delete cmd;
        };

        Command *getCmd() const {
            return cmd;
        }

        bool isStopped() const {
            return stopped;
        }

        time_t getStartTime() const {
            return startTime;
        }

        time_t getStopTime() const {
            return stopTime;
        }

        int getJobId() const {
            return jobId;
        }

        bool operator==(const JobEntry &rhs) const {
            return jobId == rhs.jobId;
        }

        bool operator!=(const JobEntry &rhs) const {
            return !(rhs == *this);
        };

        pid_t getJobPid(){
            return pid;
        }

        void stopCmd(){
            stopped = true;
        }
        void continueCmd(){
            stopped = false;
        }
    };
    class notExist: public std::exception{
        int jobId;

        public:
        notExist(int _jobId = 0): jobId(_jobId){};

        const char *what() const noexcept override {
            return ("job-id " + std::to_string(jobId) + string(" does not exist")).c_str();
        }
    };
    class emptyList: public std::exception{

};

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs() {};
    ~JobsList();
    void addJob(Command* cmd,pid_t p,bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    int getSize();

    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId = NULL);
    JobEntry *getLastStoppedJob(int *jobId);

    void update();
    bool contains(int jobId);

    void killCommand(JobEntry* job, bool toPrint = true);
private:
    void removeFinishedJobs();
    void printKilledCommand(JobEntry* job);

    // TODO: Add extra methods or modify exisitng ones as needed
};


#endif //HW1_JOBSLIST_H
