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
    update();
    /*if(jobs.size() >= 100) //TODO: define & throw
        ;//*/
    jobs.push_back(new JobEntry(cmd,++maxId,p));

    cout << "add job before FG" << endl;

    if (!onBG)
        bringFG(maxId);

    cout << "end of addJobs" << endl;
}

void JobsList::printJobsList() {
    update();

    if (jobs.empty())
        return;

    //cout << "Job List: " << endl;

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
    runFG();
    removeFinishedJobs();

    cout << "flag update. jobs size : "<< jobs.size() << endl;

    if(jobs.empty())
        maxId = 0;
    else
        maxId = jobs.back()->getJobId();

    cout << "end of update. PID" << getpid() << endl;
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

    cout <<"Flag end of removeFinishedjobs" << endl;
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

    if(!lastStopped)
        throw notExist();

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
    for(auto i : jobs){
        killCommand(i);
    }

}

void JobsList::sendSigById(int sig, int jobId) {
    update();

    JobEntry* job;
    if( job == 0){
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
        if(fg != NULL && job->getJobId() == fg->getJobId()){
            fg = NULL;
        }
    }else if(sig == SIGCONT){
        job->continueCmd();
    }else{
        kill(getJobById(jobId)->getJobPid(),sig);
    }
}

void JobsList::bringFG(int jobId) {
    assert((fg == NULL));
    //TODO: debug

    fg = getJobById(jobId);
    cout << "flag bringFG: " + fg->getJobPid() <<endl;

    if(fg->getStatus() == STOP)
        fg->continueCmd();

    cout << "start update from bringFG" << endl;
    update();
    cout << "Flag : end of bringFG " << endl;
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
    if(!fg)
        return;

    do{
        fg->updateStatus();
    }while(fg->getStatus() == RUN);

    fg = NULL;


    /*
    if (fg != NULL){
        fg->updateStatus();
        int status;
        if(fg->getStatus() == RUN){
            waitpid(fg->getJobPid(),&status, WUNTRACED);
        }
        //TODO: will cause an error in killCmd() [it will try to wait on death child]

        if(WIFSTOPPED(status)){
            fg->stopCmd();
        }else{
            fg->killCmd();
        }

        fg = NULL;
    }*/
}

pid_t JobsList::fgPid() {
    if (fg)
        return fg->getJobPid();
    return 0;
}
