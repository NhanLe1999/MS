//
//  MJWaitingHUD.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/21/17.
//

#include "MJWaitingHUD.h"
#include "platform/CCPlatformConfig.h"
#include "GameData.h"
#include "LicenseInfoModel.h"
USING_NS_CC;
INITIALIZE_READER_INSTANCE(MJWaitingHUDReader);

std::once_flag mj_waiting_hud_reader;

MJWaitingHUD * MJWaitingHUD::createHUD() {
    std::call_once(mj_waiting_hud_reader, [] {
        REGISTER_READER(MJWaitingHUDReader);
    });
    
    auto hud = reinterpret_cast<MJWaitingHUD *>(CSLoader::createNodeWithVisibleSize("csb/MJLoading.csb"));
    if (hud) {
        return hud;
    }
    
    CC_SAFE_DELETE(hud);
    return nullptr;
}

void MJWaitingHUD::show(Node * blocked) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    auto hud = createHUD();
    hud->setName("mjhud");
    auto timeline = CSLoader::createTimeline("csb/MJLoading.csb");
    hud->runAction(timeline);
    timeline->play("run", true);
    if (blocked == nullptr) {
        blocked = Director::getInstance()->getRunningScene();
    }
    blocked->addChild(hud, INT_MAX - 1);
    
    if (LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
        auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(hud, "Text_1");
        if (text) {
            text->setString("Vui lòng đợi!");
        }
    }
}

void MJWaitingHUD::hide(cocos2d::Node * blocked) {
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([blocked] {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        return;
#endif
        auto b = blocked;
        if (b == nullptr) {
            b = Director::getInstance()->getRunningScene();
        }
        auto hud = utils::findChild(b, "mjhud");
        if (hud){
            hud->removeFromParent();
        }
    });
}
