#define COUNT_ARRAY 10
#include <stdio.h> 
#include <windows.h> 

DWORD dwTlsIndex[COUNT_ARRAY];
DWORD dwTlsGCD;
DWORD dwTlsSum;

VOID ErrorExit(LPCSTR);

DWORD gcd(DWORD a, DWORD b) {
	return (a == 0) ? b : gcd(b % a, a);
}

VOID findGCD(VOID)
{
	LPVOID lpvData = TlsGetValue(dwTlsIndex[0]);
	DWORD res = (DWORD)lpvData;

	for (int i = 1; i < COUNT_ARRAY; i++)
	{
		lpvData = TlsGetValue(dwTlsIndex[i]);
		if ((lpvData == 0) && (GetLastError() != ERROR_SUCCESS))
			ErrorExit("TlsGetValue error");

		res = gcd(res, (DWORD)lpvData);
	}

	if (!TlsSetValue(dwTlsGCD, (LPVOID)res))
		ErrorExit("TlsSetValue error");
}

VOID findSum(VOID)
{
	LPVOID lpvData;
	DWORD sum = 0;

	for (int i = 0; i < COUNT_ARRAY; i++)
	{
		lpvData = TlsGetValue(dwTlsIndex[i]);
		if ((lpvData == 0) && (GetLastError() != ERROR_SUCCESS))
			ErrorExit("TlsGetValue error");

		sum += (DWORD)lpvData;
	}

	if (!TlsSetValue(dwTlsSum, (LPVOID)sum))
		ErrorExit("TlsSetValue error");
}

DWORD WINAPI ThreadFunc(VOID)
{
	LPVOID lpvData;

	if (!TlsSetValue(dwTlsGCD, (LPVOID)1))
		ErrorExit("TlsSetValue error");

	srand(GetCurrentThreadId());
	for (int i = 0; i < COUNT_ARRAY; i++)
	{
		lpvData = (LPVOID)(10 + rand() % 90);
		if (!TlsSetValue(dwTlsIndex[i], lpvData))
			ErrorExit("TlsSetValue error");

		printf("thread %d: index=%d: lpvData=%d\n", GetCurrentThreadId(), dwTlsIndex[i], lpvData);
	}

	findGCD();
	findSum();

	printf("\n\n--thread=%d: gcd=%d: sum=%d--\n\n", GetCurrentThreadId(), (DWORD)TlsGetValue(dwTlsGCD), (DWORD)TlsGetValue(dwTlsSum));

	// Release the dynamic memory before the thread returns. 

	for (int i = 0; i < COUNT_ARRAY; i++)
	{
		lpvData = TlsGetValue(dwTlsIndex[i]);
		if (lpvData != 0)
			LocalFree((HLOCAL)lpvData);
	}

	lpvData = TlsGetValue(dwTlsGCD);
	if (lpvData != 0)
		LocalFree((HLOCAL)lpvData);

	lpvData = TlsGetValue(dwTlsSum);
	if (lpvData != 0)
		LocalFree((HLOCAL)lpvData);

	return 0;

}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		ErrorExit("Count arg != 2");
	}

	DWORD theadCount = 0;
	sscanf(argv[1], "%d", &theadCount);
	DWORD IDThread;
	HANDLE *hThread = new HANDLE[theadCount];
	int i;

	// Выделите индекс TLS.
	
	for (int i = 0; i < COUNT_ARRAY; i++)
	{
		if ((dwTlsIndex[i] = TlsAlloc()) == TLS_OUT_OF_INDEXES)
			ErrorExit("TlsAlloc failed");
	}
	if ((dwTlsGCD = TlsAlloc()) == TLS_OUT_OF_INDEXES || (dwTlsSum = TlsAlloc()) == TLS_OUT_OF_INDEXES)
		ErrorExit("TlsAlloc failed");

	// Создать несколько потоков.

	for (i = 0; i < theadCount; i++)
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

	for (i = 0; i < theadCount; i++)
		WaitForSingleObject(hThread[i], INFINITE);
	
	for (int i = 0; i < COUNT_ARRAY; i++)
	{
		TlsFree(dwTlsIndex[i]);
	}
	TlsFree(dwTlsGCD);
	TlsFree(dwTlsSum);

	return 0;
}

VOID ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(1);
}
