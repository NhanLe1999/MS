//
//  MJFullLicenseView.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJFullLicenseView_hpp
#define MJFullLicenseView_hpp

#include "cocostudiomacro2.h"
#include "LicenseInfoModel.h"
using namespace cocos2d;
using namespace ui;

namespace license { struct Package; }

class MJFullLicenseViewDelegate {
public:
    virtual void onFullLicenseViewFreeUse() {};
    virtual void onFullLicenseonRemoveDevice(license::Package) {};
    virtual void onFullLicenseonSubscribe() {};
};

class MJFullLicenseView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJFullLicenseView);
    static MJFullLicenseView * createView(license::Package lic);
    void setDelegate(MJFullLicenseViewDelegate *delegate);
private:
    void didLoadFromCSB(license::Package lic);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJFullLicenseViewDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    void onSubscribe(cocos2d::Ref *sender);
    void onFreeUse(cocos2d::Ref *sender);
    void onRemoveDevice(cocos2d::Ref *sender);
    void onShow();
    void onHide();
    bool is_showed = false;
    license::Package _license;
    
};

CREATE_READER(MJFullLicenseView);

#endif
