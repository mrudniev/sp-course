#include <stdio.h>
#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>

void AnsiToUnicode(LPVOID);
void UnicodeToAnsi(LPVOID);

using namespace std;
const int NBUFF = 1000;
int main(int argc, char * argv[])
{
	setlocale(0, "rus");
	
	if (argc > 3 || argc < 3)
	{
		printf("Error! The number of arguments must be 3!");
		return 0;
	}

	HANDLE rFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (rFile == INVALID_HANDLE_VALUE)
	{
		printf("Error! Error create file. Program terminate.");
		return 0;
	}

	LPVOID buffer = malloc(NBUFF);
	memset(buffer, 0, NBUFF);
	DWORD readBytes;

	if (0 == ReadFile(rFile, buffer, NBUFF, &readBytes, NULL)) {
		printf("Error read file. Program terminate.");
		return 0;
	}
	CloseHandle(rFile);

	if (strcmp(argv[2], "-a") == 0)
		AnsiToUnicode(buffer);
	else if (strcmp(argv[2], "-u") == 0)
		UnicodeToAnsi(buffer);
	else
		printf("Error. Unknown input parametr. Program terminate.");

	return 0;
}

void AnsiToUnicode(LPVOID buffer) {
	int outBytes;
	HANDLE wFile = CreateFile("out_unicode.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WCHAR *outBuffer = new WCHAR[NBUFF];
	outBytes = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buffer, -1, outBuffer, NBUFF) * 2;
	WriteFile(wFile, outBuffer, outBytes, NULL, NULL);
	LocalFree(outBuffer);
	FlushFileBuffers(wFile);
	CloseHandle(wFile);
}

void UnicodeToAnsi(LPVOID buffer) {
	int outBytes;
	HANDLE wFile = CreateFile("out_ansi.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CHAR *outBuffer = new CHAR[NBUFF];
	outBytes = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)buffer, -1, outBuffer, NBUFF, NULL, NULL) - 1;
	WriteFile(wFile, outBuffer, outBytes, NULL, NULL);
	LocalFree(outBuffer);
	FlushFileBuffers(wFile);
	CloseHandle(wFile);
}
