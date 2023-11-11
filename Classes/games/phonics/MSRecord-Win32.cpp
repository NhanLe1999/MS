#include "MSRecord.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
USING_NS_CC;
#pragma mark - PLAY ANI OBJECT - One layer
#include "MSRecord.h"

MSRecord * MSRecord::createRecord(float timer, std::string phrase, bool isShowNativeRemainPopup) {
	auto p_food = new MSRecord();
	if (p_food && p_food->init(timer, isShowNativeRemainPopup)) {
		p_food->autorelease();
	}
	else {
		delete p_food;
		p_food = nullptr;
	}
	return p_food;
}

bool MSRecord::init(float timer, bool isShowNativeRemainPopup, bool isStartRecord) {
	if (!cocos2d::ui::Layout::init()) {
		return false;
	};
	is_record_stop = false;
	time_record = 0.0f;
	_record_duration = timer;
	text_alert = cocos2d::ui::Text::create("recording...", "fonts/monkeystories_ultra.ttf", 25);
	this->addChild(text_alert);
	text_alert->setPositionY(-120);
	text_alert->setColor(Color3B::GRAY);
	text_alert->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.5f, 1.15f), ScaleTo::create(0.5f, 1.0f), nullptr), 1000));
	text_alert->setVisible(false);
	this->schedule(schedule_selector(MSRecord::updateCountTimeRecord));
	//MSRecordBridge::startRecord();
	this->runAction(Sequence::create(DelayTime::create(_record_duration), CallFunc::create([=] {
		if (!is_record_stop) {
			this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
			is_record_stop = true;
			//MSRecordBridge::stopRecord();
			text_alert->setString("stopRecord");
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
				if (delegate) {
					delegate->onFinishRecord(RECORD_DONE);
				}
			}), nullptr));
		}
	}), nullptr));
	return true;
}

void MSRecord::startPlay(bool _is_play_once) {
	text_alert->setString("playing...");
	CCLOG("hoisunglog - onPlay2");
	//MSRecordBridge::playAudioRecord();
	this->runAction(Sequence::create(DelayTime::create(time_record + 0.1f), CallFunc::create([=] {
		text_alert->setString("done...");
	}), DelayTime::create(0.5f), CallFunc::create([=] {
		if (delegate) {
			delegate->onFinishRecord(PLAY_DONE);
		}
		this->removeFromParent();
	}), nullptr));
}

void MSRecord::stopPlay(){
    
}

void MSRecord::stopRecord() {
	if (!is_record_stop) {
		this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
		is_record_stop = true;
		/*MSRecordBridge::stopRecord();*/
		text_alert->setString("");
		this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
			if (delegate) {
				delegate->onFinishRecord(RECORD_DONE);
			}
		}), nullptr));
	}
}

void MSRecord::updateCountTimeRecord(float dt) {
	time_record += dt;
}

void MSRecord::setDelegate(MSRecordDelegate *_delegate) {
	delegate = _delegate;
}

MSRecord::PERMISSION_STATUS MSRecord::getPermissionStatus() {
	return PERMISSION_STATUS::DENIED;
}

#endif

