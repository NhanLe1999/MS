
//  APProfileAgeF2p.cpp
//  MJStory
//
//  Created by Cuong Cao Nguyen on 6/15/2022
//

#include "MSLanguageSelection.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "MSIntro.h"
#include "CleverTapManager.h"
#include "HelperManager.h"
#include "APProfileAgeF2p.h"
#include "MSHome.h"
#include "MSInstallPromotion.h"
#include "StoryWelcomePopUp.h"
#include "APUpdateProfileAPI.h"
#include "MSFree2PaidPermissionPopup.h"
#include "CameraHelper.h"

USING_NS_CC;

std::once_flag ms_f2p_cuong_flag;
void MSFree2PaidPermissionPopup::onChangeView()
{
    _text_description = mSFree2PaidPermissionPopup->_text_description;

    if (_text_description)
    {
        std::string rickText = LANGUAGE_MANAGER->getDisplayTextByKey("key.free2paid.popup.notification").c_str();
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _text_description->setFontName("fonts/leelawdb.ttf") : _text_description->setFontName("fonts/Montserrat-Black.ttf");

        auto notiText = cocos2d::ui::RichText::createWithXML(rickText);
        if (auto notiText1 = utils::findChild(_text_description->getParent(), "notiText"))
        {
            notiText1->removeFromParent();
        }
        _text_description->getParent()->addChild(notiText);
        notiText->setAnchorPoint(_text_description->getAnchorPoint());
        notiText->setPosition(_text_description->getPosition() - Vec2(0, 45));
        notiText->setContentSize(_text_description->getContentSize() * 0.9);
        notiText->ignoreContentAdaptWithSize(false);
        notiText->setHorizontalAlignment(RichText::HorizontalAlignment::CENTER);
        _text_description->setVisible(false);
    }
}

MSFree2PaidPermissionPopup* MSFree2PaidPermissionPopup::show()
{
    mSFree2PaidPermissionPopup = (MSFree2PaidPermissionPopup*)MSAISpeakingPermissionPopup::show(this);

    if (auto image = utils::findChild<Layout*>(mSFree2PaidPermissionPopup, "background_icon"))
    {
        image->setVisible(false);
    }
    
    if (auto line = utils::findChild<ui::ImageView*>(mSFree2PaidPermissionPopup, "line"))
    {
        line->setVisible(true);
    }

    mSFree2PaidPermissionPopup->setScaleX(0.9);
    mSFree2PaidPermissionPopup->setScaleY(0.8);

    return mSFree2PaidPermissionPopup;
}

void MSFree2PaidPermissionPopup::onAccept(Ref* sender)
{
    _delegate ? _delegate->onAccept(sender) : (void) nullptr;
    requestPermissionNotifications();
    this->removeFromParent();

    CCLOG("onAccept");
}

void MSFree2PaidPermissionPopup::onClose(Ref* sender)
{
    CCLOG("onClose");
}

void MSFree2PaidPermissionPopup::requestPermissionNotifications()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CameraHelper::GetInstance()->requestNotifyPermision();
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity","requestPermissionNotificationAndroid");
#endif
}

bool MSFree2PaidPermissionPopup::checkPermissionNotifications()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
       return CameraHelper::GetInstance()->checkNotifyPermision();
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return JniHelper::callStaticBooleanMethod("org.cocos2dx.cpp.AppActivity","checkPermissionNotificationAndroid");
#endif
       return true;
}
