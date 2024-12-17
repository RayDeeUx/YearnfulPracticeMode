#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	void onYearnfulSettings(CCObject* sender) {
		openSettingsPopup(Mod::get());
	}
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;
		if (!Loader::get()->isModLoaded("geode.node-ids")) return true;
		const auto leftSideMenu = this->getChildByIDRecursive("left-side-menu");
		if (!leftSideMenu) return true;
		const auto buttonSprite = CircleButtonSprite::createWithSpriteFrameName("checkpointLogo.png"_spr);
		buttonSprite->getTopNode()->setScale(1.0f); // yoinked from hiimjustin000 -- untested
		buttonSprite->setID("mod-settings-sprite"_spr);
		const auto button = CCMenuItemSpriteExtra::create(buttonSprite, this, menu_selector(MyLevelInfoLayer::onYearnfulSettings));
		button->setID("mod-settings-button"_spr);
		leftSideMenu->addChild(button);
		leftSideMenu->updateLayout();
		return true;
	}
};