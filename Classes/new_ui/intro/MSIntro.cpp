//
//  MSIntro.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/27/18.
//

#include "MSIntro.h"
#include "GAFWrapper.h"
#include "ActionFlipPage.h"
#include "MJDefinitions.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "MMFirebaseDatabase.h"

#include "MSIntroLoad.h"
#include "MJDefault.h"
#include "MJHelper.h"

#include "platform/CCPlatformConfig.h"

std::once_flag ms_intro_reader_flag;

USING_NS_CC;
INITIALIZE_READER(MSIntro);

MSIntro * MSIntro::createIntro() {
    std::call_once(ms_intro_reader_flag, [] {
        REGISTER_CSB_READER(MSIntro);
    });
    
    auto csb_name = "csb/new_ui/intro/Intro.csb";
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    csb_name = "csb/new_ui/intro/Intro_Mac.csb";
#endif
    
    auto csb_name_VN = mj::helper::getCSBName(csb_name);
    auto intro = reinterpret_cast<MSIntro *>(CSLoader::createNodeWithVisibleSize(csb_name_VN));
    if (intro) {
        intro->didLoadFromCSB();
        return intro;
    }
    
    CC_SAFE_DELETE(intro);
    return nullptr;
}

Scene * MSIntro::createScene() 
{
    auto scene = Scene::create();

    if (scene) 
	{
        scene->setName(ms_scene_name_intro);
        
        auto bg_img = "bg.jpg";
        if(CONFIG_MANAGER->isVietnameseStories())
		{
            bg_img = "mjstory/new_ui_vn/bg_loading.png";
        }

        auto bg = ui::ImageView::create(bg_img);
        bg->setPosition(Director::getInstance()->getVisibleSize() / 2);
        bg->setContentSize(Director::getInstance()->getVisibleSize());
        bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
        scene->addChild(bg);
        
        auto load_view = MSIntroLoad::createView();
        scene->addChild(load_view);
        load_view->setLocalZOrder(0);
        load_view->setVisible(false);
        load_view->setName("loadview");
        
        if (!MJDEFAULT->getBoolForKey(key_first_api_called, true)) 
		{
            load_view->setVisible(true);
        } 
		else 
		{
            if(MJDEFAULT->getBoolForKey(key_onboarding_part_1, true))
			{
                load_view->setVisible(true);
            }
			else
			{
                auto view = MSIntro::createIntro();
                view->setName("intro_onboard");

                auto bg2 = bg->clone();
                bg2->setLocalZOrder(-1);
                bg2->setName("bg");
                view->addChild(bg2);
                scene->addChild(view);
                view->setLocalZOrder(10);
            }
        }
  
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void MSIntro::didLoadFromCSB() {
    if (CONFIG_MANAGER->isVietnameseStories()) {
        auto logo = this->getChildByName<ui::ImageView *>("logo");
        logo->loadTexture("mjstory/monkey_logo_vi.png");
    }
    
    _current_screen = IntroScreen::SPLASH;
    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    scrollview->jumpToTop();
//    scrollview->scrollToBottom(10, true);
    
    std::map<std::string, std::string> button_gaf;
    if (CONFIG_MANAGER->isVietnameseStories()) {
		button_gaf = std::map<std::string, std::string>({
            {"button_stories", "stories/stories.gaf"},
            {"button_audiobooks", "audiobooks/audiobooks.gaf"}
        });
        std::vector<std::string> list_name = {"sidebar.des.stories", "sidebar.des.audiobooks", "sidebar.des.parents"};
        for (int i = 0; i < (int) list_name.size(); i++) {
            auto text = utils::findChild<ui::Text *>(this, list_name[i]);
            if (text) {
                text->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), list_name[i]));
            }
        }
        
        auto btlesson = utils::findChild<ui::Button *>(this, "button_lessons");
        btlesson->setVisible(false);
        utils::findChild<ui::Button *>(this, "button_audiobooks")->setPosition(btlesson->getPosition());
        
        utils::findChild(this, StringUtils::format("dot_%d", 2))->setVisible(false);
        for (int i = 4; i > 2; i--) {
            auto _dot = utils::findChild(this, StringUtils::format("dot_%d", i));
            _dot->setPositionX(_dot->getPositionX()-33.75);
        }
        utils::findChild(this, "panel_dot")->setPositionX(Director::getInstance()->getWinSize().width/2 + 33.75/2);
        
        utils::findChild<cocos2d::ui::ImageView *>(this, "page_1")->loadTexture("mjstory/new_ui/intro_page_1_vn.png");
        utils::findChild<cocos2d::ui::ImageView *>(this, "page_2")->loadTexture("mjstory/new_ui/intro_page_2_vn.png");
    }else {
		button_gaf = std::map<std::string, std::string>({
            {"button_stories", "stories/stories.gaf"},
            {"button_lessons", "lessons/lessons.gaf"},
            {"button_audiobooks", "audiobooks/audiobooks.gaf"},
            {"button_material", ""},
        });
    }
    
    for (auto p : button_gaf) {
        if (p.first.compare("button_material") == 0) {
            continue;
        }
        
        auto button = utils::findChild<ui::Button *>(this, p.first);
        auto gaf = GAFWrapper::createGAF("gaf/sidebar/" + p.second);
        auto image = button->getChildByName("image");
        gaf->setPosition(image->getPosition() + Point(0, 5));
        gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
        gaf->setName("gaf");
        button->addChild(gaf);
        gaf->playSequence("idle2");
        image->removeFromParent();
    }
    
    auto sample = utils::findChild(this, "sample");
    sample->setVisible(false);
    
    utils::findChild<ui::Widget *>(this, "touch_layer")->setEnabled(false);
    
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setOpacity(0);
    auto monkey = GAFWrapper::createGAF("gaf/monkey_load/monkey_load.gaf");
    monkey->setName("monkey");
    monkey->setPosition(title->getPosition());
    this->addChild(monkey);
    monkey->play(true);

    auto subtitle = utils::findChild<ui::Text *>(this, "subtitle");
    subtitle->setTextColor(Color4B(0xff, 0xff, 0xff, 225));
    subtitle->setString("");
    
    auto panel_dot = utils::findChild(this, "panel_dot");
    panel_dot->setVisible(false);
    
    utils::findChild(this, "button_started")->setOpacity(0);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_device_registered, [=](EventCustom * e) {
        monkey->runAction(Sequence::create(FadeOut::create(0.25), CallFuncN::create([](Node * n) { n->setVisible(false); }), NULL));
        title->setString("");
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        utils::findChild<ui::Text *>(this, "subtitle")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("intro.drag"));
#else
        utils::findChild<ui::Text *>(this, "subtitle")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("intro.swipe"));
#endif
        title->runAction(FadeIn::create(0.25));
        utils::findChild<ui::Widget *>(this, "touch_layer")->setEnabled(true);
        
        if (CONFIG_MANAGER->isVietnameseStories() && LANGUAGE_MANAGER->getDisplayLangId() == 4) {
            utils::findChild<ui::Text *>(this, "label_started")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("intro.start.vi"));
        } else {
            utils::findChild<ui::Text *>(this, "label_started")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("intro.start"));
        }
        
        panel_dot->setVisible(true);
        auto counter = 0;
        for (auto c : panel_dot->getChildren()) {
            c->setPosition(c->getPosition() + Vec2(0, -100));
            c->setOpacity(0);
            c->runAction(FadeIn::create(0.25));
            c->runAction(Sequence::create(DelayTime::create(counter++ * .1), EaseBackOut::create(MoveBy::create(0.5, Vec2(0, 100))), NULL));
        }

		auto key_listener = EventListenerKeyboard::create();
		key_listener->onKeyReleased = [this](EventKeyboard::KeyCode key_code, Event * e) {
			switch (key_code) {
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_DPAD_LEFT:
				this->onPrevScreen();
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
				this->onNextScreen();
				break;
			default:
				break;
			}
		};

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);
        
        this->schedule([this](float) {
            showHand();
        }, 5, 100, 5, "show_guide");
    }), this);
}

void MSIntro::onSelectItem(std::string item_name) {
    std::map<std::string, std::string> scrolls;
    if (CONFIG_MANAGER->isVietnameseStories()) {
		scrolls = std::map<std::string, std::string>({
            {"button_stories", "mjstory/new_ui/scroll_stories2.png"},
            {"button_audiobooks", "mjstory/new_ui/scroll_audiobooks2.png"},
        });
    }else {
		scrolls = std::map<std::string, std::string>({
            {"button_stories", "mjstory/new_ui/scroll_stories.png"},
            {"button_lessons", "mjstory/new_ui/scroll_lessons.png"},
            {"button_audiobooks", "mjstory/new_ui/scroll_audiobooks.png"},
            {"button_material", "mjstory/material/mat_btn_tab_worksheet.png"},
        });
    }
    
    for (auto p : scrolls) {
        auto bt_name = p.first;
        auto bt = utils::findChild(this, bt_name);
        bt->getChildByName<GAFWrapper *>("gaf")->playSequence("idle");
    }
    
    auto button_background = utils::findChild(this, "button_background");
    auto button = utils::findChild(this, item_name);
    auto gaf = button->getChildByName<GAFWrapper *>("gaf");
    gaf->playSequence("action");
    
    button_background->runAction(EaseQuarticActionOut::create(MoveTo::create(0.25, button->getPosition())));
    
    auto scroll_view = utils::findChild<ui::ScrollView *>(this, "scrollview");
    scroll_view->setName("");
    auto new_scroll_view = reinterpret_cast<ui::ScrollView *>(scroll_view->clone());
    new_scroll_view->setName("scrollview");
    new_scroll_view->cocos2d::Node::getChildByName<ui::ImageView *>("content")->loadTexture(scrolls.at(item_name));
    new_scroll_view->cocos2d::Node::getChildByName<ui::ImageView *>("content")->ignoreContentAdaptWithSize(true);
    new_scroll_view->setInnerContainerSize(new_scroll_view->cocos2d::Node::getChildByName<ui::ImageView *>("content")->getVirtualRendererSize());
    new_scroll_view->jumpToTop();
    scroll_view->getParent()->addChild(new_scroll_view);
    
    scroll_view->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) {
        n->removeFromParent();
    }), NULL));
    
    new_scroll_view->setOpacity(0);
    new_scroll_view->runAction(FadeIn::create(0.5));

    std::vector<int> scroll_percent = {0, 0, 0};
    switch (_current_screen) {
        case STORIES:
            scroll_percent = {36, 72, 100};
            break;
            
        case AUDIOBOOKS:
            scroll_percent = {31, 62, 100};
            break;
            
        case LESSONS:
            scroll_percent = {30, 60, 100};
            break;
            
        default:
            break;
    }

    new_scroll_view->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create([scroll_percent](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(scroll_percent[0], 1, true);
    }), DelayTime::create(2), CallFuncN::create([scroll_percent](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(scroll_percent[1], 1, true);
    }), DelayTime::create(2), CallFuncN::create([scroll_percent](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(scroll_percent[2], 1, true);
    }), NULL));
}

void MSIntro::onTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touch) {
    auto w = reinterpret_cast<ui::Widget *>(sender);
    switch (touch) {
        case ui::Widget::TouchEventType::BEGAN:
            break;
            
        case ui::Widget::TouchEventType::MOVED:
            break;
            
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED: {
            auto sp = w->getTouchBeganPosition();
            auto ep = w->getTouchEndPosition();
            if (sp.distance(ep) < 100.f)
                break;
            auto v = ep - sp;
            auto a = v.getAngle();
            if (fabs(M_PI_2 - fabs(a)) > M_PI / 3) {
                if (fabs(a) > M_PI_2) {
                    onNextScreen();
                } else {
                    onPrevScreen();
                }
            }
            break;
        }
            
        default:
            break;
    }
}

void MSIntro::onNextScreen() {
    if (_current_screen == 4) {
        return;
    }

    auto current_dot = utils::findChild(this, StringUtils::format("dot_%d", _current_screen));
    current_dot->runAction(TintTo::create(0.5, Color3B::WHITE));
    
    _current_screen = (IntroScreen)(_current_screen + 1);
    if (CONFIG_MANAGER->isVietnameseStories() && _current_screen == IntroScreen::LESSONS) {
        _current_screen = IntroScreen::AUDIOBOOKS;
    }
    
    auto tint_color = Color3B(16, 75, 99);
    if(CONFIG_MANAGER->isVietnameseStories()){
        tint_color = Color3B(255, 203, 0);
    }
    current_dot = utils::findChild(this, StringUtils::format("dot_%d", _current_screen));
    current_dot->runAction(TintTo::create(0.5, tint_color));
    
    changeText(false);
    
    switch (_current_screen) {
        case IntroScreen::STORIES:
            onShowDevice();
            break;
            
        case IntroScreen::AUDIOBOOKS:
            onSelectItem("button_audiobooks");
            break;
            
        case IntroScreen::LESSONS:
            onSelectItem("button_lessons");
            break;
            
        case IntroScreen::IN_STORIES:
            showSample();
            break;
            
        default:
            break;
    }
    
    this->unschedule("show_guide");
    this->removeChildByName("hand_guide");
    if (_current_screen != IntroScreen::IN_STORIES) {
        this->schedule([this](float) {
            showHand();
        }, 7, 100, 5, "show_guide");
    }
}

void MSIntro::onPrevScreen() {
    if (_current_screen == 0) {
        return;
    }
    
    auto current_dot = utils::findChild(this, StringUtils::format("dot_%d", _current_screen));
    current_dot->runAction(TintTo::create(0.5, Color3B::WHITE));
    
    _current_screen = (IntroScreen)(_current_screen - 1);
    if (CONFIG_MANAGER->isVietnameseStories() && _current_screen == IntroScreen::LESSONS) {
        _current_screen = IntroScreen::STORIES;
    }

    auto tint_color = Color3B(16, 75, 99);
    if(CONFIG_MANAGER->isVietnameseStories()){
        tint_color = Color3B(255, 203, 0);
    }
    current_dot = utils::findChild(this, StringUtils::format("dot_%d", _current_screen));
    current_dot->runAction(TintTo::create(0.5, tint_color));
    
    changeText(true);
    
    switch (_current_screen) {
        case IntroScreen::SPLASH:
            onHideDevice();
            break;
            
        case IntroScreen::STORIES:
            onSelectItem("button_stories");
            break;
            
        case IntroScreen::MATERIAL:
            onSelectItem("button_material");
            break;
            
        case IntroScreen::AUDIOBOOKS:
            hideSample();
            onSelectItem("button_audiobooks");
            break;
            
        case IntroScreen::LESSONS:
            onSelectItem("button_lessons");
            break;
        case IntroScreen::AI_SPEAKING:
            onSelectItem("button_ai_speaking");
            break;
            
        case IntroScreen::IN_STORIES:
            break;
    }
    
    this->removeChildByName("hand_guide");
    this->schedule([this](float) {
        showHand();
    }, 5, 100, 5, "show_guide");
}

void MSIntro::changeText(bool from_left) {
    std::vector<std::string> intro_texts;
    std::vector<std::string> intro_subtitle;
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == 4 && CONFIG_MANAGER->isVietnameseStories())  {
        intro_texts = {
            LANGUAGE_MANAGER->getDisplayTextByKey(""),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.stories"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.lessons"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.audiobooks"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.samplestory.vi")
        };
        
         intro_subtitle = {
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.swipe"),
    #else
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.drag"),
    #endif
       
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.stories.sub.vi"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.lessons.sub.vi"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.audiobooks.sub.vi"),
            LANGUAGE_MANAGER->getDisplayTextByKey("")
         };
    } else {
        intro_texts = {
            LANGUAGE_MANAGER->getDisplayTextByKey(""),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.stories"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.lessons"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.audiobooks"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.samplestory")
        };
        
        intro_subtitle = {
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.swipe"),
    #else
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.drag"),
    #endif
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.stories.sub"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.lessons.sub"),
            LANGUAGE_MANAGER->getDisplayTextByKey("intro.audiobooks.sub"),
            LANGUAGE_MANAGER->getDisplayTextByKey("")
        };
    }
    
    
    auto current_title = utils::findChild<ui::Text *>(this, "title");
    auto new_title = reinterpret_cast<ui::Text *>(current_title->clone());
    new_title->setString(mj::helper::to_upper(intro_texts[_current_screen]));
    
    current_title->setName("");
    new_title->setName("title");
    current_title->getParent()->addChild(new_title);
    
    new_title->setPosition(current_title->getPosition() + (from_left ? -1 : 1) * Point(800, 0));
    new_title->setOpacity(0);
    
    new_title->runAction(FadeIn::create(0.5));
    new_title->runAction(EaseCircleActionOut::create(MoveTo::create(0.5, Point(this->getContentSize().width / 2, new_title->getPositionY()))));
    
    current_title->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) { n->removeFromParent(); }), nullptr));
    current_title->runAction(EaseCircleActionOut::create(MoveBy::create(0.5, (from_left ? 1 : -1) * Point(800, 0))));
    
    current_title = utils::findChild<ui::Text *>(this, "subtitle");
    new_title = reinterpret_cast<ui::Text *>(current_title->clone());
    new_title->setString(intro_subtitle[_current_screen]);
    
    current_title->setName("");
    new_title->setName("subtitle");
    current_title->getParent()->addChild(new_title);
    
    new_title->setPosition(current_title->getPosition() + (from_left ? -1 : 1) * Point(800, 0));
    new_title->setOpacity(0);
    
    new_title->runAction(FadeIn::create(0.5));
    new_title->runAction(EaseCircleActionOut::create(MoveTo::create(0.5, Point(this->getContentSize().width / 2, new_title->getPositionY()))));
    
    current_title->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) { n->removeFromParent(); }), nullptr));
    current_title->runAction(EaseCircleActionOut::create(MoveBy::create(0.5, (from_left ? 1 : -1) * Point(800, 0))));
}

void MSIntro::onShowDevice() {
    
    auto device = utils::findChild(this, "device");
    device->runAction(EaseCircleActionOut::create(MoveBy::create(0.5, Vec2(0, -600))));
    
    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    if (CONFIG_MANAGER->isVietnameseStories()) {
        scrollview->cocos2d::Node::getChildByName<ui::ImageView *>("content")->loadTexture("mjstory/new_ui/scroll_stories2.png");
    }
    scrollview->stopAllActions();
    scrollview->jumpToTop();
    scrollview->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create([](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(36, 1, true);
    }), DelayTime::create(2), CallFuncN::create([](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(72, 1, true);
    }), DelayTime::create(2), CallFuncN::create([](Node * n) {
        auto s = reinterpret_cast<ui::ScrollView *>(n);
        s->scrollToPercentVertical(100, 1, true);
    }), NULL));    
}

void MSIntro::onHideDevice() {
    auto device = utils::findChild(this, "device");
    device->runAction(EaseCircleActionOut::create(MoveBy::create(0.5, Vec2(0, 600))));
}

void MSIntro::showSample() {
    auto sample = utils::findChild(this, "sample");
    sample->stopAllActions();
    sample->setVisible(true);
    sample->setOpacity(0);
    
    auto page_1 = utils::findChild(this, "page_1");
    page_1->stopAllActions();
    page_1->setNodeToParentTransform(Mat4::IDENTITY);
    auto page_2 = utils::findChild(this, "page_2");
    page_2->stopAllActions();
    page_2->setVisible(false);

    sample->runAction(Sequence::create(FadeIn::create(0.5), CallFunc::create([page_1, page_2] {
        page_2->setVisible(true);
        page_1->runAction(Sequence::create(DelayTime::create(3), ActionFlipPage::create(1, false), nullptr));
    }), nullptr));
    
    auto button = utils::findChild(this, "button_started");
    button->setLocalZOrder(1);
    button->setPosition(Point(this->getContentSize().width / 2, button->getPositionY()));
    button->setVisible(false);
    button->setScale(0.2);
    button->setOpacity(0x00);
    button->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create([](Node * n) {
        n->setVisible(true);
        n->runAction(EaseBackOut::create(ScaleTo::create(0.5, 1)));
        n->runAction(FadeIn::create(0.25));
    }), NULL));
    
//    utils::findChild(this, "title")->setVisible(false);
}

void MSIntro::hideSample() {
    auto sample = utils::findChild(this, "sample");
    sample->stopAllActions();
    sample->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([=](Node * n) {
        n->setVisible(false);
    }), nullptr));
    
    auto page_1 = utils::findChild(this, "page_1");
    page_1->stopAllActions();
    
    auto page_2 = utils::findChild(this, "page_2");
    page_2->stopAllActions();
    
    auto button = utils::findChild(this, "button_started");
    button->runAction(FadeOut::create(0.5));
    button->runAction(EaseCircleActionOut::create(MoveTo::create(0.5, Point(this->getContentSize().width / 2 + 800, button->getPositionY()))));
    
    utils::findChild(this, "title")->setVisible(true);
}

#include "MSEventLogger.h"
void MSIntro::onStart(cocos2d::Ref *sender) {
    MS_LOGGER.logEventExplore();
    
    auto scene = Director::getInstance()->getRunningScene();
    auto load_view = scene->getChildByName("loadview");
    load_view->setVisible(true);
    load_view->setOpacity(0);
    load_view->runAction(FadeIn::create(0.5));
    this->runAction(Sequence::create(DelayTime::create(0.6), CallFuncN::create([](Node * n) {
        n->removeFromParent();
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_intro_completed);
    }), NULL));
    for (auto c : this->getChildren()) {
        if (c->getName() == "bg") {
            c->runAction(FadeOut::create(0.25));
            continue;
        }
        c->runAction(FadeOut::create(0.5));
        if (c->getPositionY() >= this->getContentSize().height/ 2) {
            c->runAction(MoveBy::create(0.5, Vec2(0, 1000)));
        } else {
            c->runAction(MoveBy::create(0.5, Vec2(0, -1000)));
        }
    }
    
    MJDEFAULT->setBoolForKey(key_onboarding_part_1, true);
}

void MSIntro::reloadIntro() {
    auto scene = Director::getInstance()->getRunningScene();
    auto load_view = scene->getChildByName("loadview");
    
    if (load_view) {
        ((MSIntroLoad *)load_view)->reloadIntro();
    }
}

ui::Widget::ccWidgetTouchCallback MSIntro::onLocateTouchCallback(const std::string &name) {
    if (name == "onTouch") {
        return CC_CALLBACK_2(MSIntro::onTouch, this);
    }
    
    return nullptr;
}

ui::Widget::ccWidgetClickCallback MSIntro::onLocateClickCallback(const std::string &name) {
    if (name == "onStart") {
        return CC_CALLBACK_1(MSIntro::onStart, this);
    }
    
    return nullptr;
}

void MSIntro::showHand() {
    auto hand = ui::ImageView::create("mjstory/new_ui/hand_swipe.png");
    hand->setPosition(Point(Director::getInstance()->getVisibleSize().width - 200, 150));
    cocos2d::FiniteTimeAction * action = MoveTo::create(2, Point(200, 150));
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    hand = ui::ImageView::create("mjstory/new_ui/arrow_key_left.png");
    hand->setPosition(Point(Director::getInstance()->getVisibleSize().width / 2, 150));
    action = Sequence::create(FadeIn::create(0.5), DelayTime::create(3), NULL);
#endif
    hand->setName("hand_guide");
    auto title = utils::findChild(this, "title");
    auto subtitle = utils::findChild(this, "subtitle");
    hand->runAction(Sequence::create(CallFunc::create([title, subtitle] {
        title->runAction(FadeOut::create(0.5));
        subtitle->runAction(FadeOut::create(0.5));
    }), action, CallFunc::create([title, subtitle] {
        title->runAction(FadeIn::create(0.5));
        subtitle->runAction(FadeIn::create(0.5));
    }), RemoveSelf::create(), nullptr));
    
    this->addChild(hand);
}
