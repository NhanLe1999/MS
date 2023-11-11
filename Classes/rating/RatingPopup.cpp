#include "RatingPopup.h"
#include "RatingReviewPopup.h"
#include "StoryLanguageManager.h"
#include "PostingReviewAPI.h"
#include "MJPlatformConfig.h"
#include "CleverTapManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
INITIALIZE_READER(RatingPopup);

std::once_flag rating_popup_flag;

#define FADING_DURATION			0.05f
#define FADING_DURATION_OFFSET	0.05f

RatingPopup * RatingPopup::createView() 
{
	std::call_once(rating_popup_flag, [] {
		REGISTER_CSB_READER(RatingPopup);
		});

	auto p = static_cast<RatingPopup*>(CSLoader::createNodeWithVisibleSize("csb/rating/RatingPopup.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool RatingPopup::didLoadFromCSB()
{
	if (!PopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_onCancelCallBack = [=]() {

        pushCleverTapEvent("exit_button");

		RatingManager::GetInstance()->reset();
	};

	if (auto popupBg = cocos2d::utils::findChild(this, "popup_bg"))
	{
		auto cancelButton = cocos2d::utils::findChild(this, "cancel_button");

		cocos2d::Vector<Node*> nodes ;

		nodes.pushBack(popupBg);
		nodes.pushBack(cancelButton);

		for (auto node : popupBg->getChildren())
		{
			nodes.pushBack(node);

			for (auto n : node->getChildren())
			{
				nodes.pushBack(n);
			}
		}

		for (auto node : cancelButton->getChildren())
		{
			nodes.pushBack(node);

			for (auto n : node->getChildren())
			{
				nodes.pushBack(n);
			}
		}

		for (int i = 0; i < nodes.size(); i++)
		{
			auto node = nodes.at(i);
			node->setOpacity(0);
			node->runAction(FadeIn::create(FADING_DURATION + i* FADING_DURATION_OFFSET));
		}
	}
	//showingAnimation(utils::findChild(this, "popup_bg"));

	return true;
}

void RatingPopup::onEnter() 
{
	Layout::onEnter();
}

void RatingPopup::onExit() 
{
	Layout::onExit();
}

void RatingPopup::onHappyClicked(cocos2d::Ref * sender)
{
    pushCleverTapEvent("happy_button");

	//avoid double click
	auto button = dynamic_cast<ui::Button*>(sender);

	button->setEnabled(false);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sRateMe");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    doRate();
#endif

    RatingManager::GetInstance()->reset();

	PostingReviewAPI::call(-1, "", (int)RatingReviewPopup::Type::HAPPY + 1 , [=](bool isSuccess, std::string msg, const rapidjson::Value &data) {

		if (isSuccess)
		{
			CCLOG("rating|post review success!");
		}
		else
		{
			CCLOG("rating|post failed : %s !", msg.c_str());
		}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			RatingManager::GetInstance()->pushCleverTapTrackingData("happy", true, -1, "none", "none");

			this->removeFromParentAndCleanup(true);
		});

	});

}

void RatingPopup::onConfusedClicked(cocos2d::Ref * sender)
{
	pushCleverTapEvent("confused_button");
    
    auto button = dynamic_cast<ui::Button*>(sender);

	button->setEnabled(false);

	addRatingReviewPopup(RatingReviewPopup::Type::CONFUSED);
}

void RatingPopup::onSadClicked(cocos2d::Ref * sender)
{
    pushCleverTapEvent("sad_button");
    
	auto button = dynamic_cast<ui::Button*>(sender);

	button->setEnabled(false);

	addRatingReviewPopup(RatingReviewPopup::Type::SAD);
}

void RatingPopup::addRatingReviewPopup(RatingReviewPopup::Type i_type)
{
	auto ratingReviewPopup = RatingReviewPopup::createView(i_type);

	ratingReviewPopup->setPositionX(ratingReviewPopup->getPositionX() + ratingReviewPopup->getContentSize().width);

	Director::getInstance()->getRunningScene()->addChild(ratingReviewPopup, INT_MAX - 1);

	ratingReviewPopup->runAction(Sequence::create(MoveBy::create(0.5f, { -ratingReviewPopup->getContentSize().width ,0 }), CallFunc::create([=]() {

		this->removeFromParentAndCleanup(true);

	}), nullptr));

}

void RatingPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		std::string text = LANGUAGE_MANAGER->getDisplayTextByKey("rating_tile");

        std::string titleConfig = "<font color='#000000' size='30' face='fonts/Roboto-Regular.ttf'>" + text +"<br/> <font color='#000000' size='32' face='fonts/Roboto-Bold.ttf'>" + (CONFIG_MANAGER->isVietnameseStories() ?"VMonkey":"Monkey Stories") + " </font>" + " ? </font>";
        
        if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
        {
            titleConfig = "<font color='#000000' size='30' face='fonts/leelawdb.ttf'>" + text +"<br/> <font color='#000000' size='32' face='fonts/leelawdb.ttf'>" + (CONFIG_MANAGER->isVietnameseStories() ?"VMonkey":"Monkey Stories") + " </font>" + " ? </font>";
        }
        
		auto richText = ui::RichText::createWithXML(titleConfig);
		richText->ignoreContentAdaptWithSize(false);
		richText->setContentSize(title->getContentSize());
		richText->setPosition(title->getPosition());
		richText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
		title->getParent()->addChild(richText, title->getLocalZOrder() + 1);

		title->setVisible(false);
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "sad_title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_sad_tile"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "confused_title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_confused_tile"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "happy_title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_happy_tile"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "cancel_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Regular.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_cancel_text"));
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback RatingPopup::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(RatingPopup, onCancel),
		CLICK_MAP(RatingPopup, onHappyClicked),
		CLICK_MAP(RatingPopup, onConfusedClicked),
		CLICK_MAP(RatingPopup, onSadClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}

void RatingPopup::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value("home_screen")},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("rating_popup")},
    });
}
