//
//	MPBubbleBoom.h
//	Created by mintzy on 04/03/18
//

#include "MPBubbleBoom_Item.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"

INITIALIZE_READER(MPBubbleBoom_Item);

#define CSB_NAME "csb/game/MPBubbleBoom/MPBubbleBoomItem.csb"
#define GAF_BOOM  "gaf/mpbubbleboom/mp_bubbleboom_boom/bubbleboom_boom.gaf"
#define GAF_EFFECT_1 "gaf/mpbubbleboom/mp_bubbleboom_finbb_1/bubbleboom_finbb_1.gaf"
#define GAF_EFFECT_2 "gaf/mpbubbleboom/mp_bubbleboom_finbb_2/bubbleboom_finbb_2.gaf"

#define VMONKEY_PUNCTUATION_MASKS_FONT "fonts/vmonkey_punctuation_custom_font.ttf"

MPBubbleBoom_Item * MPBubbleBoom_Item::createItem(mj::PHONIC_INFO phonic_info, bool bubble_star)


{
	auto p = static_cast<MPBubbleBoom_Item *> (cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(phonic_info, bubble_star);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void MPBubbleBoom_Item::setEnableClick(bool clickable)
{
	this->clickable = clickable;
}

void MPBubbleBoom_Item::setDelegate(MPBubbleBoom_ItemDelegate * delegate)
{
	_delegate = delegate;
}



cocos2d::ui::Widget::ccWidgetClickCallback MPBubbleBoom_Item::onLocateClickCallback(const std::string & callback_name)
{
	if (callback_name == "onClickedBubble") {
		return CC_CALLBACK_1(MPBubbleBoom_Item::onClickedBubbleBoom, this);
	}
	return nullptr;
}

void MPBubbleBoom_Item::didLoadFromCSB(mj::PHONIC_INFO phonic_info, bool bubble_star)
{
	_phonic_info = phonic_info;
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->is_phonic_bubble = is_phonic_bubble;
	is_star_bubble = bubble_star;
	root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "layout_root");

	img_bubble = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(root_layout, "img_bubble");
	lbl_text = cocos2d::utils::findChild<cocos2d::ui::Text *>(root_layout, "lbl_text");
	img_toy = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(root_layout, "img_toy");
	lbl_text->setFontName((CONFIG_MANAGER->isVietnameseStories()||CONFIG_MANAGER->isTestActVmonkey())?VMONKEY_PUNCTUATION_MASKS_FONT:font_name);
	lbl_text->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	img_toy->setVisible(false);
	gaf::GAFAsset * asset_effect_1;
	try {
		asset_effect_1 = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_finbb_1");
	}
	catch (const std::out_of_range& oor) {
		PHONIC_MANAGER->cacheGAF("mp_bubbleboom_finbb_1",GAF_EFFECT_1);
		asset_effect_1 = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_finbb_1");
	}
	GAFWrapper * effect1 = GAFWrapper::createGAF(asset_effect_1);
	effect1->setPosition(root_layout->getContentSize() / 2);
	effect1->setVisible(false);
	//effect1->play(true);
	//effect1->setScale(1.5f);
	root_layout->addChild(effect1, 1000000, "effect_1");

	gaf::GAFAsset * asset_effect_2;
	try {
		asset_effect_2 = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_finbb_2");
	}
	catch (const std::out_of_range& oor) {
		PHONIC_MANAGER->cacheGAF("mp_bubbleboom_finbb_2", GAF_EFFECT_2);
		asset_effect_2 = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_finbb_2");
	}
	GAFWrapper * effect2 = GAFWrapper::createGAF(asset_effect_2);
	effect2->setPosition(root_layout->getContentSize() / 2);
	effect2->setVisible(false);
	//effect2->play(true);
	//effect2->setScale(1.5f);
	root_layout->addChild(effect2, 1000002, "effect_2");



	std::string img_type = bubble_star ? "bubblestar" : "bubble";
	int img_index = cocos2d::random(1, 5);
	img_bubble->loadTexture(cocos2d::StringUtils::format("games/mpbubbleboom/mp_bubbleboom_%s%d.png", img_type.c_str(), img_index));
	is_boomed = false;
	is_used = false;


}
void MPBubbleBoom_Item::fitValueTextSize()
{

	auto size = this->getContentSize();
	auto label_size = lbl_text->getContentSize();
	float fSize = lbl_text->getFontSize();
	while (label_size.width > size.width * 0.75 && label_size.height > size.height * 0.75) {
		fSize -= 2;
		lbl_text->setFontSize(fSize);
		label_size = lbl_text->getContentSize();
	}
	while (label_size.width < size.width * 0.75 && label_size.height < size.height * 0.75) {
		fSize += 2;
		lbl_text->setFontSize(fSize);
		label_size = lbl_text->getContentSize();
	}

	lbl_text->setPosition(Vec2(size.width / 2 - 5, size.height / 2));

}
void MPBubbleBoom_Item::onClickedBubbleBoom(cocos2d::Ref * sender)
{
	CCLOG("onClickedBubbleBoom");

	if (_delegate && clickable) {
		root_layout->removeChildByName("guide_gaf");
		_delegate->onClickedBubbleBoomDelegate(this);
	}
}

void MPBubbleBoom_Item::onChangeBubbleBoomState(cocos2d::Ref * sender, int state)
{
	bubble_state = state;
	if (_delegate) {
		_delegate->onChangeBubbleBoomStateDelegate(this, bubble_state);
	}
}

void MPBubbleBoom_Item::onBubbleFinishAction(std::string action_name, bool loop)
{
	root_layout->stopAllActions();
	if (action_name == "idle") {
		auto idle_action = Sequence::create(
			EaseQuadraticActionInOut::create(MoveBy::create(1.5f, Vec2(15, 1.5))),
			EaseQuadraticActionInOut::create(MoveBy::create(1.5f, Vec2(-31, 0))),
			EaseQuadraticActionInOut::create(MoveBy::create(1.5f, Vec2(29, -4.5))),
			EaseQuadraticActionInOut::create(MoveBy::create(1.5f, Vec2(-13, 3))), NULL);
		//auto bubble_disco = Sequence::create(
		//	DelayTime::create(0.25f), CallFunc::create([=] {
		//	int img_index = cocos2d::random(1, 5);
		//	img_bubble->loadTexture(cocos2d::StringUtils::format("games/mpbubbleboom/mp_bubbleboom_bubblestar%d.png", img_index));

		//}), NULL);
		int num_loop = loop ? 1000000 : 1;
		root_layout->runAction(Repeat::create(idle_action, num_loop));
		//	root_layout->runAction(Repeat::create(bubble_disco, num_loop));
	}
	else if (action_name == "action") {
		root_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		root_layout->ignoreAnchorPointForPosition(true);
		auto action_bubble = Spawn::create(ScaleTo::create(0.75f, 0.65f),
			Sequence::create(
				MoveBy::create(0.075, Vec2(15, 3.5)),
				MoveBy::create(0.075, Vec2(-12, -1.0)),
				MoveBy::create(0.075, Vec2(9, -1.5)),
				MoveBy::create(0.075, Vec2(-12.5, -2.3)),
				MoveBy::create(0.075, Vec2(0.5, 1.3)),
				MoveBy::create(0.075, Vec2(-0.5, 3.6)),
				MoveBy::create(0.075, Vec2(3.7, -1.0)),
				MoveBy::create(0.075, Vec2(8.8, -1.6)),
				MoveBy::create(0.075, Vec2(-12.5, -2.3)),
				MoveBy::create(0.075, Vec2(0.5, 1.3)),
				NULL), NULL);
		int num_loop = loop ? 1000000 : 1;
		root_layout->runAction(Repeat::create(action_bubble, num_loop));
	}
}

void MPBubbleBoom_Item::onBubbleBoomed(std::function<void()> callback)
{
	if (is_boomed) return;
	onChangeBubbleBoomState(this, BOOMED);
	auto guide_gaf = root_layout->getChildByName("guide_gaf");
	auto effect_1 = root_layout->getChildByName("effect_1");
	//	auto effect_2 = root_layout->getChildByName("effect_2");

	if (guide_gaf) guide_gaf->removeFromParent();
	if (effect_1) effect_1->removeFromParent();
	//	if (effect_2) effect_2->removeFromParent();

	is_boomed = true;
	is_used = true;
	this->stopAllActions();
    gaf::GAFAsset * asset_boom;
    try {
        asset_boom = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_boom");
    }
    catch (const std::out_of_range& oor) {
        PHONIC_MANAGER->cacheGAF("mp_bubbleboom_boom", GAF_BOOM);
        asset_boom = PHONIC_MANAGER->gaf_cache.at("mp_bubbleboom_boom");
    }
	auto gaf_boom = GAFWrapper::createGAF(asset_boom);
	gaf_boom->setPosition(this->getContentSize() / 2);
	img_bubble->setVisible(false);
	root_layout->addChild(gaf_boom, 1000);
	root_layout->runAction(ScaleTo::create(0.15f, 0.95));
	lbl_text->setScale(0.1f);

	lbl_text->runAction(Sequence::create(ScaleTo::create(0.1f, 1.15f), DelayTime::create(0.6f), CallFunc::create([=] {
		if (callback) {
			callback();
		}
	}), NULL));
	gaf_boom->playSequence("action", false, [=](gaf::GAFObject *obj, const std::string &sqName) {
		gaf_boom->removeFromParent();
	});
}

bool MPBubbleBoom_Item::isPhonicBubble() const
{
	return is_phonic_bubble;
};

void MPBubbleBoom_Item::onAutoClick()
{
	if (_delegate && clickable) {
		_delegate->onClickedBubbleBoomDelegate(this);
	}
}

void MPBubbleBoom_Item::showTapGuideGame(std::string gaf_path)
{
	auto hand = GAFWrapper::createGAF(gaf_path);
	root_layout->addChild(hand, 1900000000, "guide_gaf");
	hand->setPosition(root_layout->getContentSize() / 2);
	hand->play(false, [=](gaf::GAFObject *obj) {
		hand->removeFromParent();
	});
}

void MPBubbleBoom_Item::setBubbleSize(cocos2d::Size bubble_size)
{
	this->setContentSize(bubble_size);
	root_layout->setContentSize(bubble_size);
	img_bubble->setContentSize(bubble_size);
	img_bubble->setPosition(bubble_size / 2);
	img_toy->setContentSize(bubble_size * 0.8);
	img_toy->setPosition(bubble_size / 2);
	fitValueTextSize();
}

bool MPBubbleBoom_Item::isBoomed()
{
	return is_boomed;
}

bool MPBubbleBoom_Item::getUsed()
{
	return is_used;
}

bool MPBubbleBoom_Item::isStarBubble()
{
	return is_star_bubble;
}

void MPBubbleBoom_Item::setUsed(bool is_used)
{
	this->is_used = is_used;
}

void MPBubbleBoom_Item::resetBubble()
{
	this->is_used = false;
	this->is_boomed = false;
	bubble_state = NONE;
	lbl_text->setScale(1.0f);
	clickable = true;
}

void MPBubbleBoom_Item::setBubbleConfig(bool is_phonic_bubble, bool show_text, bool upper_case, bool newType)
{
	this->is_phonic_bubble = is_phonic_bubble;

	if (_phonic_info.text_full.empty()) {
		_phonic_info.text_full = " ";
	}
	if (_phonic_info.text_phonic.empty()) {
		_phonic_info.text_phonic = " ";
	}
	lbl_text->setVisible(false);
	img_toy->setVisible(false);
	if (is_phonic_bubble) {
		std::string text_item = _phonic_info.text_phonic;

		lbl_text->setString(text_item);
		lbl_text->setVisible(true);
	}
	else {
		if (show_text) {
			lbl_text->setString(_phonic_info.text_full);
			lbl_text->setVisible(true);
		}
		else {
			img_toy->loadTexture(_phonic_info.image_path);
			img_toy->setVisible(true);
		}
	}
	//this->setBubbleSize(Size(300, 300));
	fitValueTextSize();

	if (is_star_bubble) {
		img_toy->setVisible(true);
		auto effect1 = root_layout->getChildByName<GAFWrapper *>("effect_1");
		//effect1->setPosition(root_layout->getContentSize() / 2);
		effect1->setVisible(true);
		effect1->setOpacity(0.70);
		effect1->play(true);

		auto * effect2 = root_layout->getChildByName<GAFWrapper *>("effect_2");
		//effect2->setPosition(root_layout->getContentSize() / 2);
		effect2->setVisible(true);
		effect2->setOpacity(0.70);
		effect2->play(true);

		if (newType) {
			lbl_text->setPositionY(lbl_text->getPositionY() + lbl_text->getContentSize().height / 4);
			auto image = ImageView::create(_phonic_info.image_path);
			image->setPosition(Vec2(lbl_text->getPosition().x, lbl_text->getPosition().y - lbl_text->getContentSize().height / 2 - image->getContentSize().height / 4));
			root_layout->addChild(image);
		}
	}

	std::string img_type = is_star_bubble ? "bubblestar" : "bubble";
	int img_index = cocos2d::random(1, 5);
	img_bubble->loadTexture(cocos2d::StringUtils::format("games/mpbubbleboom/mp_bubbleboom_%s%d.png", img_type.c_str(), img_index));
	img_bubble->setVisible(true);
}
