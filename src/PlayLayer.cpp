#include <Geode/modify/PlayLayer.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {

	void togglePracticeMode(bool status) {
		PlayLayer::togglePracticeMode(status);
		if (!status || !isEnabled) return;
		for (auto checkpoint : CCArrayExt<CheckpointObject*>(this->m_checkpointArray)) {
			if (!checkpoint->m_physicalCheckpointObject->isVisible()) {
				checkpoint->m_physicalCheckpointObject->setVisible(true);
			}
		}
	}
	CheckpointObject* markCheckpoint() {
		auto ret = PlayLayer::markCheckpoint();
		if (m_isPracticeMode || m_isTestMode || !isEnabled) return ret;
		if (Mod::get()->getSettingValue<bool>("hideInNormalMode")) {
			ret->m_physicalCheckpointObject->setVisible(false);
		}
		return ret;
	}
};