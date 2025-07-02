#pragma once

#include <string>
#include <windows.h>

void checkIfErrorIs(LSTATUS status, int targetStatus, const std::string& errorMessage);
void checkIfErrorIsNot(LSTATUS status, int targetStatus, const std::string& errorMessage);
void checkErrorStatus(LSTATUS status, int targetStatus, const std::string& errorMessage, bool equal);