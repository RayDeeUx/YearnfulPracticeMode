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
		m_fields->yearningLastCheckpointTime = 0.0;
	}
	void hitGround(GameObject* object, bool unknown) {
		PlayerObject::hitGround(object, unknown);
		if (!m_playLayer || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (m_playLayer->m_isPracticeMode || m_playLayer->m_isTestMode || !m_fields->canCheckpointNow || this != m_playLayer->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		CheckpointObject* checkpoint = m_playLayer->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->canCheckpointNow = false;
	}
	void incrementJumps() {
		PlayerObject::incrementJumps();
		if (!m_playLayer || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (m_playLayer->m_isPracticeMode || m_playLayer->m_isTestMode || m_fields->canCheckpointNow || this != m_playLayer->m_player1 || this->isFlyingAndYearning() || this->m_isSpider) return;
		m_fields->canCheckpointNow = true;
	}
	void playerDestroyed(bool otherPlayer) {
		PlayerObject::playerDestroyed(otherPlayer);
		if (!m_playLayer || !isEnabled) return MyPlayerObject::resetTimer();
		if (m_playLayer->m_isPracticeMode || m_playLayer->m_isTestMode || (this != m_playLayer->m_player1 && this != m_playLayer->m_player2)) return;
		if (!autoPracticeMode && m_playLayer->m_checkpointArray && m_playLayer->m_checkpointArray->getChildrenCount() > 0) return m_playLayer->removeAllCheckpoints();
		m_playLayer->togglePracticeMode(true);
		if (m_playLayer->m_currentCheckpoint) m_playLayer->loadFromCheckpoint(m_playLayer->m_currentCheckpoint);
	}
	void update(float dt) {
		PlayerObject::update(dt);
		if (!m_playLayer || !isEnabled || this->m_isDead) return MyPlayerObject::resetTimer();
		if (m_playLayer->m_isPracticeMode || m_playLayer->m_isTestMode || (this != m_playLayer->m_player1 && this != m_playLayer->m_player2) || (!this->isFlyingAndYearning() && !this->m_isSpider)) return;
		m_fields->yearningLastCheckpointTime += dt;
		if (m_fields->yearningLastCheckpointTime < (Mod::get()->getSettingValue<double>("checkpointDelay") * 10)) return;
		CheckpointObject* checkpoint = m_playLayer->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->yearningLastCheckpointTime = 0.0;
	}
};