#include "datareader.h"

template <typename T>
T readData(HANDLE handler, uint32_t address){
    T t;
    ReadProcessMemory(handler, (LPVOID)address, &t, sizeof(T), nullptr);
    return t;
}

template <typename T>
void readDataBlock(HANDLE handler, uint32_t address, uint16_t count, std::vector<T>* out){
    // this function read contiguous data block in memory.
    T* t;
    ReadProcessMemory(handler, (LPVOID)address, t, count*(sizeof(T)), nullptr);
    for(int i = 0; i < count; i++){
        out->push_back(t[i]);
    }
}

template <typename T>
void readDataByInterval(HANDLE handler, uint32_t address, uint16_t count, uint16_t interval, std::vector<T>* out){
    // this function is for reading data in a fixed interval
    // for example, player info struct in csgo.
    for(int i = 0; i < count; i++){
        T t;
        ReadProcessMemory(handler, (LPVOID)(address + i*interval), &t, sizeof(T), nullptr);
        out->push_back(t);
    }
}

// instantiate the template
template uint32_t readData<uint32_t>(HANDLE handler, uint32_t address);
template uint8_t readData<uint8_t>(HANDLE handler, uint32_t address);
template void readDataBlock<uint32_t>(HANDLE handler, uint32_t address, uint16_t count, std::vector<uint32_t>* out);

template void readDataByInterval<uint32_t>(HANDLE handler, uint32_t address, uint16_t count, uint16_t interval, std::vector<uint32_t>* out);
template void readDataByInterval<float>(HANDLE handler, uint32_t address, uint16_t count, uint16_t interval, std::vector<float>* out);
