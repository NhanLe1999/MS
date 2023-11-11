#include "GamePhanLoai.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Math_Func.h"

std::once_flag game_phan_loai_reader;


#define SOUND_INTRO "sounds/vm_phan_loai/intro.mp3"
#define SOUND_BGM "sounds/vm_phan_loai/bgm.mp3"
#define SOUND_CORRECT "sounds/vm_phan_loai/correct_2.mp3"
#define SOUND_WRONG "sounds/vm_phan_loai/wrong_3.mp3"
#define SOUND_TOUCH "sounds/vm_phan_loai/touch_%d.mp3"
#define SOUND_DROP "sounds/vm_phan_loai/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/vm_phan_loai/moveback_2.mp3"

#define GAF_DROP "gaf/vm_phan_loai/drop_effect/drop_effect.gaf"

#define CSB_NAME "csb/game/vm_phan_loai/GamePhanLoai.csb"



INITIALIZE_READER(GamePhanLoai);

GamePhanLoai * GamePhanLoai::createGame()
{
	std::call_once(game_phan_loai_reader, [] {
		REGISTER_CSB_READER(GamePhanLoai);
	});
	auto p = static_cast<GamePhanLoai *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		//p->retain();
		p->didLoadFromCSB();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

GamePhanLoai * GamePhanLoai::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(game_phan_loai_reader, [] {
		REGISTER_CSB_READER(GamePhanLoai);
	});
	auto p = static_cast<GamePhanLoai *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		//p->retain();
		p->didLoadFromCSB(json_file,images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void GamePhanLoai::didLoadFromCSB()
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse< rapidjson::kParseDefaultFlags>(str.c_str());
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	this->generateData(document.GetObject(),images);
	
}

void GamePhanLoai::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
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

void GamePhanLoai::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	m_images = images;
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
    
	// GAME CONFIG
	slot_mode = 0;
	auto value_game = value_data["game_config"].GetObject();
	if (value_game.HasMember("show_text") && value_game["show_text"].GetBool()) {
		slot_mode += GamePhanLoai_Slot::SHOW_TEXT;
	}
	if (value_game.HasMember("show_icon") && value_game["show_icon"].GetBool()) {
		slot_mode += GamePhanLoai_Slot::SHOW_ICON;
	}
	if (value_game.HasMember("show_background") && value_game["show_background"].GetBool()) {
		slot_mode += GamePhanLoai_Slot::SHOW_CUSTOM_BG;
	}
	if (value_game.HasMember("show_default_background") && value_game["show_default_background"].GetBool()) {
		slot_mode += GamePhanLoai_Slot::SHOW_BACKGROUND;
	}
    is_ngang = false;
	if (value_game.HasMember("sap_xep_slot")) {
        auto type = value_game["sap_xep_slot"].GetString();
        if (strcmp(type, "ngang") == 0){
            is_ngang = true;
        }
    }
	// DATA CONFIG
	auto list_object = value_data["item"].GetArray();
	GamePhanLoaiInfo game_data;

	for (int i = 0; i < (int)list_object.Size(); i++) {
		Type_Info object_info;
		auto object_value = list_object[i].GetObject();
		for (int j = 0; j < (int)object_value["type"].Size(); j++) {
			object_info.iType.push_back(object_value["type"][j].GetInt());
		}
		std::string icon_id = object_value["item_data"].GetString();
		auto dataImg = math::resource::ResHelper::getImage(images, icon_id);

		object_info.text = dataImg->getText("name_1", lang);
		object_info.audio = dataImg->getAudios("name_1", lang).front();
		object_info.image_path = dataImg->path;

		game_data.list_objects.push_back(object_info);

	}

	auto list_type = value_data["box"].GetArray();
	for (int i = 0; i < (int)list_type.Size(); i++) {
		auto object_value = list_type[i].GetObject();
		std::vector<Type_Info> data_type_info;

		std::string icon_id = object_value["audio_mc_path"].GetString();
		auto dataImg = math::resource::ResHelper::getImage(images, icon_id);
		Type_Info question_info;
		question_info.text = dataImg->getQuestionVMonkey("name_1", lang);
		question_info.audio = dataImg->getAudios("name_1", lang).front();
		game_data.questions.push_back(question_info);

		auto listData = object_value["data"].GetArray();
		for (int j = 0; j < (int)listData.Size(); j++) {
			Type_Info object_info;
			object_info.iType.push_back(listData[j]["id"].GetInt());
			auto image_id = listData[j]["text"].GetString();
			auto image = math::resource::ResHelper::getImage(images, image_id);
			object_info.text = image->getText("name_1", lang);
			object_info.audio = image->getAudios("name_1", lang).front();
			object_info.image_path = image->path;
			object_info.background = "";
			if (listData[j].HasMember("background")) {
				auto bg_id = listData[j]["background"].GetString();
				auto bg_img = math::resource::ResHelper::getImage(images, bg_id);
				object_info.background = bg_img->path;
			}
			
			m_slot_trace.insert({ object_info.iType.front(),image->tracing });
			data_type_info.push_back(object_info);
		}

		game_data.list_types.push_back(data_type_info);
	}

	m_game_index = 0;
	m_game_data = game_data;
}

void GamePhanLoai::onEnter()
{
	MJMainGBase::onEnter();
	loadFrames();
	playBackgroundMusic(SOUND_BGM);
	showIntroGame();
}

void GamePhanLoai::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);
	
	m_layout_root = utils::findChild<ui::Layout*>(this, "root_layout");
	m_layout_root->setTouchEnabled(true);
	m_layout_root->addClickEventListener([=](Ref*) {
		CCLOG("clicked layout root");
		scheduleSoundGuiding();
		});
	utils::findChild<ui::Button*>(this, "btn_loa")->setVisible(false);
	auto loa_bg = utils::findChild<ui::ImageView*>(this, "loa_bg");
	loa_bg->setVisible(false);
	auto loa_panel = utils::findChild<ui::Layout*>(this, "loa_panel");
	loa_panel->setPositionX(loa_panel->getPosition().x - loa_bg->getContentSize().width);
	loa_panel->setPositionY(loa_panel->getPositionY() - 10);

	m_btn_loa = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	m_btn_loa->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(m_btn_loa);
	m_lbl_question = utils::findChild < ui::Text*> (this, "lbl_question");
	m_bg_question = utils::findChild<ui::ImageView*>(this, "question_bg");
	utils::findChild<ui::ImageView*>(this, "answer_bg")->setPosition(Vec2(screen_size.width / 2 + 330, 300));

	m_btn_loa->addClickEventListener(CC_CALLBACK_1(GamePhanLoai::onClickedSpeaker, this));
	m_bg_question->addClickEventListener(CC_CALLBACK_1(GamePhanLoai::onClickedSpeaker, this));

	m_btn_loa->setTouchEnabled(true);
	m_bg_question->setTouchEnabled(true);

	m_layout_locked = cocos2d::ui::Layout::create();
	m_layout_locked->addClickEventListener([=](Ref*) {
		CCLOG("clicked layout locked");
		scheduleSoundGuiding();
		});
	m_layout_locked->setContentSize(screen_size);
	m_layout_locked->setTouchEnabled(true);
	this->addChild(m_layout_locked, 1000000);

	m_lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	m_lbl_question->setFontSize(40);
	
}

void GamePhanLoai::showIntroGame()
{
	AUDIO_ENGINE->playSound(SOUND_INTRO, false, [=] {
		if (is_auto_play) {
			autoPlay();
		}
		else {
			startGame();
		}

	});
}

void GamePhanLoai::scheduleSoundGuiding()
{
	CCLOG("schedule sound guild");
	this->unschedule("soundGuiding");
	this->schedule([=](float)
		{
			std::string sound_question = m_game_data.questions.at(m_game_index).audio;
			if (cocos2d::FileUtils::getInstance()->isFileExist(sound_question))
			{
				if (num_paired == 0)
				{
					auto item2 = static_cast<GamePhanLoai_Item*>(m_layout_root->getChildByName("item0"));
					for (auto a : m_slots)
					{
						if (a && item2->isContainValue(a->getCompareValue()))
						{
							auto pos1 = item2->getPosition();
							auto pos2 = a->getPosition();
							auto a = ImageView::create("themes/default/hand.png");
							a->setPosition(pos1);
							a->setZOrder(100);
							m_layout_root->addChild(a);
							a->runAction(Sequence::create(MoveTo::create(2, pos2), FadeOut::create(0.25), RemoveSelf::create(), nullptr));
							CCLOG("ds");
						}
					}
				}
				AUDIO_ENGINE->playSound(sound_question, false, ([=] {
					scheduleSoundGuiding();
					}));
			}
			
		}, 10, 100, 10, "soundGuiding");
}

void GamePhanLoai::startGame()
{
	initQuestion();
	onClickedSpeaker(m_btn_loa);
	showItemsAndSlots();
	scheduleSoundGuiding();
}

void GamePhanLoai::onFinishGameVMonkey()
{
	MJMainGBase::onFinishGameVMonkey();
}

void GamePhanLoai::update(float dt)
{
}

void GamePhanLoai::onClickedSlotDelegate(cocos2d::Ref * sender)
{
	auto slot = static_cast<GamePhanLoai_Slot*>(sender);
    //AUDIO_ENGINE->stopEffect(main_audio_id);
	AUDIO_ENGINE->stopAllAudio();
	sound_slot = slot->_info.audio_full;
	main_audio_id = AUDIO_ENGINE->play(sound_slot, false, 1.0f, ([=] {
		scheduleSoundGuiding();
		}));
}

void GamePhanLoai::onClickedItem(cocos2d::Ref * sender)
{
	auto item = static_cast<GamePhanLoai_Item *>(sender);
	//setTouchLock(true);
	AUDIO_ENGINE->stopSound(sound_item);
	sound_item = item->getInfo().audio_full;
	AUDIO_ENGINE->playEffect(sound_item);
	//auto start_pos = item->getParent()->convertToWorldSpace(item->_start_pos);
	//zoomTo(item, start_pos, 1.0f, 0.25f, [=] {
	//	AUDIO_ENGINE->stopSound(item->getInfo().audio_full);
	//	setTouchLock(false);
	//	item->setTouchHandleType(GamePhanLoai_ItemTouchType::TOUCH);
	//	item->setLocalZOrder(20);
	//	for (auto item : m_items) {
	//		item->setTouchEnable(true);
	//	}
	//});
}

void GamePhanLoai::onMoveBeginItem(cocos2d::Ref * sender)
{
	AUDIO_ENGINE->stopAllAudio();
	auto item = static_cast<GamePhanLoai_Item *>(sender);
	std::string sound_touch = StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2));
	AUDIO_ENGINE->playSound(sound_touch, false);
	item->setLocalZOrder(21);
	hideTapGuideGame();
}

void GamePhanLoai::onMoveEndedItem(cocos2d::Ref * sender, bool is_item_moved)
{
	auto item = static_cast<GamePhanLoai_Item *>(sender);
	if (is_item_moved) {
		item->setLocalZOrder(20);
		auto pos = item->getPosition();
		GamePhanLoai_Slot * slot = nullptr;
		for (auto w : m_slots) {
			auto bounding = w->getBoundingBox();
			if (bounding.containsPoint(pos)) {
				slot = w;
			}
		}
		if (slot && item->isContainValue(slot->getCompareValue())) {
			AUDIO_ENGINE->playSound(SOUND_CORRECT, false, ([=] {
				std::string sound_object = item->getInfo().audio_full;
				if (cocos2d::FileUtils::getInstance()->isFileExist(sound_object))
				{
					AUDIO_ENGINE->playSound(sound_object);
				} 
				}));
			onPair(item, slot);
		}
		else {
			AUDIO_ENGINE->playSound(SOUND_WRONG);
			// schedule guiding
			scheduleSoundGuiding();
			item->backToStart();
		}
	}
	else {
		onClickedItem(item);
		scheduleSoundGuiding();
		//setTouchLock(true);
		//for (auto item : m_items) {
		//	item->setTouchEnable(false);
		//}
		//zoomTo(item, screen_size / 2, 3.0f, 0.25f, [=] {
		//	AUDIO_ENGINE->playSound(item->getInfo().audio_full);
		//	setTouchLock(false);
		//	item->setTouchHandleType(GamePhanLoai_ItemTouchType::CLICK);
		//});
	}
}

void GamePhanLoai::onNextGame()
{
	++m_game_index;
	if (m_game_index < m_game_data.list_types.size()) {
		startGame();
	}
	else {
		this->scheduleOnce([=](float) {
			onFinishGameVMonkey();
		}, 1.0f, "delay endgame");
	}

	
}


void GamePhanLoai::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void GamePhanLoai::onExit()
{
	MJMainGBase::onExit();
}

void GamePhanLoai::onRefreshGame()
{
	for (GamePhanLoai_Item * item : m_items) {
		item->hideItem(0.0f, [=] {
			item->removeFromParent();
		});
	}

	for (GamePhanLoai_Slot * slot : m_slots) {
		slot->onHide(0.0f, [=] {
			slot->removeFromParent();
		});
	}
}

void GamePhanLoai::setTouchLock(bool locked)
{
	m_layout_locked->setTouchEnabled(locked);
}

void GamePhanLoai::onClickedSpeaker(Ref * sender)
{
	CCLOG("clicked sprek");
	AUDIO_ENGINE->stopAllAudio();
	main_audio_id = AUDIO_ENGINE->play(sound_question,false, 1.0f,  ([=] {
		scheduleSoundGuiding();
		}));
}


void GamePhanLoai::initQuestion()
{
	m_lbl_question->setString(m_game_data.questions.at(m_game_index).text);
	sound_question = m_game_data.questions.at(m_game_index).audio;
	m_items.clear();
	m_slots.clear();
	num_paired = 0;

	int order = 1;


	int slot_size_type = m_game_data.list_types.at(m_game_index).size();
	for (auto info : m_game_data.list_types.at(m_game_index)) {

		mj::PHONIC_INFO slot_info;
		slot_info.text_full = info.text;
		slot_info.audio_full = info.audio;
		slot_info.image_path = info.image_path;
		auto slot = GamePhanLoai_Slot::createSlot(slot_info,info.background);
		int slot_type = info.iType.front();
		slot->setSlotEnabled(slot_size_type,slot_mode);
		slot->setCompareValue(slot_type);
		slot->setDelegate(this);
		slot->setVisible(false);
		m_layout_root->addChild(slot, 10);
		m_slots.push_back(slot);

		int k = 0;
		for (auto o_info : m_game_data.list_objects) {
			bool is_this_level_item = false;
			for (int c : o_info.iType) {
				if (c == slot_type) {
					is_this_level_item = true;
					break;
				}
			}
			if (is_this_level_item) {
				mj::PHONIC_INFO item_info;
				item_info.text_full = o_info.text;
				item_info.audio_full = o_info.audio;
				item_info.image_path = o_info.image_path;
				auto item = GamePhanLoai_Item::createItem(item_info);
				item->setCompareValue(o_info.iType);
				item->setDelegate(this);
				item->setVisible(false);
				item->setName("item" + to_string(k));
				k++;
				m_layout_root->addChild(item, 20);
				m_items.push_back(item);
			}

		}
	}

	total_item = m_items.size();

	if (m_slots.size() == 1) {
		// mode 1 box -> thêm icon khác vào
		int slot_type = m_slots.front()->getCompareValue();
		for (auto o_info : m_game_data.list_objects) {
			bool is_this_level_item = false;
			for (int c : o_info.iType) {
				if (c == slot_type) {
					is_this_level_item = true;
					break;
				}
			}
			if (!is_this_level_item) {
				mj::PHONIC_INFO item_info;
				item_info.text_full = o_info.text;
				item_info.audio_full = o_info.audio;
				item_info.image_path = o_info.image_path;
				auto item = GamePhanLoai_Item::createItem(item_info);
				item->setCompareValue(o_info.iType);
				item->setDelegate(this);
				item->setVisible(false);
				m_layout_root->addChild(item, 20);
				m_items.push_back(item);
			}

		}
	}

	Size fix_size = Size(0, 0);
	
	if (slot_mode & GamePhanLoai_Slot::SHOW_TEXT) {
		if (slot_size_type == 3) {
			fix_size = fix_size + Size(0, 20);
		}
		else {
			fix_size = fix_size + Size(0, 30);
		}
		
	}

	auto answer_bg = utils::findChild<ui::ImageView*>(this, "answer_bg");
	auto item_origin = answer_bg->getPosition() - answer_bg->getContentSize()/2;
	auto space_item = answer_bg->getContentSize() - Size(0, 100);
	
	mj::helper::shuffle(m_items);
	math::func::smartSetPosition(m_items, Size(160,110),Rect(item_origin,space_item),0,true);
	
	int row = 1;
	int col = m_slots.size();
    
    if (m_slots.size() == 2 && is_ngang){
        row = 2;
        col = 1;
    }
    
	if (m_slots.size() == 4) {
		row = 2;
		col = 2;
		fix_size = fix_size + Size(10, 10);
	}

	// nếu chỉ hiển thị background riêng -> set slot size
	if (!(slot_mode & GamePhanLoai_Slot::SHOW_BACKGROUND) && (slot_mode & GamePhanLoai_Slot::SHOW_CUSTOM_BG)) {
		for (auto slot : m_slots) {
                slot->setSlotSize(cocos2d::Size(500, 500));

		}
	}
    if (m_slots.size() == 2 && is_ngang){
        for (auto slot : m_slots) {
            slot->setSlotSize(cocos2d::Size(250, 400));
        }
    }
	math::func::setPositionListOnFrame(m_slots, row, col, m_slots.front()->getContentSize() + fix_size, Vec2(screen_size.width / 2 - 185, 300), 0);
	
	// nếu hiển thị background riêng -> lấy điểm từ ảnh
	//if ((slot_mode & GamePhanLoai_Slot::SHOW_CUSTOM_BG)) {
	//	for (auto slot : m_slots) {
	//		try {
	//			auto trace_data = math::JSON::MMTrace::create(m_slot_trace.at(slot->getCompareValue()));
	//			if (trace_data.points.size() > 0) {
	//				std::vector<Vec2> locations;
	//				for (auto point : trace_data.points) {
	//					auto loc = trace_data.getLocation(point, slot->getPosition(), slot->getContentSize());
	//					locations.push_back(loc);
	//				}
	//				m_slot_positions.insert({ slot,locations });
	//			}
	//		}
	//		catch (exception e) {
	//			CCLOG("PARSE TRACE ERROR");
	//		}
	//	}
	//}

	if (m_slot_positions.size() == m_slots.size()) return;
	m_slot_positions.clear();
	int num_row = (int)m_slots.front()->getContentSize().height / 120;
	
	fix_size = fix_size + Size(30, 30);

	
	Vec2 fix_pos = Vec2(15, 15);
	if (slot_mode & GamePhanLoai_Slot::SHOW_ICON) {
		fix_pos += Vec2(50, 0);
		fix_size = fix_size + Size(50, 0);
	}

	int total_node = num_row * 4;
	int max_line = 4;

	// trường hợp mỗi slot chỉ chứa 1 object -> vị trí object vào giữa
	if (m_slots.size() == m_items.size()) {
		total_node = 1;
		max_line = 1;
	}

	for (auto slot : m_slots) {
		auto slot_origin = slot->getPosition() - slot->getContentSize() / 2 +fix_pos;
		auto slot_space = slot->getContentSize() - fix_size;
		auto locations = math::func::getLocations(total_node, max_line, Rect(slot_origin,slot_space), screen_size);
		m_slot_positions.insert({ slot, locations });
	}
}

void GamePhanLoai::showItemsAndSlots()
{

	for (GamePhanLoai_Item * item : m_items) {
		item->setVisible(true);
		item->showItem(0.25f, [item] {
			item->setTouchHandleType(GamePhanLoai_ItemTouchType::TOUCH);
		});
	}

	for (GamePhanLoai_Slot * slot : m_slots) {
		slot->setVisible(true);
		slot->onShow(0.25f);
	}

	setTouchLock(false);
}

void GamePhanLoai::onPair(GamePhanLoai_Item * item, GamePhanLoai_Slot * slot)
{
	++num_paired;
	item->setTouchHandleType(GamePhanLoai_ItemTouchType::NONE);

	auto item_position = item->getPosition();
	
	
	std::vector<Vec2>::iterator pos_it = m_slot_positions.at(slot).begin();
	float min_distance = INT_MAX;
	for (auto it = m_slot_positions.at(slot).begin(); it != m_slot_positions.at(slot).end(); ++it) {
		float distance = (*it).distance(item_position);
		if (distance < min_distance) {
			min_distance = distance;
			pos_it = it;
		}
	}
	auto position = (*pos_it);
	m_slot_positions.at(slot).erase(pos_it);
	item->runAction(cocos2d::MoveTo::create(0.25f, position));
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(StringUtils::format(SOUND_DROP, cocos2d::random(1, 3)));
		auto gaf_drop = GAFWrapper::createGAF(GAF_DROP);
		gaf_drop->setPosition(position);
		m_layout_root->addChild(gaf_drop, 30);
		gaf_drop->play(false, [gaf_drop](gaf::GAFObject *) {
			gaf_drop->removeFromParent();
		});
	}, 0.2f, "drop_item");

	if (num_paired == total_item) {
		this->unschedule("soundGuiding");
		this->scheduleOnce([=](float) {
			this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=] {
				playEndingScript();
				}), nullptr));
			}, 0.25f, "ending script");
	}
	else scheduleSoundGuiding();
}

void GamePhanLoai::zoomTo(Ref * sender, Vec2 position, float scale,float duration,std::function<void()> callback)
{
	auto item = (Layout*)sender;
	item->runAction(Sequence::create(
		Spawn::createWithTwoActions(
			ScaleTo::create(duration,scale),
			MoveTo::create(duration,position)
		)
		, 
		CallFunc::create([=] {
		if (callback) callback();
	}), NULL));
}

void GamePhanLoai::playEndingScript()
{	
	AUDIO_ENGINE->stopSound(sound_question);
	sound_question = "";
	onNextGame();
}



void GamePhanLoai::autoPlay()
{
	schedule([=](float) {

	}, 4.0f, 100, 5.0f, "auto_lay");

}
