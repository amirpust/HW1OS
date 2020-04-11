#include <iostream>
#include <signal.h>
#include "signals.h"
#include "SmallShell.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    if(SmallShell::getJobs().getSize() == 0)
        return;

    SmallShell::getInstance().getJobs().getLastJob(NULL)->stopCmd();
    kill(SmallShell::getInstance().getJobs().getLastJob()->getJobPid(), sig_num);
    cout << "smash: got ctrl-Z" << endl;

}

void ctrlCHandler(int sig_num) {
    if(SmallShell::getJobs().getSize() == 0)
        return;

    pid_t pid = SmallShell::getInstance().getJobs().getLastJob()->getJobPid();
    SmallShell::getInstance().getJobs().killCommand(
            SmallShell::getInstance().getJobs().getLastJob(),false);
    cout<< "smash: got ctrl-C"<<endl;
    cout<< "smash: process " + std::to_string(pid) + " was killed"<<endl;

}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
}

