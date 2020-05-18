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
	cout << "\tИнформация о системе:";
	cout << endl << "Размер страницы: " << temp.dwPageSize;
	cout << endl << "Самый низкий адрес памяти: " << temp.lpMinimumApplicationAddress;
	cout << endl << "Самый высокий адрес памяти: " << temp.lpMaximumApplicationAddress;
	cout << endl << "Участник зарезервирован для будущего использования: " << temp.wReserved;
	cout << endl << "Маска, представляющая набор процессоров, настроенных в системе: " << temp.dwActiveProcessorMask;
	cout << endl << "Количество логических процессоров в текущей группе: " << temp.dwNumberOfProcessors;
	cout << endl << "Количество логических процессоров в текущей группе: " << temp.dwNumberOfProcessors;
	cout << endl << "Устаревший элемент, который сохраняется для совместимости: " << temp.dwProcessorType;
	cout << endl << "Детализация начального адреса, по которому может быть выделена виртуальная память: " << temp.dwAllocationGranularity;
	cout << endl << "Архитектурно-зависимый уровень процессора: " << temp.wProcessorLevel;
	cout << endl << "Архитектурно-зависимая версия процессора: " << temp.wProcessorRevision;
	cout << endl << "Устаревший элемент, который сохраняется для совместимости: " << temp.dwOemId;

	MEMORYSTATUS mem;
	GlobalMemoryStatus(&mem);
	cout << endl << "Максимальный объем памяти, который может быть зафиксирован текущим процессом, в байтах: " << mem.dwAvailPageFile;
	cout << endl << "Объем физической памяти, доступный в настоящее время, в байтах: " << mem.dwAvailPhys;
	cout << endl << "Объем незарезервированной и незафиксированной памяти, в настоящее время находящейся в пользовательской части виртуального адресного пространства вызывающего процесса, в байтах: " << mem.dwAvailVirtual;
	cout << endl << "Размер MEMORYSTATUS структура данных, в байтах: " << mem.dwLength;
	cout << endl << "Число от 0 до 100, указывающее приблизительный процент используемой физической памяти: " << mem.dwMemoryLoad;
	cout << endl << "Текущий Размер фиксированного лимита памяти, в байтах: " << mem.dwTotalPageFile;
	cout << endl << "Объем фактической физической памяти в байтах: " << mem.dwTotalPhys;
	cout << endl << "Размер пользовательской части виртуального адресного пространства вызывающего процесса в байтах: " << mem.dwTotalVirtual;
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
	_tprintf(_T("Ошибка номер: %d. %s"), dword, lpvoid);
	LocalFree(lpvoid);
	ExitProcess(dword);
}