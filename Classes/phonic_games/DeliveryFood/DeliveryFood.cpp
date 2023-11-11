#include "DeliveryFood.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"

std::once_flag delivery_food_reader;
using namespace rapidjson;

#define BACKGROUND_INTRO "games/deliveryfood/Food_delivery.png"
#define IMG_BACKGROUND "games/deliveryfood/bg.png"
#define IMG_COIN_BG "games/deliveryfood/3.png"
#define GAF_COIN "gaf/deliveryfood/coin/coin.gaf"
#define GAF_EFFECT "gaf/deliveryfood/effect/effect.gaf"
#define GAF_EFFECT1 "gaf/deliveryfood/effect1/effect.gaf"

#define SOUND_INTRO "sounds/deliveryfood/Food_delivery.mp3"
#define SOUND_BGM "sounds/deliveryfood/df_bgm.mp3"
#define SOUND_WOOHOO "sounds/deliveryfood/df_woohoo.mp3"
#define SOUND_CORRECT "sounds/deliveryfood/df_correct.mp3"
#define SOUND_WRONG "sounds/deliveryfood/df_wrong%d.mp3"
#define SOUND_OPEN "sounds/deliveryfood/df_open.mp3"
#define SOUND_CLOSE "sounds/deliveryfood/df_close.mp3"
#define SOUND_GIVEFOOD "sounds/deliveryfood/df_givefood.mp3"
#define SOUND_IDLE "sounds/deliveryfood/df_idle.mp3"
#define SOUND_COIN "sounds/deliveryfood/df_coin.mp3"
#define SOUND_QUESTION "sounds/deliveryfood/df_question.mp3"
#define SOUND_QUESTION_VI_AM "sounds/deliveryfood/df_question_vi_am.mp3"
#define SOUND_QUESTION_VI_VAN "sounds/deliveryfood/df_question_vi_van.mp3"



DeliveryFood * DeliveryFood::createGame(std::string json_file)
{
	auto p = new DeliveryFood();
	if (p->init()) {
		p->autorelease();
		p->didLoadFromCSB(json_file);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

DeliveryFood * DeliveryFood::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	auto p = new DeliveryFood();
	if (p->init()) {
		p->autorelease();
		p->didLoadFromCSB(json_file,images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void DeliveryFood::didLoadFromCSB(std::string json_file)
{
	if (json_file.size() < 5) {
		json_file = "json/deliveryfood/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;

	document.Parse< rapidjson::kParseDefaultFlags>(str.c_str());
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	this->generateData(document.GetObject());
	
}

void DeliveryFood::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;

	document.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	this->generateData(document.GetObject(),images);
}

void DeliveryFood::generateData(rapidjson::Value value_data)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;

	std::string game_mode = game_config_data.FindMember("game_mode")->value.GetString();
	m_is_drag_game = (game_mode == "drag");
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		DeliveryFoodInfo data;
		if (item.HasMember("text_phonic")) {
			data.info.text_phonic = item.FindMember("text_phonic")->value.GetString();
		}
		if (item.HasMember("audio_phonic")) {
			data.info.audio_phonic = item.FindMember("audio_phonic")->value.GetString();
		}
		if (item.HasMember("text_full")) {
			data.info.text_full = item.FindMember("text_full")->value.GetString();
		}
		if (item.HasMember("audio_full")) {
			data.info.audio_full = item.FindMember("audio_full")->value.GetString();
		}
		if (item.HasMember("repeat")) {
			data.repeat = item.FindMember("repeat")->value.GetInt();
		}

		m_game_data.push_back(data);
	}
	m_total_coin = 0;
	for (auto data : m_game_data) {
		m_total_coin += data.repeat;
	}
}

void DeliveryFood::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;

	if (JSON_SAFE_CHECK(game_config_data, "custom_intro_audio", String))
	{
		auto customIntroAudio = game_config_data["custom_intro_audio"].GetString();
		auto imageFull = math::resource::ResHelper::getImage(images, customIntroAudio);
		_audioIntro = imageFull->getAudio();
	}


	std::string game_mode = game_config_data.FindMember("game_mode")->value.GetString();
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && game_config_data.HasMember("audio_intro_vi")){
       auto audio_intro = game_config_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }

	m_is_drag_game = (game_mode == "drag");
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		if (item.HasMember("image_id") && item["image_id"].IsString()) {
			DeliveryFoodInfo data;
			auto image_id = item["image_id"].GetString();
			auto image = math::resource::ResHelper::getImage(images,image_id);
			data.info.text_full = image->getText("name_1");
			data.info.text_phonic = image->getText("name_n");
			data.info.audio_full = image->getAudios("name_1").front();
			data.info.audio_phonic = image->getAudios("name_n").front();
			if (item.HasMember("repeat")) {
				data.repeat = item.FindMember("repeat")->value.GetInt();
			}
			m_game_data.push_back(data);
		}
	}
	m_total_coin = 0;
	for (auto data : m_game_data) {
		m_total_coin += data.repeat;
	}
}

void DeliveryFood::onEnter()
{
	MJMainGBase::onEnter();
	this->loadFrames([=]() {
		startGame();
	});
}

void DeliveryFood::loadFrames(std::function<void()> callback)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }

	screen_size = cocos2d::Director::getInstance()->getWinSize();
	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(screen_size);
	m_layout_root->setTouchEnabled(true);
	this->addChild(m_layout_root);

	m_layout_locked = cocos2d::ui::Layout::create();
	m_layout_locked->setContentSize(screen_size);
	m_layout_locked->setTouchEnabled(true);
	this->addChild(m_layout_locked, 1000000);

	m_background = cocos2d::ui::ImageView::create(IMG_BACKGROUND);
	m_background->setPosition(screen_size / 2);
	m_background->setScale9Enabled(true);
	auto virtual_size = m_background->getVirtualRendererSize();
	m_background->setCapInsets(cocos2d::Rect(1, 0.8 * virtual_size.height, virtual_size.width - 2,virtual_size.height - 2));
	m_background->setContentSize(cocos2d::Size(m_background->getContentSize().width, m_background->getContentSize().height * 1.15));
	m_layout_root->addChild(m_background);

	_backgroundIntro = cocos2d::ui::ImageView::create(BACKGROUND_INTRO);
	_backgroundIntro->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_backgroundIntro->setScale9Enabled(true);
	_backgroundIntro->setPosition(screen_size/2);
	_backgroundIntro->setOpacity(0);
	m_layout_root->addChild(_backgroundIntro);

	_backgroundIntro->runAction(Sequence::create(
		FadeIn::create(0.5f),
		CallFunc::create([=]() {
			AUDIO_ENGINE->play(SOUND_INTRO);
			}),
		DelayTime::create(2.0f),
		FadeOut::create(0.5f),
		CallFunc::create([=]() {
				initWindows();
				initHands();
				// init coin
				m_coin_earn = 0;
				m_coin_background = cocos2d::ui::ImageView::create(IMG_COIN_BG);
				m_coin_background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
				m_coin_background->setScale9Enabled(true);
				auto vitural_size = m_coin_background->getVirtualRendererSize();
				m_coin_background->setCapInsets(cocos2d::Rect(1, 1, vitural_size.width - 2, vitural_size.height - 2));
				m_coin_background->setPosition(cocos2d::Vec2(25, 50));
				m_layout_root->addChild(m_coin_background);

				m_lbl_coin = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%d/%d", m_coin_earn, m_total_coin), font_name, 40);
				m_lbl_coin->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
				m_lbl_coin->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
				m_lbl_coin->setPosition(cocos2d::Vec2(125, 50));
				m_layout_root->addChild(m_lbl_coin, 2);

				m_gaf_coin = GAFWrapper::createGAF(GAF_COIN);
				m_gaf_coin->setPosition(cocos2d::Vec2(60, 45));
				m_gaf_coin->setScale(0.25f);
				m_gaf_coin->play(true);
				m_layout_root->addChild(m_gaf_coin, 2);

				m_touchable = false;
				auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
				speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
				speaker->addClickEventListener([=](cocos2d::Ref* sender) {
					showGuideGame();
					if (m_touchable) {
						if (CONFIG_MANAGER->isGameVMonkey()) {
							AUDIO_ENGINE->playSound(question_vi);
						}
						else {
							auto path = _audioIntro != "" ? _audioIntro : SOUND_QUESTION;
							AUDIO_ENGINE->playSound(path);
						}
					}

					});
				this->addChild(speaker);

				PHONIC_MANAGER->cacheGAF(GAF_COIN, GAF_COIN);
				PHONIC_MANAGER->cacheGAF(GAF_EFFECT, GAF_EFFECT);
				PHONIC_MANAGER->cacheGAF(GAF_EFFECT1, GAF_EFFECT1);
				if (callback) {
					callback();
				}
		}),
		NULL));
}

void DeliveryFood::showIntroGame()
{
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 1900000000);
	auto path = _audioIntro != "" ? _audioIntro : SOUND_QUESTION;
    if (CONFIG_MANAGER->isGameVMonkey()){
        path = question_vi.c_str();
    }
	AUDIO_ENGINE->playSound(path, false, [=] {
		if (is_auto_play) {
			autoPlay();
		}
		else {		
			touch_layout->removeFromParent();
            m_touchable = true;
		}

	});
	unschedule("animated_guide");
	showGuideGame(10);
}

void DeliveryFood::startGame()
{
	m_wrong_count = 0;
	m_hand_index = -1;
	playBackgroundMusic(SOUND_BGM);
	showIntroGame();
	onNextHand();	

}

void DeliveryFood::onRefresh(cocos2d::Ref * sender)
{
}

void DeliveryFood::onFinishGame()
{
	MJMainGBase::onFinishGame();
}

void DeliveryFood::update(float dt)
{
}

void DeliveryFood::onClickedWindowDelegate(cocos2d::Ref * sender)
{
	unschedule("animated_guide");
	auto window = static_cast<DeliveryFood_Window *>(sender);
	if (m_is_drag_game) {
		window->offerFood();
		AUDIO_ENGINE->playSound(window->getInfo().audio_full, false,[=]() {
			showGuideGame();
			});
	}
	else {
		if (m_selected_hand) {
			m_selected_hand->_start_pos = m_selected_hand->getPosition();
		}
		deliverFoodForCustomer(window);
	}
}

void DeliveryFood::onClickedHand(cocos2d::Ref * sender)
{
	auto obj = static_cast<DeliveryFood_Hand *>(sender);
	AUDIO_ENGINE->playSound(obj->getInfo().audio_phonic);
	obj->_start_pos = obj->getPosition();
	showGuideGame();
}

void DeliveryFood::onMoveBeginHand(cocos2d::Ref * sender)
{
	auto obj = static_cast<DeliveryFood_Hand *>(sender);
	AUDIO_ENGINE->playSound(obj->getInfo().audio_phonic);
	unschedule("animated_guide");
}

void DeliveryFood::onMoveEndedHand(cocos2d::Ref * sender)
{
	auto hand = static_cast<DeliveryFood_Hand *>(sender);
	auto pos = hand->getPosition();
	DeliveryFood_Window * window = nullptr;
	for (auto w : m_windows) {
		auto bounding = w->getBoundingBox();
		if (bounding.containsPoint(pos)) {
			window = w;
		}
	}
	if (window) {
		deliverFoodForCustomer(window);
	}
	else {
		hand->backToStart();
	}
	showGuideGame();
}

void DeliveryFood::onNextGame()
{
	this->scheduleOnce([=](float) {
		onFinishGame();
	}, 1.0f, "delay endgame");
	
}

void DeliveryFood::onNextHand()
{
	++m_hand_index;
	if (m_coin_earn == m_total_coin) {
		onNextGame();
		return;
	}
	if (m_hand_index >= m_hands.size()) {
		initHands();
	}
	m_selected_hand = m_hands[m_hand_index];
	m_layout_root->addChild(m_selected_hand, 100000);
	m_selected_hand->setDelegate(this);
	if (m_is_drag_game) {
		m_selected_hand->setTouchHandleType(DeliveryFoodHandTouchType::TOUCH);
	}
	else {
		m_selected_hand->setTouchHandleType(DeliveryFoodHandTouchType::CLICK);
	}
	onRefreshWindow();	
	m_selected_hand->showHand(0.25f, [=] {
		setTouchLock(false);	
	});
	
}

void DeliveryFood::onExitTransitionDidStart()
{
    for(auto hand : m_hands)
    {
        CC_SAFE_RELEASE(hand);
    }
    PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}

void DeliveryFood::initWindows()
{
    mj::helper::shuffle(m_game_data);
	auto locations = getLocationWindows(6);
	mj::helper::shuffle(locations);
	for (int i = 0; i < locations.size(); ++i) {
		auto window = DeliveryFood_Window::createWindow(m_game_data[i].info, i + 1);
		window->setDelegate(this);
		window->setPosition(locations[i]);
		m_layout_root->addChild(window);
		//window->showWindow();
		m_windows.push_back(window);
	}
	for (auto win : m_windows) {
		int z_order = screen_size.width + screen_size.height - win->getPositionX() - win->getPositionY();
		win->setLocalZOrder(z_order);
	}
}

void DeliveryFood::initHands()
{
    mj::helper::shuffle(m_game_data);
	for (auto data : m_game_data) {
		for (int i = 0; i < data.repeat; ++i) {
			auto hand = DeliveryFood_Hand::createHand(data.info);
			hand->retain();
			hand->setPosition(Vec2(screen_size.width / 2, 90));
			hand->idle();
			m_hands.push_back(hand);
			//	m_layout_root->addChild(hand, 1000);
		}
	}
}

void DeliveryFood::onRefreshWindow()
{
	if (!m_selected_hand) return;
	for (auto window : m_windows) {
		window->getLableText()->runAction(FadeOut::create(0.25f));
	}
	this->scheduleOnce([=](float dt) {
		std::vector<mj::PHONIC_INFO> answers;
		answers.push_back(m_selected_hand->getInfo());
		mj::helper::shuffle(m_game_data);
		for (auto info : m_game_data) {
			if (info.info.text_phonic != m_selected_hand->getInfo().text_phonic) {
				answers.push_back(info.info);
			}
			if (answers.size() == 6) break;
		}
		while (answers.size() < m_windows.size()) {
			mj::PHONIC_INFO info;
			answers.push_back(info);
		}
		mj::helper::shuffle(answers);
		for (int i = 0; i < answers.size() && i < m_windows.size(); ++i) {
			m_windows[i]->setInfo(answers[i]);
			m_windows[i]->showWindow();
		}
	}, 0.25f, "on_refresh_windows");
	
}

void DeliveryFood::earnCoin(cocos2d::Vec2 pos, DeliveryFood_Window* window)
{
	++m_coin_earn;
	auto gaf_effect1 = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at(GAF_EFFECT1));
	gaf_effect1->setPosition(pos);
	m_layout_root->addChild(gaf_effect1, 120000);
	gaf_effect1->play(false, [=](gaf::GAFObject * obj) {
		gaf_effect1->removeFromParent();
	});

	auto gaf_coin = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at(GAF_COIN));
	gaf_coin->setPosition(pos);
	gaf_coin->setScale(0.5f);
	m_layout_root->addChild(gaf_coin, 110000);
	gaf_coin->play(true);
	gaf_coin->runAction(cocos2d::Sequence::create(
		MoveBy::create(0.25f, Vec2(0, 150)),
		DelayTime::create(0.25f),
		Spawn::create(MoveTo::create(0.25f, m_gaf_coin->getPosition()), ScaleTo::create(0.25f, 0.25f), NULL),
		CallFunc::create([=] {
			gaf_coin->removeFromParent();
			m_lbl_coin->setString(cocos2d::StringUtils::format("%d/%d", m_coin_earn, m_total_coin));
			auto gaf_effect2 = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at(GAF_EFFECT));
			gaf_effect2->setPosition(m_gaf_coin->getPosition());
			m_layout_root->addChild(gaf_effect2, 110000);
			gaf_effect2->play(false, [=](gaf::GAFObject * obj) {
				gaf_effect2->removeFromParent();
			});
			//	m_gaf_coin->runAction(Sequence::create(ScaleTo::create(0.15f, 0.35f), ScaleTo::create(0.15f, 0.25f), NULL));
			m_lbl_coin->runAction(Sequence::create(ScaleTo::create(0.15f, 1.25f), ScaleTo::create(0.15f, 1.0f), NULL));
			if (window->getInfo().audio_full != "") {
				AUDIO_ENGINE->playSound(SOUND_COIN);
				AUDIO_ENGINE->play(window->getInfo().audio_full, false, 1.0f, [=]() {
					onNextHand();
					showGuideGame();
					});
			}
			else {
				AUDIO_ENGINE->playSound(SOUND_COIN, false, [=]() {
					onNextHand();
					unschedule("animated_guide");
					showGuideGame();
				});
			}
			
		
		}),
		NULL));
}

void DeliveryFood::deliverFoodForCustomer(DeliveryFood_Window * window)
{
	if (!m_selected_hand) return;
	setTouchLock(true);
	if (m_selected_hand->getInfo().text_phonic == window->getInfo().text_phonic) {
		m_wrong_count = 0;
		this->unschedule("offer");
		m_selected_hand->servedFood(window->getPosition());
		AUDIO_ENGINE->playSound(SOUND_GIVEFOOD);
		AUDIO_ENGINE->playSound(SOUND_CORRECT,false,nullptr,0.8f);
		AUDIO_ENGINE->playSound(SOUND_WOOHOO,false, nullptr, 0.8f);
		earnCoin(window->getPosition(),window);
		window->playWin(m_selected_hand->getHandId(), [=] {
		});
	}
	else {
		AUDIO_ENGINE->playSound(cocos2d::StringUtils::format(SOUND_WRONG,cocos2d::random(1,4)));
		window->playLose();
		m_selected_hand->backToStart();
		setTouchLock(false);
		++m_wrong_count;
		if (m_wrong_count >= 3) {
			showOffer(0);
			showHandGuideGame(8,true);
			unschedule("animated_guide");
			showGuideGame();
		}
	}
}


void DeliveryFood::setTouchLock(bool locked)
{
	m_layout_locked->setTouchEnabled(locked);
}

std::vector<Vec2> DeliveryFood::getLocationWindows(int number_window, cocos2d::Size window_size, int padding)
{
	int floor1{ 0 };
	int floor2{ 0 };

	if (number_window > 6) number_window = 6;
	switch (number_window)
	{
	case 1:
		floor1 = 1;
		floor2 = 0;
		break;
	case 2:
		floor1 = 1;
		floor2 = 1;
		break;
	case 3:
		floor1 = 2;
		floor2 = 1;
		break;
	case 4:
		floor1 = 2;
		floor2 = 2;
		break;
	case 5:
		floor1 = 3;
		floor2 = 2;
	case 6:
		floor1 = 3;
		floor2 = 3;
		break;
	}
	std::vector<cocos2d::Vec2> locations;
	locations.clear();
	cocos2d::Vec2 start_pos1 = Vec2(screen_size.width / 2 - (floor1 - 1) * (window_size.width + padding) * 0.5f, 376);
	for (int i = 0; i < floor1; ++i) {
		cocos2d::Vec2 pos(start_pos1.x + i * (window_size.width + padding), start_pos1.y);
		locations.push_back(pos);
	}
	cocos2d::Vec2 start_pos2 = Vec2(screen_size.width / 2 - (floor2 - 1) * (window_size.width + padding) * 0.5f, 616);
	for (int i = 0; i < floor2; ++i) {
		cocos2d::Vec2 pos(start_pos2.x + i * (window_size.width + padding), start_pos2.y);
		locations.push_back(pos);
	}
	return locations;
}


void DeliveryFood::showOffer(float dt)
{
	if (!m_selected_hand) return;
	this->scheduleOnce([=](float dt) {
		std::vector<DeliveryFood_Window* > available_windows;
		for (auto window : m_windows) {
			if (window->getInfo().audio_phonic == m_selected_hand->getInfo().audio_phonic) {
				available_windows.push_back(window);
			}
		}
		/*available_windows.clear();
		for (auto win : m_windows) {
			if (win->getWindowState() == DeliveryFood_WindowState::SHOWING || win->getWindowState() == DeliveryFood_WindowState::CLOSE) {
				available_windows.push_back(win);
			}
		}*/
		if (available_windows.size() == 0) return;
		int index = cocos2d::random(0, (int)available_windows.size() - 1);
		available_windows[index]->offerFood();
		m_wrong_count = 0;
		AUDIO_ENGINE->playSound(available_windows[index]->getInfo().audio_full);
	}, dt,"offer");
}


void DeliveryFood::showGuideGame(int intervalTime /*intervalTime = 8.0f */ )
{
	unschedule("animated_guide");
	this->scheduleOnce([=](float) {
		showHandGuideGame(intervalTime);
	}, intervalTime,"animated_guide");
}

void DeliveryFood::hideGuideGame()
{
	unschedule("animated_guide");
	
}
void DeliveryFood::showHandGuideGame(int  intervalTime, bool offerFood)
{
	if (!m_selected_hand) return;
	Vec2 pos;
	for (auto window : m_windows) {
		if (window->getInfo().audio_phonic == m_selected_hand->getInfo().audio_phonic) {
			pos = window->getPosition();
			break;
		}
	}
	if (m_is_drag_game) {
		if (offerFood) {

		}
		else {
			isShowingGuid = true;
			m_selected_hand->setTouchEnable(false);
			m_selected_hand->setVisibleLayout(false, true);
			auto preMovePosition = m_selected_hand->getPosition();

			m_selected_hand->runAction(Sequence::create(DelayTime::create(10.0f / 24),
				MoveTo::create(1.0f, pos),
				DelayTime::create(0.15f),
				CallFunc::create([=] {
					m_selected_hand->setPosition(preMovePosition);
					m_selected_hand->setVisibleLayout(true, false);
					m_selected_hand->setTouchEnable(true);
					})
				, NULL)
			);
		}
		auto path = _audioIntro != "" ? _audioIntro : SOUND_QUESTION;
		AUDIO_ENGINE->playSound(path, false, [=]() {
			this->scheduleOnce([=](float) {
				showHandGuideGame();
				}, intervalTime, "animated_guide");
			});
		if (is_auto_play) {
			m_selected_hand->setTouchEnable(false);
			m_selected_hand->_start_pos = Vec2(screen_size.width / 2, 90);
			m_selected_hand->setVisibleLayout(false, true);
			m_selected_hand->setPosition(Vec2(screen_size.width / 2, 125));
			m_selected_hand->runAction(Sequence::create(DelayTime::create(10.0f / 24), MoveTo::create(0.35f, pos), DelayTime::create(0.15f), CallFunc::create([=] {
				m_selected_hand->backToStart();
				}), NULL));
		}

	}
	else {
		std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf";
		auto gaf_hand = GAFWrapper::createGAF(gaf_guide_path);
		m_layout_root->addChild(gaf_hand, 1000000);
		gaf_hand->setPosition(pos);
		gaf_hand->play(false, [=](gaf::GAFObject * obj) {
			gaf_hand->removeFromParent();
		});
	}
}

void DeliveryFood::autoPlay()
{
	schedule([=](float) {
		if (m_is_drag_game) {
			if (!m_selected_hand) return;
			m_selected_hand->setTouchEnable(false);
			m_selected_hand->_start_pos = Vec2(screen_size.width / 2, 90);
			m_selected_hand->setVisibleLayout(false, true);
			m_selected_hand->setPosition(Vec2(screen_size.width / 2, 125));
			DeliveryFood_Window * window;
			int chose_type = cocos2d::random(0, 1);
			if (chose_type) {
				for (auto win : m_windows) {
					if (m_selected_hand->getInfo().text_phonic == win->getInfo().text_phonic) {
						window = win;
						break;
					}
				}
			}
			else {
				int index = cocos2d::random(0, (int)m_windows.size() - 1);
				window = m_windows[index];
			}

			m_selected_hand->runAction(Sequence::create(DelayTime::create(10.0f / 24), MoveTo::create(0.35f, window->getPosition()), CallFunc::create([=] {
				onMoveEndedHand(m_selected_hand);
			}), NULL));
		}
		else {
			int index = cocos2d::random(0, (int)m_windows.size() - 1);
			onClickedWindowDelegate(m_windows[index]);

		}
	}, 4.0f, 100, 5.0f, "auto_lay");

}
