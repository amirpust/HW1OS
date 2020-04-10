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
        };

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
        //TODO: Throw an exception
        return nullptr;
    }
    void removeJobById(int jobId){

    }
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);

    void update(){
        removeFinishedJobs();}

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
