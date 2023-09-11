#pragma once
#include <stdio.h>
#include <string>

struct LogManager{
    char* INFO = "[INFO]";
    char* WARN = "[WARN]";
    char* ERR = "[ERROR]";
    void log(char* msg, char* level){
        printf("%s %s\n", level, msg);
    }
    void log(std::string msg, char* level){
        printf("%s %s\n", level, msg.c_str());
    }
};

extern LogManager logManager;

std::string wchar2string(char *wchar);
