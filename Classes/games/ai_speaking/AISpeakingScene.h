//#pragma once
//#include "cocostudiomacro2.h"
//#include "cocos2d.h"
//#include "SpineWrapper.h"
//#include "MJBaseActivity.h"
//#include "HSAudioEngine.h"
//#include "MSRecord.h"
//#include "AISpeakingReviewScreen.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#include "CleverTapManager.h"
//#include "MJStoryInfoModel.h"
//
//#define AI_SPEAKING_FOLDER_PATH "ai_speaking"
//
//#define LET_TALK_SOUND_PATH_01 "sounds/monkey_talking/let_s_talk.mp3"
//#define LET_TALK_SOUND_PATH_02 "sounds/monkey_talking/let_s_talk_1.mp3"
//#define LET_TALK_SOUND_PATH_03 "sounds/monkey_talking/let_s_talk_2.mp3"
//
//#define LISTEN_AND_REPEAT_SOUND_PATH_01 "sounds/monkey_talking/listen_and_repeat.mp3"
//#define LISTEN_AND_REPEAT_SOUND_PATH_02 "sounds/monkey_talking/listen_and_repeat_1.mp3"
//#define LISTEN_AND_REPEAT_SOUND_PATH_03 "sounds/monkey_talking/listen_and_repeat_2.mp3"
//
//#define LISTEN_AND_REPEAT_SOUND_PATH_01 "sounds/monkey_talking/press_the_call_button_to_start_talking.mp3"
//#define LISTEN_AND_REPEAT_SOUND_PATH_02 "sounds/monkey_talking/press_the_call_button_to_start_talking_1.mp3"
//#define LISTEN_AND_REPEAT_SOUND_PATH_03 "sounds/monkey_talking/press_the_call_button_to_start_talking_2.mp3"
//
//#define PRESS_CALL_BUTTON_SOUND_PATH_01 "sounds/monkey_talking/press_the_call_button_to_start_talking.mp3"
//#define PRESS_CALL_BUTTON_SOUND_PATH_02 "sounds/monkey_talking/press_the_call_button_to_start_talking_1.mp3"
//#define PRESS_CALL_BUTTON_SOUND_PATH_03 "sounds/monkey_talking/press_the_call_button_to_start_talking_2.mp3"
//
//#define CALLING_SOUND_EFFECT_PATH   "sounds/monkey_talking/calling_sound_effect.mp3"
//
//#define AI_SEAPKING_RESOURCE_PATH   "games/ai_speaking/"
//#define USER_OUTRO_VIDEO_PATH                       StringUtils::format("%s/user_video_outro.mp4",AI_SPEAKING_FOLDER_PATH)
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//struct Audio {
//	int id;
//	std::string path;
//	float duration;
//	std::vector<math::resource::Sync_Text> sync_data;
//	int voice_id;
//};
//
//struct AISpeakingActorVersionData {
//
//    std::string _versionName;
//    std::string _introVideoPath;
//    std::string _introMIMICVideoPath;
//    std::string _introTalkingVideoPath;
//    std::string _yourTurnVideoPath;
//    std::string _cheerUpVideoPath;
//    std::string _listeningVideoPath;
//    std::string _outroMIMICVideoPath;
//    std::string _outroVideoPath;
//};
//
//struct AISpeakingActorCommonResourceData {
//
//    std::string _actorName;
//
//    std::vector<AISpeakingActorVersionData> _verisonList ;
//
//};
//
//struct AISpeakingCustomVideoInfo {
//
//    enum class VideoTurnType {
//        INTRO_COMMON = 0,
//        INTRO_MIMIC,
//        INTRO_TALKING,
//        NORMAL_QUESTION_SPEED,
//        SLOW_QUESTION_SPEED,
//        YOUR_TURN,
//        LISTENING,
//        CHEER_UP,
//        OUTRO
//    };
//    int         _videoID = -1;
//    int         _turnID = -1;
//    std::string _videoUrl = "";
//    float       _duration   = -1.0f;
//    float       _timeStart  = -1.0f;
//    float       _timeEnd    = -1.0f;
//    ms::AISpeakingMode                   _mode;
//
//    bool _isStartCallbackCalled = false;
//
//    bool _isEndCallbackCalled = false;
//
//    std::function<void(AISpeakingCustomVideoInfo*)> _startCallback = nullptr;
//    std::function<void(AISpeakingCustomVideoInfo*)> _endCallback = nullptr;
//
//    VideoTurnType _videoType;
//
//    AISpeakingCustomVideoInfo(){}
//
//    AISpeakingCustomVideoInfo(int videoID, VideoTurnType type,std::string videoUrl,std::function<void(AISpeakingCustomVideoInfo*)> startCallback  = nullptr ,std::function<void(AISpeakingCustomVideoInfo*)> endCallback = nullptr,int turnID = -1,ms::AISpeakingMode mode = ms::AISpeakingMode::TALKING){
//
//
//        _videoID        = videoID;
//
//        _videoUrl       = videoUrl;
//
//        _startCallback  = startCallback;
//
//        _endCallback    = endCallback;
//
//        _videoType      = type;
//
//        _turnID         = turnID;
//
//        _mode           = mode;
//    }
//
//};
//
//struct AISpeakingTurnInfo {
//
//    int         _id;//gobalID
//    int         _localID;
//    std::string _normalVideoPath = "";
//    std::string _slowVideoPath = "";
//    std::string _text; //text can question or answer
//    std::string _questionText = ""; //for talking mode only.
//    std::string _imagePath = "";
//    Audio       _audio;
//
//    std::unordered_map<int,AISpeakingCustomVideoInfo*> _videoInfoList;
//};
//
//struct AISpeakingSessionInfo {
//    int                              _sessionID = -1;
//    std::vector<AISpeakingTurnInfo>  _turns;
//    AISpeakingCustomVideoInfo        _introMIMICOrTalkingVideo;
//    ms::AISpeakingMode                   _mode;
//    int                              _currentTurnID = 0;
//};
//
//struct AISpeakingCheerUpConfig {
//
//    std::vector<int> _cheerUpMIMICDisplayTurnIDs;
//    std::vector<int> _cheerUpTALKINGDisplayTurnIDs;
//    std::vector<std::string> _cheerUpVideoPath;
//};
//
//
//struct AISpeakingData {
//
//    std::vector<AISpeakingSessionInfo> _listInfo;
//
//    AISpeakingCustomVideoInfo _introVideo;
//
//    AISpeakingCustomVideoInfo _outroVideo;
//
//    std::vector<std::string> _introVideoPath ;
//    std::vector<std::string> _introMIMICModeVideoPath ;
//    std::vector<std::string> _introTALKINGModeVideoPath ;
//    std::vector<std::string> _yourTurnVideoPath;
//    std::vector<std::string> _listeningActorVideoPath;//idle
//    std::vector<std::string> _outroVideoPath;
//    std::vector<std::string> _outroMIMICVideoPath;
//    int                     _currentSessionID = 0;
//    AISpeakingCheerUpConfig _cheerUpConfig;
//};
//
//
//class AISpeakingScene : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol,public MSRecordDelegate {
//
//public:
//
//    CREATE_FUNC(AISpeakingScene);
//
//    static cocos2d::Scene* createScene(std::vector<math::resource::Image*> images);
//
//    static AISpeakingScene* createView(std::vector<math::resource::Image*> images, mj::model::AISpeakingLessonInfo lessonInfo = mj::model::AISpeakingLessonInfo(), ms::AISpeakingMode mode = ms::AISpeakingMode::DEFAULT);
//
//    void didLoadFromCSB(std::vector<math::resource::Image*> images, mj::model::AISpeakingLessonInfo lessonInfo = mj::model::AISpeakingLessonInfo(), ms::AISpeakingMode mode = ms::AISpeakingMode::DEFAULT);
//
//    ~AISpeakingScene();
//
//    void onScenePause();
//
//    void onSceneResume();
//
//private:
//
//    std::vector<mj::model::TalkingCommonInfo> getRamdomCommonInfo( std::string actorName);
//    bool initData(std::vector<math::resource::Image*> images);
//
//    CC_SYNTHESIZE(ms::delegate::CollectionAISpeakingLessonItem *, _delegate, Delegate);
//
//    bool initVideoInfoList();
//
//    void playGame();
//
//    void playGameScheduleUpdate(float dt);
//
//    void playIntroActorCallback(AISpeakingCustomVideoInfo *videoInfo);
//
//    void playNormalSpeedQuestionStartCallback(AISpeakingCustomVideoInfo *videoInfo);
//
//    void playNormalSpeedQuestionEndCallback(AISpeakingCustomVideoInfo *videoInfo);
//
//    void playListeningActorStateStartCallback(AISpeakingCustomVideoInfo *videoInfo);
//
//    void playListeningActorStateEndCallback(AISpeakingCustomVideoInfo *videoInfo);
//
//    void playOutroStartCallback();
//
//    void playOutroEndCallback();
//
//    void playUserStateShowText();
//
//    void playUserStateRecordAudio();
//
//    void resetRecord();
//
//    void resetAll();
//
//    void onFinishRecord(int finish_type, std::string url ) override;
//
//    void onRecordButtonClicked(Ref* sender);
//
//    void resetCurrentState(Ref* sender);
//
//    void onTextButtonClicked(Ref* sender);
//
//    void onBackButtonClicked(Ref* sender);
//
//    void onExit() override;
//    void pushEventAIspeakingToCleaverTap();
//    void commonExit();
//
//    void setLayoutVisible(bool isHidden);
//
//    void getDataStringList(std::string key,std::vector<math::resource::Image*> images, const rapidjson::Value& valueData, std::vector<std::string> &outputRef);
//
//    void setAISpeakingMode(ms::AISpeakingMode mode){
//        _mode = mode;
//    };
//    ms::AISpeakingMode getAISpeakingMode(){
//        return _mode;
//    };
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
//
//
//private:
//
//    float getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput);
//	cocos2d::Node* _introLayer = nullptr;
//	cocos2d::Node* _gameLayer = nullptr;
//	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
//
//	AISpeakingData          _data;
//    std::vector<std::pair<int,std::string>> _videoUrls ;
//
//	MSRecord* _record = nullptr;
//    int _idAudioQuestion = -1, _learedVideocount = 0, _timeToClickRecoding = 0, _recordUserListeningVideoID = 0, _currentAudioID = -1, _currentPlayingVideoID = -1, _isClickAnswer = 0;
//    float _timeCount = 0.0f, _scaleTextSize = 1.0f;
//    cocos2d::Node* _videoAreaAtGameLayer = nullptr;
//
//    cocos2d::Node* _videoAreaAtIntroLayer = nullptr;
//
//    cocos2d::Node* _recordButton = nullptr;
//
//    MergedVideoInfo _mergedVideoInfo;
//
//    cocos2d::experimental::ui::VideoPlayer* _videoPlayer = nullptr;
//
//    std::unordered_map<int,AISpeakingCustomVideoInfo*> _videoInfoCacheList;
//
//    std::unordered_map<int,AISpeakingTurnInfo*> _turnInfoCacheList;
//
//    std::unordered_map<int, std::pair<float,float>>  _videoInfoTracks;
//
//    cocos2d::ui::Button* _textButton = nullptr;
//
//    std::vector<AISpeakingReviewScreenTurnInfo> _reviewTurnList;
//
//    cocos2d::Node* _rootLayout = nullptr;
//    ImageView* _speakerIcon = nullptr;
//    bool _isListeningVideo = false, _skipMIMIC = false, _isResetCurrentState = false, _isClickHangUpPopup = false;
//
//    CC_SYNTHESIZE(mj::model::AISpeakingLessonInfo, _lessonInfo, LessonInfo);
//    ms::AISpeakingMode _mode = ms::AISpeakingMode::DEFAULT;
//
//    };
//
//CREATE_READER(AISpeakingScene);
//#endif
//
////#endif
