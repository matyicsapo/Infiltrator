#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <boost/lexical_cast.hpp>

class Convert {
    Convert () {}

	Convert (Convert const&);
	Convert& operator= (Convert const&);

public:
    static Convert* Instance ();

    template<class T>
    T ToNum (std::string s) {
    	T result;
		result = boost::lexical_cast<T>(s);
        return result;
    }

	// HACK
	// couldn't get unsigned values working with template specifications
    unsigned char ToUnsignedChar (std::string s) {
    	unsigned char result;
    	result = static_cast<unsigned char>(boost::lexical_cast<unsigned short>(s));
    	return result;
    }
};

#endif // CONVERT_HPP
