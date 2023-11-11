//
//  MSCollectionLessonItem.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/13/17.
//

#ifndef MSCollectionLessonItem_hpp
#define MSCollectionLessonItem_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "StoryAlert.h"
#include "MSFreeContentPage.h"

namespace ms {namespace delegate {
    class CollectionLessonItem;
}}

class MSCollectionLessonItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate {
public:
    CREATE_FUNC(MSCollectionLessonItem);
    static MSCollectionLessonItem* createItem(mj::model::LessonInfo item, bool isLessonLearn = false);
    
    void setLessonState(mj::model::LessonInfo::State state);
    void setDisplayMode(); // hiển thị trong tab downloaded
    void enableDeleteMode();
    void disableDeleteMode();
    void openLessonItemForce();
    void setLastLesson();
    void downloadingLesson(bool learnWhenReady = false);
private:
    void didLoadFromCSB(mj::model::LessonInfo item, bool isLessonLearn = false);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;

    void onExit() override;
    void onLearn(cocos2d::Ref * sender);
    void onDownload(cocos2d::Ref * sender);
    void onRelearn(cocos2d::Ref * sender);
    void onLocked(cocos2d::Ref * sender);
    void onEnter() override;
    void onLessonReady();
    void onLessonError();
    
    void addFirstActivityProgressListener();
    
    CC_SYNTHESIZE(ms::delegate::CollectionLessonItem *, _delegate, Delegate);
    
    CC_SYNTHESIZE(mj::model::LessonInfo, _info, Info);
    void changeTextDisplayLang();
    float getLessonSize(std::vector<mj::model::ActInfo> acts);
    void onAlertOK(cocos2d::Ref *sender) override;
    void onAlertCancel(cocos2d::Ref *sender) override;
    void onAlertClose(cocos2d::Ref *sender) override;
    void onDownloadWithoutAlert();
    bool _is_downloading = false;
    bool _isLessonLearned = false;
    bool _isSkipPopDowload = false;
    int _countClickRetry = -1;
    bool _isLessonReady = false;
    bool  _learnWhenReady = false;
    mj::model::ActInfo _infoActDownload;

    std::vector<mj::model::ActInfo> _activities = {};
    double _precent = 0.0;
    
    static int _lesson_locked_audio_id;

    StoryAlert* _storyAlert = nullptr;
};

CREATE_READER(MSCollectionLessonItem);

#endif /* MSCollectionLessonItem_hpp */
