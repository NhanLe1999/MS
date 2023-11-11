#include "AISpeakingReplayPopup.h"
#include "CameraHelper.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "MsHelperGame.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

INITIALIZE_READER(AISpeakingReplayPopup);

using namespace cocos2d;

std::once_flag ai_speaking_replay_popup;

AISpeakingReplayPopup* AISpeakingReplayPopup::createView() {

	std::call_once(ai_speaking_replay_popup, [] {

		REGISTER_CSB_READER(AISpeakingReplayPopup);

		});

	auto view = reinterpret_cast<AISpeakingReplayPopup*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingReplayPopup.csb"));
	if (view) {
		view->didLoadFromCSB();
		return view;
	}
	return nullptr;
}

void AISpeakingReplayPopup::didLoadFromCSB() {

	_screenSize = Director::getInstance()->getVisibleSize();

	if (auto rootLayout = cocos2d::utils::findChild(this, "root_layout")) {

		const float k_scaleFactor = ((_screenSize.width / _screenSize.height) / (4.0f / 3.0f));

		rootLayout->setScale(std::min(k_scaleFactor, 1.2f));
	}
}

void AISpeakingReplayPopup::onTryAgain(Ref* sender) {


}

void AISpeakingReplayPopup::onLetTalk(Ref* sender) {


}

void AISpeakingReplayPopup::onReplay(Ref* sender) {


}

void AISpeakingReplayPopup::onClose(Ref* sender) {

	this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingReplayPopup::onLocateClickCallback(const std::string& callBackName) {

	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
	   CLICK_MAP(AISpeakingReplayPopup, onTryAgain),
	   CLICK_MAP(AISpeakingReplayPopup, onLetTalk),
	   CLICK_MAP(AISpeakingReplayPopup, onReplay),
	   CLICK_MAP(AISpeakingReplayPopup, onClose)
	};

	if (function_map.find(callBackName) != function_map.end()) {
		return function_map.at(callBackName);
	}

	return nullptr;
}
#endif