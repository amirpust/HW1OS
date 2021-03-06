//
// Created by jonat on 10/04/2020.
//

#include "SmallShell.h"
#include <stdio.h>

#define MAX_ARGS 20

void SmallShell::executeCommand(const char *cmd_line) {
    PRINT_START;
    PRINT_PARAM(jobs.getSize());
    char *cmd_l = new char[strlen(cmd_line) + 1];
    strcpy(cmd_l, cmd_line);
    redirectionType rdType = noRedirect;

    bool onBG = _isBackgroundComamnd(cmd_l);
    _removeBackgroundSign(cmd_l);

    char redirection[COMMAND_ARGS_MAX_LENGTH];
   /* if (rdType != noRedirect) {
        _getRedirection(cmd_l, redirection);
        _removeRedirection(cmd_l);
    }*/

    Command *cmd = CreateCommand(cmd_l,cmd_line);

    PRINT_PARAM(cmd->getType());
    if (cmd->getType() == builtIn) {
        int stdOut = -1;

        if (rdType != noRedirect) {
            stdOut = dup(1);
            prepare(redirection, rdType);
        }
        cmd->execute();
        cleanUp(stdOut);
        PRINT_END_PARAM(jobs.getSize());
        return;

    } else if (cmd->getType() == external) {
        pid_t pid = fork();
        if (pid == 0) {//Child
            setpgrp();
            if (rdType != noRedirect) {
                prepare(redirection, rdType);
            }
            cmd->execute();
            exit(-1); //we'll probably use execv and wont reach here.
        } else {                                      //Parent
            PRINT_PARAM(pid);
            jobs.addJob(cmd, pid, onBG);
        }
    }
    PRINT_END;
}
const std::string SmallShell::getName() const {
    return name;
}

void SmallShell::setName(const char* s) {
    if (!s)
        name = defaultName;
    else
        name = string(s);
}


/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line, const char* og_line) {
    PRINT_PARAM(cmd_line);
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit"
    };
    //TODO: check for white space
    string str = string(cmd_line);
    if (str.find(commands[0]) == 0)
        return new chpromptCommand(cmd_line,og_line);

    if (str.find(commands[1]) == 0)
        return new showpidCommand(cmd_line,og_line);

    if (str.find(commands[2]) == 0)
        return new pwdCommand(cmd_line,og_line);

    if (str.find(commands[3]) == 0)
        return new cdCommand(cmd_line,og_line);

    if (str.find(commands[4]) == 0)
        return new jobsCommand(cmd_line,og_line);

    if (str.find(commands[5]) == 0)
        return new killCommand(cmd_line,og_line);

    if (str.find(commands[6]) == 0)
        return new fgCommand(cmd_line,og_line);

    if (str.find(commands[7]) == 0)
        return new bgCommand(cmd_line,og_line);

    if (str.find(commands[8]) == 0)
        return new quitCommand(cmd_line,og_line);

    return new ExternalCommand(cmd_line,og_line);
}

const char *SmallShell::popLastDir() {
    delete[] currentDir;
    currentDir  = new char[strlen(dirHistory.top()) + 1];
    strcpy(currentDir,dirHistory.top());
    delete[] dirHistory.top();
    dirHistory.pop();
    return currentDir;
}

void SmallShell::pushDir(const char *dir) {
    dirHistory.push(currentDir);

    currentDir = new char[strlen(dir) + 1];
    strcpy(currentDir,dir);
}

int SmallShell::dirHistorySize() {
    return dirHistory.size();
}

char *SmallShell::getCurrentDir() const {
    return currentDir;
}

void SmallShell::setCurrentDir(char *currentDir) {
    SmallShell::currentDir = currentDir;
}

JobsList &SmallShell::getJobs() {
    return jobs;
}

pid_t SmallShell::getPid() const {
      return myPid;
}
