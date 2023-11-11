//
//  MSTabSubscribe_Item.hpp
//  MJStory
//
//  Created by Hoi Sung on 12/15/17.
//

#ifndef MSTabSubscribe_Item_hpp
#define MSTabSubscribe_Item_hpp

#include "cocostudiomacro2.h"
#include "MJInAppInfo.h"

namespace ms {
    namespace delegate {
        class SubscriptionItem {
        public:
            virtual void onSelectProduct(mj::InAppPackage package) = 0;
        };
    }
}

class MSTabSubscribe_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSTabSubscribe_Item);
    static MSTabSubscribe_Item * createTab();
    void setContentSize(const cocos2d::Size & size) override;
    void reloadPrice();
    void layoutPrice();
    
private:
    void didLoadFromCSB();
    void changeTextDisplayLang();
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBuyClicked(cocos2d::Ref *sender);
    std::string _propertyPrice = "";
    
    CC_SYNTHESIZE(ms::delegate::SubscriptionItem *, _delegate, Delegate);
    CC_SYNTHESIZE(mj::InAppPackage, _package, Package);
    std::map<std::string, std::tuple<std::string, std::string, std::string, int>> text_mapping;
};

CREATE_READER(MSTabSubscribe_Item);

#endif /* MSTabSubscribe_Item_hpp */
