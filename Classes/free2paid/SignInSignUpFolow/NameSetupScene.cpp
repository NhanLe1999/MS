#include "NameSetupScene.h"


USING_NS_CC;
INITIALIZE_READER(NameSetupScene);


std::once_flag ms_f2p_name_setup_reader;//4.

Scene* NameSetupScene::createScene()
{
    auto scene = Scene::create();
    if (scene) {
        auto view = NameSetupScene::createView();
        scene->addChild(view);
    }
    else {
        CC_SAFE_DELETE(scene);
    }
    return scene;
}
NameSetupScene* NameSetupScene::createView()
{
    std::call_once(ms_f2p_name_setup_reader, []
        {
            REGISTER_CSB_READER(NameSetupScene);
        });

    auto view = static_cast<NameSetupScene*>(CSLoader::createNodeWithVisibleSize("csb/free2paid/SignInSignUpFlow/NameSetup.csb"));

    if (view)
    {
        view->didLoadFromCSB();

        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void NameSetupScene::didLoadFromCSB()
{
    LANGUAGE_MANAGER->changeDisplayLanguage(this);
    mj::helper::autoScaleRootLayout(1.4, this, "textfield-bg");
    mj::helper::autoScaleRootLayout(1.4, this, "continue-button");
    mj::helper::autoScaleRootLayout(1.4, this, "lb.free2paid.create.child.profile");
    mj::helper::autoScaleRootLayout(1.4, this, "lb.free2paid.ask.name");
    if (auto placeholder = utils::findChild<TextField*>(this, "lb.placeholder.textfield.name.setup"))
    {
        placeholder->setPlaceHolderColor(Color4B(196, 196, 196, 255));
        auto placeholderText = placeholder->getPlaceHolder();
        cocos2d::ui::Text* text = cocos2d::ui::Text::create(placeholderText, placeholder->getFontName(), placeholder->getFontSize());
        auto a = text->getContentSize();
        placeholder->setContentSize(text->getContentSize());
    }
    if (auto textFieldBackground = utils::findChild<ImageView*>(this, "textfield-bg"))
    {
        _touchArea = textFieldBackground->getContentSize();
    }
    if (auto button_back = utils::findChild<Button*>(this, "button_back"))
    {
        button_back->setVisible(false);
    }
    flag = true;
    if (auto textfield = utils::findChild<ui::TextField*>(this, "mainTextField"))
    {
        textfield->setTextColor(Color4B(77, 77, 77, 255));
        textfield->setCursorEnabled(true);
        textfield->setCursorChar('|');
        textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
        textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        textfield->setMaxLengthEnabled(true);
        textfield->setMaxLength(24);
        textfield->setTextAreaSize(Size(_touchArea.x, _touchArea.y));
        _textFieldSizeBase = textfield->getContentSize();
        textfield->addEventListener([=](Ref* sender, ui::TextField::EventType event)
            {
                switch (event)
                {
                case TextField::EventType::ATTACH_WITH_IME:
                {
                    if (auto placeholder = utils::findChild<TextField*>(this, "lb.placeholder.textfield.name.setup"))
                    {
                        placeholder->setVisible(false);
                    }
                    CCLOG("xxx|ATTACH_WITH_IME");
                    break;
                }
                case TextField::EventType::DETACH_WITH_IME:
                {
                    if (textfield->getString() == "")
                    {
                        textfield->setContentSize(Size(_textFieldSizeBase.x, _textFieldSizeBase.y));
                        if (auto placeholder = utils::findChild<TextField*>(this, "lb.placeholder.textfield.name.setup"))
                        {
                            placeholder->setVisible(true);
                        }
                    }
                    CCLOG("xxx|DETACH_WITH_IME");
                    break;
                }
                case TextField::EventType::INSERT_TEXT:
                {
                    if (auto placeholder = utils::findChild<TextField*>(this, "lb.placeholder.textfield.name.setup"))
                    {
                        cocos2d::ui::Text* textOfTextField = cocos2d::ui::Text::create(textfield->getString(), textfield->getFontName(), textfield->getFontSize());
                    }
                    if (textfield->getString().size() < 2)
                    {
                        HELPER_MANAGER->trimSpace(textfield);
                    }
                    //HELPER_MANAGER->trimSpace(textfield);
                    _childName = textfield->getString();
                    CCLOG("%s", _childName.c_str());
                }
                case TextField::EventType::DELETE_BACKWARD:
                {
                    textfield->setInsertText(false);
                    //HELPER_MANAGER->trimSpace(textfield);
                    _childName = textfield->getString();
                    this->doCheckNext();
                    break;
                }
                default:break;
                }
            });
    }

    registerKeyboardEvent();

    scheduleWindowsTab([=]() {
        if (auto textField = utils::findChild<TextField*>(this, "textField")) {
            textField->attachWithIME();
        }
        });

    doCheckNext();

}

void NameSetupScene::onNext(cocos2d::Ref* sender)
{
    MJDEFAULT->setStringForKey(key_f2p_acount_name, _childName);
    
    auto timePicker = BirthdayPicker::createView([=](int day, int month, int year) {
        });
    Director::getInstance()->getRunningScene()->addChild(timePicker, INT_MAX - 1);

    timePicker->scrollToDefaultData(1, 1, 2019);

    this->removeFromParent();
}

void NameSetupScene::returnPreviousScreen(cocos2d::Ref* sender)
{
    this->removeFromParent();
}

void NameSetupScene::doCheckNext()
{
    auto name = utils::findChild<ui::TextField*>(this, "mainTextField")->getString();
    auto can_next = (!name.empty());
    auto btn_next = utils::findChild<ui::Button*>(this, "continue-button");
    btn_next->setBright(can_next);
    btn_next->setTouchEnabled(can_next);

}

void NameSetupScene::scheduleWindowsTab(std::function<void()> _callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    this->schedule([=](float dt) {

        if (_pressedKeys.size())
        {
            if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_SHIFT) != _pressedKeys.end())
            {
                return;
            }
            else if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_TAB) != _pressedKeys.end())
            {

                if (_callback) {
                    _pressedKeys.clear();
                    _callback();
                }

            }
        }

        }, "scheduleWindowsTab");
#endif
}

void NameSetupScene::scheduleEnter(std::function<void()> _callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    this->schedule([=](float dt) {

        if (_pressedKeys.size())
        {
            if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_ENTER) != _pressedKeys.end()) {

                if (_callback) {

                    _pressedKeys.clear();
                    _callback();
                }
            }
        }
        }, "scheduleEnter");
#endif
}

void NameSetupScene::registerKeyboardEvent()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, cocos2d:: Event* event) {
        _pressedKeys.insert(keyCode);
    };

    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        _pressedKeys.erase(keyCode);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

#endif
}

cocos2d::ui::Widget::ccWidgetClickCallback NameSetupScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(NameSetupScene, returnPreviousScreen),
        CLICK_MAP(NameSetupScene, onNext),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}
