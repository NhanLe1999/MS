///////////////////////////////////////////////////////////////////////
//  MSAISpeakingSharePopup_cpp
//  Implementation of the Class MSAISpeakingSharePopup_cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MSAISpeakingSharePopup.h"
#include "MJDefinitions.h"
#include <lib/logger/MSEventLogger.h>
#include <mjlib/Common.h>

USING_NS_CC;
INITIALIZE_READER_INSTANCE(MSAISpeakingSharePopupReader);

std::once_flag ms_collection_ai_speaking_share_popup;
cocos2d::Data ms_collection_ai_speaking_share_popup_node_data;

MSAISpeakingSharePopup* MSAISpeakingSharePopup::createItem() {
    std::call_once(ms_collection_ai_speaking_share_popup, [] {
        REGISTER_CSB_READER(MSAISpeakingSharePopup);        
        ms_collection_ai_speaking_share_popup_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/AISpeaking/AISpeakingShareVideoPopup.csb");
    });
    
    auto node = reinterpret_cast<MSAISpeakingSharePopup*>(CSLoader::createNodeWithVisibleSize("csb/new_ui/AISpeaking/AISpeakingShareVideoPopup.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

MSAISpeakingSharePopup* MSAISpeakingSharePopup::showPopup(ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate,Node* scene, int z_order)
{
    CRASH_AUTO_LOG;
    mj::helper::deleteWaitView();

    auto mMSAISpeakingSharePopup = MSAISpeakingSharePopup::createItem();

    if (!mMSAISpeakingSharePopup)
    {
        CCLOG("MSAISpeakingSharePopup::show| cannot parse .csb file.");

        return nullptr;
    }
    
    mMSAISpeakingSharePopup->setName("MSAISpeakingSharePopup");
    mMSAISpeakingSharePopup->setVisible(false);
    Node* sceneTemp = scene;
    if (!sceneTemp)
    {
        sceneTemp = DIRECTOR->getRunningScene();

    }
    
    sceneTemp->addChild(mMSAISpeakingSharePopup, z_order);
    mMSAISpeakingSharePopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mMSAISpeakingSharePopup->setPosition(Vec2(sceneTemp->getContentSize().width / 2, sceneTemp->getContentSize().height / 2));
    mMSAISpeakingSharePopup = utils::findChild<MSAISpeakingSharePopup*>(sceneTemp, "MSAISpeakingSharePopup");
    if (aISpeakingSharePopupDelegate)
        mMSAISpeakingSharePopup->setDelegate(aISpeakingSharePopupDelegate);
    if (mMSAISpeakingSharePopup)
    {
        int widthSizeFinalOfPopup = sceneTemp->getContentSize().width*0.8;
        float scaleFinal = widthSizeFinalOfPopup / mMSAISpeakingSharePopup->getContentSize().width;
        
        mMSAISpeakingSharePopup->setScale(scaleFinal);
        mMSAISpeakingSharePopup->setVisible(true);
        //mMSAISpeakingSharePopup->runAction(ScaleTo::create(0.5f, scaleFinal));
    }
    return mMSAISpeakingSharePopup;
}


void MSAISpeakingSharePopup::didLoadFromCSB()
{
    initialView();
    ui::Helper::doLayout(this);
}

void MSAISpeakingSharePopup::onEnter()
{
    CRASH_AUTO_LOG;
    Layout::onEnter();
    didLoadFromCSB();
}

void MSAISpeakingSharePopup::onExit()
{
    Layout::onExit();
}

void MSAISpeakingSharePopup::onCloseButtonClick(cocos2d::Ref* sender)
{
    _delegate ? _delegate->onCloseButtonClick(sender) : (void) nullptr;
    
    if(_onExitCallback) {
        _onExitCallback();
    }
    Layout::onExit();
    this->removeFromParent();
}

void MSAISpeakingSharePopup::onShareButtonClicked(cocos2d::Ref* sender)
{
    _delegate ? _delegate->onShareButtonClicked(sender) : (void) nullptr;
    if(_shareCallback) {
        _shareCallback();
    }
    //Layout::onExit();
    //this->removeFromParent();
}

void MSAISpeakingSharePopup::onFinishButtonClicked(cocos2d::Ref* sender)
{
    _delegate ? _delegate->onFinishButtonClicked(sender) : (void) nullptr;
    if(_finishCallback) {
        _finishCallback();
    }
    Layout::onExit();
    this->removeFromParent();
}


void MSAISpeakingSharePopup::initialView()
{

    _close_button = utils::findChild<Button*>(this, "close_button");
    _finish_button = utils::findChild<Button*>(this, "finish_button");
    _share_button = utils::findChild<Button*>(this, "share_button");
    _lb_share_video_title = utils::findChild<Text*>(this, "lb_share_video_title");
    _text_share = utils::findChild<Text*>(this, "text_share");
    _text_finish = utils::findChild<Text*>(this, "text_finish");
    _background_overlay = utils::findChild<Layout*>(this, "background_overlay");
    _background_overlay->setContentSize(_background_overlay->getContentSize()*2);
    _background_blue = utils::findChild<Layout*>(this, "background_blue");
    _background_white = utils::findChild<Layout*>(this, "background_white");

    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _lb_share_video_title->setFontName("fonts/leelawdb.ttf") : _lb_share_video_title->setFontName("fonts/Montserrat-ExtraBold.ttf");
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _text_share->setFontName("fonts/leelawdb.ttf") : _text_share->setFontName("fonts/Montserrat-ExtraBold.ttf");
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _text_finish->setFontName("fonts/leelawdb.ttf") : _text_finish->setFontName("fonts/Montserrat-ExtraBold.ttf");
    _lb_share_video_title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.share.description.text").c_str());
    _text_share->setString(LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.share.text").c_str());
    _text_finish->setString(LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.share.finish.text").c_str());
}

cocos2d::ui::Widget::ccWidgetClickCallback MSAISpeakingSharePopup::onLocateClickCallback(const std::string& callback_name)
{
    CRASH_AUTO_LOG;
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSAISpeakingSharePopup, onCloseButtonClick),
        CLICK_MAP(MSAISpeakingSharePopup, onShareButtonClicked),
        CLICK_MAP(MSAISpeakingSharePopup, onFinishButtonClicked),
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
