#include "ProcessReader.h"

reader::Reader::Reader(std::string_view name, std::string_view target) {
	if (name.empty())throw std::exception("进程名未传入");

	//遍历所有进程 也就是任务管理器-详细信息  里面的所有内容
	//首先获取当前进程快照
	HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pro;									//pro是用来记录进程地址的，也就是一个暂存
	pro.dwSize = sizeof pro;

	//遍历进程列表
	for (Process32First(_handle, &pro); Process32Next(_handle, &pro);) {
		if (name.compare(pro.szExeFile) == 0) {
			pid = pro.th32ProcessID;
			target_Process_Recorder = OpenProcess(PROCESS_ALL_ACCESS, false, pid);//得到目标进程的句柄
			//OpenProcess()函数是打开进程的意思，第一个参数是希望得到的权限
			break;
		}
	}
	if (pid == 0)throw std::exception("未找到该进程！");


	_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	MODULEENTRY32 mod;
	mod.dwSize = sizeof mod;
	for (Module32First(_handle, &mod); Module32Next(_handle, &mod);) {
		if (target.compare(mod.szModule) == 0) {
			target_address_Recorder = (uintptr_t)mod.modBaseAddr;
			break;
		}
	}
	if (target_address_Recorder == 0x0)throw std::exception("未找到起始静态地址");

}
