#include "APPhoneCodePopup.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "APPopup.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "MJPlatformConfig.h"
#include "APLoginAPI.h"
#include "APPasswordAPI.h"
#include "APSendOTPAPI.h"
#include "HelperManager.h"
#include "Math_Func.h"

USING_NS_CC;
INITIALIZE_READER(APPhoneCodePopup);

std::once_flag v2_ap_phone_code_popup_reader_flag;

APPhoneCodePopup * APPhoneCodePopup::createView()
{
	std::call_once(v2_ap_phone_code_popup_reader_flag, []
	{
		REGISTER_CSB_READER(APPhoneCodePopup);
	});

	auto p = static_cast<APPhoneCodePopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new_v2/APPhoneCodePopup.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

APPhoneCodePopup::~APPhoneCodePopup()
{
    for (auto item : list_item)
    {
        if(item)
        {
            item->release();
        }
    }
}

bool APPhoneCodePopup::didLoadFromCSB()
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	if (auto popupBg = utils::findChild(this, "popup_bg"))
	{
		popupBg->setOpacity(0);

		popupBg->runAction(FadeIn::create(0.5f));
	}

	listview = cocos2d::utils::findChild<ui::ListView*>(this, "listview");
    
    listview->removeAllItems();
    
    auto item = cocos2d::utils::findChild<ui::Button*>(this, "item");
    
    std::vector<PhoneCodeInfo> listPhoneCodeInfo = HelperManager::GetInstance()->getPhoneCodeList();
    
    if(listPhoneCodeInfo.empty())//cannot get data from server.
    {
        listPhoneCodeInfo.push_back(PhoneCodeInfo{"VN","VietNam","+84",false});
        listPhoneCodeInfo.push_back(PhoneCodeInfo{"EN","US","+1",false});
        listPhoneCodeInfo.push_back(PhoneCodeInfo{"TH","Thailand","+66",false});
    }
	
    list_item.clear();
    
    for(auto info:listPhoneCodeInfo)
    {
        auto newItem = item->clone();
        
        newItem->setName(info.id);

		if (auto flag = cocos2d::utils::findChild<ui::ImageView*>(newItem, "flag"))
		{
			const std::string k_path = StringUtils::format("flags/%s.png", info.id.c_str());
			
			if (auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(k_path))
			{
				flag->loadTexture(k_path, ui::Widget::TextureResType::PLIST);
			}
			else
			{
				CCLOG("<APPhoneCodePopup> cannot load sprite frame: %s", k_path.c_str());
			}
		}

		if (auto txtCountry = cocos2d::utils::findChild<ui::Text*>(newItem, "txt_country"))
		{
			txtCountry->setString(info.name);
		}

		if (auto txtPhoneCode = cocos2d::utils::findChild<ui::Text*>(newItem, "txt_phone_code"))
		{
			txtPhoneCode->setString(info.code);
		}
        
		newItem->setName(info.name);

        listview->pushBackCustomItem(newItem);
		newItem->retain();
        newItem->addClickEventListener([=](Ref*)
        {
            if(m_delegate)
            {
                m_delegate->onPhoneCodeItemSelected(info);
				
				if (auto popupBg = utils::findChild(this, "popup_bg"))
				{
					popupBg->runAction(Sequence::createWithTwoActions(FadeOut::create(0.5f), CallFunc::create([=]() {
					
						this->removeFromParent();

					})));
				}
            }
        });

		list_item.push_back(newItem);
    }

    item->removeFromParent();
    
	listview->addEventListener([=](Ref* pSender, ui::ScrollView::EventType type)
	{
		ListView* listView = dynamic_cast<ListView*>(pSender);

		if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
		{
			return;
		}

		auto top = listView->getTopmostItemInCurrentView();
		auto bottom = listView->getBottommostItemInCurrentView();

		for (size_t i = 0; i < listView->getChildrenCount(); i++)
		{
			listView->getChildren().at(i)->setVisible(false);
		}

		for (size_t i = listView->getIndex(top); i <= listView->getIndex(bottom); i++)
		{
			if (auto node = listView->getItem(i))
			{
				node->setVisible(true);
			}
		}
	});

	this->schedule([=](float dt) {
	
		if (!listview)
		{
			return;
		}
	
		auto top = listview->getTopmostItemInCurrentView();
		auto bottom = listview->getBottommostItemInCurrentView();

		for (size_t i = 0; i < listview->getChildrenCount(); i++)
		{
			listview->getChildren().at(i)->setVisible(false);
		}

		for (size_t i = listview->getIndex(top); i <= listview->getIndex(bottom); i++)
		{
			if (auto node = listview->getItem(i))
			{
				node->setVisible(true);
			}
		}
	
	}, "sadasdasd");


	textfield_search = cocos2d::utils::findChild<ui::TextField*>(this, "input_textfield_search");
	textfield_search->addEventListener(CC_CALLBACK_2(APPhoneCodePopup::onTextfieldChanged, this));
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        if (auto background = utils::findChild<ui::Layout *>(this, "background"))
        {
            background->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
            
            auto bg1 = Scale9Sprite::create("account/square_w_128.png");
            bg1->setPreferredSize(Size(888, 720));
            bg1->setPosition(Point(background->getContentSize().width/2, background->getContentSize().height/2 - 6));
            bg1->setColor(Color3B(42, 176, 222));
            background->addChild(bg1);
            
            auto bg2 = Scale9Sprite::create("account/square_w_128.png");
            bg2->setPreferredSize(bg1->getPreferredSize() - Size(8, 8));
            bg2->setPosition(bg1->getPosition());
            background->addChild(bg2);
        }
        
        if (auto button_back = cocos2d::utils::findChild<ui::Button *>(this, "btn_back"))
        {
            button_back->loadTextureNormal("account/new_v3_vn/button_back_left.png");
            button_back->loadTexturePressed("account/new_v3_vn/button_back_left.png");
            button_back->resetNormalRender();
            button_back->resetPressedRender();
            button_back->resetDisabledRender();
            button_back->getRendererNormal()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererClicked()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererDisabled()->setPosition(button_back->getContentSize()/2);
        }
    }
    
	return true;
}

void APPhoneCodePopup::onExit()
{
    APPopupBase::onExit();
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "input_textfield_search"));
}

void APPhoneCodePopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.select.country.code"));
	}

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "input_textfield_search"))
	{
        inputTextfield->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");

		inputTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.search"));
	}
}

void APPhoneCodePopup::onItemClicked(Ref * sender)
{
    
}

cocos2d::ui::Widget::ccWidgetClickCallback APPhoneCodePopup::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APPhoneCodePopup, onCancel),//locate at parent (APPopupBase)
		//CLICK_MAP(APPhoneCodePopup, onItemClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }
	return nullptr;
}
void APPhoneCodePopup::onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
{
	//auto box = utils::findChild(this, "bg_ngang");
	switch (type)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//|| (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	case TextField::EventType::ATTACH_WITH_IME:
		//box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
		break;
	case TextField::EventType::DETACH_WITH_IME:
		//box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
		break;
#endif
	case TextField::EventType::INSERT_TEXT:
	{
		CCLOG("Insert");
        (textfield_search->getString().size() == 0)?textfield_search->setTextColor(Color4B::WHITE): textfield_search->setTextColor(Color4B::BLACK);

		this->updateListView(textfield_search->getString());
        
        break;
	}
	case TextField::EventType::DELETE_BACKWARD:
	{
		CCLOG("Delete");
        (textfield_search->getString().size() == 0)?textfield_search->setTextColor(Color4B::WHITE): textfield_search->setTextColor(Color4B::BLACK);

		this->updateListView(textfield_search->getString());
        
        break;
	}
	default:
		break;
	}

}
void APPhoneCodePopup::updateListView(std::string a)
{
	CCLOG("Update listview");
	
    listview->removeAllItems();
    
	for (auto item : list_item)
    {
		std::string name = math::func::lowerString(item->getName());
		std::string key = math::func::lowerString(a);
		if (name.find(key) != std::string::npos) {
			listview->pushBackCustomItem(item);
		}
	}
    
    if (listview->getItems().empty()){
        listview->pushBackCustomItem(list_item[0]);
    }
	
}

