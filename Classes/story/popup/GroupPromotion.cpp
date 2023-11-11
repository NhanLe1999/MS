//
//  GroupPromotion.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/8/19.
//

#include "GroupPromotion.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "MSEventLogger.h"
#include "MJPay.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(GroupPromotion);

#define key_last_time_show_group_promotion "key.last.time.show.group.promo"
#define key_count_attemp_show_group_promotion "key.count.attemp.group.promo"
#define key_count_show_group_promotion "key.count.show.group.promo"

const std::vector<std::string> PROMO_CONTENT_VI = {
    "Tham gia ngay group cộng đồng phụ huynh đang\ncho con sử dụng chương trình Monkey Stories!",
    "Phong trào học tiếng anh cùng Monkey Stories đang\ndiễn ra rất sôi nổi trên group cộng đồng phụ huynh,\nba mẹ tham gia group ngay để theo dõi nhé!",
    "Tham gia group những ông bố, bà mẹ đang cho con\ntheo học Monkey Stories để giao lưu và chia sẻ\nkinh nghiệm ba mẹ nhé!"
};

const std::vector<std::string> PROMO_CONTENT_EN = {
    "Monkey Stories now has a Facebook group for parents! Join now by clicking this button below.",
    "Would you like to keep up with the latest news about learning Monkey Stories? Click to join Facebook group for parents!",
    "Join our Monkey Stories Facebook group for parents where you can ask questions, share ideas, get support, and connect with others!"
};

const std::vector<std::string> PROMO_TITLE_VI = {
    "GROUP ĐỒNG HÀNH CÙNG CON\nHỌC MONKEY",
    "PHONG TRÀO HỌC TIẾNG ANH\nCÙNG MONKEY STORIES",
    "TÌM BẠN ĐỒNG HÀNH\nHỌC CHƯƠNG TRÌNH MONKEY CÙNG CON"
};

bool GroupPromotion::shouldShow() {
    
    //disable this feature
    return false;
    //nếu chưa mua thì không hiện popup
    if (!mj::Pay::getInstance().isActive()) {
        return false;
    }
    
    //chỉ áp dụng đối với user VN
    if (!CONFIG_MANAGER->isCountryVN()) {
        return false;
    }
    
    //đã show đủ 3 lần
    if (MJDEFAULT->getIntegerForKey(key_count_show_group_promotion) > 3) {
        return false;
    }
    
    
    //nếu cách 3 ngày không hiện thì sẽ hiện
    auto last_time_show = MJDEFAULT->getIntegerForKey(key_last_time_show_group_promotion);
    auto current_time = mj::helper::getTime();
    if (current_time - last_time_show > 60 * 60 * 24 * 3) {
        return true;
    }
    
    //nếu sau 5 lần attemp thì sẽ hiện
    auto count_attemp_to_show = MJDEFAULT->getIntegerForKey(key_count_attemp_show_group_promotion);
    if (count_attemp_to_show % 5 == 1) {
        return true;
    }
    
    return false;
}

void GroupPromotion::attempToShow(cocos2d::Node * node, std::function<void()> callback) {
    auto count_attemp_to_show = MJDEFAULT->getIntegerForKey(key_count_attemp_show_group_promotion);
    MJDEFAULT->setIntegerForKey(key_count_attemp_show_group_promotion, count_attemp_to_show + 1);
    
    if (!shouldShow()) {
        if (callback)
            callback();
        return;
    }
    
    if (!node) {
        node = Director::getInstance()->getRunningScene();
    }
    
    auto popup = GroupPromotion::createView();
    popup->_callback = callback;
    node->addChild(popup, INT_MAX - 1);
    MJDEFAULT->setIntegerForKey(key_last_time_show_group_promotion, (int)mj::helper::getTime());
    MJDEFAULT->setIntegerForKey(key_count_show_group_promotion, MJDEFAULT->getIntegerForKey(key_count_show_group_promotion) + 1);
}

std::once_flag group_promotion_reader_flag;
GroupPromotion * GroupPromotion::createView() {
    std::call_once(group_promotion_reader_flag, [] {
        REGISTER_CSB_READER(GroupPromotion);
    });
    
    auto p = reinterpret_cast<GroupPromotion *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/GroupPromotion.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void GroupPromotion::didLoadFromCSB() {
    this->setTouchEnabled(true);
    auto content_id = cocos2d::random(0, 2);
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == 1) {
        utils::findChild<ui::Text *>(this, "title")->setString("");
        auto content = utils::findChild<ui::Text *>(this, "content");
        content->setString(PROMO_CONTENT_EN[content_id]);
        content->setPosition(content->getPosition() + Point(0, 40));
        utils::findChild<ui::Button *>(this, "button_join")->loadTextureNormal("group_promo_join_en.png");
    } else {
        utils::findChild<ui::Text *>(this, "title")->setString(PROMO_TITLE_VI[content_id]);
        utils::findChild<ui::Text *>(this, "content")->setString(PROMO_CONTENT_VI[content_id]);
        utils::findChild<ui::Button *>(this, "button_join")->loadTextureNormal("group_promo_join_vi.png");
    }
}

void GroupPromotion::onEnter() {
    ui::Layout::onEnter();
    
    auto background = utils::findChild(this, "bg");
    background->setOpacity(0);
    background->runAction(FadeIn::create(0.5));
    
    auto content = utils::findChild(this, "popup");
    content->setPosition(content->getPosition() + Point(0, 800));
    
    auto monkey = utils::findChild(this, "monkey");
    monkey->setVisible(false);
    monkey->setPosition(monkey->getPosition() + Point(0, -600));
    
    content->runAction(Sequence::create(EaseQuarticActionOut::create(MoveBy::create(1, Point(0, -800))), CallFunc::create([monkey] {
        monkey->setVisible(true);
        monkey->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5, Point(0, 600))));
    }), nullptr));
    
    ms::EventLogger::getInstance().logEvent("ms_show_facebook_group_promotion", {});
}

void GroupPromotion::onExit() {
    ui::Layout::onExit();
}

void GroupPromotion::onClose(cocos2d::Ref * sender) {
    ui::Layout::onEnter();
    
    auto content = utils::findChild(this, "popup");
    auto monkey = utils::findChild(this, "monkey");
    auto background = utils::findChild(this, "bg");
    
    background->runAction(FadeOut::create(0.25));
    monkey->runAction(Sequence::create(EaseQuarticActionIn::create(MoveBy::create(0.25, Point(0, -600))), CallFunc::create([monkey, content, this] {
        monkey->setVisible(false);
        content->runAction(Sequence::create(EaseQuarticActionIn::create(MoveBy::create(0.25, Point(0, 800))), CallFunc::create([this] {
            if (_callback) {
                _callback();
            }
            this->removeFromParent();
        }), nullptr));
    }), NULL));
}

void GroupPromotion::onJoin(cocos2d::Ref * sender) {
    ms::EventLogger::getInstance().logEvent("ms_click_facebook_group_promotion", {});
    ms::EventLogger::getInstance().setUserProperties("click_to_join_fb_group", "yes");
        if(CONFIG_MANAGER->isAppVNStories())
    {
        Application::getInstance()->openURL(GROUP_VM_URL);
    }
    else
    {
        Application::getInstance()->openURL(GROUP_MS_URL);
    }
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback GroupPromotion::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(GroupPromotion, onClose),
        CLICK_MAP(GroupPromotion, onJoin),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
