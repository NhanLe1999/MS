//
//  MJSubscriptionItem.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJSubscriptionItem_hpp
#define MJSubscriptionItem_hpp

#include "CocoStudioMacro.h"
#include "MJInAppInfo.h"

class MJSubscriptionItemDelegate {
public:
    virtual void onSelectProduct(mj::InAppPackage package) = 0;
};

class MJSubscriptionItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJSubscriptionItem);
    void reloadPrice();
    void changeDisplayText();

private:
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & call_back);
    void onClick(cocos2d::Ref * sender);
    void onDetails(cocos2d::Ref * sender);
    CC_SYNTHESIZE(MJSubscriptionItemDelegate *, _delegate, Delegate);
    CC_SYNTHESIZE(mj::InAppPackage, _package, Package);
    CC_SYNTHESIZE(int, _discount, Discount);
};

CREATE_READER_CLASS(MJSubscriptionItemReader, MJSubscriptionItem);

#endif /* MJSubscriptionItem_hpp */
