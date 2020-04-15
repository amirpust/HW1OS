//
// Created by jonat on 10/04/2020.
//

#include "JobsList.h"
#include <ctime>

JobsList::~JobsList() {
    for (auto i : jobs)
        delete i;
}

void JobsList::addJob(Command *cmd,pid_t p) {
    update();
    /*if(jobs.size() >= 100) //TODO: define & throw
        ;//*/
    jobs.push_back(new JobEntry(cmd,++maxId,p));
}

void JobsList::printJobsList() {
    update();

    cout << "Job List: " << endl;

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
    runFG();
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

    auto i = jobs.end();
    i--;
    do{
        if((*i)->getStatus() != RUN){
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
    }else if (sig == SIGTSTP){
        job->stopCmd();
        if(jobId == fg->getJobId()){
            fg = NULL;
            //TODO: dill with times
        }
    }else if(sig == SIGCONT){
        job->continueCmd();
    }else{
        //TODO: try to figure the state of the process
        kill(getJobById(jobId)->getJobPid(),sig);
    }

    update();
}

void JobsList::bringFG(int jobId) {
    fg = getJobById(jobId);
    cout << "flag bringFG: " + fg->getJobPid() <<endl;
    fg->continueCmd();
    update();
}

void JobsList::resumeOnBG(int jobId) {
    if(jobId == 0){
        getLastStoppedJob()->continueCmd();
    }else{
        JobEntry* job = getJobById(jobId);
        if (job->getStatus() != RUN)
            throw inBG();

        job->continueCmd();
    }
}

void JobsList::runFG() {
    if (fg != NULL){
        fg->updateStatus();
        int status;
        if(fg->getStatus() == RUN){
            waitpid(fg->getJobPid(),&status, WUNTRACED);
        }

        if(WIFSTOPPED(status)){
            fg->setStatus(STOP);
        }else{
            fg->setStatus(END);
        }

        fg = NULL;
    }
}
