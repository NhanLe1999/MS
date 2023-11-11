//
//  MSActivityBase.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//
#pragma once

#include "cocos2d.h"
#include "StoryDataModel.h"
#include "MJBaseActivity.h"
#include "MSPreActivity.h"
#include "MSLessonManager.h"
#include "UnzipErrorPopup.h"

#define STARTING_FROM_LESSON_TAG    925
#define STARTING_FROM_STORY_TAG     926 //cau hoi tuong tac

class MSActivityWrapper : public cocos2d::ui::Layout, public ms::delegate::Activity, public ms::delegate::PreActivity
{
public:
    
    CREATE_FUNC(MSActivityWrapper);
    
    enum class START_FROM
    {
        LESSON,
        END_STORY,//for cau hoi tuong tac
    };
    
    static void preSetupBeforeStartGame();
    
    static MSActivityWrapper * createWithInfo(mj::model::ActInfo info, START_FROM i_startFrom = START_FROM::LESSON);
    
    static void pushCleverTapTrackingData_Vmonkey(const mj::model::LessonInfo& lesson, int numb_of_activities, int currentActivityID);
  
    void onActivityEndWithReason(ms::delegate::Activity::EndReason reason);

private:
    
    void onEnter() override;
    void onExit() override;

    bool isResourceReady();
    void waitAndStartDownloading();
    void onDowloadingResource();
    void onResourceDownloaded();
    
    void showActivity();
    
    bool initWithInfo(mj::model::ActInfo info,START_FROM i_startFrom );
    
    mj::model::ActInfo _info;
    
    void onActivityEnd(Ref * sender, ms::delegate::Activity::EndReason reason) override;
    void onActivityReady();
    void onActivityError();
    
    void onStartOver() override;
    void onBackToLesson() override;
    void onContinueLearn(mj::model::ActInfo) override;

    std::function<void()> _callbacknResourceDownloaded = nullptr;
    std::function<void()> _callbacknshowActivity = nullptr;
    std::function<void(ms::delegate::Activity::EndReason reason)> _callbacknOnActivityEndWithReason = nullptr;
    std::function<void(bool success)> _callbacknOnUnzip = nullptr;
    void setCallbackResourceDownloaded();
    void setCallbackShowActivity();
    void setCallbackOnActivityEndWithReason();
    void setCallbackOnUnzip();
    
    std::once_flag _flag_show_activity;
    
    std::once_flag _display_activity;

    std::pair<int, std::vector<mj::model::ActInfo>> _listActivityOfLesson = { -1, {} };
    
    CC_SYNTHESIZE(START_FROM, m_startFrom, StartFrom);

};

