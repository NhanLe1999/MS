//
//  MJPaySubPopup.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPaySubPopup_hpp
#define MJPaySubPopup_hpp

#include "CocoStudioMacro.h"
#include "MJRequest.h"
#include "MJInAppInfo.h"
#include "MJHelper.h"

class MJPaySubPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    static MJPaySubPopup * createPopup(mj::PayMethod method, mj::ListPackages packages, std::string product_id, int discount);

protected:
    virtual void didLoadFromCSB(mj::ListPackages packages, std::string product_id, int _discount);

    std::string _product_id;
    int _discount;
    mj::ListPackages _packages;
    mj::PayMethod _pay_method;

    virtual void onSwitch(cocos2d::Ref * sender);
    virtual void onNext(cocos2d::Ref * sender) = 0;
    virtual void onBack(cocos2d::Ref * sender);

    void reloadPriceTag();

private:
};

#endif /* MJPaySubPopup_hpp */
