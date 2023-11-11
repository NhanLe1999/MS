//
//  MSLessonPreview.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/14/17.
//

#ifndef MSLessonPreview_hpp
#define MSLessonPreview_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSPreActivity.h"

// preview content of a lesson

class MSLessonPreview : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::PreActivity {
public:
    CREATE_FUNC(MSLessonPreview);
    static MSLessonPreview * createView(mj::model::LessonInfo info);
    
    void setPlayFrom(const std::string& str) {m_playFrom = str;};
    void pushCleverTapTrackingData();
    static MSLessonPreview* getInstance();
    static cocos2d::Scene*createScene(mj::model::LessonInfo info);
    static MSLessonPreview* _msLesson;
    int _lessonID = 0;
    std::string _gameZip = "";
    bool _isPushSence = false;
private:
	~MSLessonPreview();
    void didLoadFromCSB(mj::model::LessonInfo info);
    void onExit() override;

    /**
     * button callback functions
     */
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    void onPlay(cocos2d::Ref * sender);

    /**
     * go back to lessons browser
     */
    void onBackToLesson() override;
    
    /**
     * start learning lesson
     */
    void onContinueLearn(mj::model::ActInfo info) override;
    
    std::string                     m_playFrom = "tab_lessons"; // lesson được gọi từ đâu: tab_lesson/tab_free/library
    mj::model::ActInfo              m_curretActivityinfo;
    long long                       m_currentTime = -1;
    mj::model::LessonInfo           _info;
    std::vector<mj::model::ActInfo> _activities;
    static int                      _activity_lock_audio_id;
    bool                            is_phonic_lesson = false;
    int number_activity_learned = 0;
    int _currentProfileId = 0;
    std::string _textNextAct = "";
    int _gradeIdPhonic = -1;
    int numberLearnAct = -1;
    int _lessonId = 0;
    
    void changeTextDisplayLang();
    void onEnter() override;
    void reloadState();
};

CREATE_READER(MSLessonPreview);

#endif /* MSLessonPreview_hpp */
