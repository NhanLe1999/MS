//
//  MJLicenseActiveSuccess.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 11/29/18.
//

#include "MJLicenseActiveSuccess.h"
#include "GameData.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(MJLicenseActiveSuccess);

std::once_flag mj_license_active_success_reader_flag;

MJLicenseActiveSuccess * MJLicenseActiveSuccess::createView(std::string message, std::function<void()> exit_callback) {
    std::call_once(mj_license_active_success_reader_flag, [] {
        REGISTER_CSB_READER(MJLicenseActiveSuccess);
    });
    
    auto view = reinterpret_cast<MJLicenseActiveSuccess *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/MJLicenseActiveSuccess.csb"));
    if (view) {
        view->didLoadFromCSB(message, exit_callback);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

void MJLicenseActiveSuccess::didLoadFromCSB(std::string message, std::function<void()> exit_callback) {
//    auto pack = DataSQL::getInstance()->getLicensePackages();
//    if (DataSQL::getInstance()->getLicensePackages().size() <= 1) {
//        utils::findChild(this, "btok")->setVisible(false);
//        auto btcancel = utils::findChild(this, "btcancel");
//        btcancel->setPositionX(btcancel->getParent()->getContentSize().width / 2);
//    }
    
    _exit_callback = exit_callback;
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild<ui::ImageView*>(this, "logo_vi")->setVisible(true);
        utils::findChild<ui::ImageView*>(this, "logo")->setVisible(false);
    }
    
    auto msg = utils::findChild<ui::Text *>(this, "msg");
    msg->setString(message);
    
    std::string rich_string = "<font color='#000000' size='20' face='fonts/Montserrat-Regular.ttf'>Tham gia ngay Cộng đồng hàng ngàn phụ huynh sử dụng<br/>Monkey Stories để được <font color='#000000' size='20' face='fonts/Montserrat-Bold.ttf'>hỗ trợ chuyên môn</font> và <font color='#000000' size='20' face='fonts/Montserrat-Bold.ttf'>chia sẻ kinh nghiệm</font>!</font>";
    
    if (CONFIG_MANAGER->isAppVNStories()) {
        rich_string = "<font color='#000000' size='20' face='fonts/Montserrat-Regular.ttf'>Tham gia ngay Cộng đồng hàng ngàn phụ huynh sử dụng<br/>VMonkey để được <font color='#000000' size='20' face='fonts/Montserrat-Bold.ttf'>hỗ trợ chuyên môn</font> và <font color='#000000' size='20' face='fonts/Montserrat-Bold.ttf'>chia sẻ kinh nghiệm</font>!</font>";
    }
    
    auto footer_content = utils::findChild<ui::Text *>(this, "footer_content");
    auto rich = ui::RichText::createWithXML(rich_string);
    rich->setContentSize(footer_content->getContentSize());
    rich->setScaleX(footer_content->getScaleX());
    rich->setPosition(footer_content->getPosition());
    rich->setAnchorPoint(footer_content->getAnchorPoint());
    rich->ignoreContentAdaptWithSize(false);
    rich->setHorizontalAlignment(ui::RichText::HorizontalAlignment::LEFT);
    rich->setWrapMode(ui::RichText::WRAP_PER_WORD);
    rich->setVisible(true);
    footer_content->setVisible(false);
    footer_content->getParent()->addChild(rich);
    
    if (!CONFIG_MANAGER->isCountryVN()) {
        utils::findChild(this, "footer")->setVisible(false);
    } else {
        auto bg = utils::findChild(this, "bg");
        bg->setPositionY(bg->getPositionY() + 500);
        bg->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5, Vec2(0, -500))));
        auto footer = utils::findChild(this, "footer");
        footer->setPositionY(-200);
        footer->runAction(Sequence::create(DelayTime::create(0.75), EaseQuarticActionOut::create(MoveBy::create(1, Vec2(0, 200))), NULL));
    }
    
//    auto button_name = GameData::getInstance()->isVN ? "install/group_footer_join_vi.png" : "install/group_footer_join_en.png";
    utils::findChild<ui::Button *>(this, "button_join")->loadTextureNormal("install/group_footer_join_vi.png");
    utils::findChild<ui::Button *>(this, "button_join")->loadTexturePressed("install/group_footer_join_vi.png");
    
    auto text_cancel = utils::findChild<ui::Text*>(this,"lbcancel");
    bool is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;
    text_cancel->setString(is_vn ? "Đóng" : "Close");

    const float k_screenRatioFactor = Director::getInstance()->getVisibleSize().width/Director::getInstance()->getVisibleSize().height;

    auto colorLayout = utils::findChild<ui::Layout*>(this, "color_layout");

    if(colorLayout && k_screenRatioFactor >= 18.f/9.f)
    {
        colorLayout->setIgnoreAnchorPointForPosition(false);
        colorLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        colorLayout->setScale(1.2f);//110%
        colorLayout->setPosition(colorLayout->getParent()->getContentSize()/2);
    }
}

void MJLicenseActiveSuccess::onEnter() {
    ui::Layout::onEnter();
}

void MJLicenseActiveSuccess::onClose(Ref * sender) {
    if (_exit_callback) {
        _exit_callback();
    }
    this->removeFromParent();
}

#include "MSEventLogger.h"
#include "MJDefault.h"
void MJLicenseActiveSuccess::onJoin(Ref * sender) {
    if(CONFIG_MANAGER->isAppVNStories())
    {
        Application::getInstance()->openURL(GROUP_VM_URL);
    }
    else
    {
        Application::getInstance()->openURL(GROUP_MS_URL);
    }
    
    MJDEFAULT->setBoolForKey("user.clicked.to.join.group", true);
    ms::EventLogger::getInstance().setUserProperties("click_to_join_fb_group", MJDEFAULT->getBoolForKey("user.clicked.to.join.group", false) ? "yes" : "no");
}

#include "MJListLicense.h"
void MJLicenseActiveSuccess::onManage(Ref * sender) {
    auto list_license = MJListLicense::createView({});
    Director::getInstance()->getRunningScene()->addChild(list_license, 10000000);
}

ui::Widget::ccWidgetClickCallback MJLicenseActiveSuccess::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJLicenseActiveSuccess, onClose),
        CLICK_MAP(MJLicenseActiveSuccess, onManage),
        CLICK_MAP(MJLicenseActiveSuccess, onJoin)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
