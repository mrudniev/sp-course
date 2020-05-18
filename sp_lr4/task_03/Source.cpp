#define SIZEARRAY 10000000
#include <Windows.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

DWORD arr[SIZEARRAY], sum = 0, sr = 0, min = 0, max = 0;
CRITICAL_SECTION csArray;
DWORD type = 0;

VOID InitArray()
{
	srand(time(NULL));
	for (int i = 0; i < SIZEARRAY; i++)
	{
		arr[i] = 1 + rand() % 1000;
	}
}

VOID ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(1);
}

DWORD WINAPI ThreadFuncSum(VOID)
{
	if (type)
		EnterCriticalSection(&csArray);
	for (int val : arr)
	{
		sum += val;
	}
	if (type)
		LeaveCriticalSection(&csArray);

	return 0;
}

DWORD WINAPI ThreadFuncSr(VOID)
{
	DWORD s = 0;
	if (type)
		EnterCriticalSection(&csArray);
	for (int val : arr)
	{
		s += val;
	}
	if (type)
		LeaveCriticalSection(&csArray);
	sr = s / SIZEARRAY;

	return 0;
}

DWORD WINAPI ThreadFuncMinMax(VOID)
{
	min = arr[0];
	max = arr[0];
	if (type)
		EnterCriticalSection(&csArray);
	for (int val : arr)
	{
		if (min > val) min = val;
		if (max < val) max = val;
	}
	if (type)
		LeaveCriticalSection(&csArray);

	return 0;
}

int main()
{
	InitArray();
	scanf_s("%d", &type);
	clock_t start = clock();

	if (type)
		InitializeCriticalSection(&csArray);

	HANDLE hThreadSum, hTreadSr, hTreadMinMax;
	DWORD IDThreadSum, IDTreadSr, IDTreadMinMax;

	hThreadSum = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFuncSum, NULL, 0, &IDThreadSum);             
	if (hThreadSum == NULL)
		ErrorExit("CreateThread error\n");

	hTreadSr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFuncSr, NULL, 0, &IDTreadSr);
	if (hTreadSr == NULL)
		ErrorExit("CreateThread error\n");
	
	hTreadMinMax = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFuncMinMax, NULL, 0, &IDTreadMinMax);
	if (hTreadMinMax == NULL)
		ErrorExit("CreateThread error\n");

	WaitForSingleObject(hThreadSum, INFINITE);
	WaitForSingleObject(hTreadSr, INFINITE);
	WaitForSingleObject(hTreadMinMax, INFINITE);

	CloseHandle(hThreadSum);
	CloseHandle(hTreadSr);
	CloseHandle(hTreadMinMax);

	if (type)
		DeleteCriticalSection(&csArray);

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("1. Sum = %d\n2. Sr = %d\n3. Min = %d | Max = %d\nThe time: %f seconds\n", sum, sr, min, max, seconds);
	system("pause");

	return 0;
}