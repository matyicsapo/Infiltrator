#ifndef CFGPARSER_HPP
#define CFGPARSER_HPP

#include <string>

#include <map>
#include <fstream>

#include <vector>

#include "Convert.hpp"

#include <cassert>

class CfgParser {
    std::map<std::string, std::ifstream*> openFiles;

    std::ifstream* Begin (std::string fileName);
    void End (std::string fileName, bool close);
    void Close (std::string fileName);

	CfgParser () {}

	~CfgParser ();

	CfgParser (CfgParser const&);
	CfgParser& operator= (CfgParser const&);

public:
	static CfgParser* Instance ();

	// CfgContents === //
	class CfgContents {
        class CfgEntries;

    public:
		/// first : key name
		///
		/// second : value
		typedef std::pair<std::string, std::string> CfgEntryPair;

		/// first  : section name
		///
		/// second : entrymap
		typedef std::pair<std::string, CfgEntries*> CfgSectionPair;

	    typedef std::vector<CfgEntryPair*> CfgEntryMap;

	    typedef std::vector<CfgSectionPair*> CfgSectionMap;

    private:
        // CfgEntries === //
        class CfgEntries {
            CfgEntryMap* entries;

        public:
            CfgEntries () : entries(new CfgEntryMap()) {}

            CfgEntries (const CfgEntries& e) {
                entries = new CfgEntryMap( *(e.entries) );
            }

            ~CfgEntries () {
            	for (CfgEntryMap::iterator itEntries = entries->begin(); itEntries != entries->end(); itEntries++) {
					delete (*itEntries);
				}

                delete entries; entries = 0;
            }

            std::string operator [] (std::string key) {
            	std::string s("");

                for (CfgEntryMap::iterator itEntries = entries->begin(); itEntries != entries->end(); itEntries++) {

					if ( (*itEntries)->first == key ) {
						s = (*itEntries)->second;
						return s;
					}

				}

				return s;
            }

            CfgEntryMap::iterator GetIteratorBegin () {
                return entries->begin();
            }

            CfgEntryMap::iterator GetIteratorEnd () {
                return entries->end();
            }

            friend class CfgParser;
        };
        // === CfgEntries //

        CfgSectionMap sections;

    public:
		CfgContents () {}

        ~CfgContents () {
            for (CfgSectionMap::iterator itSections = sections.begin(); itSections != sections.end(); itSections++) {
                delete (*itSections)->second; (*itSections)->second = 0;
            };
        }

        CfgEntries& operator [] (std::string section) {
            for (CfgSectionMap::iterator itSections = sections.begin(); itSections != sections.end(); itSections++) {
				if ( (*itSections)->first == section) {
					return *(*itSections)->second;
				}
			}

			assert(0);
        }

        CfgSectionMap::iterator GetIteratorBegin () {
            return sections.begin();
        }

        CfgSectionMap::iterator GetIteratorEnd () {
            return sections.end();
        }

        friend class CfgParser;
	};
	// === CfgContents //

/*
    // CfgContents === //
	class CfgContents {
        class CfgEntries;

    public:
        /// first : key name
        ///
        /// second : value
	    typedef std::map<std::string, std::string> CfgEntryMap;

	    /// first  : section name
	    ///
	    /// second : entrymap pointer
	    typedef std::map<std::string, CfgEntries*> CfgSectionMap;

    private:
        // CfgEntries === //
        class CfgEntries {
            CfgEntryMap* entries;

        public:
            CfgEntries () {
                entries = new CfgEntryMap();
            }

            CfgEntries (const CfgEntries& e) {
                entries = new CfgEntryMap(*(e.entries));
            }

            ~CfgEntries () {
                delete entries; entries = 0;
            }

            std::string operator [] (std::string key) {
                return (*entries)[key];
            }

            CfgEntryMap::iterator GetIteratorBegin () {
                return entries->begin();
            }

            CfgEntryMap::iterator GetIteratorEnd () {
                return entries->end();
            }

            friend class CfgParser;
        };
        // === CfgEntries //

// why is this a pointer? ******************************************************************************************************
        CfgSectionMap* sections;

    public:
        CfgContents () {
            sections = new CfgSectionMap();
        }

        ~CfgContents () {
            for (CfgSectionMap::iterator itSections = sections->begin(); itSections != sections->end(); itSections++) {
                delete itSections->second;
                itSections->second = 0;
            }

            delete sections; sections = 0;
        }

        CfgEntries operator [] (std::string sectionName) {
            return CfgEntries(*(*sections)[sectionName]);
        }

        CfgSectionMap::iterator GetIteratorBegin () {
            return sections->begin();
        }

        CfgSectionMap::iterator GetIteratorEnd () {
            return sections->end();
        }

        friend class CfgParser;
	};
	// === CfgContents //
*/

	bool GetContents (std::string fileName, CfgContents& contents);

    // //
    // obsolete methods
    // despite keeping the file open was possible it had to be searched for every Get
/*
	std::string GetString (std::string fileName, std::string key, std::string section = "", bool close = true);

	int GetInt (std::string fileName, std::string key, std::string section = "", bool close = true);
	float GetFloat (std::string fileName, std::string key, std::string section = "", bool close = true);

	std::vector<std::string>* GetAllSections (std::string fileName, bool close = true);
*/
};

#endif
