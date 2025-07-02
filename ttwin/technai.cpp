#include <windows.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

using std::cout;
using std::string;

enum class error_code {
	OPENING_ERROR,
	WRITING_ERROR,
	MASSEGE_BOX_ERROR
};

const std::string messageBoxPath = "C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp";


void checkIfErrorIs(LSTATUS status, int cond, error_code errorCode) {
	if (status == cond) {
		throw errorCode;
	}
}


void checkIfErrorIsNot(LSTATUS status, int cond, error_code errorCode) {
	if (status != cond) {
		throw(errorCode);
	}
}


void checkErrorStatus(LSTATUS status, int cond,  error_code errorCode, bool equal = false) {
	if (equal) {
		checkIfErrorIs(status, cond, errorCode);
	}
	else {
		checkIfErrorIsNot(status, cond, errorCode);
	}
}

/*
* @brief Set the executable of the message box to the run registry so it will execute when the computer is booted
* @throws OPENING_ERROR throw this error if the RegOpenKeyA fails
* @throws WRITING_ERROR throw this error if the RegSetVAlueExA fails
*/
void setRunRegistry() {
	HKEY hkey;
	LSTATUS openStatus = RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	checkErrorStatus(openStatus, ERROR_SUCCESS,  error_code::OPENING_ERROR);
	
	LSTATUS writingStatus = RegSetValueExA(hkey, "messageBox", 0, REG_SZ, reinterpret_cast<const BYTE*>(messageBoxPath.c_str()), sizeof(messageBoxPath) + 1);
	checkErrorStatus(writingStatus, ERROR_SUCCESS, error_code::WRITING_ERROR);
}


int main() {
	HANDLE mutexHandler;
	int messageBoxStatus;
	LPCSTR message = "MANAGEMENT PROGRAM IS UP";
	try {
		setRunRegistry(); //make sure that the message-box executable is in the run registry

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
			messageBoxStatus = MessageBoxA(NULL, message, "message", MB_OK);
			checkErrorStatus(messageBoxStatus, 0, error_code::MASSEGE_BOX_ERROR, true);
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
			break;
		}
	}
	catch (error_code errorCode) {
		switch (errorCode) {
		case error_code::OPENING_ERROR:
			cout << "Fail to open the registery\n";
			return 1;

		case error_code::WRITING_ERROR:
			cout << "Fail to write to the registry\n";
			return 1;

		case error_code::MASSEGE_BOX_ERROR:
			cout << "Fail to pop a meesage box\n";
			return 1;

		default:
			cout << "Unknown error\n";
			return 1;
		}
		return 0;
	}
}