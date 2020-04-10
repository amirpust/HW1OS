
#ifndef HW1_COMMANDEXCEPTIONS_H
#define HW1_COMMANDEXCEPTIONS_H

//#include "Commands.h"
#include <exception>
#include <iostream>

using std::exception;
using std::string;

class commandException: public exception {
    const char* cmd;

protected:

    virtual const char * print() const= 0;

public:
    commandException(const char *cmd) : cmd(cmd) {}

    const char *what() const noexcept override {
        string str = "smash error: ";
        str.append(cmd);
        str.append(": ");
        str.append(print());
        return str.c_str();
    }
};

class invalidArgs : public commandException{

protected:
    const char * print() const override  {
        return " invalid arguments";
    }
};

class tooManyArgs : public commandException{
public:
    explicit tooManyArgs(const char* cmd): commandException(cmd){};
    const char * print() const override {
        return " too many arguments";
    }
};

class emptyDirHistory : public commandException{
public:
    explicit emptyDirHistory(const char* cmd) : commandException(cmd){};
protected:
    const char *print() const override {
        return " OLDPWD not set";
    }
};



class jobNotExists : public commandException{
    int jobId;

protected:
    const char *print() const override {
        return ("job-id " + std::to_string(jobId) + string(" does not exist")).c_str();
    }

public:
    jobNotExists(const char* _cmd, int _jobId): commandException(_cmd), jobId(_jobId){};
};

#endif //HW1_COMMANDEXCEPTIONS_H
