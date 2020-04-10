#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
using std::vector;
using std::string;

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

class Command {
    const char* cmd_line;
    // TODO: Add your data members
 public:
  Command(const char* cmd_line) : cmd_line(cmd_line) {};
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
public:
    fgCommand() : BuiltInCommand(nullptr){};
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

class JobsList;
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

class JobsList {

public:
    class JobEntry {
        Command* cmd;
        bool isStopped;
        time_t startTime;
        time_t stopTime;
        int jobId;
    };

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs() {};
    ~JobsList() = default;
    void addJob(Command* cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
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

#endif //SMASH_COMMAND_H_
