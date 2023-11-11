//
//  MJSubscriptionPage.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJSubscriptionPage_hpp
#define MJSubscriptionPage_hpp

#include "CocoStudioMacro.h"
#include "MJSubscriptionItem.h"
#include "MJRequest.h"
#include "MJInAppInfo.h"
#include "MJHelper.h"
#include "MJInAppInfo.h"
#include "ui/CocosGUI.h"

#include "MJPay.h"

using namespace cocos2d;
using namespace std;
using namespace ui;
class MJSubscriptionPage : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJSubscriptionItemDelegate, public mj::RequestPriceDelegate {
public:
    CREATE_FUNC(MJSubscriptionPage);
    static MJSubscriptionPage * createPage(const int & discount);
    void changeTextDisplayLang();
    void loadData();
    void showCalculator();

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    void onSelectProduct(mj::InAppPackage product) override;
    void onBack(cocos2d::Ref * sender);
    void onRestore(cocos2d::Ref * sender);
    void onPayDetails(cocos2d::Ref * sender);
    void onPrivacy(cocos2d::Ref * sender);
    void onTOS(cocos2d::Ref * sender);

//    void onRequestSuccess(int tag, rapidjson::Value data) override;
//    void onRequestFailed(int tag, int error_code, std::string message) override;
    void onGetPriceInfo(mj::ListPackages packages) override;

    mj::ListPackages _packages;

    void showPriceInfo();


    int _discount;
    mj::PayMethod _pay_method;
    
    std::string classes_name;
    std::vector<MJSubscriptionItem *> product_item;
    void onInfoPageKage(cocos2d::Ref *sender);
    
};

CREATE_READER_CLASS(MJSubscriptionPageReader, MJSubscriptionPage);

#endif /* MJSubscriptionPage_hpp */
