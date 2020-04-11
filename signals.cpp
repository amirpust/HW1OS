#include <iostream>
#include <signal.h>
#include "signals.h"
#include "SmallShell.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    SmallShell::getInstance().getJobs().getLastJob(NULL)->stopCmd();
    kill(SmallShell::getInstance().getJobs().getLastJob()->getJobPid(), sig_num);
    cout << "smash: got ctrl-Z" << endl;

	// TODO: Add your implementation
}

void ctrlCHandler(int sig_num) {
  // TODO: Add your implementation
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
}

