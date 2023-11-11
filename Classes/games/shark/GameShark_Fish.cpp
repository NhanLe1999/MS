
#include "GameShark_Fish.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameShark_Fish);

USING_NS_CC;
#define CSB_NAME "csb/game/shark/GameShark_Fish.csb"

MJGameShark_Fish * MJGameShark_Fish::createFish(FishInfo item_info) {
    auto p = static_cast<MJGameShark_Fish *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark_Fish::didLoadFromCSB(FishInfo item_info) {
    _item_info = item_info;
    didLoadFromCSB(false);
}

MJGameShark_Fish * MJGameShark_Fish::createShark() {
    auto p = static_cast<MJGameShark_Fish *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark_Fish::didLoadFromCSB(bool is_shark) {
    this->setContentSize(cocos2d::Size(217, 132));
    ui::Helper::doLayout(this);
    auto layout_touch = this->getChildByName<Layout *>("layout_touch");
    isShark = is_shark;
    if (isShark) {
        gaf_fish = GAFWrapper::createGAF("gaf/shark/shark/shark.gaf");
        layout_touch->setTouchEnabled(false);
        
//        auto layout = Layout::create();
//        layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//        layout->setBackGroundColor(Color3B::RED);
//        this->addChild(layout, 10000);
//        layout->setAnchorPoint(Vec2(0.5, 0.5));
//        layout->setPosition(Vec2(0, 0));
//        layout->setContentSize(cocos2d::Size(30, 30));
        
    }else {
        fish_id = random(1, 4);
        gaf_fish = GAFWrapper::createGAF(StringUtils::format("gaf/shark/fish_%d/fish_%d.gaf", fish_id, fish_id));
//        layout_touch->setContentSize(this->getContentSize());
        
        text = ui::Text::create(_item_info.text, "fonts/monkeystories_ultra.ttf", 30);
        this->addChild(text, 1000000);
        text->setTextColor(Color4B::BLACK);
        
        
    }
    this->addChild(gaf_fish);
    if (isShark) {
        gaf_fish->setPositionY(30);
        gaf_fish->playSequence("1", true);
        
        
        
    }else gaf_fish->play(true);
    
}

void MJGameShark_Fish::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameShark_Fish::run(){
    auto screensize = Director::getInstance()->getWinSize();
    auto speed = random(100.f, 200.f);
    Point destination;
    auto ranY = random(100, 700);
//    CCLOG("this->getPositionX() = %f", this->getPositionY());
    if (this->getPositionX() < screensize.width / 2) {
        destination = Point(screensize.width, ranY);
        flipx = false;
        if (fish_id == 2) {
            text->setPosition(Vec2(15, 10));
        }else if (fish_id == 3) {
            text->setPosition(Vec2(40, -3));
        }else if (fish_id == 4) {
            text->setPosition(Vec2(-8, 0));
        }
    } else {
        destination = Point(0, ranY);
        flipx = true;
        if (fish_id == 2) {
            text->setPosition(Vec2(-15, 10));
        }else if (fish_id == 3) {
            text->setPosition(Vec2(-40, -3));
        }else if (fish_id == 4) {
            text->setPosition(Vec2(8, 0));
        }

    }
    
    gaf_fish->setFlippedX(flipx);
    auto distance = destination.distance(this->getPosition());
    auto time = distance / speed;
    this->runAction(Sequence::create(MoveTo::create(time, destination), CallFunc::create([=]() {
        this->run();
    }), nullptr));
}

void MJGameShark_Fish::sharkRunToGoal(Vec2 pos_goal, bool fish_flip){
    auto screensize = Director::getInstance()->getWinSize();
    auto speed = 3000.f;
    Vec2 destination;
    if (this->getPositionX() < 0) {
        flipx = true;
        destination = Vec2(screensize.width+500, screensize.height/2);
        if (fish_flip == flipx) {
            gaf_fish->setPositionX(-100);
        }else gaf_fish->setPositionX(-50);
        
        
    } else {
        flipx = false;
        destination = Vec2(-500, screensize.height/2);
        if (fish_flip == flipx) {
            gaf_fish->setPositionX(100);
        }else gaf_fish->setPositionX(50);
        

    }
    gaf_fish->setFlippedX(flipx);
    auto distance = pos_goal.distance(this->getPosition());
    time_shark = distance / speed;
    
    pos_tap_to = pos_goal;
//    if (fish_flip) {
//        pos_tap_to.x -= 30;
//        if (pos_tap_to.x > screensize.width) {
//            pos_tap_to.x = screensize.width;
//        }
//    }else {
//        pos_tap_to.x += 30;
//        if (pos_tap_to.x < 0){
//            pos_tap_to.x = 0;
//        }
//    }
    
    this->runAction(Sequence::create(MoveTo::create(time_shark, pos_goal), CallFunc::create([=]() {
        gaf_fish->playSequence("3", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            gaf_fish->playSequence("1", true);
            auto distance = destination.distance(this->getPosition());
            time_shark = distance / speed;
            this->runAction(MoveTo::create(time_shark, destination));
            
        });
        AUDIO_ENGINE->playEffect("sounds/shark/shark_eat.mp3");
        if (delegate) {
            delegate->onSharkRun(this);
        }
        
    }), nullptr));
    
    gaf_fish->playSequence("4", true);
}

void MJGameShark_Fish::onTapFish(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onFish(this);
    }
}

void MJGameShark_Fish::fishRunWhenTap(Vec2 fish_tap_to, float duration){
//    this->runAction(MoveTo::create(duration+0.5f, fish_tap_to));
}

void MJGameShark_Fish::setDelegate(MJGameShark_FishDelegate *_delegate){
    this->delegate = _delegate;
}

ui::Widget::ccWidgetClickCallback MJGameShark_Fish::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onTapFish") {
        return CC_CALLBACK_1(MJGameShark_Fish::onTapFish, this);
    }
    return nullptr;
}
