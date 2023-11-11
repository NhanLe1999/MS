//
//  ViralReceiveGift.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#include "ViralReceiveGift.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "MJLicenseAlert.h"
#include "LicenseInfoModel.h"
#include "MJWaitingHUD.h"
#include "MJAPISendMail.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"

INITIALIZE_READER(ViralReceiveGift);

ViralReceiveGift* ViralReceiveGift::createPopup(){

    auto p = static_cast<ViralReceiveGift *>(CSLoader::createNodeWithVisibleSize("csb/ViralReceiveGift.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void ViralReceiveGift::show() {
    auto popup = ViralReceiveGift::createPopup();
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX - 1);
}

void ViralReceiveGift::didLoadFromCSB(){
    
    textField_phone = utils::findChild<ui::TextField*>(this, "phone");
    textField_phone->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44 ));
    
}

void ViralReceiveGift::onEnter(){
    Layout::onEnter();
    utils::findChild<ui::Text*>(this, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.receive.popup.title").c_str());
    utils::findChild<ui::Text*>(this, "title_sub")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.receive.popup.content").c_str());
    utils::findChild<ui::Text*>(this, "bttitle")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.receive.popup.button").c_str());
    textField_phone->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("viral.sub.popup.phone").c_str());
    
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralReceiveGift.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    runani->play("show", false);
}

void ViralReceiveGift::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void ViralReceiveGift::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

void ViralReceiveGift::onClose(Ref* sender){
     auto ani_name = "hide";
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralReceiveGift.csb"));
    runani->setTag(1010);
    this->runAction(runani);
    runani->play(ani_name, false);
    runani->setLastFrameCallFunc([=]{
        this->removeFromParent();
    });
}

#include "MSIntroGrade.h"
#include "MJAPIVerifyViral.h"
#include "StoryAlert.h"
void ViralReceiveGift::onReceive(Ref* sender){
    CCLOG("receive now");
    mj::helper::playButtonFX();
    
    auto phone_number = textField_phone->getString();
    if(phone_number.empty()){
          MJLicenseAlert::showAlert(0, is_vn?"Hãy nhập số điện thoại của bạn":"Please enter your phone number", "", is_vn?"Đóng":"Close", nullptr);
        return;
    }
    
    //app đã có đk số điện thoại
    //kiểm tra nếu số đt trùng thì pass, khác thì yêu cầu nhập lại
//    if(!MJDEFAULT->getStringForKey("user.phone").empty() && MJDEFAULT->getStringForKey("user.phone") != phone_number){
//         MJLicenseAlert::showAlert(0, is_vn?"Vui lòng nhập đúng số điện thoại đã đăng kí":"Please enter the registed number", "", is_vn?"Đóng":"Close", nullptr);
//        return;
//    }else{
//        MJDEFAULT->setStringForKey("user.phone", phone_number);
//    }
    
    //TODO
    //verify số đt
    mj::MJAPIVerifyViral::call(phone_number, [=](bool is_success, std::string msg){
        //refresh app
        if(is_success){
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
                MJDEFAULT->setBoolForKey(key_first_api_called, true);
                MJDEFAULT->setBoolForKey("key.is.refer", true);
                MJDEFAULT->setIntegerForKey(key_freetime_expire , 0);
                Director::getInstance()->replaceScene(MSIntroGrade::createScene());
            });
        }else{
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
                if(msg == ""){
                    StoryAlert::showAlert( LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error"), "OK", "") ;
                }else{
                    StoryAlert::showAlert(msg, "OK", "");
                }
            });
        }
    });

}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback ViralReceiveGift::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(ViralReceiveGift, onClose),
        CLICK_MAP(ViralReceiveGift, onReceive),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
