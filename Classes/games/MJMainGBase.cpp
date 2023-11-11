//
//  MJMainGBase.cpp
//
//  Created by Ha Van Hoi on 3/9/16.
//
//
#include "MJMainGBase.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MJPlatformConfig.h"
#include "HelperManager.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <regex.h>
#endif

void MJMainGBase::onEnter(){
    cocos2d::ui::Layout::onEnter();
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    auto layout_block_touch = ui::Layout::create();
    layout_block_touch->setContentSize(Director::getInstance()->getVisibleSize());
    layout_block_touch->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    layout_block_touch->setPosition(Point::ZERO);
    if (CONFIG_MANAGER->isAutoPlay()) {
        layout_block_touch->setTouchEnabled(true);
        layout_block_touch->setSwallowTouches(true);
        this->addChild(layout_block_touch, INT_MAX - 1);
    }
    else {
        layout_block_touch->setTouchEnabled(true);
        this->addChild(layout_block_touch, -1);
    }
}

void MJMainGBase::onExit()
{
    cocos2d::ui::Layout::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
  ///  HelperManager::GetInstance()->addSidebarBlack();
}

int MJMainGBase::playBackgroundMusic(std::string file_path, float volume){
     if(MJDEFAULT->getBoolForKey(key_background_music)){
        this->scheduleOnce([=](float){
            auto audio_id = AUDIO_ENGINE->playEffect(file_path, true);
            cocos2d::experimental::AudioEngine::setVolume(audio_id, volume);
            return audio_id;
        }, 0.5f, "playBackgroundMusic");
     }
    return -1;
}

void MJMainGBase::setConfigImages(std::vector<math::resource::Image*> images)
{
	m_images = images;
}


void MJMainGBase::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    this->hideTapGuideGame();
    this->removeAllChildren();
    getGameSTT("onExit");
//    AUDIO_ENGINE->stopAllEffects();
    
}

void MJMainGBase::onFinishGame(){
    if (CONFIG_MANAGER->isVietnameseStories()){
        onFinishGameVMonkey();
        return;
    }
    AUDIO_ENGINE->stopAllAudio();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    getGameSTT("finish");
}

void MJMainGBase::onFinishGameVMonkey(){
    AUDIO_ENGINE->stopAllAudio();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    getGameSTT("finish_game_Vmonkey");
}

void MJMainGBase::startParticle(int type, cocos2d::Node *parent){
    auto particle_index = random(1,6);
    ParticleSystemQuad *exploisive = ParticleSystemQuad::create(StringUtils::format("plist/particle/particle_acti_%d.plist", particle_index));
    this->addChild(exploisive, 100001);
    exploisive->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height+50));
    //    exploisive->setDisplayFrame(cocos2d::SpriteFrame::create("particle/9.png", cocos2d::Rect(0,0,100,100)));
    exploisive->start();
//    exploisive->setVisible(false);
    exploisive->setName("particle");
}

void MJMainGBase::showTapGuideGame(Vec2 pos, int loop, float delay_time, std::string gaf_guide_path){
    auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
    if (guide_layout_check) return;
    auto guide_layout = cocos2d::ui::Layout::create();
    guide_layout->setContentSize(screen_size);
    Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
    guide_layout->setName("guide_layout");
    
    guide_layout->schedule([=](float) {
        auto hand = GAFWrapper::createGAF(gaf_guide_path);
        guide_layout->addChild(hand, 1900000000);
        hand->setPosition(pos);
        hand->play(false, [=] (gaf::GAFObject *obj){
            hand->removeFromParent();
        });
    }, 5, loop, delay_time, "animated_guide");
}

void MJMainGBase::showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop, float delay_time, float dt,std::string gaf_guide_path, cocos2d::ui::Layout* parentLayout){
    auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
    if (guide_layout_check)
    {
        guide_layout_check->removeFromParent();
    }
    auto guide_layout = cocos2d::ui::Layout::create();
    guide_layout->setContentSize(screen_size);
    
    if (parentLayout == NULL) {
        Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX );
    }
    else {
        parentLayout->addChild(guide_layout, parentLayout->getZOrder());
    }
    
    guide_layout->setName("guide_layout");
    
    guide_layout->schedule([=](float) {
        auto hand = GAFWrapper::createGAF(gaf_guide_path);
        guide_layout->addChild(hand, 1900000000);
        hand->setPosition(posStart);
        hand->play(false, [=] (gaf::GAFObject *obj){
            hand->runAction(Sequence::create(MoveTo::create(0.6f, posEnd),
                                             CallFunc::create([=](){
                hand->play(false, [=] (gaf::GAFObject *obj){
                    hand->removeFromParent();
                });
            }), NULL));
        });
    }, dt, loop, delay_time, "animated_guide");
}

void MJMainGBase::hideTapGuideGame(){
    if(Director::getInstance()->getRunningScene()->getChildByName("guide_layout")){
         Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
    }
   
}

void MJMainGBase::setDelegate(MJMainGBaseDelegate *delegate){
    _delegate = delegate;
}

void MJMainGBase::getGameSTT(string stt){
    if(_delegate){
        _delegate->getGameStatus(stt);
    }
}

MJMainGBase * MJMainGBase::createActivity(mj::model::ActInfo info, std::string configPath, std::vector<math::resource::Image*> images)
{
    MJMainGBase * gameView = nullptr;
//    switch (info.sub_id)
//    {
//
//        case 180:
//            gameView = GymGame::createGame(configPath, false);
//            break;
//        case 181:
//            gameView = PenguinGame::createGame(configPath, false);
//            break;
//        case 184:
//            gameView = Snail::createGame(configPath);
//            break;
//        case 188:
//            gameView = DinosaurChasingGame::createGame(configPath, false);
//            break;
//        case 189:
//            gameView = WhackAMoleGame::createGame(configPath, false);
//            break;
//        case 190:
//            gameView = WizardCauldronGame::createGame(configPath, false);
//            break;
//        case 191:
//            gameView = FrogSentenceGame::createGame(configPath, false);
//            break;
//        case 192:
//            gameView = CatchingThiefGame::createGame(configPath, false);
//            break;
//        case 193:
//            gameView = CarOnHighwayGame::createGame(configPath, false);
//            break;
//        case 194:
//            gameView = SpaceStationGame::createGame(configPath, false);
//            break;
//        case 195:
//            gameView = FireFighter::createGame(configPath, images, false);
//            break;
//        case 196:
//            gameView = Robot::createGame(configPath, images, false);
//            break;
//        case 197:
//            gameView = FishingShip::createGame(configPath, false);
//            break;
//        case 198:
//            gameView = MonsterTeeth::createGame(configPath, images, false);
//            break;
//        case 199:
//            gameView = BalloonTheClown::createGame(configPath, false);
//            break;
//        case 200:
//            gameView = HatchingEggs::createGame(configPath, images, false);
//            break;
//        case 201:
//            gameView = CaterpillaRacing::createGame(configPath, false);
//            break;
//        case 202:
//            gameView = TapMonkey::createGame(configPath, images, false);
//            break;
//        case 203:
//            gameView = WordWheel::createGame(configPath, images, false);
//            break;
//        case 204:
//            gameView = Firefly::createGame(configPath, images, false);
//            break;
//        case 205:
//            gameView = CoverStory::createGame(configPath, images, false);
//            break;
//        case 206:
//            gameView = QuestionWord::createGame(configPath, images, false);
//            break;
//        case 207:
//            gameView = MPTapBigBag::createGame(configPath, images);
//            break;
//        case 208:
//            gameView = MPBubbleBoom::createGame(configPath, images);
//            break;
//        case 209:
//            gameView = CloudCatcher::createGame(configPath, images);
//            break;
//        case 210:
//            gameView = DeliveryFood::createGame(configPath, images);
//            break;
//        case 211:
//            gameView = FoodRestaurant::createGame(configPath, images);
//            break;
//        case 212:
//            gameView = SheepVsWolves::createGame(configPath, false);
//            break;
//        case 213:
//            gameView = SpaceShip::createGame(configPath);
//            break;
//        case 215:
//            gameView = DefeatTheDragon::createGame(configPath);
//            break;
//        case 216:
//            gameView = WaterBalloon::createGame(configPath, images);
//            break;
//        case 217:
//            gameView = FootballKick::createGame(configPath, images);
//            break;
//        case 218:
//            gameView = WhoAreThey::createGame(configPath, images, false);
//            break;
//        case 219:
//            gameView = WhatAreTheyThinking::createGame(configPath, false);
//            break;
//        case 220:
//            gameView = GearsForCar::createGame(configPath, images);
//            break;
//        case 221:
//            gameView = SayWhy::createGame(configPath);
//            break;
//        case 222:
//            gameView = BreakTheEgg::createBreakTheEgg(configPath);
//            break;
//        case 223:
//            gameView = WhatHappenedNext::createGame(configPath, false);
//            break;
//        case 224:
//            gameView = GamePiggy::create();
//            break;
//        case 226:
//            gameView = Ufo::createGame(configPath, false);
//            break;
//        case 246:
//            gameView = ShootingStar::createGame(configPath);
//            break;
//        case 250:
//            gameView = GingerBreadMan::createGame(configPath);
//            break;
//        case 251:
//            gameView = WordTv::createGame(configPath);
//            break;
//        case 252:
//            gameView = SkiingRace::createGame(configPath, false);
//            break;
//        case 253:
//            gameView = MexicanCactus::createGame(configPath);
//            break;
//        case 254:
//            gameView = BuggiesV2::createGame(configPath);
//            break;
//        case 255:
//            gameView = GameBuildBridge::createGame(configPath);
//            break;
//        case 256:
//            gameView = GameNinja::createView();
//            break;
//        case 257:
//            gameView = ClownOnBike::createGame(configPath);
//            break;
//        case 258:
//            gameView = Seapearl::createGame(configPath);
//            break;
//        case 263:
//            gameView = FruitJumpRope::createGame(configPath);
//            break;
//        case 264:
//            gameView = SweetLand::createGame(configPath);
//            break;
//        case 265:
//            gameView = HotAirBalloon::createGame(configPath);
//            break;
//        case 266:
//            gameView = RythmMatching::createGame(configPath);
//            break;
//        case 267:
//            gameView = TracingLetter::createGame(configPath);
//            break;
//        case 268:
//            gameView = WaterPipe::create();
//            break;
//        default:
//            break;
//    }
    
    return gameView;
    
}

void MJMainGBase::onCloseGame(ms::delegate::Activity::EndReason reason)
{
    if (CONFIG_MANAGER->isVietnameseStories()){
        onFinishGameVMonkey();
        return;
    }
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
    this->hideTapGuideGame();
	getGameSTT("finnish_no_end_game");
    //_activitydelegate ? _activitydelegate->onActivityEnd(this,reason) : (void) nullptr;
    //this->removeFromParent();
}
