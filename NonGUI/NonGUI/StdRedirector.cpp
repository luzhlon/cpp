#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include "StdRedirector.h"

StdRedirector::StdRedirector()
{
	CreatePipe(&hInRead, &hInWrite, 0, 10240);
	CreatePipe(&hOutRead, &hOutWrite, 0, 10240);
	CreatePipe(&hErrRead, &hErrWrite, 0, 10240);

	int fd;
	FILE *fp;
	SetStdHandle(STD_OUTPUT_HANDLE, hOutWrite);//���ñ�׼������
	fd = _open_osfhandle((intptr_t)hOutWrite,
		_O_WRONLY | _O_TEXT);//��ϵͳ���ת��ΪPOSIX�ļ�������low-level I/O
	fp = _fdopen(fd, "w");//��low-level I/Oת��Ϊ��׼C�ļ�
	*stdout = *fp;//�ض���
	setvbuf(stdout, NULL, _IONBF, 0);//�����޻���
	//*stderr = *fp;
	//setvbuf(stderr, NULL, _IONBF, 0);

	SetStdHandle(STD_INPUT_HANDLE, hInRead);
	fd = _open_osfhandle((intptr_t)hInRead, _O_RDONLY | _O_TEXT);
	fp = _fdopen(fd, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	SetStdHandle(STD_ERROR_HANDLE, hErrWrite);
	fd = _open_osfhandle((intptr_t)hErrWrite, _O_WRONLY | _O_TEXT);
	fp = _fdopen(fd, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
}

bool StdRedirector::ReadFromStdOutput(LPVOID Buf, DWORD nToRead, LPDWORD pnRead)
{
	DWORD r;
	if (!pnRead) pnRead = &r;
	return ReadFile(hOutRead, Buf, nToRead, pnRead, 0);
}

bool StdRedirector::ReadFromStdError(LPVOID Buf, DWORD nToRead, LPDWORD pnRead)
{
	DWORD r;
	if (!pnRead) pnRead = &r;
	return ReadFile(hErrRead, Buf, nToRead, pnRead, 0);
}

bool StdRedirector::WriteToStdInput(LPVOID Buf, DWORD nToWtrie, LPDWORD pnWrite)
{
	DWORD w;
	if (!pnWrite) pnWrite = &w;
	return WriteFile(hInWrite, Buf, nToWtrie, pnWrite, 0);
}

void StdRedirector::WaitInput()
{
	WaitForSingleObject(hInRead, INFINITE);
	return;
}

bool StdRedirector::WriteNextLine()
{
	DWORD w;
	char *NL = "\r\n";
	return WriteFile(hInWrite, NL, strlen(NL), &w, 0);
}

bool StdRedirector::SetOutputToConsole()
{
	AllocConsole();

	FILE *fp = fopen("CONOUT$", "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);//�����޻���

	return true;
}

StdRedirector::~StdRedirector()
{
	CloseHandle(hInRead);
	CloseHandle(hInWrite);
	CloseHandle(hOutRead);
	CloseHandle(hOutWrite);
	CloseHandle(hErrRead);
	CloseHandle(hErrWrite);

	FreeConsole();
}
