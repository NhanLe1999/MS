
#include "GameXepChu_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameXepChu_Item);

#define CSB_NAME "csb/game/xep_chu/GameXepChu_Item.csb"

GameXepChu_Item * GameXepChu_Item::createItem(ItemInfo item_info, Color4B aColor) {
    auto p = static_cast<GameXepChu_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info, aColor);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameXepChu_Item::didLoadFromCSB(ItemInfo item_info, Color4B aColor) {
	this->setTouchEnabled(true);
	_item_info = item_info;
	curColor = aColor;
	timeRandJump = rand() % 3 + 5;
	rotate_info = rand() % 60 - 30;
	text = cocos2d::utils::findChild<cocos2d::ui::Text*>(this, "text");
	text->setString(_item_info.text);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(cocos2d::Size(text->getContentSize().width, text->getContentSize().height*0.6f));
	text->setPosition(this->getContentSize() / 2);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	if (_item_info.img_path!=""){
		ImageView* img = ImageView::create(_item_info.img_path);
		this->addChild(img,1,"img");
		img->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		img->setPosition(text->getPosition() + Vec2(0, -20));
		img->setVisible(false);
	}


	auto touchLayout = text->getChildByName("touchPanel");
	touchLayout->setContentSize(cocos2d::Size(285, 122));
	touchLayout->setPosition(text->getContentSize() / 2);

	auto gach_chan = cocos2d::utils::findChild<Layout*>(this, "gach_chan");
	gach_chan->setContentSize(cocos2d::Size(text->getContentSize().width, 2));
	//gach_chan->setPosition(text->getPosition() + Vec2(0, -text->getContentSize().height*0.4f));
	gach_chan->setPosition(Vec2(text->getContentSize().width / 2, 15));
	gach_chan->setColor(Color3B(curColor));
	gach_chan->setVisible(false);
}


void GameXepChu_Item::update(float delta)
{
	int numAc = (int)this->getActionManager()->getNumberOfRunningActions();
	if (item_state == ITEM_WORD_STATE::IWS_IDLE){
		timeToJump += delta;
	}

	if (timeToJump > timeRandJump){
		timeToJump = 0;
		this->runAction(Sequence::create(
			ScaleTo::create(0.2f, 0.8f, 0.7f)
			, ScaleTo::create(0.2f,0.6f, 0.6f)
			, ScaleTo::create(0.2f, 0.7f, 0.8f)
			, ScaleTo::create(0.2f, 0.6f, 0.6f)
			, nullptr));
	}
}

void GameXepChu_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void GameXepChu_Item::setTextBlank()
{
	auto textBlank = static_cast<Text*>(text->clone());
	this->addChild(textBlank,1,"textBlank");
	textBlank->setTextColor(Color4B::WHITE);
	textBlank->enableOutline(Color4B::BLACK, 2);
}

void GameXepChu_Item::setFlyType()
{
	auto touchLayout = text->getChildByName("touchPanel");
	touchLayout->setContentSize(cocos2d::Size(text->getContentSize().width < 400 ? 400 : text->getContentSize().width, text->getContentSize().height * 2));
}

void GameXepChu_Item::onHold()
{
	item_state = ITEM_WORD_STATE::IWS_HOLDING;
	auto acScale = Sequence::create(ScaleTo::create(0.2f, 1.2f, 1.1f), ScaleTo::create(0.2f, 0.9f, 0.95f),nullptr);

	this->stopAllActions();
	this->runAction(RotateTo::create(0.1f, 0));
	this->runAction(Repeat::create(acScale,1000));
}

void GameXepChu_Item::onShowImage()
{
	if (auto textBlank = static_cast<Text*>(this->getChildByName("textBlank")))
	{
		textBlank->removeFromParent();
	}
	
	AUDIO_ENGINE->playEffect("sounds/xep_chu/correct.mp3");
	//AUDIO_ENGINE->playEffect(_item_info.audio_path);
	if (_item_info.img_path != ""){
		auto img = this->getChildByName<ImageView*>("img");

		float fw = 150 / img->getContentSize().width;
		float fh = 150 / img->getContentSize().height;

		img->setScale(0.0f);
		img->setVisible(true);
		img->runAction(ScaleTo::create(0.5f, fw < fh ? fw : fh));

		text->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.6f), MoveBy::create(0.5f, Vec2(0, -40))), CallFunc::create([this](){
			text->setTextColor(Color4B::BLACK);
			AUDIO_ENGINE->playSound(_item_info.audio_path);
		}),nullptr));
	}
	else{
		text->runAction(Sequence::create(
			CallFunc::create([=](){
		
		}), ScaleTo::create(0.25f, 1.1f), ScaleTo::create(0.25f, 1.0f)
			, CallFunc::create([=](){
			text->setTextColor(Color4B::BLACK);
		}), nullptr));
	}
	text->setTextColor(curColor);
	text->runAction(Sequence::create(
		Spawn::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create([this] {
			if (_item_info.audio_path != "")
			{
				AUDIO_ENGINE->playSound(_item_info.audio_path);
			}
			})),
		 CallFunc::create([=](){
		text->setTextColor(Color4B::BLACK);
		item_state = ITEM_WORD_STATE::IWS_TRUE_PLACE;
		if (aCallbackFunc){
			aCallbackFunc();
		}
	}), nullptr));
}

void GameXepChu_Item::onFinishAudio(bool isFinish)
{
	auto gach_chan = cocos2d::utils::findChild<Layout*>(this, "gach_chan");
	if (isFinish){
		gach_chan->setVisible(true);
		text->setTextColor(curColor);
	}
	else{
		gach_chan->setVisible(false);
		text->setTextColor(Color4B::BLACK);
	}
}

void GameXepChu_Item::setCallback(const std::function<void()> afunc)
{
	aCallbackFunc = afunc;
}

void GameXepChu_Item::setTextColor(Color4B color)
{
	text->setTextColor(color);

	text->setColor(Color3B(color));
}
void GameXepChu_Item::runActionText(cocos2d::Vec2 post)
{
	this->runAction(Sequence::create(
		Spawn::createWithTwoActions(MoveTo::create(0.2f, this->convertToNodeSpace(post)), ScaleTo::create(0.2f, 2.0f)), 
		NULL));
}

std::string GameXepChu_Item::getTextItem()
{
	return text->getString();
}
