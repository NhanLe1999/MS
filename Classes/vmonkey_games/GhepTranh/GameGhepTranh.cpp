//
//  GameGhepTranh.cpp
//
//  Created by MinhTzy on 02/05/2019.
//
//

#include "GameGhepTranh.h"

#define SOUND_CORRECT "sounds/vm_ghep_tranh/correct_%d.mp3"
#define SOUND_WRONG "sounds/vm_ghep_tranh/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/vm_ghep_tranh/touch_%d.mp3"
#define SOUND_DROP "sounds/vm_ghep_tranh/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/vm_ghep_tranh/moveback_%d.mp3"

#define GAF_DROP "gaf/vm_ghep_tranh/drop_effect/drop_effect.gaf"

#define CSB_NAME "csb/game/vm_ghep_tranh/GameGhepTranh.csb"

INITIALIZE_READER(GameGhepTranh);

std::once_flag GameGhepTranh_reader;

GameGhepTranh* GameGhepTranh::createGame(std::string json_path)
{
	std::call_once(GameGhepTranh_reader, [] {
		REGISTER_CSB_READER(GameGhepTranh);
	});
	auto quiz = static_cast<GameGhepTranh *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (quiz->init()) {
		quiz->initGame(json_path);
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool GameGhepTranh::init() {
	return MJMainGBase::init();
}


#pragma mark - Quiz Life-cycle
void GameGhepTranh::initGame(std::string json_path)
{
	//Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("data")->value;
	//auto data_manager = setupDataManager(json_data);

	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	std::vector<math::resource::Image*> all_imgs = math::resource::ResHelper::getImages("icons.json");
	//auto all_imgs = data_manager->getImages(0, "pair");
	for (math::resource::Image* img : all_imgs) {
		int role = img->getComparisionValue("role");
		if (role == 0) {
			img_complete = img;
		}
		else if (role == 1 || role == 2) {
			img_suggess = img;
			img_target = img;
		}
		else if (role == 3) {
			img_start = img;
		}
		else {
			img_list.push_back(img);
		}
	}
	std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool {
		return a->getComparisionValue("number") < b->getComparisionValue("number");
	});
	std::vector<math::resource::Image*> imgs;
	imgs.push_back(img_complete);

	//parseQuestion();
	//auto opt_keys = getQuestion()->getOptionalKeys();
	//for (std::string key : opt_keys) {
	//	getQuestion()->setOptionalTexts(key, imgs);
	//}

	if (JSON_PARSE.getBool(false, json_data, "game", "suggess_front")) {
		suggess_index = INT_MAX - 10;
	}
	//Gen object
	screen_size = Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);
	image_size = JSON_PARSE.getSize(json_data, "game", "picture_size");
	image_center = cocos2d::Vec2(screen_size.width/2 + 255, 300);
	float scale_value = 1;
	start_size = cocos2d::Size(500, 592);
	start_center = cocos2d::Vec2(screen_size.width / 2 - 255 , 300);

	comp_object = mpool->getObject();
	comp_object->setEnable(0, math::common::deny, img_complete, nullptr);
	comp_object->setObjectSize(image_size);
	comp_object->fixWidthObject();
	image_size = comp_object->getContentSize();
	comp_object->setPosition(image_center);

	scale_value = image_size.width / comp_object->getImage()->getVirtualRendererSize().width * 0.9f;

	sugg_object = mpool->getObject();
	sugg_object->setEnable(1, math::common::deny, img_suggess, nullptr);
	sugg_object->setObjectSize(image_size);
	sugg_object->fixWidthObject();
	sugg_object->setPosition(image_center);

	/*---- PARSE DATA -----*/
	trace_data = math::JSON::MMTrace::create(img_target->tracing);
	trace_start = math::JSON::MMTrace::create(img_start->tracing);

	//cocos2d::Size slot_size = cocos2d::Size(80, 80);
	std::vector<Vec2> locations;
	for (int i = 0; i < trace_data.points.size(); i++) {
		int index = trace_data.points[i].index;
		math::resource::Image* image = nullptr;
		for (math::resource::Image* img : img_list) {
			if (img->getComparisionValue("number") == index) {
				image = img;

				auto object = mpool->getObject();
				object->setEnable(index, math::common::move, image, nullptr);
				auto object_size = object->getImage()->getVirtualRendererSize() * scale_value;
				object->setObjectSize(object_size);
				object->setTouchDetectedType(math::common::nontrans);
				object->setMoveCallback(CC_CALLBACK_1(GameGhepTranh::onMoveObjectBegan, this), CC_CALLBACK_2(GameGhepTranh::onMoveObjectEnded, this));
				locations.push_back(trace_start.getLocation(index, start_center, start_size));
				//object->setPosition(trace_start.getLocation(index, start_center, start_size));
				_objects.push_back(object);

				auto slot = mpool->getSlot();
				slot->setEndble(index, nullptr);
				slot->setSlotSize(object_size);
				slot->setPosition(trace_data.getLocation(index, image_center, image_size));
				_slots.push_back(slot);
			}
		}
	}
	/*mj::helper::shuffle(locations);
	for (int i = 0; i < _objects.size(); ++i) {
		_objects[i]->setPosition(locations[i]);

	}*/
	//math::func::setPositionsRandom(_objects, Rect(start_center - start_size/2, start_size));
	mj::helper::shuffle(_objects);
	math::func::smartSetPosition(_objects,Size(200,150), Rect(start_center - start_size / 2, start_size));
}
void GameGhepTranh::randomShow(std::vector<Math_Object*> list, float delay, math::animation::ObjectShowType show) {
	auto list_size = static_cast<int>(list.size());
	std::vector<int> random_show;
	for (int i = 0; i < list_size; i++) {
		random_show.push_back(i);
	}
	int number_div = (list_size + 4) / 5;
	if (number_div > 1) {
		math::func::shuffle(random_show);
	}
	for (int i = 0; i < list_size; i++) {
		list[i]->onShow((random_show[i] / number_div)*0.1 + delay, show);
	}
}
bool GameGhepTranh::isPointInnerSlot(Math_Slot * slot, cocos2d::Vec2 point, int padding)
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
void GameGhepTranh::loadFrames()
{

	m_layout_root = utils::findChild<ui::Layout*>(this, "root_layout");
	m_btn_loa = utils::findChild<ui::Button*>(this, "btn_loa");
	m_lbl_question = utils::findChild < ui::Text*>(this, "lbl_question");
	m_bg_question = utils::findChild<ui::ImageView*>(this, "question_bg");
	_panel = utils::findChild<ui::Layout*>(this, "panel");

	m_btn_loa->addClickEventListener(CC_CALLBACK_1(GameGhepTranh::onClickedSpeaker, this));
	m_bg_question->addClickEventListener(CC_CALLBACK_1(GameGhepTranh::onClickedSpeaker, this));

	m_btn_loa->setTouchEnabled(true);
	m_bg_question->setTouchEnabled(true);

	m_layout_locked = cocos2d::ui::Layout::create();
	m_layout_locked->setContentSize(screen_size);
	m_layout_locked->setTouchEnabled(false);
	this->addChild(m_layout_locked, 1000000);

	m_lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	m_lbl_question->setString(" ");
}
void GameGhepTranh::onClickedSpeaker(Ref * sender)
{
	if (img_complete)
		AUDIO_ENGINE->playSound(img_complete->getAudios("name_1", lang).front());
}
void GameGhepTranh::disableControlQuiz(float time) {
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

void GameGhepTranh::enableControlQuiz() {
	unschedule("disable_touch_quiz");
	m_layout_locked->setTouchEnabled(false);
	m_layout_locked->setVisible(false);
}
void GameGhepTranh::onStart() {
	//quizBegan(0.5, true);

	//auto line = cocos2d::ui::ImageView::create(getUIPath("common/line/line_1.png"));
	//line->setPosition(cocos2d::Vec2(screen_size.width*0.5, screen_size.height*0.5));
	//addChildNode(line);
	//ui_elements.push_back(line);
	if (img_complete)
		m_lbl_question->setString(img_complete->getQuestionVMonkey("name_1", lang));
	addChildNode(sugg_object, suggess_index);
	sugg_object->onShow(0);

	mj::helper::shuffle(_objects);
	for (Math_Object* obj : _objects) {
		addChildNode(obj, obj->getID());
		
	}
	randomShow(_objects);

	for (Math_Slot* slot : _slots) {
		addChildNode(slot, 0);
		slot->onShow(0);
	}
}

void GameGhepTranh::onStartQuestion() {
	enableControlQuiz();
}

void GameGhepTranh::onExplanation(bool correct) {
	disableControlQuiz();
	if (correct) {
		complete = true;

		for (Math_Object* object : _objects) {
			object->onHide(0, math::animation::No_Effect);
		}
		sugg_object->onHide(0, math::animation::No_Effect);
		comp_object->setPosition(image_center);
		addChildNode(comp_object);
		comp_object->onShow(0, math::animation::No_Effect);
		scheduleOnce([=](float dt) {
			//for (auto child : ui_elements) {
			//	child->runAction(cocos2d::FadeOut::create(0.25));
			//}
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_CORRECT, cocos2d::random(1, 3)));
			comp_object->runAction(cocos2d::Spawn::createWithTwoActions(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(screen_size.width*0.5, image_center.y)), cocos2d::ScaleTo::create(0.5, 1.5)));
			comp_object->playAudio();
		}, 0.5, "go complete");

		//onEndingDisplay([=](std::string) {
			nextQuestion(2.5f);
		//}, 1.5);
	}
}

void GameGhepTranh::onComplete() {
	//TODO: Thuc hien them cac lenh o day
	if (complete) {
		comp_object->onHide(0);
	}
	else {
		for (Math_Object* object : _objects) {
			object->onHide(0, math::animation::No_Effect);
		}
		sugg_object->onHide(0, math::animation::No_Effect);
	}
	this->scheduleOnce([=](float) {
		MJMainGBase::onFinishGameVMonkey();
	}, 0.15f, "finnish_game");
	
}

void GameGhepTranh::onGuideStarted() {
	Math_Slot* slot = nullptr;
	for (auto _slot : _slots) {
		if (!_slot->isUsed()) {
			slot = _slot;
			break;
		}
	}
	if (slot) {
		for (auto object : _objects) {
			if (!object->isLocked()) {
				bool addable = false;
				int slot_id = slot->getId();
				if (slot_id == object->getID()) {
					addable = true;
				}
				else {
					math::JSON::MMPart family;
					for (int i = 0; i < trace_data.paths.size(); i++) {
						for (int index : trace_data.paths[i].points) {
							if (index == object->getID()) {
								family = trace_data.paths[i];
								break;
							}
						}
					}
					for (int value : family.points) {
						if (slot_id == value) {
							addable = true;
						}
					}
				}
				if (addable) {
					GuideCenter->guideByDragDrop(object->getPosition(), slot->getWorldPosition());
					return;
				}
			}
		}
	}
	GuideCenter->guideCount();
}

void GameGhepTranh::nextQuestion(float dt)
{
	this->scheduleOnce([=](float) {
		onComplete();
	}, dt, "next question");
}

void GameGhepTranh::onEnter()
{
	MJMainGBase::onEnter();
	loadFrames();
	onStart();
}

#pragma mark - object touch events callback
void GameGhepTranh::onMoveObjectBegan(cocos2d::Node *node) {
	auto object = static_cast<Math_Object*>(node);
	//object->onDragObject();
	AUDIO_ENGINE->playSound(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)));

	// lock các object khác

	for (int i = 0; i < _objects.size(); ++i) {
		if (_objects[i] != object) {
			_objects[i]->setLocked(true);
		}
	}
}

void GameGhepTranh::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
	auto object = static_cast<Math_Object*>(node);
	auto object_index = object->getID();
	bool addable = false;
	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed() && isPointInnerSlot(slot,position, 50)) {
			//In slot not use
			int slot_id = slot->getId();
			if (slot_id == object_index) {
				addable = true;
			}
			else {
				//math::JSON::MMPart family;
				//for (int i = 0; i < trace_data.paths.size(); i++) {
				//	for (int index : trace_data.paths[i].points) {
				//		if (index == object_index) {
				//			family = trace_data.paths[i];
				//			break;
				//		}
				//	}
				//}
				//for (int value : family.points) {
				//	if (slot_id == value) {
				//		addable = true;
				//	}
				//}
				addable = false;
			}

			if (addable) {
				GuideCenter->resetIncorrectCount();
				object->removeFromParentAndCleanup(false);
				addChildNode(object, object_index);
				object->setTouch(math::common::deny);
				object->setLocked(true);
				slot->setLinker(object);
				object->runAction(cocos2d::EaseBackOut::create(Spawn::create(cocos2d::MoveTo::create(0.1, slot->getPosition()), ScaleTo::create(0.1f, 1.0f / 0.9), NULL)));
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
				break;
			}
			else {
				GuideCenter->increamentIncorrect();
			}
		}
	}

	// enable các object chưa được di chuyển
	for (auto object : _objects) {
		if (object->isLocked() && object->getTouch() == math::common::move) {
			object->setLocked(false);
		}
	}


	if (!addable) {
		disableControlQuiz(0.25);
		if (object->getPosition().distance(object->getSeedPosition()) > 20) {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_MOVEBACK,cocos2d::random(1,3)));
		}
		object->backToStart();
	}
	else {
		for (Math_Slot * slot : _slots) {
			if (!slot->isUsed()) {
				return;
			}
		}
		disableControlQuiz();
		//stopSpeakQuestion();
		//setState(Math_Quiz::QuizState::PAUSING);
		scheduleOnce([=](float) {
			this->onExplanation(true);
		}, 0.5, "explain");
	}
}

void GameGhepTranh::addChildNode(Node * node, int zOrder)
{
	_panel->addChild(node, zOrder);
}
