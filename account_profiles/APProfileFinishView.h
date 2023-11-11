//
//  APProfileAvatarView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APProfileFinishView_hpp
#define APProfileFinishView_hpp

#include "cocostudiomacro2.h"
#include "APProfile.h"
class APProfileFinishView : public cocos2d::ui::Layout, cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(APProfileFinishView);
	static APProfileFinishView * createView(const std::string& name, const bool is_boy, const int age, const std::string& avatar, bool is_new = false);
	static APProfileFinishView * createView(const ap::Profile& profile, bool is_new = false);
private:
	void didLoadFromCSB(const std::string& name, const bool is_boy, const int age, const std::string& avatar, bool is_new);

	void onEnter() override;
	void onExit() override;
    
    void changeDisplayLanguage();
	void onClose(cocos2d::Ref * sender);
	ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
};

CREATE_READER(APProfileFinishView);

#endif
