//
//  MJ_BuggiesV2_ChooseCar.h
//

#include "BuggiesV2_ChooseCar.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
INITIALIZE_READER(BuggiesV2_ChooseCar);
#define CSB_NAME "csb/game/BuggiesV2/BuggiesChooseCar.csb"
std::once_flag buggiesv2_choosecar_reader;
using namespace cocos2d;

#define bg_game "games/buggiesv2/buggies.png"

struct ChooseCarInfo {
	int tag;
	std::string car_path;
};



const std::vector<ChooseCarInfo> CARS_INFO = {
	{1, "games/buggiesv2/car_1.png"},
	{2, "games/buggiesv2/car_2.png"},
	{3, "games/buggiesv2/car_3.png" }
};

BuggiesV2_ChooseCar * BuggiesV2_ChooseCar::createView(int type) {
	std::call_once(buggiesv2_choosecar_reader, [] {
		REGISTER_CSB_READER(BuggiesV2_ChooseCar);
	});
	auto p = static_cast<BuggiesV2_ChooseCar *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(type);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void BuggiesV2_ChooseCar::didLoadFromCSB(int type) {
	cocos2d::utils::findChild(this, StringUtils::format("bg_choose_%d", type))->setVisible(true);

	this->map_index = type;

	auto list = this->getChildByName<ui::ListView *>("list");
	list->setMagneticType(ui::ListView::MagneticType::CENTER);
	list->setMagneticAllowedOutOfBoundary(true);
	list->setScrollBarOpacity(0);
	list->setScrollDuration(0.5f);
	list->setBounceEnabled(true);
	list->setFocused(true);
	

	auto screen_size = Director::getInstance()->getWinSize();

	float width = 300;
	float height = 0;
	auto start_pos = Vec2(screen_size.width / 2 - CARS_INFO.size() / 2 * width, 600/2 - CARS_INFO.size()/2 * height );
	//get choose your car label
	auto chooseYourCarLabel = this->getChildByName<ui::ImageView*>("choose_your_car");
	if (chooseYourCarLabel)
	{
		chooseYourCarLabel->setVisible(false);
	}
	

	//show game intro before choose car
	
	if (FileUtils::getInstance()->isFileExist(bg_game))
	{
		_imgGameName = ui::ImageView::create(bg_game);
		_imgGameName->setPosition(Point(screen_size.width / 2, screen_size.height - _imgGameName->getContentSize().height));
		this->addChild(_imgGameName, 100);
	}
	
	AUDIO_ENGINE->playEffect("sounds/buggiesv2/cpeedy_spell!.mp3");
	auto sequenceImgNameFadeout = Sequence::create(FadeOut::create(2.f), CallFunc::create([=] {
		AUDIO_ENGINE->playEffect("sounds/buggiesv2/choose_your_car.mp3");
		if (_imgGameName)
		{
			_imgGameName->removeFromParent();
		}
		for (auto i = 0; i < (int)CARS_INFO.size(); ++i) {
			
			if (FileUtils::getInstance()->isFileExist(CARS_INFO[i].car_path))
			{
				auto item = cocos2d::ui::ImageView::create();
				item->setTouchEnabled(true);
				item->addClickEventListener(CC_CALLBACK_1(BuggiesV2_ChooseCar::onSelectedCar, this));
				item->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
				item->loadTexture(CARS_INFO[i].car_path);
				item->setTag(CARS_INFO[i].tag);
				item->setTouchEnabled(true);
				item->setPosition(Vec2(start_pos.x + i * width, start_pos.y + i * height));
				item->setRotation(-90);
				this->addChild(item);
			}
			
			//list->pushBackCustomItem(item);
		}

		if (chooseYourCarLabel)
		{
			chooseYourCarLabel->setVisible(true);
		}
		

	}), NULL);
	
		
	if (_imgGameName)
	{
		_imgGameName->runAction(sequenceImgNameFadeout);
	}
	//list->jumpToItem(1, cocos2d::Vec2::ANCHOR_MIDDLE, cocos2d::Vec2::ANCHOR_MIDDLE);

	auto btn_back = this->getChildByName("btn_back");
	btn_back->setPositionX(screen_size.width / 2 - 260);


	auto btn_next = this->getChildByName("btn_next");
	btn_next->setPositionX(screen_size.width / 2 + 260);

	this->getChildByName("btn_let_go")->setVisible(false);
	btn_back->setVisible(false);
	btn_next->setVisible(false);
	list->setVisible(false);
	

}

void BuggiesV2_ChooseCar::onEnter() {
	cocos2d::ui::Layout::onEnter();

}

void BuggiesV2_ChooseCar::setDelegate(BuggiesV2_ChooseCarDelegate *delegate) {
	_delegate = delegate;
}

void BuggiesV2_ChooseCar::onSelectedCar(cocos2d::Ref * sender)
{
	auto car = static_cast<cocos2d::ui::ImageView*>(sender);
	auto name = car->getRenderFile().file;
	if (_delegate) {
		_delegate->onChooseCar(name, map_index);
		this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), NULL));
	}
}

void BuggiesV2_ChooseCar::onClickedNext(cocos2d::Ref * sender)
{
	auto list = this->getChildByName<ui::ListView *>("list");
	int cur_index = list->getIndex(list->getCenterItemInCurrentView());
	CCLOG("Click Next %d", cur_index);
	list->scrollToItem(cur_index + 1, cocos2d::Vec2::ANCHOR_MIDDLE, cocos2d::Vec2::ANCHOR_MIDDLE);
}

void BuggiesV2_ChooseCar::onClickedBack(cocos2d::Ref * sender)
{
	
	auto list = this->getChildByName<ui::ListView *>("list");
	int cur_index = list->getIndex(list->getCenterItemInCurrentView());
	
	CCLOG("Click Back %d", cur_index);
	
	list->scrollToItem(cur_index - 1, cocos2d::Vec2::ANCHOR_MIDDLE, cocos2d::Vec2::ANCHOR_MIDDLE);
	
	
}

void BuggiesV2_ChooseCar::onClickedLetsGo(cocos2d::Ref * sender)
{
	CCLOG("Click Let's Go");
	auto list = this->getChildByName<ui::ListView *>("list");
	auto car = static_cast<cocos2d::ui::ImageView*>(list->getCenterItemInCurrentView());
	auto name = car->getRenderFile().file;
	/*auto list_split = mj::helper::split(name, '_');
	car_index = list_split[1];*/
	if (_delegate) {
		_delegate->onChooseCar(name, map_index);
		this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), NULL));
	}
}

//void BuggiesV2_ChooseCar::onChooseCar(cocos2d::Ref *sender) {
//	auto car = static_cast<cocos2d::ui::ImageView*>(sender);
//	auto name = car->getName();
//	auto list_split = mj::helper::split(name, '_');
//	car_index = list_split[1];
//	auto pos_move = car->getPosition();
//	auto parent = car->getParent();
//	auto list_child = parent->getChildren();
//	for (auto child : list_child) {
//		child->setVisible(false);
//	}
//	car->setVisible(true);
//	pos_move = parent->convertToWorldSpace(pos_move);
//	auto screen_size = cocos2d::Director::getInstance()->getWinSize();
//	pos_move = Vec2(screen_size / 2) - pos_move;
//
//	if (_delegate) {
//		auto touch_layout = cocos2d::ui::Layout::create();
//		touch_layout->setContentSize(screen_size);
//		this->addChild(touch_layout, 1000000);
//		touch_layout->setBackGroundColor(Color3B::WHITE);
//		touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//		touch_layout->setOpacity(0);
//		auto scale_vl = 6.0f;
//		this->runAction(Sequence::create(Spawn::create(EaseSineIn::create(MoveBy::create(1.0f, pos_move*scale_vl)), EaseSineIn::create(ScaleTo::create(1.0f, scale_vl)), NULL), CallFunc::create([=] {
//			_delegate->onChooseCar(car_index, map_index);
//			touch_layout->runAction(Sequence::create(FadeTo::create(1.0f, 255), FadeOut::create(0.5f), NULL));
//		}), FadeOut::create(1.0f), DelayTime::create(0.5f), CallFunc::create([=] {
//			this->removeFromParent();
//		}), NULL));
//	}
//}

ui::Widget::ccWidgetClickCallback BuggiesV2_ChooseCar::onLocateClickCallback(const std::string &name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		{ "onClickedBack",CC_CALLBACK_1(BuggiesV2_ChooseCar::onClickedBack,this) },
		{ "onClickedNext",CC_CALLBACK_1(BuggiesV2_ChooseCar::onClickedNext,this) },
		{ "onClickedLetsGo",CC_CALLBACK_1(BuggiesV2_ChooseCar::onClickedLetsGo,this) }
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}
