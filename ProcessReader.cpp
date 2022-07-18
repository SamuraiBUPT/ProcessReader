#include "ProcessReader.h"

reader::Reader::Reader(std::string_view name, std::string_view target) {
	if (name.empty())throw std::exception("������δ����");

	//�������н��� Ҳ�������������-��ϸ��Ϣ  �������������
	//���Ȼ�ȡ��ǰ���̿���
	HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pro;									//pro��������¼���̵�ַ�ģ�Ҳ����һ���ݴ�
	pro.dwSize = sizeof pro;

	//���������б�
	for (Process32First(_handle, &pro); Process32Next(_handle, &pro);) {
		if (name.compare(pro.szExeFile) == 0) {
			pid = pro.th32ProcessID;
			target_Process_Recorder = OpenProcess(PROCESS_ALL_ACCESS, false, pid);//�õ�Ŀ����̵ľ��
			//OpenProcess()�����Ǵ򿪽��̵���˼����һ��������ϣ���õ���Ȩ��
			break;
		}
	}
	if (pid == 0)throw std::exception("δ�ҵ��ý��̣�");


	_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	MODULEENTRY32 mod;
	mod.dwSize = sizeof mod;
	for (Module32First(_handle, &mod); Module32Next(_handle, &mod);) {
		if (target.compare(mod.szModule) == 0) {
			target_address_Recorder = (uintptr_t)mod.modBaseAddr;
			break;
		}
	}
	if (target_address_Recorder == 0x0)throw std::exception("δ�ҵ���ʼ��̬��ַ");

}
