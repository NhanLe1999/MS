//
//  MSTabLessonSelect.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 3/25/19.
//

#ifndef MSTabLessonSelect_hpp
#define MSTabLessonSelect_hpp

#include "cocostudiomacro2.h"
#include "../story/popup/StoryAlert.h"
namespace ms {
    enum LessonTab {
        STORIES_LESSON,
        PHONICS_LESSON
    };

    class MSTabLessonSelectDelegate {
    public:
        virtual void onSelectedLesson(LessonTab tab) = 0;
    };

}

class MSTabLessonSelect : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate {
    
public:
    CREATE_FUNC(MSTabLessonSelect);
    static MSTabLessonSelect * createTab();
    void setContentSize(const cocos2d::Size & size) override;
    void showOnboard();
private:
    ~MSTabLessonSelect();
    void didLoadFromCSB();
    void onEnter() override;
    void onChangeDisplayLanguage(int lang_id);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    CC_SYNTHESIZE(ms::MSTabLessonSelectDelegate *, _delegate, Delegate);
    
    void onMSSelect(cocos2d::Ref * sender);
    void onMPSelect(cocos2d::Ref * sender);
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
};
CREATE_READER(MSTabLessonSelect);
#endif /* MSTabLessonSelect_hpp */
