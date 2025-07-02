#pragma once

#include <string>

class Exception{
public:
	/**
	* @brief Builder for Exception class
	* @param string that describe the error
	* @return A HashException item
	*/
	Exception(const std::string& error);

	/**
	* @brief Exception method to get the error massege
	* @return Exception massege
	*/
	const std::string& getError() const;

	/**
	* @brief Exception method to compare between 2 exceptions
	* @return boolean value of the comparison
	*/
	bool operator== (Exception& otherException);

private:
	std::string m_error; // The error string
};