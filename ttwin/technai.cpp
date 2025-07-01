#include <windows.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

using std::cout;
using std::string;

enum error_code {
	OPENING_ERROR,
	WRITING_ERROR,
};

#define MESSAGE_BOX_PATH "C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp"

/*
* @brief Set the executable of the message box to the run registry so it will execute when the computer is booted
*/
void setRunRegistry() {

	HKEY hkey;
	LSTATUS openStatus = RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	if (openStatus != ERROR_SUCCESS) {
		throw OPENING_ERROR;
	}
	
	LSTATUS writingStatus = RegSetValueExA(hkey, "messageBox", 0, REG_SZ, (const BYTE*)MESSAGE_BOX_PATH, sizeof(MESSAGE_BOX_PATH) + 1);
	if (writingStatus != ERROR_SUCCESS) {
		throw WRITING_ERROR;
	}
}


int main() {
	HANDLE mutexHandler;
	LPCSTR message = "MANAGEMENT PROGRAM IS UP";
	try {
		setRunRegistry(); //make sure that the message-box executable is in the run registry
	}
	catch (error_code errorCode) {
		if (errorCode == OPENING_ERROR) {
			cout << "Fail to open the registery\n";
			return 1;
		}
		else if (errorCode == WRITING_ERROR) {
			cout << "Fail to write to the registry\n";
			return 1;
		}
		else {
			cout << "Unknown error\n";
			return 1;
		}
	}
	mutexHandler = CreateMutexA(NULL, FALSE, "tech_mutex");
	if (mutexHandler == NULL) {
		cout << "Creating mutex failed\n";
		return 1;
	}
	DWORD dwWaitResult = WaitForSingleObject(
		mutexHandler,    
		INFINITE);  

	switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			MessageBoxA(NULL, message, "message", MB_OK);
			Sleep((DWORD)3600000);
			if (!ReleaseMutex(mutexHandler))
			{
				cout << "Failed to release mutex\n";
				return 1;
			}
			if (!CloseHandle(mutexHandler)) {
				cout << "Failed to close mutex handler\n";
				return 1;
			}
		case WAIT_ABANDONED:
			cout << "The mutex is get abandoned!\n";
			return 1;
	}
	return 0;
}