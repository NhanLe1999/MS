
#ifndef HatchingEggs_Item_HPP
#define HatchingEggs_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;
class HatchingEggs_ItemDelegate {
public:
    virtual void onClickedDelegate(cocos2d::Ref *sender) {};
};
class HatchingEggs_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(HatchingEggs_Item);
    static HatchingEggs_Item * createItem(mj::PHONIC_INFO item_info);
    void playEffect(bool correct);
    void setDelegate(HatchingEggs_ItemDelegate* delegate);
    std::string _audio;
private:
    void onEnter() override;
	void didLoadFromCSB(mj::PHONIC_INFO item_info);
    void onClickChicken(Ref* sender);
    std::string font_name = "fonts/Linotte Regular.otf";
    int font_sz = 46;
    
    HatchingEggs_ItemDelegate* _delegate;

};

CREATE_READER(HatchingEggs_Item);

#endif
