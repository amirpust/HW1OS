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
            stopTime = time(nullptr);
            stopped = true;
            kill(pid, SIGSTOP);
        }
        void continueCmd(){
            stopped = false;
            kill(pid, SIGCONT); //TODO:Check
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
    class inBG: public std::exception{

    };

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    JobEntry* fg;

    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs(), fg(NULL) {};
    ~JobsList();
    void addJob(Command* cmd,pid_t p,bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    int getSize();

    void removeJobById(int jobId);

    bool contains(int jobId);

    void sendSigById(int sig, int jobId = 0);

    void bringFG(int jobId);
    void resumeOnBG(int jobId);

    JobEntry * getJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId = NULL);
    JobEntry *getLastStoppedJob(int *jobId = NULL);
    void killCommand(JobEntry* job, bool toPrint = true);
private:
    void removeFinishedJobs();
    void printKilledCommand(JobEntry* job);
    void runFG();
    void update();

    // TODO: Add extra methods or modify exisitng ones as needed
};


#endif //HW1_JOBSLIST_H
