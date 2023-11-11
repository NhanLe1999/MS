//
//  MJDeviceInfoAlert.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJDeviceInfoAlert_hpp
#define MJDeviceInfoAlert_hpp

#include "cocostudiomacro2.h"
using namespace cocos2d;
using namespace ui;

class MJDeviceInfoAlertDelegate {
public:
    virtual void onDeActiveDeviceCell() {};
};

class MJDeviceInfoAlert : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJDeviceInfoAlert);
    
    struct NotiAlertInfo {
        int notification_id = 0;
        std::string message = "";
        std::string url = "";
        int time_created = 0;
        std::string title = "";
    };
    
    static MJDeviceInfoAlert * createView(cocos2d::ValueMap noti_map);
    void setDelegate(MJDeviceInfoAlertDelegate *delegate);
private:
    void didLoadFromCSB(cocos2d::ValueMap noti_map);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJDeviceInfoAlertDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    void onViewListDevice(cocos2d::Ref *sender);
    void onShow();
    void onHide();
    bool is_showed = false;
    NotiAlertInfo _info;
};

CREATE_READER(MJDeviceInfoAlert);

#endif
