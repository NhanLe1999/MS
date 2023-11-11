//
//  APProfileAvatarView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APProfileAvatarView_hpp
#define APProfileAvatarView_hpp

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"

class APProfileAvatarView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APProfileAvatarView);
    static APProfileAvatarView * createView(const std::string & name, const bool is_boy, const int age);
    void  setEnableCloseButton(bool isEnableCloseButton);

private:
    void didLoadFromCSB(const std::string& name, const bool is_boy, const int age);
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(int lang_id);

	void updateFX(float dt);
    void onNext(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	CC_SYNTHESIZE(ap::delegate::ProfileDelegate *, _delegate, Delegate);
	std::string _name = "";;
	bool _is_boy = false;
	int _age = 0;
	std::string _avatar = "";
	cocos2d::ui::ListView* _listview;
    
    void pushCleverTapEvent(std::string i_clickedType);

};

CREATE_READER(APProfileAvatarView);

#endif /* APProfileAvatarView_hpp */
