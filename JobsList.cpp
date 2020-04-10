//
// Created by jonat on 10/04/2020.
//

#include "JobsList.h"
#include <time.h>








JobsList::~JobsList() {
    for (auto i : jobs)
        delete i;
}

void JobsList::addJob(Command *cmd, bool isStopped) {
    update();
    if(jobs.size() >= 100) //TODO: define & throw
        ;//
    jobs.push_back(new JobEntry(cmd, isStopped, ++maxId));
}

void JobsList::printJobsList() {
    update();

    cout << "Job List: " << endl;

    for (auto i : jobs){
        cout << "[" << i->getJobId() << "] " << i->getCmd()->print();
        cout << " : " << i->getCmd()->getCommandPid() << " ";
        if(i->isStopped()){
            cout << i->getStartTime() - i->getStopTime()<< " secs (stopped)";
        }else{
            time_t temp;
            time(&temp);
            cout << i->getStartTime() - temp << " secs";
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
    update();

    for (auto i : jobs)
        if(i->getJobId() == jobId){
            //TODO
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
    for (auto i : jobs){
        if (i->getCmd()->isFinished());
        //    jobs.erase(i);
        //TODO: memory leak
    }
}


