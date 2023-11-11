#include "GameSapXep.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Math_Func.h"

std::once_flag game_sap_xep_reader;


#define SOUND_INTRO "sounds/vm_sap_xep/intro.mp3"
#define SOUND_BGM "sounds/vm_sap_xep/bgm.mp3"
#define SOUND_READ_THE_STORY "sounds/vm_sap_xep/read_the_story.mp3"
#define SOUND_CORRECT "sounds/vm_sap_xep/correct_%d.mp3"
#define SOUND_WRONG "sounds/vm_sap_xep/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/vm_sap_xep/touch_%d.mp3"
#define SOUND_DROP "sounds/vm_sap_xep/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/vm_sap_xep/moveback_%d.mp3"
#define SOUND_DROP_1 "sounds/vm_sap_xep/drop_1.mp3"
#define GAF_DROP "gaf/vm_sap_xep/drop_effect/drop_effect.gaf"
#define CSB_NAME "csb/game/vm_sap_xep/GameSapXep.csb"
#define BUTTON_NEXT "hdr/games/vm_sap_xep/item_next.png"
#define BUTTON_PLAY "hdr/games/vm_sap_xep/item_play.png"
#define BUTTON_BG_NEXT "hdr/games/vm_sap_xep/Vector-Smart-Object1.png"

INITIALIZE_READER(GameSapXep);

GameSapXep * GameSapXep::createGame(std::string json_file)
{
	std::call_once(game_sap_xep_reader, [] {
		REGISTER_CSB_READER(GameSapXep);
	});
	auto p = static_cast<GameSapXep *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		p->didLoadFromCSB(json_file);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

GameSapXep * GameSapXep::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(game_sap_xep_reader, [] {
		REGISTER_CSB_READER(GameSapXep);
	});
	auto p = static_cast<GameSapXep *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		p->didLoadFromCSB(json_file,images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void GameSapXep::didLoadFromCSB(std::string json_file)
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

void GameSapXep::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
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


void GameSapXep::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

    
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
		rapidjson::Value& question = json_data[i].FindMember("question")->value;
		GameSapXepInfo info;
		auto image_question = math::resource::ResHelper::getImage(images, question.GetString());
		info.question.text_full = image_question->getQuestionVMonkey("name_1",lang);
		info.question.audio_full = image_question->getAudios("name_1", lang).front();
		rapidjson::Value& answers_data = json_data[i].FindMember("answer")->value;
		for (int j = 0; j < answers_data.Size(); ++j) {
			rapidjson::Value& answer = answers_data[j].FindMember("item")->value;
			mj::PHONIC_INFO pi;
			auto image_answer = math::resource::ResHelper::getImage(images, answer.GetString());
			pi.text_full = image_answer->getText("name_1", lang);
			pi.audio_full = image_answer->getAudios("name_1", lang).front();
			pi.image_path = image_answer->path;
			info.answer.push_back(pi);
		}
		m_game_data.push_back(info);
	}

	m_game_index = 0;
	loadFrames();
}

void GameSapXep::onEnter()
{
	MJMainGBase::onEnter();
	playBackgroundMusic(SOUND_BGM);
	showIntroGame();
}

void GameSapXep::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);
	
	m_layout_root = utils::findChild<ui::Layout*>(this, "root_layout");
	m_btn_loa = utils::findChild<ui::Button*>(this, "btn_loa");
	m_lbl_question = utils::findChild < ui::Text*> (this, "lbl_question");
	m_bg_question = utils::findChild<ui::ImageView*>(this, "question_bg");

	m_btn_loa->addClickEventListener(CC_CALLBACK_1(GameSapXep::onClickedSpeaker, this));
	m_bg_question->addClickEventListener(CC_CALLBACK_1(GameSapXep::onClickedSpeaker, this));

	m_layout_locked = cocos2d::ui::Layout::create();
	m_layout_locked->setContentSize(screen_size);
	m_layout_locked->setTouchEnabled(true);
	this->addChild(m_layout_locked, 1000000);

	m_lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	m_lbl_question->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_lbl_question->setTextVerticalAlignment(TextVAlignment::CENTER);

	buttonBGNext = ImageView::create(BUTTON_BG_NEXT);
	buttonBGNext->setPosition(Vec2(screen_size.width - 100, 120));
	buttonBGNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(buttonBGNext);
	buttonBGNext->ignoreContentAdaptWithSize(false);
	buttonBGNext->setContentSize(Size(100, 100));

	buttonNext = ImageView::create(BUTTON_NEXT);
	buttonNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonNext->setPosition(buttonBGNext->getContentSize()/2);
	buttonNext->ignoreContentAdaptWithSize(false);
	buttonNext->setContentSize(Size(60, 46));
	buttonBGNext->addChild(buttonNext,10000);

	buttonPlay = ImageView::create(BUTTON_PLAY);
	buttonPlay->setPosition(Vec2(screen_size.width / 2, 120));
	buttonPlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonPlay->ignoreContentAdaptWithSize(false);
	buttonPlay->setContentSize(Size(110, 110));
	this->addChild(buttonPlay);

	buttonPlay->setVisible(false);
	buttonNext->setVisible(false);
	buttonBGNext->setVisible(false);
}

void GameSapXep::showIntroGame()
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

void GameSapXep::startGame()
{
	m_current_data = m_game_data[m_game_index];
	initQuestion();
	AUDIO_ENGINE->playSound(SOUND_READ_THE_STORY, false, [=]() {
		showItemsAndSlots();
		}, 1.0f);
	m_btn_loa->setTouchEnabled(true);
}

void GameSapXep::onFinishGameVMonkey()
{
	MJMainGBase::onFinishGameVMonkey();
}

void GameSapXep::update(float dt)
{
}

void GameSapXep::onClickedSlotDelegate(cocos2d::Ref * sender)
{

}

void GameSapXep::onClickedItem(cocos2d::Ref * sender)
{
	auto item = static_cast<GameSapXep_Item *>(sender);
	setTouchLock(true);
	
	this->unschedule("inActiveMode");
	inActiveMode();
	auto start_pos = item->getParent()->convertToWorldSpace(item->_start_pos);
	item->setLocalZOrder(21);
	zoomTo(item, start_pos, 1.0f, 0.25f, [=] {
		AUDIO_ENGINE->stopSound(item->getInfo().audio_full);
		setTouchLock(false);
		item->setTouchHandleType(GameSapXep_ItemTouchType::TOUCH);
		item->setLocalZOrder(20);
		for (auto item : m_items) {
            if(!item->isPaired()){
                item->setTouchEnable(true);
            }
		}
		if (num_paired < 1) {
			showTutorial(10.0f);
		} 
	});
}

void GameSapXep::onMoveBeginItem(cocos2d::Ref * sender)
{
	AUDIO_ENGINE->playSound(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)));
	auto item = static_cast<GameSapXep_Item *>(sender);
	stopGuiding();
	this->unschedule("inActiveMode");
	item->setLocalZOrder(21);
	hideTapGuideGame();
}

void GameSapXep::onMoveEndedItem(cocos2d::Ref * sender, bool is_item_moved)
{
	auto item = static_cast<GameSapXep_Item *>(sender);
	if (is_item_moved) {
		this->unschedule("inActiveMode");
		
		item->setLocalZOrder(20);
		auto pos = item->getPosition();
		GameSapXep_Slot * slot = nullptr;
		for (auto w : m_slots) {
			auto bounding = w->getBoundingBox();
			if (bounding.containsPoint(pos)) {
				slot = w;
			}
		}
		if (slot && slot->getLinker() == item) {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_CORRECT, cocos2d::random(1, 3)));
			for (int i = 0; i < m_slots.size(); i++) {
				if (item->getTag() == m_slots[i]->getTag()) {
					stopGuiding();
				}
			}
			onPair(item, slot);
			if (num_paired < 3) {
				inActiveMode();
			}
		}
		else {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_WRONG, cocos2d::random(1, 3)));
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_MOVEBACK, cocos2d::random(1, 3)));
			if (num_paired < 1) {
				showTutorial(10.0f);
			}
			item->backToStart();
			inActiveMode();
		}
	}
	else {
		setTouchLock(true);
		for (auto item : m_items) {
			item->setTouchEnable(false);
		}
		zoomTo(item, screen_size / 2, 3.0f, 0.25f, [=] {
			// stop sound question and previous sound item
			AUDIO_ENGINE->stopSound(sound_item);
			AUDIO_ENGINE->stopSound(sound_question);
			stopGuiding();
			sound_item = item->getInfo().audio_full;
			AUDIO_ENGINE->playSound(sound_item);
			setTouchLock(false);
			item->setTouchHandleType(GameSapXep_ItemTouchType::CLICK);
		});
	}
}

void GameSapXep::onNextGame()
{
	++m_game_index;
	if (m_game_index < m_game_data.size()) {
		startGame();
	}
	else {
		this->scheduleOnce([=](float) {
			onFinishGameVMonkey();
		}, 0.25f, "delay endgame");
	}
}

void GameSapXep::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void GameSapXep::onExit()
{
	MJMainGBase::onExit();
}

void GameSapXep::onRefreshGame()
{
	
	for (GameSapXep_Item * item : m_items) {
		item->hideItem(0.0f, [=] {
			item->removeFromParent();
		});
	}

	for (GameSapXep_Slot * slot : m_slots) {
		slot->onHide(0.0f, [=] {
			slot->removeFromParent();
		});
	}
}

void GameSapXep::setTouchLock(bool locked)
{
	m_layout_locked->setTouchEnabled(locked);
}

void GameSapXep::onClickedSpeaker(Ref * sender)
{
	// stop sound item
	stopGuiding();
	m_btn_loa->setTouchEnabled(false);
	AUDIO_ENGINE->stopSound(sound_item);
    AUDIO_ENGINE->stopSound(sound_question);
	sound_item = "";
	AUDIO_ENGINE->playSound(sound_question, false,[=] {
		m_btn_loa->setTouchEnabled(true);
		if (num_paired < 1) {
			showTutorial(10.0f);
		}
		inActiveMode();
		});
}

void GameSapXep::initQuestion()
{
	m_lbl_question->setString(m_current_data.question.text_full);
	m_items.clear();
	m_slots.clear();
	sound_question = m_current_data.question.audio_full;
	int order = 1;
	for (auto info : m_current_data.answer) {
		auto slot = GameSapXep_Slot::createSlot(order);
		auto item = GameSapXep_Item::createItem(info);
		slot->setLinker(item);
		item->setTag(order - 1);
		slot->setTag(order - 1);
		item->setDelegate(this);
		item->setVisible(false);
		slot->setVisible(false);
		m_layout_root->addChild(slot,10);
		m_layout_root->addChild(item,20);
		m_slots.push_back(slot);
		m_items.push_back(item);
		++order;
	}
	m_btn_loa->getParent()->setVisible(false);
	m_lbl_question->setVisible(false);
	m_bg_question->setVisible(false);
	auto items_width = std::min(screen_size.width, 250.0f * m_items.size()) + 10;
	auto start_x = (screen_size.width - items_width) / 2;
	//mj::helper::shuffle(m_items);
	math::func::setPositionListOnFrame(m_items,m_items.size(),1, Size(250,250),Vec2(screen_size.width/2,125),0);
	math::func::setPositionListOnFrame(m_slots,m_slots.size(),1, Size(250,250), Vec2(screen_size.width/2,425),0);
	num_paired = 0;
}

void GameSapXep::showItemsAndSlots()
{
	this->scheduleOnce([=](float) {
		playSequence(0, [=] {
			for (auto item : m_items) {
				item->setFlag1(false);
			}
			showInformationGame();
			});
		}, 0.5f, "delay play sequence");
	setTouchLock(false);
}

void GameSapXep::inActiveMode()
{
	this->schedule([=](float) {

		AUDIO_ENGINE->playSound(sound_question);
	}, 10.0f, CC_REPEAT_FOREVER, 10.0f, "inActiveMode");
}

void GameSapXep::showInformationGame()
{
	this->runAction(Sequence::create(CallFunc::create([=]() {
		shufflingItem();
		})
		,DelayTime::create(4.0f)
		, CallFunc::create([=]() {
			showSlots();
			})
		,DelayTime::create(5.8f)
		, CallFunc::create([=]() {
				m_btn_loa->getParent()->setVisible(true);
				m_bg_question->setVisible(true);
				m_lbl_question->setVisible(true);
				onClickedSpeaker(m_btn_loa);
				if (num_paired < 1) {
					showTutorial(10.0f);
				}
				for (auto item : m_items) {
					item->setFlag1(true);
				}
			})
	, nullptr));
}

void GameSapXep::onPair(GameSapXep_Item * item, GameSapXep_Slot * slot)
{
	++num_paired;
	item->setTouchHandleType(GameSapXep_ItemTouchType::NONE);
	item->runAction(cocos2d::MoveTo::create(0.25f, slot->getPosition()));
	
    item->setPaired(true);
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(StringUtils::format(SOUND_DROP, cocos2d::random(1, 3)));
		auto gaf_drop = GAFWrapper::createGAF(GAF_DROP);
		gaf_drop->setScale(std::min(slot->getContentSize().width, slot->getContentSize().height) / 100.0f);
		gaf_drop->setPosition(slot->getPosition());
		m_layout_root->addChild(gaf_drop, 30);
		gaf_drop->play(false, [gaf_drop](gaf::GAFObject *) {
			gaf_drop->removeFromParent();
		});
	}, 0.1f, "drop_item");
	if (num_paired == m_current_data.answer.size()) {
		this->unschedule("inActiveMode");
		buttonPlay->setVisible(true);
		buttonBGNext->setVisible(true);
		buttonNext->setVisible(true);
		buttonPlay->setTouchEnabled(true);
		buttonBGNext->setTouchEnabled(true);
		m_btn_loa->getParent()->setVisible(false);
		m_lbl_question->setVisible(false);
		m_bg_question->setVisible(false);
		buttonPlayClick();
		buttonNextClick();

		this->schedule([=](float) {
			buttonBGNext->runAction(EaseBackOut::create(Sequence::create(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2, 1.0f), nullptr)));
			}, 5.0f, CC_REPEAT_FOREVER, 0.0f, "next_game");
	}
}

void GameSapXep::zoomTo(Ref * sender, Vec2 position, float scale,float duration,std::function<void()> callback)
{
	auto item = (Layout*)sender;
	item->runAction(Sequence::create(
		Spawn::createWithTwoActions(
			ScaleTo::create(duration,scale),
			MoveTo::create(duration,position)
		)
		,CallFunc::create([=] {
		if (callback) callback();
	}), NULL));
}

void GameSapXep::playEndingScript()
{
	// stop sound question
	AUDIO_ENGINE->stopSound(sound_question);
	AUDIO_ENGINE->stopSound(sound_item);
	sound_question = "";
	sound_item = "";
	m_btn_loa->setTouchEnabled(true);
	m_bg_question->setTouchEnabled(true);

	onNextGame();
}

void GameSapXep::playSequence(int index, std::function<void()> callback)
{
	if (index < 0 || index >= m_slots.size()) {
		if (callback) callback();
		return;
	}
	auto slot = m_slots[index];
	auto item = slot->getLinker();
	slot->_start_pos = slot->getPosition();
	item->_start_pos = item->getPosition();
	slot->setLocalZOrder(21);
	item->setLocalZOrder(22);
	zoomTo(slot, screen_size / 2, 3.0f, 0.25f, [=] {
		item->setVisible(true);
		AUDIO_ENGINE->playSound(item->getInfo().audio_full, false, [=] {
			
			zoomTo(slot,slot->_start_pos, 1.0f, 0.25f, [=] {
				slot->setLocalZOrder(10);
				item->setLocalZOrder(20);
				this->scheduleOnce([=](float) {
					playSequence(index + 1, callback);
				}, 0.25f, "delay play sequence");
				
			});
			zoomTo(item, item->_start_pos, 1.0f, 0.25f);
		});
	});
	zoomTo(item, screen_size / 2, 3.0f, 0.25f);
}

ActionInterval* GameSapXep::getAction(std::vector<std::pair<float, Vec2>> listRotateAndPosition, float time) {
	cocos2d::Vector<FiniteTimeAction*> actions;
	for (auto rotateAndPosition : listRotateAndPosition) {
		FiniteTimeAction* action = Sequence::createWithTwoActions(CallFunc::create([=]() {
			AUDIO_ENGINE->play(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)), false, 0.5);
			}), Spawn::create(RotateTo::create(time, rotateAndPosition.first), MoveTo::create(time, rotateAndPosition.second), NULL));
		actions.pushBack(action);
	}
	//actions.pushBack(NULL);
	return Sequence::create(actions);
}
void GameSapXep::showSlots() {
	
	for (int i = 0; i < m_slots.size(); i++) {
		m_slots[i]->setScale(5);
		listPosSlots.push_back(m_slots[i]->getPosition());
		m_slots[i]->setPosition(Vec2(screen_size.width + 700, screen_size.height / 2));
		m_slots[i]->setVisible(true);
		m_slots[i]->onShow(0.25f);
		m_slots[i]->setName("slot" + std::to_string(i));
		
	}
	this->runAction(Sequence::create(CallFunc::create([=]() {
		if (m_slots.size() > 0)
		{
			m_slots[0]->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(Spawn::create(MoveTo::create(0.8f, listPosSlots[0]), ScaleTo::create(0.8f, 1), nullptr)), CallFunc::create([=] {AUDIO_ENGINE->playEffect(SOUND_DROP_1); }), nullptr));
		}
	})
	,DelayTime::create(2.0f)
	,CallFunc::create([=]() {
		if (m_slots.size() > 1)
		{
			m_slots[1]->runAction(Sequence::create(EaseBackOut::create(Spawn::create(MoveTo::create(0.8f, listPosSlots[1]), ScaleTo::create(0.8f, 1), nullptr)), CallFunc::create([=] {AUDIO_ENGINE->playEffect(SOUND_DROP_1); }), nullptr));
		}
	})
	, DelayTime::create(1.2f)
	,CallFunc::create([=]() {
		if (m_slots.size() > 2)
		{
			m_slots[2]->runAction(Sequence::create(EaseBackOut::create(Spawn::create(MoveTo::create(0.8f, listPosSlots[2]), ScaleTo::create(0.8f, 1), nullptr)), CallFunc::create([=] {AUDIO_ENGINE->playEffect(SOUND_DROP_1); }), nullptr));
		}
	})
	, DelayTime::create(1.2f)
	,CallFunc::create([=]() {
		if (m_slots.size() > 3)
		{
			m_slots[3]->runAction(Sequence::create(EaseBackOut::create(Spawn::create(MoveTo::create(0.8f, listPosSlots[3]), ScaleTo::create(0.8f, 1), nullptr)), CallFunc::create([=] {AUDIO_ENGINE->playEffect(SOUND_DROP_1); }), nullptr));
		}
	})
	,nullptr));
}
void GameSapXep::shufflingItem() {

	std::vector<Vec2> listPos;
	for (auto item : m_items) {
		item->setVisible(true);
		item->setTouchHandleType(GameSapXep_ItemTouchType::TOUCH);
		listPos.push_back(item->getPosition());
	}
	
	this->scheduleOnce([=](float dt) {
		float timeState = 0.5;
		if (m_items.size() == 4) {
			std::vector<std::pair<float, Vec2>> list0 = { {15, listPos[1] + Vec2(-80, -50) + Vec2(0,300)},  {7, listPos[3] + Vec2(-40, 25) + Vec2(0,300)}, {-7, listPos[1] + Vec2(-50, -80) + Vec2(0,300)}, {0, listPos[3] + Vec2(0,300)} };
			std::vector<std::pair<float, Vec2>> list1 = { {-10, listPos[2] + Vec2(50, 80) + Vec2(0,300)},  {-5, listPos[0] + Vec2(40, -25) + Vec2(0,300)}, {10, listPos[2] + Vec2(80, 50) + Vec2(0,300)}, {0, listPos[0] + Vec2(0,300)} };
			std::vector<std::pair<float, Vec2>> list2 = { {10, listPos[3] + Vec2(-50, 80) + Vec2(0,300)},  {5, listPos[1] + Vec2(-40, -25) + Vec2(0,300)}, {-7, listPos[0] + Vec2(50, -80) + Vec2(0,300)}, {0, listPos[1] + Vec2(0,300)} };
			std::vector<std::pair<float, Vec2>> list3 = { {-5, listPos[0] + Vec2(50, -80) + Vec2(0,300)},  {-10, listPos[2] + Vec2(40, 25) + Vec2(0,300)}, {-5, listPos[3] + Vec2(-50, 80) + Vec2(0,300)}, {0, listPos[2] + Vec2(0,300)} };
			mj::helper::shuffle(m_items);
			m_items[0]->runAction(Sequence::create(MoveTo::create(0.2f, m_items[0]->getPosition() + Vec2(0, 300)), DelayTime::create(0.5f), getAction(list0, 0.5), DelayTime::create(0.5f), MoveTo::create(0.2f, m_items[3]->getPosition()), nullptr));
			m_items[1]->runAction(Sequence::create(MoveTo::create(0.2f, m_items[1]->getPosition() + Vec2(0, 300)), DelayTime::create(0.5f), getAction(list1, 0.5), DelayTime::create(0.5f), MoveTo::create(0.2f, m_items[2]->getPosition()), nullptr));
			m_items[2]->runAction(Sequence::create(MoveTo::create(0.2f, m_items[2]->getPosition() + Vec2(0, 300)), DelayTime::create(0.5f), getAction(list2, 0.5), DelayTime::create(0.5f), MoveTo::create(0.2f, m_items[0]->getPosition()), nullptr));
			m_items[3]->runAction(Sequence::create(MoveTo::create(0.2f, m_items[3]->getPosition() + Vec2(0, 300)), DelayTime::create(0.5f), getAction(list3, 0.5), DelayTime::create(0.5f), MoveTo::create(0.2f, m_items[1]->getPosition()), nullptr));
		}
		}, 0.5, "delay");
}

void GameSapXep::autoPlay()
{
	schedule([=](float) {
	}, 4.0f, 100, 5.0f, "auto_lay");

}

void GameSapXep::showTutorial(float dt) {
	Point start, stop;
	auto a = m_slots.at(0);
	for (auto o : m_items) {
		if (o->getTag() == a->getTag()) {
			auto bb_start = a->getBoundingBox();
			auto bb_stop = o->getBoundingBox();
			stop = Point(bb_start.getMidX(), bb_start.getMidY());
			stop = a->getParent()->convertToWorldSpace(stop);
			start = Point(bb_stop.getMidX(), bb_stop.getMidY());
			start = o->getParent()->convertToWorldSpace(start);
			break;
		}
	}
	this->schedule([=](float) {
		this->showGuide(start, stop);
		}, dt, 100, 10.0f, "show_guide_matching");
}


void GameSapXep::showGuide(Point start, Point stop) {

	auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
	this->addChild(hand, INT_MAX - 1);
	hand->setPosition(start - Point(0, 25));
	hand->setName("guidingHand");

	hand->runAction(Sequence::create(MoveTo::create(1.2f, stop), RemoveSelf::create(), nullptr));
}

void GameSapXep::stopGuiding() {
	this->unschedule("show_guide_matching");
	this->unschedule("inActiveMode");
	if (auto hand = utils::findChild<ImageView*>(this, "guidingHand")) {
		hand->stopAllActions();
		hand->removeFromParent();
	}
}

void GameSapXep::buttonPlayClick() {
	buttonPlay->addClickEventListener([=](Ref*) {
		this->unschedule("next_game");
		buttonPlay->setVisible(false);
		buttonBGNext->setVisible(false);
		playSequence(0, [=]() {
			playEndingScript();
			});
		});
}

void GameSapXep::buttonNextClick() {
	buttonBGNext->addClickEventListener([=](Ref*) {
		playEndingScript();
		buttonBGNext->setTouchEnabled(false);
		buttonPlay->setTouchEnabled(false);
	});
}