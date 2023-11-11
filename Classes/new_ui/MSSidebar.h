//
//  MSSidebar.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSSidebar_hpp
#define MSSidebar_hpp

#include "cocostudiomacro2.h"
#include "StoryAlert.h"
#include "MJDefinitions.h"
#include "MSSidebarDelegate.h"

namespace ms {
    class SidebarDelegate;
}

class MSSidebar : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate {
public:
    static MSSidebar * defaultSidebar;
    CREATE_FUNC(MSSidebar);
    static MSSidebar * createSidebar();
    void showLanguages();
    void hideLanguages();
    void enableQR();
    void onLessionTabAutoSelect();
    void onReadTabAutoSelect();
    void onPromotionsForce();
    void onStoriesForce();
    void onLessonsForce();
    void onAudiobooksForce();
    void onQRForce();
    void onMaterialForce();
    void onSettingsForce();
    void onAccountForce(bool showCalculator = false);
    void onSubscriptionsForce();
    void onParentForce();
    void updateProfileAfterCreateProfile();
    void onAISpeakingSelect(); 
    void onAISpeakingForce();
    void SetBackGroundButtonPostion(ms::HomeTab tabs);
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertClose(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void onSignInF2P();
    void onShowParentTaskBarF2P();
    void onMaterial(cocos2d::Ref *sender);
    void onAccount(cocos2d::Ref *sender);
    void onStories(cocos2d::Ref * sender);
    void onSupport(cocos2d::Ref* sender);
    void onLessons(cocos2d::Ref * sender);
    void onSubscriptions(cocos2d::Ref * sender);
    void onSettings(cocos2d::Ref * sender);
    void onLanguages(cocos2d::Ref * sender);
    void onAudiobooks(cocos2d::Ref * sender);
    void onSelectLanguage(cocos2d::Ref * sender);
    void onFree(cocos2d::Ref * sender);
    void onPromotions(cocos2d::Ref * sender);
    void onFavourite(cocos2d::Ref * sender);
    void onLock(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    void Close(cocos2d::Ref* sender);
    void onAISpeaking(cocos2d::Ref* sender);
    void showHidenMaterialButton(bool isShow);
    void ResetAndCreatSpineForAiSpeakingButton(std::string creatSequenceName, bool loop, bool playAudio = false);
    void onLicense(cocos2d::Ref * sender);
    void onQR(cocos2d::Ref * sender);
	void onProfile(cocos2d::Ref * sender);
	void onSwitchProfile(cocos2d::Ref * sender);
    bool shouldShowQR();
    void loadDefaultAvatar(ap::Profile profile);
    
	void showProfileMenu();
	void hideProfileMenu();
	void showNumberNotification();
#pragma mark - Helper Function
    int moveBackground(cocos2d::ui::Button * button);
    
//    std::vector<cocos2d::ui::Button *> _buttons;
    std::vector<cocos2d::ui::Button *> _language_buttons;
    std::string _current_language_code;
    
    CC_SYNTHESIZE(ms::SidebarDelegate *, _delegate, Delegate);
	bool is_show_profile_menu;
    bool is_showing_languages;
    bool _is_animating = false;
    std::unordered_map<std::string, std::tuple<std::string, std::string>> languages_info;
    void changeTextDisplayLang();
    
    cocos2d::ui::Button * _current_button = nullptr;
    cocos2d::ui::Button * _saved_button = nullptr, *_onSaveUnlockButton = nullptr;
    std::vector<cocos2d::ui::Button *> _top_buttons;
    std::vector<cocos2d::ui::Button *> _bottom_buttons;
};

CREATE_READER(MSSidebar);

#endif /* MSSidebar_hpp */
