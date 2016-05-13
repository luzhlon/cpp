// ThreadClass.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include "windows.h"

using namespace std;

class Thread
{
public:
	Thread(){};
	~Thread(){};

	virtual DWORD Run(){ return 0; };
	bool Start();

protected:
	DWORD id;
};

DWORD g_thread_proc(LPVOID thread)
{
	Thread *t = (Thread *)thread;
	return t->Run();
}

bool Thread::Start()
{
	return CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)g_thread_proc, this, 0, &this->id);
}

class Thread1 : public Thread
{
public:
	Thread1(int n);
	//~Thread1(){};

	virtual DWORD Run();
private:
	int m_n;
};

Thread1::Thread1(int n) :m_n(n)
{
}

DWORD Thread1::Run()
{
	for (int i = 0; i < 10; i++)
		//printf("The %d thread.\n", m_n);
		cout << "The " << m_n << " thread.\n";
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Thread1 t1(0);
	Thread1 t2(1);
	puts("Thread 1 start.");
	t1.Start();
	puts("Thread 2 start.");
	t2.Start();
	Sleep(1000);
	return 0;
}

