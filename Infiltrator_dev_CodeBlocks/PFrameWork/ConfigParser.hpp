#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <map>
#include <vector>

#include "Conversion.hpp"

class CfgEntries;

/// for internal use only
///
/// first : key name
///
/// second : value
typedef std::pair<std::string, std::string> CfgEntryPair;

/// for internal use only
///
/// first  : section name
///
/// second : entrymap
typedef std::pair<std::string, CfgEntries*> CfgSectionPair;

typedef std::vector<CfgEntryPair*> CfgEntryMap;

typedef std::vector<CfgSectionPair*> CfgSectionMap;

// CfgEntries === //
class CfgEntries {
private:
	CfgEntryMap* entries;

public:
	CfgEntries () : entries(new CfgEntryMap()) {}

	CfgEntries (const CfgEntries& e) { entries = new CfgEntryMap( *(e.entries) ); }

	~CfgEntries ();

	std::string operator[] (std::string key);

	CfgEntryMap::iterator GetIteratorBegin () { return entries->begin(); }

	CfgEntryMap::iterator GetIteratorEnd () { return entries->end(); }

	const CfgEntryMap* GetVector () { return entries; }

	friend class ConfigParser;
};
// === CfgEntries //

// CfgContents === //
class CfgContents {
private:
	CfgSectionMap sections;

public:
	CfgContents () {}

	~CfgContents ();

	CfgEntries& operator[] (std::string section);

	CfgSectionMap::iterator GetIteratorBegin () { return sections.begin(); }

	CfgSectionMap::iterator GetIteratorEnd () { return sections.end(); }

	friend class ConfigParser;
};
// === CfgContents //

class ConfigParser {
private:
    std::map<std::string, std::ifstream*> openFiles;

    std::ifstream* Begin (std::string fileName);
    void End (std::string fileName, bool close);
    void Close (std::string fileName);

	ConfigParser () {}

	~ConfigParser ();

	ConfigParser (ConfigParser const&);
	ConfigParser& operator= (ConfigParser const&);

public:
	static ConfigParser* Instance ();

	bool GetContents (std::string fileName, CfgContents& contents);
};

#define CfgParser ConfigParser::Instance()


#endif // CONFIGPARSER_HPP
