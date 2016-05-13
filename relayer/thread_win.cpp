#include "thread_win.h"


thread_win::thread_win()
{
	exit_code = 0;
}


thread_win::~thread_win()
{
}

void thread_win::start()
{
	handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_proc, this, NULL, &id);
}

void thread_win::run()
{
	set_exit_code(0);
}

DWORD _stdcall thread_proc(void *thread)
{
	thread_win *t = (thread_win*)thread;
	t->run();
	int code = t->exit_code;
	delete t;
	return code;
}