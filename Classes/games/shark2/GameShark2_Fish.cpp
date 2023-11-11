
#include "GameShark2_Fish.h"
#include "HSAudioEngine.h"
#include "GameShark2.h"
INITIALIZE_READER(MJGameShark2_Fish);

USING_NS_CC;
#define CSB_NAME "csb/game/shark2/GameShark2_Fish.csb"

MJGameShark2_Fish * MJGameShark2_Fish::createFish(std::string text) {
    auto p = static_cast<MJGameShark2_Fish *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(text);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark2_Fish::didLoadFromCSB(std::string text) {
   
    didLoadFromCSB(false);
}

MJGameShark2_Fish * MJGameShark2_Fish::createShark() {
    auto p = static_cast<MJGameShark2_Fish *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark2_Fish::didLoadFromCSB(bool is_shark) {
    this->setContentSize(cocos2d::Size(205, 180));
    ui::Helper::doLayout(this);
    auto layout_touch = this->getChildByName<Layout *>("layout_touch");
    isShark = is_shark;
    if (isShark) {
        gaf_fish = GAFWrapper::createGAF("gaf/shark2/shark/shark.gaf");
        layout_touch->setTouchEnabled(false);
        
    }else {
        fish_id = random(0, 3);
		switch (fish_id)
		{
		case 0:{
				   gaf_fish = GAFWrapper::createGAF("gaf/shark2/Blue_fish/Blue_fish.gaf");
				   break;
		}
		case 1:{
				   gaf_fish = GAFWrapper::createGAF("gaf/shark2/green_fish/green_fish.gaf");
				   break;
		}
		case 2:{
				   gaf_fish = GAFWrapper::createGAF("gaf/shark2/orange_fish/orange_fish.gaf");
				   break;
		}
		case 3:{
				   gaf_fish = GAFWrapper::createGAF("gaf/shark2/pink_fish/pink_fish.gaf");
				   break;
		}
		default:
			break;
		}
        std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/monkeystories_ultra.ttf";
		text = ui::Text::create(text_in_fish, font_name, 36);
        this->addChild(text, 1000000);
        text->setTextColor(Color4B::BLACK);
		text->setPosition(Vec2(25, 0));
        
    }
    this->addChild(gaf_fish);
    if (isShark) {
        gaf_fish->setPositionY(30);
        gaf_fish->playSequence("open", false);
        
    }else gaf_fish->play(true);
    
}

void MJGameShark2_Fish::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameShark2_Fish::run(){
	taped = false;
	in_anim = true;
    auto screensize = Director::getInstance()->getWinSize();
    auto speed = random(100.f, 200.f);
	this->stopAllActions();
	this->setVisible(true);
	this->setPosition(listPos_.at(0));
	
    if (this->getPositionX() < screensize.width / 2) {
        flipx = false;
    } else {
        flipx = true;
		text->setPosition(Vec2(-25, 0));
    }
    
    gaf_fish->setFlippedX(flipx);
    auto distance = listPos_.at(0).distance(listPos_.at(1));
    auto time = distance / speed + 1;
	auto _pos = text->getPosition();
	if (_pos.x > 0) {
		this->runAction(Sequence::create(
			EaseIn::create(MoveTo::create(time, listPos_.at(1)), 1)
			, DelayTime::create(0.1f)
			, EaseOut::create(MoveBy::create(time, Vec2(600,600)), 1)
			, CallFunc::create([=]() {
				delegate->resetText(this);
				in_anim = false;
				freeTime = 0;
				}), nullptr));
	}
	else if(_pos.x < 0)
	{
		this->runAction(Sequence::create(
			EaseIn::create(MoveTo::create(time, listPos_.at(1)), 1)
			, DelayTime::create(0.1f)
			, EaseOut::create(MoveBy::create(time, screensize / -2), 1)
			, CallFunc::create([=]() {
				delegate->resetText(this);
				in_anim = false;
				freeTime = 0;
				}), nullptr));
	}
}

void MJGameShark2_Fish::sharkRunToGoal(Ref* target, bool fish_flip){
	Vec2 pos_goal = static_cast<MJGameShark2_Fish*>(target)->getPosition();
	in_anim = true;
    auto screensize = Director::getInstance()->getWinSize();
    auto speed = 3000.f;
    Vec2 destination;
	if (pos_goal.x > screensize.width / 2) {
		this->setPositionX(-600);
        flipx = false;
		destination = Vec2(screensize.width + 600, screensize.height / 2);
        if (fish_flip == flipx) {
            gaf_fish->setPositionX(-100);
        }else 
			gaf_fish->setPositionX(-50);
        
    } else {
		this->setPositionX(screensize.width + 600);
        flipx = true;
		destination = Vec2(-600, screensize.height / 2);
        if (fish_flip == flipx) {
            gaf_fish->setPositionX(100);
        }else 
			gaf_fish->setPositionX(50);
    }
    gaf_fish->setFlippedX(flipx);
    auto distance = pos_goal.distance(this->getPosition());
    time_shark = distance / speed;
    
    pos_tap_to = pos_goal;
    
    this->runAction(Sequence::create(EaseIn::create(MoveTo::create(time_shark, pos_goal),1.5f), CallFunc::create([=]() {
        gaf_fish->playSequence("bite", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            gaf_fish->playSequence("swim", true);
            auto distance = destination.distance(this->getPosition());
            time_shark = distance / speed;
			this->runAction(Sequence::createWithTwoActions(EaseOut::create(MoveTo::create(time_shark, destination), 1.5f), CallFunc::create([=](){in_anim = false; })));
            
        });
        AUDIO_ENGINE->playEffect("sounds/shark2/shark_eat.mp3");
        if (delegate) {
			delegate->onSharkRun(target);
        }
        
    }), nullptr));
	if (gaf_fish) {
		gaf_fish->playSequence("open", false);
	}
}

void MJGameShark2_Fish::onTapFish(cocos2d::Ref *sender){
	MJGameShark2::getInstance()->stopInActiveMode();
	MJGameShark2::getInstance()->inActiveMode();
	MJGameShark2::getInstance()->stopRadio();
	MJGameShark2::getInstance()->_speaker->setTouchEnabled(true);
	if (!taped){
		taped = true;
		if (delegate) {
			delegate->onFish(this);
		}
	}
}

void MJGameShark2_Fish::fishRunWhenTap(Vec2 fish_tap_to, float duration){
//    this->runAction(MoveTo::create(duration+0.5f, fish_tap_to));
}

void MJGameShark2_Fish::runInStart()
{
	taped = true;
	auto screensize = Director::getInstance()->getWinSize();
	this->setPosition(Vec2(-200,screensize.height/2));
	this->runAction(MoveTo::create(8.0f, Vec2(screensize.width + 100, screensize.height/2)));
}

void MJGameShark2_Fish::runOutStart()
{
	auto screensize = Director::getInstance()->getWinSize();
	//this->runAction(MoveBy::create(4.25f, Vec2(screensize.width, screensize.height/2)));
}

void MJGameShark2_Fish::setPoss(std::vector<Vec2> listPoss)
{
	listPos_ = listPoss;
}

void MJGameShark2_Fish::resetText(std::string newText)
{
	text_in_fish = newText;
	text->setString(newText);
}

void MJGameShark2_Fish::resetGaf()
{
	int old_id = fish_id;
	do 
	{
		fish_id = random(0, 3);
	} while (old_id == fish_id);

	gaf_fish->removeFromParent();
	switch (fish_id)
	{
	case 0:{
			   gaf_fish = GAFWrapper::createGAF("gaf/shark2/Blue_fish/Blue_fish.gaf");
			   break;
	}
	case 1:{
			   gaf_fish = GAFWrapper::createGAF("gaf/shark2/green_fish/green_fish.gaf");
			   break;
	}
	case 2:{
			   gaf_fish = GAFWrapper::createGAF("gaf/shark2/orange_fish/orange_fish.gaf");
			   break;
	}
	case 3:{
			   gaf_fish = GAFWrapper::createGAF("gaf/shark2/pink_fish/pink_fish.gaf");
			   break;
	}
	default:
		break;
	}
	this->addChild(gaf_fish);
	gaf_fish->play(true);
}

void MJGameShark2_Fish::returnBase()
{
	this->stopAllActions();
	in_anim = true;
	this->runAction(Sequence::create(DelayTime::create(0.2f)
		, EaseOut::create(MoveTo::create(0.75f, listPos_.at(0)), 2.0f)
		, CallFunc::create([=]() {
		//delegate->resetText(this);
		//freeTime = 0;
		in_anim = false;
	}), nullptr));
}

void MJGameShark2_Fish::setDelegate(MJGameShark2_FishDelegate *_delegate){
    this->delegate = _delegate;
}

ui::Widget::ccWidgetClickCallback MJGameShark2_Fish::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onTapFish") {
        return CC_CALLBACK_1(MJGameShark2_Fish::onTapFish, this);
    }
    return nullptr;
}
