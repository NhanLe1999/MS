#include "GameOChu.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Math_Func.h"
#include "ParseJSONHelper.hpp"
#include "PoolManager.h"
std::once_flag game_o_chu_reader;


#define SOUND_INTRO "sounds/vm_o_chu/intro.mp3"
#define SOUND_BGM "sounds/vm_o_chu/bgm.mp3"
#define SOUND_CORRECT "sounds/vm_o_chu/correct_%d.mp3"
#define SOUND_WRONG "sounds/vm_o_chu/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/vm_o_chu/touch_%d.mp3"
#define SOUND_DROP "sounds/vm_o_chu/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/vm_o_chu/moveback_2.mp3"

#define GAF_DROP "gaf/vm_o_chu/drop_effect/drop_effect.gaf"

#define CSB_NAME "csb/game/GameOChu/GameOChu.csb"



INITIALIZE_READER(GameOChu);

GameOChu * GameOChu::createGame(std::string json_file)
{
	std::call_once(game_o_chu_reader, [] {
		REGISTER_CSB_READER(GameOChu);
	});
	auto p = static_cast<GameOChu *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		//p->retain();
		p->didLoadFromCSB(json_file);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

GameOChu * GameOChu::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(game_o_chu_reader, [] {
		REGISTER_CSB_READER(GameOChu);
	});
	auto p = static_cast<GameOChu *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p->init()) {
		p->didLoadFromCSB(json_file, images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void GameOChu::didLoadFromCSB(std::string json_file)
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse< rapidjson::kParseDefaultFlags>(str.c_str());
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	this->generateData(document.GetObject(), images);

}

void GameOChu::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
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
	this->generateData(document.GetObject(), images);
}


void GameOChu::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
	// Game config
	rapidjson::Value& value_config = JSON_PARSE.getChild(value_data, "game_config");
	auto img_bg_id = JSON_PARSE.getString("", value_config, "background");
	auto img_char_id = JSON_PARSE.getString("", value_config, "icon");

	// Data config

	rapidjson::Value& value_game = JSON_PARSE.getChild(value_data,"data");
	for (int i = 0; i < value_game.Size(); ++i) {
		rapidjson::Value& singleGame = value_game[i];
		GameOChuInfo info;

		auto img_ques_id = JSON_PARSE.getString("",singleGame, "question");
		auto img_ques = math::resource::ResHelper::getImage(images, img_ques_id);
		info.question.text_full = img_ques->getQuestionVMonkey("name_1", lang);
		info.question.audio_full = img_ques->getAudios("name_1", lang).front();

		auto img_ans_id = JSON_PARSE.getString("", singleGame, "answer");
		auto img_ans = math::resource::ResHelper::getImage(images, img_ans_id);
		info.answer.text_full = img_ans->getText("name_1", lang);
		info.answer.audio_full = img_ans->getAudios("name_1", lang).front();


		rapidjson::Value& list_phonic = JSON_PARSE.getChild(singleGame, "list_phonic");
		for (int i = 0; i < list_phonic.Size(); ++i) {
			mj::PHONIC_INFO pi;
			auto img_phonic = math::resource::ResHelper::getImage(images, list_phonic[i]["text"].GetString());
			pi.text_full = pi.text_phonic = img_phonic->getText("name_1",lang);
			pi.audio_full = pi.audio_phonic = img_phonic->getAudios("name_1", lang).front();
			info.sync_text.push_back(pi);
            if (list_phonic[i].HasMember("is_drag")){
                info.is_drags.push_back(list_phonic[i]["is_drag"].GetBool());
            }else{
                info.is_drags.push_back(true);
            }
		}

		if (singleGame.HasMember("distracting")) {
			rapidjson::Value& list_distracting = JSON_PARSE.getChild(singleGame, "distracting");
			for (int i = 0; i < list_distracting.Size(); ++i) {
				mj::PHONIC_INFO pi;
				auto img_phonic = math::resource::ResHelper::getImage(images, list_distracting[i]["text"].GetString());
				pi.text_full = pi.text_phonic = img_phonic->getText("name_1", lang);
				pi.audio_full = pi.audio_phonic = img_phonic->getAudios("name_1", lang).front();
				info.distracting.push_back(pi);
			}
		}

		m_game_data.push_back(info);
	}

}

void GameOChu::onEnter()
{
	MJMainGBase::onEnter();
	loadFrames();
	playBackgroundMusic(SOUND_BGM);
	showIntroGame();
}

void GameOChu::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);

	m_layout_root = utils::findChild<ui::Layout*>(this, "root_layout");
	m_btn_loa = utils::findChild<ui::Button*>(this, "btn_speaker");
	m_lbl_question = utils::findChild < ui::Text*>(this, "lbl_question");
	m_bg_question = utils::findChild<ui::ImageView*>(this, "question_bg");
    
	m_btn_loa->addClickEventListener(CC_CALLBACK_1(GameOChu::onClickedSpeaker, this));
	m_bg_question->addClickEventListener(CC_CALLBACK_1(GameOChu::onClickedSpeaker, this));

	m_btn_loa->setTouchEnabled(true);
	m_bg_question->setTouchEnabled(true);

	m_layout_locked = cocos2d::ui::Layout::create();
	m_layout_locked->setContentSize(screen_size);
	m_layout_locked->setTouchEnabled(true);
	this->addChild(m_layout_locked, 1000000);

	m_lbl_question->setFontName(font_name);

}

void GameOChu::showIntroGame()
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

void GameOChu::startGame()
{
	initQuestion();
	onClickedSpeaker(m_btn_loa);
	showItemsAndSlots();
	enableControlQuiz();
	onShowGuideGame(15.0f);
}

void GameOChu::onFinishGameVMonkey()
{
	MJMainGBase::onFinishGameVMonkey();
}

void GameOChu::update(float dt)
{
}

void GameOChu::onNextGame()
{
	++m_game_index;
	if (m_game_index < m_game_data.size()) {
		startGame();
	}
	else {
		this->scheduleOnce([=](float) {
			onFinishGameVMonkey();
		}, 0.5f, "delay endgame");
	}
}


void GameOChu::onExitTransitionDidStart()
{
    MJMainGBase::onExitTransitionDidStart();
}

void GameOChu::onExit()
{
	MJMainGBase::onExit();
}

void GameOChu::onRefreshGame()
{
	for (auto item : m_items) {
		item->onHide(0.0f);
	}

	for (auto slot : m_slots) {
		slot->onHide(0.0f);
	}
}

void GameOChu::setTouchLock(bool locked)
{
	m_layout_locked->setTouchEnabled(locked);
}

void GameOChu::onClickedSpeaker(Ref * sender)
{
    AUDIO_ENGINE->stopSound(m_current_data.question.audio_full);
	q_id = AUDIO_ENGINE->playSound(m_current_data.question.audio_full);
}


void GameOChu::initQuestion()
{
	count_wrong = 0;
	m_current_data = m_game_data.at(m_game_index);
	m_lbl_question->setString(m_current_data.question.text_full);
    auto lbl_height = m_lbl_question->getVirtualRendererSize().height;
    auto bg_height = m_bg_question->getContentSize().height;
    if (lbl_height > bg_height){
        m_bg_question->setScale9Enabled(true);
        m_bg_question->setContentSize(Size(700,190));
        m_lbl_question->setPosition(m_bg_question->getContentSize()/2);
        m_lbl_question->setContentSize(Size(700,190));
        m_lbl_question->setFontSize(37);
    }
	m_items.clear();
	m_slots.clear();
    m_items_no_drag.clear();
	auto slot_size = Size(200, 200);
	auto blank_image = math::resource::ResHelper::createImage("");

	auto slot_bg = math::resource::ResHelper::createImage("games/vm_o_chu/Vector Smart Object.png");
	
	int index = 0;
	for (auto text : m_current_data.sync_text) {
		auto slot = mpool->getSlot();
		slot->setEndble(index, slot_bg);
		slot->setSlotSize(slot_size);
		slot->setValueText("");
		slot->setVisible(false);
		m_layout_root->addChild(slot, 10);
		m_slots.push_back(slot);

		auto item = mpool->getObject();
		item->setEnable(index, math::common::move, blank_image, nullptr);
		item->setValueText(mj::helper::to_upper(text.text_full));
		
		item->setObjectSize(slot_size);
		item->getLabelValue()->setFontSize(0.65f * slot_size.height);
		item->getLabelValue()->disableEffect();
		item->setValueTextPosition("middle");
		item->fitValueTextSize();
		
		item->setSuperNode(m_layout_root);
		item->setMoveCallback(CC_CALLBACK_1(GameOChu::onMoveBegan, this), CC_CALLBACK_2(GameOChu::onMoveEnded, this));
		item->setImageVisible(false);
		item->setVisible(false);
		m_layout_root->addChild(item, 20);
        if (m_current_data.is_drags[index]){
            m_items.push_back(item);
        }else{
            m_items_no_drag.push_back(item);
            item->setPosition(slot->getPosition());
            slot->setLinker(item);
            item->setTouch(math::common::TouchPermission::deny);
        }
		++index;
	}

	for (auto text : m_current_data.distracting) {
		auto item = mpool->getObject();
		item->setEnable(index, math::common::move, blank_image, nullptr);
		item->setValueText(mj::helper::to_upper(text.text_full));
		
		item->setImageVisible(false);
		item->setObjectSize(slot_size);
		item->getLabelValue()->setFontSize(0.65f * slot_size.height);
		item->getLabelValue()->disableEffect();
		item->setValueTextPosition("middle");
		item->fitValueTextSize();

		item->setSuperNode(m_layout_root);
		item->setMoveCallback(CC_CALLBACK_1(GameOChu::onMoveBegan, this), CC_CALLBACK_2(GameOChu::onMoveEnded, this));
		item->setVisible(false);
		m_layout_root->addChild(item, 20);
		m_items.push_back(item);

		++index;
	}

	auto answer_bg = utils::findChild<ui::ImageView*>(this, "bg_answer");
	auto item_middle = answer_bg->getPosition();
	auto item_origin = answer_bg->getPosition() - answer_bg->getContentSize() / 2;
	auto space_item = answer_bg->getContentSize() - Size(0, 100);

	mj::helper::shuffle(m_items);
	math::func::setPositionListOnFrame(m_items, m_items.size(), 1, Size(200, 200),item_middle, 0);
	math::func::setPositionListOnFrame(m_slots, m_slots.size(),1, Size(250, 250), Vec2(screen_size.width / 2, screen_size.height/2), 0);
}

void GameOChu::showItemsAndSlots()
{

	for (auto item : m_items) {
		item->setVisible(true);
		item->onShow(0.25f);
		item->showValueTextWithAnimation(true);
	}

	for (auto slot : m_slots) {
		slot->setVisible(true);
		slot->onShow(0.25f);
        if (slot->isUsed()){
            slot->getLinker()->setVisible(true);
            slot->getLinker()->onShow(0.25f);
            slot->getLinker()->showValueTextWithAnimation(true);
            slot->getLinker()->setPosition(slot->getPosition());
        }
	}

	setTouchLock(false);
}

void GameOChu::onMoveBegan(cocos2d::Node *node) {
	hideTapGuideGame();
	auto object = static_cast<Math_Object*>(node);
	//object->onDragObject();
	AUDIO_ENGINE->playSound(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)));

	// lock các object khác

	for (int i = 0; i < m_items.size(); ++i) {
		if (m_items[i] != object) {
			m_items[i]->setLocked(true);
		}
	}
}

void GameOChu::onMoveEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
	auto object = static_cast<Math_Object*>(node);
	auto object_index = object->getID();
	bool addable = false;
	for (Math_Slot* slot : m_slots) {
		if (!slot->isUsed() && isPointInnerSlot(slot, position, 0)) {
			//In slot not use
			int slot_id = slot->getId();
			if (slot_id == object_index) {
				addable = true;
			}
			else {
				addable = false;
			}

			if (addable) {
				onPair(object, slot);
				break;
			}
		}
	}

	// enable các object chưa được di chuyển
	for (auto object : m_items) {
		if (object->isLocked() && object->getTouch() == math::common::move) {
			object->setLocked(false);
		}
	}


	if (!addable) {
		++count_wrong;
		if (count_wrong > 3) {
			count_wrong = 0;
			onShowGuideGame(1.0f);
		}
		disableControlQuiz(0.25f);
		if (object->getPosition().distance(object->getSeedPosition()) > 20) {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_MOVEBACK, cocos2d::random(1, 3)));
		}
		object->backToStart();
	}
	else {
		count_wrong = 0;
		for (Math_Slot * slot : m_slots) {
			if (!slot->isUsed()) {
				return;
			}
		}
		disableControlQuiz();
		//stopSpeakQuestion();
		AUDIO_ENGINE->stopSound(m_current_data.question.audio_full);
		//setState(Math_Quiz::QuizState::PAUSING);
		scheduleOnce([=](float) {
			playEndingScript();
		}, 0.5, "explain");
	}
}

bool GameOChu::isPointInnerSlot(Math_Slot * slot, cocos2d::Vec2 point, int padding)
{
	cocos2d::Rect slot_bound = slot->getBoundingBox();
	cocos2d::Vec2 center = slot->getWorldPosition();

	slot_bound.origin = cocos2d::Vec2(center.x - slot_bound.size.width / 2 - padding, center.y - slot_bound.size.height / 2 - padding);
	slot_bound.size = cocos2d::Size(slot_bound.size.width + padding * 2, slot_bound.size.height + padding * 2);
	if (slot_bound.containsPoint(point)) {
		return true;
	}
	return false;
}

void GameOChu::onShowGuideGame(float dt)
{
	hideTapGuideGame();
	for (auto slot : m_slots) {
		if (!slot->isUsed()) {
			for (auto item : m_items) {
				if (item->getID() == slot->getId()) {
					showDragGuideGame(item->getPosition(), slot->getPosition(), 100, dt, 15);
				}
			}
			break;
		}
	}
}

void GameOChu::disableControlQuiz(float time) {
	unschedule("disable_touch_quiz");
	m_layout_locked->setVisible(true);
	m_layout_locked->setTouchEnabled(true);
	if (time > 0) {
		scheduleOnce([=](float dt) {
			m_layout_locked->setTouchEnabled(false);
			m_layout_locked->setVisible(false);
		}, time, "disable_touch_quiz");
	}
}

void GameOChu::enableControlQuiz() {
	unschedule("disable_touch_quiz");
	m_layout_locked->setTouchEnabled(false);
	m_layout_locked->setVisible(false);
}

void GameOChu::onPair(Math_Object * item, Math_Slot * slot)
{
	hideTapGuideGame();
	AUDIO_ENGINE->playSound(StringUtils::format(SOUND_CORRECT, cocos2d::random(1, 3)));
	item->setTouch(math::common::deny);
	item->setLocked(true);
	slot->setLinker(item);
	item->runAction(cocos2d::EaseBackOut::create(Spawn::create(cocos2d::MoveTo::create(0.1, slot->getPosition()), ScaleTo::create(0.1f, 1.0f / 0.9), NULL)));
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
}

void GameOChu::zoomTo(Ref * sender, Vec2 position, float scale, float duration, std::function<void()> callback)
{
	auto item = (Layout*)sender;
	item->runAction(Sequence::create(
		Spawn::createWithTwoActions(
			ScaleTo::create(duration, scale),
			MoveTo::create(duration, position)
		)
		,
		CallFunc::create([=] {
		if (callback) callback();
	}), NULL));
}

void GameOChu::playEndingScript()
{
	for (auto slot : m_slots) {
		slot->getLinker()->runAction(Blink::create(2.0f,5));
	}
	AUDIO_ENGINE->playSound(m_current_data.answer.audio_full, false, [=] {
		onNextGame();
	});
	
}



void GameOChu::autoPlay()
{
	schedule([=](float) {

	}, 4.0f, 100, 5.0f, "auto_lay");

}
