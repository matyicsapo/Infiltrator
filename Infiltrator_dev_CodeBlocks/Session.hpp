#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

class Session {
private:
	static Session* instance;

    Session () : profileName(""), language(""), allGold(0) {}

	Session (Session const&);
	Session& operator= (Session const&);

public:
	static Session* Instance ();

	static void Release ();

	void SaveProfile ();
	bool LoadProfile (std::string profileName);

	std::string profileName;
	std::string language;
	int allGold;
};

#define CurrentSession Session::Instance()

#endif // SESSION_HPP
