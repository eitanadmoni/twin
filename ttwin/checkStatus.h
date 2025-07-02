#pragma once
#include <string>
#include <windows.h>

void checkIfErrorIs(LSTATUS status, int targetStatus, std::string errorMessage);
void checkIfErrorIsNot(LSTATUS status, int targetStatus, std::string errorMessage);
void checkErrorStatus(LSTATUS status, int targetStatus, std::string errorMessage, bool equal);