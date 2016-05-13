#ifndef __THREAD_WIN__
#define __THREAD_WIN__

#include <Windows.h>

class thread_win
{
	friend DWORD _stdcall thread_proc(void *thread);
public:
	thread_win();
	~thread_win();

	void start();
protected:
	virtual void run();
	void set_exit_code(int code){ exit_code = code; }

private:
	DWORD id;
	HANDLE handle;
	int exit_code;
};

DWORD _stdcall thread_proc(void *thread);

#endif
