#include "Convert.hpp"

#include <boost/lexical_cast.hpp>

Convert* Convert::Instance () {
	static Convert instance;

	return &instance;
}

// specializations put into different file
template <>
unsigned char Convert::ToNum<unsigned char> (std::string s) {
	unsigned char result;

	result = static_cast<unsigned char>(boost::lexical_cast<short>(s));

	return result;
}

template <>
char Convert::ToNum<char> (std::string s) {
	char result;

	result = static_cast<char>(boost::lexical_cast<short>(s));

	return result;
}
