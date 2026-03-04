#include <Geode/modify/PlayLayer.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
	void playEndAnimationToPos(cocos2d::CCPoint position) {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->getChildrenCount() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::playEndAnimationToPos(position);
	}
	void playPlatformerEndAnimationToPos(cocos2d::CCPoint position, bool instant) {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->getChildrenCount() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::playPlatformerEndAnimationToPos(position, instant);
	}
	void levelComplete() {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->getChildrenCount() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::levelComplete();
	}
	void togglePracticeMode(bool status) {
		PlayLayer::togglePracticeMode(status);
		if (!status || !isEnabled || !m_checkpointArray || m_checkpointArray->getChildrenCount() < 1) return;
		for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
			if (checkpoint && checkpoint->m_physicalCheckpointObject && !checkpoint->m_physicalCheckpointObject->isVisible()) {
				checkpoint->m_physicalCheckpointObject->setVisible(true);
			}
		}
	}
};