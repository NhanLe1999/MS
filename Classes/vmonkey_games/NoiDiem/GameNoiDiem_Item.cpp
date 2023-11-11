#include "GameNoiDiem_Item.h"

GameNoiDiem_Item * GameNoiDiem_Item::createItem(int id, math::common::TouchPermission touch, math::resource::Image * image, std::string gaf_file_path, bool color)
{
	auto item = new GameNoiDiem_Item();
	if (item) {
		item->autorelease();
		item->didLoadFromCSB(id, touch, image, gaf_file_path, color);
	}
	else {
		CC_SAFE_DELETE(item);
		item = nullptr;
	}
	return item;
}

GameNoiDiem_Item::GameNoiDiem_Item()
	: _image(nullptr)
	, _background(nullptr)
	, super_node(nullptr)
	, _lbl_value(nullptr)
	, _image_touch(nullptr)
	, _gaf(nullptr)
	, _coloring(false)
	, fadein_sequence("")
	, idle_sequence("")
	, correct_sequence("")
{
}

void GameNoiDiem_Item::didLoadFromCSB(int id, math::common::TouchPermission touch, math::resource::Image * image, std::string gaf_file_path, bool color)
{
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setCascadeOpacityEnabled(true);
	_id = id;
	_touch = touch;
	_coloring = color;
	_locked = false;

	//setBackground(background);
	setGAF(gaf_file_path);
	setImage(image);
	initLabel(" ");
}

void GameNoiDiem_Item::setItemSize(cocos2d::Size size)
{
	this->setContentSize(size);

	float scalar = 0.8f;
	if (_image) {
		_image->setContentSize(size);
		_image->setPosition(size / 2);
	}

	if (_background) {
		_background->setContentSize(size);
		_background->setPosition(size / 2);
	}

	if (_lbl_value) {
		_lbl_value->setFontSize(size.height*.5);
		_lbl_value->setPosition(size / 2);
	}

}

void GameNoiDiem_Item::setImageColor(cocos2d::Color3B color)
{
	if (_image) {
		_image->setColor(color);
	}
}

void GameNoiDiem_Item::setImageVisible(bool visible)
{
	if (_image) {
		_image->setVisible(visible);
	}
}

void GameNoiDiem_Item::setBackgroundVisible(bool visible)
{
	if (_background) {
		_background->setVisible(visible);
	}
}

void GameNoiDiem_Item::setBackgroundColor(cocos2d::Color3B color)
{
	this->setBackGroundColor(color);
	this->setBackGroundColorType(BackGroundColorType::SOLID);
	this->setBackGroundColorOpacity(255);
}

void GameNoiDiem_Item::setGAFVisible(bool visible)
{
	if (_gaf) {
		_gaf->setVisible(visible);
	}

}

void GameNoiDiem_Item::setSuperNode(cocos2d::Node * node)
{
	super_node = node;
}

void GameNoiDiem_Item::onShow(float dt, math::animation::ObjectShowType show, int delta)
{
	this->stopAllActions();
	captureState();
	if (getParent() != nullptr) {
		if (super_node == nullptr) {
			super_node = getParent();
		}
	}
	if (show == math::animation::ScaleUp || show == math::animation::ScaleUpAndRotation || delta == 0) {
		this->setOpacity(0);
		delta = 100;
	}
	else {
		this->setOpacity(255);
	}

	switch (show) {
	case math::animation::No_Effect:
	{
		this->setOpacity(255);
		//this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::FadeIn::create(0.25)));
	}
	break;
	case math::animation::ScaleUp:
	{
		this->setScale(0);
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
	}
	break;
	case math::animation::SlideUp:
	{
		this->setPosition(cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y - delta));
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
	}
	break;
	case math::animation::SlideDown:
	{
		this->setPosition(cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y + delta));
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
	}
	break;
	case math::animation::SlideLeft:
	{
		this->setPosition(cocos2d::Vec2(_captured_node.position.x + delta, _captured_node.position.y));
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
	}
	break;
	case math::animation::SlideRight:
	{
		this->setPosition(cocos2d::Vec2(_captured_node.position.x - delta, _captured_node.position.y));
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
	}
	break;
	default:
	{
		this->setRotation(180);
		this->setScale(0);
		this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::create(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)), cocos2d::RotateTo::create(0.4, 0), NULL)));
	}
	break;
	}

}

void GameNoiDiem_Item::onShowGAF(float dt)
{
	if (_gaf) {
		this->scheduleOnce([=](float delay) {
			_gaf->setVisible(true);
			_gaf->playSequence(fadein_sequence, false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				_gaf->playSequence(idle_sequence, true);
			});
			_coloring = false;
		}, dt, "show gaf");
	}

}

void GameNoiDiem_Item::onCorrectGAF(float dt)
{

	if (_gaf) {
		this->scheduleOnce([=](float delay) {
			if (correct_sequence.empty()) {
				_gaf->setVisible(false);
			}
			else {
				_gaf->setVisible(true);
				_gaf->playSequence(correct_sequence, false, [=](gaf::GAFObject * obj, const std::string &sqN) {
					_gaf->setVisible(false);
				});
			}
		}, dt, "show gaf");
	}
}


void GameNoiDiem_Item::onHide(float delay_time)
{
	this->runAction(cocos2d::Sequence::create(
		cocos2d::DelayTime::create(delay_time), 
	    cocos2d::FadeOut::create(0.25), 
		cocos2d::CallFunc::create([=]() {
		this->removeFromParentAndCleanup(true);
	}), NULL));
}

void GameNoiDiem_Item::setColoring(bool coloring)
{
	_coloring = coloring;
}

bool GameNoiDiem_Item::isColoring()
{
	return _coloring;
}

bool GameNoiDiem_Item::isDetected(cocos2d::Vec2 position)
{
	cocos2d::Size imageSize;
	cocos2d::Size content_size = this->getContentSize();
	imageSize = _image->getContentSize();

	cocos2d::Vec2 fix_position = cocos2d::Vec2(this->getContentSize().width - imageSize.width, this->getContentSize().height - imageSize.height) / 2;
	auto point_converted = cocos2d::Vec2(position.x - fix_position.x, position.y - fix_position.y);

	switch (_detected_type) {
	case math::common::DetectedType::box:
	{
		CCLOG("Tap In Bounding Box");
		//return object_touch_rect.containsPoint(point);
	}
	break;
	case math::common::DetectedType::circle:
	{
		CCLOG("Tap In Circle Or Elipse");
		point_converted = cocos2d::Vec2(point_converted.x, point_converted.y);
		//return Shape2DHitUtility::tapsOnElipse2D(circle_info.first, circle_info.second*elipse_info.first, circle_info.second*elipse_info.second, pointInObject);
	}
	break;
	case math::common::DetectedType::nontrans:
	{
		CCLOG("Tap On Non Transparent");
		//auto convert_touch=cocos2d::Vec2(_image_touch->getWidth()/imageSize.width, _image_touch->getHeight()/imageSize.height);
		//point_converted=cocos2d::Vec2(point_converted.x*convert_touch.x, point_converted.y*convert_touch.y);
		auto convert_touch = cocos2d::Vec2(_image_touch->getWidth() / imageSize.width, _image_touch->getHeight() / imageSize.height);
		point_converted = cocos2d::Vec2(point_converted.x*convert_touch.x, point_converted.y*convert_touch.y);
		return Shape2DHitUtility::tapsOnNonTransparent(point_converted, _image_touch);

		//return Shape2DHitUtility::touchOnImage(_img_src->path, _image->getContentSize(), point_converted);
	}
	break;
	default:
	{
		CCLOG("Tap In Polygon");
		//return Shape2DHitUtility::tapsOnPolygon2D(point_converted, vert);
	}
	break;
	}
	return true;
}

int GameNoiDiem_Item::getId()
{
	return _id;
}

void GameNoiDiem_Item::onTouchItem(bool correct, float delay, int opacity)
{
	scheduleOnce([=](float) {
		if (correct) {
			audio_helper->play("audio_correct.mp3");

			float current_scale = this->getScale();
			this->runAction(cocos2d::Sequence::create(cocos2d::EaseSineOut::create(cocos2d::ScaleTo::create(0.1, current_scale*1.2)), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.15, current_scale)), cocos2d::CallFunc::create([=]() {
				if (_image)
					_image->setOpacity(opacity);
				if (_lbl_value)
					_lbl_value->setOpacity(opacity);
				if (_background)
					_background->setOpacity(opacity);
			}), NULL));
		}
		else {
			audio_helper->play("audio_wrong.mp3");

			_image->setColor(cocos2d::Color3B(253, 192, 193));
			auto shake = cocos2d::Sequence::create(cocos2d::MoveBy::create(0.15, cocos2d::Vec2(8, 0)), cocos2d::MoveBy::create(0.15, cocos2d::Vec2(-8, 0)), NULL);
			this->runAction(cocos2d::Sequence::create(cocos2d::Repeat::create(shake, 2), cocos2d::CallFunc::create([=]() {
				_image->setColor(cocos2d::Color3B::WHITE);
			}), NULL));
		}
	}, delay, cocos2d::StringUtils::format("onTouchObject %d", _id));
}

bool GameNoiDiem_Item::isTouchedAt(cocos2d::Vec2 point)
{
	cocos2d::Vec2 locationInNode = this->convertToNodeSpace(point);
	cocos2d::Size s = this->getBoundingBox().size;
	cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
	{
		if (isDetected(locationInNode)) {
			return true;
		}
	}
	return false;
}

void GameNoiDiem_Item::setTouchDetectedType(math::common::DetectedType detected_type)
{
	_detected_type = detected_type;
}

void GameNoiDiem_Item::initLabel(std::string text)
{
	_lbl_value = cocos2d::ui::Text::create();
	_lbl_value->setPositionZ(4);
	_lbl_value->setString(text);
	_lbl_value->setTextColor(cocos2d::Color4B::BLACK);
	_lbl_value->setVisible(false);

	_lbl_value->enableOutline(cocos2d::Color4B::WHITE, 3);
	addChild(_lbl_value, 2);
}

void GameNoiDiem_Item::setGAF(std::string gaf_file_path)
{
	if (gaf_file_path.size() > 4) {
		_gaf = GAFWrapper::createGAF(gaf_file_path);
		_gaf->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		_gaf->setPosition(this->getContentSize() / 2);
		_gaf->setVisible(false);
		_gaf->setCascadeOpacityEnabled(true);
		addChild(_gaf, 4);
		// sequence
		if (gaf_file_path.find("cloud") != std::string::npos) {
			fadein_sequence = "fade_in";
			idle_sequence = "idle";
			correct_sequence = "correct";
		}
		else if (gaf_file_path.find("arrow") != std::string::npos) {
			fadein_sequence = "fade_in";
			idle_sequence = "loop";
			correct_sequence = "";
		}
	}


}

void GameNoiDiem_Item::setImage(math::resource::Image * image)
{
	if (!_image) {
		_image = cocos2d::ui::ImageView::create();
		_image->setPosition(this->getContentSize() / 2);
		addChild(_image, 1);
		_image->setScale9Enabled(true);
		_image->setCascadeOpacityEnabled(true);
		_image->setPositionZ(3);
	}
	if (image != nullptr) {
		_img_src = image;
		_img_src->used = true;
		_image->loadTexture(image->path);

		auto render_size = _image->getVirtualRendererSize();
		cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
		_image->setCapInsets(cap);

		if (_image_touch != nullptr) {
			delete _image_touch;
			_image_touch = nullptr;
		}
		_image_touch = new cocos2d::Image();
		_image_touch->initWithImageFile(image->path);
		_image->setVisible(true);
	}
}

void GameNoiDiem_Item::setBackground(math::resource::Image * background)
{
	if (!_background) {
		_background = cocos2d::ui::ImageView::create();
		_background->setPosition(this->getContentSize() / 2);
		_background->setCascadeOpacityEnabled(true);
		addChild(_background);
	}
	if (background != nullptr) {

		_bg_src = background;
		_bg_src->used = true;
		_background->setScale9Enabled(true);
		_background->loadTexture(background->path);

		auto render_size = _background->getVirtualRendererSize();
		cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
		_background->setCapInsets(cap);
		_background->setVisible(true);
	}
	_background->setVisible(false);

}


void GameNoiDiem_Item::captureState() {
	_captured_node.position = this->getPosition();
	_captured_node.scale = this->getScale();
	_captured_node.rotation = this->getRotation();

}

void GameNoiDiem_Item::restoreCapturedState() {
	this->setPosition(_captured_node.position);
	this->setScale(_captured_node.scale);
	this->setRotation(_captured_node.rotation);
}

GAFWrapper * GameNoiDiem_Item::getGAF()
{
	return _gaf;
}
