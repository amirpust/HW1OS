#include <iostream>
#include <signal.h>
#include "signals.h"
#include "SmallShell.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    cout << "smash: got ctrl-Z" << endl;
    //kill(SmallShell::getInstance().getJobs().fgPid(), SIGSTOP);
    SmallShell::getInstance().getJobs().sendSigById(sig_num);
}

void ctrlCHandler(int sig_num) {
    cout<< "smash: got ctrl-C"<<endl;
    cout<< "smash: process " + std::to_string(SmallShell::getInstance().getJobs().fgPid()) + " was killed"<<endl;
    SmallShell::getInstance().getJobs().sendSigById(sig_num);
    //kill(SmallShell::getInstance().getJobs().fgPid() ,SIGKILL);
    //exit(9);
}

void alarmHandler(int sig_num) {
//    cout << "smash got an alarm" <<  endl;
//    const char* cmd = SmallShell::getInstance();//TODO: get last command in the shell
//    cout << "smash: " + cmd + "timed out!"<< endl;
}

