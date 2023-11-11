//
//  GroupPromotionNotification.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/18/19.
//

#ifndef GroupPromotionNotification_hpp
#define GroupPromotionNotification_hpp

#include "cocostudiomacro2.h"

class GroupPromoNotification : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GroupPromoNotification);
    static GroupPromoNotification * createView();
	CC_SYNTHESIZE(cocos2d::Node *, m_delegateParent, DelegateParent);
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    
    void onJoin(cocos2d::Ref * sender);
    void onClickBackground(cocos2d::Ref *sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(GroupPromoNotification);

#endif /* GroupPromotionNotification_hpp */
