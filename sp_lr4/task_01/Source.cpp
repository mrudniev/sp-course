#include <Windows.h>
#include <stdio.h>

int main()
{
	HANDLE mut;
	mut = CreateMutex(NULL, FALSE, "VSushynskyi");
	DWORD result, i;
	result = WaitForSingleObject(mut, 0);
	if (result == WAIT_OBJECT_0)
	{
		printf("Is Running...");
		scanf_s("%d", &i);
		ReleaseMutex(mut);
	}
	else
	{
		printf("Fail...");
		system("pause");
	}
		
	CloseHandle(mut);

	return 0;
}