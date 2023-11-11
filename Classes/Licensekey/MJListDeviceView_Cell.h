//
//  MJListDeviceView_Cell.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJListDeviceView_Cell_hpp
#define MJListDeviceView_Cell_hpp

#include "cocostudiomacro2.h"
#include "LicenseInfoModel.h"
using namespace cocos2d;
using namespace ui;

class MJListDeviceView_CellDelegate {
public:
    virtual void onDeActiveDeviceCell(license::LicenseDeviceInfo device_info) {};
};

class MJListDeviceView_Cell : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJListDeviceView_Cell);
    static MJListDeviceView_Cell * createCell(license::LicenseDeviceInfo device_info, bool show_btremove);
    void setDelegate(MJListDeviceView_CellDelegate *delegate);
private:
    void didLoadFromCSB(license::LicenseDeviceInfo device_info, bool show_btremove);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJListDeviceView_CellDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onRemove(cocos2d::Ref *sender);
    license::LicenseDeviceInfo device_info;
    bool show_btremove;
    
};

CREATE_READER(MJListDeviceView_Cell);

#endif
