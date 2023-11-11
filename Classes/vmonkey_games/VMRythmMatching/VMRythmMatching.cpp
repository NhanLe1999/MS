#include "VMRythmMatching.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"

#define RYTHM_MATCHING_CSB "csb/game/rythmmatching/VMRythmMatching.csb"
std::once_flag vm_rythm_matching_reader;
INITIALIZE_READER(VMRythmMatching);


#define rm_sound_path "sounds/rythmmatching/%s.mp3"

VMRythmMatching::VMRythmMatching()
	: game_index (0)
{
}

VMRythmMatching * VMRythmMatching::createGame(std::string json_file)
{
	std::call_once(vm_rythm_matching_reader, [] {
		REGISTER_CSB_READER(VMRythmMatching);
		REGISTER_CSB_READER(VMRythmMatching_Item);
		REGISTER_CSB_READER(VMRythmMatching_Group);
	});
	auto game = static_cast<VMRythmMatching*> (cocos2d::CSLoader::createNodeWithVisibleSize(RYTHM_MATCHING_CSB));
	if (game) {
		game->didLoadFromCSB(json_file);
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	return game;
}

void VMRythmMatching::didLoadFromCSB(std::string json_file)
{
    _isShowHand = true;
	m_configPath = json_file;
	loadFrames();
}

void VMRythmMatching::generateData()
{
    
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
    
	auto buffer_string = cocos2d::FileUtils::getInstance()->getStringFromFile(m_configPath);
	rapidjson::Document document;
	document.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

	if (document.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	// log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	// get data
	rapidjson::Value game_config = document.FindMember("game_config")->value.GetObject();
	number_turnplay = game_config.FindMember("number_turnplay")->value.GetInt();

	
	if (game_config.HasMember("instruction")) {
		auto image = math::resource::ResHelper::getImage(m_images, game_config["instruction"].GetString());
		m_instruction = image->getAudios("name_1", lang).empty() ? "" : image->getAudios("name_1", lang).front() ;
	}
	else {
		m_instruction = "sounds/rythmmatching/intro.mp3";
	}

	if(JSON_SAFE_CHECK(game_config, "game_play", Int))
	{
		m_play_type = (RM_GAME_PLAY)game_config["game_play"].GetInt();
	}
	rapidjson::Value data_config = document.FindMember("data")->value.GetArray();
	for (rapidjson::SizeType i = 0; i < data_config.Size(); ++i) {
		rapidjson::Value couple_items_config = data_config[i]["couple_of_icon"].GetArray();

		RYTHM_GAME_INFO game_info;
		for (rapidjson::SizeType j = 0; j < couple_items_config.Size(); ++j) {
			rapidjson::Value pair_config = couple_items_config[j].GetObject();

			// icon 1
			std::string icon1 = pair_config.FindMember("icon_1")->value.GetString();
			auto image1 = math::resource::ResHelper::getImage(m_images, icon1);
			VMRythmMatching_Item::RM_ITEM_INFO item1;
			item1.image = image1->path;
			item1.text = image1->getText("name_1");
			if (image1->getAudios("name_1", lang).size() > 0)
				item1.audio = image1->getAudios("name_1", lang).front();
			item1.pair = image1->getComparisionValue("pair");
			// icon 2
			std::string icon2 = pair_config.FindMember("icon_2")->value.GetString();
			auto image2 = math::resource::ResHelper::getImage(m_images, icon2);
			VMRythmMatching_Item::RM_ITEM_INFO item2;
			item2.image = image2->path;
			item2.text = image2->getText("name_1", lang);
			if(image2->getAudios("name_1", lang).size() > 0)
				item2.audio = image2->getAudios("name_1", lang).front();
			item2.pair = image2->getComparisionValue("pair");

			if (item1.pair == -1 || item2.pair == -1) {
				item1.pair = item2.pair = 112169 + j;
			}
			//
			game_info.pair_items.push_back(std::make_pair(item1,item2));
		}

		list_game_info.push_back(game_info);
	}

	number_turnplay = std::min(number_turnplay,(int) list_game_info.size());
}

void VMRythmMatching::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();

	this->setContentSize(screen_size);
	auto bg_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "bg_layout");
	root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "root_layout");

	//bg_layout->setBackGroundColor(Color3B(125,219,245),Color3B(100,208,237));
	//bg_layout->setBackGroundColorOpacity(255);
	//bg_layout->setBackGroundColorType(BackGroundColorType::GRADIENT);

	cocos2d::utils::findChild(bg_layout, "rm_bg0")->setLocalZOrder(0);
	cocos2d::utils::findChild(bg_layout, "rm_bg3")->setLocalZOrder(3);
	cocos2d::utils::findChild(bg_layout, "rm_bg3_o")->setLocalZOrder(3);
	cocos2d::utils::findChild(bg_layout, "rm_bg5")->setLocalZOrder(5);
	cocos2d::utils::findChild(bg_layout, "rm_bg7")->setLocalZOrder(7);
	// add gaf to background
	auto gaf_bg1 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg1/rm_bg1.gaf");
	gaf_bg1->setPosition(screen_size / 2);
	bg_layout->addChild(gaf_bg1,1);
	gaf_bg1->play(true);

	auto gaf_bg2 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg2/rm_bg2.gaf");
	gaf_bg2->setPosition(Vec2(screen_size.width/2 + 100, screen_size.height/2 + 150));
	bg_layout->addChild(gaf_bg2,2);
	gaf_bg2->play(true);

	auto gaf_bg4 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg4/rm_bg4.gaf");
	gaf_bg4->setPosition(Vec2(screen_size.width/2 - 615, screen_size.height/2 + 100));
	bg_layout->addChild(gaf_bg4,4);
	gaf_bg4->play(true);
	gaf_bg4->setRotation(-5);

	auto gaf_bg6_house1 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg6_house1/rm_bg6_house1.gaf");
	gaf_bg6_house1->setPosition(Vec2(screen_size.width/2 - 500, screen_size.height/2 + 50));
	bg_layout->addChild(gaf_bg6_house1,6,"gaf_house_2");
	//gaf_bg6_house1->play(false);
	gaf_bg6_house1->setRotation(30);
	gaf_house1 = gaf_bg6_house1;

	auto gaf_bg6_house2 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg6_house2/rm_bg6_house2.gaf");
	gaf_bg6_house2->setPosition(Vec2(screen_size.width/2 + 350 , screen_size.height/2 -100));
	bg_layout->addChild(gaf_bg6_house2,6,"gaf_house_2");
	//gaf_bg6_house2->play(false);
	gaf_bg6_house2->setRotation(-30);
	gaf_house2 = gaf_bg6_house2;

	auto fill_layout = cocos2d::ui::Layout::create();
	fill_layout->setContentSize(screen_size);
	fill_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	fill_layout->setBackGroundColorOpacity(125);
	fill_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	bg_layout->addChild(fill_layout, 7);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	touch_layout->setBackGroundColor(cocos2d::Color3B::BLACK);
	touch_layout->setBackGroundColorOpacity(0);
	touch_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	this->addChild(touch_layout, 1000);

	// add speaker
	auto speaker = ui::Button::create("games/rythmmatching/loa icon.png");
	speaker->setPosition(Vec2(40, screen_size.height - 40));
	speaker->addClickEventListener([=](cocos2d::Ref * sender) {
		hideGuideGame();
        AUDIO_ENGINE->stopEffect(_audioGuiding);
		_audioGuiding = AUDIO_ENGINE->playEffect(m_instruction);
	});
	this->addChild(speaker, 10);
	
}
void VMRythmMatching::onEnter()
{
	MJMainGBase::onEnter();
	AUDIO_ENGINE->playBackgroundMusic(StringUtils::format(rm_sound_path,"rm_bgm"));
	generateData();
	showGamename();
	//prepareGame();
}

void VMRythmMatching::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void VMRythmMatching::onFinishGame()
{
	this->unscheduleAllCallbacks();
	this->stopAllActions();
	AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onFinishGameVMonkey();
}


void VMRythmMatching::showGamename()
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	auto imageName = cocos2d::ui::ImageView::create("games/rythmmatching/Word_Factory.png");
	imageName->setPosition(screen_size / 2);
	root_layout->addChild(imageName);
	imageName->runAction(Sequence::create(
		FadeIn::create(0.5f),
		CallFunc::create([=]() {
			cocos2d::experimental::AudioEngine::play2d("sounds/rythmmatching/Word_Factory.mp3");
		}),
		DelayTime::create(1.5f),
			FadeOut::create(0.25f),
			CallFunc::create([=] {
		prepareGame();
	}), NULL));
}

void VMRythmMatching::prepareGame()
{

	if (game_index >= list_game_info.size()) {
		onFinishGame();
	}
	disableTouch();
	current_game_info = list_game_info[game_index];
	mj::helper::shuffle(current_game_info.pair_items);
	if (current_game_info.pair_items.size() > 4) current_game_info.pair_items.resize(4);
	remain_item = current_game_info.pair_items.size();
	rm_groups.clear();
	rm_drag_items.clear();
	
	VMRythmMatching_Item::RM_ITEM_TYPE type1;
	VMRythmMatching_Item::RM_ITEM_TYPE type2;

	switch (m_play_type)
	{
	case VMRythmMatching::RM_IMAGE_IMAGE:
		type1 = VMRythmMatching_Item::RMI_IMAGE;
		type2 = VMRythmMatching_Item::RMI_IMAGE;
		break;
	case VMRythmMatching::RM_IMAGE_TEXT:
		type1 = VMRythmMatching_Item::RMI_IMAGE;
		type2 = VMRythmMatching_Item::RMI_TEXT;
		break;
	case VMRythmMatching::RM_TEXT_IMAGE:
		type1 = VMRythmMatching_Item::RMI_TEXT;
		type2 = VMRythmMatching_Item::RMI_IMAGE;
		break;
	case VMRythmMatching::RM_TEXT_TEXT:
		type1 = VMRythmMatching_Item::RMI_TEXT;
		type2 = VMRythmMatching_Item::RMI_TEXT;
		break;
	case VMRythmMatching::RM_BOTH:
		type1 = VMRythmMatching_Item::RMI_IMAGE_TEXT;
		type2 = VMRythmMatching_Item::RMI_IMAGE_TEXT;
		break;
	default:
		break;
	}

	for (auto info : current_game_info.pair_items) {
		auto item1 = VMRythmMatching_Item::createItem(info.first,type1,VMRythmMatching_Item::RMI_BG_WHITE,VMRythmMatching_Item::RMI_CLICK);
		item1->setDelegate(this);
		auto item2 = VMRythmMatching_Item::createItem(info.second,type2,VMRythmMatching_Item::RMI_BG_DASH);
		auto group = VMRythmMatching_Group::createGroup(item1, item2);
		group->retain();
		rm_groups.push_back(group);

		auto drag_item = VMRythmMatching_Item::createItem(info.second,type2, VMRythmMatching_Item::RMI_BG_WHITE,VMRythmMatching_Item::RMI_TOUCH);
		drag_item->setDelegate(this);
		drag_item->retain();
		rm_drag_items.push_back(drag_item);
	}
	showGroups();
}

void VMRythmMatching::showGroups()
{
	float padding = 0.0f;
	int size = rm_groups.size();
	float width = size * 250.0f;
	if (width < screen_size.width) {
		padding = (screen_size.width - width) / (size + 2);
		if (padding > 100) padding = 100;
	}
	auto locations = getFrameLocations(rm_groups.size(), cocos2d::Vec2(screen_size.width / 2, screen_size.height - 250), Size(250, 500),padding);
	int index = 0;
	for (auto group : rm_groups) {
		group->setPosition(locations[index]);
		root_layout->addChild(group);
        group->release();
		group->onShow(index * 0.1f);
		group->onShowDash(rm_groups.size() * 0.1f + 0.5);
		++index;
	}
	this->scheduleOnce([=](float dt) {
		showDragItems();
	},index * 0.1f + 1.0f,"show drag items");
}

void VMRythmMatching::showDragItems()
{
	float padding = 0.0f;
	int size = rm_groups.size();
	float width = size * 250.0f;
	if (width < screen_size.width) {
		padding = (screen_size.width - width) / (size + 2);
		if (padding > 100) padding = 100;
	}
	auto locations = getFrameLocations(rm_groups.size(), cocos2d::Vec2(screen_size.width / 2, 125), Size(250, 250),padding);
	int index = 0;
	mj::helper::shuffle(rm_drag_items);
	for (auto item : rm_drag_items) {
		item->setPosition(locations[index]);
		root_layout->addChild(item,1);
        item->release();
		item->onShow(index * 0.15f);
		++index;
	}	
	showIntroGame();
}

void VMRythmMatching::showIntroGame()
{
	if (game_index == 0) {
		AUDIO_ENGINE->playSound(m_instruction ,false, [=] (){
			startGame();
		});
	}
	else {
		startGame();
	}

}
void VMRythmMatching::startGame()
{
	showGuideGame();
	enableTouch();
	
}

void VMRythmMatching::onNextGame()
{
	if (remain_item == 0) {
		++game_index;
		AUDIO_ENGINE->stopSound(m_instruction);
		if (game_index >= list_game_info.size()) {
			onFinishGame();
		}
		else {
			prepareGame();
		}
	}
}


void VMRythmMatching::enableTouch()
{
	touch_layout->setVisible(false);
}

void VMRythmMatching::disableTouch(int bg_opacity)
{
	touch_layout->setVisible(true);
	touch_layout->setBackGroundColorOpacity(bg_opacity);
}

void VMRythmMatching::onTouchItemBegan(cocos2d::Ref * sender)
{
	hideGuideGame();
	auto item = static_cast<VMRythmMatching_Item *>(sender);
	if (!_isSpeaking) {
		_isSpeaking = true;
		auto audio_id = cocos2d::experimental::AudioEngine::play2d(item->getInfo().audio);
		cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
			_isSpeaking = false;
			});
		AUDIO_ENGINE->play(StringUtils::format(rm_sound_path, "rm_interact"));
	}
	item->setLocalZOrder(2);
}

void VMRythmMatching::onTouchItemMoved(cocos2d::Ref * sender)
{
	auto item = static_cast<VMRythmMatching_Item *>(sender);
	for (auto group : rm_groups) {
		if (group) {
			group->onDetected(item->getPosition()+ helpChildPlayEasier(item->getPosition(), group->getPosition()));
		}
	}
}

void VMRythmMatching::onTouchItemEnded(cocos2d::Ref * sender)
{
	showGuideGame();
	auto item = static_cast<VMRythmMatching_Item *>(sender);
	item->setLocalZOrder(1);
	for (auto group : rm_groups) {
		if (group && !group->isPaired() && group->isDetected(item->getPosition() + helpChildPlayEasier(item->getPosition(), group->getPosition()))) {
            _isShowHand = false;
			onPairing(group, item);
			_guideshowed = true;
			return;
		}
	}
	if (++_countWrongClick >3) {
		showHand();
	}
	item->backToStart();
}

void VMRythmMatching::onPairing(VMRythmMatching_Group * group, VMRythmMatching_Item * item)
{
	if (item->getInfo().pair == group->getItem1()->getInfo().pair) {
		group->getItem2()->setInfo(item->getInfo());
		gaf_house1->play(false);
		gaf_house2->play(false);
		
		auto pos = group->getPosition() + Vec2(0, -125);
		group->onDetected(Vec2(-10000, -10000));
		item->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.35f, pos)), CallFunc::create([=] {
			//AUDIO_ENGINE->playEffect("sounds/rythmmatching/Good job.mp3");
			group->onPairItem();
			group->onHide(1.0f);
			std::string pull_sfx = "rm_pull2";//cocos2d::random(0, 1) == 0 ? "rm_pull1" : "rm_pull2";
			AUDIO_ENGINE->play(StringUtils::format(rm_sound_path, pull_sfx.c_str()));
		}), RemoveSelf::create(), NULL));

		AUDIO_ENGINE->play(StringUtils::format(rm_sound_path, "rm_correct"));
		auto effect_light = GAFWrapper::createGAF("gaf/rythmmatching/rm_lighteffect/rm_lighteffect.gaf");
		effect_light->setPosition(pos + Vec2(300,-300));
		effect_light->play(false);
		root_layout->addChild(effect_light, -1);
		effect_light->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), NULL));
		--remain_item;

		AUDIO_ENGINE->play(StringUtils::format(rm_sound_path, "rm_workingsfx"));
		unschedule("onNextGame");
		this->scheduleOnce([=](float dt) {
			onNextGame();
		},2.25f,"onNextGame");
	}
	else {
		AUDIO_ENGINE->playEffect("sounds/rythmmatching/wrong.mp3");
		group->onDetected(Vec2(-10000, -10000));
		group->getItem2()->onItemWrong();
		item->backToStart();
	}

}


std::vector<Vec2> VMRythmMatching::getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding)
{
	std::vector<Vec2> locations;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(center.x - (number - 1) * (size.width + padding) / 2, center.y);
	for (int i = 0; i < number; ++i) {
		Vec2 pos(start_pos.x + i * (size.width + padding), start_pos.y);
		locations.push_back(pos);

	}
	return locations;
}

void VMRythmMatching::showGuideGame() {
	this->unschedule("show_guide_rhythm_machine");
	this->scheduleOnce([=](float) {
		if (!_guideshowed) {
			showHand();
		}
		_audioGuiding = cocos2d::experimental::AudioEngine::play2d(m_instruction);
		cocos2d::experimental::AudioEngine::setFinishCallback(_audioGuiding, [=](int, std::string) {
				showGuideGame();
			});

		}, 10, "show_guide_rhythm_machine");
}

void VMRythmMatching::hideGuideGame() {
	this->unschedule("show_guide_rhythm_machine");
	if (_audioGuiding != -1) {
		cocos2d::experimental::AudioEngine::stop(_audioGuiding);
	}
	Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
}
Vec2 VMRythmMatching::helpChildPlayEasier(Point p1, Point p2) {
	if (p1.y > p2.y)
		return Vec2(0, -80);
	else 
		return Vec2(0, 80);
}

void VMRythmMatching::showHand() {
    if(!_isShowHand)
    {
        return;
    }
	_countWrongClick = 0;
	auto locations = getFrameLocations(rm_groups.size(), cocos2d::Vec2(screen_size.width / 2, 125), Size(250, 250), 0.0f);
	int index = 0;
	Point _start;
	Point _stop;
    auto rmDragItems = rm_drag_items.at(0);
	for (auto group : rm_groups) {
        if(rmDragItems)
        {
            if (rmDragItems->getInfo().pair == group->getItem1()->getInfo().pair) {
                //auto cc = rmDragItems->_touch_layout;
                _start = rmDragItems->getPosition();
                _stop = locations[index] + Vec2(0, 250);
            }
            ++index;
        }
	}
	//item->getInfo().pair == group->getItem1()->getInfo().pair
	//auto item : rm_drag_items
	Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

	auto overlay = ui::Layout::create();
	overlay->setContentSize(Director::getInstance()->getVisibleSize());
	overlay->setName("guide_overlay");
	Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);

	auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
	overlay->addChild(hand, INT_MAX - 1);
	hand->setPosition(_start - Point(0, 25));

	hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, _stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}