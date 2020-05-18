#define BUFFSIZE 4096
#include <windows.h>
#include <stdio.h>
#include <string.h>

void printError();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	DWORD n = 0;
	WCHAR szCmdline[1024] = L"C:\\Program Files\\7-Zip\\7z.exe -y", path1[MAX_PATH], path2[MAX_PATH];

	printf("\t ����\n1. ���������� ������ � �����\n2. �������� ������ ����� � ����� �����\n");
	printf("������� �����: ");
	scanf_s("%d", &n);
	switch (n)
	{
		case 1:
			printf("������� ������������ ������: ");
			wscanf_s(L"%ls", path1, (unsigned)_countof(path1));
			printf("������� ������������ ���� �����������: ");
			wscanf_s(L"%ls", path2, (unsigned)_countof(path2));
			wcscat(szCmdline, L" x ");
			wcscat(szCmdline, path1);
			wcscat(szCmdline, L" -o");
			wcscat(szCmdline, path2);
			break;
		case 2:
			printf("������� ������������ �����: ");
			wscanf_s(L"%ls", path2, (unsigned)_countof(path2));
			printf("������� ������������ ������: ");
			wscanf_s(L"%ls", path1, (unsigned)_countof(path1));
			wcscat(szCmdline, L" a -tzip -ssw -mx9 ");
			wcscat(szCmdline, path1);
			wcscat(szCmdline, L" ");
			wcscat(szCmdline, path2);
			break;
		default:
			printf("�������� �����");
			break;
	}

	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	HANDLE ReadPipeHandle;
	HANDLE WritePipeHandle;      
	
	if (!CreatePipe(&ReadPipeHandle, &WritePipeHandle, &sa, 0))
		printError();
		
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdOutput = WritePipeHandle;
	si.hStdError = WritePipeHandle;
	si.dwFlags = STARTF_USESTDHANDLES;
	
	if (!CreateProcess(NULL, szCmdline,
		NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		printError();
	}

	// ������ �� pipe
	BOOL result = TRUE;
	CHAR Data[BUFFSIZE];
	for (;;)
	{
		DWORD BytesRead;
		DWORD TotalBytes;
		DWORD BytesLeft;

		// ��������� ����������� ���������� ���� � ������
		if (!PeekNamedPipe(ReadPipeHandle, Data, sizeof(Data), &BytesRead,
			&TotalBytes, &BytesLeft))printf("PeekNamedPipe");

		// ��������� ������
		if (BytesRead)
		{
			if (!ReadFile(ReadPipeHandle, Data, sizeof(Data) - 1, &BytesRead, NULL))
				printf("ReadFile");
			Data[BytesRead] = '\0';
			if (strstr(Data, "ERROR") != NULL)
			{
				result = FALSE;
				break;
			}
		}
		else
		{
			// ������� �� ����������
			if (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0)
				break;
		}
	}

	if (result)
		printf("�������� ����������� �������.");
	else
		printf("�������� ����������� � �������.");

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(ReadPipeHandle);
	CloseHandle(WritePipeHandle);

	return 0;
}

void printError()
{
	LPVOID lpMsgBuf;
	DWORD dwErrCode = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	printf("Failed with error %d. %s", dwErrCode, lpMsgBuf);

	LocalFree(lpMsgBuf);
	ExitProcess(dwErrCode);
}