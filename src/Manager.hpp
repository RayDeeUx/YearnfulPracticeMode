#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	bool enabled = false;
	bool hideInNormalMode = false;
	bool autoPracticeMode = false;
	bool mimicADOFAIPrcMd = false;

	double checkpointDelay = 0.;

	bool isFromPlayerObjectHook = false;
	std::vector<geode::Ref<CheckpointObject>> checkpointObjects;

	static Manager* get() {
		if (!instance) instance = new Manager();
		return instance;
	}

	static void loadStuff() {
		Manager* manager = Manager::get();
		
		manager->enabled = geode::Mod::get()->getSettingValue<bool>("enabled");
		listenForSettingChanges<bool>("enabled", [](const bool enabledNew) {
			Manager::get()->enabled = enabledNew;
		});

		manager->hideInNormalMode = geode::Mod::get()->getSettingValue<bool>("hideInNormalMode");
		listenForSettingChanges<bool>("hideInNormalMode", [](const bool hideInNormalModeNew) {
			Manager::get()->hideInNormalMode = hideInNormalModeNew;
		});

		manager->autoPracticeMode = geode::Mod::get()->getSettingValue<bool>("autoPracticeMode");
		listenForSettingChanges<bool>("autoPracticeMode", [](const bool autoPracticeModeNew) {
			Manager::get()->autoPracticeMode = autoPracticeModeNew;
		});

		manager->mimicADOFAIPrcMd = geode::Mod::get()->getSettingValue<bool>("mimicADOFAIPrcMd");
		listenForSettingChanges<bool>("mimicADOFAIPrcMd", [](const bool mimicADOFAIPrcMdNew) {
			Manager::get()->mimicADOFAIPrcMd = mimicADOFAIPrcMdNew;
		});

		manager->checkpointDelay = std::clamp<double>(geode::Mod::get()->getSettingValue<double>("checkpointDelay"), 1.5, 60.);
		listenForSettingChanges<double>("checkpointDelay", [](const double checkpointDelayNew) {
			Manager::get()->checkpointDelay = std::clamp<double>(checkpointDelayNew, 1.5, 60.);
		});	
	}
};