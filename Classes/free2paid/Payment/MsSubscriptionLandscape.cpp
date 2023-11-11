
#include "MsSubscriptionLandscape.h"
#include "ProfileMiniScene.h"
#include "GettingStartedScene.h"

#include "cocostudio/CocoStudio.h"
#include "CheatMenu.h"
#include "GlobalFunc.h"
#include "AnimationManager.h"
#include "MJSubscriptionGetPaymentInfo.h"
#include "APDatabase.h"
#include "MJVerifyStoreAPI.h"
#include "APPurchaseManager.h"
#include "MKLabelBMFontAnimated.h"
#include "MJHelper.h"
#include "NativeBridge.h"
#include "ShowNewAlert.h"
#include "SoundManager.h"
#include "MSIntro.h"
#include "GettingUserInfoManager.h"
#include "RatingManager.h"
#include "APAuth.h"
#include "MJ_PageLoadResource.h"
#include "APLoginView.h"
#include "F2PManager.h"
#include "MSFirebasePlugin.h"
#include "F2PManager.h"
#include "MJHelper.h"
#include "MSFirebasePlugin.h"
#define NUM_MONTHS_OF_THE_YEAR 12
#define URL_IAP "https://tienganhchobe.monkeystories.vn/rts-ms-gia-moi?app_id=40&firebase_id=%s&utm_source=App&utm_medium=App_Mai_MS&utm_campaign=BangGiaAppMS&utm_content=banggiaMS&device_id=$replacement$##w:deivice_id[0]$/replacement$&amp;users_id=$replacement$##w:monkey_uid[0]$/replacement$&amp;profile_id=$replacement$##w:monkey_profile_id[0]$/replacement$"

USING_NS_CC;
INITIALIZE_READER(MSNewTabSubscribe);

Scene* MSNewTabSubscribe::createScene(monkey::iap::FROM from)
{
    auto scene = Scene::create();

    if (scene)
    {
        scene->setName(MSNEW_TAB_SUBSCRIBE_SCENCE_NAME);
        auto view = MSNewTabSubscribe::createTab(from);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

std::once_flag ms_tab_subscribe_reader_new;
MSNewTabSubscribe* MSNewTabSubscribe::createTab(monkey::iap::FROM from) {
    std::call_once(ms_tab_subscribe_reader_new, [] {
        REGISTER_CSB_READER(MSNewTabSubscribe);

        });
    auto csb_file = std::string("free2paid/payment/MJSubscriptionLandscape.csb");
    auto node = reinterpret_cast<MSNewTabSubscribe*>(CSLoader::createNodeWithVisibleSize(csb_file));
    if (node) {
        node->didLoadFromCSB(from);
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}


void MSNewTabSubscribe::didLoadFromCSB(monkey::iap::FROM from) {

    _screenSize = Director::getInstance()->getVisibleSize();
    _from = from;
    this->setName(MSNEW_TAB_SUBSCRIBE_VIEW_NAME);
    if(_from != monkey::iap::FROM::FREETOWPAID)
    {
        _sourceGotoPayment = MJDEFAULT->getStringForKey(key_source_subscribe, "Clevertap");
    }
    
    if(_from == monkey::iap::FROM::DEFAULT)
    {
        if(auto buttonBack = utils::findChild(this, "button_back"))
        {
            buttonBack->setVisible(false);
        }
    }

#if CC_PLATFORM_HUAWEI
    if (auto btn = utils::findChild(this, "btn_restore")) {
        btn->setVisible(false);
    }
#endif
    //MJSubscription::pushEvent();
    cocos2d::Size screenSize = cocos2d::Director::getInstance()->getVisibleSize();
    float ipad_check = screenSize.width > screenSize.height ? (screenSize.width / screenSize.height) : (screenSize.height / screenSize.width);
    if (ipad_check < 1.7) {

        MK_SAFELY_SET_FONT_SIZE_FOR_TEXT(this, "txt_sub_title", 35);
        if (screenSize.width < screenSize.height) {
            if (auto background = utils::findChild(this, "background_01")) {
                background->setPosition(background->getPosition().x, -28);

            }
        }
    }
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
        MK_SAFELY_SET_FONT_SIZE_FOR_TEXT(this, "lb.sub.review", 13.0f);
        MK_SAFELY_SET_FONT_SIZE_FOR_TEXT(this, "txt_sub_title", 33);
    }
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) {
        MK_SAFELY_SET_FONT_SIZE_FOR_TEXT(this, "lb.sub.review", 13.0f);
        if (auto text = utils::findChild(this, "lb.sub.review")) {
            text->setPositionY(text->getPositionY());
        }
    }

    IAPManager::getInstance()->loadAndUpdatePackageInfoFromAppInfoJson();
    MJDEFAULT->setBoolForKey(KEY_SUBSCRIPTION_IS_BACK, false);

    auto packages = IAPManager::getInstance()->getPackages();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "US");
    k_countryCode = (k_countryCode == "VN" || k_countryCode == "TH" || k_countryCode == "ID"|| k_countryCode == "MY") ? k_countryCode : "US";

    auto currentPackagesOnStoreMap = IAPManager::getInstance()->getCurrentSellingPackagesOnStore();
    auto currentOfflinePackagesOnStoreMap = IAPManager::getInstance()->getCurrentOfflineSellingPackagesOnStore();
    std::vector<std::pair<std::string, InAppPackageModel>> sortedActualPackagess;
    if (currentPackagesOnStoreMap.count(k_countryCode) > 0) { //can't get packages from server => will get offline packages

        std::vector<std::string> packagesOnStore = currentPackagesOnStoreMap.at(k_countryCode);

        for (auto packName : packagesOnStore) {

            if (packages.count(packName) > 0) {
                sortedActualPackagess.push_back({ packName, packages.at(packName) });
                _actualPackages.insert({ packName,packages.at(packName) });
            }
        }
    }
    //offline
    else if (currentOfflinePackagesOnStoreMap.count(k_countryCode) > 0) {

        std::vector<std::string> packagesOnStore = currentOfflinePackagesOnStoreMap.at(k_countryCode);

        for (auto packName : packagesOnStore) {

            if (packages.count(packName) > 0) {
                sortedActualPackagess.push_back({ packName, packages.at(packName) });
                _actualPackages.insert({ packName,packages.at(packName) });
            }
        }
    }

    if (auto lbNew = cocos2d::utils::findChild<ui::Text*>(this, "lb.renew")) {

        if (auto lb = (Label*)lbNew->getVirtualRenderer()) {

            lb->enableUnderline();
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<Node*> subscriptionUIButtons = {
        utils::findChild(this,"top_rounded_rectangle"),
        utils::findChild(this,"center_rectangle"),
        utils::findChild(this,"bottom_rounded_rectangle"),
    };

    if (_actualPackages.size() > subscriptionUIButtons.size()) {

        CCLOG("%s something wrong!", __FUNCTION__);
    }

    std::vector<std::pair<std::string, InAppPackageModel>> sortedActualPackages;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

    for (auto& pName : _actualPackages) {

        sortedActualPackages.push_back({ pName.first,pName.second });
    }
    std::reverse(sortedActualPackages.begin(), sortedActualPackages.end());
#else
    for (auto& pName : sortedActualPackagess) {

        sortedActualPackages.push_back({ pName.first,pName.second });
    }
#endif

    showItems(sortedActualPackages, subscriptionUIButtons);

    //load "cancel anytime" label
    
    if (auto subscribeTile = cocos2d::utils::findChild<ui::Text*>(cocos2d::utils::findChild(this, "button_subscribe"), "lb.subscribe.now"))
    {
        auto newTitle = MKLabelBMFontAnimated::createWithTTF(
            subscribeTile->getString(),
            subscribeTile->getFontName(),
            subscribeTile->getFontSize());

        newTitle->setPosition(subscribeTile->getPosition());
        newTitle->setTextColor(subscribeTile->getTextColor());
        newTitle->setName(subscribeTile->getName());

        subscribeTile->getParent()->addChild(newTitle, subscribeTile->getZOrder());

        newTitle->runAction(RepeatForever::create(Sequence::createWithTwoActions(CallFunc::create([=]() {

            newTitle->animateInSwell(1.0f);

            }), DelayTime::create(8.0f))));

        subscribeTile->removeFromParent();
    }

    auto nodes = this->getChildren();

    std::vector<Node*> children;

    for (auto node : nodes) {
        if (node->getName() != "background")
            children.push_back(node);
    }

    for (int i = 0; i < children.size(); i++) {

        if (auto child = children.at(i)) {

            auto pos = child->getPosition();

            child->setOpacity(0);

            child->setPositionX(child->getPosition().x + _screenSize.width * 0.5f);

            child->runAction(Spawn::createWithTwoActions(FadeIn::create(0.125f), MoveTo::create(0.125f, pos)));
        }
    }

    LANGUAGE_MANAGER->changeDisplayLanguage(this);

    if (auto subscribeTile = cocos2d::utils::findChild<ui::Text*>(this, "txt_sub_title")) {

        std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_subscription");

        MK_SAFELY_SET_TEXT_FOR_NODE(subscribeTile, title);
    }

    std::string k_productID = getCurrentPackageID();

    bool isLifeTimeOr1MonthPackage = (k_productID.find("alllanguage") != std::string::npos);

    if (auto subscribeTile = cocos2d::utils::findChild<MKLabelBMFontAnimated*>(cocos2d::utils::findChild(this, "button_subscribe"), "lb.subscribe.now"))
    {
        MK_SAFELY_SET_TEXT_FOR_NODE(subscribeTile, LANGUAGE_MANAGER->getDisplayTextByKey("pay.action.buy"));
    }

    if (auto label = utils::findChild(this, "lb.sub.restore")) {
        ui::Helper::doLayout(label);
    }

    ui::Helper::doLayout(utils::findChild(this, "btn_restore"));

    if (!IAP_MANAGER->isPlatformSupportIAP()) {

        MK_SAFELY_REMOVE(this, "btn_restore");
    }

}

void MSNewTabSubscribe::showItems(std::vector<std::pair<std::string, InAppPackageModel>> items, std::vector<Node*> subscriptionUIButtons)
{
    int i = 0;

    for (auto package : items) {

        if (i >= subscriptionUIButtons.size()) {
            break;
        }

        if (auto subItem = subscriptionUIButtons.at(i++)) {

            InAppPackageModel packInfo = package.second;

            displaySubscriptionItemWithInfo(utils::findChild<ui::Button*>(subItem, "button"), packInfo);

            if (i == 1) { //make sure we have the default jumping action.
                onSubscriptionItemClickedNotSound(utils::findChild<ui::Button*>(subItem, "button"));
            }

            if (IAP_MANAGER->isLifeTimePackage(package.first)) {

                onSubscriptionItemClickedNotSound(utils::findChild<ui::Button*>(subItem, "button"));
            }
        }
    }
}

void MSNewTabSubscribe::onPayInApp(std::string productID)
{

    if (productID.empty()) {
       
        return;
    }

    if (NativeBridge::networkAvaiable()) {

        IAPManager::getInstance()->onIAP(productID);
    }
    else
    {
        std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
        
        std::string message = LANGUAGE_MANAGER->getDisplayTextByKey("ap_network_error");
        
        std::string ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
        
        ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);

    }
}

void MSNewTabSubscribe::onExitExecute()
{
    std::string productID = "";

    if (auto child = utils::findChild(utils::findChild(this, "sub_items_layout"), "selectedButton")) {

        if (auto packID = utils::findChild<ui::Text*>(child, "sub.item.product.id")) {
            productID = packID->getString();
        }
    }
    this->removeFromParent();
}

void MSNewTabSubscribe::changeDisplayLanguage()
{
    
}

void MSNewTabSubscribe::onProtionItemClicked(std::string productID)
{
//    string k_url = StringUtils::format(URL_IAP, ms::analytics::Firebase::analyticsId.c_str());
//    cocos2d::Application::getInstance()->openURL(k_url);
#if (CC_PLATFORM_HUAWEI || (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_MAC))
    string k_url = StringUtils::format(URL_IAP, ms::analytics::Firebase::analyticsId.c_str());
    cocos2d::Application::getInstance()->openURL(k_url);
#else

    MK_RETURN_IF(productID.empty());
    
    _isclickBuyNow = true;

   // MJLoadingLayer::show();
    mj::helper::showWaitView();
    removePaymentListenerEvents();

    registerFailedEvents();

    IAPManager::getInstance()->checkIAPConditions([=]() {

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            const std::string cannotBuyReason = IAPManager::getInstance()->getFailReason();

            mj::helper::deleteWaitView();

          //  MJLoadingLayer::hide();
            CleverTapManager::GetInstance()->pushEvent("purchase_screen_choose_package",{
                            {"choose_package", cocos2d::Value(mj::helper::getPackageTime(getCurrentPackageID()))},
                            {"verify", cocos2d::Value(cannotBuyReason.empty() ? "true" : "false")}
                        });
            if (!cannotBuyReason.empty()) {

                std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
                
                std::string message = LANGUAGE_MANAGER->getDisplayTextByKey("ap_network_error");
                
                std::string ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
                
                ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);

                return;
            }

            onIAP(productID);

            });
        });
#endif
}

MSNewTabSubscribe::~MSNewTabSubscribe()
{
}

void MSNewTabSubscribe::onEnter() {
    ui::Layout::onEnter();

}

void MSNewTabSubscribe::onClose(cocos2d::Ref* sender)
{
    _isclickBuyNow = false;
    pushCleverTapEvent();
    if (auto bottom = utils::findChild<Layout*>(this->getParent(), "bottom")) {
        if (!bottom->isVisible()) {
            bottom->setVisible(true);
        }
    }

    SOUND_MANAGER->playClickedEffect();
    
    if(_callbackOnclose)
    {
        _callbackOnclose();
    }
    
    this->removeFromParent();
}

void MSNewTabSubscribe::onRestoreButtonClicked(cocos2d::Ref* sender)
{
    SOUND_MANAGER->playClickedEffect();

    removePaymentListenerEvents();

    registerFailedEvents();

    registerSuccessEvents("", true);

    if (IAP_MANAGER->isPlatformSupportIAP()) {
        mj::helper::showWaitView();
        
        IAPManager::getInstance()->onIAPRestore();
    }
}

void MSNewTabSubscribe::onBack(cocos2d::Ref* sender)
{
    if (_callbackOnBack)
    {
        _callbackOnBack();
    }
    this->removeFromParent();

}

void MSNewTabSubscribe::onBeginIap(cocos2d::Ref* sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);

    MK_RETURN_IF(!button);

    if (!IAP_MANAGER->isPlatformSupportIAP()) {

        button->setEnabled(false);

        button->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([=]() {

            button->setEnabled(true);

            })));
    }

    SOUND_MANAGER->playClickedEffect();

    std::string k_productID = getCurrentPackageID();
    onProtionItemClicked(k_productID);
}

void MSNewTabSubscribe::onSubmit(cocos2d::Ref* sender)
{
    _isclickBuyNow = true;
    pushCleverTapEvent();
    std::string k_productID = getCurrentPackageID();
    MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [=]() {
//        CleverTapManager::GetInstance()->pushEvent("purchase_screen_choose_package",{
//            {"choose_package", cocos2d::Value(getCurrentPackageID())}
//            });
        onBeginIap(sender);
        });
}

void MSNewTabSubscribe::onSubscriptionItemClicked(cocos2d::Ref* sender)
{
    SOUND_MANAGER->playClickedEffect();

    _isCountClickPackage = true;
    onSubscriptionItemClickedNotSound(sender);
}


void MSNewTabSubscribe::onSubscriptionItemClickedNotSound(Ref* sender) {

    if (auto parent = utils::findChild(this, "sub_items_layout")) {

        for (auto child : utils::findChildren(*parent, "selectedButton")) {

            child->removeFromParent();
        }
    }

    if (auto button = dynamic_cast<ui::Button*>(sender)) {

        if (auto parent = utils::findChild(this, "sub_items_layout")) {

            if (auto selectedItem = utils::findChild<ui::Button*>(this, "selected_button")) {

                selectedItem->setVisible(false);

                auto item = selectedItem->clone();

                item->setVisible(true);

                item->setName("selectedButton");


                item->setPosition(button->getParent()->getPosition());

                parent->addChild(item, 1000);

                if (auto currentProductIDText = utils::findChild<ui::Text*>(button, "sub.item.product.id")) {

                    if (_actualPackages.count(currentProductIDText->getString()) > 0) {

                        _packInfo = _actualPackages.at(currentProductIDText->getString());

                        bool isLifeTimeOr1MonthPackage = (_packInfo._productId.find("alllanguage") != std::string::npos);

                        if (auto subscribeTile = cocos2d::utils::findChild<MKLabelBMFontAnimated*>(cocos2d::utils::findChild(this, "button_subscribe"), "lb.subscribe.now"))
                        {
                            MK_SAFELY_SET_TEXT_FOR_NODE(subscribeTile, LANGUAGE_MANAGER->getDisplayTextByKey("pay.action.buy"));
                        }

                        displaySubscriptionItemWithInfo(dynamic_cast<ui::Button*>(item), _packInfo);

                        if (_isCountClickPackage)
                        {
                            _currenPackge = IAP_MANAGER->getProductPackage(_packInfo._productId);

                            OnClickSelectButton(nullptr);
                        }
                    }
                }
            }
        }
    }
}

void MSNewTabSubscribe::OnClickSelectButton(cocos2d::Ref* sender)
{
    switch (_currenPackge)
    {
    case monkey::iap::ONE_MONTH:
        _numberClick1MonthPackge++;
        break;
    case monkey::iap::SIX_MONTH:
        _numberClick6MonthPackge++;
        break;
    case monkey::iap::ONE_YEAR:
        _numberClick1YearPackge++;
        break;
    case monkey::iap::LIFT_TIME:
        _numberClickLifeTimePackge++;
        break;
    default:
        break;
    }
}

ui::Widget::ccWidgetClickCallback MSNewTabSubscribe::onLocateClickCallback(const std::string& callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {

        	CLICK_MAP(MSNewTabSubscribe, onSubscriptionItemClicked),
            CLICK_MAP(MSNewTabSubscribe, onClose),
            CLICK_MAP(MSNewTabSubscribe, onSubmit),
            CLICK_MAP(MSNewTabSubscribe, onBack),
            CLICK_MAP(MSNewTabSubscribe, onRestoreButtonClicked),
            CLICK_MAP(MSNewTabSubscribe, OnClickSelectButton),
    };

    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    return nullptr;
}

void MSNewTabSubscribe::displaySubscriptionItemWithInfo(ui::Button* i_button, InAppPackageModel packInfo)
{
    if (!i_button) {

        CCLOG("%s", __FUNCTION__);

        return;
    }

    MK_SAFELY_SET_TEXT(i_button, "sub.item.product.id", packInfo._productId);

    MK_SAFELY_SET_TEXT(i_button, "sub.item.title", LANGUAGE_MANAGER->getDisplayTextByKey(packInfo._productInfoNameKey));

    if (auto originPrice = utils::findChild<ui::Text*>(i_button, "sub.item.original.price")) {

        const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");

        if (IAP_MANAGER->isPlatformSupportIAP()) {

            packInfo._priceOrigin = std::roundf(
                    packInfo._priceSale * (100.f / (100.f - packInfo._saleoff)));
        }

        if ((k_countryCode == "VN" || k_countryCode == "TH") && IAP_MANAGER->getNameProductPackage(packInfo._productId) == "6 month")
        {
            originPrice->setVisible(false);
        }

        originPrice->setString(packInfo.getOriginPriceAsString() + " " + packInfo._currency);
        
        MJDEFAULT->setStringForKey("key_packinfo_currency", packInfo._currency);
        MJDEFAULT->setFloatForKey("key_packinfo_price", packInfo._priceOrigin);

        if (auto strikeThroughLine = utils::findChild<ui::Layout*>(originPrice, "center_line")) {

            strikeThroughLine->setContentSize({ originPrice->getContentSize().width * 1.1f, strikeThroughLine->getContentSize().height });

            strikeThroughLine->setPositionX(originPrice->getContentSize().width / 2);
        }
    }

    if (auto text = utils::findChild<ui::Text*>(i_button, "sub.item.price")) {

        text->setString(packInfo.getSalePriceAsString() + " " + packInfo._currency);

        if (packInfo._priceSale < 0 || packInfo._saleoff <= 0) {

            const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");

            if (k_countryCode != "VN" && k_countryCode != "TH")
            {
                text->setVisible(false);
            }

            if (auto originPrice = utils::findChild<ui::Text*>(i_button, "sub.item.original.price")) {

                originPrice->setTextColor(text->getTextColor());

                if (auto strikeThroughLine = utils::findChild<ui::Layout*>(originPrice, "center_line")) {
                    strikeThroughLine->setVisible(false);
                }
            }
        }
    }


    if (auto text = utils::findChild<ui::Text*>(i_button, "sub.item.discount")) {

        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey(""));

        const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");

        if ((IAP_MANAGER->isLifeTimePackage(packInfo._productId) && (k_countryCode == "VN" || k_countryCode == "TH")) 
            || (IAP_MANAGER->is1YearPackage(packInfo._productId) && k_countryCode != "VN" && k_countryCode != "TH")) {

            text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.popular"));
        }
        else
        {
            std::string str = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.discount.format").c_str(), (int)packInfo._saleoff);
            text->setString(str);
        }

        MK_SAFELY_SET_TEXT_FOR_NODE(text, text->getString());

        CCLOG("i_button->getName():%s", i_button->getName().c_str());

        text->getParent()->setVisible(i_button->getName() == "selectedButton" ? packInfo._saleoff > 0:false);
    }

    if (auto itemPrice = utils::findChild<Text*>(i_button, "sub.item.price")) {
        auto textColor = i_button->getName() == "selectedButton" ? Color4B(Color3B(1, 196, 248)) : Color4B(85, 85, 85, 255);
        itemPrice->setTextColor(Color4B(textColor));
    }

}

void MSNewTabSubscribe::removePaymentListenerEvents() {

    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_FAILURE);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_306);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_307);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_308);

}

void MSNewTabSubscribe::registerFailedEvents() {

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_306, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_306);

        std::unordered_map<std::string, std::string>* data = static_cast<std::unordered_map<std::string, std::string>*>(eventCustom->getUserData());

        std::unordered_map<std::string, std::string> map = (data != nullptr ? *data : std::unordered_map<std::string, std::string>());

        CC_SAFE_DELETE(data);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJLoadingLayer::hide();

            std::string platform_name = map.count("platform_name") > 0 ? map.at("platform_name") : "--";
            std::string time_purchase = map.count("time_purchase") > 0 ? map.at("time_purchase") : "--";

            std::string ok, message;
            ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
            message = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.purchase.address").c_str(), platform_name.c_str(), time_purchase.c_str());
            
            std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");

            StoryAlert::showAlert(message, ok, "can", false, 20, nullptr, 0, "", message, 30);
            
           // ShowNewAlert::createAlert(title, message, ok, ok, true, nullptr, true, false);

            });

        });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_307, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_307);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            std::string ok, message;

            ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
            message = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.purchase1");
            
            std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");

            ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);

            });

        });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_308, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_308);

        std::string* data = static_cast<std::string*>(eventCustom->getUserData());

        std::string userName = data != nullptr ? std::string(*data) : "";

        CC_SAFE_DELETE(data);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJLoadingLayer::hide();

            std::string ok, message;
            ok = LANGUAGE_MANAGER->getDisplayTextByKey("key.logout");
            message = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.register.message").c_str(), userName.c_str());
            
            std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
            
           // auto alert = ShowNewAlert::createAlert(title, message, ok, ok, true, nullptr, true, false);

            auto alert = StoryAlert::showAlert(message, ok, "can", false, 28, nullptr, 0, "", message, 30);
            
            alert->setOkButtonCallback([=] () {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom*) {
                        MK_RUN_IN_COCOS_THREAD(Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
                        this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");)
                        }), this);

                    logOutApp();
                    });
            });


            });

        });

}

void MSNewTabSubscribe::logOutApp()
{
    auto waiting_layout = Layout::create();
    waiting_layout->setContentSize(Director::getInstance()->getVisibleSize());
    waiting_layout->setBackGroundColor(Color3B::BLACK);
    waiting_layout->setBackGroundColorType(BackGroundColorType::SOLID);
    waiting_layout->setBackGroundColorOpacity(200);
    waiting_layout->setTouchEnabled(true);
    waiting_layout->setName("waiting_logout");
    Director::getInstance()->getRunningScene()->addChild(waiting_layout);

    auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
    waiting_layout->addChild(indicator);
    indicator->setPosition(waiting_layout->getContentSize() / 2);
    indicator->setVisible(true);
    indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
    ap::Auth::signOut(this);
    //reload data

    auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom* event)
        {

            ms::EventLogger::getInstance().logEvent("event_signout_done", {}); cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_successfully");
            MJDEFAULT->setBoolForKey(key_onboarding_license, false);
            MJDEFAULT->setBoolForKey(key_logged_in, false);
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                auto scene = MSIntro::createScene();
                Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
                });
        });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom* event) {
        ms::EventLogger::getInstance().logEvent("event_signout_fail", {});
        cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
        waiting_layout->removeFromParent();

        });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);

    //delete unused data key.
    GettingUserInfoManager::GetInstance()->deleteData();
    RatingManager::GetInstance()->deleteData();

    mj::PageLoadResource::getInstance().deleteCacheAllStory();
    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
    FileUtils::getInstance()->purgeCachedEntries();
}

void MSNewTabSubscribe::registerSuccessEvents(std::string k_productID, bool isRestore) {

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_PURCHASED_FAILURE, [=](EventCustom* eventCustom) {//show popup get user info

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_FAILURE);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");
            
            MJSubscriptionGetPaymentInfo::Type typePop = MJDEFAULT->getStringForKey(key_popup_fail_payment, "phone") == "phone" ? MJSubscriptionGetPaymentInfo::Type::PHONE : MJSubscriptionGetPaymentInfo::Type::EMAIL;

            auto view = MJSubscriptionGetPaymentInfo::createView(typePop, "");
            mj::api::VerifyStoreAPI::_isCallStore = false;
            view->_sourceGotoPayment = _sourceGotoPayment;
            _paymentSuccessful = false;
            std::string k_productID = getCurrentPackageID();
            pushEventChoosePackage(k_productID);
            this->addChild(view, INT_MAX - 1);
            return;

            if (!MJDEFAULT->getBoolForKey(key_is_submit, false))
            {
                if (k_countryCode == "VN" || k_countryCode == "TH" || k_countryCode == "US")
                {
                    MJSubscriptionGetPaymentInfo::Type typePop = MJDEFAULT->getStringForKey(key_popup_fail_payment, "phone") == "phone" ? MJSubscriptionGetPaymentInfo::Type::PHONE : MJSubscriptionGetPaymentInfo::Type::EMAIL;

                    auto view = MJSubscriptionGetPaymentInfo::createView(typePop, "");

                    this->addChild(view, INT_MAX - 1);
                }
                else
                {

                    std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
                    
                    std::string message = LANGUAGE_MANAGER->getDisplayTextByKey("pay.purchase.failed");
                    
                    std::string ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");

                    StoryAlert::showAlert(message, ok, "Close");
                    
                    //ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);
                    
                }
            }

            });

        });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS, [=](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJParentPopup::PopupInfo purchaseSuccessInfo;

            std::string title, message, ok;

            title = LANGUAGE_MANAGER->getDisplayTextByKey(isRestore ? "subscription.restore.success" : "subscription.purchase.success");
            message = isRestore ? title : LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.access.all");
            ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.start.learning.now");

            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom* e) {

                this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));

                    });

                }), this);
            
            std::function<void()> callbackOnpaymentSucess = nullptr;
            
            if (!isRestore) {
                _paymentSuccessful = true;
                std::string k_productID = getCurrentPackageID();
                pushEventChoosePackage(k_productID);
            }

            if(_from == monkey::iap::FROM::FREETOWPAID&& !isRestore)
            {
                ShowNewAlert* alert = nullptr;
                message = LANGUAGE_MANAGER->getDisplayTextByKey("message.payment.sucess");
                title = LANGUAGE_MANAGER->getDisplayTextByKey("title.payment.sucess");
                
                ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
                
                callbackOnpaymentSucess = [=] () {
                    CallAPILoadUpdate();
                };
                
                F2P_MANAGER->setKeyF2P_Paid(true);
                
                alert = ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);
                
                alert->setSizeTextMessage(Size(450, 115));
                alert->setFontSizeTextMessage(24.0);
                alert->setFontSizeTextTitle(25);
                
                alert->setOnOkCallback([=] () {
                    if(callbackOnpaymentSucess)
                    {
                        callbackOnpaymentSucess();
                    }
                });

                
                
            }else{
                
                StoryAlert* alert = nullptr;
                
                callbackOnpaymentSucess = [=] () {
                    if(F2P_MANAGER->isSkipUser())
                    {
                        CallAPILoadUpdate();
                    }
                    else{
                        
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
                        
                    }
                };
                
                alert = StoryAlert::showAlert(message, ok, title, false, 30, nullptr, 0, "", "", 32);
                
                alert->setOkButtonCallback([=] () {
                    if(callbackOnpaymentSucess)
                    {
                        callbackOnpaymentSucess();
                    }
                });
            }
            });

        });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS, [=](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS);

        std::string message = "";

        std::unordered_map<std::string, std::string > newMap = std::unordered_map<std::string, std::string >();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)||(CC_TARGET_PLATFORM == CC_PLATFORM_MAC )

        std::unordered_map<std::string, std::string >* map = static_cast<std::unordered_map<std::string, std::string>*>(eventCustom->getUserData());

        newMap = map != nullptr ? *map : std::unordered_map<std::string, std::string >();


        CC_SAFE_DELETE(map);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string* data = static_cast<std::string*>(eventCustom->getUserData());

        message = data != nullptr ? std::string(*data) : "";

        CC_SAFE_DELETE(data);
#endif
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            std::string messageA, title, ok;
            title = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.changesuccess");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            messageA = message;
#else
            std::string packageRenew = newMap.count("package_renew") > 0 ? newMap.at("package_renew") : "";

            int timeRenew = -1;

            if (newMap.count("time_renew") > 0) {

                SAFE_EXECUTE_CODE
                (
                    timeRenew = stoi(newMap.at("time_renew"));
                )
            }

            std::string priceStr = "<price>";

            std::string packageName = "<package_name>";

            if (IAP_MANAGER->getPackages().count(packageRenew) > 0) {

                auto packInfo = IAP_MANAGER->getPackages().at(packageRenew);

                priceStr = packInfo.getSalePriceAsString() + " " + packInfo._currency;

                packageName = LANGUAGE_MANAGER->getDisplayTextByKey(packInfo._productInfoNameKey);
            }

            std::string timeRenewStr = mj::helper::getStringTime(timeRenew, "%R %d/%m/%G");

            std::string str = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.changesubscription2");

            messageA = StringUtils::format(str.c_str(), packageName.c_str(), priceStr.c_str(), timeRenewStr.c_str());

#endif
            ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.start.learning.now");

            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom* e) {

                this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));

                    });

                }), this);
            
            std::function<void()> callbackOnpaymentSucess = nullptr;

            _paymentSuccessful = true;
            std::string k_productID = getCurrentPackageID();
            pushEventChoosePackage(k_productID);

            if(_from == monkey::iap::FROM::FREETOWPAID)
            {
                messageA = LANGUAGE_MANAGER->getDisplayTextByKey("message.payment.sucess");
                title = LANGUAGE_MANAGER->getDisplayTextByKey("title.payment.sucess");
                
                ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
                
                callbackOnpaymentSucess = [=] () {
                    CallAPILoadUpdate();
                };
                
                ShowNewAlert* alert = ShowNewAlert::createAlert(title, messageA, ok, ok, _from != monkey::iap::FROM::FREETOWPAID, nullptr, true, false);
                
                alert->setSizeTextMessage(Size(450, 115));
                alert->setFontSizeTextMessage(24.0);
                alert->setFontSizeTextTitle(25);
                
                alert->setOnOkCallback([callbackOnpaymentSucess] () {
                    mj::helper::deleteWaitView();
                    if(callbackOnpaymentSucess)
                    {
                        callbackOnpaymentSucess();
                    }
                    //onlogin
                });

                alert->setOnCloseCallback([callbackOnpaymentSucess]() {
                    mj::helper::deleteWaitView();
                    if (callbackOnpaymentSucess)
                    {
                        callbackOnpaymentSucess();
                    }
                    //onlogin
                    });
                
                F2P_MANAGER->setKeyF2P_Paid(true);
                
                alert->setOnCancelCallback([] () {
                    mj::helper::deleteWaitView();
                });
                
                
            }else{
                callbackOnpaymentSucess = [] () {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
                        });
                };
                
                auto alert = StoryAlert::showAlert(messageA, ok, title, false, 28, nullptr, 0, "", messageA, 30);
                
                alert->setOkButtonCallback([=]() {

                    mj::helper::deleteWaitView();
                    if(callbackOnpaymentSucess)
                    {
                        callbackOnpaymentSucess();
                    }
                });

                alert->setCloseButtonCallback([]() {
                    mj::helper::deleteWaitView();
                });
                
            }

        });

});

}

void MSNewTabSubscribe::CallAPILoadUpdate()
{
    mj::helper::showWaitView();
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ap::api::LoadUpdate::call(CC_CALLBACK_2(MSNewTabSubscribe::APILoadUpdateCallBack, this)); });
            }),
        NULL));

}

void MSNewTabSubscribe::APILoadUpdateCallBack(bool isSucess, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::REGISTER, StartLoginFrom::Default, StartRegisterFrom::MSNewTabSubscribe);

        loginView->setName("loginView");
        Director::getInstance()->getRunningScene()->addChild(loginView);

        animationMoveToRight(loginView);
        mj::helper::deleteWaitView();
        this->removeFromParent();
    });

}
void MSNewTabSubscribe::onIAP(std::string k_productID)
{
    MK_RETURN_IF(k_productID.empty());

    removePaymentListenerEvents();

    registerSuccessEvents(k_productID, false);
    
    registerFailedEvents();
    
    onIAPInapp(k_productID);

}


void MSNewTabSubscribe::onIAPInapp(std::string productID)
{
    mj::helper::showWaitView();
    IAPManager::getInstance()->checkIAPConditions([=]() {

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            const std::string cannotBuyReason = IAPManager::getInstance()->getFailReason();

            if (!cannotBuyReason.empty()) {
                return;
            }

            if (productID.empty()) {
                CCLOG("%s cannot get product id!", __FUNCTION__);
                return;
            }

            if (NativeBridge::networkAvaiable()) {
                IAPManager::getInstance()->onIAP(productID);
            }
            else
            {
                IAP_MANAGER->showErrorPopupByCode(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.error.network"));
            }

            });
        });
}

std::string MSNewTabSubscribe::getCurrentPackageID()
{
    std::string k_productID = "";

    if (auto child = utils::findChild(utils::findChild(this, "sub_items_layout"), "selectedButton")) {

        if (auto packID = utils::findChild<ui::Text*>(child, "sub.item.product.id")) {

            k_productID = packID->getString();
        }
    }
    return k_productID;
}


void MSNewTabSubscribe::onExit()
{
    removePaymentListenerEvents();
    ui::Layout::onExit();
   
}

void MSNewTabSubscribe::pushCleverTapEvent()
{
    MJDEFAULT->deleteValueForKey(key_source_subscribe);
    
    std::string userTypeText = APPURCHASE_MANAGER->getStringTypeUser(_from);
    if (MJDEFAULT->getBoolForKey("do_not_buy_and_expire"))
    {
        if ((ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4) == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
        {
            userTypeText = "free";
        }
        else
        {
            userTypeText = "trial";
        }
        MJDEFAULT->setBoolForKey("do_not_buy_and_expire", false);
    }

    std::vector<std::pair<std::string, cocos2d::Value>> value = {
        {"click_type", cocos2d::Value(_isclickBuyNow ? "buy_now" : "close")},
        {"user_type", cocos2d::Value(userTypeText)},
        {"source", cocos2d::Value(_sourceGotoPayment)},
        {"frequency_of_click_1m_package", cocos2d::Value(_numberClick1MonthPackge)},
        {"frequency_of_click_6m_package", cocos2d::Value(_numberClick6MonthPackge)},
        {"frequency_of_click_1y_package", cocos2d::Value(_numberClick1YearPackge)},
        {"frequency_of_click_lifetime_package", cocos2d::Value(_numberClickLifeTimePackge)},
    };

    CleverTapManager::GetInstance()->pushEvent("purchase_screen_view", value);
}

void MSNewTabSubscribe::pushEventChoosePackage(std::string packgeId)
{
    std::string userTypeText = APPURCHASE_MANAGER->getStringTypeUser(_from);

    if (MJDEFAULT->getBoolForKey("do_not_buy_and_expire"))
    {
        if ((ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4) == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
        {
            userTypeText = "free";
        }
        else
        {
            userTypeText = "trial";
        }
        MJDEFAULT->setBoolForKey("do_not_buy_and_expire", false);
    }
    
    std::string choosePackgeId = IAP_MANAGER->getNameProductPackage(packgeId);
   
    std::vector<std::pair<std::string, cocos2d::Value>> value = {
        {"choose_package", cocos2d::Value(choosePackgeId)},
        {"payment_successful", cocos2d::Value(_paymentSuccessful)},
        {"user_type", cocos2d::Value(userTypeText)},
    };
    
    if(_from == monkey::iap::FROM::FREETOWPAID)
    {
        value.push_back({"source", cocos2d::Value(_sourceGotoPayment)});
    }

    CleverTapManager::GetInstance()->pushEvent("purchase_screen_popup_confirm", value);
}

void MSNewTabSubscribe::pushEventPopupConfirm(std::string typeClick, bool isSucces)
{
    CleverTapManager::GetInstance()->pushEvent("purchase_screen_popup_confirm", {
        {"click_type",cocos2d::Value(typeClick)},
        {"source",cocos2d::Value(_sourceGotoPayment)},
        {"purchased_successful",cocos2d::Value(isSucces)},
        });
}


void MSNewTabSubscribe::runActionSequenAnimation()
{
    auto posx = this->getPosition();
    
    this->setPositionY(-100);
    
    this->runAction(Sequence::create(DelayTime::create(0.2f),
                                     EaseSineOut::create(MoveTo::create(0.5, posx))
                                     , NULL));
}
