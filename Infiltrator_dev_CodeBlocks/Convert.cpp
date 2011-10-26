#include "Convert.hpp"

Convert* Convert::Instance () {
	static Convert instance;

	return &instance;
}

template<>
char Convert::ToNum<char> (std::string s) {
	char result;
	result = static_cast<char>(boost::lexical_cast<short>(s));
	return result;
}
