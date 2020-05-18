#include <iostream>
#include <cstring>
#include <Windows.h>
#include <tchar.h>

using namespace std;
void SoutInfo();
void SoutError();

int main(int argc, char * argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if (argc != 2) {
		cout << "Error!" << endl;
		return 0;
	}

	if (strcmp(argv[1], "-e") == 0) {
		LPTSTR pszBuf = (LPTSTR)LocalAlloc(LPTR, -1);
		if (pszBuf == NULL)
			SoutError();
	}
	else if (strcmp(argv[1], "-i") == 0)
		SoutInfo();
	else
		cout << "Error input data!" << endl;

	return 0;
}

void SoutInfo() {
	SYSTEM_INFO temp;
	GetSystemInfo(&temp);
	cout << "\t���������� � �������:";
	cout << endl << "������ ��������: " << temp.dwPageSize;
	cout << endl << "����� ������ ����� ������: " << temp.lpMinimumApplicationAddress;
	cout << endl << "����� ������� ����� ������: " << temp.lpMaximumApplicationAddress;
	cout << endl << "�������� �������������� ��� �������� �������������: " << temp.wReserved;
	cout << endl << "�����, �������������� ����� �����������, ����������� � �������: " << temp.dwActiveProcessorMask;
	cout << endl << "���������� ���������� ����������� � ������� ������: " << temp.dwNumberOfProcessors;
	cout << endl << "���������� ���������� ����������� � ������� ������: " << temp.dwNumberOfProcessors;
	cout << endl << "���������� �������, ������� ����������� ��� �������������: " << temp.dwProcessorType;
	cout << endl << "����������� ���������� ������, �� �������� ����� ���� �������� ����������� ������: " << temp.dwAllocationGranularity;
	cout << endl << "������������-��������� ������� ����������: " << temp.wProcessorLevel;
	cout << endl << "������������-��������� ������ ����������: " << temp.wProcessorRevision;
	cout << endl << "���������� �������, ������� ����������� ��� �������������: " << temp.dwOemId;

	MEMORYSTATUS mem;
	GlobalMemoryStatus(&mem);
	cout << endl << "������������ ����� ������, ������� ����� ���� ������������ ������� ���������, � ������: " << mem.dwAvailPageFile;
	cout << endl << "����� ���������� ������, ��������� � ��������� �����, � ������: " << mem.dwAvailPhys;
	cout << endl << "����� ������������������� � ����������������� ������, � ��������� ����� ����������� � ���������������� ����� ������������ ��������� ������������ ����������� ��������, � ������: " << mem.dwAvailVirtual;
	cout << endl << "������ MEMORYSTATUS ��������� ������, � ������: " << mem.dwLength;
	cout << endl << "����� �� 0 �� 100, ����������� ��������������� ������� ������������ ���������� ������: " << mem.dwMemoryLoad;
	cout << endl << "������� ������ �������������� ������ ������, � ������: " << mem.dwTotalPageFile;
	cout << endl << "����� ����������� ���������� ������ � ������: " << mem.dwTotalPhys;
	cout << endl << "������ ���������������� ����� ������������ ��������� ������������ ����������� �������� � ������: " << mem.dwTotalVirtual;
}

void SoutError() {
	LPVOID lpvoid;
	DWORD dword = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		dword, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpvoid, 
		0, 
		NULL
	);
	_tprintf(_T("������ �����: %d. %s"), dword, lpvoid);
	LocalFree(lpvoid);
	ExitProcess(dword);
}