//
//  MSIntroLoad.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#ifndef MSIntroLoad_hpp
#define MSIntroLoad_hpp

#include "cocostudiomacro2.h"
#include "StoryAlert.h"
#include "APProfileDelegate.h"

#define MSINTROLOAD_VIEW "IntroLoadView"

enum ApiTag {
    kMSIntroLoadAlertTag_ApiRequired = 123,
    kMSIntroLoadAlertTag_ApiMaterial = 124,
    kMSIntroLoadAlertTag_ApiFisrtInstall = 125,
    kMSIntroLoadAlertTag_ApiRegister = 126,
    kMSIntroLoadAlertTag_ApiProfileList = 127,
    kMSIntroLoadAlertTag_ApiLoadUpdateInfo = 128,
    kMSIntroLoadAlertTag_ApiAllAudiobookZip = 129,
    kMSIntroLoadAlertTag_ApiStoryZip = 130,
    kMSIntroLoadAlertTag_ApiCatagoriesZip = 131,
    kMSIntroLoadAlertTag_ApiGameListZip = 132,
    kMSIntroLoadAlertTag_ApiLessonListZip = 133,
    kMSIntroLoadAlertTag_APIAISpeakingTopicsZip = 134,
    kMSIntroLoadAlertTag_APIAISpeakingItemsZip = 135,
    kMSIntroLoadAlertTag_ApiLoadUpdate = 136,


};

class GAFWrapper;

class MSIntroLoad : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate{
public:
    CREATE_FUNC(MSIntroLoad);
    static cocos2d::Scene* createScene();
    static MSIntroLoad * createView();
    
    void reloadIntro();
    
    void onFisrtInstallThumbCalled(bool is_success);

    void setPercent(double percent);

private:
    ~MSIntroLoad();
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;

    void changeLanguage();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void onLoadUpdateAPICalled(bool is_success,std::string message);
    void onLoadUpdateInfoAPICalled(bool is_success, std::string message);

    void onCheckUpdateApp(bool is_success = true);
    void onCheckUpdateGameListAPICalled(bool is_success);
    void onCheckUpdateCatagoriesAPICalled(bool is_success);
    void onCheckUpdateLessonListAPICalled(bool is_success);

    void onProfileListAPICalled(bool is_success,std::string message);
    void onAppInfoAPICalled(bool is_success, int v_server_audio, int v_server_story, int v_server_material);
    void onRegisterAPICalled(bool is_success);
    void onHomeAPICalled(bool is_success);
    void onFisrtInstallCall(bool is_success);
    void onAllAudiobooksCalled(bool is_success);
    void onThumbResourceCalled(bool is_success);
    void onCommonResourceCalled(bool is_success);
    void onMaterialAPICalled(bool is_success);
    void onAISpeakingTopicCalled(bool is_success);
    void onAISpeakingCommonCalled(bool is_success);
    void onAISpeakingLessonCalled(bool is_success);

    void onCallRequestFalseApiAgain(ApiTag apiCalltag);
    void onSkipRequestFalseApi(ApiTag apiCalltag);

    void addTextDeviceID();
    void goToF2PFlow();
    void goToNormalFlowAfterAPIDone();
    void onAPIDone();
    void updateProfileInClevertab(int idProfile);
    
    void preloadThumb();
    
    void goHome();
    void onNextScene();
    
    void onNetworkError(int tag = kMSIntroLoadAlertTag_ApiRequired);
    
    
    bool _is_api_required;
	bool _is_profile_onboard_new;
    bool _checkOntimeAPIDone = false;

    cocos2d::ui::LoadingBar * _loading_bar = nullptr;
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;

    void onSampleStory(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    void onRestore(cocos2d::Ref * sender);
    
    void onTestActVMonkey();
	bool CompareVersionApp(std::string currVerion);

	bool _show_sample;
    bool _read_sample;
    bool _show_license;
    int _sample_id;
    
    bool _is_api_done;
	bool _is_onboarded;
    
    std::unordered_map<int, std::pair<int, int>> _sample_stories;
    
    void updateProgressBar(float dt);
    GAFWrapper * _monkey;
    
    int _v_server_audio;
    int _v_server_story;
    int _v_server_material;
    int _v_server_lesson;
    int _v_server_ai_speaking_topic;
    int _v_server_ai_speaking_items;

    int _v_local_audio;
    int _v_local_story;
    int _v_local_material;
    int _v_local_lesson;
    int _v_local_ai_speaking_topic;
    int _v_local_ai_speaking_items;

//    std::string _profile_id = "";
    StoryAlert * _storyAlert = nullptr;
};

CREATE_READER(MSIntroLoad);

#endif /* MSIntroLoad_hpp */
