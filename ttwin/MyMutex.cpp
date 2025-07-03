#include "MyMutex.h"

#include "Exception.h"

#include <iostream>

using std::cerr;
using std::endl;


MyMutex::MyMutex(LPCSTR name) {
	this->m_mutexHandler = CreateMutexA(NULL, FALSE, name);
	if (this->m_mutexHandler == NULL) {
		throw Exception("Creating mutex failed");
	}
}

MyMutex::~MyMutex() {
	if (!ReleaseMutex(this->m_mutexHandler)) {
		cerr << "Failed to release mutex" << endl;
	}
	if (!CloseHandle(this->m_mutexHandler)) {
		cerr << "Failed to close mutex handler" << endl;
	}
}

HANDLE MyMutex::getHandler() {
	return this->m_mutexHandler;
}
