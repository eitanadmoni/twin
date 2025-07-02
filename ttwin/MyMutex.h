#pragma once

#include <windows.h>


class MyMutex {
public:
	MyMutex(LPCSTR name);
	~MyMutex();
	HANDLE getHandler();
private:
	HANDLE m_mutexHandler;

};