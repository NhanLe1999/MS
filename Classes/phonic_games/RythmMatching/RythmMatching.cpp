#include "RythmMatching.h"


#define RYTHM_MATCHING_CSB "csb/game/rythmmatching/RythmMatching.csb"
std::once_flag rythm_matching_reader;
INITIALIZE_READER(RythmMatching);


#define rm_sound_path "sounds/rythmmatching/%s.mp3"

RythmMatching::RythmMatching()
	: game_index (0)
{
}

RythmMatching * RythmMatching::createGame(std::string json_file)
{
	std::call_once(rythm_matching_reader, [] {
		REGISTER_CSB_READER(RythmMatching);
		REGISTER_CSB_READER(RythmMatching_Item);
		REGISTER_CSB_READER(RythmMatching_Group);
	});
	auto game = static_cast<RythmMatching*> (cocos2d::CSLoader::createNodeWithVisibleSize(RYTHM_MATCHING_CSB));
	if (game) {
		game->didLoadFromCSB(json_file);
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	return game;
}

void RythmMatching::didLoadFromCSB(std::string json_file)
{
	m_configPath = json_file;
	loadFrames();
}

void RythmMatching::generateData()
{
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
	rapidjson::Value data_config = document.FindMember("data")->value.GetArray();
	for (rapidjson::SizeType i = 0; i < data_config.Size(); ++i) {
		rapidjson::Value couple_items_config = data_config[i]["couple_of_icon"].GetArray();

		RYTHM_GAME_INFO game_info;
		for (rapidjson::SizeType j = 0; j < couple_items_config.Size(); ++j) {
			rapidjson::Value pair_config = couple_items_config[j].GetObject();

			// icon 1
			std::string icon1 = pair_config.FindMember("icon_1")->value.GetString();
			auto image1 = math::resource::ResHelper::getImage(m_images, icon1);
			RythmMatching_Item::RM_ITEM_INFO item1;
			item1.image = image1->path;
			item1.text = image1->getText("name_1");
			item1.audio = image1->getAudios("name_1").front();			
			item1.pair = image1->getComparisionValue("pair");
			// icon 2
			std::string icon2 = pair_config.FindMember("icon_2")->value.GetString();
			auto image2 = math::resource::ResHelper::getImage(m_images, icon2);
			RythmMatching_Item::RM_ITEM_INFO item2;
			item2.image = image2->path;
			item2.text = image2->getText("name_1");
			item2.audio = image2->getAudios("name_1").front();
			item2.pair = image2->getComparisionValue("pair");

			//
			game_info.pair_items.push_back(std::make_pair(item1,item2));
		}

		list_game_info.push_back(game_info);
	}

	number_turnplay = std::min(number_turnplay,(int) list_game_info.size());
}

void RythmMatching::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();

	this->setContentSize(screen_size);
	auto bg_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "bg_layout");
	root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "root_layout");

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
	gaf_bg2->setPosition(Vec2(screen_size.width/2 + 100,screen_size.height/2 + 150));
	bg_layout->addChild(gaf_bg2,2);
	gaf_bg2->play(true);

	auto gaf_bg4 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg4/rm_bg4.gaf");
	gaf_bg4->setPosition(Vec2(screen_size.width/2 - 615,screen_size.height/2 + 100));
	bg_layout->addChild(gaf_bg4,4);
	gaf_bg4->play(true);
	gaf_bg4->setRotation(-5);

	auto gaf_bg6_house1 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg6_house1/rm_bg6_house1.gaf");
	gaf_bg6_house1->setPosition(Vec2(screen_size.width/2 - 500,screen_size.height/2 + 50));
	bg_layout->addChild(gaf_bg6_house1,6,"gaf_house_2");
	//gaf_bg6_house1->play(false);
	gaf_bg6_house1->setRotation(30);
	gaf_house1 = gaf_bg6_house1;

	auto gaf_bg6_house2 = GAFWrapper::createGAF("gaf/rythmmatching/rm_bg6_house2/rm_bg6_house2.gaf");
	gaf_bg6_house2->setPosition(Vec2(screen_size.width/2 + 350 ,screen_size.height/2 -100));
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
    
    // speaker
    m_touchable = false;
    auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    speaker->setPosition(Vec2(70,Director::getInstance()->getWinSize().height - 60));
    speaker->addClickEventListener([=](cocos2d::Ref* sender){
        if(m_touchable)
            AUDIO_ENGINE->playSound("sounds/rythmmatching/intro.mp3");
    });
    this->addChild(speaker);
	
}
void RythmMatching::onEnter()
{
	MJMainGBase::onEnter();
	playBackgroundMusic(StringUtils::format(rm_sound_path,"rm_bgm"));
	generateData();
	//showGamename();
	prepareGame();
}

void RythmMatching::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void RythmMatching::onFinishGame()
{
	MJMainGBase::onFinishGame();
}


void RythmMatching::showGamename()
{
	auto game_name = cocos2d::ui::Text::create("Rythm Matching", font_name, 100);
	game_name->setPosition(screen_size / 2);
	root_layout->addChild(game_name);
	game_name->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(0.5f), FadeOut::create(0.25f), CallFunc::create([=] {
		prepareGame();
	}), NULL));
}

void RythmMatching::prepareGame()
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
	
	for (auto info : current_game_info.pair_items) {
		auto item1 = RythmMatching_Item::createItem(info.first,RythmMatching_Item::RMI_BG_YELLOW,RythmMatching_Item::RMI_CLICK);
		item1->setDelegate(this);
		auto item2 = RythmMatching_Item::createItem(info.second,RythmMatching_Item::RMI_BG_DASH);
		auto group = RythmMatching_Group::createGroup(item1, item2);
		group->retain();
		rm_groups.push_back(group);

		auto drag_item = RythmMatching_Item::createItem(info.second, RythmMatching_Item::RMI_BG_WHITE,RythmMatching_Item::RMI_TOUCH);
		drag_item->setDelegate(this);
		drag_item->retain();
		rm_drag_items.push_back(drag_item);
	}
	showGroups();
}

void RythmMatching::showGroups()
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

void RythmMatching::showDragItems()
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

void RythmMatching::showIntroGame()
{
	if (game_index == 0) {
		AUDIO_ENGINE->playSound("sounds/rythmmatching/intro.mp3", false, [=] {
			startGame();
		});
	}
	else {
		startGame();
	}

}
void RythmMatching::startGame()
{
    m_touchable = true;
	enableTouch();
	
}

void RythmMatching::onNextGame()
{
	if (remain_item == 0) {
		++game_index;
		if (game_index >= list_game_info.size()) {
			onFinishGame();
		}
		else {
			prepareGame();
		}
	}
}


void RythmMatching::enableTouch()
{
	touch_layout->setVisible(false);
}

void RythmMatching::disableTouch(int bg_opacity)
{
	touch_layout->setVisible(true);
	touch_layout->setBackGroundColorOpacity(bg_opacity);
}

void RythmMatching::onTouchItemBegan(cocos2d::Ref * sender)
{
	auto item = static_cast<RythmMatching_Item *>(sender);
	AUDIO_ENGINE->playEffect(item->getInfo().audio);
	AUDIO_ENGINE->play(StringUtils::format(rm_sound_path, "rm_interact"));
	item->setLocalZOrder(2);
}

void RythmMatching::onTouchItemMoved(cocos2d::Ref * sender)
{
	auto item = static_cast<RythmMatching_Item *>(sender);
	for (auto group : rm_groups) {
		if (group) {
			group->onDetected(item->getPosition());
		}
	}
}

void RythmMatching::onTouchItemEnded(cocos2d::Ref * sender)
{
	auto item = static_cast<RythmMatching_Item *>(sender);
	item->setLocalZOrder(1);
	for (auto group : rm_groups) {
		if (group && !group->isPaired() && group->isDetected(item->getPosition())) {
			onPairing(group, item);
			return;
		}
	}
	item->backToStart();
}

void RythmMatching::onPairing(RythmMatching_Group * group, RythmMatching_Item * item)
{
	if (item->getInfo().pair == group->getItem1()->getInfo().pair) {
		group->getItem2()->setInfo(item->getInfo());
		gaf_house1->play(false);
		gaf_house2->play(false);
		
		auto pos = group->getPosition() + Vec2(0, -125);
		group->onDetected(Vec2(-10000, -10000));
		item->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.35f, pos)), CallFunc::create([=] {
			AUDIO_ENGINE->playEffect("sounds/rythmmatching/Good job.mp3");
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
		gaf_house1->play(false);
		gaf_house2->play(false);

		unschedule("onNextGame");
		this->scheduleOnce([=](float dt) {
			onNextGame();
		},2.25f,"onNextGame");
	}
	else {
		AUDIO_ENGINE->playEffect("sounds/rythmmatching/Oops.mp3");
		group->onDetected(Vec2(-10000, -10000));
		group->getItem2()->onItemWrong();
		item->backToStart();
	}

}


std::vector<Vec2> RythmMatching::getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding)
{
	std::vector<Vec2> locations;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(center.x - (number - 1) * (size.width + padding) / 2, center.y);
	for (int i = 0; i < number; ++i) {
		Vec2 pos(start_pos.x + i * (size.width + padding), start_pos.y);
		locations.push_back(pos);

	}
	return locations;
}
