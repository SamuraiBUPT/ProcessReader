#include <vector>
#include <string>
#include <cstdint>
#include <windows.h>
#include <TlHelp32.h>
#include <stdexcept>
#include <vector>

#ifndef TH32CS_SNAPMODULE32
#define TH32CS_SNAPMODULE32 0x00000010
#endif


class ProcessBinder{
    // A reader for process memory
public:
    ProcessBinder();
    ProcessBinder(std::string process_name);
    ~ProcessBinder();

    void bind_process(std::string process_name);
    void bind_module(std::string module_name);

    uint32_t get_client_dll();
    HANDLE get_handler();


private:
    HANDLE pHandler = nullptr;
    uint16_t pid;
    uintptr_t client_dll = 0x0;
    PROCESSENTRY32 process_entry;
    MODULEENTRY32 module_entry;

};