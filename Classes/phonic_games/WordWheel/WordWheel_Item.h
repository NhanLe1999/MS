
#ifndef WordWheel_Item_HPP
#define WordWheel_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class WordWheel_ItemDelegate {
public:
    virtual void onTouchedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {};
};
class WordWheel_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(WordWheel_Item);
    static WordWheel_Item * createItem(mj::PHONIC_INFO item_info);
    void playEffect(float angle);
    void setDelegate(WordWheel_ItemDelegate* delegate);
    std::string _audio;
    
private:
    void onEnter() override;
	void didLoadFromCSB(mj::PHONIC_INFO item_info);
    
    //touch item
    void onTouched(Ref* sender, ui::Widget::TouchEventType type);
    std::string text_info;
    
    std::string font_name = "fonts/Linotte Regular.ttf";
    int font_sz = 50;
    Layout* item;
    Layout* _layout;
    WordWheel_ItemDelegate* _delegate;
    

};

CREATE_READER(WordWheel_Item);

#endif
