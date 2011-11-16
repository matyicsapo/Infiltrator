#include "Conversion.hpp"

Conversion* Conversion::Instance () {
	static Conversion instance;

	return &instance;
}

template<>
char Conversion::ToNum<char> (std::string s) {
	char result;
	result = static_cast<char>(boost::lexical_cast<short>(s));
	return result;
}
