//
//  StoryInfoCover.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryInfoCover__
#define __StoryInfoCover__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "StoryInfoObj.h"
#include "base_include.h"
#include "HSLib.h"
#include "LTZipHelper.h"
#include "HSBaseLayout.h"
#include "MJStoryInfoModel.h"
#include "MSLessonManager.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "UnzipErrorPopup.h"
#include "MSDownloader.h"
namespace mj {
    class GetLinkDownloadAPI;
}

namespace ms {
    namespace delegate {
    class Activity;
    
    }}

using namespace std;
using namespace cocos2d;
using namespace ui;
class StoryInfoCover : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol, public LTZipHelperDelegate
{
public:
    CREATE_FUNC(StoryInfoCover);
    StoryInfoObj _storyInfo;
    static Scene* createScene(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from, Size& senceSize);
    static StoryInfoCover* createView(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from);
    void initWithData(StoryInfoObj info, bool isPortrait = false, int type = 1,const mj::FROMSTORY &from = mj::FROMSTORY::DEFAULT);
    int _type;
    enum COVER_TYPE {
        Home_Type = 1,
        Story_Type = 2,
    };
    bool is_activity_play = false;
    void startDownload();
    bool is_sample_read = false;
    bool _isReadStories = false;
    
    mj::FROMSTORY _from = mj::FROMSTORY::DEFAULT;
    
    cocos2d::Rect getThumbBoundingBox();
private:
    void didLoadFromCSB(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    void onExit() override;
    
    bool isDownloaded = false;

    void onClose(cocos2d::Ref *sender);
    void onDownload(cocos2d::Ref *sender);
    void onPlayStory(cocos2d::Ref *sender);
    void onGoback(cocos2d::Ref *sender);
    void onSkip(cocos2d::Ref *sender);
    
    void setCallbackOnPlayStory();
    std::function<void()> _callbackOnPlayStory = nullptr;
    
    void doDownload();
    
    void addFaceMask();
    
    Layout *panel_read, *panel_download, *panel_progress;
    Text *text_btdownload, *text_downloadguide, *text_status, *text_percent, *text_readtome, *text_autoplay, *text_autoitmyseft;
    ImageView *img_book;
    ui::LoadingBar * progress;
//    ProgressTimer *progress;
    ValueMap _mapLangContent;
    void onPanelLayoutTouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eventType);
    Vec2 pointTouch;
    Layout *panel_bg;
    void startSyncText();
    ValueVector timeline;
    ValueMap _mapT0;
    int iStart = 0;
    int iEnd = 1;
    int sync_index = 0, cau_index = 0;
    void highlightTextAction();
    Color3B normal_color = Color3B::BLACK;
    Color3B highlight_color = Color3B::RED;
    ui::Text * namestory_label, * detail_label, * design_label, * lbwritten, * lbillus;
    void onStartPlayStory();
    std::string _audio_title, _audio_written, _audio_design;
    void highlightWrittenDesign();
    float _time_delay_design = 2;
    float _time_delay_written = 2;
    
    void onDownloadAndUnzipDone();
    
    void onDownloadError(int errorCode, std::string message) override;
    void onUnzipError(int errorCode, std::string) override;
    void onDownloadSuccess() override;
    void onUnzipSuccess() override;
    void onProgressChange(LTZipHelper::DownloadState download_state, float percent) override;
   
    bool ischeck = false;
    bool isStoryDownloaded = false;
    bool _is_downloading = false;
    bool is_story_has_plist0 = false;
    bool _checkDownload = false;
    
    void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest) override;
    void getLinkDownloadStory(bool is_retry);
    std::string replaceString(std::string subject, const std::string search,
                              const std::string replace);
    void loadAllPageStories();
    std::string linkdownload;
    bool _isPortrait = false;
    void onLevelInfo(cocos2d::Ref *sender);
    void displayLevelInfoGuide(float dt);
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertClose(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    bool isReadDoneTitle = false;
    void deleteSourceStoryWhenOver50();
    void onKeyboardPage(int index_keyboard);

    int attemp_download = 0;
    
    CC_SYNTHESIZE(ms::delegate::Activity *, _delegate, Delegate);
    
    mj::GetLinkDownloadAPI * _getlink_api = nullptr;
    void onReadWithNarration(cocos2d::Ref * sender);
    void onNarrationNormal(cocos2d::Ref * sender);
    
    void setCallbackonNarrationNormal();
    std::function<void()> _callbackonNarrationNormal = nullptr;
    void setCallbackInNarrationNormal();
    std::function<void()> _callbackInNarrationNormal = nullptr;
    
    void onNarrationMute(cocos2d::Ref * sender);
    
    void onAddFace(cocos2d::Ref * sender);
    
    float getStorySize(std::vector<mj::model::ActInfo> acts);
    
    std::vector<mj::model::ActInfo> activities = {} ;
    void downloadResourceQuizzVmonkey();
    void downloadAct(mj::model::ActInfo _info, int index);
    float per_Story;
    float per_VMQuizz;
    float current_percent;
    bool _readNameStroy = true;
    void onDownloadErrorQuizz(int errorCode, std::string message, int index);
    
    bool m_isHaveInteractiveQuestions = false;
    
    float story_size;
    float act_size;
    
    void estimate();

    void InitDisplay();

    void InitObjects(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from);
    
    void pushDownloadStoryEvent(bool didDownloadSuccess, bool didUnzipSuccess, std::string url = "");
    bool isStoryAvailable();
    void setOnprocess(double index);
    bool _usingSence = false;
    
private:
    
    std::function<void()> _callback = nullptr;
    
    StoryAlert* _storyAlert = nullptr;
    int _countClickRetry = -1;

    std::pair<int, std::string> _valueLinkError = {-1, ""};
    std::vector<std::string> _listUtlDownloadSucce = {};
    std::vector<std::string> _listUrlDownloadError = {};
    int _maxRetryCount = -1;
    std::vector<std::string> _valueDomainDownloadStories = {};
    std::vector<std::string> _valueDomainDownloadAudioBookAndActivity = {};
    std::string keyTypeError = "";
    int _indexDownload = 0;
    bool _isNewDownload = true;
    std::vector<std::string> _listEvent = {};
    std::string _urlStrory = "";

    int _countRetryDownloadUrl = 0;
    MSCollectionStoryItem* _info = nullptr; 
   
public:
    //download level
    void downloadAction(string url);
};

CREATE_READER_CLASS(StoryInfoCoverReader, StoryInfoCover);

#endif /* defined(__ALTP__StoryInfoCover__) */
