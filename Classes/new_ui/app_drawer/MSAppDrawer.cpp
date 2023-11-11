//
//  MSAppDrawer.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#include "MSAppDrawer.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "AudioEngine.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"
#include "manager/CleverTapManager.h"
#include "MJDefault.h"
#include "HelperManager.h"
USING_NS_CC;
INITIALIZE_READER(MSAppDrawer);

#define MJDEFAULT MJDefault::getInstance()
#define app_drawer_sound_fx "sounds/ui/button_fx/6.mp3"

std::unordered_map<std::string, Color3B> APP_COLOR_CODES =
{
    {"ms", Color3B(43, 183, 231)},
    {"vm", Color3B(156, 204, 68)},
	{"mj", Color3B(156, 204, 68)},
    {"mm", Color3B(253, 204, 1)}
};

std::once_flag ms_app_drawer_reader;

MSAppDrawer * MSAppDrawer::createDrawer() 
{
	std::call_once(ms_app_drawer_reader, [] 
	{
        REGISTER_CSB_READER(MSAppDrawer);
    });
    
    auto drawer = reinterpret_cast<MSAppDrawer *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/apps/AppDrawer.csb"));
    
	if (drawer) 
	{
        drawer->didLoadFromCSB();
        return drawer;
    }
    
    CC_SAFE_DELETE(drawer);
    
	return nullptr;
}

MSAppDrawer::~MSAppDrawer()
{

}

void MSAppDrawer::didLoadFromCSB()
{
    auto controller = utils::findChild(this, "panel_control");
    controller->setPosition(controller->getPosition() + Point(MJ_PLATFORM_CONFIG.getNotchHeight() - 60, 0));
    
    auto content = utils::findChild(this, "content");
    content->setContentSize(content->getContentSize() + Size(60 - MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
    content->setPosition(content->getPosition() + Point(MJ_PLATFORM_CONFIG.getNotchHeight() - 60, 0));
    
    auto appLayers = CSLoader::createNode("csb/new_ui/apps/AppsCrossPromotion.csb");
	appLayers->setAnchorPoint(Point::ANCHOR_MIDDLE);
	appLayers->setPosition(content->getContentSize() / 2);
    content->addChild(appLayers);
    
	//remove left unused button
	if (auto listView = utils::findChild<ui::ListView*>(this, "button_list"))
	{
		listView->removeChildByName(CONFIG_MANAGER->isVietnameseStories() ? "vm_item": "ms_item");
	}

	//remove unused layer
	if (CONFIG_MANAGER->isVietnameseStories()) 
	{
		if (auto node = utils::findChild<ImageView*>(appLayers, "vm"))
		{
			node->removeFromParent();
		}
	}
	else // ms app -> remove mj,mm here if we use this code on that apps.
	{
		if (auto node = utils::findChild<ImageView*>(appLayers, "ms"))
		{
			node->removeFromParent();
		}
	}

	m_appContentLayers =
	{
		utils::findChild<ImageView*>(this, "mm"),
		utils::findChild<ImageView*>(this, "vm"),
		utils::findChild<ImageView*>(this, "ms"),
		utils::findChild<ImageView*>(this, "mj"),
	};

	// localization (th,en,vi)
	for (auto layer : m_appContentLayers)
	{
		if (!layer)
		{
			continue;
		}

		if (auto node = utils::findChild(layer, "vi_description"))
		{
			node->setVisible(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE);
		}

		if (auto node = utils::findChild(layer, "en_description"))
		{
			node->setVisible(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE);
		}

		if (auto node = utils::findChild(layer, "th_description"))
		{
			node->setVisible(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE);
		}
        
        if (auto node = utils::findChild(layer, "id_description"))
        {
            node->setVisible(CONFIG_MANAGER->isVietnameseStories() ? false : LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE);
        }
	}

	//back key event
    auto backkeyListener = EventListenerKeyboard::create();

	backkeyListener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event)
	{
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) 
		{
            this->onClose(utils::findChild(this, "exit_button"));
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkeyListener, this);

	updateHightlightBackground(utils::findChild(this, "mm_item"));
	Node* exit_button = utils::findChild(this, "exit_button");
	exit_button->setPosition(exit_button->getPosition()+Vec2(-40,-10));
}

void MSAppDrawer::onEnter()
{
    ui::Layout::onEnter();
    
    utils::findChild(this, "button_play")->setLocalZOrder(100);
}

void MSAppDrawer::onClose(Ref * sender) 
{
	//HelperManager::GetInstance()->addSidebarBlack();
    mj::helper::playButtonFXClose();
	if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
	{
		ms::HomeTab preTabUnclock = (ms::HomeTab)MJDEFAULT->getIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::LESSONS);
		_delegate ? _delegate->onSelectedTab(preTabUnclock, ms::MovingDirection::NONE) : (void) nullptr;
	}
	this->removeFromParent();
}

void MSAppDrawer::moveBackground(ui::Button * button, Color3B color)
{
    auto background = utils::findChild(this, "selection_background");
    background->stopAllActions();
    background->runAction(MoveTo::create(0.25f, button->getPosition() + Point(1.5, 0)));
    background->runAction(TintTo::create(0.25f, color));
}

void MSAppDrawer::onItemClick(cocos2d::Ref *sender)
{
	auto node = dynamic_cast<cocos2d::Node*>(sender);

	if (!node)
	{
		return;
	}

	experimental::AudioEngine::play2d(app_drawer_sound_fx);

	updateHightlightBackground(node->getParent());

	std::string senderName = node->getParent()->getName();

	if (senderName.size() >= 2)
	{
		senderName = senderName.substr(0, 2);
	}

	for (auto child : m_appContentLayers)
	{
		if (!child) continue;
	
		child->runAction(senderName == child->getName() ?(Action*)FadeIn::create(0.5) :(Action*)FadeOut::create(0.5));
	}
}

void MSAppDrawer::onPlay(cocos2d::Ref *sender) {

	mj::helper::playButtonFX();

	cocos2d::Node *node = nullptr;

	for (auto layer : m_appContentLayers)
	{
		if (!layer) continue;
		
		if (layer->getOpacity() >= 200)
		{
			node = layer;

			break;
		}
	}

	if (node)
	{
		//moveBackground(reinterpret_cast<ui::Button *>(sender), APP_COLOR_CODES.at("learntoread"));
		isDownloadClicked = true;

		productName = node->getName();

		if (productName == "mm")
		{
			productName = "Monkey Math";
			MJ_PLATFORM_CONFIG.showMathStorePage();
		}
		else if (productName == "ms")
		{
			productName = "Monkey Stories";
			MJ_PLATFORM_CONFIG.showStoriesStorePage();
		}
		else if (productName == "mj")
		{
			productName = "Monkey Junior";
			MJ_PLATFORM_CONFIG.showLearnToReadStorePage();
		}
		else //vm_item
		{
			productName = "VMonkey";
			MJ_PLATFORM_CONFIG.showVMonkeyPage();
		}

		pushCleverTapTrackingData();
	}
}

void MSAppDrawer::updateHightlightBackground(cocos2d::Node * i_node)
{
	if (!i_node) return;

	std::vector<cocos2d::Node*> items =
	{
		utils::findChild<ImageView*>(this, "vm_item"),
		utils::findChild<ImageView*>(this, "ms_item"),
		utils::findChild<ImageView*>(this, "mj_item"),
		utils::findChild<ImageView*>(this, "mm_item"),
	};

	for (auto it : items)
	{
		if (!it) continue;

		if (auto selectBg = it->getChildByName("selection_background"))
		{
			selectBg->setVisible(false);
		}

		if (auto selectBg = it->getChildByName("button"))
		{
			selectBg->setLocalZOrder(100);
		}
	}

	std::string prefixName = i_node->getName();

	prefixName = (prefixName.size() >= 2)? prefixName.substr(0, 2) : "vm";

	if (auto selectBg = utils::findChild<ui::Widget*>(i_node, "selection_background"))
	{
		selectBg->setColor(Color3B(144, 238, 144));

		selectBg->setVisible(true);

		selectBg->runAction(TintTo::create(0.25f, APP_COLOR_CODES.at(prefixName)));
	}
}

ui::Widget::ccWidgetClickCallback MSAppDrawer::onLocateClickCallback(const std::string & callback_name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(MSAppDrawer, onClose),
		CLICK_MAP(MSAppDrawer, onPlay),
		CLICK_MAP(MSAppDrawer, onItemClick),
	};

	if (function_map.count(callback_name) > 0)
	{
		return function_map.at(callback_name);
	}

	return nullptr;
}

void MSAppDrawer::pushCleverTapTrackingData()
{
	try
	{
		CleverTapManager::GetInstance()->pushEvent("click_more_app_button",
			{
				{"view_screen",cocos2d::Value(true)},
				{"download",cocos2d::Value(isDownloadClicked)},
				{"product",cocos2d::Value(productName)},
			});
	}
	catch (const std::exception&)
	{

	}
}

