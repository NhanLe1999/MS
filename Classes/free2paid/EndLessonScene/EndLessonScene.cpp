#include "EndLessonScene.h"
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
#include "HelperManager.h"
#include "MJHelper.h"
#include "MSFreeContentPage.h"

USING_NS_CC;
INITIALIZE_READER(EndLessonScene);

#define MS_END_LESSON_SCENE_NAME "EndLessonScene"

std::once_flag ms_f2p_end_lesson_scene_reader;//4.

Scene* EndLessonScene::createScene(ms::f2p::FreeContentOption freeContentOption)
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = EndLessonScene::createView(freeContentOption);
        view->setName("EndLessonScene");
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

EndLessonScene* EndLessonScene::createView(ms::f2p::FreeContentOption freeContentOption)
{
    std::call_once(ms_f2p_end_lesson_scene_reader, []
        {
            REGISTER_CSB_READER(EndLessonScene);
        });

    auto view = static_cast<EndLessonScene*>(CSLoader::createNodeWithVisibleSize("csb/free2paid/EndLessonScene.csb"));
    if (view)
    {
        view->didLoadFromCSB(freeContentOption);

        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void EndLessonScene::didLoadFromCSB(ms::f2p::FreeContentOption freeContentOption)
{
    this->setName(MS_END_LESSON_SCENE_NAME);
    _freeContentOption = freeContentOption;
    auto myScreenContentSize = Director::getInstance()->getVisibleSize();

    if (auto myLayout = utils::findChild<Layout*>(this, "root_layout")) {
        myLayout->setPosition(myScreenContentSize / 2);
        float maxScaleSize = 1.3;
        mj::helper::autoScaleRootLayout(maxScaleSize, this, "background");

    }
    auto urls = MJDEFAULT->getArrayStringForKey(key_f2p_save_items_learned_avata_url, {});
    
    if (getFreeContentOption() != ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
    {
        if (urls.size() >= 1)
        {
            this->showScreenOneImage(urls.at(0));
        }
        else
        {
            this->showScreenOneImage("free2paid/images/book1.png");
        }

    }
    else
    {
        if (urls.size() >= 2)
        {
            this->showScreenTwoImage(urls.at(0), urls.at(1));
        }
        else
        {
            this->showScreenTwoImage();
        }

    }
    this->setContentSize(myScreenContentSize);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //preview->setContentSize(this->getContentSize());
    this->setPosition(Vec2(myScreenContentSize.width / 2, myScreenContentSize.height / 2));
    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0
}

void EndLessonScene::onClickTopButton(cocos2d::Ref* sender)
{
    if (_onUnlockCallback)
    {
        _onUnlockCallback();
        this->removeFromParent();
    }else{
        auto subscribeTab = MSNewTabSubscribe::createTab();
        subscribeTab->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
        subscribeTab->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
 
        subscribeTab->setName(mshome_name_tabsubscribe);
        this->removeChildByName(mshome_name_tabsubscribe);
        this->addChild(subscribeTab, kMSHomeZOrder_parentSetting);
        subscribeTab->runActionSequenAnimation();
        subscribeTab->setOpacity(0);
        subscribeTab->runAction(FadeIn::create(1));
    }
}

void EndLessonScene::onClickBottomButton(cocos2d::Ref* sender)
{
    if (_onComeBackLaterCallback)
    {
        _onComeBackLaterCallback();
        this->removeFromParent();
    }
    else{
        auto scene = MSFreeContentPage::createScene();
       Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, scene));
    }
}

ms::f2p::FreeContentOption EndLessonScene::getFreeContentOption()
{
    return _freeContentOption;
}


void EndLessonScene::showScreenOneImage(std::string imageLessonPath)
{
    if (auto myPanel2 = utils::findChild<Layout*>(this, "panel_2")) {
        myPanel2->setVisible(false);
    }
    if (auto panel1 = utils::findChild<Layout*>(this, "panel_1")) {
        panel1->setVisible(true);
        if (imageLessonPath != "")
        {
            auto avata = RemoteImageView::createImage(imageLessonPath, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::OTHER);
            if (auto myBookImage = utils::findChild<ImageView*>(panel1, "image_book"))
            {
                myBookImage->setVisible(false);
                myBookImage->getParent()->addChild(avata, myBookImage->getZOrder());
                avata->setContentSize(myBookImage->getContentSize());
                avata->setAnchorPoint(myBookImage->getAnchorPoint());
                avata->setPosition(myBookImage->getPosition());

                avata->setVisible(true);
                if (auto monkey = utils::findChild<ImageView*>(panel1, "image_monkey"))
                {
                    monkey->setZOrder(avata->getZOrder() + 1);
                }
            }
        }
    }
}

void EndLessonScene::showScreenTwoImage(std::string imageLessonPath_1, std::string imageLessonPath_2)
{
    if (auto myPanel1 = utils::findChild<Layout*>(this, "panel_1")) {
        myPanel1->setVisible(false);
    }

    if (auto panel_2 = utils::findChild<Layout*>(this, "panel_2")) {
        panel_2->setVisible(true);
        if (imageLessonPath_1 != "")
        {
            auto avata_1 = RemoteImageView::createImage(imageLessonPath_1, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::OTHER);
            if (auto myBookImage_1 = utils::findChild<ImageView*>(panel_2, "image_book_1"))
            {
                myBookImage_1->setVisible(false);
                myBookImage_1->getParent()->addChild(avata_1, myBookImage_1->getZOrder());
                avata_1->setContentSize(myBookImage_1->getContentSize());
                avata_1->setAnchorPoint(myBookImage_1->getAnchorPoint());
                avata_1->setPosition(myBookImage_1->getPosition());
                avata_1->setVisible(true);
            }
            if (auto monkey = utils::findChild<ImageView*>(panel_2, "image_monkey"))
            {
                monkey->setZOrder(avata_1->getZOrder() + 1);
            }
        }

        if (imageLessonPath_2 != "")
        {
            auto avata_2 = RemoteImageView::createImage(imageLessonPath_2, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::OTHER);
            if (auto myBookImage_2 = utils::findChild<ImageView*>(panel_2, "image_book_2"))
            {
                myBookImage_2->setVisible(false);
                myBookImage_2->getParent()->addChild(avata_2, myBookImage_2->getZOrder());
                avata_2->setContentSize(myBookImage_2->getContentSize());
                avata_2->setAnchorPoint(myBookImage_2->getAnchorPoint());
                avata_2->setPosition(myBookImage_2->getPosition());
                avata_2->setVisible(true);
            }
        }
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback EndLessonScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(EndLessonScene, onClickTopButton),
        CLICK_MAP(EndLessonScene, onClickBottomButton),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}
