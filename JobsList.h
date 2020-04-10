//
// Created by jonat on 10/04/2020.
//

#ifndef HW1_JOBSLIST_H
#define HW1_JOBSLIST_H

#include "Commands.h"
#include <exception>


class JobsList {
public:
    class JobEntry {
        Command* cmd;
        bool stopped;
        time_t startTime;
        time_t stopTime;
        int jobId;
    public:
        JobEntry(Command* _cmd, bool stopped, int _jobId) :
        cmd(_cmd), stopped(_stopped), jobId(_jobId){
            time(&startTime);
        }

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

    };
    class notExist: public std::exception{
        int jobId;

        public:
        notExist(int _jobId): jobId(_jobId){};

        const char *what() const noexcept override {
            return ("job-id " + std::to_string(jobId) + string(" does not exist")).c_str();
        }
    };

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs() {};
    ~JobsList();
    void addJob(Command* cmd, bool isStopped = false){
        update();
        if(jobs.size() >= 100) //TODO: define & throw
            ;//
        jobs.push_back(new JobEntry(cmd, isStopped, ++maxId));
    }
    void printJobsList(){
        update();

        for (auto i : jobs){
            cout << "[" << i->getJobId() << "] " << i->getCmd()->print();
            cout << " : " << i->getCmd()->getPid() << " ";
            if(i->isStopped()){
                cout << i->getStartTime() - i->getStopTime()<< " secs (stopped)"
            }else{
                cout << i->getStartTime() - time() << " secs";
            }
            cout << endl;
        }
    }
    void killAllJobs(); //TODO

    JobEntry * getJobById(int jobId) {
        update();

        for (auto i : jobs)
            if(i->getJobId() == jobId)
                return i;

        throw notExist(jobId);
    }
    void removeJobById(int jobId){
        update();

        for (auto i : jobs)
            if(i->getJobId() == jobId){
                //TODO
            }
        throw notExist(jobId);
    }
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);

    void update() {
        removeFinishedJobs();
        if(jobs.empty())
            maxId = 0;
        else
            maxId = jobs.back()->getJobId();
    }
    bool contains(int jobId){
        for (auto i: jobs)
            if( i->getJobId() == jobId)
                return true;
        return false;
    }

private:
    void removeFinishedJobs(){
        for (vector<*JobEntry>::iterator i = jobs.begin(); i != jobs.end(); i++){
            if ((*i)->getCmd()->isFinished()){
                delete (*i)->getCmd();
                jobs.erase(i);
            }
        }
    } //TODO


    // TODO: Add extra methods or modify exisitng ones as needed
};


#endif //HW1_JOBSLIST_H
