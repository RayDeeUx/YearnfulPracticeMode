#include <ninxout.options_api/include/API.hpp>

#define ADD_LEVELINFOLAYER_TOGGLE(displayName, settingsID, detailedDesc)\
	OptionsAPI::addPreLevelSetting<bool>(\
		displayName,\
		settingsID""_spr,\
		[](GJGameLevel*) {\
			const bool origValue = Mod::get()->getSettingValue<bool>(settingsID);\
			Mod::get()->setSettingValue<bool>(settingsID, !origValue);\
		},\
		[](GJGameLevel*) {\
			return Mod::get()->getSettingValue<bool>(settingsID);\
		},\
		"<cl>(From YearnfulPracticeMode)</c>\n" detailedDesc\
	);

#define ADD_PAUSELAYER_TOGGLE(displayName, settingsID, detailedDesc)\
	OptionsAPI::addMidLevelSetting<bool>(\
		displayName,\
		settingsID""_spr,\
		[](GJBaseGameLayer* gjbgl) {\
			const bool origValue = Mod::get()->getSettingValue<bool>(settingsID);\
			Mod::get()->setSettingValue<bool>(settingsID, !origValue);\
		},\
		[](GJBaseGameLayer* gjbgl) {\
			return Mod::get()->getSettingValue<bool>(settingsID);\
		},\
		"<cl>(From YearnfulPracticeMode)</c>\n" detailedDesc\
	);

$on_mod(Loaded) {
	ADD_LEVELINFOLAYER_TOGGLE("Hide Checkpoints in Normal Mode", "hideInNormalMode", "Self-explanatory.")
	ADD_LEVELINFOLAYER_TOGGLE("Auto Practice Mode on Death", "autoPracticeMode", "Automatically enable <cg>Practice Mode</c> on player death.\n\n<cy>If this is disabled,</c> <cj>all</c> <cg>placed checkpoints</c> <cy>will be</c> <cr>deleted</c> <cy>before</c> <co>restarting</c> <cy>the level.</c>")

	ADD_PAUSELAYER_TOGGLE("Hide Checkpoints in Normal Mode", "hideInNormalMode", "Self-explanatory.\n\n<co>(Requires re-entering the level to apply.)</c>")
	ADD_PAUSELAYER_TOGGLE("Auto Practice Mode on Death", "autoPracticeMode", "Automatically enable <cg>Practice Mode</c> on player death.\n\n<cy>If this is disabled,</c> <cj>all</c> <cg>placed checkpoints</c> <cy>will be</c> <cr>deleted</c> <cy>before</c> <co>restarting</c> <cy>the level.</c>")
}

#define ECLIPSE_MODULES_HPP
#include <eclipse.eclipse-menu/include/eclipse.hpp>

using namespace eclipse;
using namespace geode::prelude;

void addSetting(MenuTab& tab, const std::string& settingsKey) {
	if (std::shared_ptr<SettingV3> setting = Mod::get()->getSetting(settingsKey); setting) {
		const std::string& colorlessDesc = string::replace(string::replace(string::replace(string::replace(string::replace(string::replace(string::replace(setting->getDescription().value_or("Self-explanatory."), "<cr>", ""), "<co>", ""), "<cl>", ""), "</c>", ""), "<cj>", ""), "<cy>", ""), "<cg>", "");
		tab.addModSettingToggle(setting).setDescription(colorlessDesc);
	}
}

$on_mod(Loaded) {
	Loader::get()->queueInMainThread([] {
		Mod* mod = Mod::get();
		auto tab = MenuTab::find("YearnfulPracticeMode");

		addSetting(tab, "enabled");
		addSetting(tab, "hideInNormalMode");
		addSetting(tab, "autoPracticeMode");
	}
}