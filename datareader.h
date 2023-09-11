#include <cstdint>
#include <vector>
#include <windows.h>

template <typename T>
T readData(HANDLE handler, uint32_t address);   // return the data at the address

template <typename T>
void readDataBlock(HANDLE handler, 
                uint32_t address,           // __In__
                uint16_t count,             // __In__
                std::vector<T>* out);       // __Out__

template <typename T>
void readDataByInterval(HANDLE handler, 
                        uint32_t address,       // __In__
                        uint16_t count,         // __In__
                        uint16_t interval,      // __In__
                        std::vector<T>* out);   // __Out__