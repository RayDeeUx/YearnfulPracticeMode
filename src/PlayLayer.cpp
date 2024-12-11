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
};
