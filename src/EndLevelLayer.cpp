#include <Geode/modify/EndLevelLayer.hpp>

#define isEnabled Mod::get()->getSettingValue<bool>("enabled")

using namespace geode::prelude;

class $modify(MyEndLevelLayer, EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();
		if (m_playLayer && !m_playLayer->m_isPracticeMode && isEnabled) m_playLayer->removeAllCheckpoints();
	}
};