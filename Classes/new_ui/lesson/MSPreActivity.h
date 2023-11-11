//
//  MSPreActivity.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#ifndef MSPreActivity_hpp
#define MSPreActivity_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

namespace ms { namespace delegate {
    class PreActivity {
    public:
        virtual void onStartOver() {};
        virtual void onBackToLesson() {};
        virtual void onContinueLearn(mj::model::ActInfo) {};
    };
}}

class MSPreActivity : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSPreActivity);
    static MSPreActivity * createView(mj::model::ActInfo info);
    
private:
    void didLoadFromCSB(mj::model::ActInfo info);
    CC_SYNTHESIZE(ms::delegate::PreActivity *, _delegate, Delegate);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBackToLesson(cocos2d::Ref * sender);
    void onContinue(cocos2d::Ref * sender);
};

CREATE_READER(MSPreActivity);

#endif /* MSPreActivity_hpp */
