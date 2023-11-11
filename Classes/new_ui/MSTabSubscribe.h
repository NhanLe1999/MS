//
//  MSTabSubscribe.hpp
//  MJStory
//
//  Created by Hoi Sung on 12/15/17.
//

#ifndef MSTabSubscribe_hpp
#define MSTabSubscribe_hpp

#include "cocostudiomacro2.h"
#include "MJPay.h"
#include "MSTabSubscribe_Item.h"
#include "MJHelper.h"
#include "IAPManager.h"
#include "MSSidebarDelegate.h"
#include "APLoginView.h"
#include "APPopupBase.h"

class MSTabSubscribe_Item;

class MSTabSubscribe : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::SubscriptionItem, public mj::RequestPriceDelegate, public StoryAlertDelegate {
public:
    CREATE_FUNC(MSTabSubscribe);
    static MSTabSubscribe * createTab();
    static MSTabSubscribe * createTabVoucher();
    void setContentSize(const cocos2d::Size & size) override;
    
    void runAnimationAppear();
    void runAnimationDisappear();
    void pushCleverTapEvent();

    void pushEventPopupConfirm(std::string typeClick, bool isSucces);

    void onExit() override;
    
private:
	~MSTabSubscribe();
    void changeDisplayLanguage() override;
    void showPriceInfo();
    void didLoadFromCSB(bool dumy = false);
    void changeTextDisplayLang();
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onClose(cocos2d::Ref *sender);
    void createLayoutRun();
    void onStartYourFreeWeek(cocos2d::Ref *sender);
    cocos2d::ui::Scale9Sprite *layout_run;
    cocos2d::ui::Layout *layout_check_pos, *layout_content;
    void updateCheckPos(float dt);
    cocos2d::Size win_size;
    bool isrun_down = false, isrun_up = false;
    CC_SYNTHESIZE(ms::SidebarDelegate*, _delegate, Delegate);
    void onSelectProduct(mj::InAppPackage package) override;
    void onGetPriceInfo(mj::ListPackages packages) override;
    void resetPackget(mj::ListPackages);

    std::function<void(mj::InAppPackage package)> _callbackIsClickChoosePackge = nullptr;
    
    mj::PayMethod _pay_method;
    mj::ListPackages _packages;
    mj::ListPackages _packagesCopy;
    std::vector<MSTabSubscribe_Item *> _package_items;
    void onRestoreClicked(cocos2d::Ref *sender);
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    void onDetail(cocos2d::Ref * sender);
    void setCallbackOnclickChoosePackge();
	void registerSuccessEvents(std::string k_productID, bool isRestore);
    void onIAPInapp(std::string productID);

    void onIAP(std::string k_productID);
    void onPayInApp(std::string productID);
    void removePaymentListenerEvents();
    void registerFailedEvents();
    void logOutApp();
    
    void CallAPILoadUpdate();
    void APILoadUpdateCallBack(bool isSucess, std::string msg);
    std::unordered_map<std::string, InAppPackageModel> _actualPackages;
    
    mj::InAppPackage _product;
    
    bool _has_voucher = false;

	std::string choose_pakage	= "none",
				click_type		= "none";
    std::string _source = "";
    bool _isclickBuyNow = false;
};

CREATE_READER(MSTabSubscribe);

#endif /* MSTabSubscribe_hpp */
