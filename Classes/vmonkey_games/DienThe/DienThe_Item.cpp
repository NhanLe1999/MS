#include "DienThe_Item.h"

DienThe_Item * DienThe_Item::createItem(math::resource::Image* img_item, std::string mask)
{
	auto * obj = new DienThe_Item();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(img_item,mask);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

std::string DienThe_Item::getImagePath()
{
    if(_img_item)
    {
        return _img_item->path;
    }
}

void DienThe_Item::showItem(float dt, std::function<void()> call_back)
{
	this->setOpacity(0);
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeIn::create(0.25), cocos2d::CallFunc::create([=] {
		if (call_back) call_back();
	}),NULL));

}

void DienThe_Item::hideItem(float dt, std::function<void()> call_back)
{
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),cocos2d::FadeOut::create(0.25f), cocos2d::CallFunc::create([=]() {
		if (call_back) call_back();
	}), NULL));
}


void DienThe_Item::backToStart(float time)
{
	m_layout_touch->setTouchEnabled(false);
	this->runAction(cocos2d::Sequence::create(cocos2d::EaseQuadraticActionIn::create(cocos2d::MoveTo::create(time, _start_pos)), cocos2d::CallFunc::create([=] {
		m_layout_touch->setTouchEnabled(true);
	}), NULL));
}


void DienThe_Item::setTouchHandleType(DienThe_ItemTouchType type)
{
	
	switch (type)
	{
	case DienThe_ItemTouchType::NONE:
		m_layout_touch->setTouchEnabled(false);
		break;
	case DienThe_ItemTouchType::CLICK:
		m_layout_touch->addTouchEventListener(nullptr);
		m_layout_touch->setTouchEnabled(true);
		m_layout_touch->addClickEventListener([=](cocos2d::Ref * sender) {
			if (_delegate) {
				_delegate->onClickedItem(this);
			}
		});
		break;
	case DienThe_ItemTouchType::TOUCH:
		m_layout_touch->addClickEventListener(nullptr);
		m_layout_touch->setTouchEnabled(true);
		m_layout_touch->addTouchEventListener([=](cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type) {
			cocos2d::Vec2 move_pos;
			switch (type)

			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				CCLOG("onTouchBegan");
				m_is_moved = false;
				_start_pos = this->getPosition();
				if (_delegate) {
					_delegate->onMoveBeginItem(this);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				m_is_moved = true;
				if (_delegate) {
					move_pos = m_layout_touch->getTouchMovePosition();
					this->setPosition(move_pos);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				if (_delegate) {
					_delegate->onMoveEndedItem(this,m_is_moved);
				}
				break;
			default:
				break;
			}
		});
		break;
	default:
		break;
	}
}

void DienThe_Item::setTouchEnable(bool enable)
{
	m_layout_touch->setTouchEnabled(enable);
}


void DienThe_Item::createTexture()
{
	/*auto item_size = this->getContentSize();
	cocos2d::Sprite * cutzone = cocos2d::Sprite::create("games/vm_sap_xep/Layer-45-copy-2.png");
	cocos2d::Sprite * model = cocos2d::Sprite::create(m_item_info.image_path);
	
	cutzone->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	model->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	cutzone->setPosition(item_size / 2);
	model->setPosition(item_size / 2);

	cocos2d::RenderTexture * render = cocos2d::RenderTexture::create(item_size.width, item_size.height);
	render->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	render->beginWithClear(1, 1, 1, 1);
	model->visit();
	cutzone->setBlendFunc(cocos2d::BlendFunc{ GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA });
	cutzone->visit();
	render->end();
	render->getSprite()->setPosition(cocos2d::ccp(item_size.width / 2, item_size.height / 2));

	this->addChild(render);*/
}

void DienThe_Item::didLoadFromCSB(math::resource::Image* img_item, std::string mask)
{
	_img_item = img_item;
	auto item_size = cocos2d::Size(125, 125);

	this->setContentSize(item_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setCascadeOpacityEnabled(true);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	

	m_img_item = cocos2d::ui::ImageView::create(_img_item->path);
	float scale = item_size.height / m_img_item->getVirtualRendererSize().height;
	m_img_item->setScale(scale);
	m_img_item->setPosition(cocos2d::Vec2(item_size / 2) + cocos2d::Vec2(0,5));
	this->addChild(m_img_item);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setContentSize(item_size);
	this->addChild(m_layout_touch, 10000);

	//createTexture();
}


