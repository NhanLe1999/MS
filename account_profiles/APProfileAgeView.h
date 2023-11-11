//
//  APProfileAge.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APProfileAge_hpp
#define APProfileAge_hpp

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"

class APProfileAgeView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APProfileAgeView);
    static APProfileAgeView * createView(const std::string & name, const bool is_boy);
    void setEnableCloseButton(bool isEnableCloseButton);
private:
    void didLoadFromCSB(const std::string & name, const bool is_boy);
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(int lang_id);
    
	void onClose(cocos2d::Ref * sender);
    void onNext(cocos2d::Ref * sender);
	void onSelectAge(cocos2d::Ref* sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	void doCheckNext();

	CC_SYNTHESIZE(ap::delegate::ProfileDelegate *, _delegate, Delegate);

	std::string _name = "";
	bool _is_boy = false;
	int _age = 0;
    
    void pushCleverTapEvent(std::string i_clickedType);
};

CREATE_READER(APProfileAgeView);

#endif /* APProfileAge_hpp */
