#include <Geode/modify/PlayerObject.hpp>
#include "Manager.hpp"

#define isEnabled Manager::get()->enabled
#define hideInNormalMode Manager::get()->hideInNormalMode
#define autoPracticeMode Manager::get()->autoPracticeMode

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
		if (!m_gameLayer || m_gameLayer->m_isEditor || !isEnabled || m_isDead) return MyPlayerObject::resetTimer();
		PlayLayer* playLayer = static_cast<PlayLayer*>(m_gameLayer);
		if (playLayer->m_isPracticeMode || playLayer->m_isTestMode || !m_fields->canCheckpointNow || this != playLayer->m_player1 || this->isFlyingAndYearning() || m_isSpider) return;
		CheckpointObject* checkpoint = playLayer->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->canCheckpointNow = false;
	}
	void incrementJumps() {
		PlayerObject::incrementJumps();
		if (!m_gameLayer || m_gameLayer->m_isEditor || !isEnabled || m_isDead) return MyPlayerObject::resetTimer();
		PlayLayer* playLayer = static_cast<PlayLayer*>(m_gameLayer);
		if (playLayer->m_isPracticeMode || playLayer->m_isTestMode || m_fields->canCheckpointNow || this != playLayer->m_player1 || this->isFlyingAndYearning() || m_isSpider) return;
		m_fields->canCheckpointNow = true;
	}
	void playerDestroyed(bool otherPlayer) {
		PlayerObject::playerDestroyed(otherPlayer);
		if (!m_gameLayer || m_gameLayer->m_isEditor || !isEnabled) return MyPlayerObject::resetTimer();
		PlayLayer* playLayer = static_cast<PlayLayer*>(m_gameLayer);
		if (playLayer->m_isPracticeMode || playLayer->m_isTestMode || (this != playLayer->m_player1 && this != playLayer->m_player2)) return;
		if (!autoPracticeMode && playLayer->m_checkpointArray && playLayer->m_checkpointArray->count() > 0) {
			Manager::get()->isFromPlayerObjectHook = true;
			playLayer->removeAllCheckpoints();
			return;
		}
		playLayer->togglePracticeMode(true);
		if (playLayer->m_currentCheckpoint) playLayer->loadFromCheckpoint(playLayer->m_currentCheckpoint);
	}
	void update(float dt) {
		PlayerObject::update(dt);
		if (!m_gameLayer || m_gameLayer->m_isEditor || !isEnabled || m_isDead) return MyPlayerObject::resetTimer();
		PlayLayer* playLayer = static_cast<PlayLayer*>(m_gameLayer);
		if (playLayer->m_isPracticeMode || playLayer->m_isTestMode || (this != playLayer->m_player1 && this != playLayer->m_player2) || (!this->isFlyingAndYearning() && !m_isSpider)) return;
		m_fields->yearningLastCheckpointTime += dt;
		if (m_fields->yearningLastCheckpointTime < (Mod::get()->getSettingValue<double>("checkpointDelay") * 10)) return;
		CheckpointObject* checkpoint = playLayer->markCheckpoint();
		if (hideInNormalMode) checkpoint->m_physicalCheckpointObject->setVisible(false);
		m_fields->yearningLastCheckpointTime = 0.0;
	}
};

$on_mod(Loaded) {
	Manager::loadStuff();
}