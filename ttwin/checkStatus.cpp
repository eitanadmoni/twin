#include "checkStatus.h"

#include "Exception.h"

#include <windows.h>
#include <string>


using std::string;


void checkIfErrorIs(LSTATUS status, int targetStatus,  const string& errorMessage) {
	if (status == targetStatus) {
		throw Exception(errorMessage);
	}
}


void checkIfErrorIsNot(LSTATUS status, int targetStatus, const string&  errorMessage) {
	if (status != targetStatus) {
		throw Exception(errorMessage);
	}
}


void checkErrorStatus(LSTATUS status, int targetStatus, const string& errorMessage, bool equal) {
	if (equal) {
		checkIfErrorIs(status, targetStatus, errorMessage);
	}
	else {
		checkIfErrorIsNot(status, targetStatus, errorMessage);
	}
}