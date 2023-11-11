///////////////////////////////////////////////////////////////////////
//  MSAISpeakingPermissionPopup_cpp
//  Implementation of the Class MSAISpeakingPermissionPopup_cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MSAISpeakingPermissionPopup.h"
#include "MJDefinitions.h"
#include <lib/logger/MSEventLogger.h>
#include <mjlib/Common.h>

USING_NS_CC;
INITIALIZE_READER(MSAISpeakingPermissionPopup);

std::once_flag ms_collection_ai_speaking_permissionPopup;
cocos2d::Data ms_collection_ai_speaking_permission_popup_node_data;

MSAISpeakingPermissionPopup* MSAISpeakingPermissionPopup::createItem(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate) {
    std::call_once(ms_collection_ai_speaking_permissionPopup, [] {
        REGISTER_CSB_READER(MSAISpeakingPermissionPopup);
        ms_collection_ai_speaking_permission_popup_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/AISpeaking/AISpeakingAskPermissionPopup.csb");
    });
    
    auto node = reinterpret_cast<MSAISpeakingPermissionPopup*>(CSLoader::createNode(ms_collection_ai_speaking_permission_popup_node_data));
    if (node) {
        node->didLoadFromCSB(aISpeakingPermissionPopupDelegate);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

MSAISpeakingPermissionPopup* MSAISpeakingPermissionPopup::show(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate,Node* scene, int z_order)
{
  
    if(utils::findChild<MSAISpeakingPermissionPopup*>(scene, "MSAISpeakingPermissionPopup"))
        return nullptr;
    CRASH_AUTO_LOG;
    mj::helper::deleteWaitView();

    auto mSAISpeakingPermissionPopup = MSAISpeakingPermissionPopup::createItem(aISpeakingPermissionPopupDelegate);
    if (!mSAISpeakingPermissionPopup)
    {
        CCLOG("MSAISpeakingPermissionPopup::show| cannot parse .csb file.");

        return nullptr;
    }
    mSAISpeakingPermissionPopup->setName("MSAISpeakingPermissionPopup");
    if (!scene)
    {
        scene = DIRECTOR->getRunningScene();
    }
        if(auto alert = utils::findChild(scene, "MSAISpeakingPermissionPopup"))
            return nullptr;
        scene->addChild(mSAISpeakingPermissionPopup, z_order);
        mSAISpeakingPermissionPopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        mSAISpeakingPermissionPopup->setPosition(Vec2(scene->getContentSize().width / 2, scene->getContentSize().height / 2));
        mSAISpeakingPermissionPopup = utils::findChild<MSAISpeakingPermissionPopup*>(scene, "MSAISpeakingPermissionPopup");

    if (mSAISpeakingPermissionPopup)
    {
        int widthSizeFinalOfPopup = scene->getContentSize().width*0.8;
        float scaleFinal = widthSizeFinalOfPopup / mSAISpeakingPermissionPopup->getContentSize().width;
        
        mSAISpeakingPermissionPopup->setScale(scaleFinal);
        mSAISpeakingPermissionPopup->setVisible(true);

    }
    return mSAISpeakingPermissionPopup;
}


void MSAISpeakingPermissionPopup::didLoadFromCSB(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate)
{
    if(aISpeakingPermissionPopupDelegate)
        this->setDelegate(aISpeakingPermissionPopupDelegate);

    initialView();
    ui::Helper::doLayout(this);
}

void MSAISpeakingPermissionPopup::onEnter()
{
    CRASH_AUTO_LOG;
    Layout::onEnter();
}

void MSAISpeakingPermissionPopup::onExit()
{
    Layout::onExit();
}

void MSAISpeakingPermissionPopup::onClose(cocos2d::Ref* sender)
{
    _delegate ? _delegate->onClose(sender) : (void) nullptr;
    this->removeFromParent();
}

void MSAISpeakingPermissionPopup::onAccept(cocos2d::Ref* sender)
{
    _delegate ? _delegate->onAccept(sender) : (void) nullptr;
    this->removeFromParent();
}

void MSAISpeakingPermissionPopup::initialView()
{
    _btn_close = utils::findChild<Button*>(this, "button_close");
    _btn_accept = utils::findChild<Button*>(this, "button_accept");
    _text_title = utils::findChild<Text*>(this, "text_title");
    _text_description = utils::findChild<Text*>(this, "text_description");
    _text_accept = utils::findChild<Text*>(this, "text_accept");

    _background_overlay = utils::findChild<Layout*>(this, "background_overlay");
    _blue_background = utils::findChild<Layout*>(this, "blue_background");
    _white_image = utils::findChild<Layout*>(this, "white_image");
    std::string rickText =  LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.text").c_str();
    std::string acceptText =  LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.accept.text").c_str();
    std::string titleText =  LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text").c_str();
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  _text_accept->setFontName("fonts/leelawdb.ttf") : _text_accept->setFontName("fonts/Montserrat-ExtraBold.ttf");
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  _text_title->setFontName("fonts/leelawdb.ttf") : _text_title->setFontName("fonts/Montserrat-ExtraBold.ttf");
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  _text_description->setFontName("fonts/leelawdb.ttf") : _text_description->setFontName("fonts/Montserrat-ExtraBold.ttf");
    _text_accept->setString(acceptText);
    _text_title->setString(titleText);

    auto notiText = cocos2d::ui::RichText::createWithXML(rickText);
    if (_text_description)
    {
        _text_description->getParent()->addChild(notiText);
        notiText->setAnchorPoint(_text_description->getAnchorPoint());
        notiText->setPosition(_text_description->getPosition());
        notiText->setContentSize(_text_description->getContentSize());
        notiText->ignoreContentAdaptWithSize(false);
        notiText->setHorizontalAlignment(RichText::HorizontalAlignment::CENTER);
        _text_description->setVisible(false);
        notiText->setName("notiText");
        notiText->setFontSize(27);
    }
}

void MSAISpeakingPermissionPopup::onChangeView()
{
}

cocos2d::ui::Widget::ccWidgetClickCallback MSAISpeakingPermissionPopup::onLocateClickCallback(const std::string& callback_name)
{
    CRASH_AUTO_LOG;
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSAISpeakingPermissionPopup, onClose),
        CLICK_MAP(MSAISpeakingPermissionPopup, onAccept),
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
