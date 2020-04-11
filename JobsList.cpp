//
// Created by jonat on 10/04/2020.
//

#include "JobsList.h"
#include <time.h>








JobsList::~JobsList() {
    for (auto i : jobs)
        delete i;
}

void JobsList::addJob(Command *cmd,pid_t p,bool isStopped) {
    update();
    if(jobs.size() >= 100) //TODO: define & throw
        ;//
    jobs.push_back(new JobEntry(cmd, isStopped, ++maxId,p));
}

void JobsList::printJobsList() {
    update();

    cout << "Job List: " << endl;

    for (auto i : jobs){
        cout << "[" << i->getJobId() << "] " << i->getCmd()->print();
        cout << " : " << i->getCmd()->getCommandPid() << " ";
        if(i->isStopped()){
            cout << i->getStopTime() - i-> getStartTime() << " secs (stopped)";
        }else{
            time_t temp;
            time(&temp);
            cout << temp - i->getStartTime()  << " secs";
        }
        cout << endl;
    }
}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    update();

    for (auto i : jobs)
        if(i->getJobId() == jobId)
            return i;

    throw notExist(jobId);
}

void JobsList::removeJobById(int jobId) {
    for(auto i = jobs.begin(); i != jobs.end(); i++){
        if((*i)->getJobId() == jobId){
            JobEntry* temp = *i;
            jobs.erase(i);
            delete temp;
            return;
        }
    }
    throw notExist(jobId);
}

void JobsList::update() {
    removeFinishedJobs();

    if(jobs.empty())
        maxId = 0;
    else
        maxId = jobs.back()->getJobId();
}

bool JobsList::contains(int jobId) {
    for (auto i: jobs)
        if( i->getJobId() == jobId)
            return true;
    return false;
}

void JobsList::removeFinishedJobs() {

    vector<JobEntry*> temp;
    for(auto i : jobs){
        int status;
        waitpid(i->getJobPid(), &status, WNOHANG);
        if(kill(i->getJobPid(),0) == 0) {
            temp.push_back(i);
        }else {
            delete i;
        }
    }
    jobs.clear();

    for(auto i:temp)
        jobs.push_back(i);
}

int JobsList::getSize() {
    return jobs.size();
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    JobEntry* temp = jobs.back();
    if(lastJobId)
        *lastJobId = temp->getJobId();
    return temp;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    if(jobs.empty())
        throw emptyList();
    auto i = jobs.end();
    i--;
    do{
        if((*i)->isStopped()){
            if(jobId)
                *jobId = (*i)->getJobId();
            return (*i);
        }
        i--;
    }while(i != jobs.begin());
    throw notExist();
}

void JobsList::printKilledCommand(JobsList::JobEntry *job) {
    cout << job->getJobPid() << ": " << job->getCmd()->getCmdLine() << endl;
}

void JobsList::killCommand(JobsList::JobEntry *job) {
    printKilledCommand(job);
    delete job->getCmd();
    kill(job->getJobPid(), SIGKILL);
}



