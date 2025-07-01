#include <windows.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

using std::cout;
std::mutex lock;


/*
* Function to set the executable of the message box to the run registry so it will execute will the computer is booted
*/
void setRunRegistry() {
	HKEY hkey;
	LSTATUS open_status = RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	if (open_status != ERROR_SUCCESS) {
		cout << "opening failed\n";
	}
	
	LSTATUS writing_status = RegSetValueExA(hkey, "messageBox", 0, REG_SZ, (const BYTE*)"C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp", sizeof("C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp") + 1);
	if (writing_status != ERROR_SUCCESS) {
		cout << "writing failed\n";
	}
}


int main() {
	HANDLE mutex_handler;
	LPCSTR message = "MANAGEMENT PROGRAM IS UP";
	setRunRegistry(); //make sure that the message-box executable is in the run registry
	
	mutex_handler = CreateMutexA(NULL, false, "tech_mutex");
	if (mutex_handler == NULL) {
		cout << "Creating mutex failed\n";
		return 1;
	}
	DWORD dwWaitResult = WaitForSingleObject(
		mutex_handler,    
		INFINITE);  

	switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			MessageBoxA(NULL, message, "message", MB_OK);
			Sleep((DWORD)3600000);
			if (!ReleaseMutex(mutex_handler))
			{
				cout << "Failed to release mutex\n";
				return 1;
			}
		case WAIT_ABANDONED:
			cout << "The mutex is get abandoned!\n";
			return 1;
	}
	return 0;
}