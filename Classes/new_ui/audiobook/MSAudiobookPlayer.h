//
//  MSAudiobookPlayer.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/16/17.
//

#ifndef MSAudiobookPlayer_hpp
#define MSAudiobookPlayer_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "network/CCDownloader.h"
#include "StorySubscribePopup.h"
#include "StoryAlert.h"
#include "StoryConfigManager.h"
#include "MSDownloader.h"

#define max_time_free_read 30.0f

#include "manager/RatingManager.h"

class StoryAlert;

namespace mj {
    class AudioSyncDataAPI;
}
class MSAudiobookPlayer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StorySubscribePopupDelegate, public StoryAlertDelegate {

public:
    enum class MSAudiobookPlayerType 
	{
		 SINGLE_AUDIO,
		 NOVEL_LIST,
		 NORMAL_LIST //Play all button
    };
public:
    CREATE_FUNC(MSAudiobookPlayer);
    static MSAudiobookPlayer * createView(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type = MSAudiobookPlayerType::SINGLE_AUDIO,int i_id=-1, bool playWhenReady = true);
    /**
     * animate function,
     * call on enter, exit and start playing
     */
    void playTransitionEnter();
    void playTransitionExit();
    void playTransitionPlay();
    void playTransitionPlayWithoutAnimation();
    
    void onPlay(cocos2d::Ref * sender);
    void readData();
	void PrepareSyncTextData(std::function<void(bool)> callback = nullptr);
	void PerpareData(bool is_retry = false, bool isRetryAudiobookDetail = false, std::function<void()> callback = nullptr);
	void ShowHidePlayOneUI(bool iShow = false);
    inline mj::model::AudiobookInfo getInfo() { return _info; }
    
    /**
     * text highlights indexing
     */
    void indexing();
    
    cocos2d::Rect getThumbBoundingBox();
    
    void onPlayerPlay(cocos2d::Ref * sender);
    void onPlayerPause(cocos2d::Ref * sender);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS 
    void pushCleverTapTrackingData();
#else
    void pushCleverTapTrackingDataForWinMac();
#endif
    
    inline bool isPlayingPlaylist() { return (m_currentAudioBookPlayerType != MSAudiobookPlayerType::SINGLE_AUDIO) ; }
private:
	~MSAudiobookPlayer();
    void didLoadFromCSB(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type,int i_id,  bool playWhenReady = true);
    void onEnter() override;
    void onExit() override;

    /**
     * load information of audiobook
     */
#if NEW_AUDIO_BOOK
#else
    void getData_detail(bool is_retry = true); 
#endif
    void getData(bool is_retry = false, bool isRetryAudiobookDetail = false, std::function<void()> callback = nullptr);
    /**
     * button callback functions
     */
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    
    void onBack(cocos2d::Ref * sender);
    void onPlayerBackward(cocos2d::Ref * sender);
    void onPlayerForward(cocos2d::Ref * sender);
    void onSliderEvent(cocos2d::Ref * sender, cocos2d::ui::Slider::EventType event);
    void onRetry(cocos2d::Ref * sender);
    void onDownload(cocos2d::Ref * sender);
    void onPlaylist(cocos2d::Ref * sender);
    void onNextTrack(cocos2d::Ref * sender);
    void onPreviousTrack(cocos2d::Ref * sender);
    void setShowTextDetail(bool isShow);
    void onDismissOnboard(cocos2d::Ref * sender);
    
    void onFavourite(cocos2d::Ref * sender);
    
//    void update(float dt) override;
    void updateSync(float dt);
    
    void onAPICallSuccess();
    void onAPICallError();
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    void onAlertClose(cocos2d::Ref* sender) override;
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    void onGoback(cocos2d::Ref* sender);
    void onSkip(cocos2d::Ref* sender);
//#endif
    

    std::vector<mj::model::SyncBlock> _sync;
    int _current_highlight_index = -1;
    cocos2d::ui::Text * _text = nullptr;
    cocos2d::ui::Slider * _slider = nullptr;
    cocos2d::ui::Layout * _story_container = nullptr;
    int _audio_id;
    
    bool _should_animate_scrolling;
    bool _is_playing;
    bool _isPushEvent = false;

    int _timeStartAudio = 0;
    int _timetotalListenAudio = 0;
//    int _page_index;
    
    std::string _cached_audio;
    
    mj::model::AudiobookInfo _info;
    std::vector<cocos2d::Point> _letter_positioning;
    
    void onAudiobookFinish();
    bool is_app_active = false;
    float time_play_enable = 0.0f;
    void onSubscribeOk() override;
    std::vector<int> _italic_idx;
    void onSubscribeCancel() override;
    void onSubscribeFree() override;
    void showSubScribePopup();
    
    bool _is_favourited;
    bool _is_playlist;
    bool _showing = false;
    bool book_is_available = false;
    long long _saved_time;
    
    StoryAlert * _alert = nullptr;
    void setAduioFreePlayMode();
    
    int _time_start_playing = 0;
    double _percent_played = 0;
    
    Color3B AUDIO_HIGHLIGHT_COLOR = Color3B::RED;
    Color3B AUDIO_NORMAL_COLOR = Color3B::BLACK;
    
    mj::AudioSyncDataAPI * _sync_request = nullptr;

    void updateCurrentTimeLabel(const float i_duration);

    void updateText();

private: // private variables

    bool m_isSliderBarSeekingEnable = true;
    CC_SYNTHESIZE(MSAudiobookPlayerType,m_currentAudioBookPlayerType,CurrentAudioBookPlayerType);
    int m_textUpdateCount = 0;
    std::string m_textContent = "";
    bool m_willCallFromSyncRequest = false;

	//for tracking
	int m_completedPercent = 0;
	int m_clickNext = 0,
		m_clickPause = 0,
		m_clickPrevious = 0;
	bool m_isCloseClick = false;
    
    std::string m_author = "empty";
    
    std::function<void(ms::utils::DownloadUnit unit)> _downloadErrorCallback = nullptr;
    std::function<void()> _apiErrorCallback     = nullptr;
    bool _isAlertShowing = false;
    int _countClickRetry = -1;
    std::vector<std::string> _valueDomainDownloadAudioBookAndActivity = {};
    std::string _typeError = "";
    long long _time = 0;
    bool _completed = false;
	bool _playWhenReady = true;
};

CREATE_READER(MSAudiobookPlayer);

#endif /* MSAudiobookPlayer_hpp */
