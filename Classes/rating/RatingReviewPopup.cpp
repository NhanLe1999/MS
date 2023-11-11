#include "RatingReviewPopup.h"
#include "StoryLanguageManager.h"
#include "manager/RatingManager.h"
#include "PostingReviewAPI.h"
#include "StoryAlert.h"
#include <regex>

USING_NS_CC;
INITIALIZE_READER(RatingReviewPopup);

std::once_flag rating_review_popup_flag;

#define SAD_LAYOUT_HEIGHT				220.0F
#define SAD_WRITING_REVIEW_BOX_HEIGHT	250.0f


RatingReviewPopup * RatingReviewPopup::createView(Type i_type)
{
	std::call_once(rating_review_popup_flag, [] {
		REGISTER_CSB_READER(RatingReviewPopup);
		});

	auto p = static_cast<RatingReviewPopup*>(CSLoader::createNodeWithVisibleSize("csb/rating/RatingReviewPopup.csb"));

	if (p && p->didLoadFromCSB(i_type))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool RatingReviewPopup::didLoadFromCSB(Type i_type)
{
	m_type = i_type;

	if (!PopupBase::didLoadFromCSB())
	{
		return false;
	}

	setUpUIForMS();

	if (auto cb = cocos2d::utils::findChild<ui::Button*>(this, "combo_box"))
	{
		if (m_type == Type::SAD)
		{
			cb->getParent()->setContentSize({ cb->getParent()->getContentSize().width, SAD_WRITING_REVIEW_BOX_HEIGHT });

			if (auto reviewTextbox = cocos2d::utils::findChild<ui::TextField*>(this, "review_textbox"))
			{
				reviewTextbox->setContentSize({ reviewTextbox->getContentSize().width, SAD_LAYOUT_HEIGHT });
			}

			cb->setPosition({ cb->getPosition().x,297.08f});
		}

		auto problems = RatingManager::GetInstance()->getProblems();

		if (!problems.empty())
		{
			cb->setVisible(false);

			m_comboBox = ComboBox::createComboBox(cb->getContentSize(), RatingManager::GetInstance()->getProblems(),CONFIG_MANAGER->isVietnameseStories()? "rating/button_bg.png":"mjstory/new_ui/login/button seleced.png", CONFIG_MANAGER->isVietnameseStories() ? "rating/listview_bg.png" : "rating/ms_listview_bg.png", "rating/triangle.png");
			m_comboBox->setPosition(cb->getPosition());
			cb->getParent()->addChild(m_comboBox, cb->getZOrder());
		}
	}

	if (auto facebookButton = cocos2d::utils::findChild<ui::Button*>(this, "facebook_button"))
	{
		facebookButton->setVisible(m_type == RatingReviewPopup::Type::CONFUSED);
	}

	if (auto reviewTextbox = cocos2d::utils::findChild<ui::TextField*>(this, "review_textbox"))
	{
		reviewTextbox->setPlaceHolderColor(Color4B(155, 155, 155, 255));
		reviewTextbox->setTextColor(Color4B(155, 155, 155, 255));
		reviewTextbox->setCursorChar('|');
		reviewTextbox->setCursorEnabled(true);
		reviewTextbox->addEventListener([=](Ref *pSender, cocos2d::ui::TextField::EventType i_type) {

			auto box = utils::findChild(this, "popup_bg");

			switch (i_type)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			case TextField::EventType::ATTACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
				break;
			case TextField::EventType::DETACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
				break;
#endif
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			{
				(reviewTextbox->getString().size() == 0) ? reviewTextbox->setTextColor(Color4B(155, 155, 155, 255)) : reviewTextbox->setTextColor(Color4B::BLACK);

				break;
			}
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			{
				(reviewTextbox->getString().size() == 0) ? reviewTextbox->setTextColor(Color4B(155, 155, 155, 255)) : reviewTextbox->setTextColor(Color4B::BLACK);

				break;
			}
			default:
				break;
			}

		});
	}

	m_onCancelCallBack = [=]() {

		RatingManager::GetInstance()->reset();
	};

	//showingAnimation(utils::findChild(this, "popup_bg"));

	return true;
}

void RatingReviewPopup::onEnter()
{
	Layout::onEnter();
}

void RatingReviewPopup::onExit()
{
	Layout::onExit();
}

void RatingReviewPopup::onPost(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<ui::Button*>(sender);

	button->setEnabled(false);

	auto reviewTextbox = cocos2d::utils::findChild<ui::TextField*>(this, "review_textbox");
	
	if (reviewTextbox->getString().empty())
	{
		StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("rating_empty_texbox"), "OK", "");

		button->setEnabled(true);

		return;
	}

	auto lst = RatingManager::GetInstance()->getProblems();

	const int	k_id = lst.at(m_comboBox->getCurrentItemID()).id;
	const auto	k_name = lst.at(m_comboBox->getCurrentItemID()).name;

	PostingReviewAPI::call(k_id, reviewTextbox->getString(), (int)m_type + 1, [=](bool isSuccess, std::string msg, const rapidjson::Value &data) {
	
		if (isSuccess)
		{
			CCLOG("rating|post review success!");

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				RatingManager::GetInstance()->pushCleverTapTrackingData(m_type==Type::CONFUSED?"confused":"sad", true, k_id, k_name,reviewTextbox->getString());

				RatingManager::GetInstance()->reset();
				this->removeFromParentAndCleanup(true);
			});
		}
		else
		{
			showErrorAlert(msg);//onCocos thread.

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]() {
				
					button->setEnabled(true);
				
				})));

			});
			CCLOG("rating|post failed : %s !", msg.c_str());
		}
	
	});

}

void RatingReviewPopup::onFacebookClicked(cocos2d::Ref * sender)
{
	Application::getInstance()->openURL(CONFIG_MANAGER->isAppVNStories()? GROUP_VM_URL: GROUP_MS_URL);
}

void RatingReviewPopup::changeDisplayLanguage()
{

	if (m_type == RatingReviewPopup::Type::SAD)
	{
		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");
			title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_sad_title"));
		}

		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "description"))
		{
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"Roboto-Regular.ttf");

			std::string des = LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_sad_description");
			des = std::regex_replace(des, std::regex("APPNAME"), CONFIG_MANAGER->isVietnameseStories() ?"VMonkey":"Monkey Stories");
			title->setString(des);
		}
	}
	else
	{
		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");
			title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_confused_title"));
		}

		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "description"))
		{
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"Roboto-Regular.ttf");
			std::string des = LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_confused_description");
			des = std::regex_replace(des, std::regex("APPNAME"), CONFIG_MANAGER->isAppVNStories() ? "VMonkey" : "Monkey Stories");
			title->setString(des);
			//title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_confused_description"));
		}
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "post_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_post_text"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "fb_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Roboto-Regular.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_review_fb_text"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		auto icon = Sprite::create(m_type == RatingReviewPopup::Type::SAD ? "rating/sad.png" : "rating/confuse.png");

		icon->setPosition(title->getPosition() + Vec2(title->getContentSize().width/2 + icon->getContentSize().width*0.8f, 0));

		title->getParent()->addChild(icon, title->getZOrder());
	}

	if (auto reviewTextbox = cocos2d::utils::findChild<ui::TextField*>(this, "review_textbox"))
	{
        reviewTextbox->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Roboto-Regular.ttf");

		reviewTextbox->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("feedback.textfield.placeholder"));
	}
    
}

cocos2d::ui::Widget::ccWidgetClickCallback RatingReviewPopup::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(RatingReviewPopup, onCancel),
		CLICK_MAP(RatingReviewPopup, onPost),
		CLICK_MAP(RatingReviewPopup, onFacebookClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void RatingReviewPopup::setUpUIForMS()
{
	if (CONFIG_MANAGER->isVietnameseStories())
	{
		return;
	}

	if (auto titleBackground = cocos2d::utils::findChild<ui::ImageView*>(this, "title_bg"))
	{
		titleBackground->loadTexture("rating/ms_title_bg.png");
	}

	if (auto postButton = cocos2d::utils::findChild<ui::Button*>(this, "post_button"))
	{
		postButton->loadTextures("mjstory/new_ui/login/button seleced.png","","");
		postButton->setTitleText("");
	}

	if (auto facebookButton = cocos2d::utils::findChild<ui::Button*>(this, "facebook_button"))
	{
		facebookButton->loadTextures("mjstory/new_ui/login/button seleced.png", "", "");
	}
}
