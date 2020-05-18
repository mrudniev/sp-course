#include <Windows.h>
#include <stdio.h>

VOID ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(1);
}

DWORD WINAPI ThreadFunc(VOID)
{
	//Sleep(3000);
	printf("thead = %d", GetCurrentThreadId());
	HANDLE wFile = CreateFile("data.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (wFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile.");
		return 0;
	}
	
	CHAR outBuffer[200], time[16];
	DWORD bytes;
	SYSTEMTIME st;
	GetLocalTime(&st);

	bytes = sprintf_s(time, 16, "%02d:%02d:%02d:%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	bytes += sprintf_s(outBuffer, 200, "thead = %d | time = %s\n", GetCurrentThreadId(), time);
	
	WriteFile(wFile, outBuffer, bytes, NULL, NULL);
	//FlushFileBuffers(wFile);
	CloseHandle(wFile);
	
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		ErrorExit("Count arg != 2");
	}

	DWORD theadCount = 0;
	sscanf_s(argv[1], "%d", &theadCount);
	DWORD IDThread;
	HANDLE *hThread = new HANDLE[theadCount];

	for (DWORD i = 0; i < theadCount; i++)
	{
		hThread[i] = CreateThread(NULL, // default security attributes 
			0,                           // use default stack size 
			(LPTHREAD_START_ROUTINE)ThreadFunc, // thread function 
			NULL,                    // no thread function argument 
			0,                       // use default creation flags 
			&IDThread);              // returns thread identifier 

	  // Проверьте возвращаемое значение для успеха.
		if (hThread[i] == NULL)
			ErrorExit("CreateThread error\n");
	}

	WaitForMultipleObjects(theadCount, hThread, TRUE, INFINITE);

	return 0;
}