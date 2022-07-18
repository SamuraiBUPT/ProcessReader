#pragma once
/*		ǰ�����ã���C++��׼����ΪC++17
		���ַ�������Ϊ���ֽ�				*/
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

namespace reader {

	std::vector<uintptr_t> address_Recorder(64);			//��Ϊvector����Ҳ��һ���࣬Ӧ�÷��ڹ��캯������ȥʵ�֣����߷�������
	class Reader {
	public:
		int pid;										//���������м�¼ProcessID�ģ���������������
		HANDLE target_Process_Recorder;					//������¼���̵ľ��
		uintptr_t target_address_Recorder = 0x0;						//������¼���̵ľ�̬��ַ����ʼ��ַ�����Ǵ����￪ʼƫ�ƣ�


		Reader(std::string_view name,std::string_view target);						//���캯��  string_view��std���棬����Ҫ����



		//����Read��װ����Բ�ͬread���ͽ��з�װ
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
