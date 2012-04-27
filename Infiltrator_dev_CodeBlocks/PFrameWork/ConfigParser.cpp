#include "ConfigParser.hpp"

#include <cassert>
#include <fstream>
//#include <iostream>

ConfigParser::~ConfigParser () {
    for (std::map<std::string, std::ifstream*>::iterator it = openFiles.begin(); it != openFiles.end(); it++) {
        it->second->close();
    }
}

ConfigParser* ConfigParser::Instance () {
	static ConfigParser instance;

	return &instance;
}

std::ifstream* ConfigParser::Begin (std::string fileName) {
    std::ifstream* cfgFile;

    std::map<std::string, std::ifstream*>::iterator it = openFiles.find(fileName);
    if (it != openFiles.end()) {
        cfgFile = it->second;
    }
    else {
        cfgFile = new std::ifstream(fileName.c_str());

        if (cfgFile->fail()) {
            // ERROR - couldn't open file
            return 0;
        }

		// CHANGE 2011.11.17.
        openFiles.insert(std::pair<std::string, std::ifstream*>(fileName, cfgFile));
    }

    return cfgFile;
}

void ConfigParser::End (std::string fileName) {
	std::map<std::string, std::ifstream*>::iterator it = openFiles.find(fileName);
    if (it != openFiles.end()) {
        it->second->close();
        openFiles.erase(it);
    }
}

// CHANGE 2011.11.17.
/*
void ConfigParser::End (std::string fileName, bool close) {
    if (close) {
        Close(fileName);
    }
    else {
        std::map<std::string, std::ifstream*>::iterator it = openFiles.find(fileName);
        if (it != openFiles.end()) {
            openFiles.insert(std::pair<std::string, std::ifstream*>(fileName, it->second));
        }
    }
}

void ConfigParser::Close (std::string fileName) {
    std::map<std::string, std::ifstream*>::iterator it = openFiles.find(fileName);
    if (it != openFiles.end()) {
        it->second->close();
        openFiles.erase(it);
    }
}*/

bool ConfigParser::GetContents (std::string fileName, CfgContents& contents) {
    std::ifstream* cfgFile = Begin(fileName);

    if (cfgFile == 0) {
        return false;
    }

    std::string line;

    std::string lastSectionName = "";

    while (!cfgFile->eof()) {
        std::getline(*cfgFile, line);

        size_t posSectionClose = line.find(']', 2);
        if (line[0] == '[' && posSectionClose != std::string::npos) {
            // it's a section
            lastSectionName = "";
            for (size_t i = 1; i < posSectionClose; i++) {
                lastSectionName += line[i];
            }
        }
        else if (line[0] != ';' && line[0] != '#') {
            // it's not a comment
            // it should be a valid entry..don't use whitespaces!!
            size_t pos = line.find_first_of("=");
            if (pos != std::string::npos) {
                std::string key = "";
                for (size_t i = 0; i < pos; i++) {
                    key += line[i];
                }

                std::string value = "";
                for (size_t i = pos+1; i < line.length(); i++) {
                    value += line[i];
                }

				size_t returnPos = value.rfind("\r");
                if (returnPos != std::string::npos) {
                	std::string::iterator itFirst = value.begin() + returnPos;
                	std::string::iterator itLast = itFirst + 1;
                	value.erase(itFirst, itLast);
                }

                CfgEntries* entriesForThisSection;

				CfgSectionMap::iterator it = contents.sections.end();
                for (CfgSectionMap::iterator itSections = contents.sections.begin(); itSections != contents.sections.end();
						itSections++) {
					if ( (*itSections)->first == lastSectionName) {
						it = itSections;
						break;
					}
				}

                if (it != contents.sections.end()) {
                    entriesForThisSection = (*it)->second;
                }
                else {
                    entriesForThisSection = new CfgEntries();

					contents.sections.push_back(new CfgSectionPair(lastSectionName, entriesForThisSection));
                }

                entriesForThisSection->entries->push_back(new CfgEntryPair(key, value));
            }
        }
    }

    End(fileName);

    return true;
}

// CfgEntries === //
CfgEntries::~CfgEntries () {
	for (CfgEntryMap::iterator itEntries = entries->begin(); itEntries != entries->end(); itEntries++) {
		delete (*itEntries);
	}

	delete entries; entries = 0;
}

std::string CfgEntries::operator[] (std::string key) {
	std::string s("");

	for (CfgEntryMap::iterator itEntries = entries->begin(); itEntries != entries->end(); itEntries++) {

		if ( (*itEntries)->first == key ) {
			s = (*itEntries)->second;
			return s;
		}

	}

	return s;
}
// === CfgEntries //

// CfgContents === //
CfgContents::~CfgContents () {
	for (CfgSectionMap::iterator itSections = sections.begin(); itSections != sections.end(); itSections++) {
		delete (*itSections)->second; (*itSections)->second = 0;
	};
}

CfgEntries& CfgContents::operator[] (std::string section) {
	for (CfgSectionMap::iterator itSections = sections.begin(); itSections != sections.end(); itSections++) {
		if ( (*itSections)->first == section) {
			return *(*itSections)->second;
		}
	}

	assert(0);
}

bool CfgContents::Exists (std::string section) {
	for (CfgSectionMap::iterator itSections = sections.begin(); itSections != sections.end(); itSections++) {
		if ( (*itSections)->first == section) {
			return true;
		}
	}

	return false;
}
// === CfgContents //
