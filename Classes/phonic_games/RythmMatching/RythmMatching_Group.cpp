#include "RythmMatching_Group.h"

#define RYTHM_MATCHING_GROUP_CSB "csb/game/rythmmatching/RythmMatching_Group.csb"


INITIALIZE_READER(RythmMatching_Group);

RythmMatching_Group::RythmMatching_Group()
	: is_paired(false)
{
}

RythmMatching_Group * RythmMatching_Group::createGroup(RythmMatching_Item * item1, RythmMatching_Item * item2)
{
	auto group = static_cast<RythmMatching_Group *>(cocos2d::CSLoader::createNode(RYTHM_MATCHING_GROUP_CSB));
	if (group) {
		group->didLoadFromCSB(item1, item2);
	}
	else {
		CC_SAFE_DELETE(group);
		group = nullptr;
	}
	return group;
}


void RythmMatching_Group::didLoadFromCSB(RythmMatching_Item * item1, RythmMatching_Item * item2)
{

	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_item1 = item1;
	_item2 = item2;
	auto group_size = this->getContentSize();

	_item1->setPosition(Vec2(group_size.width / 2, 0.75 * group_size.height));
	//_item1->setBackgroundItemType(RythmMatching_Item::RMI_BG_YELLOW);
	addChild(_item1);

	_item2->setPosition(Vec2(group_size.width / 2, 0.25 * group_size.height));
	//_item2->setBackgroundItemType(RythmMatching_Item::RMI_BG_DASH);
	addChild(_item2);
}
void RythmMatching_Group::onShow(float delay)
{

	auto start_pos = this->getPosition();
	
	this->setPosition(start_pos + Vec2(0, 500));
	this->runAction(Sequence::create(DelayTime::create(delay), EaseBackOut::create(MoveTo::create(0.5f, start_pos)), CallFunc::create([=] {
	}), NULL));
}

void RythmMatching_Group::onShowDash(float delay)
{

	_item2->setOpacity(0);
	_item2->runAction(Sequence::create(DelayTime::create(delay),
		Repeat::create(Sequence::createWithTwoActions(FadeTo::create(0.2f,255),FadeTo::create(0.2f,100)),2),FadeTo::create(0.1f,255),NULL));
}

void RythmMatching_Group::onHide(float delay)
{

	this->runAction(Sequence::create(DelayTime::create(delay), MoveBy::create(0.25f, Vec2(0, 500)),/*RemoveSelf::create(),*/NULL));
}

void RythmMatching_Group::onPairItem()
{

	is_paired = true;
	_item2->onItemCorrect();

	this->runAction(Sequence::create(
		MoveBy::create(0.15f, Vec2(0, 25)), 
		MoveBy::create(0.15f, Vec2(0, -25)), 
		CallFunc::create([=] {
		_item2->onItemCorrect();
		_item2->runAction(Sequence::create(MoveBy::create(0.15f,Vec2(0,-10)),MoveBy::create(0.25f, Vec2(0, 35)),NULL));
		auto chain = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "chain");
		chain->runAction(Sequence::create(DelayTime::create(0.15f), MoveBy::create(0.25f, Vec2(0, 25)), NULL));
	}) , NULL));
}

void RythmMatching_Group::onDetected(cocos2d::Vec2 point)
{

	auto dash_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(_item2, "dash_layout");
	auto box_gw = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(dash_layout, "box_gw");
	auto box_dash = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(dash_layout, "box_dash");
	bool is_detected = isDetected(point);
	box_gw->setVisible(is_detected);
	box_dash->setVisible(!is_detected);
	float scale = is_detected ? 1.15f : 1.0f;
	box_gw->setScale(scale);
}

bool RythmMatching_Group::isDetected(cocos2d::Vec2 point)
{

	auto pos = this->convertToNodeSpace(point);
	return _item2->getBoundingBox().containsPoint(pos);
}

bool RythmMatching_Group::isPaired()
{
	return is_paired;
}

RythmMatching_Item * RythmMatching_Group::getItem1()
{
	return _item1;
}

RythmMatching_Item * RythmMatching_Group::getItem2()
{
	return _item2;
}

