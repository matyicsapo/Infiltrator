#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <sstream>

class Convert {
    Convert () {}

	//~Convert () {}

	Convert (Convert const&);
	Convert& operator= (Convert const&);

public:
    static Convert* Instance ();

    template <class T>
    T ToNum (std::string s) {
        T result;

        std::stringstream ss(s); ss >> result;

        return result;
    }
};

#endif
