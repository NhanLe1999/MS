//
//  DatePicker.cpp
//  VN Stories
//
//  Created by AnhB Nguyen on 3/9/19.
//

#include "DatePicker.h"
#include "cocos2d.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

USING_NS_CC;
INITIALIZE_READER(DatePicker);
std::once_flag DatePicker_reader;

DatePicker* DatePicker::showPicker(){
    std::call_once(DatePicker_reader, [] {
        REGISTER_CSB_READER(DatePicker);
    });
    auto csb_name = "csb/new_ui/intro/DatePicker.csb";
    auto view = reinterpret_cast<DatePicker *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (view) {
        view->didLoadFromCSB();
    } else {
        CC_SAFE_DELETE(view);
    }
    
    return view;
}

void DatePicker::didLoadFromCSB(){
    date_scroll = utils::findChild<ListView*>(this, "Date");
    date_scroll->setMagneticType(ui::ListView::MagneticType::CENTER);
    date_scroll->setMagneticAllowedOutOfBoundary(false);
    date_scroll->setScrollBarEnabled(false);

    for(int i = 0; i < 33; i++){
        auto layout = Layout::create();
        layout->setContentSize(Size(56, 60));
        layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
        layout->setCascadeOpacityEnabled(true);
        date_scroll->addChild(layout);
        
        auto date = StringUtils::format("%i", i);
        if(i < 1 || i > 31){
            date = " ";
        }
        auto text = Text::create(date, "fonts/Montserrat-Medium.ttf", 30);
        text->setTextColor(Color4B::BLACK);
        layout->setTag(i);
//        date_scroll->setScrollBarOpacity(0);
        text->setPosition(layout->getContentSize()/2);
        layout->addChild(text);
    }
    date_scroll->jumpToItem(14, Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE);
    
    month_scroll = utils::findChild<ListView*>(this, "Month");
    month_scroll->setMagneticType(ui::ListView::MagneticType::CENTER);
    month_scroll->setMagneticAllowedOutOfBoundary(false);
    month_scroll->setScrollBarEnabled(false);
    
    for(int i = 0; i < 14; i++){
        auto layout = Layout::create();
        layout->setContentSize(Size(138, 60));
        layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
        layout->setCascadeOpacityEnabled(true);
        month_scroll->addChild(layout);
        
        auto month = StringUtils::format("Tháng %i", i);
        if(i < 1 || i > 12) {
            month = " ";
        };
        auto text = Text::create(month, "fonts/Montserrat-Medium.ttf", 30);
        layout->setTag(i);
        text->setTextColor(Color4B::BLACK);
//        month_scroll->setScrollBarOpacity(0);
        text->setPosition(layout->getContentSize()/2);
        layout->addChild(text);
        
    }
    month_scroll->jumpToItem(3, Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE);
    
    year_scroll = utils::findChild<ListView*>(this, "Year");
    year_scroll->setMagneticType(ui::ListView::MagneticType::CENTER);
    year_scroll->setMagneticAllowedOutOfBoundary(false);
    year_scroll->setScrollBarEnabled(false);
    for(int i = 1996; i < 2021; i++){
        auto layout = Layout::create();
        layout->setContentSize(Size(56, 60));
        layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
        layout->setCascadeOpacityEnabled(true);
        year_scroll->addChild(layout);
        
        auto year = StringUtils::format("%i", i);
        if(i < 1997 || i > 2019){
            year = " ";
        }
        auto text = Text::create(year, "fonts/Montserrat-Medium.ttf", 30);
        text->setTextColor(Color4B::BLACK);
        layout->setTag(i);
//        year_scroll->setScrollBarOpacity(0);
        text->setPosition(layout->getContentSize()/2);
        layout->addChild(text);
    }
    year_scroll->jumpToItem(5, Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE);
    
    utils::findChild<Layout*> (this, "btn")->addClickEventListener(CC_CALLBACK_0(DatePicker::onOk, this));
    
    auto timeline = CSLoader::createTimeline("csb/new_ui/intro/DatePicker.csb");
    this->runAction(timeline);
    timeline->play("fade_in", false);
    timeline->setLastFrameCallFunc([=]{
        this->schedule([=](float){
            auto date = date_scroll->getCenterItemInCurrentView();
            auto month = month_scroll->getCenterItemInCurrentView();
            auto year = year_scroll->getCenterItemInCurrentView();
            
            for(auto item : date_scroll->getChildren()){
                if (item == date) {
//                    item->setScale(1.2);
                    item->setOpacity(255);
                }else{
                    item->setScale(1.0);
                    item->setOpacity(100);
                }
            }
            
            for(auto item : month_scroll->getChildren()){
                if (item == month) {
//                    item->setScale(1.2);
                    item->setOpacity(255);
                }else{
                    item->setScale(1.0);
                    ((Text*)item)->setOpacity(255);
                    item->setOpacity(100);
                }
            }
            
            for(auto item : year_scroll->getChildren()){
                if (item == year) {
//                    item->setScale(1.2);
                    item->setOpacity(255);
                }else{
                    item->setScale(1.0);
                    item->setOpacity(100);
                }
            }
        }, 0.0, 100000000 , 0.0, "update");
    });
}

#include "MJAPI_RegisterInstall.h"
void DatePicker::onOk(){
    utils::findChild<Layout*> (this, "btn")->setTouchEnabled(false);
    
    //save data
    auto date = date_scroll->getCenterItemInCurrentView()->getTag();
    auto month = month_scroll->getCenterItemInCurrentView()->getTag();
    auto year = year_scroll->getCenterItemInCurrentView()->getTag();
    
    struct tm tm;
    time_t rawtime;
    time(&rawtime);
    tm = * localtime(&rawtime);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = date;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
//    long long time_stamp = timegm(&tm);
    long long time_stamp = mktime(&tm);
    CCLOG("timestamp: %lld", time_stamp);

    auto saved_name = MJDEFAULT->getStringForKey("install.name", "");
    auto saved_phone = MJDEFAULT->getStringForKey("install.phone", "");
    auto saved_mail = MJDEFAULT->getStringForKey("install.mail", "");
    auto saved_verified = MJDEFAULT->getBoolForKey("install.verified", false);
    
    mj::api::RegisterInstall::call(saved_name, saved_phone, saved_mail, time_stamp, saved_verified, [](bool, std::string) {
        
    });
    
    auto timeline = CSLoader::createTimeline("csb/new_ui/intro/DatePicker.csb");
    this->runAction(timeline);
    timeline->play("fade_out", false);
    timeline->setLastFrameCallFunc([=]{
        this->removeFromParent();
    });
}

