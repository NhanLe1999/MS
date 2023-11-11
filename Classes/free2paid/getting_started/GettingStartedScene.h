/*
*Author: Dat Hoang
*Created: 06/15/2022
*/
#pragma once

#include "cocostudiomacro2.h"
#include "APPurchaseManager.h"
#include "ShowNewAlert.h"
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
#include "MSLanguageSelection.h"
#include "APLoginAPI.h"
#include "SplashScene.h"
#include "APLoginScreen.h"
#include "CheatMenu.h"
#include "MJDefinitions.h"

#define CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE "CLICK TYPE OF OB CHOOSE ACCOUNT TYPE"
#define TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE "TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE"

class GettingStartedScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

    CREATE_FUNC(GettingStartedScene);

    static cocos2d::Scene* createScene();

    static GettingStartedScene* createView();//1.

private:

    void didLoadFromCSB();//2.

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

    void onSignInBtnClicked(cocos2d::Ref* sender);

    void onStartNowBtnClicked(cocos2d::Ref* sender);

    void onBackBtnClicked(cocos2d::Ref* sender);
    
    void CallAPILogin();
    void APILoginCallBack(int error_code,bool isSucess, std::string msg);
    
    void CallAPILoadUpdate();
    void APILoadUpdateCallBack(bool isSucess, std::string msg);

    void CallAPIProfileList();
    void APIProfileListCallBack(bool isSucess, std::string msg);
    void showErrorAlert(std::string msg);
    void showAlertHavePaidAccount();
    void onExit() override;
    void gotoSplashPage();
    void APLoginViewPage(APLoginView::SCREEN_TYPE SCREEN_TYPE);
private:
    //khai bao bien

    int _timeStartScreen = 0;

};

CREATE_READER(GettingStartedScene);

