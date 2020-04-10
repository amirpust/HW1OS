#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include "SmallShell.h"

using std::vector;
using std::string;

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

class Command {
    const char* cmd_line;
protected:
    char** args;
    int size;
    // TODO: Add your data members
 public:
  Command(const char* cmd_line);;
  virtual ~Command() = default;
  virtual void execute() = 0;
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
 public:
  BuiltInCommand(const char* cmd_line) : Command(cmd_line) {};
  virtual ~BuiltInCommand() = default;
};

class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line);
  virtual ~ExternalCommand() {}
  void execute() override;
};

class PipeCommand : public Command {
  // TODO: Add your data members
 public:
  PipeCommand(const char* cmd_line);
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command {
 // TODO: Add your data members
 public:
  explicit RedirectionCommand(const char* cmd_line);
  virtual ~RedirectionCommand() {}
  void execute() override;
  //void prepare() override;
  //void cleanup() override;
};

class chpromptCommand : public BuiltInCommand{
    const char* newName;
public:
    explicit chpromptCommand(const char* _newName) : BuiltInCommand(nullptr)
                        , newName(_newName) {};
    void execute() override;
};

class showpidCommand : public BuiltInCommand{

public:
    showpidCommand():BuiltInCommand(nullptr){};

    void execute() override{
        std::cout << SmallShell::getInstance().getName() << " pid is "
                        << SmallShell::getInstance().getpid();
    };
};

class pwdCommand : public BuiltInCommand{
public:
    pwdCommand() : BuiltInCommand(nullptr){};
    void execute() override ;
};

class cdCommand : public BuiltInCommand{
const char* newDir;
public:
    explicit cdCommand(const char* dir) : BuiltInCommand(nullptr), newDir(dir){};
    void execute() override ;
};

class jobsCommand : public BuiltInCommand{

public:
    jobsCommand() : BuiltInCommand(nullptr){};
    void execute() override ;
};

class killCommand : public BuiltInCommand{
    int sigNum;
    int jobId;

public:
    killCommand(const char* _signum, const char* _jobId):sigNum(0),jobId(0)
    ,BuiltInCommand(nullptr){
        sscanf(_signum,"%d", &sigNum);
        sscanf(_jobId,"%d", &jobId);

    };
    void execute() override ;//TODO
};

class fgCommand : public BuiltInCommand{
    int jobId;

public:
    fgCommand(const char* _jobId) : jobId(0), BuiltInCommand(nullptr){
        sscanf(_jobId, "%d", &jobId);
    };
    void execute() override ;
};

class bgCommand : public BuiltInCommand{
public:
    bgCommand() : BuiltInCommand(nullptr){};
    void execute() override ;
};

class quitCommand : public BuiltInCommand{
public:
    quitCommand(): BuiltInCommand(nullptr){};
    void execute() override ;
};

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


// TODO: should it really inhirit from BuiltInCommand ?
class CopyCommand : public BuiltInCommand {
 public:
  CopyCommand(const char* cmd_line);
  virtual ~CopyCommand() {}
  void execute() override;
};

// TODO: add more classes if needed 
// maybe chprompt , timeout ?



#endif //SMASH_COMMAND_H_
