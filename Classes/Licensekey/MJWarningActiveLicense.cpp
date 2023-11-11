//
//  MJWarningActiveLicense.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/16/18.
//

#include "MJWarningActiveLicense.h"

USING_NS_CC;
INITIALIZE_READER(MJWarningActiveLicense);

std::once_flag mj_warning_active_license_reader_flag;

MJWarningActiveLicense * MJWarningActiveLicense::createView(const std::string & title, const std::string & message, const bool & one_button, std::function<void()> callback) {
    std::call_once(mj_warning_active_license_reader_flag, [] {
        REGISTER_CSB_READER(MJWarningActiveLicense);
    });
    
    auto view = reinterpret_cast<MJWarningActiveLicense *>(CSLoader::createNodeWithVisibleSize("csb/MJWarningActive.csb"));
    if (view) {
        view->didLoadFromCSB(title, message, one_button, callback);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

void MJWarningActiveLicense::didLoadFromCSB(const std::string & title, const std::string & message, const bool & one_button, std::function<void()> callback) {
    _callback = callback;
    auto bg = this->getChildByName("color_layout");
    if (bg) {
        bg->setOpacity(0xcc);
    }
    
    auto text_title = utils::findChild<ui::Text *>(this, "title");
    text_title->setString(title);
    
    auto text_message = utils::findChild<ui::Text *>(this, "msg");
    text_message->setString(message);
}

void MJWarningActiveLicense::onEnter() {
    ui::Layout::onEnter();
}

void MJWarningActiveLicense::onOK(Ref *sender) {
    if (_callback) {
        _callback();
    }
    
    this->removeFromParent();
}

void MJWarningActiveLicense::onCancel(Ref * sender) {
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MJWarningActiveLicense::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJWarningActiveLicense, onOK),
        CLICK_MAP(MJWarningActiveLicense, onCancel)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
