//
//  MJOverLicenseView.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJOverLicenseView_hpp
#define MJOverLicenseView_hpp

#include "cocostudiomacro2.h"
using namespace cocos2d;
using namespace ui;

class MJOverLicenseViewDelegate {
public:
    virtual void onOverLicenseViewFreeUse() {};
    virtual void onOverLicenseViewListDevice() {};
    virtual void onOverLicenseViewSubscribe() {};
};

class MJOverLicenseView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJOverLicenseView);
    static MJOverLicenseView * createView(std::string message);
    void setDelegate(MJOverLicenseViewDelegate *delegate);
private:
    void didLoadFromCSB(std::string message);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJOverLicenseViewDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    void onSubscribe(cocos2d::Ref *sender);
    void onFreeUse(cocos2d::Ref *sender);
    void onViewListDevice(cocos2d::Ref *sender);
    void onShow();
    void onHide();
    bool is_showed = false;
    std::string message;
};

CREATE_READER(MJOverLicenseView);

#endif
