//
// Created by jonat on 10/04/2020.
//

#include "SmallShell.h"
#include <stdio.h>

#define MAX_ARGS 20

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    // for example:
    Command* cmd = CreateCommand(cmd_line);
    cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
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


SmallShell::SmallShell() : jobs(), defaultName("smash"), name(defaultName),dirHistory() {
// TODO: add your implementation
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit"
    };

    //TODO:
    char** args = new char*[MAX_ARGS];
    int args_num = _parseCommandLine(cmd_line, args);
    string command = string(args[0]);

    if (command == commands[0]){
        return new chpromptCommand(args[1]);
    }
    if (command == commands[1]){
        return new showpidCommand();
    }
    if (command == commands[2]){
        return new pwdCommand();
    }
    if (command == commands[3]){
        if (args_num > 2)
            throw new Command::tooManyArgs("cd");
        return new cdCommand(args[1]);
    }
    if (command == commands[4]){
        return new jobsCommand();
    }
    if (command == commands[5]){
        if (args_num != 3)
            throw Command::invalidArgs("kill");
        return new killCommand(args[1], args[2]);
    }
    if (command == commands[6]){
        if(args_num != 2)
            throw Command::invalidArgs("fg");
        return new fgCommand(args[1]);
    }
    if (command == commands[7]){
        if(args_num > 2)
            throw Command::invalidArgs("bg");
        return new bgCommand(args[1]);
    }
    if (command == commands[8]){
        return new quitCommand(args[1]);
    }

    return nullptr;
}

const char *SmallShell::popLastDir() {
    const char* dir = dirHistory.top();
    dirHistory.pop();
    return dir;
}

void SmallShell::pushDir(const char *dir) {
    dirHistory.push(dir);
}

int SmallShell::dirHistorySize() {
    return dirHistory.size();
}
