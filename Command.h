#ifndef HW1_COMMAND_H
#define HW1_COMMAND_H

#include <unistd.h>
#include "CommandExceptions.h"
#include <string.h>


#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

typedef enum {builtIn, external} cmdType;
typedef enum{stdChannel,fileChannel,errChannel}outputChannel;
class Command{
    string fixedCommand;
    string originalCommand;
    string splitCmd[COMMAND_MAX_ARGS];
    cmdType type;
    int sizeOfSplit;
    outputChannel out;
    string path;

public:
    Command(string &_fixedCommand, string &_originalCommand
            , string *_splitCmd, cmdType _type, int _sizeOfSplit,
            outputChannel _out, string &_path)
            : fixedCommand(_fixedCommand), originalCommand(_originalCommand),
             type(_type), sizeOfSplit(_sizeOfSplit), out(_out),
                                                     path(_path) {
        for (int i = 0; i < _sizeOfSplit ; ++i) {
            splitCmd[i].copy(_splitCmd[i]);
        }
    }


};


#endif //HW1_COMMAND_H
