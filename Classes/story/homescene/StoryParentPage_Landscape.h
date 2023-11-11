//
//  StoryParentPage_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#ifndef StoryParentPage_Landscape_hpp
#define StoryParentPage_Landscape_hpp

#include "CocoStudioMacro.h"
#include "StoryParentItem_Landscape.h"
#include "ui/CocosGUI.h"
#include "MSSidebarDelegate.h"
#include "APPopupBase.h"

using namespace cocos2d;
using namespace std;
using namespace ui;

#define key_terms_of_service_url_submit "https://www.monkeyenglish.net/en/terms-of-use-app"
#define key_terms_of_service_url_vn   "https://monkey.edu.vn/dieu-khoan-su-dung"
#define key_terms_of_service_url_th   "https://www.monkeyenglish.net/th/tips-for-parents/terms-of-service-th.html"
#define key_terms_of_service_url_id   "https://www.monkeyenglish.net/id/tips-for-parents/masa-waktu-layanan-.html"
#define key_terms_of_service_url_en   "https://www.monkeyenglish.net/en/terms-of-use"

#define key_privacy_policy_url_submit "https://www.monkeyenglish.net/en/policy-app"
#define key_privacy_policy_url_vn   "http://www.vmonkey.vn/privacy-policy.html"
#define key_privacy_policy_url_th   "https://www.monkeyenglish.net/th/tips-for-parents/privacy-policy-th.html"
#define key_privacy_policy_url_id   "https://www.monkeyenglish.net/id/tips-for-parents/kebijakan-privasi.html"
#define key_privacy_policy_url_en   "https://www.monkeyenglish.net/en/policy"
class StoryParentPage_Landscape : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryParentItemDelegate {

public:
    CREATE_FUNC(StoryParentPage_Landscape);
    static StoryParentPage_Landscape * createPage();
    void changeTextDisplayLang();

    void showCalculator();
    void goToLicensePage();
    void goToAccountProfilePage();
    void onBack(cocos2d::Ref * sender);
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    void onResetFree(cocos2d::Ref * sender);

    bool didLoadFromCSB() override;
    void onEnter() override;
    void onClose();
    CC_SYNTHESIZE(ms::SidebarDelegate*, _delegate, Delegate);
    void onSelectItem(const int & tag, const std::string & name) override;
    void onDeleteAccount();
    void changeDisplayLanguage() override;

    cocos2d::ui::Layout * _content;
    Text *title;
    
    void displayItem();
    string classes_name;
    cocos2d::ui::ListView *list_section;

    bool _not_select_display_language;
    void showListDeviceView();
    void showListLicenseView();
};

CREATE_READER_CLASS(StoryParentPage_LandscapeReader, StoryParentPage_Landscape);

#endif /* StoryParentPage_Landscape_hpp */
