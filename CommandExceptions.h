
#ifndef HW1_COMMANDEXCEPTIONS_H
#define HW1_COMMANDEXCEPTIONS_H

//#include "Commands.h"
#include <exception>
#include <iostream>


using std::exception;
using std::string;

class commandException: public exception {
protected:
    const char* cmd;

public:

    virtual string print() const= 0;
    commandException(const char *cmd) : cmd(cmd) {
    }


};


class invalidArgs : public commandException{

protected:
    string print() const override  {
        return  " error: " + string(cmd) + ": invalid arguments";
    }
};

class tooManyArgs : public commandException{
public:
    explicit tooManyArgs(const char* cmd): commandException(cmd){};
    string print() const override {
        return  " error: " + string(cmd) + ":  too many arguments";
    }
};

class emptyDirHistory : public commandException{
public:
    explicit emptyDirHistory(const char* cmd) : commandException(cmd){};
protected:
    string print() const override {
        return " error: " + string(cmd) + ":  OLDPWD not set";
    }
};



class jobNotExists : public commandException{
    int jobId;

protected:
    string print() const override {
        return ("job-id " + std::to_string(jobId) + string(" does not exist")).c_str();
    }

public:
    jobNotExists(const char* _cmd, int _jobId): commandException(_cmd), jobId(_jobId){};
};

#endif //HW1_COMMANDEXCEPTIONS_H
