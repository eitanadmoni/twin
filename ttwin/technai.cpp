#include "Exception.h"
#include "checkStatus.h"
#include "MyMutex.h"

#include <windows.h>
#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;
using std::cerr;


enum class returnValue {
	SUCCESS,
	FAILURE,
};


const string OPENING_ERROR = "Failed to open registry";
const string WRITING_ERROR = "Failed to set value to registry";
const string MESSAGE_BOX_ERROR = "Failed to pop message box";
LPCSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";


/*
* @brief Set the executable of the message box to the run registry so it will execute when the computer is booted
* @throws OPENING_ERROR throw this error if the RegOpenKeyA fails
* @throws WRITING_ERROR throw this error if the RegSetVAlueExA fails
*/
void setRunRegistry(string MessageBoxPath) {
	HKEY hkey;
	LSTATUS openStatus = RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	checkErrorStatus(openStatus, ERROR_SUCCESS,  OPENING_ERROR, false);
	
	LSTATUS writingStatus = RegSetValueExA(hkey, "messageBox", 0, REG_SZ, reinterpret_cast<const BYTE*>(MessageBoxPath.c_str()), sizeof(MessageBoxPath) + 1);
	checkErrorStatus(writingStatus, ERROR_SUCCESS, WRITING_ERROR, false);
}


int main(int argc, char** argv) {
	int messageBoxStatus = 0;
	MyMutex mutex = MyMutex("MassegeBoxMutex");
	const string MessageBoxPath = argv[0]; //this is the path to current program which we want to set to run registry
	try {
		setRunRegistry(MessageBoxPath); //make sure that the message-box executable is in the run registry

		DWORD dwWaitResult = WaitForSingleObject(
			mutex.getHandler(),
			INFINITE);

		switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			messageBoxStatus = MessageBoxA(NULL, MESSAGE, "message", MB_OK);
			checkErrorStatus(messageBoxStatus, 0, MESSAGE_BOX_ERROR, true);
			Sleep(static_cast<DWORD>(3600000));
			break;

		case WAIT_ABANDONED:
			cerr << "The mutex is abandoned!" << endl;
			return static_cast<int>(returnValue::FAILURE);
		}
	} catch (const Exception& e) {
		cerr << e.getError() << endl;
		return static_cast<int>(returnValue::FAILURE);
	}
	return static_cast<int>(returnValue::SUCCESS);
}