#include <Geode/modify/PlayLayer.hpp>
#include "Manager.hpp"

#define isEnabled Manager::get()->enabled
#define isMimicADOFAIPrcMd Manager::get()->mimicADOFAIPrcMd

using namespace geode::prelude;

bool togglingOffPracticeModeManually = false;

class $modify(MyPlayLayer, PlayLayer) {
	struct Fields {
		~Fields() {
			togglingOffPracticeModeManually = false;
			if (!Manager::get()->checkpointObjects.empty()) Manager::get()->checkpointObjects.clear();
		}
	};
	void removeAllCheckpoints() {
		if (!togglingOffPracticeModeManually && !m_isPracticeMode && isEnabled && isMimicADOFAIPrcMd && Manager::get()->isFromPlayerObjectHook && m_checkpointArray && m_checkpointArray->count() > 0) {
			for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					checkpoint->m_physicalCheckpointObject->setVisible(false);
					checkpoint->m_physicalCheckpointObject->setOpacity(0);
					Manager::get()->checkpointObjects.push_back(geode::Ref(checkpoint));
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
					checkpoint->m_physicalCheckpointObject->setVisible(false);
					checkpoint->m_physicalCheckpointObject->setOpacity(0);
					PlayLayer::storeCheckpoint(checkpoint);
					checkpoint->m_physicalCheckpointObject->setVisible(false);
					checkpoint->m_physicalCheckpointObject->setOpacity(0);
				}
			}
			Manager::get()->isFromPlayerObjectHook = false;
			Manager::get()->checkpointObjects.clear();
			FMODAudioEngine::get()->resumeAllAudio();
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
		if (m_isPracticeMode && !togglingOffPracticeModeManually && isEnabled && m_currentCheckpoint && m_checkpointArray && m_checkpointArray->count() > 0 && std::ranges::find(Manager::get()->checkpointObjects, Ref(m_currentCheckpoint)) != Manager::get()->checkpointObjects.end()) {
			Manager::get()->checkpointObjects.erase(std::remove(Manager::get()->checkpointObjects.begin(), Manager::get()->checkpointObjects.end(), Ref(m_currentCheckpoint)), Manager::get()->checkpointObjects.end());
		}
		PlayLayer::removeCheckpoint(first);
	}
	void togglePracticeMode(bool status) {
		CheckpointObject* targetCheckpoint = nullptr;
		if (status && isEnabled && isMimicADOFAIPrcMd && m_checkpointArray && m_checkpointArray->count() > 0) {
			if (CheckpointObject* checkedCheckpoint = static_cast<CheckpointObject*>(m_checkpointArray->objectAtIndex(m_checkpointArray->count() - 1)); checkedCheckpoint && isMimicADOFAIPrcMd) {
				targetCheckpoint = checkedCheckpoint;
			}
		}
		if (!status) togglingOffPracticeModeManually = true;
		PlayLayer::togglePracticeMode(status);
		if (!status) togglingOffPracticeModeManually = false;
		if (!status && isEnabled && m_checkpointArray && m_checkpointArray->count() > 0) {
			for (auto checkpoint : CCArrayExt<CheckpointObject*>(m_checkpointArray)) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					checkpoint->m_physicalCheckpointObject->setVisible(false);
				}
			}
		} else if (isEnabled && status && m_checkpointArray && isMimicADOFAIPrcMd && !Manager::get()->checkpointObjects.empty() && m_checkpointArray->count() < 1) {
			for (CheckpointObject* checkpoint : Manager::get()->checkpointObjects) {
				if (checkpoint && checkpoint->m_physicalCheckpointObject) {
					checkpoint->m_physicalCheckpointObject->setVisible(false);
					checkpoint->m_physicalCheckpointObject->setOpacity(0);
					PlayLayer::storeCheckpoint(checkpoint);
					checkpoint->m_physicalCheckpointObject->setVisible(false);
					checkpoint->m_physicalCheckpointObject->setOpacity(0);
				}
			}
			if (CheckpointObject* checkedCheckpoint = Manager::get()->checkpointObjects.back(); checkedCheckpoint) {
				targetCheckpoint = checkedCheckpoint;
			}
			Manager::get()->checkpointObjects.clear();
		}
		if (!status || !isEnabled || !m_checkpointArray || m_checkpointArray->count() < 1) return;
		if (isMimicADOFAIPrcMd && targetCheckpoint && status) { 
			m_currentCheckpoint = targetCheckpoint;
			PlayLayer::loadFromCheckpoint(targetCheckpoint);
			PlayLayer::resetLevel();
			FMODAudioEngine::get()->resumeAllAudio();
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