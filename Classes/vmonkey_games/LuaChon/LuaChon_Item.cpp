#include "LuaChon_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(LuaChon_Item);

#define CSB_NAME "csb/game/LuaChon/LuaChon_Item.csb"
//#define img_path "games/LuaChon/"

LuaChon_Item * LuaChon_Item::createItem(std::string text, std::string audio,bool is_true) {

	auto p = static_cast<LuaChon_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(text, audio,is_true);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void LuaChon_Item::didLoadFromCSB(std::string text, std::string audio, bool is_true)
{
	_audio = audio;
	this->is_true = is_true;
	item_layout = utils::findChild<Layout*>(this, "it_layout");
	auto image_answer = utils::findChild<ImageView*>(this, "answer");
	image_answer->loadTexture(text);
//    item_layout->setTouchEnabled(true);
	item_layout->addClickEventListener([=](cocos2d::Ref * sender) {
		if (_delegate) {
			_delegate->onClickedBox(this);
		}
	});
	image_answer->setVisible(false);
	
	//
	auto item_size = this->getContentSize();
	cocos2d::Sprite * cutzone = cocos2d::Sprite::create("games/LuaChon/Layer-20-copy.png");
	cocos2d::Sprite * model = cocos2d::Sprite::create(text);

	cutzone->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	model->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	cutzone->setPosition(item_size / 2);
	model->setPosition(item_size / 2);
	auto size = model->getContentSize();
	model->setScale(std::min(item_size.height/size.height, item_size.width / size.width));
	cocos2d::RenderTexture * render = cocos2d::RenderTexture::create(item_size.width, item_size.height);
	render->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	render->beginWithClear(1, 1, 1, 1);
	model->visit();
	cutzone->setBlendFunc(cocos2d::BlendFunc{ GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA });
	cutzone->visit();
	render->end();
	render->getSprite()->setPosition(cocos2d::ccp(item_size.width / 2, item_size.height / 2));
	render->getSprite()->setScale(0.95);
	this->addChild(render);
	
	/*image_test->setCapInsets(Rect(100, 50, image_test->getContentSize().width - 200, image_test->getContentSize().height - 100));
	image_test->ignoreContentAdaptWithSize(false);
	image_test->setScale9Enabled(true);
	image_test->setContentSize(this->getContentSize()*1.1);
	image_test->setPosition(this->getContentSize() / 2);
	this->addChild(image_test);*/

}

void LuaChon_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();

}

void LuaChon_Item::showItem() {
	this->setScale(0);
	this->runAction(EaseIn::create(ScaleTo::create(0.5, 1.f), 1.5));
	AUDIO_ENGINE->playEffect("sounds/LuaChon/turn.mp3");
}

void LuaChon_Item::setTouchEnable(bool enable) {
	item_layout->setTouchEnabled(enable);
}

void LuaChon_Item::definePosRot() {
    _position = this->getPosition();
    _rotate = this->getRotation();
}
void LuaChon_Item::resetPosRot() {
    this->setPosition(_position);
    this->setRotation(_rotate);
}
void LuaChon_Item::setDelegate(LuaChon_ItemDelegate *delegate) {
	_delegate = delegate;
}

void LuaChon_Item::setAudio(std::string audio) {
	_audio = audio;
}


void LuaChon_Item::playAudio() {
	AUDIO_ENGINE->playEffect(_audio);
}

std::string LuaChon_Item::getAudio() {
	return _audio;
}
