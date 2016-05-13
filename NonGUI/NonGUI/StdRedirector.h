#ifndef __STDREDIRECTOR_H_
#define __STDREDIRECTOR_H_

#include <windows.h>

class StdRedirector
{
public:
	StdRedirector();
	~StdRedirector();

	bool ReadFromStdOutput(LPVOID, DWORD, LPDWORD pnRead = NULL);
	bool ReadFromStdError(LPVOID, DWORD, LPDWORD pnRead = NULL);
	bool WriteToStdInput(LPVOID, DWORD, LPDWORD pnWrite = NULL);
	bool WriteNextLine();
	void WaitInput();
	bool SetOutputToConsole();

private:
	HANDLE hInRead;
	HANDLE hInWrite;
	HANDLE hOutRead;
	HANDLE hOutWrite;
	HANDLE hErrRead;
	HANDLE hErrWrite;
};

#endif
