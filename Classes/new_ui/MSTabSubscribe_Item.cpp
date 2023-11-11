//
//  MSTabSubscribe_Item.cpp
//  MJStory
//
//  Created by Hoi Sung on 12/15/17.
//

#include "MSTabSubscribe_Item.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MJPay.h"
#include "StoryConfigManager.h"
#include "MSFirebasePlugin.h"

USING_NS_CC;
INITIALIZE_READER(MSTabSubscribe_Item);

const std::map<std::string, int> ORIGINAL_PRICE = {
    {"com.earlystart.stories.vn.6month", 582000},
    {"com.earlystart.stories.vn.1year", 665000},
    {"com.earlystart.stories.vn.lifetime", 1999000}
};

MSTabSubscribe_Item * MSTabSubscribe_Item::createTab() {
    auto node = reinterpret_cast<MSTabSubscribe_Item *>(CSLoader::createNode("csb/new_ui/Subscribe_Item.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSTabSubscribe_Item::didLoadFromCSB() {
    this->setClippingEnabled(true);
    
    changeTextDisplayLang();
}
void MSTabSubscribe_Item::onEnter() {
    ui::Layout::onEnter();
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
}

void MSTabSubscribe_Item::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
}

void MSTabSubscribe_Item::reloadPrice() {
    
    auto packages = mj::Pay::getInstance().getPackages();
    
    auto it = std::find_if(std::begin(packages), std::end(packages), [=](mj::InAppPackage p) {
        return p.product_id == _package.product_id;
    });
    
    if (it != std::end(packages)) {
        _package = * it;
    }
    
    auto is_vn = CONFIG_MANAGER->isCountryVN();
    
    double      actual_price = 0;
    std::string currency_code = "";
  
    actual_price    = _package.getActualPrice(40);
    currency_code   = "₫";
   
    //get price from store
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && !CC_PLATFORM_HUAWEI))
    actual_price = _package.getStorePrice(40);
    currency_code = _package.store_currency_code;
#else
	actual_price = _package.getActualPrice(40);
	currency_code = is_vn?currency_code:_package.currency;
#endif
    
    std::string details_billed, plan_description, button_action;

    std::vector<std::string> timePackge = { "1month", "6month", "1year", "lifetime" };

    std::string purchasePackageTime = "";

    for (int i = 0; i < timePackge.size(); i++)
    {
        if (_package.product_id.find(timePackge[i]) != std::string::npos)
        {
            purchasePackageTime = timePackge[i];
        }
    }

    std::string time_unit = "";


    if (purchasePackageTime == "1month")
    {
        time_unit = std::string("1 ") + LANGUAGE_MANAGER->getDisplayTextByKey("price.time.small.unit");
        plan_description = LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.1mon");
        button_action = LANGUAGE_MANAGER->getDisplayTextByKey("sub.text.subscribenow");
    }
    else if (purchasePackageTime == "6month") 
    {
        time_unit = std::string("6 ") + LANGUAGE_MANAGER->getDisplayTextByKey("price.time.small.unit");
        plan_description =  LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.6mon");
        button_action = LANGUAGE_MANAGER->getDisplayTextByKey("sub.text.subscribenow");
    } 
    else if (purchasePackageTime == "1year")
    {
        time_unit = std::string("1 ") + LANGUAGE_MANAGER->getDisplayTextByKey("price.time.great.unit");
        plan_description = LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.1year");
        button_action = LANGUAGE_MANAGER->getDisplayTextByKey("sub.text.startyourweek");
    }
    else {
        plan_description = LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.lifetime");
        button_action = LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.purchase");
    }

    int number_of_months;
    
    auto split = mj::helper::split(_package.product_id, '.');
    auto duration = * split.rbegin();
    

    if (actual_price == 0) {
        actual_price = MJDEFAULT->getFloatForKey(_package.product_id + ".store_price", 0);
    }
    
    if (currency_code.empty()) {
        currency_code = MJDEFAULT->getStringForKey(_package.product_id + ".store_currency", "");
    }
    

    auto text_details = utils::findChild<ui::Text *>(this, "text.detail");
    text_details->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Italic.ttf");
    auto time = _package.product_id.substr(_package.product_id.find_last_of(".")+1);
    text_details->setString(LANGUAGE_MANAGER->getDisplayTextByKey("price.text.detail"));

    text_details->setVisible(false);

    auto text_plan = utils::findChild<ui::Text *>(this, "text.title");
    text_plan->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
    text_plan->setString(plan_description);
    
    auto text_action = utils::findChild<ui::Text *>(this, "button.title");
    text_action->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    text_action->setString(button_action);
    
    auto text_price = utils::findChild<ui::Text *>(this, "text.price");
    auto text_price_sub = utils::findChild<ui::Text *>(this, "text.price.time");
    auto text_separate = utils::findChild<ui::Text *>(this, "text.separate");

    if (actual_price != 0) {
        MJDEFAULT->setFloatForKey(_package.product_id + ".store_price", actual_price);
        MJDEFAULT->setStringForKey(_package.product_id + ".store_currency", currency_code);
            if (is_vn) {
                _propertyPrice = mj::helper::prettyInteger(actual_price);
                text_price->setString(StringUtils::format("%s%s", _propertyPrice.c_str(), currency_code.c_str()));
                
                if(text_price_sub){
                    text_price_sub->setString(StringUtils::format("%s",time_unit.c_str()));
                    auto width = text_price->getContentSize().width + text_separate->getContentSize().width + text_price_sub->getContentSize().width;
                    auto space = (this->getContentSize().width - width)/2;
                    text_price->setPositionX(space);
                    text_separate->setPositionX(space + text_price->getContentSize().width);
                    text_price_sub->setPositionX(text_separate->getPositionX() + text_separate->getContentSize().width);
                }
                
            } else {
                if (CONFIG_MANAGER->isCountryVN()) {
                    _propertyPrice = mj::helper::prettyPrice(actual_price, currency_code);
                    text_price->setString(StringUtils::format("%s%s", _propertyPrice.c_str(), currency_code.c_str()));
                } else {
                    _propertyPrice = mj::helper::prettyPrice(actual_price, currency_code);
                    text_price->setString(StringUtils::format("%s%s", currency_code.c_str(), _propertyPrice.c_str()));
                }
                
                if(text_price_sub){
                    text_price_sub->setString(StringUtils::format("%s", time_unit.c_str()));
                    auto width = text_price->getContentSize().width + text_separate->getContentSize().width + text_price_sub->getContentSize().width;
                    auto space = (this->getContentSize().width - width)/2;
                    text_price->setPositionX(space);
                    text_separate->setPositionX(space + text_price->getContentSize().width);
                    text_price_sub->setPositionX(text_separate->getPositionX() + text_separate->getContentSize().width);
                }
            }
    } else {
        text_price->setString("---");
    }
    
    auto text_price_original = utils::findChild<ui::Text *>(this, "text.price.original");
    if (_package.price_original != 0) {
        auto original_price = _package.price_original;
        if (CONFIG_MANAGER->isCountryVN()) {
            text_price_original->setString(StringUtils::format("%s%s", mj::helper::prettyPrice(original_price,"").c_str(), currency_code.c_str()));
        } else {
            text_price_original->setString(StringUtils::format("%s%s", currency_code.c_str(), mj::helper::prettyPrice(original_price,currency_code).c_str()));
        }
    } else {
        text_price_original->setVisible(false);
        utils::findChild(this, "strikethrough")->setVisible(false);
    }

    if (!CONFIG_MANAGER->isAppVNStories()) {
        text_price_original->setVisible(false);
        utils::findChild(this, "strikethrough")->setVisible(false);
    } else if (!CONFIG_MANAGER->isCountryVN()) {
        text_price_original->setVisible(false);
        utils::findChild(this, "strikethrough")->setVisible(false);
    }
    if (text_price_sub){
        text_price_sub->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-SemiBold.ttf");
    }
    layoutPrice();
}

void MSTabSubscribe_Item::layoutPrice() {
    auto text_price = utils::findChild<ui::Text *>(this, "text.price");
    auto text_duration = utils::findChild<ui::Text *>(this, "text.price.time");
    
    text_price->setContentSize(text_price->getVirtualRendererSize());
    if (text_duration) {
        text_duration->setContentSize(text_duration->getVirtualRendererSize());
        auto sz = text_duration->getContentSize();
        text_duration->getParent()->setContentSize(text_duration->getContentSize());
        text_duration->setPosition(Point(0, 4));
    }
    
    auto listview_price = utils::findChild<ui::ListView *>(this, "listview_price");
    listview_price->setTouchEnabled(false);
    auto listview_width = 0;
    for (auto c : listview_price->getItems()) {
        listview_width += c->getContentSize().width;
    }
    
    listview_price->setContentSize(Size(listview_width, listview_price->getContentSize().height));
    listview_price->forceDoLayout();
    
    auto text_price_original = utils::findChild<ui::Text *>(this, "text.price.original");
//    auto bb = text_price->getBoundingBox();
//    bb.origin = text_price->getParent()->convertToWorldSpace(bb.origin);
//    auto pos = Point(bb.getMidX(), bb.getMidY());
//    pos = text_price_original->getParent()->convertToNodeSpace(pos);
    text_price_original->setPosition(Point(this->getContentSize().width / 2, text_price_original->getPositionY()));
    text_price_original->setContentSize(text_price_original->getVirtualRendererSize());
    
    auto strikethrough = utils::findChild(this, "strikethrough");
    strikethrough->setContentSize(Size(text_price_original->getContentSize().width + 5, 2));
    strikethrough->setPosition(text_price_original->getPosition());
}

void MSTabSubscribe_Item::changeTextDisplayLang(){
//    utils::findChild<ui::Text *>(this, "free.story.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.story.title"));
//    utils::findChild<ui::Text *>(this, "free.lesson.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.lesson.title"));

}

void MSTabSubscribe_Item::onBuyClicked(cocos2d::Ref *sender){
    mj::helper::playButtonFX();
    if (_delegate) {
        _delegate->onSelectProduct(this->getPackage());
    }

    std::string text_price = utils::findChild<ui::Text *>(this, "text.price")->getString();

    std::string text_title = utils::findChild<ui::Text*>(this, "text.title")->getString();
    if (text_title == LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.item.1mon"))
    {
        text_title = "1month";
    }
    if (text_title == LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.item.6mon"))
    {
        text_title = "6month";
    }
    if (text_title == LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.item.1year"))
    {
        text_title = "1year";
    }
    if (text_title == LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.item.lifetime"))
    {
        text_title = "lifetime";
    }

    MJDEFAULT->setStringForKey(key_property_price, _propertyPrice);
    MJDEFAULT->setStringForKey(key_choose_package, _package.product_id);
    MJDEFAULT->setStringForKey(key_package_length, text_title);

    try
    {
//        CleverTapManager::GetInstance()->pushEvent("purchase_screen_choose_package", {
//            {"choose_package", cocos2d::Value(_package.product_name)},
//         });
    }
    catch (const std::exception&)
    {

    }
}

ui::Widget::ccWidgetClickCallback MSTabSubscribe_Item::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSTabSubscribe_Item, onBuyClicked),
    };
    
    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    return nullptr;
}
