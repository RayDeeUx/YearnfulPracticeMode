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
					checkpoint->m_physicalCheckpointObject->setVisible(false);
				}
			}
		}
		PlayLayer::removeAllCheckpoints();
	}
	void resetLevel() {
		PlayLayer::resetLevel();
		if (!m_isPracticeMode && isEnabled && isMimicADOFAIPrcMd && Manager::get()->isFromPlayerObjectHook && !Manager::get()->checkpointObjects.empty() && m_checkpointArray && m_checkpointArray->count() < 1) {
			for (CheckpointObject* checkpoint : Manager::get()->checkpointObjects) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					PlayLayer::storeCheckpoint(checkpoint);
					checkpoint->m_physicalCheckpointObject->setVisible(false);
				}
			}
			Manager::get()->isFromPlayerObjectHook = false;
			Manager::get()->checkpointObjects.clear();
		}
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
	void removeCheckpoint(bool first) {
		if (isEnabled && m_currentCheckpoint && m_checkpointArray && m_checkpointArray->count() > 0 && std::ranges::find(Manager::get()->checkpointObjects, Ref(m_currentCheckpoint)) != Manager::get()->checkpointObjects.end()) {
			Manager::get()->checkpointObjects.erase(std::remove(Manager::get()->checkpointObjects.begin(), Manager::get()->checkpointObjects.end(), Ref(m_currentCheckpoint)), Manager::get()->checkpointObjects.end());
		}
		PlayLayer::removeCheckpoint(first);
	}
	void togglePracticeMode(bool status) {
		if (status && isEnabled && m_checkpointArray && m_checkpointArray->count() > 0) {
			if (CheckpointObject* checkedCheckpoint = static_cast<CheckpointObject*>(m_checkpointArray->objectAtIndex(m_checkpointArray->count() - 1)); checkedCheckpoint && isMimicADOFAIPrcMd) {
				m_currentCheckpoint = checkedCheckpoint;
			}
		}
		PlayLayer::togglePracticeMode(status);
		if (!status || !isEnabled || !m_checkpointArray || m_checkpointArray->count() < 1) return;
		if (isMimicADOFAIPrcMd && m_currentCheckpoint) PlayLayer::loadFromCheckpoint(m_currentCheckpoint);
		for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
			if (checkpoint && checkpoint->m_physicalCheckpointObject && !checkpoint->m_physicalCheckpointObject->isVisible()) {
				checkpoint->m_physicalCheckpointObject->setVisible(true);
			}
		}
	}
};

$on_mod(Loaded) {
	listenForSettingChanges<bool>("hideInNormalMode", [](const bool hideInNormalModeNew) {
		PlayLayer* pl = PlayLayer::get();
		if (!pl || pl->m_isPracticeMode || !pl->m_checkpointArray || pl->m_checkpointArray->count() < 1) return;
		for (auto checkpoint : CCArrayExt<CheckpointObject*>(pl->m_checkpointArray)) {
			if (checkpoint && checkpoint->m_physicalCheckpointObject) {
				checkpoint->m_physicalCheckpointObject->setVisible(hideInNormalModeNew);
			}
		}
	});
}