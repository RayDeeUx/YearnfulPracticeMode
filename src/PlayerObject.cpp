#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
	struct Fields {
		bool canCheckpointNow = false;
		double yearningLastCheckpointTime = 0.0;
	};
	bool isFlyingAndYearning() {
		return m_isShip || m_isBird || m_isDart || m_isSwing;
	}
	void hitGround(GameObject* object, bool unknown) {
		PlayerObject::hitGround(object, unknown);
		if (!isEnabled) return;
		const auto pl = PlayLayer::get();
		if (!pl || this->m_isDead || pl->m_isPracticeMode || !m_fields->canCheckpointNow || this != pl->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		bool originalPractice = pl->m_isPracticeMode;
		pl->m_isPracticeMode = true;
		pl->markCheckpoint();
		pl->m_isPracticeMode = originalPractice;
		m_fields->canCheckpointNow = false;
	}
	void incrementJumps() {
		PlayerObject::incrementJumps();
		if (!isEnabled) return;
		const auto pl = PlayLayer::get();
		if (!pl || this->m_isDead || pl->m_isPracticeMode || m_fields->canCheckpointNow || this != pl->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		m_fields->canCheckpointNow = true;
	}
	void playerDestroyed(bool otherPlayer) {
		PlayerObject::playerDestroyed(otherPlayer);
		if (!isEnabled) return;
		const auto pl = PlayLayer::get();
		if (!pl || pl->m_isPracticeMode || (this != pl->m_player1 && this != pl->m_player2)) return;
		pl->togglePracticeMode(true);
		if (pl->m_currentCheckpoint) pl->loadFromCheckpoint(pl->m_currentCheckpoint);
	}
	void update(float dt) {
		PlayerObject::update(dt);
		if (!isEnabled) return;
		const auto pl = PlayLayer::get();
		if (!pl || this->m_isDead || pl->m_isPracticeMode || (this != pl->m_player1 && this != pl->m_player2) || (!this->isFlyingAndYearning() && !this->m_isSpider)) return;
		this->m_fields->yearningLastCheckpointTime += dt;
		if (this->m_fields->yearningLastCheckpointTime < (Mod::get()->getSettingValue<double>("checkpointDelay") * 10)) return;
		bool originalPractice = pl->m_isPracticeMode;
		pl->m_isPracticeMode = true;
		pl->markCheckpoint();
		pl->m_isPracticeMode = originalPractice;
		this->m_fields->yearningLastCheckpointTime = 0.0;
	}
};

class $modify(MyEndLevelLayer, EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();
		if (m_playLayer && !m_playLayer->m_isPracticeMode && isEnabled) m_playLayer->removeAllCheckpoints();
	}
};

class $modify(MyPauseLayer, PauseLayer) {
	void onYearnfulSettings(CCObject* sender) {
		openSettingsPopup(Mod::get());
	}
	void customSetup() {
		PauseLayer::customSetup();
		if (!Loader::get()->isModLoaded("geode.node-ids")) return;
		const auto leftButtonMenu = this->getChildByIDRecursive("left-button-menu");
		if (!leftButtonMenu) return;
		const auto buttonSprite = CircleButtonSprite::createWithSprite("checkpointLogo.png"_spr);
		buttonSprite->setID("mod-settings-sprite"_spr);
		const auto button = CCMenuItemSpriteExtra::create(buttonSprite, this, menu_selector(MyPauseLayer::onYearnfulSettings));
		button->setID("mod-settings-button"_spr);
		leftButtonMenu->addChild(button);
		leftButtonMenu->updateLayout();
	}
};