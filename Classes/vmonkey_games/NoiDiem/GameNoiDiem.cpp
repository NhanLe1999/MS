//
//  GameNoiDiem.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "GameNoiDiem.h"
#define detect_size cocos2d::Size(50, 50)
#define detect_size_level 3

#define step_draw 1.f
#define line_draw 15.f
#define range_detect 40.f
#define range_active 25.f

INITIALIZE_READER(GameNoiDiem);
std::once_flag GameNoiDiem_reader;
#define CSB_NAME "csb/game/vm_noi_diem/GameNoiDiem.csb"


GameNoiDiem * GameNoiDiem::createGame(std::string configPath)
{
	std::call_once(GameNoiDiem_reader, [] {
		REGISTER_CSB_READER(GameNoiDiem);
	});
	GameNoiDiem* quiz = static_cast<GameNoiDiem*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (quiz->init()) {

		quiz->didLoadFromCSB(configPath);
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

void GameNoiDiem::didLoadFromCSB(std::string configPath)
{
	m_configPath = configPath;
	loadFrames();
}

bool GameNoiDiem::init() {
	return MJMainGBase::init();
}


#pragma mark - Quiz Life-cycle

void GameNoiDiem::generateData()
{
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(m_configPath);

	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif

	rapidjson::Value& json_data = document.FindMember("data")->value;

	tracing_size = /*Size(500, 500);*/cocos2d::Size(json_data.FindMember("size")->value.FindMember("width")->value.GetInt(), json_data.FindMember("size")->value.FindMember("height")->value.GetInt());


	std::vector<std::string> colors;
	colors.push_back("red");
	colors.push_back("pink");
	colors.push_back("purple");
	colors.push_back("orange");
	colors.push_back("blue");
	color_tracing = math::func::getRandomNode(colors);
	color_line = cocos2d::Color4F(getColor3B(color_tracing));
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	//TODO: GAME CONFIG
	time_hint = 10;
	shuffle_data = false;
	hint_drawer = false;
	hint_animation = false;
	if (json_data.HasMember("game_config")) {
		rapidjson::Value& game_config = json_data["game_config"];
		time_hint = JSON_PARSE.getInt(10, game_config, "hint_time");
		shuffle_data = JSON_PARSE.getBool(false, game_config, "play_random");
		hint_drawer = JSON_PARSE.getBool(false, game_config, "hint_drawer");
		hint_animation = JSON_PARSE.getBool(false, game_config, "hint_animation");

	}
	//TODO: DATA CONFIG
	rapidjson::Value& data_config = JSON_PARSE.getChild(json_data, "data_config");
	if (data_config.IsNull()) return;

	std::string intro = JSON_PARSE.getString("", data_config, "intro");
	auto intro_img = math::resource::ResHelper::getImage(images, intro);
	if (intro_img != nullptr)
		sound_intro = intro_img->getAudios("name_1").front();

	rapidjson::Value& question_data = JSON_PARSE.getChild(data_config, "data");
	for (int i = 0; i < question_data.Size(); ++i) {
		GameNoiDiem_Info info;
		rapidjson::Value& questions = JSON_PARSE.getChild(question_data[i], "questions");
		for (int j = 0; j < questions.Size(); ++j) {
			std::string question = JSON_PARSE.getString("", questions[i], "item");
			auto image_question = math::resource::ResHelper::getImage(images, question);
			auto text = image_question->getQuestionVMonkey("name_1", lang);
			auto audio = image_question->getAudios("name_1", lang).front();
			info.question.append(text).append(" ");
			info.question_audios.push_back(audio);
		}
		std::string tracing = JSON_PARSE.getString("", question_data[i], "tracing");
		info.image_tracing = math::resource::ResHelper::getImage(images, tracing);

		info.image_correct = nullptr;
		if (question_data[i].HasMember("answer")) {
			std::string answer = JSON_PARSE.getString("", question_data[i], "answer");
			info.image_correct = math::resource::ResHelper::getImage(images, answer);
		}

		game_info.push_back(info);
	}

	rapidjson::Value& optional_data = JSON_PARSE.getChild(data_config, "optional");
	tracing_items.clear();
	for (int i = 0; i < optional_data.Size(); ++i) {
		auto image = math::resource::ResHelper::getImage(images, JSON_PARSE.getString("", optional_data[i], "item"));
		tracing_items.push_back(image);
	}
}

void GameNoiDiem::finishQuestion()
{
	if (game_object) {
		game_object->onHide(0.15f);
	}
	if (lbl_tracing) {
		lbl_tracing->runAction(Sequence::create(DelayTime::create(0.15f), FadeOut::create(0.25f), RemoveSelf::create(), NULL));
	}
	if (img_character) {
		img_character->runAction(Sequence::create(DelayTime::create(0.15f), FadeOut::create(0.25f), RemoveSelf::create(), NULL));
	}
}


void GameNoiDiem::onEnter()
{
	MJMainGBase::onEnter();
	generateData();
	playBackgroundMusic("sonds/vm_noi_diem/tl_bgm.mp3");
	addEvents();
	game_index = -1;
	onStart();
}

void GameNoiDiem::loadFrames()
{
	screen_size = Director::getInstance()->getWinSize();

	this->setContentSize(screen_size);

	auto _bg_quiz = cocos2d::ui::ImageView::create("");
	_bg_quiz->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_bg_quiz->setPosition(screen_size / 2);

	/*---- layout 'move' object ----*/
	_panel_move = Layout::create();
	_panel_move->setContentSize(screen_size);
	_panel_move->setLayoutType(Layout::Type::ABSOLUTE);
	_panel_move->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_panel_move->setBackGroundColor(cocos2d::Color3B::BLACK);
	_panel_move->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel_move->setPosition(screen_size / 2);
	_panel_move->setOpacity(0);
	_panel_move->setSwallowTouches(true);
	this->addChild(_panel_move, 10);

	/*---- Layout main: ----*/
	_panel = this->getChildByName<Layout *>("root_layout");
	lbl_question = utils::findChild<ui::Text*>(_panel, "text_question");
	//lbl_question->ignoreContentAdaptWithSize(true);
	lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	lbl_question->setFontSize(32);
	lbl_question->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbl_question->setTextVerticalAlignment(TextVAlignment::CENTER);

	btn_speaker = utils::findChild<ui::Button*>(_panel, "btn_loa");
	btn_speaker->setTitleText("");
	btn_speaker->addClickEventListener([=](Ref* sender) {
		btn_speaker->setTouchEnabled(false);
		speakQuestion();
	});
	/*---- Layout ngăn tương tác: ----*/
	_panel_disable = Layout::create();
	_panel_disable->setContentSize(screen_size);
	_panel_disable->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel_disable->setPosition(screen_size / 2);
	_panel_disable->setVisible(false);
	this->addChild(_panel_disable, INT_MAX - 1);



}

void GameNoiDiem::onStart() {

	//quizBegan(0, false);
	disableControlQuiz();
	++game_index;
	img_item = nullptr;
	img_tracing = nullptr;
	if (game_index >= game_info.size()) {
		onFinishGameVMonkey();
		return;
	}

	/*---- PARSE DATA -----*/
	current_info = game_info.at(game_index);
	img_tracing = current_info.image_tracing;
	img_item = current_info.image_correct;
	trace_data = math::JSON::MMTrace::create(img_tracing->tracing);


	/*----gen question----*/
	float question_width = lbl_question->getContentSize().width;
	lbl_question->setString(current_info.question);
	//auto size = lbl_question->getVirtualRendererSize();
	//auto question_size = cocos2d::Size(question_width, size.height + 40);
	//auto center = utils::findChild(utils::findChild(this,"question_box"),"box")->getPosition();
	//lbl_question->setContentSize(question_size);
	//lbl_question->setPosition(center);


	/*---- GEN OBJECT -----*/
	auto tracing_box = _panel->getChildByName("tracing_box");
	tracing_center = tracing_box->getPosition() + Vec2(0, tracing_box->getContentSize().height / 2);

	game_object = GameNoiDiem_Item::createItem(0, math::common::deny, img_tracing);
	game_object->setItemSize(tracing_size);
	game_object->setPosition(tracing_center);
	game_object->setImageColor(cocos2d::Color3B::WHITE);
	_panel->addChild(game_object);
	game_object->setSuperNode(_panel_move);

	_drawers.clear();
	gaf_list.clear();
	point_objects.clear();

	std::unordered_map<int, math::JSON::MMPoint> points;
	for (auto point : trace_data.points) {
		points.insert({ point.index,point });
	}
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		int item_index = i % tracing_items.size();
		for (auto index : trace_data.paths[i].points) {
			auto point = points.at(index);
			auto image = tracing_items[item_index];
			GameNoiDiem_Item * object;
			// kiểm tra có phải file gaf không
			if (image->media_type == 3) {
				object = GameNoiDiem_Item::createItem(point.index, math::common::deny, nullptr, image->path);
			}
			else {
				object = GameNoiDiem_Item::createItem(point.index, math::common::deny, image);
			}

			object->setItemSize(detect_size);
			object->setImageVisible(false);
			cocos2d::Vec2 location = trace_data.getLocation(point, tracing_center, tracing_size);
			object->setPosition(location);
			object->setColoring(true);
			_panel->addChild(object, 5);
			object->setSuperNode(_panel_move);
			point_objects.push_back(object);
		}
	}

	// create new drawer
	_drawer = cocos2d::DrawNode::create();
	_panel->addChild(_drawer, 2);
	_drawers.push_back(_drawer);

	startQuestion();
}

void GameNoiDiem::startQuestion() {
	AUDIO_ENGINE->playEffect("sounds/vm_noi_diem/tl_appear.mp3");
	GuideCenter->setGuideEnabled();
	game_object->onShow(0.25f, math::animation::ScaleUp);

	speakQuestion();
	index_path = -1;
	startTracing(true, 0.5f);
}

void GameNoiDiem::nextQuestion(float delay_time)
{
	finishQuestion();
	this->scheduleOnce([=](float dt) {
		onStart();
	}, delay_time, "next question");
}

void GameNoiDiem::onExitTransitionDidStart()
{
	GuideCenter->stopGuide();
	MJMainGBase::onExitTransitionDidStart();
}

void GameNoiDiem::onExplanation(bool correct) {
	disableControlQuiz();
	GuideCenter->setGuideDisabled();

	// stop audio question
	for (auto ques : current_info.question_audios)
		AUDIO_ENGINE->stopSound(ques);
	btn_speaker->setTouchEnabled(false);


	for (auto drawer : _drawers) {
		if (drawer) {
			drawer->clear();
			drawer->removeFromParentAndCleanup(true);
			drawer = nullptr;
		}

	}
	_drawer = nullptr;
	_drawers.clear();

	for (Node* gaf : gaf_list) {
		if (gaf) {
			gaf->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=]() {
				gaf->removeFromParentAndCleanup(true);
			}), NULL));
		}
	}
	gaf_list.clear();

	auto duration = 1.0f;

	auto showEndQuiz = CallFunc::create([=] {
		if (img_item == nullptr) {
			nextQuestion();
		}
		else {
			showImageEndQuiz();
		}

	});
	auto effect = GAFWrapper::createGAF("gaf/vm_noi_diem/tl_sparkle/tl_sparkle.gaf");
	effect->setPosition(game_object->getPosition());
	effect->play(false, [effect](gaf::GAFObject * obj) {
		effect->removeFromParent();
	});
	_panel->addChild(effect);

	AUDIO_ENGINE->playSound("sounds/vm_noi_diem/tl_correct.mp3");
	AUDIO_ENGINE->playSound(img_tracing->getAudios("name_1", lang).front());
	game_object->setImageColor(getColor3B(color_tracing));

	game_object->runAction(
		Sequence::create(
			DelayTime::create(duration), showEndQuiz, NULL));

}

void GameNoiDiem::showImageEndQuiz()
{
	img_character = nullptr;

	AUDIO_ENGINE->playEffect("sounds/vm_noi_diem/tl_imageshow.mp3");
	float duration = 0.5f;
	auto image = ui::ImageView::create(img_item->path);
	image->setScale9Enabled(true);
	auto virtual_size = image->getVirtualRendererSize();
	image->setCapInsets(Rect(1, 1, virtual_size.width - 2, virtual_size.height - 2));

	image->setContentSize(tracing_size);
	image->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	image->setPosition(Vec2(tracing_center));
	image->setOpacity(0);
	image->runAction(FadeIn::create(0.5f));
	_panel->addChild(image, 3);
	img_character = image;
	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(img_item->getAudios("name_1", lang).front(), false, [=] {
			this->scheduleOnce([=](float) {
				nextQuestion();
			}, 1.0f, "next question");
		});
	}, duration, "play image audio");

}

void GameNoiDiem::onFinishGameVMonkey()
{
	GuideCenter->stopGuide();
	this->scheduleOnce([=](float dt) {
		MJMainGBase::onFinishGameVMonkey();
	}, 0.5f, "finish game");

}


void GameNoiDiem::onGuideStarted() {
	disableControlQuiz();
	std::vector<GameNoiDiem_Item*> not_actives;
	for (GameNoiDiem_Item* object : item_list) {
		if (!object->isColoring()) {
			not_actives.push_back(object);
		}
	}
	GuideCenter->resetIncorrectCount();
	if (not_actives.empty()) {
		GuideCenter->guideNone();
	}
	else {
		if (not_actives.size() == 1) {
			GuideCenter->guideByTouch(not_actives.front()->getWorldPosition());
		}
		else {
			std::vector<cocos2d::Vec2> vec2points;
			for (GameNoiDiem_Item* object : not_actives) {
				vec2points.push_back(object->getWorldPosition());
			}
			GuideCenter->guideBySwipe(vec2points);
		}
	}
}

void GameNoiDiem::onGuideDone()
{
	enableControlQuiz();
}

#pragma mark - object touch events callback

bool GameNoiDiem::isInPath(cocos2d::Vec2 point) {
	if (current_line.isPointInnerEdge(point)) {
		auto d0 = current_line.distance(point);
		return d0 <= range_detect;
	}
	else {
		auto d1 = current_line.p1.distance(point);
		auto d2 = current_line.p2.distance(point);
		return d1 <= range_detect || d2 <= range_detect;
	}
}

void GameNoiDiem::startTracing(bool next_path, float dt)
{
	CCLOG(__FUNCTION__);
	disableControlQuiz();
	scheduleOnce([=](float) {

		if (next_path) {
			index_path++;
			GuideCenter->resetIncorrectCount();
			item_list.clear();
		}
		else {
			if (_drawer) {
				_drawer->clear();
			}
		}

		path_reload = true;
		path_done = false;
		is_completed = false;

		float delay = 0;
		float delta = 0.11f;
		if (item_list.size() > 0) {
			CCLOG("Reset items");
			if (hint_drawer && !hint_animation)
				for (auto item : item_list) {
					if (item && item->isColoring() && (item->getPosition().equals(current_line.p1) || item->getPosition().equals(current_line.p2))) {
						this->scheduleOnce([](float) {
							AUDIO_ENGINE->playEffect("sounds/vm_noi_diem/tl_dot_fadein.mp3");
						}, delay, cocos2d::StringUtils::format("play fadein effect %d", item->getId()));

						item->onShowGAF(delay);
						delay += delta;
						item->setColoring(false);
						item->setImageVisible(true);
						item->onShow(delay);
					}
				}
		}
		else {
			CCLOG("show items");
			int index = 0;
			GameNoiDiem_Item* last_item = nullptr;
			auto points = trace_data.paths[index_path].points;
			for (int i : points) {
				for (auto it = point_objects.begin(); it != point_objects.end(); ++it) {
					auto item = *(it);
					if (item != nullptr && item->getId() == i && item->isColoring()) {
						this->scheduleOnce([](float) {
							AUDIO_ENGINE->playEffect("sounds/vm_noi_diem/tl_dot_fadein.mp3");
						}, delay, cocos2d::StringUtils::format("play fadein effect %d %p", item->getId(), item));

						item->setColoring(false);
						item->setImageVisible(true);
						item_list.push_back(item);
						item->onShow(delay);
						item->onShowGAF(delay);
						//item->setBackgroundColor(cocos2d::Color3B::BLUE);
						delay += delta;
						++index;
						last_item = item;
						point_objects.erase(it);
						break;

					}
				}
			}
			if (item_list.size() >= 2) {
				current_line.setup(item_list[0]->getPosition(), item_list[1]->getPosition());
			}
			else if (item_list.size() == 1) {
				current_line.setup(item_list[0]->getPosition(), item_list[0]->getPosition());
			}
		}

		/*if (next_path && end_of_last_path == item_list.front()->getId()) {
			this->activeItem(item_list.front());
			end_of_last_path = item_list.back()->getId();
			return;
		}*/
		end_of_last_path = item_list.back()->getId();

		//Add balloon
		enableControlQuiz();

	}, dt, "starting path");
}

void GameNoiDiem::disableControlQuiz(float time)
{
	unschedule("disable_touch_quiz");
	_panel_disable->setVisible(true);
	//_panel_move->getEventDispatcher()->setEnabled(false);
	if (time > 0) {
		scheduleOnce([=](float dt) {
			//_panel_move->getEventDispatcher()->setEnabled(true);
			_panel_disable->setVisible(false);
		}, time, "disable_touch_quiz");
	}
}

void GameNoiDiem::enableControlQuiz()
{
	unschedule("disable_touch_quiz");
	//_panel_move->getEventDispatcher()->setEnabled(true);
	_panel_disable->setVisible(false);
}

GameNoiDiem_Item* GameNoiDiem::nextItem() {
	if (item_list.empty()) {
		return nullptr;
	}
    for (int i = item_list.size()-2; i >= 0; i--){
        if (item_list[i]->isColoring()){
            if(!item_list[i+1]->isColoring())
                return item_list[i+1];
            else
                return nullptr;
        }
        if (i == 0 && !item_list[i]->isColoring()){
            return item_list[i];
        }
    }
//	for (GameNoiDiem_Item* item : item_list) {
//		if (!item->isColoring()) {
//			return item;
//		}
//	}
	return nullptr;
}

void GameNoiDiem::activeItem(GameNoiDiem_Item* object) {
	if (object == NULL) {
		return;
	}
	if (object->isColoring()) {
		return;
	}

	// create new drawer
	if (hint_drawer)
	{
		_drawer = cocos2d::DrawNode::create();
		_panel->addChild(_drawer, 2);
		_drawers.push_back(_drawer);
	}



	AUDIO_ENGINE->playSound("sounds/vm_noi_diem/tl_dot_action.mp3");
	if (!object->correct_sequence.empty()) {
		AUDIO_ENGINE->play(img_tracing->getAudios("name_1", lang).front());
	}
	object->setColoring(true);
	object->onTouchItem(true, 0, 0);
	object->onCorrectGAF(0);

	GuideCenter->resetIncorrectCount();
	if (hint_animation) {
		//show flowers
		int flower = cocos2d::random(1, 7);
		auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgameui/tracing/tracing%d/tracing%d.gaf", m_theme->getThemePath().c_str(), flower, flower));
		auto anim = gaf_asset->createObjectAndRun(false);
		anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		anim->setPosition(object->getPosition());
		anim->setSequenceDelegate([=](gaf::GAFObject* object, const std::string& sequenceName) {
			if (sequenceName == "grow") {
				object->playSequence("idle", true);
			}
		});
		object->getParent()->addChild(anim, 4);
		gaf_list.push_back(anim);
	}

	auto next_item = nextItem();
	if (next_item) {
		//Next sub path
		current_line.setup(object->getPosition(), next_item->getPosition());
	}
	else {
		//end of path
		path_done = true;
		if (index_path == trace_data.paths.size() - 1) {
			is_completed = true;
		}
		goNextPath();
	}
}

void GameNoiDiem::speakQuestion()
{
	btn_speaker->setTouchEnabled(false);
	AUDIO_ENGINE->playChain(current_info.question_audios, [=] {
		btn_speaker->setTouchEnabled(true);
	});
}

void GameNoiDiem::goNextPath()
{

	disableControlQuiz();
	for (auto item : item_list) {
		if (item) item->onHide(0.0f);
	}
	item_list.clear();
	if (is_completed) {
		//stopSpeakQuestion();
		//setState(Math_Quiz::QuizState::PAUSING);
		AUDIO_ENGINE->playEffect("sounds/vm_noi_diem/tl_tracingdone.mp3");
		//AUDIO_ENGINE->play("sounds/vm_noi_diem/tl_good_job.mp3");
		CCLOG("next game");

		scheduleOnce([=](float) {
			this->onExplanation(true);
		}, 0.75f, "go_completed");
	}
	else {
		/*int begin_of_next_path = trace_data.paths[index_path + 1].points.front();
		if (begin_of_next_path == end_of_last_path) {*/
        startTracing(true,0.5f);
		/*}
		else {
			startTracing(true, 0.25f);
		}*/
	}
}


void GameNoiDiem::onShowItem(Node * item, float delay, float duration, cocos2d::CallFunc * call_func)
{
	item->setScale(0);
	item->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::EaseElasticOut::create(cocos2d::ScaleTo::create(duration, 1)), call_func, NULL));
}

cocos2d::Color3B GameNoiDiem::getColor3B(std::string color)
{
	auto m_color = color;
	if (m_color == "red")       return cocos2d::Color3B(255, 51, 0);
	if (m_color == "yellow")    return cocos2d::Color3B(255, 221, 59);
	if (m_color == "grey")      return cocos2d::Color3B(169, 169, 169);
	if (m_color == "blue")      return cocos2d::Color3B(86, 154, 239);
	if (m_color == "brown")     return cocos2d::Color3B(149, 75, 23);
	if (m_color == "green")     return cocos2d::Color3B(107, 214, 95);
	if (m_color == "orange")    return cocos2d::Color3B(255, 102, 0);
	if (m_color == "pink")      return cocos2d::Color3B(253, 106, 188);
	if (m_color == "purple")    return cocos2d::Color3B(149, 89, 237);
	if (m_color == "white")     return cocos2d::Color3B(255, 255, 255);
	return cocos2d::Color3B(0, 0, 0);
}

void GameNoiDiem::drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2)
{
	if (_drawer && hint_drawer) {
		_drawer->drawSegment(p1, p2, line_draw, color_line);
	}
}


void GameNoiDiem::addEvents()
{
	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameNoiDiem::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(GameNoiDiem::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(GameNoiDiem::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(GameNoiDiem::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, _panel_move);

	GuideCenter->startGuide(CC_CALLBACK_0(GameNoiDiem::onGuideStarted, this), CC_CALLBACK_0(GameNoiDiem::onGuideDone, this));
	GuideCenter->setIncorrectTime(3);
    GuideCenter->setInactiveTime(3);
}

GameNoiDiem_Item* GameNoiDiem::touchOnObject(cocos2d::Vec2 point) {
	for (GameNoiDiem_Item* object : item_list) {
		if (!object->isColoring() && object->isTouchedAt(point)) {
			return object;
		}
	}
	return nullptr;
}

bool GameNoiDiem::isTraceDone() {
	for (GameNoiDiem_Item* object : item_list) {
		if (!object->isColoring()) {
			return false;
		}
	}
	return true;
}

bool GameNoiDiem::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) {
	current_point = touch->getLocation();
	last_point = current_point;
	moveable = true;
	drawable = false;

	return moveable;
}

void GameNoiDiem::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
	if (moveable) {
		current_point = touch->getLocation();
		if (hint_drawer && !drawable) {
			auto d0 = current_line.distanceA(current_point);
			if (d0 <= range_active) {
				auto next_item = nextItem();
                if (next_item){
                    if (next_item->getPosition().equals(current_line.p1))
                    {
                        path_reload = false;
                        drawSegment(last_point, current_point);
                        activeItem(next_item);
                    }
                    drawable = true;
                }
			}
		}
		else {
			if (!isInPath(current_point)) {
				moveable = false;
				drawable = false;
				AUDIO_ENGINE->playSound("sounds/vm_noi_diem/tl_dot_wrong.mp3");
				if (!path_done) {
					startTracing(false);
					GuideCenter->increamentIncorrect();
				}
			}
			else {
				path_reload = false;
				drawSegment(last_point, current_point);
				last_point = current_point;
				auto d0 = current_line.distanceB(current_point);
				if (d0 <= range_active) {
					auto next_item = nextItem();
					activeItem(next_item);
				}
			}
		}
	}
	else {

	}
}

void GameNoiDiem::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {

}

void GameNoiDiem::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {

}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void GameNoiDiem::autoTesting() {

	this->scheduleOnce([=](float dt) {
		//this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}



