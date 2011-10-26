#include "CfgParser.hpp"

CfgParser::~CfgParser () {
    for (std::map<std::string, std::ifstream*>::iterator it = openFiles.begin(); it != openFiles.end(); it++) {
        it->second->close();
    }
}

CfgParser* CfgParser::Instance () {
	static CfgParser instance;

	return &instance;
}

std::ifstream* CfgParser::Begin (std::string fileName) {
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
    }

    return cfgFile;
}

void CfgParser::End (std::string fileName, bool close) {
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

void CfgParser::Close (std::string fileName) {
    std::map<std::string, std::ifstream*>::iterator it = openFiles.find(fileName);
    if (it != openFiles.end()) {
        it->second->close();
        openFiles.erase(it);
    }
}

bool CfgParser::GetContents (std::string fileName, CfgParser::CfgContents& contents) {
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
        else if (line[0] != ';' || line[0] != '#') {
            // it's not a comment
            // it should be a valid entry..don't use whitespaces!!
            size_t pos = line.find_first_of("=:");
            if (pos != std::string::npos) {
                std::string key = "";
                for (size_t i = 0; i < pos; i++) {
                    key += line[i];
                }

                std::string value = "";
                for (size_t i = pos+1; i < line.length(); i++) {
                    value += line[i];
                }

                CfgContents::CfgEntries* entriesForThisSection;

				CfgContents::CfgSectionMap::iterator it = contents.sections.end();
                for (CfgContents::CfgSectionMap::iterator itSections = contents.sections.begin(); itSections != contents.sections.end();
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
                    entriesForThisSection = new CfgContents::CfgEntries();

					contents.sections.push_back(new CfgContents::CfgSectionPair(lastSectionName, entriesForThisSection));
                }

                entriesForThisSection->entries->push_back(new CfgContents::CfgEntryPair(key, value));
            }
        }
    }

    End(fileName, true);

    return true;
}
