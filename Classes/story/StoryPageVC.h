//
//  StoryPageVC.h
//  ViettelPost
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __ViettelPost__StoryPageVC__
#define __ViettelPost__StoryPageVC__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "HSBaseScene.h"
#include "PageMenuControl.h"
#include "StoryAlert.h"
#include "StoryInfoObj.h"
#include "GameBase.h"
#include "StoryEndingPage.h"
#include "StorySubscribePopup.h"
#include "MSEventLogger.h"

#define linerun_time 3.0f
#define linerun_start_x -610.0f
#define linerun_end_x 1400.0f

namespace ms { namespace delegate { class Activity; }}

using namespace cocos2d;
using namespace ui;
class StoryPageVC : public HSBaseScene, public PageMenuControlDelegate, public StorySubscribePopupDelegate,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	StoryPageVC();
    ~StoryPageVC();

    CC_SYNTHESIZE(ms::delegate::Activity *, _delegate, Delegate);
    
    CREATE_FUNC(StoryPageVC);
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    void onExit() override;
    int _status = 0;
    void onReplacePage(int index_replace);
    void showNetworkDownWhenRate();
    void showRecomentStories(float duration_delay = 0.0f);
    void showGame(int type = 3);
    void displaymenucontrol(bool isNotHide = false);
    void showGameOver(bool is_interrupted = false);
    void showRatePopup();
    void showRatePopupBuy();
    int _free_play_type = 0;
    void setInfoStory(StoryInfoObj info_story, bool activity_play_mode, bool sample_mode, bool isReadStories = false,  mj::FROMSTORY from = mj::FROMSTORY::DEFAULT);
    StoryInfoObj getInfoStory();
    enum FREE_PLAY_TYPE {
        FREE_TIME_TYPE = 1,
        FREE_OFDAY_TYPE = 2,
        RATE_TYPE = 3,
    };
    void nextPageFromCoverInfStory();
    Button *btclose;
    void sendEventAnalyticsWhenCloseStory();
    bool is_activity_play = false;
    bool is_sample_read = false;
    void setShowHideMenuControl(bool isShow);//isshow = false => hide = true
    void setDisableCancelRecordAction(bool i_input);

    void pushEventInteractiveQuestion();
    void pushCleverTapTrackingData();
    void setTimeUserExitApp();
    void pushFireBaseInteractiveQuestionActivitySkipped();
    static StoryPageVC* getInstance();
    static StoryPageVC* _storyVC;
    std::string _gameZip = "";
private:
    void initPageTurnLayout();
    void transformPageByAngle(Node * n, double angle);
    float transformPageTurn(Node * n, double distance);
    
    float _current_page_angle = 0.f;
    float _prev_page_angle = 0.f;
    Node * _current_page = nullptr;
    Node * _prev_page = nullptr;
    Node * _next_page = nullptr;
    
    ui::Layout * _grad_prev = nullptr;
    ui::Layout * _grad_next = nullptr;
    
    bool _is_moving_next = false;
    bool _is_moving_prev = false;
    
    void onClose(Ref *sender);
    PageMenuControl *_menucontrol = nullptr;
    bool menucontrol_isrunning = false;
    bool isCloseClicked = false;
    bool isloadallpage = false;
    int subscribe_type = 0;
    
    void showRateConfirm();
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void showSubscribePopup();
    void displayEndingPage();
    void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest) override;
    void sentStoryIdDownloadSuccess(int type);
    bool checkStoryCanRead(int index_replace);
    enum DOWNLOAD_SUCCESS_TYPE {
        DOWNLOAD_SUCCESS = 1,
        FREE_TIME_USE = 2,
    };
    bool isSentStoryDownloadSuccess = false;
    void showMonkeyPush();
    bool isRunningMove = false;
    int audio_monkey_foot = 0, audio_pagepush = 0;
    int pagecheck = 2;
    bool check_free_can_read = false;
    bool checkFreeCanRead();
    void createCoverPage();
    StoryInfoObj _storyInfo;
    void backToCoverInfoStory();
    
    void runAirplaneEndStory(int type, bool isHaveGame = true);
    void addTheEndPage(int type, bool isHaveGame = true);
    enum AIRPLANE_TYPE {
        ENDPAGE_TYPE = 1,
        ENDGAME_TYPE = 2,
        NOYOUHAVELEARN_TYPE = 3,
        NOALL_TYPE = 4,
    };
    
    enum ENDSTORY_TYPE {
        YOUHAVELEARN_GAME_ENDSTORY = 3,
        YOUHAVELEARN_ONLY_ENDSTORY = 2,
        GAME_ONLY_ENDSTORY = 1,
        NULL_ENDSTORY = 0,
    };
    
    int getTypeEndingPage();
    void updateShowGame(float dt);
    void updateShowRecommentPage(float dt);
    Node *game_linerun = nullptr;
    MJGBase *game_page = nullptr;
    StoryEndingPage *recomment_page = nullptr;
    void onClickBtPlayPause(bool is_play) override;
    void onClickBtClose() override;
    void onClickBtFavorite(bool i_isFavorited) override;
    cocos2d::ValueMap _map_event_analytics;
    bool is_story_plist = false;
    bool checkLessonPageStoryCanRead(int page_index_replace);
    bool checkLessonFreeCanRead();
    void onSubscribeOk() override;
    void onSubscribeCancel() override;
    void onSubscribeFree() override;
    void runAirplaneEndActivity();

    int _time_start_reading;
    int _page_read;
    int _countNumberClickSkip = 0;
    int _countNumberClickPlayNow = 0;
    int _totalActivity = 0;
    int _timeStartActivity = 0;
    int _totalTimeLearnActvity = 0;
    int _ageUser = 0;
    int _stopActivity = 0;
    int _totalAct = 0;
    int _numberPage = 0;
    int _timeStartReadStory = 0;
    int _timeReadStory = 0;
    bool _checkStartGame = false;
    bool _checkClickButtonPlay = false;
    bool _checkEndStory = false;
    bool _checkClickButtonCloseOfStories = false;
    bool _checkPushEvent = true;
    bool _checkPushEventLearnStroy = true;
    bool _checkCloseInteractiveQuestion = false;

    std::string name_grade = "";
    std::string time_activity = "";
    
    void startGame(int story_id, int act_order);
    std::vector<mj::model::ActInfo> activities = {} ;
    int act_index = 0;
    Layout* layout_game = nullptr;
    Layout* layout_block_touch = nullptr;
    void runTransitionEndActivity();
    //
    void addLayerActivityEnd(Node* layerAct);
    void clickButtonPlay(Node* layerAct, cocos2d::ui::ListView* listview_bar);
    void clickButtonSkip(Node* layerAct,ImageView* arrow, ListView* listview_bar, ui::ImageView* img_checked_csb, ui::ImageView* image_thurm, ui::ImageView* img_normal_csb);
    void clickButtonClose();
    void readAudioOfActivity();
    void endActivity();
    //
private://region for tracking data
    ms::StoryLearningTracker    m_StoryTracker;
    ms::FinishStoryTracker      m_finishStoryTracker;
    int64_t                     m_currentTime;
    bool                        m_hasPendingTrackingEvents = true;
    bool _isReadStories = false;

    void pushStoryTrackerDataToServer();
    void pushFinishStoryTrackerDataToServer();
    bool m_disableCancelRecordAction = false;
    
    struct CloseButtonData
    {
        bool onMove = false;
        bool onBot = false;
    };
    CloseButtonData close_button_data;

    int _countClickButtonPlayAndSkip = 0;

    std::string _gameNameSkip = "", _gameNamePlay = "", _gameNameExit = "";
    mj::FROMSTORY _from = mj::FROMSTORY::DEFAULT;
};

CREATE_READER_CLASS(StoryPageVCReader, StoryPageVC);

#endif /* defined(__ALTP__StoryPageVC__) */
