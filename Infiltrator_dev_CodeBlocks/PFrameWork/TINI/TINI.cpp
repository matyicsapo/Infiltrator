#include "TINI.hpp"

//#include <iostream>
#include <stdio.h>

using namespace TINI;

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
TINIObject::TINIObject () : fileName("") {}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
TINIObject::TINIObject (std::string fileName) : fileName("") {
	SetFile(fileName);
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
TINIObject::~TINIObject () {
	file.close();
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
void TINIObject::SetFile (std::string fileName) {
	file.close();

	this->fileName = fileName;

	file.open(fileName.c_str(), std::fstream::in | std::fstream::out);

	if (!file.is_open()) {
		// HACKy file creation
		file.close();
		file.clear();

		file.open(fileName.c_str(), std::fstream::out | std::fstream::trunc);
		file << "dummy_DO_NOT_EDIT_dummy" << std::endl;
		file.close();

		file.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);

		if (!file.is_open()) {
//std::cout << "ERROR: " << fileName << " could not be opened" << std::endl;
		}
		else {
//std::cout << "WARNING: " << fileName << " had to be created" << std::endl;
		}
	}
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
void TINIObject::Begin () {
	if (!file.is_open() && fileName != "") {
		SetFile(fileName);
	}
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
void TINIObject::End () {
	file.close();
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
void TINIObject::SetValue (std::string section, std::string key, std::string value) {
	if (!file.is_open()) {
//std::cout << "ERROR: no file is open for this TINIObject instance." << std::endl;
		return;
	}

	// clearing flags and resetting seek get position
	file.clear();
	file.seekg(0, std::ios::beg);

	std::string newFileName = fileName + "__TINI";
	std::fstream newFile(newFileName.c_str(), std::ios::out);

	if (!newFile.is_open()) {
//std::cout << "ERROR: " << newFile << " temporary file could not be created/opened. Terminating." << std::endl;
		return;
	}

	std::string line;

	bool haveFoundSection = "" == section;
	bool haveFoundMatch = false;

	while (!file.eof()) {
		std::getline(file, line);

		if (line == "" && file.eof())
			break;

		if (!haveFoundMatch)  {
			size_t posSectionClose = line.find(']', 2);
			if (line[0] == '[' && posSectionClose != std::string::npos) {
				std::string lSection = "";
				for (size_t i = 1; i < posSectionClose; i++) {
					lSection += line[i];
				}

				if (haveFoundSection) {
					// have already found section but key wasn't, but since we're just passing into a new one
					// we'll still append the new value at the end of the section
					newFile << key << "=" << value << std::endl;
					haveFoundMatch = true;
				}

				newFile << line << std::endl;

				haveFoundSection = lSection == section;
			}
			else if (haveFoundSection && (line[0] != ';' && line[0] != '#')) {
				size_t pos = line.find_first_of("=");

				if (pos == std::string::npos) {
					newFile << line << std::endl;
				}
				else {
					std::string lKey = "";
					for (size_t i = 0; i < pos; i++) {
						lKey += line[i];
					}

					if (lKey == key) {
						haveFoundMatch = true;
						newFile << key << "=" << value << std::endl;
					}
					else {
						newFile << line << std::endl;
					}
				}
			}
			else /*(!file.eof())*/ {
				newFile << line << std::endl;
			}
		}
		else /*if (file.eof())*/ {
			// if have found our match then we just flush the rest
			newFile << line << std::endl;
		}
	}

	if (!haveFoundMatch) {
		if (section != "" && !haveFoundSection) {
			newFile << "[" << section << "]" << std::endl;
		}

		newFile << key << "=" << value << std::endl;
	}

	file.close();
	if (std::remove(fileName.c_str()) != 0) {
//std::cout << "ERROR: failed to REMOVE old file" << std::endl;
	}

	newFile.close();
	if (std::rename(newFileName.c_str(), fileName.c_str()) != 0) {
//std::cout << "ERROR: failed to RENAME new file" << std::endl;
	}
	//newFile.close();

	SetFile(fileName);
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
std::string TINIObject::GetValue (std::string section, std::string key) {
	if (!file.is_open()) {
//std::cout << "ERROR: no file is open for this TINIObject instance." << std::endl;
		return "";
	}

	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	std::string sectionFound = "";
	bool isSectionFound = sectionFound == section;

	while (!file.eof()) {
		std::getline(file, line);

		if (line == "")
			continue;

		size_t posSectionClose = line.find(']', 2);
		if (line[0] == '[' && posSectionClose != std::string::npos) {
			// it IS a section

			if (isSectionFound) {
//std::cout << "WARNING: parsed section: " << section << " but key: " << key << " not found. Terminating." << std::endl;
				return "";
			}

			sectionFound = "";
			for (size_t i = 1; i < posSectionClose; i++) {
				sectionFound += line[i];
			}

			isSectionFound = sectionFound == section;
		}
        else if (isSectionFound) {
        	if (line[0] == ';' || line[0] == '#')
				continue;

        	// it's not a comment

            size_t pos = line.find_first_of("=:");

			if (pos == std::string::npos)
				continue;

            std::string keyFound = "";
			for (size_t i = 0; i < pos; i++) {
				keyFound += line[i];
			}

			if (keyFound == key) {
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

                return value;
			}
        }
	}

//std::cout << "WARNING: no results for section: " << section << " & key: " << key << std::endl;
	return "";
}

/// ///////////////////////////////////////////////////////////////////////////
///
/// ///////////////////////////////////////////////////////////////////////////
void TINIObject::RemoveEntry (std::string section, std::string key) {
	if (!file.is_open()) {
//std::cout << "ERROR: no file is open for this TINIObject instance." << std::endl;
		return;
	}

	// clearing flags and resetting seek get position
	file.clear();
	file.seekg(0, std::ios::beg);

	std::string newFileName = fileName + "__TINI";
	std::fstream newFile(newFileName.c_str(), std::ios::out);

	if (!newFile.is_open()) {
//std::cout << "ERROR: " << newFile << " temporary file could not be created/opened. Terminating." << std::endl;
		return;
	}

	std::string line;

	bool haveFoundSection = "" == section;
	bool haveFoundMatch = false;

	while (!file.eof()) {
		std::getline(file, line);

		if (!haveFoundMatch)  {
			size_t posSectionClose = line.find(']', 2);
			if (line[0] == '[' && posSectionClose != std::string::npos) {
				if (haveFoundSection) {
					newFile.close();
					std::remove(newFileName.c_str());
					return;
				}

				std::string lSection = "";
				for (size_t i = 1; i < posSectionClose; i++) {
					lSection += line[i];
				}

				newFile << line << std::endl;

				haveFoundSection = lSection == section;
			}
			else if (haveFoundSection && (line[0] != ';' && line[0] != '#')) {
				size_t pos = line.find_first_of("=");

				if (pos == std::string::npos) {
					newFile << line << std::endl;
				}
				else {
					std::string lKey = "";
					for (size_t i = 0; i < pos; i++) {
						lKey += line[i];
					}

					if (lKey == key) {
						haveFoundMatch = true;
//						newFile << key << "=" << value << std::endl;
					}
					else {
						newFile << line << std::endl;
					}
				}
			}
			else {
				newFile << line << std::endl;
			}
		}
		else {
			newFile << line << std::endl;
		}
	}

	file.close();
	if (std::remove(fileName.c_str()) != 0) {
//std::cout << "ERROR: failed to REMOVE old file" << std::endl;
	}

	newFile.close();
	if (std::rename(newFileName.c_str(), fileName.c_str()) != 0) {
//std::cout << "ERROR: failed to RENAME new file" << std::endl;
	}
	//newFile.close();

	SetFile(fileName);
}
