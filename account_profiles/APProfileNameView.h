//
//  APProfileNameView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APProfileNameView_hpp
#define APProfileNameView_hpp

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"

class APProfileNameView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APProfileNameView);
    static APProfileNameView * createView();
    void setEnableCloseButton(bool isEnableCloseButton);//kien
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(int lang_id);
    
    void onNext(cocos2d::Ref * sender);
	void onSelectBoy(cocos2d::Ref* sender);
	void onSelectGirl(cocos2d::Ref* sender);
	void onClose(cocos2d::Ref* sender);

	void doCheckNext();

	CC_SYNTHESIZE(ap::delegate::ProfileDelegate *, _delegate, Delegate);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void pushCleverTapEvent(std::string i_clickedType);

};

CREATE_READER(APProfileNameView);

#endif /* APProfileNameView_hpp */
