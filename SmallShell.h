#ifndef HW1_SMALLSHELL_H
#define HW1_SMALLSHELL_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "JobsList.h"
#include <stack>
#include <unistd.h>

class SmallShell {
private:
    //TODO: Add your data members
    JobsList jobs;
    const std::string defaultName;
    char* currentDir;
    std::string name;
    std::stack<const char*> dirHistory;
    SmallShell() : jobs(), defaultName("smash"), name(defaultName),dirHistory(){
        currentDir = new char[4096] ;
        getcwd(currentDir, 4096);
    };

public:
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell(){
        while(!dirHistory.empty()){
            popLastDir();
        }
        delete []currentDir;
    };
    void executeCommand(const char* cmd_line);
    const std::string getName() const;
    void setName(const char* s);
    //TODO: add extra methods as needed
    const char* popLastDir();
    void pushDir(const char* dir);
    int dirHistorySize();
    char *getCurrentDir() const;
    void setCurrentDir(char *currentDir);

    void printJobs() {
        jobs.printJobsList();
    }
};


#endif //HW1_SMALLSHELL_H
