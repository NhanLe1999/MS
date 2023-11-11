#include "SheepVsWolves_VM_Item.h"
#include "HSAudioEngine.h"

#define GAF_SHEEP_WOLVES "gaf/SheepVsWolves_VM/sheep_sheep/sheep_sheep.gaf"
#define IMG_SHEEP_PLACEMENT "games/SheepVsWolves_VM/sheepplacement.png"


SheepVsWolves_VM_Item * SheepVsWolves_VM_Item::createItem(SheepVsWolvesVM::PHONIC_INFO info, SheepVsWolves_VMType type)
{
	auto item = new SheepVsWolves_VM_Item();
	if (item && item->init()) {
		item->didLoadFromCSB(info, type);
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

SheepVsWolves_VMType SheepVsWolves_VM_Item::getSheepType() const
{
	return m_type;
}

SheepVsWolvesVM::PHONIC_INFO SheepVsWolves_VM_Item::getInfo() const
{
	return m_sheep_info;
}

void SheepVsWolves_VM_Item::setSheepFlipped(bool flipX, bool flipY)
{
	this->setFlippedX(flipX);
	this->setFlippedY(flipY);
	m_lbl_info->setFlippedX(flipX);
	m_lbl_info->setFlippedY(flipY);
	m_img_sheepplacement->setFlippedX(flipX);
	m_img_sheepplacement->setFlippedY(flipY);
}

void SheepVsWolves_VM_Item::playSheepSequence(std::string sequenceName, bool loop, std::function<void(gaf::GAFObject*obj, const std::string&sqName)> call_back)
{
	m_lbl_info->stopAllActions();
	m_lbl_info->setPosition(cocos2d::Vec2(sheep_size.width * 0.425, sheep_size.height * 0.5));
	if (sequenceName == "bark") {
		float bark = cocos2d::rand_0_1();
		if (bark < 1.0f / 3) {
			AUDIO_ENGINE->playSound("sounds/SheepVsWolves_VM/sheep_bark1_1.mp3");
			m_gaf_sheep_wolves->playSequence("w_bark", loop, call_back);
		}
		else if (bark < 2.0f / 3) {
			AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/SheepVsWolves_VM/sheep_bark2_%d.mp3", cocos2d::random(1, 4)));
			m_gaf_sheep_wolves->playSequence("w_barkx1", loop, call_back);
		}
		else {
			AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/SheepVsWolves_VM/sheep_bark3_%d.mp3", cocos2d::random(1, 2)));
			m_gaf_sheep_wolves->playSequence("w_barkx2", loop, call_back);
		}
	}
	else if (sequenceName == "stand_idle") {
		m_gaf_sheep_wolves->playSequence("stand_idle", loop, call_back);
	}
	else if (sequenceName == "stand_sit") {
		m_gaf_sheep_wolves->playSequence("stand_sit", loop, call_back);
	}
	else if (sequenceName == "sit_idle") {
		m_gaf_sheep_wolves->playSequence("sit_idle", loop, call_back);
		m_lbl_info->setPosition(cocos2d::Vec2(sheep_size.width * 0.425, sheep_size.height * 0.35));
	}
	else if (sequenceName == "sit_stand") {
		m_gaf_sheep_wolves->playSequence("sit_stand", loop, call_back);
	}
	else if (sequenceName == "run") {
		m_gaf_sheep_wolves->playSequence("run", loop, call_back);
	}
	else if (sequenceName == "wrun") {
		m_gaf_sheep_wolves->playSequence("wrun", loop, call_back);
	}
	else if (sequenceName == "jump") {
		m_gaf_sheep_wolves->playSequence("jump", loop, call_back);
		int num_loop = loop ? 100000 : 1;
		m_lbl_info->runAction(cocos2d::Repeat::create(
			cocos2d::Sequence::create(cocos2d::MoveBy::create(6.0f / 32.0f, cocos2d::Vec2(4,30)), cocos2d::DelayTime::create(12.0f / 32),cocos2d::MoveBy::create(6.0f / 32.0f, cocos2d::Vec2(-4, -30)),NULL), num_loop));
	}
	else if (sequenceName == "wjump") {
		m_gaf_sheep_wolves->playSequence("wjump", loop, call_back);
		int num_loop = loop ? 100000 : 1;
		m_lbl_info->runAction(cocos2d::Repeat::create(
			cocos2d::Sequence::create(cocos2d::MoveBy::create(6.0f / 32.0f, cocos2d::Vec2(4, 30)), cocos2d::DelayTime::create(12.0f / 32), cocos2d::MoveBy::create(6.0f / 32.0f, cocos2d::Vec2(-4, -30)), NULL), num_loop));
	}
	else if (sequenceName == "kicked") {
		m_gaf_sheep_wolves->playSequence("kicked", loop, call_back);
	}
	else if (sequenceName == "hit") {
		m_gaf_sheep_wolves->playSequence("hit", loop, call_back);
	}
	else if (sequenceName == "correct") {
		m_gaf_sheep_wolves->playSequence("s_correct", loop, call_back);
	}
	else if (sequenceName == "walk") {
		m_gaf_sheep_wolves->playSequence("walk_pre", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			m_gaf_sheep_wolves->playSequence("walk", loop, call_back);
		});
	}
}

void SheepVsWolves_VM_Item::setDelegate(SheepVsWolves_VM_ItemDelegate * delegate)
{
	this->_delegate = delegate;
}

cocos2d::ui::Text * SheepVsWolves_VM_Item::getLableInfo()
{
	return m_lbl_info;
}

void SheepVsWolves_VM_Item::setSheepMissing(bool missing)
{
	m_sheep_info.missing = missing;
	if (missing) {
		m_lbl_info->setString("");
		m_lbl_info->setTextColor(cocos2d::Color4B::WHITE);
		m_gaf_sheep_wolves->setVisible(false);
		m_img_sheepplacement->setVisible(true);
	}
	else {
		m_lbl_info->setString(m_sheep_info.text_phonic);
		m_lbl_info->setTextColor(cocos2d::Color4B::BLACK);
		m_gaf_sheep_wolves->setVisible(true);
		m_img_sheepplacement->setVisible(false);
	}
}

void SheepVsWolves_VM_Item::setEnableClick(bool enable)
{
	m_layout_touch->setTouchEnabled(enable);
}

bool SheepVsWolves_VM_Item::isClickEnabled()
{
	return m_layout_touch->isTouchEnabled();
}

void SheepVsWolves_VM_Item::didLoadFromCSB(SheepVsWolvesVM::PHONIC_INFO info, SheepVsWolves_VMType type)
{
	sheep_size = cocos2d::Size(128, 128);
	m_sheep_info = info;
	m_type = type;
	/*this->setBackGroundColor(cocos2d::Color3B::BLUE);
	this->setBackGroundColorOpacity(255);
	this->setBackGroundColorType(BackGroundColorType::SOLID);*/
	this->setContentSize(sheep_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	//	this->setCascadeOpacityEnabled(true);

	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(sheep_size);
	m_layout_root->setCascadeOpacityEnabled(true);
	/*m_layout_root->setBackGroundColor(cocos2d::Color3B::BLUE);
	m_layout_root->setBackGroundColorOpacity(255);
	m_layout_root->setBackGroundColorType(BackGroundColorType::SOLID);*/
	this->addChild(m_layout_root);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setContentSize(sheep_size);
	m_layout_touch->setCascadeOpacityEnabled(true);
	/*	m_layout_touch->setBackGroundColor(cocos2d::Color3B::BLUE);
		m_layout_touch->setBackGroundColorOpacity(255);
		m_layout_touch->setBackGroundColorType(BackGroundColorType::SOLID)*/;
	this->addChild(m_layout_touch, 1000);

	m_gaf_sheep_wolves = GAFWrapper::createGAF(GAF_SHEEP_WOLVES);
	m_gaf_sheep_wolves->setPosition(cocos2d::Vec2(48, 64));
	m_gaf_sheep_wolves->setCascadeOpacityEnabled(true);
	m_gaf_sheep_wolves->playSequence("stand_idle", true);
	m_layout_root->addChild(m_gaf_sheep_wolves);

	m_img_sheepplacement = cocos2d::ui::ImageView::create(IMG_SHEEP_PLACEMENT);
	m_img_sheepplacement->setPosition(sheep_size / 2);
	m_img_sheepplacement->setFlippedX(true);
	m_layout_root->addChild(m_img_sheepplacement);

	if (info.missing) {
		m_gaf_sheep_wolves->setVisible(false);
	}
	else {
		m_img_sheepplacement->setVisible(false);
	}

	m_lbl_info = cocos2d::ui::Text::create(info.text_phonic, font_name, 45);
	m_lbl_info->setPosition(cocos2d::Vec2(sheep_size.width * 0.425, sheep_size.height * 0.5));
	m_layout_root->addChild(m_lbl_info, 6);
	m_lbl_info->setTextColor(cocos2d::Color4B::BLACK);
	if (info.missing) {
		m_lbl_info->setString("");
		m_lbl_info->setTextColor(cocos2d::Color4B::WHITE);
	}

	// add event
	m_layout_touch->setTouchEnabled(true);
	m_layout_touch->addClickEventListener([=](cocos2d::Ref * sender) {
		CCLOG("onClickSheepVsWolves_VM");
		if (_delegate) {
			_delegate->onClickSheepVsWolves_VMDelegate(this);
		}
	});
}
