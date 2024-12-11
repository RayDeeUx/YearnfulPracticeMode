#include <Geode/modify/PlayerObject.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")
#define hideInNormalMode Mod::get()->getSettingValue<bool>("hideInNormalMode")
#define autoPracticeMode Mod::get()->getSettingValue<bool>("autoPracticeMode")

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
	struct Fields {
		bool canCheckpointNow = false;
		double yearningLastCheckpointTime = 0.0;
	};
	bool isFlyingAndYearning() {
		return m_isShip || m_isBird || m_isDart || m_isSwing;
	}
	void resetTimer() {
		if (m_fields) m_fields->yearningLastCheckpointTime = 0.0;
	}
	void hitGround(GameObject* object, bool unknown) {
		PlayerObject::hitGround(object, unknown);
		const auto pl = PlayLayer::get();
		if (!pl || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (pl->m_isPracticeMode || pl->m_isTestMode || !m_fields->canCheckpointNow || this != pl->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		CheckpointObject* checkpoint = pl->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->canCheckpointNow = false;
	}
	void incrementJumps() {
		PlayerObject::incrementJumps();
		const auto pl = PlayLayer::get();
		if (!pl || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (pl->m_isPracticeMode || pl->m_isTestMode || m_fields->canCheckpointNow || this != pl->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		m_fields->canCheckpointNow = true;
	}
	void playerDestroyed(bool otherPlayer) {
		PlayerObject::playerDestroyed(otherPlayer);
		const auto pl = PlayLayer::get();
		if (!pl || !isEnabled) return MyPlayerObject::resetTimer();
		if (pl->m_isPracticeMode || pl->m_isTestMode || (this != pl->m_player1 && this != pl->m_player2)) return;
		if (!autoPracticeMode) return pl->removeAllCheckpoints();
		pl->togglePracticeMode(true);
		if (pl->m_currentCheckpoint) pl->loadFromCheckpoint(pl->m_currentCheckpoint);
	}
	void update(float dt) {
		PlayerObject::update(dt);
		if (!m_fields) return;
		const auto pl = PlayLayer::get();
		if (!pl || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (pl->m_isPracticeMode || pl->m_isTestMode || (this != pl->m_player1 && this != pl->m_player2) || (!this->isFlyingAndYearning() && !this->m_isSpider)) return;
		m_fields->yearningLastCheckpointTime += dt;
		if (m_fields->yearningLastCheckpointTime < (Mod::get()->getSettingValue<double>("checkpointDelay") * 10)) return;
		CheckpointObject* checkpoint = pl->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->yearningLastCheckpointTime = 0.0;
	}
};
