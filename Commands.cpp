#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
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

Command::Command(const char *cmd_line) : cmd_line(cmd_line) {
    args = new char*[COMMAND_MAX_ARGS];
    int args_num = _parseCommandLine(cmd_line, args);
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
              << getpid();
}

void pwdCommand::execute() {
    char* buffer = new char[4096] ;
    cout << getcwd(buffer, 4096); //TODO: check if there is better solution
    delete[] buffer;
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
    SmallShell::getInstance().printJobs();
}

