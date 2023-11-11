//
//  ViralPopup_Item.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#ifndef ViralPopup_Item_hpp
#define ViralPopup_Item_hpp

#include "cocostudiomacro2.h"

class ViralPopup_ItemDelegate {
public:
    virtual void onBtnShare(cocos2d::Ref* sender) = 0;

};

class ViralPopup_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(ViralPopup_Item);
    static ViralPopup_Item* createPopup();
    void onPlayEffect(bool is_shown, std::function<void()> callback = nullptr);
    
private:
    void didLoadFromCSB();
    void onShare(Ref* sender);
    void onClose(Ref* sender);
    void onReceiveGift(Ref* sender);
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    
    CC_SYNTHESIZE(ViralPopup_ItemDelegate*, _delegate, Delegate);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
  
};
CREATE_READER(ViralPopup_Item)



#endif /* ViralPopup_Item_hpp */
