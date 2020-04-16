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
typedef enum{override, append, noRedirect} redirectionType;
typedef  enum{pipeRegular,pipeStderr,noPipe}pipeType;

class Command {
protected:
    const char* cmd_line; //without '&'
    char* args[COMMAND_MAX_ARGS];// split cmd_line
    int size;//size of args
    const cmdType type;
    const char* original_cmd;

    Command(const Command& toCopy):cmd_line(toCopy.cmd_line){

    };
public:
    Command(const char* cmd_line,const char *original_line, cmdType);

    virtual ~Command() = default;//TODO: delete allocated memory
    virtual void execute() = 0;
    const char* print()const;
    //virtual void prepare();
    //virtual void cleanup();
    const cmdType getType() const;
};

class BuiltInCommand : public Command {
 public:
 explicit BuiltInCommand(const char* cmd_line, const char* og_line)
 : Command(cmd_line,og_line, builtIn) {};
  virtual ~BuiltInCommand() = default;
};

class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line, const char* og_line)
  : Command(cmd_line,og_line ,external){};
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

//changes prompt's name
class chpromptCommand : public BuiltInCommand{
public:
    explicit chpromptCommand(const char* cmd_line, const char* og_line);
    void execute() override;

};

//printing the pid of the shell
class showpidCommand : public BuiltInCommand{

public:
    explicit showpidCommand(const char* cmd_line, const char* og_line);

    void execute() override;
};

//printing the path of the shell
class pwdCommand : public BuiltInCommand{
public:
    explicit pwdCommand(const char* cmd_line, const char* og_line)
    : BuiltInCommand(cmd_line,og_line){};
    void execute() override;
};

//changes shell directory
class cdCommand : public BuiltInCommand{
public:
    explicit cdCommand(const char* cmd_line, const char* og_line);
    void execute() override;
};

//printing all the jobs in jobList in sorted order
class jobsCommand : public BuiltInCommand{

public:
    explicit jobsCommand(const char* cmd_line, const char* og_line);
    void execute() override ;
};


class killCommand : public BuiltInCommand{
    int sigNum;
    int jobId;

public:
    explicit killCommand(const char* cmd_line, const char* og_line);
    void execute() override;
};


class fgCommand : public BuiltInCommand{
    int jobId;

public:
    fgCommand(const char* cmd_line, const char* og_line);
    void execute() override ;
};


class bgCommand : public BuiltInCommand{
    int jobId;
public:
    bgCommand(const char* cmd_line, const char* og_line);

    void execute() override;
};


class quitCommand : public BuiltInCommand{
public:
    quitCommand(const char* cmd_line, const char* og_line);
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
redirectionType _isredirection(const char* cmd_line);
void _getRedirection(char* cmd_line,char* direction);
void _removeRedirection(char* cmd_line);
void prepare(char* path,redirectionType rd);
void cleanUp(int fd);
redirectionType identifyRedirection(char* cmd_line, bool *bg, char* cmd_args);
pipeType identifyPipe(char* cmd_line,char* left, char* right);

#endif //SMASH_COMMAND_H_
