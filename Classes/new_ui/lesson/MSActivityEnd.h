//
//  MSActivityEnd.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/22/17.
//

#ifndef MSActivityEnd_hpp
#define MSActivityEnd_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

namespace ms { namespace delegate {
    class PreActivity;
}}

class MSActivityEnd : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSActivityEnd);
    static MSActivityEnd * createPopup(mj::model::ActInfo activity, bool animate_next = true);
    void readActivityName(std::string name);
private:
    void didLoadFromCSB(mj::model::ActInfo activity, bool animate_next);
    void onEnter() override;
    void onExit() override;
    
    CC_SYNTHESIZE(ms::delegate::PreActivity *, _delegate, Delegate);
    void onBackToLesson(cocos2d::Ref * sender);
    void onContinue(cocos2d::Ref * sender);
    void setCallbackActivityName();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    
    bool _is_animate_next = false;
    
    mj::model::ActInfo _info;
    mj::model::ActInfo _next_info;
    std::function<void(std::string act_name)> _callbackActivityName = nullptr;
	struct CloseButtonData
	{
		bool onMove = false;
		bool onBot = false;
	};
	CloseButtonData close_button_data;
	int _countContiue = 0;
};

CREATE_READER(MSActivityEnd);

#endif /* MSActivityEnd_hpp */
