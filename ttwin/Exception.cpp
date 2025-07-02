#include <string>
#include "Exception.h"

using std::string;

Exception::Exception(const string& error) : m_error{ error }
{} // Initializer List so the block is empty

const string& Exception::getError() const { return m_error; }

bool Exception::operator== (Exception& otherException) {
	if (this->getError() == otherException.getError()) {
		return true;
	}
	return false;
}