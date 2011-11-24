#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <boost/lexical_cast.hpp>

class Conversion {
    Conversion () {}

	Conversion (Conversion const&);
	Conversion& operator= (Conversion const&);

public:
    static Conversion* Instance ();

    template<class T>
    T ToNum (std::string s) {
    	T result;
		result = boost::lexical_cast<T>(s);
        return result;
    }

	// HACK - couldn't get unsigned values working with template specifications
    unsigned char ToUnsignedChar (std::string s) {
    	unsigned char result;
    	result = static_cast<unsigned char>(boost::lexical_cast<unsigned short>(s));
    	return result;
    }
};

#define Convert Conversion::Instance()

#endif // CONVERSION_HPP
