//
//  MSLicenseLimitPopup.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#include "MSLicenseLimitPopup.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(MSLicenseLimitPopup);

std::once_flag ms_license_limit_popup_reader_flag;

MSLicenseLimitPopup * MSLicenseLimitPopup::createView() {
    std::call_once(ms_license_limit_popup_reader_flag, [] {
        REGISTER_CSB_READER(MSLicenseLimitPopup);
    });
    
    auto csb = "csb/new_ui/LicenseLimitNotice.csb";
    if (LANGUAGE_MANAGER->getDisplayLangId() == 1) {
        csb = "csb/new_ui/LicenseLimitNotice_en.csb";
    }
    auto view = reinterpret_cast<MSLicenseLimitPopup *>(CSLoader::createNodeWithVisibleSize(csb));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

void MSLicenseLimitPopup::didLoadFromCSB() {
    
}

void MSLicenseLimitPopup::onEnter() {
    ui::Layout::onEnter();
}

void MSLicenseLimitPopup::onOK(cocos2d::Ref *sender) {
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MSLicenseLimitPopup::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSLicenseLimitPopup, onOK)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
