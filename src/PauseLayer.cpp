#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void onYearnfulSettings(CCObject* sender) {
		openSettingsPopup(Mod::get());
	}
	void customSetup() {
		PauseLayer::customSetup();
		if (!Loader::get()->isModLoaded("geode.node-ids")) return;
		const auto leftButtonMenu = this->getChildByIDRecursive("left-button-menu");
		if (!leftButtonMenu) return;
		const auto buttonSprite = CircleButtonSprite::createWithSpriteFrameName("checkpointLogo.png"_spr);
		buttonSprite->setID("mod-settings-sprite"_spr);
		const auto button = CCMenuItemSpriteExtra::create(buttonSprite, this, menu_selector(MyPauseLayer::onYearnfulSettings));
		button->setID("mod-settings-button"_spr);
		leftButtonMenu->addChild(button);
		leftButtonMenu->updateLayout();
	}
};