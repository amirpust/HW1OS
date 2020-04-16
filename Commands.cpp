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
  return i - 1;

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

void redirectionAUX(char* cmd_line, bool* bg,char* cmd_args){
    _getRedirection(cmd_line,cmd_args);//cmd_args might be null we should handel it
    _removeRedirection(cmd_line);
    if(!*bg){
        *bg = _isBackgroundComamnd(cmd_line);
        _removeBackgroundSign(cmd_line);
    }
}

void _removeRedirection(char* cmd_line){
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_first_of('>');
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // replace the '>' (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = '\0';
}

void _getRedirection(char* cmd_line,char* direction){
    char* args[COMMAND_ARGS_MAX_LENGTH];
    _parseCommandLine(cmd_line,args);
    int i = 0;
    while(args[i] != nullptr){
        if(!strcmp(args[i],">") || !strcmp(args[i], ">>")){
            //TODO : free allocated memory;
            if(args[i + 1] == nullptr)
                return;
            strcpy(direction,args[i + 1]);
        }
        i++;
    }
}

redirectionType identifyRedirection(char* cmd_line, bool *bg, char* cmd_args){
    *bg = _isBackgroundComamnd(cmd_line);
    _removeBackgroundSign(cmd_line);
    const string str(cmd_line);
    if(str.find(">>") != string::npos){
        redirectionAUX(cmd_line,bg,cmd_args);
        return append;
    }else if(str.find('>') != string::npos){
        redirectionAUX(cmd_line,bg,cmd_args);
        return override;
    }
    return noRedirect;
}

pipeType identifyPipe(char* cmd_line,char* left, char* right){
    const string str(cmd_line);
    int indexOr = str.find_first_of('|');
    int indexAnd = str.find_first_of('&');
    if (indexAnd != string::npos && indexOr != string::npos && indexOr == indexAnd - 1){
        cmd_line[indexOr] = '\0';
        strcpy(left,cmd_line);
        strcpy(right,cmd_line + indexAnd + 1);//check how to make it better;
        return pipeStderr;
    }else if(indexOr != string::npos){
        cmd_line[indexOr] = '\0';
        strcpy(left,cmd_line);
        strcpy(right,cmd_line + indexOr + 1);//check how to make it better;
        return pipeRegular;
    }
    return noPipe;
}

void prepare(char* path,redirectionType rd){
    close(1);
    if(rd == override){
        open(path,O_CREAT | O_RDWR, S_IRWXU | S_IRGRP | S_IROTH);
    }else if (rd == append){
        open(path, O_APPEND | O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH ); //TODO : check if works
    }
}

void cleanUp(int fd){
    if (fd == -1){
        return;
    }
    close(1);
    dup(fd);
    close(fd);
}
chpromptCommand::chpromptCommand(const char *cmd_line, const char* og_line)
: BuiltInCommand(cmd_line,og_line){}

void chpromptCommand::execute() {
    SmallShell::getInstance().setName(args[1]);
}

Command::Command(const char *cmd_line,const char *original_line ,cmdType _type)
: cmd_line(cmd_line),type(_type),original_cmd(original_line) {

    size = _parseCommandLine(cmd_line, args);
}

const char *Command::print() const {
    return original_cmd;
}

const cmdType Command::getType() const {
    return type;
}

showpidCommand::showpidCommand(const char *cmd_line, const char* og_line)
:BuiltInCommand(cmd_line,og_line){}

void showpidCommand::execute() {
    std::cout <<  "smash pid is "<< SmallShell::getInstance().getPid()  << endl;
}

void pwdCommand::execute() {
    cout << get_current_dir_name() << endl;
}

cdCommand::cdCommand(const char *cmd_line, const char* og_line)
: BuiltInCommand(cmd_line,og_line ){}

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

jobsCommand::jobsCommand(const char *cmd_line, const char* og_line)
: BuiltInCommand(cmd_line,og_line){}

void jobsCommand::execute() {
    try {
        SmallShell::getInstance().getJobs().printJobsList();
    }catch(JobsList::emptyList& e){
        throw emptyList(args[0]); //TODO: check the answer
    }

}

killCommand::killCommand(const char *cmd_line, const char* og_line)
: BuiltInCommand(cmd_line, og_line),sigNum(0), jobId(0){
    if(args[1] == nullptr || args[2] == nullptr || args[3] != nullptr)
        throw invalidArgs(args[0]);

    sscanf(args[1], "%d", &sigNum);
    sigNum *= -1;
    sscanf(args[2],"%d", &jobId);

}

void killCommand::execute() {
    try {
        SmallShell::getInstance().getJobs().sendSigById(sigNum,jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(args[0],jobId);
    }
}

fgCommand::fgCommand(const char *cmd_line, const char* og_line)
: jobId(0), BuiltInCommand(cmd_line,og_line){
    if(size != 2){
        throw invalidArgs(args[0]);
    }
    sscanf(args[1],"%d", &jobId);
}

void fgCommand::execute() {
    try {
        SmallShell::getInstance().getJobs().bringFG(jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(args[0], jobId);
    }catch(JobsList::emptyList& e){
        throw emptyList(args[0]);
    }
}

void bgCommand::execute() {
    try {

        SmallShell::getInstance().getJobs().resumeOnBG(jobId);
    }catch (JobsList::notExist& e){
        throw jobDoesntExist(args[0],jobId);
        //TODO : check what exception is being thrown empty or not exist
    }catch (JobsList::inBG& e){
        throw jobAlreadyBGRuning(args[0],jobId);
    }catch(JobsList::emptyList& e){
        throw emptyListBG(args[0]);
    }

}

bgCommand::bgCommand(const char *cmd_line, const char* og_line) :
BuiltInCommand(cmd_line,og_line){
    if(size > 2)
        throw invalidArgs(args[0]);
    if(size == 1)
        jobId = 0;
    else
        sscanf(args[1], "%d", &jobId);

}

void quitCommand::execute() {
    if(size > 1 && !strcmp(args[1], "kill")){
        cout << "smash: sending SIGKILL signal to " +
        std::to_string(SmallShell::getInstance().getJobs().getSize()) + " jobs:" << endl;
        SmallShell::getInstance().getJobs().killAllJobs();
    }
        kill(SmallShell::getInstance().getPid(), SIGKILL);
}

quitCommand::quitCommand(const char *cmd_line, const char* og_line)
: BuiltInCommand(cmd_line,og_line){}
