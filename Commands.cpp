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

#define MAX_ARGS 20

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

SmallShell::SmallShell() : jobs(), defaultName("smash"), name(defaultName) {
// TODO: add your implementation
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}



/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit"
    };

    char** args = new char*[MAX_ARGS];
    int args_num = _parseCommandLine(cmd_line, args);
    string command = string(args[0]);

    if (command == commands[0]){
        return new chpromptCommand(args[1]);
    }
    if (command == commands[1]){
        return new showpidCommand();
    }
    if (command == commands[2]){
        return new pwdCommand();
    }
    if (command == commands[3]){
        if (args_num > 2)
            throw new Command::tooManyArgs("cd");
        return new cdCommand(args[1]);
    }
    if (command == commands[4]){
        return new jobsCommand();
    }
    if (command == commands[5]){
        if (args_num != 3)
            throw Command::invalidArgs("kill");
        return new jobsCommand(args[1], args[2]);
    }
    if (command == commands[6]){
        if(args_num != 2)
            throw Command::invalidArgs("fg");
        return new fgCommand(args[1]);
    }
    if (command == commands[7]){
        if(args_num > 2)
            throw Command::invalidArgs("bg");
        return new bgCommand(args[1]);
    }
    if (command == commands[8]){
        return new quitCommand(args[1]);
    }

    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    // for example:
    Command* cmd = CreateCommand(cmd_line);
    cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

const std::string SmallShell::getName() const {
    return name;
}

void SmallShell::setName(const char* s) {
    if (!s)
        name = defaultName;
    else
        name = string(s);
}

void chpromptCommand::execute() {
    SmallShell::getInstance().setName(newName);
}
