#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "SmallShell.h"
#include "Commands.h"

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cerr << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cerr << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

#define DEBUG_PRINT cerr << "DEBUG: "

#define EXEC(path, arg) \
  execvp((path), (arg));

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h

chpromptCommand::chpromptCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void chpromptCommand::execute() {
    SmallShell::getInstance().setName(args[1]);
}

Command::Command(const char *cmd_line, cmdType _type) : cmd_line(cmd_line),type(_type) {
    size = _parseCommandLine(cmd_line, args);
}

const char *Command::print() const {
    return args[0];
}

pid_t Command::getCommandPid() const {
    return getpid();
}

showpidCommand::showpidCommand(const char *cmd_line) :BuiltInCommand(cmd_line){}

void showpidCommand::execute() {
    std::cout << SmallShell::getInstance().getName() << " pid is "
              << getpid()  << endl;
}

void pwdCommand::execute() {
    cout << get_current_dir_name() << endl;
}

cdCommand::cdCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void cdCommand::execute() {
    if(size > 2){
        throw tooManyArgs(args[0]);

    }
    if(strcmp(args[1],"-") == 0){
        if (SmallShell::getInstance().dirHistorySize() == 0){
            throw emptyDirHistory(args[0]);
        }
        if(chdir(SmallShell::getInstance().popLastDir()) == -1){
            //TODO: throw <syscall name (cd)> error
        }
        return;
    }

    if(chdir(args[1]) == -1){
        //TODO: throw <syscall name (cd)> error
    }else{
        SmallShell::getInstance().pushDir(args[1]);
    }
}

jobsCommand::jobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void jobsCommand::execute() {
    SmallShell::getInstance().getJobs().printJobsList();
}

killCommand::killCommand(const char *cmd_line) : BuiltInCommand(cmd_line),sigNum(0), jobId(0){
    if(args[1] == nullptr || args[2] == nullptr || args[3] != nullptr)
        throw invalidArgs(args[0]);

    sscanf(args[1], "%d", &sigNum);
    sigNum *= -1;
    sscanf(args[2],"%d", &jobId);

}

void killCommand::execute() {
    try {
        JobsList::JobEntry* job = SmallShell::getInstance().getJobs().getJobById(jobId);
        kill(job->getJobPid(),sigNum);
    }catch(exception& e){
        throw jobDoesntExist(args[0],jobId);
    }
}

fgCommand::fgCommand(const char *cmd_line) : jobId(0), BuiltInCommand(cmd_line){
    if(size < 2 || size > 2){
        throw invalidArgs(args[0]);
    }
    sscanf(args[1],"%d", &jobId);
}

void fgCommand::execute() {
    try {
      JobsList::JobEntry* job = SmallShell::getInstance().getJobs().getJobById(jobId);
      int status;
      waitpid(job->getJobPid(), &status, WUNTRACED | WCONTINUED);
      SmallShell::getInstance().getJobs().removeJobById(jobId);
    }catch(exception& e){
        //TODO : check what exception is being thrown empty or not exist
    }
}

void bgCommand::execute() {
    try {
        JobsList::JobEntry* job = nullptr;
        if(jobId == 1){
            job = SmallShell::getInstance().getJobs().getLastStoppedJob(&jobId);
        }else{
            job = SmallShell::getInstance().getJobs().getJobById(jobId);
            if(!job->isStopped()){
                throw jobAlreadyBGRuning(args[0],jobId);
            }else{
                kill(job->getJobPid(),SIGCONT);
            }
        }
    }catch (exception& e){
        //TODO : check what exception is being thrown empty or not exist
    }

}

void quitCommand::execute() {
    if(size > 1){
        cout << "smash: sending SIGKILL signal to " +
        std::to_string(SmallShell::getInstance().getJobs().getSize()) + " jobs" << endl;
        SmallShell::getInstance().getJobs().killAllJobs();
    }
        kill(getpid(), SIGKILL);
}
