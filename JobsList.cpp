//
// Created by jonat on 10/04/2020.
//

#include "JobsList.h"
#include <ctime>

JobsList::~JobsList() {
    for (auto i : jobs)
        delete i;
}

void JobsList::addJob(Command *cmd,pid_t p, bool onBG) {
    //PRINT_START;

    update();
    /*if(jobs.size() >= 100) //TODO: define & throw
        ;//*/
    jobs.push_back(new JobEntry(cmd,++maxId,p));


    if (!onBG)
        bringFG(maxId);

    //PRINT_END;
}

void JobsList::printJobsList() {
    update();

    if (jobs.empty())
        return;

    for (auto i : jobs){
        cout << "[" << i->getJobId() << "] " << i->getCmd()->print();
        cout << " : " << i->getJobPid() << " ";
        if(i->getStatus() != RUN){
            cout <<  std::difftime(i->getStopTime(), i-> getStartTime()) << " secs (stopped)";
        }else{
            time_t temp;
            time(&temp);
            cout << difftime(time(nullptr), i-> getStartTime()) << " secs";
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
    getJobById(jobId)->killCmd();
}

void JobsList::update() {
    //PRINT_START;

    runFG();
    removeFinishedJobs();

    if(jobs.empty())
        maxId = 0;
    else
        maxId = jobs.back()->getJobId();
    //PRINT_END;
}

bool JobsList::contains(int jobId) {
    for (auto i: jobs)
        if( i->getJobId() == jobId)
            return true;
    return false;
}

void JobsList::removeFinishedJobs() {
    //PRINT_START;
    vector<JobEntry*> temp;
    for(auto i : jobs){
        assert (i != NULL);
        //cout << "Flag removeFinishedJobs | jobId: " << i->getJobId() << endl;

        i->updateStatus();
        if(i->getStatus() != END) {
            temp.push_back(i);
        }else{
            delete i;
        }
    }
    jobs.clear();

    for(auto i:temp)
        jobs.push_back(i);

    //PRINT_END;
}

int JobsList::getSize() {
    return jobs.size();
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    if(jobs.empty())
        throw emptyList();

    JobEntry* temp = jobs.back();
    if(lastJobId)
        *lastJobId = temp->getJobId();
    return temp;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    if(jobs.empty())
        throw emptyList();

    JobEntry* lastStopped = NULL;
    for(auto i : jobs){
        if (i->getStatus() == STOP)
            lastStopped = i;
    }

    if(!lastStopped){
        throw notExist();
    }

    return lastStopped;
}

void JobsList::printKilledCommand(JobsList::JobEntry *job) {
    cout << job->getJobPid() << ": " << job->getCmd()->getCmdLine() << endl;
}

void JobsList::killCommand(JobsList::JobEntry *job, bool toPrint) {
    if (toPrint)
        printKilledCommand(job);

    delete job->getCmd();
    kill(job->getJobPid(), SIGKILL);
    update();
}

void JobsList::killAllJobs() {
    for(auto i : jobs) {
        killCommand(i);
    }
    update();
}

void JobsList::sendSigById(int sig, int jobId) {
    PRINT_START_PARAM(jobId);
   // update();

    JobEntry* job;
    if( jobId == 0){
        if(!fg){

            return;
            //TODO: How to handle
        }
        job = fg;
    }
    else
        job = getJobById(jobId);


    if(sig == SIGKILL){
        removeJobById(jobId);
    }else if (sig == SIGSTOP){
        job->stopCmd();
        //if(fg != NULL && job->getJobId() == fg->getJobId()){
         //   fg = NULL;
        //}
    }else if(sig == SIGCONT){
        job->continueCmd();
    }else{
        kill(job->getJobPid(),sig);
    }
    PRINT_END_PARAM(jobId);
}

void JobsList::bringFG(int jobId) {
    //PRINT_START;
    assert((fg == NULL));
    //TODO: debug

    fg = getJobById(jobId);

    if(fg->getStatus() == STOP)
        fg->continueCmd();

    update();
    //PRINT_END;
}

void JobsList::resumeOnBG(int jobId) {
    update();
    if(jobId == 0){
        getLastStoppedJob()->continueCmd();
    }else{
        JobEntry* job = getJobById(jobId);
        if (job->getStatus() == RUN)
            throw inBG();

        job->continueCmd();
    }
}

void JobsList::runFG() {
    //PRINT_START;
    if (!fg)
        return;

    do {
        fg->updateStatus();
    } while (fg->getStatus() == RUN);

    fg = NULL;

    //PRINT_END;
}

pid_t JobsList::fgPid() {
    if(fg)
        return fg->getJobPid();
    return 0;
}
