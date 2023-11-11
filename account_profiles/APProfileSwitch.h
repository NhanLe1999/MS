//
//  APProfileSwitch.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 08/16/19.
//

#ifndef APProfileSwitch_hpp
#define APProfileSwitch_hpp

#include "cocostudiomacro2.h"
#include "APProfileItem.h"
#include "MMFirebaseDatabase.h"

class APProfileSwitch : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public APProfileItemDelegate {
public:
	CREATE_FUNC(APProfileSwitch);
	static APProfileSwitch * createView();

private:
	~APProfileSwitch();
	void didLoadFromCSB();
	void onEnter() override;
	void onExit() override;
    void onChangeDisplayLanguage(int lang_id);
	void onClose(cocos2d::Ref * sender);
	void onClickSelectItem(cocos2d::Ref * sender) override;
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    void pushCleverTapEvent();
	cocos2d::ui::ListView* _listview;
};

CREATE_READER(APProfileSwitch);

#endif /* APProfileSwitch_hpp */
