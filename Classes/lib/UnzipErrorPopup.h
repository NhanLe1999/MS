//
// Created by quan on 3/2/2021.
//

/*
 * Xử lí các trường hợp khi unzip data từ server bị lỗi.
 */

#pragma once

#include <functional>

#include "cocostudiomacro2.h"
#include "cocos2d.h"

#include "MSLessonManager.h"
#include "MSTabMaterial.h"
#include "StoryDataModel.h"
#include "MSActivityEnd.h"
#include "MSActivityWrapper.h"
#include "MSTabLessons.h"
#include "MSActivityPreview.h"
#include "MSLessonPreview.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class UnzipErrorPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    enum class PopupType {
        STORY,
        STORY_IN_LESSON,
        ACTIVITY
    };
    CREATE_FUNC(UnzipErrorPopup);
    static UnzipErrorPopup *createPopup(PopupType type, mj::model::ActInfo info, cocos2d::ui::Layout* preAct);

private:
    void didLoadFromCSB();
    void onContinue(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);

    cocos2d::ui::RichText* createStoryNotification();
    cocos2d::ui::RichText* createLessonNotification();
    cocos2d::ui::RichText* createNotification(PopupType popupType);
    mj::model::ActInfo info;

    PopupType popupType;
    cocos2d::ui::Layout* preActivity;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
};

CREATE_READER(UnzipErrorPopup);
