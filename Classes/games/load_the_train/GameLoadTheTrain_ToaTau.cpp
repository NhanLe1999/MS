
#include "GameLoadTheTrain_ToaTau.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"

INITIALIZE_READER(GameLoadTheTrain_ToaTau);

#define CSB_NAME "csb/game/load_the_train/GameLoadTheTrain_ToaTau.csb"



GameLoadTheTrain_ToaTau * GameLoadTheTrain_ToaTau::createItem(ItemInfo item_info) {
	auto p = static_cast<GameLoadTheTrain_ToaTau *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameLoadTheTrain_ToaTau::didLoadFromCSB(ItemInfo item_info) {
	this->setTouchEnabled(true);
	_item_info = item_info;
	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//this->setContentSize(TOA_TAU_CONTENSIZE);

	int randNum = rand() % 4 + 1;
	std::string name = StringUtils::format("gaf/load_the_train/toa_%d/toa_%d.gaf", randNum, randNum);

	toa = GAFWrapper::createGAF(name);
	this->addChild(toa);
	toa->setPosition(Vec2(TOA_TAU_CONTENSIZE.width/2, 20));
	toa->playSequence("run", true);
	
}

void GameLoadTheTrain_ToaTau::loadString(std::string s)
{
	_item_info.text = s;
    
	if(auto tex = cocos2d::utils::findChild<Text*>(this, "text"))
    {
        if(CONFIG_MANAGER->isGameVMonkey())
        {
            if( auto label = ((Label*)tex->getVirtualRenderer()))
            {
                label->enableBold();
            }
            
            tex->setFontName(PHONIC_MANAGER->getGameFontName());
            auto str = StringUtils::StringUTF8(s);
            tex->setFontSize(str.length() >= 5?40:50);
        }
        
        tex->setString(s);
    }
    
	showToa(false);
}

void GameLoadTheTrain_ToaTau::loadColor(int color)
{
	auto img = cocos2d::utils::findChild<ImageView*>(this, "img");
	std::string name = StringUtils::format("games/load_the_train/%d.png", color);
	img->loadTexture(name);
}

void GameLoadTheTrain_ToaTau::showToa(bool isShow)
{
	auto img = cocos2d::utils::findChild<ImageView*>(this, "img");
	img->setVisible(isShow);
	img->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	img->setPosition(img->getPosition() + Vec2(0, -img->getContentSize().height / 2));
	img->runAction(Sequence::create(
		ScaleTo::create(0.1f, 1.0f, 1.1f)
		, ScaleTo::create(0.1f, 1.0f, 0.97f)
		, ScaleTo::create(0.1f, 1.0f, 1.05f)
		, ScaleTo::create(0.1f, 1.0f, 1.0f)
		, CallFunc::create([=](){
		img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		img->setPosition(img->getPosition() + Vec2(0, +img->getContentSize().height / 2));
	}), nullptr));
}

Vec2 GameLoadTheTrain_ToaTau::getToaPos()
{
	auto img = cocos2d::utils::findChild<ImageView*>(this, "img");
	return img->getPosition();
}

void GameLoadTheTrain_ToaTau::onEnter(){
    cocos2d::ui::Layout::onEnter();
}
