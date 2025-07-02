#include "Exception.h"
#include "checkStatus.h"

#include <windows.h>
#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;
using std::cerr;


enum returnValue {
	SUCCESS,
	FAILURE,
};


const string OPENING_ERROR = "Failed to open registry";
const string WRITING_ERROR = "Failed to set value to registry";
const string MESSAGE_BOX_ERROR = "Failed to pop message box";
const std::string MESSAGE_BOX_PATH = "C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\x64\\Debug\\ttwin.exe";
const LPCSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";


/*
* @brief Set the executable of the message box to the run registry so it will execute when the computer is booted
* @throws OPENING_ERROR throw this error if the RegOpenKeyA fails
* @throws WRITING_ERROR throw this error if the RegSetVAlueExA fails
*/
void setRunRegistry() {
	HKEY hkey;
	LSTATUS openStatus = RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	checkErrorStatus(openStatus, ERROR_SUCCESS,  OPENING_ERROR, false);
	
	LSTATUS writingStatus = RegSetValueExA(hkey, "messageBox", 0, REG_SZ, reinterpret_cast<const BYTE*>(MESSAGE_BOX_PATH.c_str()), sizeof(MESSAGE_BOX_PATH) + 1);
	checkErrorStatus(writingStatus, ERROR_SUCCESS, WRITING_ERROR, false);
}


int main() {
	HANDLE mutexHandler;
	int messageBoxStatus = 0;
	try {
		setRunRegistry(); //make sure that the message-box executable is in the run registry

		mutexHandler = CreateMutexA(NULL, FALSE, "tech_mutex");
		if (mutexHandler == NULL) {
			cerr << "Creating mutex failed" << endl;
			return FAILURE;
		}
		DWORD dwWaitResult = WaitForSingleObject(
			mutexHandler,
			INFINITE);

		switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			messageBoxStatus = MessageBoxA(NULL, MESSAGE, "message", MB_OK);
			checkErrorStatus(messageBoxStatus, 0, MESSAGE_BOX_ERROR, true);
			Sleep((DWORD)3600000);
			if (!ReleaseMutex(mutexHandler))
			{
				cerr << "Failed to release mutex" << endl;
				return FAILURE;
			}
			if (!CloseHandle(mutexHandler)) {
				cerr << "Failed to close mutex handler" << endl;
				return FAILURE;
			}
			break;

		case WAIT_ABANDONED:
			cerr << "The mutex is abandoned!" << endl;
			if (!CloseHandle(mutexHandler)) {
				cerr << "Failed to close mutex handler" << endl;
				return FAILURE;
			}
			return FAILURE;
		}
	} catch (const Exception& e) {
		cerr << e.getError() << endl;
		return FAILURE;
	}
	return SUCCESS;
}