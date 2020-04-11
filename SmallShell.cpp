//
// Created by jonat on 10/04/2020.
//

#include "SmallShell.h"
#include <stdio.h>

#define MAX_ARGS 20

void SmallShell::executeCommand(const char *cmd_line) {
    char* cmd_l = new char[strlen(cmd_line) + 1];
    strcpy(cmd_l,cmd_line);
    bool onBG = !_isBackgroundComamnd(cmd_l);
    _removeBackgroundSign(cmd_l);
    Command* cmd = CreateCommand(cmd_line);

    if(cmd->getType() == builtIn){
        cmd->execute();
    }else if(cmd->getType() == external){
        pid_t pid = fork();
        if(pid == 0){                                //Child
            cmd->execute();
            exit(-1); //we'll probably use execv and wont reach here.
        }else{                                      //Parent
            jobs.addJob(cmd,pid);
            if(!onBG){
                int status;
                waitpid(pid, &status);
                if(status == -1)            //TODO : handle exception
                    exit(-1);
                return;
            }else{
                return;
            }
        }
    }
    //TODO: add more types
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
Command * SmallShell::CreateCommand(const char* cmd_line) {
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit"
    };

    string str = string(cmd_line);
    if (str.find(commands[0]) == 0)
        return new chpromptCommand(cmd_line);

    if (str.find(commands[1]) == 0)
        return new showpidCommand(cmd_line);

    if (str.find(commands[2]) == 0)
        return new pwdCommand(cmd_line);

    if (str.find(commands[3]) == 0)
        return new cdCommand(cmd_line);

    if (str.find(commands[4]) == 0)
        return new jobsCommand(cmd_line);

    if (str.find(commands[5]) == 0)
        return new killCommand(cmd_line);
/*
    if (str.find(commands[6]) == 0)
        return new fgCommand(cmd_line);

    if (str.find(commands[7]) == 0)
        return new bgCommand(cmd_line);

    if (str.find(commands[8]) == 0)
        return new quitCommand(cmd_line);
*/
    return nullptr;
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
