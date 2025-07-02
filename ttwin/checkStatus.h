#pragma once

#include <string>
#include <windows.h>

/*
* @brief If status equal to targetStatus throw Exception(errorMessage)
* @param status: the status to check if equal to targer
* @param targetStatus: a targe status to check if status equal to
* @param errorMassege: a message to throw error if the statuses are equal
*/
void checkIfErrorIs(LSTATUS status, int targetStatus, const std::string& errorMessage);

/*
* @brief If status not equal to targetStatus throw Exception(errorMessage)
* @param status: the status to check if equal to target
* @param targetStatus: a targe status to check if status equal to
* @param errorMassege: a message to throw error if the statuses are not equal
*/
void checkIfErrorIsNot(LSTATUS status, int targetStatus, const std::string& errorMessage);

/*
* @brief calling checkIfErrorIs or checkIfErrorIsNot according to the equal parameter
* @param status: the status to check if equal to target
* @param targetStatus: a targe status to check if status equal to
* @param errorMassege: a message to throw error if the statuses aren't matching the condition
* @param equal: boolean value if the statuses need to be equal to each other or not
*/
void checkErrorStatus(LSTATUS status, int targetStatus, const std::string& errorMessage, bool equal);