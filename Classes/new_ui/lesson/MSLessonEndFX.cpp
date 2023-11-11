//
//  MSLessonEndFX.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/26/17.
//

#include "MSLessonEndFX.h"
#include "GAFWrapper.h"
#include "MSLessonEnd.h"
#include "MSLessonManager.h"
#include "HSAudioEngine.h"
#include "HSLib.h"
#include "MJDefinitions.h"
#include "MJPlatformConfig.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(MSLessonEndFX);

std::once_flag ms_lesson_end_fx;
MSLessonEndFX * MSLessonEndFX::createFX(int lesson_id) {
    std::call_once(ms_lesson_end_fx, [] {
        REGISTER_CSB_READER(MSLessonEndFX);
    });
    
    auto fx = reinterpret_cast<MSLessonEndFX *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/LessonEndFX.csb"));
    if (fx) {
        fx->_lesson_id = lesson_id;
        return fx;
    }
    
    CC_SAFE_DELETE(fx);
    return nullptr;
}

void MSLessonEndFX::onEnter() {
    ui::Layout::onEnter();
    
    this->scheduleOnce([](float){
        AUDIO_ENGINE->playEffect("sounds/theendpage/endingpage_airplane.mp3");
    }, 0.1, "delay_sound_fx");
    auto plane = GAFWrapper::createGAF("gaf/maybay2/maybay2.gaf");
    plane->setAnchorPoint(Point::ANCHOR_MIDDLE);
//    plane->setPosition(Point(-200, this->getContentSize().height / 2));
    plane->setPosition(Point(-250, this->getContentSize().height*0.7));
    plane->play(true);
    this->addChild(plane, 19000000);
    
    plane->runAction(EaseQuarticActionOut::create(MoveBy::create(5, Vec2(2300, 0))));
    
    auto content = this->getChildByName<ui::Layout *>("content");
    content->setContentSize(Size(0, this->getContentSize().height));
    content->runAction(EaseQuarticActionOut::create(ResizeTo::create(5, Size(2300, this->getContentSize().height))));
    
    this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=]{
        showEndingLesson();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_lesson_ended);
    }), DelayTime::create(3), CallFunc::create([=] {
        HelperManager::GetInstance()->addSidebarBlack();
    }), NULL));
    
   // Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_lesson_ended);
}

void MSLessonEndFX::showEndingLesson(){
    showFireWork();
    AUDIO_ENGINE->playEffect("theendpage/endingpage_yeahyeah.mp3");
    auto img_title = ImageView::create("mjstory/theendpage/congratulation.png");
    auto size_screen = Director::getInstance()->getWinSize();
    auto w_img = size_screen.width/2;
    img_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    img_title->setPosition(Vec2(w_img, size_screen.height*1.3));
    img_title->setName("img_title");
    this->addChild(img_title, 10);
    
    if (CONFIG_MANAGER->isVietnameseStories()){
        img_title->setVisible(false);
        monkey_dance = GAFWrapper::createGAF("gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf");
        this->addChild(monkey_dance,10);
        monkey_dance->setScale(0.65f);
        monkey_dance->setPosition(Vec2(this->getContentSize().width*0.5, 200));
        monkey_dance->setVisible(false);
    }else{
        monkey_dance = GAFWrapper::createGAF("gaf/khi_dance/khi_dance.gaf");
        img_title->addChild(monkey_dance);
//        monkey_dance->setPosition(Vec2(img_title->getContentSize().width*0.57, 90));
        monkey_dance->setPosition(Vec2(img_title->getContentSize().width*0.42, -320));
        monkey_dance->setVisible(false);
    }
    
    auto img_hill = ImageView::create("mjstory/theendpage/theendpage_hill.png");
    img_hill->setPosition(Vec2(w_img, img_hill->getContentSize().height*(-0.6)));
    this->addChild(img_hill);
    img_hill->runAction(EaseSineOut::create(MoveTo::create(1.0f, Vec2(w_img, img_hill->getContentSize().height*0.45))));
    img_hill->setName("img_hill");
    if (img_hill->getContentSize().width < size_screen.width+100){
        img_hill->setScaleX((size_screen.width+100)*1.0/img_hill->getContentSize().width);
    }
    
    cocos2d::experimental::AudioEngine::play2d(CONFIG_MANAGER->isVietnameseStories()?"theendpage/congratulation_vi.mp3":"theendpage/congratulation.mp3");
    this->scheduleOnce([](float dt) {
        if (CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->playEffect("theendpage/endingpage_weldonebg.mp3");
        AUDIO_ENGINE->playEffect("theendpage/endingpage_tungho.mp3");
    }, 0.5, "delay_sound_fx_lesson_end");
    
    monkey_dance->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        phao_giay->setVisible(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        phao_trai->setVisible(true);
        phao_phai->setVisible(true);
        phao_hoa->setVisible(true);
#endif
        monkey_dance->setVisible(true);
        AUDIO_ENGINE->playEffect("theendpage/endingpage_monkeyjump.mp3");
        monkey_dance->play(false,[=](gaf::GAFObject* obj){
            // animation của con khỉ không chạy xuống hết màn hình, thêm action này
            if (CONFIG_MANAGER->isVietnameseStories()) {
                monkey_dance->runAction(MoveBy::create(0.4f, Point(0, -200)));
            }
        });
        monkey_dance->runAction(Sequence::create(DelayTime::create(3.75f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_monkeyjump.mp3");
        }), NULL));
        phao_giay->play(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        phao_trai->play(true);
        phao_phai->play(true);
        phao_hoa->play(true);
#endif
    }), DelayTime::create(4.0f), CallFunc::create([=]{
        img_title->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f, 1.1f), CallFunc::create([=]{
            monkey_dance->pauseAnimation();
            AUDIO_ENGINE->playEffect("theendpage/endingpage_titleout.mp3");
            img_title->runAction(MoveBy::create(0.5f, Vec2(0, 600)));
            img_hill->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -img_hill->getContentSize().height)), CallFunc::create([=]{
//                auto lesson = ms::LessonManager::getInstance().getLessonById(_lesson_id);
//                auto popup = MSLessonEnd::createView(lesson);
//                popup->setDelegate(this);
//                this->addChild(popup);
            }), NULL));
            
            this->runAction(Sequence::create(FadeOut::create(0.5), CallFunc::create([this] {
                this->removeFromParent();
            }), nullptr));
            
            phao_giay->removeFromParent();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            phao_trai->removeFromParent();
            phao_phai->removeFromParent();
            phao_hoa->removeFromParent();
#endif

        }), NULL));
    }), NULL));
    
    img_title->runAction(EaseSineOut::create(MoveTo::create(1.0f, Vec2(w_img, XHSLib->getSizeScreen().height+20))));
    
}

void MSLessonEndFX::showFireWork(){
    auto size_screen = Director::getInstance()->getWinSize();
    phao_giay = GAFWrapper::createGAF(HSLib::anims[2]);
    phao_giay->setPosition(Vec2(size_screen.width/2, size_screen.height/2));
    this->addChild(phao_giay, 100);
    phao_giay->play(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    phao_trai = GAFWrapper::createGAF(HSLib::anims[3]);
    phao_trai->setPosition(Vec2(250, 330));
    this->addChild(phao_trai, 100);
    phao_trai->play(true);
    
    
    phao_phai = GAFWrapper::createGAF(HSLib::anims[3]);
    phao_phai->setScaleX(-1);
    phao_phai->setPosition(Vec2(size_screen.width - 250, 330));
    this->addChild(phao_phai, 100);
    phao_phai->play(true);
    
    phao_hoa = GAFWrapper::createGAF(HSLib::anims[4]);
    phao_hoa->setPosition(Vec2(size_screen.width/2, size_screen.height/2));
    this->addChild(phao_hoa, 100);
    phao_hoa->play(true);
#endif
    
    phao_giay->setVisible(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    phao_trai->setVisible(false);
    phao_phai->setVisible(false);
    phao_hoa->setVisible(false);
#endif
}

void MSLessonEndFX::onStartOver() {
    this->removeFromParent();
}

void MSLessonEndFX::onContinueLearn(mj::model::ActInfo) {
    this->removeFromParent();
}

void MSLessonEndFX::onBackToLesson() {
    this->removeFromParent();
}
