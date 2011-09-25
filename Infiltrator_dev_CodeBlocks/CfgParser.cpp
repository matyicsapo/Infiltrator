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

	#include <iostream> //*******************************************************************************

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

                //if (fileName == "Content/Textures/Animated/animated.ini") {
                    //if (key == "path") {
                        //std::cout << value << std::endl;
                        //std::cout << value.length() << std::endl;
                    //}
                //}

                CfgContents::CfgEntries* entriesForThisSection;

				CfgContents::CfgSectionMap::iterator it = contents.sections.end();
                for (CfgContents::CfgSectionMap::iterator itSections = contents.sections.begin(); itSections != contents.sections.end();
						itSections++) {
					if ( (*itSections)->first == lastSectionName) {
						it = itSections;
						break;
					}
				}

                //CfgContents::CfgSectionMap::iterator it = contents.sections.find(lastSectionName);

                if (it != contents.sections.end()) {
                    entriesForThisSection = (*it)->second;
                }
                else {
                    entriesForThisSection = new CfgContents::CfgEntries();

					contents.sections.push_back(new CfgContents::CfgSectionPair(lastSectionName, entriesForThisSection));

                    //contents.sections.insert(
                        //std::pair<std::string, CfgContents::CfgEntries*>(lastSectionName, entriesForThisSection));
                }

                entriesForThisSection->entries->push_back(new CfgContents::CfgEntryPair(key, value));

                //entriesForThisSection->entries->insert(std::pair<std::string, std::string>(key, value));
            }
        }
    }

    End(fileName, true);

    return true;
}

/*

std::string CfgParser::GetString (std::string fileName, std::string key, std::string section, bool close) {
    std::ifstream* cfgFile = Begin(fileName);
    if (cfgFile == 0) {
        return "";
    }

    std::string value("");

    std::string line;

    bool searching = true;

    if (section != "") {
        while (searching) {
            if (cfgFile->eof()) {
                // end of file and section not found
                End(fileName, close);
                return value;
            }
            else {
                std::getline(*cfgFile, line);
                if (line[0] == '[' && line.rfind(section, section.length()) != std::string::npos)
                    searching = false;
            }
        }

        searching = true;
    }

    while (searching) {
        if (cfgFile->eof()) {
            // end of file and key not found
            searching = false;
        }
        else {
            std::getline(*cfgFile, line);
            // if not a section and not a comment and doesn't start with a whitespace
            // identations won't be required so there won't and thus they don't have to be checked
            if (line[0] != '[' && line[0] != ';' && line[0] != ' ' && line[0] != '\t') {
                if (line.rfind(key, key.length()) != std::string::npos) {
                    // search till the end of the line or till a non-plausible character is found
                    // start index - there is an '=' sign between the key & value
                    for (unsigned int i = key.length()+1; i < line.length(); i++) {
                        value += line[i];
                    }

                    searching = false;
                }
            }
        }
    }

    End(fileName, close);

    return value;
}

int CfgParser::GetInt (std::string fileName, std::string key, std::string section, bool close) {
    std::string stringValue = GetString(fileName, key, section, close);

    int intValue;

    std::stringstream ss(stringValue);
    ss >> intValue;

    return intValue;
}

float CfgParser::GetFloat (std::string fileName, std::string key, std::string section, bool close) {
    std::string stringValue = GetString(fileName, key, section, close);

    float floatValue;

    std::stringstream ss(stringValue);
    ss >> floatValue;

    return floatValue;
}

std::vector<std::string>* CfgParser::GetAllSections (std::string fileName, bool close) {
    std::ifstream* cfgFile = Begin(fileName);
    if (cfgFile == 0) {
        return 0;
    }

    std::vector<std::string>* allSections = 0;

    std::string line;

    while (!cfgFile->eof()) {
        std::getline(*cfgFile, line);
        int iClosingBracket = line.find(']', 2);
        if (line[0] == '[' && iClosingBracket) {
            std::string sectionName("");
            for (int i = 1; i < iClosingBracket; i++) {
                sectionName += line[i];
            }

            if (allSections == 0) {
                allSections = new std::vector<std::string>();
            }

            allSections->push_back(sectionName);
        }
    }

    End(fileName, close);

    return allSections;
}
*/
