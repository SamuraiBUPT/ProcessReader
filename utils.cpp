#include "utils.h"

LogManager logManager;

extern std::string wchar2string(char *wchar)
{
    std::string str;
    int index = 0;
    while (wchar[index] != '\0')
    {
        str += (char)wchar[index];
        index++;
    }
    return str;
}