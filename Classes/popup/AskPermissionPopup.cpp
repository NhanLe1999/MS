#include "AskPermissionPopup.h"
#include "SpineWrapper.h"
#include "MSRecord.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "AnimationManager.h"
#include "GamePhonics_TextVideo.h"
USING_NS_CC;
INITIALIZE_READER(AskPermissionPopup);

std::once_flag mk_ask_permission_popup_flag;

AskPermissionPopup* AskPermissionPopup::instance;
void AskPermissionPopup::showNotificationPopup(std::function<void(bool success)> callback,cocos2d::Node* i_parent)
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//    if(callback) callback(false);
//    return;
//#endif
//    MJNotification::checkNotificationPermisssion([=](MJNotification::PERMISSION_STATUS status){
//
//        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//
//            switch (status)
//            {
//                case MJNotification::PERMISSION_STATUS::GRANTED:
//                    if(callback) callback(true);
//                    break;
//                case MJNotification::PERMISSION_STATUS::DENIED:
//                    AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::SHOW_NOTI,nullptr,nullptr,i_parent,true,[=]
//                    {
//                        if(callback) callback(false);
//
//                        MJNotification::leadUserToGrantPermissionSetting();
//                    });
//                    break;
//                case MJNotification::PERMISSION_STATUS::UNDETERMINED:
//
//                    AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::SHOW_NOTI,nullptr,nullptr,i_parent,true,[=]{
//
//                        MJNotification::askNotificationPermission(callback,callback);
//
//                    });
//                    break;
//                default:
//                    if(callback) callback(false);
//                    break;
//            }
//        });
//    });
}


void AskPermissionPopup::showPopup(PERMISSION_KIND type, std::function<void()> okCallback, std::function<void()> cancleCallback , cocos2d::Node* i_parent,bool isShow,std::function<void()> cocosOk, std::function<void()> callbackGameSpeaking)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    if(okCallback) okCallback();
    return;
#endif

    if(auto popAsk = utils::findChild(Director::getInstance()->getRunningScene(), "pop_askPermissionPopup"))
	{
        popAsk->removeFromParent();
	}

    if (auto layer = AskPermissionPopup::createPopup(type)) {
		layer->_onPermissionGranted = okCallback;
		layer->_onPermissionDenied = cancleCallback;
        layer->_callbackGameSpeaking = callbackGameSpeaking;
        layer->_onOkCallback =cocosOk;
		layer->setName("pop_askPermissionPopup");
        
        mj::helper::doAppearGradient(layer, 0.4f);
		auto blackLayout = Layout::create();
		blackLayout->setBackGroundColorType(BackGroundColorType::SOLID);
		blackLayout->setBackGroundColorOpacity(100);
		blackLayout->setBackGroundColor(Color3B::BLACK);

        if(i_parent!=nullptr){
			blackLayout->setContentSize(i_parent->getContentSize());
			layer->addChild(blackLayout,-1);
            i_parent->addChild(layer, INT_MAX - 1);
        } else if (auto currentScene = Director::getInstance()->getRunningScene()) {
			blackLayout->setContentSize(currentScene->getContentSize());
			layer->addChild(blackLayout,-1);
            currentScene->addChild(layer, INT_MAX - 1);
        }
        instance = layer;

        ANIMATION_MANAGER->transitionSlideIn(AnimationManager::Direction::LEFT, 0.25f, utils::findChild(layer,"background"));
    }
}



AskPermissionPopup * AskPermissionPopup::createPopup(PERMISSION_KIND type) {
    std::call_once(mk_ask_permission_popup_flag, [] {
        REGISTER_CSB_READER(AskPermissionPopup);
    });
    
    auto view = dynamic_cast<AskPermissionPopup *>(CSLoader::createNodeWithVisibleSize("csb/popup/AskPermissionPopup.csb"));
    
	if (view) {
		view->_permissionKind = type;
		view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

void AskPermissionPopup::didLoadFromCSB() {

    if(auto label = utils::findChild<ui::Text*>(this,"lb.onboard.infomation")) {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  label->setFontName("fonts/leelawdb.ttf") : label->setFontName("fonts/Montserrat-Bold.ttf");
        label->setString(LANGUAGE_MANAGER->getDisplayTextByKey(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "noti_title")));
    }
    
    auto maskMsg = utils::findChild<Text*>(this,"label.ask.for.permission");
    bool fileExits = isisExitPath();
	RichText* msg = nullptr;
	SpineWrapper* spine = nullptr;
	switch (_permissionKind)
	{
	case AskPermissionPopup::RECORD: 
	{

        if (fileExits) {
            spine = SpineWrapper::createWithPath("spine/mic/mic");
        }
        auto keyMsg = "onboard.ask.record.permission";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        keyMsg = "onboard.ask.record.permission.android";
#endif
		msg = RichText::createWithXML(LANGUAGE_MANAGER->getDisplayTextByKey(keyMsg));
		break;
	}
	case AskPermissionPopup::SHOW_NOTI:
	{
        if (fileExits) {
            spine = SpineWrapper::createWithPath("spine/bell/bell");
        }
		msg = RichText::createWithXML(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.ask.show.noti.permission"));
		break;
	}
		
	default:
		break;
	}

	msg->ignoreContentAdaptWithSize(false);
	msg->setAnchorPoint(maskMsg->getAnchorPoint());
	msg->setContentSize(maskMsg->getContentSize());
	msg->setPosition(maskMsg->getPosition());
	msg->setHorizontalAlignment(RichText::HorizontalAlignment::CENTER);
	maskMsg->getParent()->addChild(msg);
	maskMsg->removeFromParent();

	auto maskMic = utils::findChild<ImageView*>(this, "img.mic");
    if (maskMic && spine) {
        spine->playSequence("action", true);
        spine->setAnchorPoint(maskMic->getAnchorPoint());
        spine->setPosition(maskMic->getPosition());
        maskMic->getParent()->addChild(spine);
        spine->updateAnimationPosition(spine->getContentSize() / 2);
        maskMic->removeFromParent();
    }
	
}

bool AskPermissionPopup::isisExitPath(){
    if (FileUtils::getInstance()->isDirectoryExist("spine/mic") || FileUtils::getInstance()->isDirectoryExist("spine/bell")) {
        return true;
    } else{
        return false;
    }
}

void AskPermissionPopup::onEnter() {
    ui::Layout::onEnter();
}

void AskPermissionPopup::onOkButtonClicked(cocos2d::Ref * sender)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
    switch (_permissionKind) {
        case RECORD:
        {
            if (_callbackGameSpeaking)
            {
                _callbackGameSpeaking();
            }
            MSRecord::askPermission(_onPermissionGranted, _onPermissionDenied);
            break;
        }
        case SHOW_NOTI:
        {
            
            if(_onOkCallback)
                this->_onOkCallback();
            break;
        }
    }
	if (this) {
        //onExit();
        this->removeFromParent();
	}
#endif
}

cocos2d::ui::Widget::ccWidgetClickCallback AskPermissionPopup::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
	   CLICK_MAP(AskPermissionPopup, onOkButtonClicked),
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return cocos2d::ui::Widget::ccWidgetClickCallback();
}
void AskPermissionPopup::loadCancelButton(std::function<void()> cancelCallback) {
    if (instance == nullptr) return;
    auto okButton = utils::findChild<Button*>(instance, "Button");
    auto cancelButton = dynamic_cast<Button*>(okButton->clone());
    Node* parent = okButton->getParent();
    parent->addChild(cancelButton);

    cancelButton->setPosition(Vec2(parent->getBoundingBox().size.width * 0.3, parent->getBoundingBox().size.height * 0.08));
    okButton->setPosition(Vec2(parent->getBoundingBox().size.width * 0.7, parent->getBoundingBox().size.height * 0.08));
    if (auto cancelLabel = utils::findChild<Text*>(cancelButton,"Ok")) {
        cancelLabel->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_cancel_text"));
    }
    cancelButton->setContentSize(okButton->getContentSize());
    cancelButton->addClickEventListener([=](Ref*) {
        if (cancelCallback) cancelCallback();
        instance->removeFromParent();
    });
}
