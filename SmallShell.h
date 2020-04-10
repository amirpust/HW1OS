//
// Created by jonat on 10/04/2020.
//

#ifndef HW1_SMALLSHELL_H
#define HW1_SMALLSHELL_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Commands.h"
#include "JobsList.h"

class SmallShell {
private:
    // TODO: Add your data members
    JobsList jobs;

    const std::string defaultName;
    std::string name;
    SmallShell();
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
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    const std::string getName() const;
    void setName(const char* s);
    // TODO: add extra methods as needed

};


#endif //HW1_SMALLSHELL_H