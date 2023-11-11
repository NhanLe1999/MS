
#ifndef MSNewTabSubscribe_hpp
#define MSNewTabSubscribe_hpp

#include "cocostudiomacro2.h"
#include "StoryAlert.h"
#include "IAPManager.h"
#include "MKDefinitions.h"
#include "MJCalculator.h"
#include "APPopupBase.h"
#define MSNEW_TAB_SUBSCRIBE_SCENCE_NAME "msnew_tab_subscribe_scence_name"
#define MSNEW_TAB_SUBSCRIBE_VIEW_NAME "msnew_tab_subscribe_view_name"

class MSNewTabSubscribe : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate{
public:
    CREATE_FUNC(MSNewTabSubscribe);
    
    static cocos2d::Scene* createScene(monkey::iap::FROM from = monkey::iap::FROM::DEFAULT);
    
    static MSNewTabSubscribe* createTab(monkey::iap::FROM from = monkey::iap::FROM::DEFAULT);
    static MSNewTabSubscribe* createTabVoucher();

    static MSNewTabSubscribe* show(cocos2d::Node* parent);

    static MSNewTabSubscribe* showLanscapeScreen(cocos2d::Node* parent);

    static MSNewTabSubscribe* createView();

    void onPayInApp(std::string productID);

    void onExitExecute();

    void onProtionItemClicked(std::string packageID);
    
    void runActionSequenAnimation();
    
    void changeDisplayLanguage() override;

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _customBackAction, CustomBackAction, nullptr);

    void setStartFrom(std::string from);

    std::string getStartFrom();
    
    void setCallbackOnClose(std::function<void()> callback)
    {
        _callbackOnclose = callback;
    };
    void setCallbackOnBack(std::function<void()> callback)
    {
        _callbackOnBack = callback;
    };
    
    inline void setSourceForPayment(std::string source)
    {
        _sourceGotoPayment = source;
    };

    void pushCleverTapEvent();
    void CallAPILoadUpdate();
private:
    ~MSNewTabSubscribe();
    
    void didLoadFromCSB(monkey::iap::FROM from);
    void onEnter() override;
    void onClose(cocos2d::Ref* sender);
    void onSubscriptionItemClicked(cocos2d::Ref* sender);
    void onRestoreButtonClicked(cocos2d::Ref* sender);
    void onBack(cocos2d::Ref* sender);
    void onSubmit(cocos2d::Ref* sender);
    void OnClickSelectButton(cocos2d::Ref* sender);

    void showItems(std::vector<std::pair<std::string, InAppPackageModel>> items, std::vector<Node*> subscriptionUIButtons);

    void onExit() override;

    void onSubscriptionItemClickedNotSound(Ref* sender);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& name) override;

    void displaySubscriptionItemWithInfo(ui::Button* i_button, InAppPackageModel packInfo);

    void removePaymentListenerEvents();

    void registerFailedEvents();

    void registerSuccessEvents(std::string k_productID, bool isRestore);

    void APILoadUpdateCallBack(bool isSucess, std::string msg);

    void onIAP(std::string pID);

    void logOutApp();

    void onIAPInapp(std::string productID);

    std::string getCurrentPackageID();

    void onBeginIap(cocos2d::Ref* sender);
    
    void pushEventChoosePackage(std::string packgeId);
    
    void pushEventPopupConfirm(std::string typeClick, bool isSucces);

    std::unordered_map<std::string, InAppPackageModel> _actualPackages;

    bool _isOnboardingFlow = false;
    
    std::function<void()> _callbackOnclose = nullptr, _callbackOnBack = nullptr;

    Size _screenSize;   

    InAppPackageModel _packInfo;
    
    monkey::iap::FROM _from = monkey::iap::FROM::DEFAULT;
    
    std::string _sourceGotoPayment = "";
    
    bool _isclickBuyNow = false;
    bool _paymentSuccessful = false;

    int _numberClick1MonthPackge = 0;
    int _numberClick6MonthPackge = 0;
    int _numberClick1YearPackge = 0;
    int _numberClickLifeTimePackge = 0;
    bool _isCountClickPackage = false;
    monkey::iap::PRODUCT_PACKAGES _currenPackge;
};

CREATE_READER(MSNewTabSubscribe);

#endif /* MSNewTabSubscribe_hpp */
