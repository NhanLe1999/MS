//
//  MSLessonEnd.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#ifndef MSLessonEnd_hpp
#define MSLessonEnd_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

namespace ms { namespace delegate {
    class PreActivity;
}}

class MSLessonEnd : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSLessonEnd);
    static MSLessonEnd * createView(mj::model::LessonInfo info);
    
private:
    void didLoadFromCSB(mj::model::LessonInfo info);
    CC_SYNTHESIZE(ms::delegate::PreActivity *, _delegate, Delegate);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBackToLesson(cocos2d::Ref * sender);
    void onNextLesson(cocos2d::Ref * sender);
};

CREATE_READER(MSLessonEnd);

#endif /* MSLessonEnd_hpp */
