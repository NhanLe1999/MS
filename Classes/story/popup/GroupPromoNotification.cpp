//
//  GroupPromoNotification.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 4/23/19.
//

#include "GroupPromoNotification.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
#include "StoryParentNotification_Landscape.h"

USING_NS_CC;

INITIALIZE_READER(GroupPromoNotification);

std::once_flag group_promo_notification_reader_flag;

GroupPromoNotification * GroupPromoNotification::createView() {
    std::call_once(group_promo_notification_reader_flag, [] {
        REGISTER_CSB_READER(GroupPromoNotification);
    });
    
    auto node = reinterpret_cast<GroupPromoNotification *>(CSLoader::createNode("csb/GroupPromoNotification.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void GroupPromoNotification::didLoadFromCSB() 
{
//    if (CONFIG_MANAGER->isAppVNStories()) {
//        utils::findChild<ui::ImageView *>(this, "background")->loadTexture("group_promo_notification_vmonkey.png");
//    }
    
    // MS/VM đều đổi background sang nền trắng
    if (auto background = utils::findChild<ui::ImageView *>(this, "background"))
    {
        background->loadTexture("mjstory/parent/parent_notification_item_background_active.png");
        background->setScale9Enabled(true);
        background->setContentSize(Size(500, 265));
    }
    
    const std::vector<std::string> formatted_text = {
        "<font color='#000000' size='18' face='fonts/Montserrat-Regular.ttf'>• <font color='#000000' size='18' face='fonts/Montserrat-SemiBold.ttf'>HỖ TRỢ CHUYÊN MÔN</font> từ đội ngũ Teacher Monkey</font>",
        "<font color='#000000' size='18' face='fonts/Montserrat-Regular.ttf'>• <font color='#000000' size='18' face='fonts/Montserrat-SemiBold.ttf'>KINH NGHIỆM HAY</font> từ hàng ngàn phụ huynh</font>",
        "<font color='#000000' size='18' face='fonts/Montserrat-Regular.ttf'>• Các hoạt động kết nối <font color='#000000' size='18' face='fonts/Montserrat-SemiBold.ttf'>ĐỘC QUYỀN</font> từ Monkey</font>"
    };
    for (auto i = 1; i < 4; ++i) {
        auto text = utils::findChild<ui::Text *>(this, StringUtils::format("text_%02d", i));
        
        auto rich = ui::RichText::createWithXML(formatted_text[i - 1]);
        rich->setContentSize(text->getContentSize());
        rich->setScaleX(text->getScaleX());
        rich->setPosition(text->getPosition());
        rich->setAnchorPoint(text->getAnchorPoint());
        rich->ignoreContentAdaptWithSize(false);
        rich->setHorizontalAlignment(ui::RichText::HorizontalAlignment::LEFT);
        rich->setWrapMode(ui::RichText::WRAP_PER_WORD);
        rich->setVisible(text->isVisible());
        
        text->getParent()->addChild(rich);
        text->setVisible(false);
    }
    
    auto button_name = LANGUAGE_MANAGER->getDisplayLangId() != 1 ? "install/group_footer_join_vi.png" : "install/group_footer_join_en.png";
    utils::findChild<ui::Button *>(this, "button_join")->loadTextureNormal(button_name);
    utils::findChild<ui::Button *>(this, "button_join")->loadTexturePressed(button_name);
}

void GroupPromoNotification::onEnter() {
    ui::Layout::onEnter();
}

#include "MSEventLogger.h"
#include "MJDefault.h"
void GroupPromoNotification::onJoin(Ref * sender) 
{
	if (m_delegateParent != nullptr)
	{
		if (auto ob = dynamic_cast<StoryParentNotification_Landscape*>(m_delegateParent))
		{
			StoryNotificationInfo info;

			if (auto text = utils::findChild<ui::Text *>(this, "Text_1"))
			{
				info.title = text->getString();
			}

			ob->onNotificationItemClicked(info);
		}
	}

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

void GroupPromoNotification::onClickBackground(cocos2d::Ref *sender){
    onJoin(sender);
}

void GroupPromoNotification::onExit() {
    ui::Layout::onExit();
}

ui::Widget::ccWidgetClickCallback GroupPromoNotification::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(GroupPromoNotification, onJoin),
        CLICK_MAP(GroupPromoNotification, onClickBackground)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

