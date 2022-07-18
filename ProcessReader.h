#pragma once
/*		前置设置：把C++标准设置为C++17
		把字符集设置为多字节				*/
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

namespace reader {

	std::vector<uintptr_t> address_Recorder(64);			//因为vector本身也是一个类，应该放在构造函数里面去实现，或者放在外面
	class Reader {
	public:
		int pid;										//进程里面有记录ProcessID的，这个就是用来存的
		HANDLE target_Process_Recorder;					//用来记录进程的句柄
		uintptr_t target_address_Recorder = 0x0;						//用来记录进程的静态地址的起始地址（都是从这里开始偏移）


		Reader(std::string_view name,std::string_view target);						//构造函数  string_view在std里面，所以要加上



		//三层Read封装，针对不同read类型进行封装
		template<typename T>
		T read(uintptr_t address) {
			T t;
			ReadProcessMemory(target_Process_Recorder, reinterpret_cast<LPVOID> (address), &t, sizeof T, nullptr);
			return t;
		}

		template<typename T>
		void read_List(uintptr_t address, T* t, SIZE_T count) {
			ReadProcessMemory(target_Process_Recorder, reinterpret_cast<LPVOID> (address), t, count*(sizeof T), nullptr);
		}

		template<typename T>
		void read_chain(uintptr_t address, std::vector<uintptr_t> vec, T &output) {
			for (int i = 0; i < vec.size(); i++) {
				if (i + 1 == vec.size()) {
					output = read<T>(address + vec[i]);
				}
				else {
					address = read<uintptr_t>(address + vec[i]);
				}
			}
		}
	};


}
