#include <iostream>
#include <signal.h>
#include "signals.h"
#include "SmallShell.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    kill(getpid(), SIGSTOP);
    cout << "smash: got ctrl-Z" << endl;

}

void ctrlCHandler(int sig_num) {
    kill(getpid() ,SIGKILL);
    cout<< "smash: got ctrl-C"<<endl;
    cout<< "smash: process " + std::to_string(getpid()) + " was killed"<<endl;
    //exit(9);
}

void alarmHandler(int sig_num) {
//    cout << "smash got an alarm" <<  endl;
//    const char* cmd = SmallShell::getInstance();//TODO: get last command in the shell
//    cout << "smash: " + cmd + "timed out!"<< endl;
}

