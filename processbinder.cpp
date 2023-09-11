#include "processbinder.h"

#include <utility>
#include "utils.h"

void ProcessBinder::bind_process(std::string process_name){
    logManager.log("Target process: " + process_name, logManager.INFO);

    // get process snapshot
    HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 *process_iter = &this->process_entry;
    process_iter->dwSize = sizeof(PROCESSENTRY32);

    for(Process32First(_handle, process_iter); Process32Next(_handle, process_iter);){
        // ignore the warning by VSCode IDE, there seems to be something wrong with the old definition
        // szExeFile is actually CHAR[260], not wchar
        if(process_name == wchar2string(process_iter->szExeFile)){
            pid = process_iter->th32ProcessID;
            pHandler = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
            break;
        }
    }
    if(pid == 0){
        logManager.log("Process not found", logManager.ERR);
        throw std::runtime_error("Process not found");
    }
}

void ProcessBinder::bind_module(std::string module_name){
    HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    MODULEENTRY32* module_iter = &this->module_entry;
    for(Module32First(_handle, module_iter);Module32Next(_handle, module_iter);){
        if(module_name == wchar2string(module_iter->szModule)){
            client_dll = (uintptr_t)module_iter->modBaseAddr;
            break;
        }
    }
    if (client_dll == 0x0){
        logManager.log("Module not found", logManager.ERR);
        throw std::runtime_error("client.dll is NULL");
    }
}

ProcessBinder::ProcessBinder(){
    logManager.log("Null process handle", logManager.ERR);
    throw std::runtime_error("Process name is empty");
}

ProcessBinder::ProcessBinder(std::string process_name)
{
    this->bind_process(std::move(process_name));
    this->bind_module("client.dll");
}

ProcessBinder::~ProcessBinder()
{
    pHandler = nullptr;
    pid = 0;
    client_dll = 0x0;
}

uint32_t ProcessBinder::get_client_dll(){
    return this->client_dll;
}

HANDLE ProcessBinder::get_handler(){
    return this->pHandler;
}