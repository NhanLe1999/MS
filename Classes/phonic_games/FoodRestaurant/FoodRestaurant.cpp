//python convert.py D:\Downloads\csb2csd-master\csb2csd-masterD:\Downloads\csb2csd-master\csb2csd-master\FoodRestaurant.csb D:\Downloads\csb2csd-master\csb2csd-masterD:\Downloads\csb2csd-master\csb2csd-master\FoodRestaurant.csd
//
//	FoodRestaurant.h
//	Created by mintzy on 22/02/18
//
// 
#include "FoodRestaurant.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

INITIALIZE_READER(FoodRestaurant);

#define speed_opacity 15

std::once_flag FoodRestaurant_reader;

#define CSB_NAME "csb/game/FoodRestaurant/FoodRestaurant.csb"

#define SCROLLING_CONVEYOR_SPEED 3.0f
#define CUSTOMER_SPEED 375
#define PLATE_MOVE_OUT_SPEED 1024
#define FOOD_SPACING 350
#define GAF_EFFECT "gaf/foodrestaurant/effect/effect.gaf"
#define GAF_COIN "gaf/foodrestaurant/coin/coin.gaf"
#define GAF_COIN_EFFECT "gaf/foodrestaurant/coin_effect/effect.gaf"

#define GAME_NAME "hdr/games/foodrestaurant/Conveyor_belt Restaurants.png" 
// sound
#define FR_SOUND_BGM "sounds/foodrestaurant/msg.mp3"
#define FR_SOUND_BEGIN_SLIDE "sounds/foodrestaurant/begin_slide.mp3"
#define FR_SOUND_BIRD "sounds/foodrestaurant/bird.mp3"
#define FR_SOUND_CORRECT "sounds/foodrestaurant/correct.mp3"
#define FR_SOUND_DISH_MOVE "sounds/foodrestaurant/dish_move.mp3"
#define FR_SOUND_DRAG "sounds/foodrestaurant/drag.mp3"
#define FR_SOUND_HUZZ "sounds/foodrestaurant/huzz.mp3"
#define FR_SOUND_INTRO "sounds/foodrestaurant/intro.mp3"
#define FR_SOUND_INTRO_VI_2 "sounds/foodrestaurant/intro_vi_2.mp3"
#define FR_SOUND_INTRO_VI_3 "sounds/foodrestaurant/intro_vi_3.mp3"
#define FR_SOUND_POP "sounds/foodrestaurant/pop.mp3"
#define FR_SOUND_SLIDE "sounds/foodrestaurant/slide.mp3"
#define FR_SOUND_WRONG "sounds/foodrestaurant/wrong.mp3"
#define SOUND_GAME_NAME "sounds/foodrestaurant/Conveyor_Belt Restaurant.mp3"

FoodRestaurant * FoodRestaurant::createGame(std::string json_file)
{

	std::call_once(FoodRestaurant_reader, [] {
		REGISTER_CSB_READER(FoodRestaurant);
	});
	auto p = static_cast<FoodRestaurant *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}
FoodRestaurant * FoodRestaurant::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(FoodRestaurant_reader, [] {
		REGISTER_CSB_READER(FoodRestaurant);
	});
	auto p = static_cast<FoodRestaurant *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file,images);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void FoodRestaurant::didLoadFromCSB(std::string json_file) {
	if (json_file.size() < 5) {
		json_file = "json/foodrestaurant/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	this->generateData(document.GetObject());
}
void FoodRestaurant::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	this->generateData(document.GetObject(), images);
}


void FoodRestaurant::generateData(rapidjson::Value value_data) {
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	rapidjson::Value& json_game_config = value_data.FindMember("game_config")->value;
	rapidjson::Value& customer_onboard = json_game_config.FindMember("number_customer_onboard")->value;
	is_replace_customer = json_game_config.FindMember("replace_customer")->value.GetBool();
	customer_onboard_min = customer_onboard.FindMember("minimum")->value.GetInt();
	customer_onboard_max = customer_onboard.FindMember("maximum")->value.GetInt();
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		FoodRestaurantInfo info;
		if (item.HasMember("text")) {
			info.text = item.FindMember("text")->value.GetString();
		}
		if (item.HasMember("audio")) {
			info.audio = item.FindMember("audio")->value.GetString();
		}
		if (item.HasMember("repeat")) {
			info.number_repeat = item.FindMember("repeat")->value.GetInt();
		}
		_food_infos.push_back(info);
	}
}

void FoodRestaurant::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	rapidjson::Value& json_game_config = value_data.FindMember("game_config")->value;
	rapidjson::Value& customer_onboard = json_game_config.FindMember("number_customer_onboard")->value;
	is_replace_customer = json_game_config.FindMember("replace_customer")->value.GetBool();
	customer_onboard_min = customer_onboard.FindMember("minimum")->value.GetInt();
	customer_onboard_max = customer_onboard.FindMember("maximum")->value.GetInt();
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		if (item.HasMember("image_id") && item["image_id"].IsString()) {
			FoodRestaurantInfo info;
			auto image = math::resource::ResHelper::getImage(images, item["image_id"].GetString());
			info.text = image->getText("name_1");
			info.audio = image->getAudios("name_1").front();
			info.number_repeat = item.FindMember("repeat")->value.GetInt();
			_food_infos.push_back(info);
		}
		
		
	}
}

void FoodRestaurant::informationGame() {
	auto blinds = utils::findChild<ui::ImageView*>(this, "blinds");
	auto wall_right = utils::findChild<ui::ImageView*>(this, "wall_right");
	auto wall_left = utils::findChild<ui::ImageView*>(this, "wall_left");
	auto table = utils::findChild<ui::ImageView*>(this, "table");
	auto cups = utils::findChild<ui::ImageView*>(this, "cups");
	auto coin_bg = utils::findChild<ui::ImageView*>(this, "coin_bg");
	auto lbl_coin = utils::findChild<ui::Text*>(this, "lbl_coin");
	auto img_coin = utils::findChild<ui::ImageView*>(this, "img_coin");
	auto conveyor3 = utils::findChild<ui::ImageView*>(this, "conveyor3");
	auto conveyor2 = utils::findChild<ui::ImageView*>(this, "conveyor2");
	auto conveyor1 = utils::findChild<ui::ImageView*>(this, "conveyor1");
	blinds->setVisible(false);
	wall_right->setVisible(false);
	wall_left->setVisible(false);
	table->setVisible(false);
	cups->setVisible(false);
	conveyor1->setVisible(false);
	conveyor2->setVisible(false);
	conveyor3->setVisible(false);
	coin_bg->setVisible(false);
	lbl_coin->setVisible(false);
	img_coin->setVisible(false);

	this->runAction(Sequence::create(DelayTime::create(0.5f)
		, CallFunc::create([=]() {
			if (auto infoGame = Sprite::create(GAME_NAME)) {
				auto visibleSize = Director::getInstance()->getVisibleSize();
				infoGame->setAnchorPoint(Vec2(0.5, 0.5));
				infoGame->setPosition(Vec2(-1000, visibleSize.height / 2));
				if (visibleSize.width / visibleSize.height < 1.5) {
					infoGame->setScale(0.6);
				}
				else {
					if (visibleSize.width / visibleSize.height < 1.8) {
						infoGame->setScale(0.8);
					}
					else {
						infoGame->setScale(1);
					}
				}
				infoGame->setName("gameName");
				this->addChild(infoGame);

				infoGame->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.0f, Vec2(visibleSize / 2)))
					, DelayTime::create(0.1f)
					, CallFunc::create([=]() {
						AUDIO_ENGINE->playSound(SOUND_GAME_NAME);
						})
					, DelayTime::create(2.5f)
							, MoveTo::create(0.5f, Vec2(visibleSize.width + 1000, visibleSize.height / 2)), NULL));
			}
			})
		, DelayTime::create(4.0f)
				, CallFunc::create([=]() {
				if (auto child = utils::findChild(this, "gameName")) {
					child->removeFromParent();
				}
				blinds->setVisible(true);
				wall_right->setVisible(true);
				wall_left->setVisible(true);
				table->setVisible(true);
				cups->setVisible(true);
				conveyor1->setVisible(true);
				conveyor2->setVisible(true);
				conveyor3->setVisible(true);
				coin_bg->setVisible(true);
				lbl_coin->setVisible(true);
				img_coin->setVisible(true);
				loadFrames();
				schedule(schedule_selector(FoodRestaurant::updateConveyor), 0);
				initCustomer();
				initFoodItems();
				showIntroGame();
				startGame();
					}), NULL));
}

void FoodRestaurant::loadFrames()
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	screen_size = Director::getInstance()->getWinSize();
	root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout * >(this, "root_layout");
	customer_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(root_layout, "customer_layout");
	walk_layout = cocos2d::utils::findChild<cocos2d::ui::Layout * >(this, "walk_layout");

	conveyor1 = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(root_layout, "conveyor1");
	conveyor2 = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(root_layout, "conveyor2");
	conveyor3 = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(root_layout, "conveyor3");

    PHONIC_MANAGER->cacheGAF(GAF_EFFECT, GAF_EFFECT);
    PHONIC_MANAGER->cacheGAF(GAF_COIN, GAF_COIN);
    PHONIC_MANAGER->cacheGAF(GAF_COIN_EFFECT, GAF_COIN_EFFECT);
    
	img_coin = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(root_layout, "img_coin");
	img_coin->setVisible(false);
	gaf_coin = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache[GAF_COIN]);
	gaf_coin->setPosition(img_coin->getPosition());
	gaf_coin->play(true);
	gaf_coin->setScale(0.25f);
	root_layout->addChild(gaf_coin, 10000, "gaf_coin");

	lbl_coin = cocos2d::utils::findChild<cocos2d::ui::Text *>(root_layout, "lbl_coin");
	lbl_coin->setString(cocos2d::StringUtils::format("%d/%d", coin_count, total_point));
	lbl_coin->setFontName(font_name);

	m_game_state = FR_GAME_STATE::NONE;
	current_time = last_time = 0;
	combo_count = 0;

}

void FoodRestaurant::initFoodItems()
{
	_food_slots.clear();
	for (int i = 0; i < 10 && i < _customer_items.size(); ++i) {
		FoodItem * f_item = FoodItem::createItem(_customer_items[i]->getInfo());
		_food_items.push_back(f_item);
	}
	mj::helper::shuffle(_food_items);
	for (int i = _customer_items.size(); i < 10; ++i) {
		int info_ind = i % _food_infos.size();// cocos2d::random(0, (int)_food_infos.size() - 1);
		FoodItem * f_item = FoodItem::createItem(_food_infos[info_ind]);
		_food_items.push_back(f_item);

		if (i % _food_infos.size() == 0) {
			mj::helper::shuffle(_food_infos);
		}
	}
	//mj::helper::shuffle(_food_items);
	for (int i = 0; i < _food_items.size(); ++i) {
		FoodItemSlot * slot = new FoodItemSlot();
		slot->m_position = Vec2(screen_size.width + (i + 1) * 300, 100);
		slot->item = _food_items[i];
		_food_items[i]->setPosition(slot->m_position);
		_food_items[i]->setFoodItemDelegate(this);
		root_layout->addChild(_food_items[i], 100);
		_food_slots.push_back(slot);
	}
}

void FoodRestaurant::initCustomer()
{
	number_customer = 0;
	total_point = 0;
	num_cus_start = cocos2d::random(customer_onboard_min, customer_onboard_max);
	std::vector<FoodRestaurantInfo> _datas;
	float font_size = 80;
	for (auto info : _food_infos) {
		if (info.number_repeat <= 0) continue;
		int i = 0;
		do {
			_datas.push_back(info);
			number_customer++;
			++i;
		} while (is_replace_customer && i < info.number_repeat);
		if (info.text.size() > 4) font_size = 60;
	}
	mj::helper::shuffle(_datas);
	for (int i = 0; i < _datas.size(); ++i) {
		CustomerItem * c_item = CustomerItem::createItem(_datas[i], i + 1);
		c_item->font_size = font_size;
		c_item->retain();
		c_item->setDelegate(this);
		_customer_items.push_back(c_item);
	}
	auto it_end = (_customer_items.size() < 3) ? _customer_items.end() : _customer_items.begin() + 3;
	static std::random_device mj_rd;
	static std::mt19937 mj_g(mj_rd());
	std::shuffle(_customer_items.begin(), it_end, mj_g);

	// init customer slot
	for (int i = 0; i < customer_onboard_max; ++i) {
		auto c_slot = new CustomerItemSlot();
		c_slot->item = nullptr;
		c_slot->m_position = cocos2d::Vec2((float)(i + 1) / (customer_onboard_max + 1) * screen_size.width, screen_size.height * 0.53);
		c_slot->reserved = false;
		c_slot->check = true;
		_customer_slots.push_back(c_slot);
	}
	for (int i = 0; i < num_cus_start && i < _customer_items.size(); ++i) {
		total_point += _customer_items[i]->getInfo().number_repeat;
	}
}

void FoodRestaurant::showIntroGame()
{
	setGameState(FR_GAME_STATE::WAITING);
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 1900000000);

	auto audio_path = std::string(FR_SOUND_INTRO);
    if (CONFIG_MANAGER->isGameVMonkey()){
        if (number_customer == 2) audio_path = std::string(FR_SOUND_INTRO_VI_2);
        else audio_path = std::string(FR_SOUND_INTRO_VI_3);
    }
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
//    showGuideGame(8.0f);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
		if (is_auto_play) {
			autoTest();
			setGameState(FR_GAME_STATE::SCRIPTING);
		}
		else {
			touch_layout->removeFromParent();
			setGameState(FR_GAME_STATE::PLAYING);	
		}

	});

}

void FoodRestaurant::startGame()
{
	setGameState(FR_GAME_STATE::PLAYING);
	playBackgroundMusic(FR_SOUND_BGM);
	AUDIO_ENGINE->playSound(FR_SOUND_BEGIN_SLIDE, false, [=] {
		AUDIO_ENGINE->playSound(FR_SOUND_SLIDE, true);
	});
	AUDIO_ENGINE->playSound(FR_SOUND_BIRD, true);
	addCustomer(num_cus_start,true);
	lbl_coin->setString(cocos2d::StringUtils::format("%d/%d", coin_count, total_point));
	//this->schedule(schedule_selector(FoodRestaurant::updateAnswer));
    showGuideGame(10.0f);
	this->schedule([=](float dt) {
		if (checkNoFoodCorrect()) {
			changeFoodItemsOnSlot();
		}
	}, 1.0f, "update answer");
}

void FoodRestaurant::endGame()
{
    this->hideTapGuideGame();
    AUDIO_ENGINE->stopAllEffects();
	this->unscheduleAllCallbacks();
	setGameState(FR_GAME_STATE::ENDED);
	MJMainGBase::onFinishGame(/*CHARACTER::NAT*/);
	
}

void FoodRestaurant::earnCoin(cocos2d::Vec2 pos)
{
	// update coin collect
	GAFWrapper * gaf_coin_move = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache[GAF_COIN]);
	gaf_coin_move->setPosition(pos);
	gaf_coin_move->setScale(0.5f);
	root_layout->addChild(gaf_coin_move, 100000);
	gaf_coin_move->play(true);
	gaf_coin_move->runAction(Sequence::create(DelayTime::create(0.15f), MoveBy::create(0.25f, Vec2(0, 150)), DelayTime::create(0.25f), MoveTo::create(0.25f, Vec2(gaf_coin->getPosition())),
		CallFunc::create([=] {
		GAFWrapper * gaf_coin_effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache[GAF_COIN_EFFECT]);
		gaf_coin_effect->setPosition(gaf_coin->getPosition());
		root_layout->addChild(gaf_coin_effect, 1000001);
		gaf_coin_effect->play(false, [=](gaf::GAFObject *) {
			gaf_coin_effect->removeFromParent();
		});
		gaf_coin_move->removeFromParent();
		lbl_coin->setString(cocos2d::StringUtils::format("%d/%d", ++coin_count, total_point));
		lbl_coin->runAction(Sequence::create(ScaleTo::create(0.125, 1.25f), ScaleTo::create(0.125f, 1.0f), NULL));
	}), NULL));

}

void FoodRestaurant::showGuideGame(float delay_time)
{
	setGameState(FR_GAME_STATE::GUIDE);
	auto guide_layout_check = this->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");
	guide_layout->schedule([=](float) {
		AUDIO_ENGINE->playSound(FR_SOUND_INTRO);
		showHandGuideGame();
	}, 8.0f, 100, delay_time, "animated_guide");
}

void FoodRestaurant::inActiveMode() {
	schedule([=](float) {
		AUDIO_ENGINE->playSound(FR_SOUND_INTRO);
	}, 8.0f, 100, 10.0f, "inActiveMode");
}

void FoodRestaurant::showHandGuideGame(std::string gaf_guide_path)
{
	std::vector<FoodItemSlot *> free_foods;
	free_foods.clear();
	for (auto slot : _food_slots) {
		if (slot->item && slot->item->getPositionX() > 0 && slot->item->getPositionX() < screen_size.width) {
			free_foods.push_back(slot);
		}
	}
	std::vector<CustomerItemSlot *> free_customers;
	free_customers.clear();
	for (auto slot : _customer_slots) {
		if (slot->item && slot->check) {
			free_customers.push_back(slot);
		}
	}
	if (free_foods.size() == 0 || free_customers.size() == 0) return;

	FoodItemSlot * f_slot = nullptr;
	FoodItem * f_item = nullptr;
	CustomerItem * c_item = nullptr;
	int c_ind = cocos2d::random(0, (int)free_customers.size() - 1);
	c_item = free_customers[c_ind]->item;
	for (auto slot : free_foods) {
		if (isSameInfo(slot->item->getInfo(), c_item->getInfo())) {
			f_slot = slot;
			f_item = f_slot->item;
			break;
		}
	}
	if (f_slot == nullptr) {	
		hideGuideGame();
		showGuideGame(0.5f);
		inActiveMode();
		return;
	}
	GAFWrapper* guide_gaf = GAFWrapper::createGAF(gaf_guide_path);
	guide_gaf->setPosition(f_item->getPosition());
	//guide_gaf->setScale(2.0f);
	root_layout->addChild(guide_gaf, 10000);
	guide_gaf->play(false);
	f_item->setZOrder(1000);
	f_item->onChangeFoodItemState(FOOD_ITEM_STATE::MOVING);
	f_item->runAction(Sequence::create(MoveTo::create(0.5f, c_item->getPosition()), DelayTime::create(0.15f), CallFunc::create([=] {
		f_item->runAction(Sequence::create(MoveTo::create(0.25f,
			Vec2(f_slot->m_position.x - 0.25 * 60 * conveyor_speed, f_slot->m_position.y)), CallFunc::create([=] {
			f_item->onChangeFoodItemState(FOOD_ITEM_STATE::RUNNING);
			f_item->setZOrder(100);
			setGameState(FR_GAME_STATE::PLAYING);
		}), NULL));
	}), NULL));
	guide_gaf->runAction(Sequence::create(MoveTo::create(0.5f, c_item->getPosition()), CallFunc::create([=] {
		guide_gaf->removeFromParent();
	}), NULL));
}

void FoodRestaurant::hideGuideGame()
{
	auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check) {
		guide_layout_check->unscheduleAllCallbacks();
		guide_layout_check->removeFromParentAndCleanup(true);
	}
}

void FoodRestaurant::setGameState(FR_GAME_STATE state)
{
	if (state == m_game_state) return;
	m_game_state = state;
	switch (m_game_state)
	{
	case FR_GAME_STATE::SCRIPTING:
		break;
	case FR_GAME_STATE::WAITING:
		break;
	case FR_GAME_STATE::GUIDE:
		break;
	case FR_GAME_STATE::PLAYING:
		break;
	case FR_GAME_STATE::ENDED:
		break;
	case FR_GAME_STATE::NONE:
		break;
	default:
		break;
	}
}


void FoodRestaurant::onEnter() {
	MJMainGBase::onEnter();
	informationGame();
	//this->loadFrames();
	

}


void FoodRestaurant::onExitTransitionDidStart() {
	//cocos2d::ui::Layout::onExitTransitionDidStart();
	//this->unscheduleAllCallbacks();
	//this->unscheduleUpdate();
	//this->stopAllActions();
//    AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onExitTransitionDidStart();
}

void FoodRestaurant::updateConveyor(float dt)
{
	current_time += dt;
	if (current_time - last_time > 3.0f) {
		combo_count = combo_count >0 ? combo_count  -1 : 0;
		last_time = current_time;
	}
	conveyor_speed = (SCROLLING_CONVEYOR_SPEED + combo_count * combo_count / 7.5f) * dt * 60;
	ScrollingConveyor();
	ScrollingFood();
}

void FoodRestaurant::onRefresh(cocos2d::Ref *sender) {

}

void FoodRestaurant::onChangeFoodItemStateDelegate(FOOD_ITEM_STATE state)
{
	switch (state)
	{
	case FOOD_ITEM_STATE::SHOWED:
		break;
	case FOOD_ITEM_STATE::RUNNING:
		break;
	case FOOD_ITEM_STATE::MOVING:
		AUDIO_ENGINE->playSound(FR_SOUND_DISH_MOVE);
		AUDIO_ENGINE->playSound(FR_SOUND_DRAG);
		break;
	case FOOD_ITEM_STATE::CATCHED:
		break;
	case FOOD_ITEM_STATE::HIDE:
		break;
	default:
		break;
	}
}

void FoodRestaurant::onTouchBeganFoodItemDelegate(cocos2d::Ref * sender)
{
	FoodItem * item = static_cast<FoodItem *>(sender);
	hideTapGuideGame();
	AUDIO_ENGINE->stopSound(FR_SOUND_INTRO);
	item->setZOrder(1000);
	item->onChangeFoodItemState(FOOD_ITEM_STATE::MOVING);
}

void FoodRestaurant::onMoveEndedFoodItemDelegate(cocos2d::Ref * sender)
{
	FoodItem * f_item = static_cast<FoodItem *>(sender);
	f_item->setZOrder(100);
	auto c_slot = isOnCustomerSlot(f_item);
	if (c_slot != nullptr) {
		auto c_item = c_slot->item;
		if (isSameInfo(f_item->getInfo(), c_item->getInfo())) {
			flag = false;
			unschedule("animated_guide");
			hideGuideGame();
			last_time = current_time;
			if (combo_count < 5) {
				++combo_count;
			}
			// lock check customer slot
			auto f_slot = getSlotContaintFood(f_item);
			f_slot->item = nullptr;
			if (is_replace_customer) {
				c_slot->item = nullptr;
				c_slot->reserved = false;
			}
			else {
				c_slot->check = false;
			}
			//
			
			c_item->hideOrder([=] {
			});
			auto text_effect = cocos2d::ui::Text::create("", font_name, 100);
			text_effect->setString(f_item->getInfo().text);
			text_effect->setPosition(c_slot->m_position);
			root_layout->addChild(text_effect, 1000);
			text_effect->setScale(0);
			text_effect->enableOutline(Color4B::BLACK, 2);
			text_effect->runAction(Sequence::create(
                                                    Spawn::create(MoveTo::create(0.25f, Vec2(screen_size.width / 2, screen_size.height * 0.4)), FadeIn::create(0.25f), ScaleTo::create(0.25f, 2.0f), NULL),
				CallFunc::create([=] {
				//std::vector<std::string> chain;
				//chain.push_back(f_item->getInfo().audio);
				//chain.push_back(FR_SOUND_CORRECT);
				//AUDIO_ENGINE->playChain(chain);
				}),
				DelayTime::create(0.5f),
				FadeOut::create(0.3f),
				CallFunc::create([=] {
				text_effect->removeFromParentAndCleanup(true);
			}), NULL));
			std::vector<std::string> chain;
			chain.push_back(f_item->getInfo().audio);
			chain.push_back(FR_SOUND_CORRECT);
			//AUDIO_ENGINE->playChain(chain);
			AUDIO_ENGINE->playChain(chain);
			c_item->playGafSequence("win", false, [=] {
				c_item->playGafSequence("idle", true);
				onGoingOutRestaurant(c_item);
				if (f_item->getPositionX() > screen_size.width / 2) {
					f_item->runAction(MoveTo::create((screen_size.width - f_item->getPositionX() + 150) / PLATE_MOVE_OUT_SPEED,
						Vec2(screen_size.width + 150, f_item->getPositionY())));
				}
				else {
					f_item->runAction(MoveTo::create((f_item->getPositionX() + 150) / PLATE_MOVE_OUT_SPEED, Vec2(-150, f_item->getPositionY())));
				}
			});
			f_item->cleanPlate();
			f_item->runAction(Sequence::create(
				Spawn::create(MoveTo::create(0.15f, Vec2(c_slot->m_position.x, 280)), ScaleTo::create(0.15f, 0.8), NULL), NULL));
			auto gaf_effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache[GAF_EFFECT]);
			gaf_effect->setPosition(c_slot->m_position);
			root_layout->addChild(gaf_effect, 1000);
			gaf_effect->play(false, [=](gaf::GAFObject * obj) {
				gaf_effect->removeFromParent();
			});
			earnCoin(Vec2(c_item->getPositionX(), c_item->getPositionY()));
            //this->showGuideGame(10.0);
			inActiveMode();
		}
		else {
			c_item->playGafSequence("lose", false, [=] {
				c_item->playGafSequence("idle", true);
			});
			auto f_slot = getSlotContaintFood(f_item);
			f_item->runAction(Sequence::create(MoveTo::create(0.25f,
				Vec2(f_slot->m_position.x - 0.25 * 60 * conveyor_speed, f_slot->m_position.y)), CallFunc::create([=] {
				f_item->onChangeFoodItemState(FOOD_ITEM_STATE::RUNNING);

			}), NULL));
			if (flag) {
				this->showGuideGame(10.0);
			}
			inActiveMode();
		}
	}
	else {
		auto f_slot = getSlotContaintFood(f_item);
		f_item->runAction(Sequence::create(MoveTo::create(0.25f,
			Vec2(f_slot->m_position.x - 0.25 * 60 * conveyor_speed, f_slot->m_position.y)), CallFunc::create([=] {
			f_item->onChangeFoodItemState(FOOD_ITEM_STATE::RUNNING);
		}), NULL));
		if (flag) {
			this->showGuideGame(10.0);
		}
		inActiveMode();
	}
}

void FoodRestaurant::onChangeCustomerItemStateDelegate(CUSTOMER_STATE state)
{
	switch (state)
	{
	case CUSTOMER_STATE::IDLE:
		break;
	case CUSTOMER_STATE::WALKING:
		break;
	case CUSTOMER_STATE::WIN:
		//AUDIO_ENGINE->playSound(FR_SOUND_CORRECT);
		break;
	case CUSTOMER_STATE::LOSE:
		AUDIO_ENGINE->playSound(FR_SOUND_WRONG);
		break;
	case CUSTOMER_STATE::WAITTING_FOOD:
		break;
	case CUSTOMER_STATE::SHOW_ORDER:
		AUDIO_ENGINE->playSound(FR_SOUND_POP);
		AUDIO_ENGINE->playSound(FR_SOUND_HUZZ);
		break;
	case CUSTOMER_STATE::HIDE_ORDER:
		break;
	default:
		break;
	}
}

CustomerItemSlot * FoodRestaurant::isOnCustomerSlot(FoodItem * sender)
{
	auto pos = customer_layout->convertToNodeSpace(sender->getPosition());
	for (auto c_slot : _customer_slots) {
		if (c_slot->item != nullptr && c_slot->check) {
			auto bounding = c_slot->item->getBoundingBox();
			bounding.size.height += 200;
			if (bounding.containsPoint(pos)) {
				return c_slot;
			}
		}

	}
	return nullptr;
}

bool FoodRestaurant::onGoingToRestaurant(CustomerItem * sender, float dt)
{

	std::vector<CustomerItemSlot *> _free_slots;
	for (auto slot : _customer_slots) {
		if (slot->item == nullptr && !slot->reserved) {
			_free_slots.push_back(slot);
		}
	}

	if (_free_slots.size() == 0) return false;
	int slot_index = cocos2d::random(0, (int)_free_slots.size() - 1);
	CustomerItemSlot * free_slot = _free_slots[slot_index];
	free_slot->reserved = true;
	walk_layout->addChild(sender, 0);
	int direct = cocos2d::random(0, 1);

	/*
	* direct = 0 : From Right
	* direct = 1 : From Left
	*/
	if (direct == 0) {
		sender->setPosition(Vec2(screen_size.width, screen_size.height * 0.53));
	}
	else {
		sender->setPosition(Vec2(0, screen_size.height * 0.53));
		sender->setCustomerFlipped(true);
	}

	float distance = abs(sender->getPositionX() - free_slot->m_position.x);
	sender->playGafSequence("walk", true);
	sender->runAction(Sequence::create(DelayTime::create(dt), MoveTo::create(distance / CUSTOMER_SPEED, free_slot->m_position), CallFunc::create([=] {
		Vec2 pos = sender->getPosition();
		sender->removeFromParentAndCleanup(true);
		customer_layout->addChild(sender, 100);
		sender->setPosition(pos);
		free_slot->item = sender;
		sender->showOrder();
		sender->playGafSequence("idle", true);
	}), NULL));


	return true;

}
void FoodRestaurant::onGoingOutRestaurant(CustomerItem * sender)
{
	if (!sender) return;
	if (is_replace_customer) {
		--remain_customer_onboard;
		for (auto c_slot : _customer_slots) {
			if (c_slot->item == sender) {
				c_slot->item = nullptr;
				c_slot->check = true;
			}
		}
		Vec2 pos = sender->getPosition();
		sender->removeFromParentAndCleanup(true);
		walk_layout->addChild(sender, 100);
		sender->setPosition(pos);
		sender->playGafSequence("walk", true);
		if (!sender->isCustomerFlipped()) {
			sender->runAction(MoveTo::create(sender->getPositionX() / CUSTOMER_SPEED, Vec2(0, sender->getPositionY())));
		}
		else {
			sender->runAction(MoveTo::create((screen_size.width - sender->getPositionX()) / CUSTOMER_SPEED, Vec2(screen_size.width, sender->getPositionY())));
		}
		if (remain_customer_onboard <= 0) {
			if (customer_count < number_customer) {
				scheduleOnce([=](float dt) {
					int n = cocos2d::random(customer_onboard_min, customer_onboard_max);
					addCustomer(n);
				}, 1.0f, cocos2d::StringUtils::format("add customer %d", customer_count));
			}
			else {
				scheduleOnce([=](float dt) {
					endGame();
				}, 1.0f, "end game");
			}
		}
	}
	else {
		sender->setInfoNumberRepeat(sender->getInfo().number_repeat - 1);
		if (sender->getInfo().number_repeat > 0) {
			sender->showOrder([=] {
				for (auto c_slot : _customer_slots) {
					if (c_slot->item == sender) {
						c_slot->check = true;
					}
				}
			});
		}
		else {
			/*if (customer_count < number_customer) {
				sender->setInfo(_customer_items[customer_count]->getInfo());
				customer_count++;
				sender->showOrder([=] {
					for (auto c_slot : _customer_slots) {
						if (c_slot->item == sender) {
							c_slot->check = true;
						}
					}
				});
				return;
			}*/
			--remain_customer_onboard;
			Vec2 pos = sender->getPosition();
			sender->removeFromParentAndCleanup(true);
			walk_layout->addChild(sender, 100);
			sender->setPosition(pos);
			sender->playGafSequence("walk", true);
			if (!sender->isCustomerFlipped()) {
				sender->runAction(MoveTo::create(sender->getPositionX() / CUSTOMER_SPEED, Vec2(0, sender->getPositionY())));
			}
			else {
				sender->runAction(MoveTo::create((screen_size.width - sender->getPositionX()) / CUSTOMER_SPEED, Vec2(screen_size.width, sender->getPositionY())));
			}
			
			if (remain_customer_onboard <= 0) {
				scheduleOnce([=](float dt) {
					endGame();
				}, 1.0f, "end game");
			}

		}
	}
}
bool FoodRestaurant::isSameInfo(FoodRestaurantInfo info1, FoodRestaurantInfo info2)
{
	return info1.text == info2.text;
}

FoodItemSlot * FoodRestaurant::getSlotContaintFood(FoodItem * item)
{
	if (item == nullptr) return nullptr;
	for (auto slot : _food_slots) {
		if (item == slot->item) return slot;
	}
	return nullptr;
}

void FoodRestaurant::ScrollingConveyor()
{	
	conveyor1->setPositionX(conveyor1->getPositionX() - conveyor_speed);
	conveyor2->setPositionX(conveyor2->getPositionX() - conveyor_speed);
	conveyor3->setPositionX(conveyor3->getPositionX() - conveyor_speed);


	if (conveyor1->getPositionX() <= -conveyor1->getContentSize().width) {
		conveyor1->setPositionX(conveyor3->getPositionX() + conveyor3->getContentSize().width - 49);
	}
	if (conveyor2->getPositionX() <= -conveyor2->getContentSize().width) {
		conveyor2->setPositionX(conveyor1->getPositionX() + conveyor1->getContentSize().width - 49);
	}
	if (conveyor3->getPositionX() <= -conveyor3->getContentSize().width) {
		conveyor3->setPositionX(conveyor2->getPositionX() + conveyor2->getContentSize().width - 49);
	}
}

void FoodRestaurant::ScrollingFood()
{
	int i;
	std::vector<FoodRestaurantInfo> c_infos;
	for (auto slot : _customer_slots) {
		if (slot->item != nullptr) {
			c_infos.push_back(slot->item->getInfo());
		}
	}
	for (i = 0; i < _food_slots.size(); ++i) {
		auto slot = _food_slots[i];
		slot->m_position.x -= conveyor_speed;
		if (slot->m_position.x < -300) {
			int pre_slot = (i + _food_slots.size() - 1) % _food_slots.size();
			slot->m_position.x = _food_slots[pre_slot]->m_position.x + FOOD_SPACING;
			if (slot->item == nullptr) {
				FoodRestaurantInfo info;

			
				if (c_infos.size() > 0) {
					int ind = cocos2d::random(0, (int)c_infos.size() - 1);
					info = c_infos[ind];
				}
				else {
					int ind = cocos2d::random(0, (int)_food_infos.size() - 1);
					info = _food_infos[ind];
				}
				CCLOG("Create item info %s", info.text.c_str());
				FoodItem * f_item = FoodItem::createItem(info);
				slot->item = f_item;
				f_item->setPosition(slot->m_position);
				f_item->setFoodItemDelegate(this);
				root_layout->addChild(f_item, 100);
				slot->item = f_item;

			}
		}
		if (slot->item && slot->item->getState() != FOOD_ITEM_STATE::MOVING) {
			slot->item->setPosition(slot->m_position);
		}
	}
}

void FoodRestaurant::addCustomer(int num, bool re_loc)
{
	if (re_loc) {
		for (int i = 0; i < num; ++i) {
			_customer_slots[i]->m_position.x = (float)(i + 1) / (num + 1) * screen_size.width;
			_customer_slots[i]->reserved = false;
		}
		for (int i = num; i < customer_onboard_max; ++i) {
			_customer_slots[i]->reserved = true;
		}
	}
	float dt = 0;
	for (int i = 0; i < num && customer_count < number_customer; ++i) {
		if (onGoingToRestaurant(_customer_items[customer_count], dt)) {
			dt += 0.75f;
			++customer_count;
			++remain_customer_onboard;
		}
	}
}


bool FoodRestaurant::checkNoFoodCorrect()
{

	for (auto f_slot : _food_slots) {
		if (f_slot->item == nullptr) continue;
		for (auto c_slot : _customer_slots) {
			if (c_slot->item && c_slot->check && f_slot->item->getPositionX() > screen_size.width &&  isSameInfo(c_slot->item->getInfo(), f_slot->item->getInfo())) return false;
		}
	}
	CCLOG(__FUNCTION__);
	return true;
}

void FoodRestaurant::changeFoodItemsOnSlot()
{
	CCLOG(__FUNCTION__);
	std::vector<FoodRestaurantInfo> c_infos;
	for (auto slot : _customer_slots) {
		if (slot->item != nullptr && slot->check) {
			c_infos.push_back(slot->item->getInfo());
		}
	}
	if (c_infos.size() <= 0) return;
	int ind = 0;
	mj::helper::shuffle(c_infos);
	for (auto slot : _food_slots) {
		if ( (slot->item && slot->item->getPositionX() > screen_size.width + 200)) {
			auto rm_item = slot->item;
			slot->item = nullptr;
			if(rm_item) rm_item->removeFromParent();
			auto info = c_infos[ind++];
			FoodItem * f_item = FoodItem::createItem(info);
			slot->item = f_item;
			f_item->setPosition(slot->m_position);
			f_item->setFoodItemDelegate(this);
			root_layout->addChild(f_item, 100);
			slot->item = f_item;
			if(ind == c_infos.size() ) break;
		}

	}
}

#pragma callback
ui::Widget::ccWidgetClickCallback FoodRestaurant::onLocateClickCallback(const std::string &callback_name) {
	if (callback_name == "onRefresh") {
		return CC_CALLBACK_1(FoodRestaurant::onRefresh, this);
	}
	return nullptr;
}


#define auto_testing_delay 2.0f
void FoodRestaurant::autoTest()
{
	this->schedule([=](float dt) {
		std::vector<FoodItemSlot *> free_foods;
		free_foods.clear();
		for (auto slot : _food_slots) {
			if (slot->item && slot->item->getPositionX() > 0 && slot->item->getPositionX() < screen_size.width) {
				free_foods.push_back(slot);
			}
		}
		std::vector<CustomerItemSlot *> free_customers;
		free_customers.clear();
		for (auto slot : _customer_slots) {
			if (slot->item && slot->check) {
				free_customers.push_back(slot);
			}
		}
		if (free_foods.size() == 0 || free_customers.size() == 0) return;

		int f_ind = cocos2d::random(0, (int)free_foods.size() - 1);
		FoodItemSlot * f_slot = free_foods[f_ind];
		FoodItem * f_item = f_slot->item;
		int c_ind = cocos2d::random(0, (int)free_customers.size() - 1);
		CustomerItem * c_item = free_customers[c_ind]->item;

		onTouchBeganFoodItemDelegate(f_item);
		GAFWrapper* guide_gaf = GAFWrapper::createGAF("gaf/guidegame/hand2/hand_tap.gaf");
		guide_gaf->setPosition(f_item->getPosition());
		root_layout->addChild(guide_gaf, 10000);
		guide_gaf->playSequence("tap", false, [=](gaf::GAFObject * obj, std::string  sqName) {
			guide_gaf->playSequence("drag", true);
		});
		f_item->runAction(Sequence::create(MoveTo::create(0.5f, c_item->getPosition()), CallFunc::create([=] {
			onMoveEndedFoodItemDelegate(f_item);
		}), NULL));
		guide_gaf->runAction(Sequence::create(MoveTo::create(0.5f, c_item->getPosition()), CallFunc::create([=] {
			guide_gaf->removeFromParent();
		}), NULL));

	}, auto_testing_delay, 1000000, 2.0f, "auto test");
}

