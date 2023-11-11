
#include "GameRabbitClassify_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameRabbitClassify_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/rabbit_classify/GameRabbitClassify_Item.csb"

GameRabbitClassify_Item * GameRabbitClassify_Item::createGame(ItemInfo item_info) {
    auto p = static_cast<GameRabbitClassify_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameRabbitClassify_Item::didLoadFromCSB(ItemInfo item_info) {
	//cocos2d::ui::Helper::doLayout(this);
	this->setContentSize(cocos2d::Size(150, 150));
	_item_info = item_info;
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto imgItem = cocos2d::ui::ImageView::create(item_info.img_path, TextureResType::LOCAL);
	imgItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	imgItem->setPosition(Vec2(150, 150)/2);
	this->addChild(imgItem);
	float fWidth = imgItem->getContentSize().width;
	float fHeight = imgItem->getContentSize().height;

	if (fWidth>fHeight){
			imgItem->setScale(150.0f / fWidth);
	}
	else{
			imgItem->setScale(150.0f / fHeight);
	}
}

void GameRabbitClassify_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}


void GameRabbitClassify_Item::setDelegate(MJGameSample_ItemDelegate *_delegate){
    this->delegate = _delegate;
}


void GameRabbitClassify_Item::onChoose(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onClickItem(this);
    }
}

ui::Widget::ccWidgetClickCallback GameRabbitClassify_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onChoose") {
        return CC_CALLBACK_1(GameRabbitClassify_Item::onChoose, this);
    }
    return nullptr;
}
