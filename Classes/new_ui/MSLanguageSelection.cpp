#include "MSLanguageSelection.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "MSIntro.h"
#include "CleverTapManager.h"
#include "CheatMenu.h"

#include "MMFirebaseDatabase.h"

USING_NS_CC;
INITIALIZE_READER(MSLanguageSelection);

#define MS_LANGUAGE_SELECTION_NAME "MSLanguageSelection"
std::once_flag ms_language_selection_reader;

Scene*  MSLanguageSelection::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = MSLanguageSelection::createView();
        //view->setIgnoreAnchorPointForPosition(false);
        //view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        //view->setPosition(Director::getInstance()->getVisibleOrigin()+Director::getInstance()->getVisibleSize()/2);
        view->setName(MS_LANGUAGE_SELECTION_NAME);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }
    
    return scene;
}

MSLanguageSelection * MSLanguageSelection::createView() 
{
    std::call_once(ms_language_selection_reader, []
	{
        REGISTER_CSB_READER(MSLanguageSelection);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/LanguageSelection.csb");
    
	auto view = reinterpret_cast<MSLanguageSelection *>(CSLoader::createNode(csb_name));
    
	if (view) 
	{
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return nullptr;
}

void MSLanguageSelection::didLoadFromCSB()
{
    _timeStartScreen = int(utils::getTimeInMilliseconds() / 1000.0);
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        mm::RealtimeDatabase::getInstance().loadKeyNewLibApp(USER_ID_CONFIG);
    });
    
    std::vector<cocos2d::Node*> nodes =
    {
        utils::findChild(this, "background"),
        utils::findChild(this, "Panel_1"),
    };
    
    for(auto node : nodes)
    {
        if(!node)
        {
            continue;
        }
        
        node->setPosition(Director::getInstance()->getVisibleOrigin()+Director::getInstance()->getVisibleSize()/2);
    }
    
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        utils::findChild<ImageView*>(this, "background")->loadTexture("mjstory/new_ui_vn/bg.jpg");
        auto screen_size = utils::findChild<Layout*>(this, "Panel_1")->getContentSize();
        utils::findChild<Button*>(this, "vi_button")->setPosition(Vec2(screen_size.width/2-200,screen_size.height/2));
        utils::findChild<Button*>(this, "en_button")->setPosition(Vec2(screen_size.width/2+200,screen_size.height/2));
        
        if (auto thai_button = utils::findChild<Button*>(this, "thai_button"))
        {
            thai_button->setVisible(false);
        }
        
        if (auto indo_button = utils::findChild<Button*>(this, "indo_button"))
        {
            indo_button->setVisible(false);
        }
    }

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    CheatMenu::show(this);
#endif
    
   APPopupBase::sAnimationMove(this, APPopupBase::AnimationDirection::LEFT);
    
   APPopupBase::sAnimationMoveForNodes({
        utils::findChild<Button*>(this, "vi_button"),
        utils::findChild<Button*>(this, "en_button"),
        utils::findChild<Button*>(this, "thai_button")
   },APPopupBase::AnimationDirection::LEFT,0.8f,0.39f);
    
}

void MSLanguageSelection::onLanguageSelected(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if(!button)
    {
        return;
    }
    
    button->setEnabled(false);
    
    MJDEFAULT->setBoolForKey("is_language_selection_show",true);
    MJDEFAULT->setBoolForKey("is_the_first_selection_language",true);
    int timeEnd = int(utils::getTimeInMilliseconds() / 1000.0)- _timeStartScreen;
    std::string language = "";
    auto tag  = button->getTag();
    switch (tag) {
        case LANGUAGE_VN_CODE:
            language = "Vietnam";
            break;
        case LANGUAGE_EN_CODE:
            language = "English";
            break;
        case LANGUAGE_ID_CODE:
            language = "Indo";
            break;
        case LANGUAGE_THAI_CODE:
            language = "Thailand";
            break;
            
        default:
            break;
    }
    
    std::vector<std::pair<std::string, cocos2d::Value>> properties = {
        {"language", cocos2d::Value(language)},
        {"time_on_screen", cocos2d::Value(timeEnd)},
        {"select_or_not", cocos2d::Value("true")},
    };
    CleverTapManager::GetInstance()->pushEvent("ms_ob_select_language", properties);
    
    
    
    if(tag == LANGUAGE_VN_CODE || tag == LANGUAGE_EN_CODE || tag == LANGUAGE_THAI_CODE || tag == LANGUAGE_ID_CODE)
    {
        CleverTapManager::GetInstance()->storePendingEvent("click_popup",
        {
            {"select_language",cocos2d::Value("home_screen")},
            {"language",cocos2d::Value(tag == LANGUAGE_VN_CODE?"Vietnamese":(tag == LANGUAGE_THAI_CODE?"Thailand": (tag == LANGUAGE_ID_CODE ? "Indonesia" : "English")))},
        });
        
        MJDEFAULT->setIntegerForKey(key_lang_display, tag);
        LANGUAGE_MANAGER->updateLanguageWithID(tag);
    }
    else
    {
        MJDEFAULT->setIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);
        LANGUAGE_MANAGER->updateLanguageWithID(LANGUAGE_EN_CODE);
    }
    
    Director::getInstance()->replaceScene(MSIntro::createScene());
    
//    auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::REGISTER);
//    loginView->setOnLoginCallback(m_callback);
//    loginView->setVisible(false);
//    loginView->setName("loginView");
//    Director::getInstance()->getRunningScene()->addChild(loginView);
//
//    this->removeFromParent();
    
}

cocos2d::ui::Widget::ccWidgetClickCallback MSLanguageSelection::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(MSLanguageSelection, onLanguageSelected),
    };
    
    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}
