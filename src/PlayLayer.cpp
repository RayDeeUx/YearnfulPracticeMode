#include <Geode/modify/PlayLayer.hpp>
#include "Manager.hpp"

#define isEnabled Manager::get()->enabled
#define isMimicADOFAIPrcMd Manager::get()->mimicADOFAIPrcMd

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
	struct Fields {
		~Fields() {
			if (!Manager::get()->checkpointObjects.empty()) Manager::get()->checkpointObjects.clear();
		}
	};
	void removeAllCheckpoints() {
		if (!m_isPracticeMode && isEnabled && isMimicADOFAIPrcMd && Manager::get()->isFromPlayerObjectHook && m_checkpointArray && m_checkpointArray->count() > 0) {
			for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					Manager::get()->checkpointObjects.push_back(geode::Ref(checkpoint));
				}
			}
		}
		PlayLayer::removeAllCheckpoints();
	}
	void resetLevel() {
		PlayLayer::resetLevel();
		if (!m_isPracticeMode && isEnabled && isMimicADOFAIPrcMd && !Manager::get()->checkpointObjects.empty() && m_checkpointArray && m_checkpointArray->count() < 1) {
			for (CheckpointObject* checkpoint : Manager::get()->checkpointObjects) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					PlayLayer::storeCheckpoint(checkpoint);
				}
			}
		}
		Manager::get()->checkpointObjects.empty();
	}
	void playEndAnimationToPos(cocos2d::CCPoint position) {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->count() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::playEndAnimationToPos(position);
	}
	void playPlatformerEndAnimationToPos(cocos2d::CCPoint position, bool instant) {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->count() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::playPlatformerEndAnimationToPos(position, instant);
	}
	void levelComplete() {
		if (!m_isPracticeMode && isEnabled && m_checkpointArray && m_checkpointArray->count() > 0) PlayLayer::removeAllCheckpoints();
		PlayLayer::levelComplete();
	}
	void togglePracticeMode(bool status) {
		PlayLayer::togglePracticeMode(status);
		if (isMimicADOFAIPrcMd || !status || !isEnabled || !m_checkpointArray || m_checkpointArray->count() < 1) return;
		for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
			if (checkpoint && checkpoint->m_physicalCheckpointObject && !checkpoint->m_physicalCheckpointObject->isVisible()) {
				checkpoint->m_physicalCheckpointObject->setVisible(true);
			}
		}
	}
};