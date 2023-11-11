/*
*Author: Dat Hoang
*Created: 06/15/2022
*/
#pragma once

#include "cocostudiomacro2.h"
#include "MKDefinitions.h"

class SplashScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

    CREATE_FUNC(SplashScene);

    static cocos2d::Scene* createScene();

    static SplashScene* createView();//1.

    void pushEventTracking();

private:

    void didLoadFromCSB();//2.

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

    void onPageTurn();

    void onBtnContinueClicked(cocos2d::Ref* sender);

    void OnClickMonkeyPage1(cocos2d::Ref* sender);

    void OnClickChildrenPage2(cocos2d::Ref* sender);

    void OnClickMonkeyPage2(cocos2d::Ref* sender);

    void OnClickDiscount50(cocos2d::Ref* sender);

    void OnClickChildrenPage3(cocos2d::Ref* sender);

    void OnClickCoursePage3(cocos2d::Ref* sender);

    void onBtnPage0Clicked(cocos2d::Ref* sender);

    void onBtnPage1Clicked(cocos2d::Ref* sender);

    void onBtnPage2Clicked(cocos2d::Ref* sender);

    void onChangeDiscountPriceByCountry();

    void changePage();

    void scheduleChangePage();

    void onExit() override;

    void EndPage1();
    void EndPage2();
    void EndPage3();

private:
    //khai bao bien
    std::string _scheduleChangePageName = "ChangePage";
    int _currentPageIndex = 0;
    cocos2d::ui::PageView* _pageView;
    cocos2d::ui::ImageView* _navigation;
    int _timeOnScreen = 0;
    void pushEventTrackingChooseAcountType();
    bool isPushEvent = false;
    bool _isViewMs02 = false;
    bool _isViewMs03 = false;




    int _timeOnPage1 = 0;
    int _totalTimeOnPage1 = 0;
    bool _isClickImageMonkeyPage1 = false;

    int _timeOnPage2 = 0;
    int _totalTimeOnPage2 = 0;
    bool _isClickImageBabyPage2 = false;
    bool _isClickImageMonkeyPage2 = false;
    bool _isClickImageDiscount = false;

    int _timeOnPage3 = 0;
    int _totalTimeOnPage3 = 0;
    bool _isClickImageBabyPage3 = false;
    bool _isClickImageCourse = false;
};

CREATE_READER(SplashScene);

