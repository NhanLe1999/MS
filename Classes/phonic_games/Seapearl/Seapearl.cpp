//
// Seapearl.cpp
// Created by Minhtzy on 19/11/2018
//


#include "Seapearl.h"
#include "JsonDump.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"

#define speed_car 20000
using namespace cocos2d;
using namespace rapidjson;

Seapearl * Seapearl::createGame(std::string json_file) {
	CCLOG(__FUNCTION__);
	Seapearl * obj = new Seapearl();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(json_file);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void Seapearl::didLoadFromCSB(std::string json_file) {
	CCLOG(__FUNCTION__);
	m_configPath = json_file;
	loadFrames();
}

void Seapearl::generateData() {
	CCLOG(__FUNCTION__);
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_configPath);
	rapidjson::Document value_data;
	bool is_convert_plist_2_json = false;
	if (is_convert_plist_2_json) {
		auto value_map = FileUtils::getInstance()->getValueMapFromFile(m_configPath);
		buffer_string = JsonDump::getInstance()->MapToString(value_map);
	}

	value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
	if (value_data.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	// log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	value_data.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	auto game_config = value_data.FindMember("game_config")->value.GetObject();
	hide_text = JSON_SAFE_CHECK(game_config, "hide_text", Bool) ? game_config["hide_text"].GetBool() : false;
	if (JSON_SAFE_CHECK(game_config, "custom_intro_audio", String))
	{
		auto audioIntro = game_config["custom_intro_audio"].GetString();
		auto imageFull = math::resource::ResHelper::getImage(m_images, audioIntro);
		_introAudio = imageFull->getAudios().front();
		
	}

	if (game_config.HasMember("number_turnplay") && game_config["number_turnplay"].IsInt()) {
		number_turnplay = game_config["number_turnplay"].GetInt();
		if (JSON_SAFE_CHECK(game_config, "read_vowel", Bool)) {
			_readVowel = game_config["read_vowel"].GetBool();
		}
	}

	if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
		auto list_item_data = value_data["list_item"].GetArray();
		for (int i = 0; i < (int)list_item_data.Size(); i++) {
			auto item_data = list_item_data[i].GetObject();
			SEAPEARL_INFO item_info;
			
			auto image_id = item_data["text"].GetString();
			auto image = math::resource::ResHelper::getImage(m_images, image_id);
			item_info.text = image->getText("name_1");
			item_info.audio_path_slow = image->getAudios("name_n")[0];
			item_info.audio_path_normal = image->getAudios("name_1")[0];
			item_info.image_path = image->path;

			auto list_charater_text = image->getTextPhonic("name_p");
			auto list_charater_audio = image->getAudioPhonic("name_p");		
				for (int j = 0; j < (int)list_charater_text.size(); j++) {
					CHARACTER_INFO character_info;
					character_info.character = list_charater_text[j];
					if (_readVowel) {
						character_info.audio_path = list_charater_audio[j];
					}
					item_info.list_character.push_back(character_info);
				}
			genCharacterIndex(item_info);
			auto list_distract_info = item_data["list_distracting"].GetArray();
			for (int j = 0; j < (int)list_distract_info.Size(); j++) {
				auto distract_id = list_distract_info[j].GetString();
				auto distrac_image = math::resource::ResHelper::getImage(m_images, distract_id);
				CHARACTER_INFO character_info;
				character_info.character = distrac_image->getText("name_1");
				character_info.audio_path = distrac_image->getAudios("name_1")[0];
				item_info.list_distracting.push_back(character_info);
			}
			list_item_info.push_back(item_info);
		}
	}
}

void Seapearl::onEnter() {
	CCLOG(__FUNCTION__);
	MJMainGBase::onEnter();
	generateData();
	this->showIntroGame();
	this->playBackgroundMusic("sounds/seapearl/sp_bgm.mp3", 1);
}

void Seapearl::onSpeakerClicked(cocos2d::Ref *sender) {
	speaker->setTouchEnabled(false);
	_isSpeaking = true;
	AUDIO_ENGINE->playSound(current_item_info.audio_path_normal, false, [=]() {
		speaker->setTouchEnabled(true);
	});
}

void Seapearl::onExitTransitionDidStar() {
	this->unschedule("inActiveMode");
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
	CCLOG(__FUNCTION__);
	MJMainGBase::onExitTransitionDidStart();
}

void Seapearl::loadFrames() {
	CCLOG(__FUNCTION__);
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	// Layout
	m_bg_layout = cocos2d::ui::Layout::create();
	m_bg_layout->setContentSize(screen_size);
	this->addChild(m_bg_layout,4);

	m_root_layout = cocos2d::ui::Layout::create();
	m_root_layout->setContentSize(screen_size);
	this->addChild(m_root_layout,5);

	gachngang_layout = cocos2d::ui::Layout::create();
	this->addChild(gachngang_layout, 6);
	gachngang_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	gachngang_layout->setPosition(Vec2(screen_size.width / 2, screen_size.height - 120));

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	this->addChild(touch_layout, 7);
	touch_layout->setBackGroundColor(Color3B::BLACK);
	touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	touch_layout->setOpacity(0);
	touch_layout->setTouchEnabled(true);
	touch_layout->setVisible(false);

	// 

	auto bg = cocos2d::ui::ImageView::create(StringUtils::format("games/seapearl/sp_bg.png"));
	bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	bg->setPosition(Vec2(screen_size/ 2));
	this->addChild(bg, 0, "bg");
	
	auto l2_filter = cocos2d::ui::ImageView::create("games/seapearl/sp_l2_filter.png");
	l2_filter->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	l2_filter->setPosition(Vec2(screen_size / 2));
	this->addChild(l2_filter,2, "l2_filter");
	
	auto l4_ship = cocos2d::ui::ImageView::create("games/seapearl/sp_l4_ship.png");
	l4_ship->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	l4_ship->setPosition(Vec2(screen_size.width / 2,0));
	m_bg_layout->addChild(l4_ship, 4, "l4_ship");

	auto l5_mountain = cocos2d::ui::ImageView::create("games/seapearl/sp_l5_mountain.png");
	l5_mountain->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	l5_mountain->setPosition(Vec2(screen_size.width / 2,0));
	m_bg_layout->addChild(l5_mountain, 5, "l5_mountain");

	auto l6_silhouette = cocos2d::ui::ImageView::create("games/seapearl/sp_l6_silhouette.png");
	l6_silhouette->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	l6_silhouette->setPosition(Vec2(screen_size.width / 2,0));
	m_bg_layout->addChild(l6_silhouette, 6, "l6_silhouette");


	auto gaf_l1_sand = GAFWrapper::createGAF("gaf/seapearl/sp_l1_sand/sp_l1_sand.gaf");
	gaf_l1_sand->setPosition(Vec2(screen_size.width / 2,120));
	gaf_l1_sand->play(true);
	m_bg_layout->addChild(gaf_l1_sand, 10, "gaf_l1_sand");

	auto gaf_l2_seaweed = GAFWrapper::createGAF("gaf/seapearl/sp_l2_seaweed/sp_l2_seaweed.gaf");
	gaf_l2_seaweed->setPosition(Vec2(screen_size.width / 2,300));
	gaf_l2_seaweed->play(true);
	m_bg_layout->addChild(gaf_l2_seaweed, 11, "gaf_l2_seaweed");

	
	int segment = 5;
	auto segment_width = screen_size.width / (segment+1);
	for (int i = 1; i <= segment; ++i) {
		auto gaf_bubble = GAFWrapper::createGAF("gaf/seapearl/sp_bubble/sp_bubble.gaf");
		gaf_bubble->setPosition(Vec2(i* segment_width, -150));
		gaf_bubble->runAction(Repeat::create(Sequence::create(DelayTime::create(cocos2d::random(0.0f, 4.0f)), CallFunc::create([=]{
			gaf_bubble->play(false);
		}),DelayTime::create(128.0f/32), NULL), 100000000));
		m_bg_layout->addChild(gaf_bubble, 9, "gaf_sp_bubble");
	}

	gaf_clam = GAFWrapper::createGAF("gaf/seapearl/sp_clam/sp_clam.gaf");
	gaf_clam->setPosition(Vec2(screen_size.width / 2, 200));
	gaf_clam->playSequence("close");
	m_bg_layout->addChild(gaf_clam, 20, "gaf_sp_clam");

	text_lb = cocos2d::ui::Text::create("", font_name, 100);
	this->addChild(text_lb, 102);
	text_lb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	text_lb->setTextColor(cocos2d::Color4B::BLACK);
	text_lb->enableOutline(cocos2d::Color4B::WHITE, 5);


	 _star1 = GAFWrapper::createGAF("gaf/seapearl/sp_peffect/Rc_peffect2.gaf");
	 _star1->setAnchorPoint(Vec2(0.5, 0.5));
	m_bg_layout->addChild(_star1,21);
	_star1->setScale(0.5);
	_star1->setPosition(Vec2(screen_size.width / 4, screen_size.height/4));

	_star2 = GAFWrapper::createGAF("gaf/seapearl/sp_peffect/Rc_peffect2.gaf");
	_star2->setAnchorPoint(Vec2(0.5, 0.5));
	m_bg_layout->addChild(_star2, 21);
	_star2->setScale(0.5);
	_star2->setPosition(Vec2(3*screen_size.width / 4, screen_size.height / 4));

	_star3 = GAFWrapper::createGAF("gaf/seapearl/sp_peffect/Rc_peffect2.gaf");
	_star3->setAnchorPoint(Vec2(0.5, 0.5));
	m_bg_layout->addChild(_star3, 21);
	_star3->setScale(0.5);
	_star3->setPosition(Vec2( screen_size.width / 4,  3*screen_size.height/4));

	_star4 = GAFWrapper::createGAF("gaf/seapearl/sp_peffect/Rc_peffect2.gaf");
	_star4->setAnchorPoint(Vec2(0.5, 0.5));
	m_bg_layout->addChild(_star4, 21);
	_star4->setScale(0.5);
	_star4->setPosition(Vec2( 3*screen_size.width /4, 3 * screen_size.height / 4));

	img_bgwin = ui::ImageView::create("games/seapearl/sp_bgwin.png");
	img_bgwin->setScale9Enabled(true);
	img_bgwin->setContentSize(screen_size);
	img_bgwin->setPosition(screen_size / 2);
	img_bgwin->setVisible(false);
	m_bg_layout->addChild(img_bgwin, 20);

	img_character = ui::ImageView::create();
	img_character->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	img_character->setVisible(false);
	m_bg_layout->addChild(img_character, 26);
	_imgBegin = ui::ImageView::create();
	_imgBegin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_imgBegin->setVisible(false);
	m_bg_layout->addChild(_imgBegin, 8888);
	_txtBegin = ui::Text::create(" ", font_name, 200);
	_txtBegin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_txtBegin->setTextColor(cocos2d::Color4B::BLACK);
	_imgBegin->setVisible(false);
	m_root_layout->addChild(_txtBegin,8888);

	//gaf_bgwin = GAFWrapper::createGAF("gaf/seapearl/sp_bgwin/sp_bgwin.gaf");
	//gaf_bgwin->setPosition(screen_size / 2);
	//gaf_bgwin->setVisible(false);
	//m_bg_layout->addChild(gaf_bgwin, 21);
    if (mj::helper::isRetina()){
        fake_gaf_bgwin = ui::ImageView::create("gaf/seapearl/sp_bgwin/sp_bgwin_hdr_2.png");
        fake_gaf_bgwin_2 = ui::ImageView::create("gaf/seapearl/sp_bgwin/sp_bgwin_hdr_3.png");
    }else{
        fake_gaf_bgwin = ui::ImageView::create("gaf/seapearl/sp_bgwin/sp_bgwin_hd_2.png");
        fake_gaf_bgwin_2 = ui::ImageView::create("gaf/seapearl/sp_bgwin/sp_bgwin_hd_3.png");
    }
   
    fake_gaf_bgwin->setPosition(screen_size/2);
    fake_gaf_bgwin->setVisible(false);
    fake_gaf_bgwin->addChild(fake_gaf_bgwin_2);
    fake_gaf_bgwin_2->setPosition(fake_gaf_bgwin->getContentSize()/2);
    m_bg_layout->addChild(fake_gaf_bgwin,21);
    fake_gaf_bgwin->setScale(1.5);

	speaker = ui::Button::create("games/seapearl/loa icon.png");
	speaker->setPosition(Vec2(50, screen_size.height - 45));
	speaker->addClickEventListener(CC_CALLBACK_1(Seapearl::onSpeakerClicked,this));
	speaker->setVisible(false);
	this->addChild(speaker,6);
	
	AUDIO_ENGINE->playSound("sounds/seapearl/Sea Pearl.mp3");
}

void Seapearl::showIntroGame() {
	CCLOG(__FUNCTION__);
	// game's name is here
	auto gamename = cocos2d::ui::ImageView::create("games/seapearl/Sea Pearl.png");
	gamename->setPosition(screen_size / 2);
	m_root_layout->addChild(gamename);

	m_bg_layout->setPositionY(-screen_size.height);
	m_bg_layout->runAction(Sequence::create(DelayTime::create(0.5f),MoveTo::create(2.0f,Vec2::ZERO),NULL));
	gamename->setOpacity(0);
	gamename->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([=] {
		startGame();
	}), RemoveSelf::create(), NULL));
}

void Seapearl::startGame()
{

	auto jsonString = FileUtils::getInstance()->getStringFromFile("config.json");
	Document resultDocument;
	resultDocument.Parse(jsonString.c_str());

	auto game_config = resultDocument.FindMember("game_config")->value.GetObject();

	
	playClam(2, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		playClam(1);
	});
	//intro
    disableTouch();
	std::vector<std::string> chain;

	//chain.push_back(current_item_info.audio_path_normal);

	if (_introAudio != "")
	{
		chain.push_back(_introAudio);
	}
	else
	{
		chain.push_back("sounds/seapearl/instruction.mp3");
	}
//	for (auto phonic : current_item_info.list_character) {
//		chain.push_back(phonic.audio_path);
//	}
    AUDIO_ENGINE->playChain(chain,[=]{
        enableTouch();
		ShowImageAndWordBeforeStart();
		speaker->setVisible(true);
		speaker->setTouchEnabled(true);
    });
}


void Seapearl::update(float dt) {

}

void Seapearl::mergeLetter(Seapearl_Item *character_item) {
	CCLOG(__FUNCTION__);
	
	AUDIO_ENGINE->playSound(character_item->item_info.audio_path);

	auto pos_goal = gachngang_layout->convertToWorldSpace(this->getPositionCharacterMove());
	pos_goal.y += character_item->getBoundingBox().size.height*0.3;
	auto scale = text_lb->getFontSize()*1.0f / character_item->getLblInfo()->getFontSize();
	auto character_info_clone = character_item->getLblInfo()->clone();
	character_item->getLblInfo()->setOpacity(0);
	m_root_layout->addChild(character_info_clone, 1007);
	character_info_clone->setPosition(m_root_layout->convertToNodeSpace(character_item->getLblInfo()->getWorldPosition()));
	character_info_clone->runAction(Sequence::create(ScaleTo::create(0.15f,1.5f),DelayTime::create(0.15f),Spawn::create(MoveTo::create(0.3f, pos_goal), ScaleTo::create(0.3f, scale), NULL), CallFunc::create([=] {
		character_info_clone->removeFromParent();
		for (int i = 0; i < current_character_info.list_index.size(); i++) {
			list_letter[current_character_info.list_index[i]]->setOpacity(255);
		}
		AUDIO_ENGINE->playSound("sounds/seapearl/sp_textshow.mp3", false, nullptr, 0.5f);
	}), NULL));
}

Vec2 Seapearl::getPositionCharacterMove() {
	CCLOG(__FUNCTION__);
	auto index = current_character_info.list_index[0];
	CCLOG("index = %d", index);
	auto rect_result = list_gachngang[index]->getBoundingBox();
	auto rect_num = current_character_info.list_index.size();
	if (rect_num > 1) {
		for (int i = 1; i < rect_num; i++) {
			index = current_character_info.list_index[i];
			CCLOG("index = %d", index);
			auto rect = list_gachngang[index]->getBoundingBox();
			rect_result = rect_result.unionWithRect(rect);
		}
	}
	return rect_result.origin + rect_result.size / 2;
}

void Seapearl::onNextCharacter() {
	//_countVowel = 0;
	CCLOG(__FUNCTION__);
	if (index_character >= (int)current_item_info.list_character.size()) {
		this->mergeWord();
	}
	else {
		for (int i = 0; i < (int)current_character_info.list_index.size(); i++) {
			list_gachngang[current_character_info.list_index[i]]->stopAllActions();
			list_gachngang[current_character_info.list_index[i]]->setColor(Color3B::WHITE);
		}
		current_character_info = current_item_info.list_character[index_character];
		CCLOG("touch hit %d = %s", index_character, current_character_info.character.c_str());
		this->runEffectGachNgang();
		showCharacter();
		
	}
}

void Seapearl::onNextWord() {
	_countVowel = 0;
	CCLOG(__FUNCTION__);
	index_item++;
	if (index_item >= (int)list_item_info.size()) {
        AUDIO_ENGINE->stopAllAudio();
		this->onFinishGame();
	}
	else {
		img_bgwin->setVisible(false);
		//gaf_bgwin->setVisible(false);
        fake_gaf_bgwin->setVisible(false);
        fake_gaf_bgwin->stopAllActions();
		_star1->setVisible(false);
		_star2->setVisible(false);
		_star3->setVisible(false);
		_star4->setVisible(false);
		img_character->setVisible(false);
		_isSpeaking = true;
		startGame();
		
	}
}

std::vector<Seapearl_Item::CHARACTER_INFO> Seapearl::getDistracting(std::string key)
{
	std::vector<Seapearl_Item::CHARACTER_INFO> res;
	auto key_length = key.size();
	auto distracting = current_item_info.list_distracting;
	for (auto dis : distracting) {
		if (dis.character != key && dis.character.size() == key_length) {
			Seapearl_Item::CHARACTER_INFO item;
			item.character = dis.character;
			item.audio_path = dis.audio_path;
			res.push_back(item);
		}
	}
	int pad = 1;
	while (res.size() < 2) {
		for (auto dis : distracting) {
			if (std::abs(int(dis.character.size() - key_length)) == pad) {
				Seapearl_Item::CHARACTER_INFO item;
				item.character = dis.character;
				item.audio_path = dis.audio_path;
				res.push_back(item);
			}
		}
		++pad;
	}
	mj::helper::shuffle(res);
	return res;
}


void Seapearl::showCharacter() {
	CCLOG(__FUNCTION__);
	enableTouch();
	current_character_info = current_item_info.list_character[index_character];
	std::vector<Seapearl_Item::CHARACTER_INFO> list_character_info;
	list_character.clear();
	// Correct answer
	Seapearl_Item::CHARACTER_INFO correct_info;
	correct_info.character = current_character_info.character;
	correct_info.audio_path = current_character_info.audio_path;
	list_character_info.push_back(correct_info);
	// Wrong answer
	auto list_distract = getDistracting(current_character_info.character);
	list_character_info.push_back(list_distract[0]);
	list_character_info.push_back(list_distract[1]);

	mj::helper::shuffle(list_character_info);
	// create character
	for (auto character_info : list_character_info) {
		auto character_item = Seapearl_Item::createItem(character_info);
		character_item->setDelegate(this);
		m_root_layout->addChild(character_item, 1005);
		list_character.push_back(character_item);
		bool flipped = cocos2d::random(0, 1);
		character_item->setFlipped(flipped);
	}
	// show character
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishfadein.mp3");
	auto fish_size = list_character[0]->getContentSize();
	auto start_pos = screen_size.width / 2 - (fish_size.width + 50);
	for (int i = 0; i < list_character.size(); ++i) {
		auto x = start_pos + i * (fish_size.width + 50) + cocos2d::random(-20, 20);
		auto y = cocos2d::random(0.5f, 0.6f) * screen_size.height;
		auto end_pos = Vec2(x, y);
		auto fish = list_character[i];
		fish->setSwimArea(Rect(x - 50, y -50 , 100,100));
		fish->setPosition(Vec2(end_pos.x, screen_size.height));
		fish->getGAFFish()->playSequence("swim_slow", true);
		fish->getLblInfo()->setOpacity(0);
		fish->runAction(Spawn::createWithTwoActions(
			Sequence::create(EaseBackOut::create(MoveTo::create(0.75f, end_pos)), CallFunc::create([=] {
			fish->getLblInfo()->runAction(FadeIn::create(0.25f));
		}), DelayTime::create(0.25f), CallFunc::create([=] {
			fish->swim();
			fish->enableTouch();
		}), NULL),
			Sequence::createWithTwoActions(DelayTime::create(0.15f), CallFunc::create([=]{
			AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishenlarge.mp3");
			fish->getGAFFish()->playSequence("enlarge");
			}))));
	}
}

void Seapearl::onItemClicked(cocos2d::Ref * sender)
{
	this->disableTouch();
	auto item = static_cast<Seapearl_Item *>(sender);
	playClam(2, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		this->scheduleOnce([=](float dt_) {
			playClam(1);
		}, 1.0f, "close_clam");
		
	});

	mj::helper::hideTap();

	if (item->item_info.character == current_character_info.character) {
		_countVowel++;
		this->chooseRight(item);
	}
	else {
		this->chooseWrong(item);
	}

}

void Seapearl::chooseRight(cocos2d::Ref * sender)
{
	_isSpeaking = true;
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishcorrect.mp3", false, nullptr, 0.5f);
	auto fish = (Seapearl_Item *)sender;;
	mergeLetter(fish);
	std::vector<Seapearl_Item *> target_char;
	for (auto character : list_character) {
		if (fish != character) {
			target_char.push_back(character);
		}
	}
	auto fish_pos = fish->getPosition();
	auto end1_pos = target_char[0]->getPosition();
	auto end2_pos = target_char[1]->getPosition();
	
	if (fish_pos.getDistance(end1_pos) > fish_pos.getDistance(end2_pos)) {
		std::swap(target_char[0], target_char[1]);
		std::swap(end1_pos, end2_pos);
	}

	fish->getGAFFish()->playSequence("shrink_slow");
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishblow.mp3", false, nullptr, 0.5f);
	runFish(fish, target_char[0], [=] {
		AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishhit.mp3",false,nullptr,0.5f);
		auto vel = fish->getPosition() - end1_pos;
		target_char[0]->runAction(Sequence::create(MoveTo::create(0.5f, end1_pos - vel.getNormalized()* screen_size.width / 2), RemoveSelf::create(), NULL));
		runFish(fish, target_char[1], [=] {
			AUDIO_ENGINE->stopSound("sounds/seapearl/sp_fishhit.mp3");
			AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishhit.mp3", false, nullptr, 0.5f);
			auto vel1 = fish->getPosition() - end2_pos;
			target_char[1]->runAction(Sequence::create(MoveTo::create(0.5f, end2_pos - vel1.getNormalized()* screen_size.width / 2), RemoveSelf::create(), NULL));
			auto out_pos = Vec2((fish->getGAFFish()->isFlippedX() ? screen_size.width + 300 : -300), screen_size.height / 2);
			
			auto veloc =  target_char[1]->getPosition() - fish->getPosition() ;
			veloc.normalize();
			veloc *= -150;
			fish->runAction(Sequence::create(
				Spawn::createWithTwoActions(RotateTo::create(0.5f, 0.0f),EaseSineIn::create(MoveBy::create(0.5f,veloc)) ) , CallFunc::create([=] {
				AUDIO_ENGINE->stopSound("sounds/seapearl/sp_fishblow.mp3");
				fish->getGAFFish()->playSequence("swim_fast");
			}), MoveTo::create(0.25f, out_pos), CallFunc::create([=] {
				_isSpeaking = true;
				index_character++;
				this->onNextCharacter();
			}), RemoveSelf::create(), NULL));
		});
	});

}

void Seapearl::chooseWrong(cocos2d::Ref * sender)
{
	_isSpeaking = true;
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_fishwrong.mp3");
	this->enableTouch();
	auto fish = (Seapearl_Item *)sender;
	fish->disableTouch();
	fish->getGAFFish()->playSequence("wrong");

	auto delay_time = 13.0f / 32;
	auto delay_blink = delay_time / 4;
	auto text_blink = cocos2d::Sequence::create(DelayTime::create(delay_blink),CallFunc::create([=] {
		fish->getLblInfo()->setTextColor(cocos2d::Color4B::RED);
	}),DelayTime::create(delay_blink), CallFunc::create([=] {
		fish->getLblInfo()->setTextColor(cocos2d::Color4B::BLACK);
	}), NULL);
	fish->getLblInfo()->runAction(Sequence::create(Repeat::create(text_blink,2), CallFunc::create([=] {
		fish->getLblInfo()->setTextColor(cocos2d::Color4B::BLACK);
		fish->enableTouch();
	}), NULL));
}

void Seapearl::showText() {
	CCLOG(__FUNCTION__);
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_textshow.mp3");
	list_letter.clear();
	list_gachngang.clear();
	list_pos_letter.clear();
	current_item_info = list_item_info[index_item];
	index_character = 0;
		current_character_info = current_item_info.list_character[index_character];
	auto width_gachngang = 81, w_add_gachngang = 5;
	auto num_gachngang = StringUtils::StringUTF8(current_item_info.text).length();
	for (int i = 0; i < num_gachngang; i++) {
		auto gachngang = cocos2d::ui::ImageView::create("games/seapearl/gachchan.png");
		gachngang_layout->addChild(gachngang);
		gachngang->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		gachngang->setPositionX(i*(width_gachngang + w_add_gachngang));
		gachngang->setScale(0, 1.0f);
		gachngang->runAction(ScaleTo::create(0.25f, 1.0f));
		list_gachngang.push_back(gachngang);
	}
	gachngang_layout->setContentSize(cocos2d::Size(num_gachngang*(width_gachngang + w_add_gachngang) - w_add_gachngang, 50));
	//    gachngang_layout->setPositionX(screen_size.width/2 - gachngang_layout->getContentSize().width/2);

	text_lb->setScale(1.0f);
	text_lb->setString(current_item_info.text);
	auto pos_text = gachngang_layout->convertToWorldSpace(Vec2(0, list_gachngang[0]->getPositionY()));
	text_lb->setPosition(Vec2(screen_size.width / 2, pos_text.y + 30));
	for (int i = 0; i < (int)list_gachngang.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			auto pos_convert = text_lb->convertToNodeSpace(gachngang_layout->convertToWorldSpace(list_gachngang[i]->getPosition()));
			list_pos_letter.push_back(letter->getPosition() + Vec2(0, 0));
			CCLOG("y = %f", letter->getPositionY() - letter->getContentSize().height*0.5);
			letter->setPosition(Vec2(pos_convert.x + width_gachngang * 0.5, letter->getPositionY() - 23));
			letter->setOpacity(0);
			list_letter.push_back(letter);
		}
	}


	this->runEffectGachNgang();
}
void Seapearl::showWord()
{
	speaker->setVisible(false);
	img_bgwin->setVisible(true);
	//gaf_bgwin->setVisible(true);
	//gaf_bgwin->play(true);
   // fake_gaf_bgwin->setVisible(true);
  //  fake_gaf_bgwin->runAction(RepeatForever::create(RotateBy::create(0.5, -90)));
	
	playWinEffect();
	AUDIO_ENGINE->playSound("sounds/seapearl/sp_wordshow.mp3", false, [=] {
		playNormalText();
	});
}
void Seapearl::playWinEffect() {
		_star1->setVisible(true);
		_star1->play(false, [=](gaf::GAFObject* obj) {
			_star2->setVisible(true);
			_star2->play(false, [=](gaf::GAFObject* obj) {
				//playWinEffect();
				});
			});
		_star4->setVisible(true);
		_star4->play(false, [=](gaf::GAFObject* obj) {
			_star3->setVisible(true);
			_star3->play(false, [=](gaf::GAFObject* obj) {
				//playWinEffect();
				});
			});
}

void Seapearl::runEffectGachNgang() {
	CCLOG(__FUNCTION__);
	for (int i = 0; i < (int)current_character_info.list_index.size(); i++) {
		auto index = current_character_info.list_index[i];
		CCLOG("index = %d", index);
		auto gach_ngang = list_gachngang[index];
		gach_ngang->runAction(Repeat::create(Sequence::create(CallFunc::create([=] {
			gach_ngang->setColor(Color3B::RED);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			gach_ngang->setColor(Color3B::WHITE);
		}), DelayTime::create(0.5f), NULL), 100000));
	}
}

void Seapearl::setColorForText(Color3B color_text) {
	CCLOG(__FUNCTION__);
	for (int i = 0; i < (int)list_letter.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

void Seapearl::mergeWord() {
	CCLOG(__FUNCTION__);
	this->disableTouch();
	
	this->runAction(Sequence::create(DelayTime::create(0.65f), CallFunc::create([=] {
		auto duration = 0.5f;
		for (int i = 0; i < (int)list_gachngang.size(); i++) {
			auto gachngang = list_gachngang[i];
			gachngang->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([gachngang] {
				gachngang->removeFromParent();
			}), NULL));
		}
		list_gachngang.clear();
		text_lb->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(EaseSineIn::create(MoveTo::create(duration, Vec2(screen_size.width / 2, screen_size.height*0.65))), ScaleTo::create(duration, 1.7f), NULL), CallFunc::create([=] {
			//this->playSyncText();
			this->showWord();

		}), NULL));
		for (int i = 0; i < (int)list_pos_letter.size(); i++) {
			list_letter[i]->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(duration, list_pos_letter[i]), NULL));
		}
		this->unschedule("close_clam");
		playClam(3, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				img_character->loadTexture(current_item_info.image_path);
				img_character->setVisible(true);
				img_character->setPosition(gaf_clam->getPosition());
				img_character->setScale(200.0f / img_character->getContentSize().width);
				auto scale = img_character->getScale() * 2;
				img_character->runAction(Sequence::create(DelayTime::create(0.5f),
					Spawn::createWithTwoActions(EaseSineIn::create(MoveTo::create(duration, Vec2(screen_size.width / 2, screen_size.height*0.45))), ScaleTo::create(duration, scale)), NULL));
		});
		
	}), NULL));
}

void Seapearl::playSyncText()
{
	float dt = 0.0f;
	for (int i = 0; i < current_item_info.list_character.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->playSound(current_item_info.list_character[i].audio_path);
			for (int index : current_item_info.list_character[i].list_index) {
				text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::RED);
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::WHITE);
				}), NULL));
			}
		}), NULL));
		dt += 0.75f;
	}
	this->scheduleOnce([=](float dt) {
		playSlowText();
	}, dt, "play sync text finnished");
}

void Seapearl::playSlowText() {
	CCLOG(__FUNCTION__);
	auto audio_path = std::string(current_item_info.audio_path_slow);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		this->setColorForText(Color3B::RED);
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 20));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -20));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));
}
void Seapearl::playNormalText() {
	CCLOG(__FUNCTION__);
	auto audio_id = AUDIO_ENGINE->play(current_item_info.audio_path_normal);
	this->setColorForText(Color3B::RED);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(1.0f), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), DelayTime::create(0.25f), CallFunc::create([=] {
			this->onNextWord();
		}), NULL));
	}), NULL));
}

void Seapearl::disableTouch() {
	touch_layout->setVisible(true);
}

void Seapearl::enableTouch() {
	touch_layout->setVisible(false);
}


void Seapearl::runFish(Seapearl_Item * actor, Seapearl_Item * target, std::function<void()> call_back)
{
	actor->act(target,call_back);
}

void Seapearl::genCharacterIndex(Seapearl::SEAPEARL_INFO &info)
{
	CCLOG(__FUNCTION__);
	// Gen character index	
	int start_index = 0;
	std::queue<int> underline_index;
	for (int i = 0; i < info.list_character.size(); ++i) {
		auto phonic_data = info.list_character[i];
		for (int j = 0; j < phonic_data.character.size(); ++j) {
			if (phonic_data.character[j] == '_') {
				//underline_index
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					info.list_character[i].list_index.push_back(start_index + j);
				}
				else {
					info.list_character[i].list_index.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_data.character.size();
	}
}

void Seapearl::playClam(int state,std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback)
{
	// state 1 : close 2 : sight 3 : open
	if (clam_state == state) return;
	auto last_state = clam_state;
	clam_state = state;
	// close
	if (clam_state == 1) {
		AUDIO_ENGINE->playSound("sounds/seapearl/sp_shellclose.mp3");
		if (last_state == 2) {
			gaf_clam->playSequence("sight_close",false,callback);
		}
		else {
			gaf_clam->playSequence("close", false, callback);
		}
	}
	// sight
	else if (clam_state == 2) {
		AUDIO_ENGINE->playSound("sounds/seapearl/sp_shellopen.mp3");
		gaf_clam->playSequence("sight_open", false, callback);
	}
	// open
	else if (clam_state == 3) {
		AUDIO_ENGINE->playSound("sounds/seapearl/sp_shellopen.mp3");
		if (last_state == 1) {
			gaf_clam->playSequence("open", false, callback);
		}
		else {
			gaf_clam->playSequence("open_fromsight", false, callback);
		}
	}
	
}

void Seapearl::playClam()
{
	auto state = cocos2d::random(1, 3);
}

void Seapearl::ShowImageAndWordBeforeStart() {
	_isSpeaking = true;
	//200.0f / img_character->getContentSize().width
	playClam(3, [=](gaf::GAFObject* obj, const std::string& callback_name) {
			current_item_info = list_item_info[index_item];

			_txtBegin->setText(current_item_info.text);
			_txtBegin->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2 +100));
			_txtBegin->setVisible(true);
			_txtBegin->setScale((200.0f / _txtBegin->getContentSize().width) * 2);
			auto scaleText = _imgBegin->getScale() / 2;

			_txtBegin->runAction(Sequence::create(
				FadeIn::create(0.2f),
				CallFunc::create([=]() {
					AUDIO_ENGINE->playSound(current_item_info.audio_path_normal);
				}),
				DelayTime::create(1.f),
				Spawn::createWithTwoActions(MoveTo::create(2, gaf_clam->getPosition()), ScaleTo::create(2, scaleText)),
				FadeOut::create(0.1f),
				NULL)
			);

			_imgBegin->loadTexture(current_item_info.image_path);
			_imgBegin->setVisible(true);
			_imgBegin->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2 +250));
			_imgBegin->setScale((100.0f / _imgBegin->getContentSize().width) * 2);
			auto scaleImg = _imgBegin->getScale() / 2;

			_imgBegin->runAction(Sequence::create(
				FadeIn::create(0.2f),
				DelayTime::create(1.f),
				Spawn::createWithTwoActions(MoveTo::create(2, gaf_clam->getPosition()), ScaleTo::create(2, scaleImg)),
				FadeOut::create(0.1f),
				CallFunc::create([=] {
					playClam(1);
				}), 
				CallFunc::create([=] {
					showText();
					showCharacter();
				}),
				NULL)
			);
			this->schedule([=](float) {
				inActiveMode();
				}, 8, 100, 7, "inActiveMode");
	});
}

void Seapearl::inActiveMode() {
	if (_isSpeaking) {
		_isSpeaking = false;
	}
	else {
		_isSpeaking = true; 
		if (_countVowel < current_item_info.list_character.size() && current_item_info.list_character[_countVowel].audio_path.empty()) {
				AUDIO_ENGINE->playSound(current_item_info.audio_path_normal, false, [=]() {
					_isSpeaking = false;
					});
		}
		else {
			AUDIO_ENGINE->playSound(current_item_info.audio_path_normal, false, [=]() {
				{
					speaker->scheduleOnce([=](float) {
						if (_countVowel < current_item_info.list_character.size())
							AUDIO_ENGINE->playSound(current_item_info.list_character[_countVowel].audio_path, false, [=]() {
								_isSpeaking = false;
								});
							}, 1, "play_audio");
				}
				});
		}
		
		
	}
}