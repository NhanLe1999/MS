////
////  MSCollectionLessonItem.hpp
////  MJStory
////
////  Created by To Ngoc Linh on 11/13/17.
////
//
//#ifndef MSCollectionAiSpeakingLessonItem_hpp
//#define MSCollectionAiSpeakingLessonItem_hpp
//
//#include "cocostudiomacro2.h"
//#include "MJStoryInfoModel.h"
//#include "StoryAlert.h"
//#include "MJAPI_FirstInstall_Thumb_Zip.h"
//#include "cocostudiomacro2.h"
//#include "CameraHelper.h"
//#include "APPurchaseManager.h"
//#include "MSLessonAISpeakingManager.h"
//namespace ms {namespace delegate {
//    class CollectionAISpeakingLessonItem;
//}}
//
//class MSCollectionAiSpeakingLessonItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate{
//public:
//    CREATE_FUNC(MSCollectionAiSpeakingLessonItem);
//    static MSCollectionAiSpeakingLessonItem* createItem(mj::model::AISpeakingLessonInfo item, bool isLessonLearn = false);
//    void setLessonStateToView(mj::model::AISpeakingLessonInfo::State state);
//
//    mj::model::AISpeakingLessonInfo::State loadLessonstate();
//    void onResize(cocos2d::Size size);
//    void setDisplayMode(); // hiển thị trong tab downloaded
//    void enableDeleteMode();
//    void disableDeleteMode();
//    void openLessonItemForce();
//    void setLastLesson();
//    void initialViewInfo(mj::model::AISpeakingLessonInfo item);
//    void setLessonLockedState(mj::model::AISpeakingLessonInfo::State state);
//    void setLessonUnLockedState(mj::model::AISpeakingLessonInfo::State state);
//    void setLessonLearnedState(mj::model::AISpeakingLessonInfo::State state);
//    void setLessonPreparingState(mj::model::AISpeakingLessonInfo::State state);
//    void setLessonReadyState(mj::model::AISpeakingLessonInfo::State state);
//    void setTopicID(int topicId){};
//    int getTopicPicId(){
//
//    };
//    mj::model::AISpeakingLessonInfo getLessonAISpeakingInfo() {
//        return _aISpeakingLessonInfo;
//    }
//
//    void setLessonAISpeakingInfo(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo) {
//        _aISpeakingLessonInfo = aISpeakingLessonInfo;
//    };
//    void setStateInfo(mj::model::AISpeakingLessonInfo::State state);
//    mj::model::AISpeakingLessonInfo::State getStateInfo(){
//        return _aISpeakingLessonInfo.state;
//    };
//    void onSaveLessonState(mj::model::AISpeakingLessonInfo::State state);
//    void onUpdateTopicProccess();
//private:
//    void didLoadFromCSB(mj::model::AISpeakingLessonInfo item, bool isLessonLearn = false);
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
//    void updateDownloadLessonProcess(std::string cirlceProccess = "", float percent = 0, bool downloadError = false);
//    void showPermissionPopup();
//    void showAlertDownloadLesson();
//    bool checkPermissionCameraAndAudio();
//    void requestPermission();
//    void onExit() override;
//    void onLearn(cocos2d::Ref * sender);
//    void onDownload(cocos2d::Ref * sender);
//    void onRelearn(cocos2d::Ref * sender);
//    void onLocked(cocos2d::Ref * sender);
//    void onEnter() override;
//    void onLessonReady();
//    void onLessonError(std::string reason = "");
//    void checkCommonVideoReady(mj::model::AISpeakingLessonInfo info, std::function<void(bool)> callback = nullptr);
//    void addFirstActivityProgressListener();
//    void removeFirstActivityProgressListener();
//
//    void addFirstActivityReadyListener();
//    void removeFirstActivityReadyListener();
//
//    void addDownloadVideoCommonProgressListener();
//    void removeDownloadVideoCommonProgressListener();
//    void onUnzipToGetConfig(std::function<void(bool, std::string)> callback = nullptr, ms::CommonDownloadType type = ms::CommonDownloadType::ON_DOWNLOAD);
//    CC_SYNTHESIZE(ms::delegate::CollectionAISpeakingLessonItem *, _delegate, Delegate);
//
//    CC_SYNTHESIZE(mj::model::AISpeakingLessonInfo, _info, Info);
//    void changeTextDisplayLang();
//    float getLessonSize(std::vector<mj::model::ActAISpeakingInfo> acts);
//    void onAlertOK(cocos2d::Ref *sender) override;
//    void onAlertCancel(cocos2d::Ref *sender) override;
//    bool onDeleteDataExpected(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo);
//    void showSubscribePopup();
//    void pushEventAIspeakingToCleaverTap(bool successfulStatus = true, std::string failed_reason = "None");
//
//
//    bool _is_downloading = false, _is_downloadingAiSpeaking = false, _isExitLesson = false, _commonVideoExits = false, _isLessonLearned = false, _isNeedDownLoad = false;
//    static int _lesson_locked_audio_id;
//    mj::model::AISpeakingLessonInfo _aISpeakingLessonInfo;
//    StoryAlert* _storyAlert = nullptr;
//    Layout * _panel_backGround = nullptr, * _panel_Download = nullptr, * _maskLayout = nullptr, * _control = nullptr, * _Panel_2_0 = nullptr, *_greenLayout = nullptr, *_outLineLayout = nullptr;
//    Button* _button_locked = nullptr, * _button_relearn = nullptr, * _button_learn = nullptr, * _button_download = nullptr;
//    Text* _name = nullptr, * _order = nullptr;
//    Label* _labName = nullptr;
//    ProgressTimer* _progcess = nullptr;
//    vector<std::string> _actorNames;
//    int _percentDownloading = 0, _topicId = 0;
//    std::vector<mj::model::ActInfo> _activities;
//    static bool _isDownloadingCommonVideo;
//};
//
//CREATE_READER(MSCollectionAiSpeakingLessonItem);
//
//#endif /* MSCollectionAiSpeakingLessonItem_hpp */
