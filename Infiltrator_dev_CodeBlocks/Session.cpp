#include "Session.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/SoundManager.hpp"
#include "PFrameWork/Conversion.hpp"
#include "PFrameWork/TINI/TINI.hpp"
#include "proto/InfiltratorProfile.pb.h"
#include <fstream>

using namespace Infiltrator;

Session* Session::instance = 0;

Session* Session::Instance () {
	if (instance == 0)
		instance = new Session();

	return instance;
}

void Session::Release () {
	delete instance;
	instance = 0;
}

void Session::SaveProfile () {
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	Profile gameProfile;

	// SAVE DATA ===
	gameProfile.set_allgold(allGold);

	TINI::TINIObject conf("Saves/" + profileName + ".ini");

	conf.SetValue("", "language", language);

	conf.SetValue("", "fullscr", Convert->ToString(Game->IsFullScreen()));
	conf.SetValue("", "clientwidth", Convert->ToString(Game->GetRenderWindow()->GetWidth()));
	conf.SetValue("", "clientheight", Convert->ToString(Game->GetRenderWindow()->GetHeight()));
	conf.SetValue("", "vol_master", Convert->ToString(Sounds->GetVol_Master()));
	conf.SetValue("", "vol_music", Convert->ToString(Sounds->GetVol_Music()));
	conf.SetValue("", "vol_effect", Convert->ToString(Sounds->GetVol_Effect()));

	// updating global config
	TINI::TINIObject gconf("Saves/global.ini");

	gconf.SetValue("", "last_profile", profileName);
	gconf.SetValue("", "language", language);
	gconf.SetValue("", "fullscr", Convert->ToString(Game->IsFullScreen()));
	gconf.SetValue("", "clientwidth", Convert->ToString(Game->GetRenderWindow()->GetWidth()));
	gconf.SetValue("", "clientheight", Convert->ToString(Game->GetRenderWindow()->GetHeight()));
	gconf.SetValue("", "vol_master", Convert->ToString(Sounds->GetVol_Master()));
	gconf.SetValue("", "vol_music", Convert->ToString(Sounds->GetVol_Music()));
	gconf.SetValue("", "vol_effect", Convert->ToString(Sounds->GetVol_Effect()));
	// === SAVE DATA

	bool failed = false;
	std::fstream profileFile(("Saves/" + profileName + ".pro").c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (profileFile.is_open()) {
		if (!gameProfile.SerializeToOstream(&profileFile)) {
			failed = true;
		}
		profileFile.close();
	}

	if (!failed) {
		TINI::TINIObject* profileList = new TINI::TINIObject("Saves/profilelist.ini");
		profileList->SetValue("", profileName, "1");
		delete profileList;
	}

	google::protobuf::ShutdownProtobufLibrary();
}

bool Session::LoadProfile (std::string profileName) {
	this->profileName = profileName;
	language = "ENG";
	allGold = 0;

	if (profileName == "")
		return false;

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	std::fstream profileFile(("Saves/" + profileName + ".pro").c_str(), std::ios::in | std::ios::binary);
	if (!profileFile.is_open()) {
		return false;
	}

	Profile gameProfile;

	if (!gameProfile.ParseFromIstream(&profileFile)) {
		google::protobuf::ShutdownProtobufLibrary();
		return false;
	}

	// LOAD DATA ===
	allGold = gameProfile.allgold();

	TINI::TINIObject conf("Saves/" + profileName + ".ini");

	profileName = profileName;

	language = conf.GetValue("", "language");
	Game->Create(Convert->ToNum<int>(conf.GetValue("", "clientwidth")),
				Convert->ToNum<int>(conf.GetValue("", "clientheight")),
				conf.GetValue("", "fullscr") == "1" ? true : false);

	Sounds->SetVol_Master(Convert->ToNum<float>(conf.GetValue("", "vol_master")));
	Sounds->SetVol_Music(Convert->ToNum<float>(conf.GetValue("", "vol_music")));
	Sounds->SetVol_Effect(Convert->ToNum<float>(conf.GetValue("", "vol_effect")));
	// === LOAD DATA

	google::protobuf::ShutdownProtobufLibrary();

	return true;
}
