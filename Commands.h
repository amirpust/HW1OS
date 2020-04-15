#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include "CommandExceptions.h"
#include <string.h>
#include <fcntl.h>

using std::vector;
using std::string;

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)
typedef enum {builtIn, external, pipeCmd, redirection} cmdType;
typedef enum{override, append} redirectionType;
class Command {
protected:
    const char* cmd_line;
    char* args[COMMAND_MAX_ARGS];
    int size;
    const cmdType type;
public:
    Command(const char* cmd_line, cmdType);
    virtual ~Command() = default;
    virtual void execute() = 0;
    const char* print()const;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
    bool isFinished();;
    const cmdType getType() const;

    const char *getCmdLine() const;

};


class BuiltInCommand : public Command {
 public:
 explicit BuiltInCommand(const char* cmd_line) : Command(cmd_line, builtIn) {};
  virtual ~BuiltInCommand() = default;
};


class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line): Command(cmd_line, external){};
  virtual ~ExternalCommand() = default;
  void execute() override{
        ///I know its ugly but this way we dont allocate dynamic memory...
        char bash[5] = "bash";
        char flag[3] = "-c";
        char cmd[COMMAND_ARGS_MAX_LENGTH + 1];
        strcpy(cmd,cmd_line);
        char* cmd_args[4];
        cmd_args[0] = bash;
        cmd_args[1] = flag;
        cmd_args[2] = cmd;
        cmd_args[3] = NULL;
//TODO : free the allocated memory

        //string bash = "bash";
        //string flag = "-c";
        //cmd_args[0] = new char[bash.size() + 1];

        //cmd_args[1] = new char[flag.size() + 1];
        //cmd_args[2] = new char[strlen(cmd_line) + 1];
        //strcpy(cmd_args[0], flag.c_str());
        //strcpy(cmd_args[1], flag.c_str());
        //strcpy(cmd_args[2], cmd_line);


        execvp("/bin/bash",cmd_args);

  }
};


class PipeCommand : public Command {
  // TODO: Add your data members
 public:
  PipeCommand(const char* cmd_line): Command(cmd_line,pipeCmd){};
  virtual ~PipeCommand() {}
  void execute() override;
};

/*
class RedirectionCommand : public Command {
    int fd;
    int fdDup;
public:
    explicit RedirectionCommand(const char* cmd_line)
    : Command(cmd_line, redirection){
        // check what to do if number if args is less than needed

    };
    virtual ~RedirectionCommand() {}

    void execute() override{
        //use create
    }

    //prepare will open the wanted file
    //TODO, find first ">"
    void prepare(){
        if(strcmp(args[1],">")){
            //overwrite a file
            fd = open(args[2],O_CREAT, "w");
            close(1);//closes stdout
            fdDup = dup(fd);
        }else{
            //append to file ">>"

        }
    };
  //void cleanup() override;
};*/

//changes prompt's name
class chpromptCommand : public BuiltInCommand{
public:
    explicit chpromptCommand(const char* cmd_line);
    void execute() override;

};

//printing the pid of the shell
class showpidCommand : public BuiltInCommand{

public:
    explicit showpidCommand(const char* cmd_line);

    void execute() override;
};

//printing the path of the shell
class pwdCommand : public BuiltInCommand{
public:
    explicit pwdCommand(const char* cmd_line) : BuiltInCommand(cmd_line){};
    void execute() override;
};

//changes shell directory
class cdCommand : public BuiltInCommand{
public:
    explicit cdCommand(const char* cmd_line);
    void execute() override;
};

//printing all the jobs in jobList in sorted order
class jobsCommand : public BuiltInCommand{

public:
    explicit jobsCommand(const char* cmd_line);
    void execute() override ;
};


class killCommand : public BuiltInCommand{
    int sigNum;
    int jobId;

public:
    explicit killCommand(const char* cmd_line);
    void execute() override;
};


class fgCommand : public BuiltInCommand{
    int jobId;

public:
    fgCommand(const char* cmd_line);
    void execute() override ;
};


class bgCommand : public BuiltInCommand{
    int jobId;
public:
    bgCommand(const char* cmd_line);

    void execute() override;
};


class quitCommand : public BuiltInCommand{
public:
    quitCommand(const char* cmd_line);
    void execute() override;
};

/*
class ChangeDirCommand : public BuiltInCommand {
// TODO: Add your data members public:
  ChangeDirCommand(const char* cmd_line, char** plastPwd);
  virtual ~ChangeDirCommand() {}
  void execute() override;
};


class GetCurrDirCommand : public BuiltInCommand {
 public:
  GetCurrDirCommand(const char* cmd_line);
  virtual ~GetCurrDirCommand() {}
  void execute() override;
};


class ShowPidCommand : public BuiltInCommand {
 public:
  ShowPidCommand(const char* cmd_line);
  virtual ~ShowPidCommand() {}
  void execute() override;
};
*/
/*

class QuitCommand : public BuiltInCommand {
// TODO: Add your data members public:
  QuitCommand(const char* cmd_line, JobsList* jobs);
  virtual ~QuitCommand() {}
  void execute() override;
};


class CommandsHistory {
 protected:
  class CommandHistoryEntry {
	  // TODO: Add your data members
  };
 // TODO: Add your data members
 public:
  CommandsHistory();
  ~CommandsHistory() {}
  void addRecord(const char* cmd_line);
  void printHistory();
};


class HistoryCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  HistoryCommand(const char* cmd_line, CommandsHistory* history);
  virtual ~HistoryCommand() {}
  void execute() override;
};


class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsCommand(const char* cmd_line, JobsList* jobs);
  virtual ~JobsCommand() {}
  void execute() override;
};


class KillCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  KillCommand(const char* cmd_line, JobsList* jobs);
  virtual ~KillCommand() {}
  void execute() override;
};


class ForegroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  ForegroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~ForegroundCommand() {}
  void execute() override;
};


class BackgroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  BackgroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~BackgroundCommand() {}
  void execute() override;
};
*/

// TODO: should it really inhirit from BuiltInCommand ?
class CopyCommand : public BuiltInCommand {
 public:
  CopyCommand(const char* cmd_line);
  virtual ~CopyCommand() {}
  void execute() override;
};
void _removeBackgroundSign(char* cmd_line);
bool _isBackgroundComamnd(const char* cmd_line);


#endif //SMASH_COMMAND_H_
