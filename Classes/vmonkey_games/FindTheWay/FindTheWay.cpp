//
//  FindTheWay.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "FindTheWay.h"
#include "QuizGuideCenter.h"

#define CSB_NAME "csb/game/FindTheWay/FindTheWay.csb"
#define bg_music "sounds/findTheWay/bg_music.mp3"


USING_NS_CC;
INITIALIZE_READER(FindTheWay);

std::once_flag FindTheWay_reader;

#define detect_size cocos2d::Size(30, 30)
#define detect_size_level 3

#define step_draw 1.f
#define line_draw 5.f
#define range_detect 40.0f
#define range_active 15.0f

FindTheWay* FindTheWay::createGame() {

	std::call_once(FindTheWay_reader, [] {
		REGISTER_CSB_READER(FindTheWay);
	});

	auto p = static_cast<FindTheWay*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {

		p->didLoadFromCSB();
		return p;
	}
	CC_SAFE_DELETE(p);

	return nullptr;
}

void FindTheWay::didLoadFromCSB() {

	this->generateData();

	screen_size = Director::getInstance()->getVisibleSize();
	//    autoPlay = isAutoPlay;
	this->loadFrame();
}

void FindTheWay::generateData() {

	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	//    // get data
	string json_file = "config.json";

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& value_data = document.FindMember("data")->value;

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif

	std::vector<std::string> colors;
	colors.push_back("red");
	colors.push_back("pink");
	colors.push_back("purple");
	colors.push_back("orange");
	//colors.push_back("blue");
	color_tracing = math::func::getRandomNode(colors);
	color_line = cocos2d::Color4F(getColor3B(color_tracing));

	tracing_size = cocos2d::Size(956, 471);//
	//TODO: GAME CONFIG
	for (int i = 0; i < value_data.Size(); ++i) {
		rapidjson::Value& json_game = value_data[i];
		GameData game_data;
		game_data.map_size = cocos2d::Size(json_game.FindMember("size")->value.FindMember("width")->value.GetInt(), json_game.FindMember("size")->value.FindMember("height")->value.GetInt());
		if (json_game.HasMember("map")) {
			std::string map_id = json_game["map"].GetString();
			game_data.map_image = math::resource::ResHelper::getImage(images, map_id);
		}
		if (json_game.HasMember("question")) {
			std::string question_id = json_game["question"].GetString();
			auto img_question = math::resource::ResHelper::getImage(images, question_id);
			mj::PHONIC_INFO info;
			info.text_full = info.text_phonic = img_question->getQuestionVMonkey("name_1", lang);
			info.audio_full = info.audio_phonic = img_question->getAudios("name_1", lang).front();
			game_data.question = info;
		}
		if (json_game.HasMember("character_start")) {
			std::string character_start_id = json_game["character_start"].GetString();
			game_data.character_start = math::resource::ResHelper::getImage(images, character_start_id);
		}
		if (json_game.HasMember("character_end")) {
			std::string character_end_id = json_game["character_end"].GetString();
			game_data.character_end = math::resource::ResHelper::getImage(images, character_end_id);
		}
		if (json_game.HasMember("ending_animation")) {
			std::string _id = json_game["ending_animation"].GetString();
			game_data.animation = math::resource::ResHelper::getImage(images, _id);
		}
        if (json_game.HasMember("pos_retry")) {
            std::string _id = json_game["pos_retry"].GetString();
            if (_id == "left") game_data.retry_pos_right = false;
        }
        if (json_game.HasMember("pos_character")) {
            std::string _id = json_game["pos_character"].GetString();
            game_data.pos_character_end = "right";
            if (_id == "left") game_data.pos_character_end = "left";
            if (_id == "center") game_data.pos_character_end = "center";
        }
		mGameDataSet.push_back(game_data);
	}

}


void FindTheWay::loadFrame() {

	//    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
	AUDIO_ENGINE->playBackgroundMusic(bg_music);
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	screen_size = Director::getInstance()->getWinSize();

	this->setContentSize(screen_size);

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
	_panel = Layout::create();
	_panel->setContentSize(screen_size);
	_panel->setLayoutType(Layout::Type::ABSOLUTE);
	_panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel->setPosition(screen_size / 2);
	_panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_panel->setBackGroundColor(cocos2d::Color3B::BLUE);
	_panel->setOpacity(0);
	_panel->setSwallowTouches(true);
	this->addChild(_panel, -1);

	/*---- Layout ngăn tương tác: ----*/
	_panel_disable = Layout::create();
	_panel_disable->setContentSize(screen_size);
	_panel_disable->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel_disable->setPosition(screen_size / 2);
	_panel_disable->setVisible(false);
	_panel_disable->setTouchEnabled(true);
	this->addChild(_panel_disable, INT_MAX - 1);

	auto speaker = utils::findChild<ui::Button*>(root_layout, "speaker");
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(FindTheWay::onClickSpeaker, this));

	auto bg_ques = utils::findChild<ui::ImageView*>(root_layout, "Image_4");
	bg_ques->setTouchEnabled(true);
	bg_ques->addClickEventListener(CC_CALLBACK_1(FindTheWay::onClickSpeaker, this));

	m_lbl_question = utils::findChild<ui::Text*>(root_layout, "Text_1");
	utils::findChild(this, "findWay_bg_1")->setVisible(false);
	m_lbl_question->setPositionX(m_lbl_question->getPositionX() - 50);

	retry_btn = Button::create("games/findTheWay/btn_img.png");
	this->addChild(retry_btn);
	retry_btn->setPosition(Vec2(screen_size.width*0.9, screen_size.height*0.1));
	retry_btn->setTouchEnabled(true);
	retry_btn->addClickEventListener([=](Ref* sender) {
		//reset game
		index_path = -1;
		for (auto d : _drawers) {
			d.second->clear();
			d.second->removeFromParentAndCleanup(true);
			d.second = nullptr;
		}

		_drawers.clear();
		_drawer = nullptr;
		for (auto m_o : map_objects) {
			m_o.second->setColoring(false);
			m_o.second->setImageVisible(false);
		}
		startTracing(true, 0.25f);
		GuideCenter->setGuideEnabled();
		current_node = trace_data.paths.front().points.front();
		last_point = start_pos;
		last_item_point = start_pos;
		character_start->setPosition(start_pos);
	});

	auto retry_txt = Text::create("Thử lại", font_name, 30);
	retry_txt->enableOutline(Color4B(138, 51, 47, 255), 3);
	retry_btn->addChild(retry_txt);
	retry_txt->setPosition(retry_btn->getContentSize() / 2);

}

void FindTheWay::onEnter() {
	MJMainGBase::onEnter();
	//    AUDIO_ENGINE->play("sounds/findTheWay/bgm.mp3", true);
	//    AUDIO_ENGINE->play("sounds/findTheWay/intro.mp3", true);
	this->setContentSize(screen_size);
	this->unscheduleUpdate();
	addEvents();
	onStart();
}
void FindTheWay::onFinishGameVMonkey()
{
	GuideCenter->stopGuide();
	this->scheduleOnce([=](float dt) {
		MJMainGBase::onFinishGameVMonkey();
	}, 0.5f, "finish game");

}


void FindTheWay::onExitTransitionDidStart() {
	GuideCenter->stopGuide();
	MJMainGBase::onExitTransitionDidStart();
}

void FindTheWay::onClickSpeaker(Ref* sender) {
	speakQuestion();
}

int FindTheWay::getGuidePath(int start_point)
{
	// next path
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		if (trace_data.paths[i].points.front() == start_point) {
			if (m_right_path.at(i) || trace_data.paths.back().points.back() == trace_data.paths[i].points.back()) {
				return i;
			}
		}
	}
	// back path
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		if (trace_data.paths[i].points.back() == start_point) {
			if (!m_right_path.at(i)) {
				return -i;
			}
		}
	}
	return INT_MIN;
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback FindTheWay::onLocateClickCallback(const std::string &callback_name) {
	std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(FindTheWay, onClickSpeaker)
	};

	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}
	return nullptr;
}


#pragma mark - Quiz Life-cycle

void FindTheWay::loadQuestion()
{
    auto bg_ques = utils::findChild<ImageView*>(this, "Image_4");
    bg_ques->loadTexture("games/findTheWay/Layer-33.png");
    bg_ques->setScale9Enabled(true);
    bg_ques->setCapInsets(Rect(262,40,143,89));
    bg_ques->setContentSize(Size(794,140));
    bg_ques->setPositionX(bg_ques->getPositionX()-50);
	mCurrentData = mGameDataSet.at(game_index);
	std::vector<math::resource::Image*> image_list;
	img_tracing = mCurrentData.map_image;
	tracing_size = mCurrentData.map_size;

	m_lbl_question->setString(mCurrentData.question.text_full);
    m_lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	m_lbl_question->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_lbl_question->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_lbl_question->setPosition(bg_ques->getContentSize()/2);
	_drawers.clear();
	map_objects.clear();

	int font_sz = m_lbl_question->getFontSize();
	// fit value text size
	while (m_lbl_question->getVirtualRendererSize().width > utils::findChild<ImageView*>(this, "Image_4")->getContentSize().width * 0.85) {
		font_sz -= 2;
		m_lbl_question->setFontSize(font_sz);
		CCLOG("Scale down font: %d", font_sz);
	}
    // chuyen question sang _panel
    auto pos_in_panel = _panel->convertToNodeSpace(root_layout->convertToWorldSpace(bg_ques->getPosition()));
    bg_ques->removeFromParentAndCleanup(false);
    _panel->addChild(bg_ques,7);
    bg_ques->setPosition(pos_in_panel);
	/*---- PARSE DATA -----*/
	trace_data = math::JSON::MMTrace::create(img_tracing->tracing);

	std::unordered_map<int, math::JSON::MMPoint> points;
	for (auto point : trace_data.points) {
		points.insert({ point.index,point });
	}
	/*---- GEN OBJECT -----*/
	tracing_center = screen_size / 2; // root_layout->convertToWorldSpace(utils::findChild(this, "Image_1")->getPosition());

	game_object = Math_Object::createObject();
	game_object->setEnable(0, math::common::deny, img_tracing, nullptr);
	game_object->setObjectSize(tracing_size);
	game_object->setPosition(tracing_center);
	game_object->setImageColor(cocos2d::Color3B::WHITE);
	_panel->addChild(game_object);
	game_object->setSuperNode(_panel_move);


	// gen character
	character_start = Math_Object::createObject();
	if (mCurrentData.character_start != nullptr) {
		character_start->setEnable(0, math::common::TouchPermission::touch, mCurrentData.character_start, nullptr);
	}
	else {
		auto img = math::resource::ResHelper::createImage("games/findTheWay/findWay_Group-1.png");
		character_start->setEnable(0, math::common::TouchPermission::touch, img, nullptr);
	}
	auto image_size = character_start->getImage()->getVirtualRendererSize();
	character_start->setAnchorPoint(Vec2(0.5f, 0.15f));
	//    character_start->setObjectSize(Size(100, 100));
	if (image_size.width - 10 > image_size.height) {
		character_start->fixWidthObject();
	}
	else if (image_size.height - 10 > image_size.width) {
		character_start->fixHeightObject();
	}
	_panel->addChild(character_start, 8);

	character_end = Math_Object::createObject();
	if (mCurrentData.character_end != nullptr) {
		character_end->setEnable(0, math::common::TouchPermission::touch, mCurrentData.character_end, nullptr);
	}
	else {
		auto img = math::resource::ResHelper::createImage("games/findTheWay/findWay_Vector-Smart-Object2.png");
		character_end->setEnable(0, math::common::TouchPermission::touch, img, nullptr);
	}
	image_size = character_end->getImage()->getVirtualRendererSize();
	//    character_end->setObjectSize(Size(50, 50));
	if (image_size.width - 10 > image_size.height) {
		character_end->fixWidthObject();
	}
	else if (image_size.height - 10 > image_size.width) {
		character_end->fixHeightObject();
	}
    if (mCurrentData.pos_character_end == "right") character_end->setAnchorPoint(Vec2::ZERO);
    if (mCurrentData.pos_character_end == "left") character_end->setAnchorPoint(Vec2(1,0.3));
    if (mCurrentData.pos_character_end == "center") character_end->setAnchorPoint(Vec2(0.7,0.5));
    
	_panel->addChild(character_end, 3);

	// get position
	auto start_index = trace_data.paths.front().points.front();
	auto start_point = points.at(start_index);
	start_pos = trace_data.getLocation(start_point, tracing_center, tracing_size);
	character_start->setPosition(start_pos);
	last_point = start_pos;
	last_item_point = start_pos;

	current_node = start_index;

	auto end_index = trace_data.paths.back().points.back();
	auto end_point = points.at(end_index);
	auto end_pos = trace_data.getLocation(end_point, tracing_center, tracing_size);
	character_end->setPosition(end_pos);
	//
	CCLOG("color: %s", color_tracing.c_str());
	for (int i = 0; i < trace_data.points.size(); ++i) {

		auto point = trace_data.points[i];
		auto object = Math_Object::createObject();
		object->setEnable(point.index, math::common::deny, math::resource::ResHelper::createImage("games/findTheWay/dot.png"), nullptr);
		object->setObjectSize(detect_size);
		object->setImageVisible(false);
		cocos2d::Vec2 location = trace_data.getLocation(point, tracing_center, tracing_size);
		object->setPosition(location);
		object->setColoring(false);
		object->setColor(getColor3B(color_tracing));
		_panel->addChild(object, 3);
		object->setSuperNode(_panel_move);
		map_objects.insert({ point.index,object });
	}
	// mark correct path
	findRightPath_DFS();
    
	// create graph
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		auto points = trace_data.paths[i].points;
		for (int j = 0; j < points.size() - 1; ++j) {
			auto e1 = edge(points[j], points[j + 1]);
			//            auto e2 = edge(points[j + 1], points[j]);
			auto p1 = trace_data.getLocation(points[j], tracing_center, tracing_size);
			auto p2 = trace_data.getLocation(points[j + 1], tracing_center, tracing_size);
			e1.path_index = /*e2.path_index =*/ i;
			e1.line.setup(p1, p2);
			//            e2.line.setup(p2, p1);
			e1.priority = m_right_path.at(i) ? 2 : 1;
			if (graph.find(points[j]) != graph.end()) {
				graph.at(points[j]).push_back(e1);
			}
			else {
				graph.insert({ points[j], {e1} });
			}

			//            if (graph.find(points[j + 1]) != graph.end()) {
			//                graph.at(points[j + 1]).push_back(e2);
			//            }
			//            else {
			//                graph.insert({ points[j + 1], {e2} });
			//            }

		}

		auto e = edge(points.back(), points.back());
		auto p1 = trace_data.getLocation(points.back(), tracing_center, tracing_size);
		e.path_index = i;
		e.line.setup(p1, p1);
		if (graph.find(points.back()) == graph.end()) {
			graph.insert({ points.back(), {e} });
			continue;
		}
	}

	for (map<int, std::vector<edge>>::iterator it = graph.begin(); it != graph.end(); it++) {
		std::sort(it->second.begin(), it->second.end(), [](edge e1, edge e2)  -> bool {
			return e1.priority > e2.priority;
		});
	}
    if (mCurrentData.retry_pos_right){
        retry_btn->setPosition(Vec2(screen_size.width*0.9, screen_size.height*0.1));
    }else{
        retry_btn->setPosition(Vec2(screen_size.width*0.1, screen_size.height*0.1));
    }
	// start question
	startQuestion();
}


void FindTheWay::finishQuestion()
{
	for (auto item : map_objects) {
		item.second->onHide(0.0f);
	}
	//if (game_object) {
	//	game_object->onHide(0.15f);
	//}
	if (character_start) {
		character_start->runAction(Sequence::create(DelayTime::create(0.15f), FadeOut::create(0.25f), RemoveSelf::create(), NULL));
	}
	if (character_end) {
		character_end->runAction(Sequence::create(DelayTime::create(0.15f), FadeOut::create(0.25f), RemoveSelf::create(), NULL));
	}
	if (img_character) {
		img_character->runAction(Sequence::create(DelayTime::create(0.15f), FadeOut::create(0.25f), RemoveSelf::create(), NULL));
	}

}


void FindTheWay::onStart() {

	//quizBegan(0, false);
	disableControlQuiz();
	loadQuestion();
}

void FindTheWay::startQuestion() {
	AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_appear.mp3");
	game_object->onShow(0, math::animation::No_Effect);

	speakQuestion();
	index_path = -1;
	startTracing(true, 0.25f);
	GuideCenter->setGuideEnabled();

}

void FindTheWay::nextQuestion(float delay_time)
{
	++game_index;
	this->scheduleOnce([=](float dt) {
		if (game_index >= mGameDataSet.size()) {
			onFinishGameVMonkey();
			return;
		}
		else {
			finishQuestion();
			loadQuestion();
		}
	}, delay_time, "delay finish");

}


void FindTheWay::onExplanation(bool correct) {
	disableControlQuiz();
	GuideCenter->setGuideDisabled();
	//    _drawer = nullptr;
	//    for (auto drawer : _drawers) {
	//        if (drawer.second) {
	//            drawer.second->clear();
	//            drawer.second->removeFromParentAndCleanup(true);
	//            drawer.second = nullptr;
	//        }
	//    }
	//    _drawers.clear();
	auto duration = 2.0f;

	this->scheduleOnce([=](float) {
		if (mCurrentData.animation == nullptr) {
			nextQuestion();
		}
		else {
			showImageEndQuiz();
		}

	}, duration, "show next");
	auto effect = GAFWrapper::createGAF("gaf/findTheWay/tl_sparkle/tl_sparkle.gaf");
	effect->setPosition(game_object->getPosition());
	effect->play(false, [effect](gaf::GAFObject * obj) {
		effect->removeFromParent();
	});
	_panel->addChild(effect);

	AUDIO_ENGINE->playSound("sounds/findTheWay/tl_correct.mp3");

	//game_object->setImageColor(getColor3B(color_tracing));


}

void FindTheWay::showImageEndQuiz()
{
	img_character = nullptr;

	auto img_item = mCurrentData.animation;
	AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_imageshow.mp3");
	float duration = 0.5f;
	auto image = ui::ImageView::create(img_item->path);
	image->setScale9Enabled(true);
	auto virtual_size = image->getVirtualRendererSize();
	image->setCapInsets(Rect(1, 1, virtual_size.width - 2, virtual_size.height - 2));
	auto image_size = image->getContentSize();
	auto scale_factor = 400.0f / image_size.height;
	image->setContentSize(cocos2d::Size(image_size.width * scale_factor, image_size.height * scale_factor));
	image->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	image->setPosition(Vec2(tracing_center.x, screen_size.height));
	image->setScale(2.0f);
	image->runAction(EaseQuarticActionIn::create(Spawn::create(MoveTo::create(duration, Vec2(tracing_center.x, tracing_center.y - 100)), ScaleTo::create(duration, 1.0f), NULL)));
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

void FindTheWay::onGuideStarted() {
	disableControlQuiz();
	std::vector<Math_Object*> not_actives;
	int start_point = trace_data.paths.front().points.front();
	if (index_path != -1) {
		start_point = trace_data.paths[index_path].points.front();
	}
	int path = getGuidePath(start_point);

	if (path == INT_MIN) {
		GuideCenter->guideNone();
		return;
	}
	if (path >= 0) {
		for (int point : trace_data.paths[path].points) {
			if (!map_objects.at(point)->isColoring()) {
				not_actives.push_back(map_objects.at(point));
			}
		}
	}
	else {
		path = -path;
		for (int point : trace_data.paths[path].points) {
			if (map_objects.at(point)->isColoring()) {
				not_actives.push_back(map_objects.at(point));
			}
		}
		not_actives.reserve(not_actives.size());
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
			for (Math_Object* object : not_actives) {
				vec2points.push_back(object->getWorldPosition());
			}
			GuideCenter->guideBySwipe(vec2points);
		}
	}
}

void FindTheWay::onGuideDone()
{
	enableControlQuiz();
}


#pragma mark - object touch events callback

bool FindTheWay::isInPath(cocos2d::Vec2 point) {
	for (auto e : graph.at(current_node))
	{
		MMLine line = e.line;
		if (line.isPointInnerEdge(point)) {
			auto d0 = line.distance(point);
			if (d0 <= range_detect) {
				current_edge = e;
				index_path = current_edge.path_index;
				return true;
			}
		}
		else {
			auto d1 = line.p1.distance(point);
			auto d2 = line.p2.distance(point);
			if (d1 <= range_detect || d2 <= range_detect)
			{
				current_edge = e;
				index_path = current_edge.path_index;
				return true;
			}
		}
	}
	return false;

}

void FindTheWay::startTracing(bool next_path, float dt)
{
	CCLOG(__FUNCTION__);
	path_reload = true;
	path_done = false;
	is_completed = false;

	float delay = 0;
	float delta = 0.11f;

	GuideCenter->setGuideDisabled();
	disableControlQuiz();
	scheduleOnce([=](float) {
		GuideCenter->setGuideEnabled();
		enableControlQuiz();
	}, dt + delay, "starting path");

	if (next_path) {
		item_list.clear();
		next_paths = getNextPathIndexes(index_path);
		GuideCenter->resetIncorrectCount();
	}
	else {
		/*if (_drawer)
			_drawer->clear();*/
	}

	if (item_list.size() > 0) {
		//reset lúc đi đến cuối đường sai
		CCLOG("Reset items");
		current_node = trace_data.paths[current_edge.path_index].points.front();
		for (auto item : item_list) {
			if (item != nullptr && item->isColoring()) {
				this->scheduleOnce([](float) {
					AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_dot_fadein.mp3");
				}, delay, cocos2d::StringUtils::format("play fadein effect %d", item->getID()));
				item->onShow(delay);
				delay += delta;
				item->setColoring(false);
				//item->setImageVisible(false);
			}
		}
	}
	else {
		CCLOG("show items");
		Math_Object* last_item = nullptr;
		std::vector<int> points;
		for (auto path : next_paths) {
			for (auto point : trace_data.paths[path].points)
				points.push_back(point);
		}

		for (int i : points) {
			auto item = map_objects.at(i);
			if (item != nullptr) {
				if (!item->getImage()->isVisible()) {
					this->scheduleOnce([](float) {
						AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_dot_fadein.mp3");
					}, delay, cocos2d::StringUtils::format("play fadein effect %d %p", item->getID(), item));

					item->setImageVisible(true);
					item->onShow(delay);
				}
				item_list.push_back(item);
			}
		}
		last_item = item_list.front();
	}

	if (next_path) {
		if (index_path == -1) {
			auto show_items = Sequence::create(
				CallFunc::create([=] {
				AUDIO_ENGINE->playSound("sounds/findTheWay/tl_char_fadein.mp3");
			}),
				EaseElasticOut::create(cocos2d::ScaleTo::create(0.25f, 1.0f)), NULL);
			character_start->runAction(Sequence::create(
				DelayTime::create(delay),
				show_items, NULL));
		}
		//        not_tracing_items = getTracingObject(false);
	}
	else {
		goStartPath();
	}

}

void FindTheWay::disableControlQuiz(float time)
{
	CCLOG(__FUNCTION__);
	unschedule("disable_touch_quiz");
	_panel_disable->setVisible(true);
	//_panel_move->getEventDispatcher()->setEnabled(false);
	if (time > 0) {
		scheduleOnce([=](float dt) {
			enableControlQuiz();
		}, time, "disable_touch_quiz");
	}
}

bool FindTheWay::canNextPath(int path_index)
{
	return getNextPathIndexes(path_index).size() > 0;
}

void FindTheWay::enableControlQuiz()
{
	CCLOG(__FUNCTION__);
	unschedule("disable_touch_quiz");
	//_panel_move->getEventDispatcher()->setEnabled(true);
	_panel_disable->setVisible(false);
}

void FindTheWay::activeItem(Math_Object* object) {
	CCLOG(__FUNCTION__);
	if (object == nullptr) {
		return;
	}
	if (_drawer) {
		//_drawer->clear();
		drawSegment(last_item_point, object->getPosition());
	}
	if (_drawers.find(object->getID()) != _drawers.end()) {
		_drawer = _drawers.at(object->getID());
	}
	else {
		_drawer = cocos2d::DrawNode::create();
		_panel->addChild(_drawer, 2);
		_drawers.insert({ object->getID(),_drawer });
	}

	
	int start_point = trace_data.paths[current_edge.path_index].points.front();
	// nếu điểm hiện tại không phải là điểm bắt đầu -> hide point các path khác
	if(object->getID() != start_point)
		showPoint();
	//hidePoint();
	

	last_item_point = object->getPosition();

	// đã được đi qua rồi
	if (object->isColoring()) {
		// move back
		int start_point = trace_data.paths[current_edge.path_index].points.front();
		// nếu không phải cung tròn
		if (current_edge.end != start_point) {
			auto last_object = map_objects.at(current_node);
			//last_object->setImageVisible(true);
			//last_object->setColor(getColor3B(color_tracing));
			last_object->setColoring(false);
		}

		current_node = object->getID();
	}
	else {
		AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_dot_action.mp3");
		// move next
//        object->setColor(Color3B::WHITE);
		object->setColoring(true);
		object->setImageVisible(false);
		//object->onShow(0.0f);
		current_node = object->getID();
		if (current_node != trace_data.paths[current_edge.path_index].points.back()) {
			// 
		}
		else {
			//end of path
			path_done = true;

			int end_point = trace_data.paths.back().points.back();
			if (end_point == current_edge.end) {
				is_completed = true;
			}
			int start_point = trace_data.paths[current_edge.path_index].points.front();
			if (!is_completed && !canNextPath(current_edge.path_index)) {
				// back to start path
//                goStartPath();
			}
			else if (current_edge.end != start_point) {
				goNextPath();
			}

		}
	}


}

void FindTheWay::speakQuestion()
{
    AUDIO_ENGINE->stopSound(mCurrentData.question.audio_full);
    q_id = AUDIO_ENGINE->playSound(mCurrentData.question.audio_full);
}

void FindTheWay::goNextPath()
{
	disableControlQuiz();
	/*for (auto item : item_list) {
		if (item) item->setImageVisible(false);
	}*/
	item_list.clear();
	//    not_tracing_items.clear();
	if (is_completed) {
		moveable = false;
		//stopSpeakQuestion();
		AUDIO_ENGINE->stopSound(mCurrentData.question.audio_full);
		//setState(Math_Quiz::QuizState::PAUSING);
		AUDIO_ENGINE->playEffect("sounds/findTheWay/tl_tracingdone.mp3");
		//AUDIO_ENGINE->play("sounds/findTheWay/tl_good_job.mp3");
		CCLOG("next game");
		character_start->stopAllActions();
		character_start->runAction(Sequence::create(
			RotateTo::create(0.1, 5), RotateTo::create(0.2, -5), RotateTo::create(0.1, 0),
			RotateTo::create(0.1, 5), RotateTo::create(0.2, -5), RotateTo::create(0.1, 0), NULL));
		scheduleOnce([=](float) {
			this->onExplanation(true);
		}, 0.5f, "go_completed");
	}
	else {
		startTracing(true);
	}
}

void FindTheWay::goStartPath()
{
	CCLOG(__FUNCTION__);
	moveable = false;
	drawable = false;
	path_done = false;
	float delay = 0.25f;
	disableControlQuiz(delay + 0.1f);
	auto points = trace_data.paths[current_edge.path_index].points;
	cocos2d::Vector<FiniteTimeAction*> fta;

	//    // nếu đang đi đúng đường -> trở về điểm trước đó
	//    if (m_right_path.at(current_edge.path_index)) {
	//
	//        auto object = map_objects.at(current_node);
	//        if (object->isColoring()) {
	//            object->setImageVisible(false);
	//            //object->setColor(getColor3B(color_tracing));
	//            object->setColoring(false);
	//        }
	//        fta.pushBack(MoveTo::create(delay, map_objects.at(current_node)->getPosition()));
	//        //current_node = current_edge.start;
	//    }
	//    // đi sai đường -> trở về điểm đầu tiên
	//    else {
	//
	for (int i = (int)points.size() - 1; i >= 0; --i) {
		auto object = map_objects.at(points[i]);
		if (object->isColoring()) {
			object->setImageVisible(true);
			object->setColor(getColor3B(color_tracing));
			object->setColoring(false);
			if (_drawers.find(object->getID()) != _drawers.end()) {
				if (_drawers.at(object->getID()))
					_drawers.at(object->getID())->clear();
			}
		}
	}

	current_node = points.front();
	//        fta.pushBack(MoveTo::create(delay, map_objects.at(points.front())->getPosition()));
	fta.pushBack(MoveTo::create(delay, map_objects.at(points.front())->getPosition()));
	//    }


	/*if (_drawer) {
		_drawer->clear();
	}*/

	character_start->stopAllActions();
	character_start->runAction(cocos2d::Sequence::create(fta));
}

std::vector<int> FindTheWay::getNextPathIndexes(int path_index)
{
	std::vector<int> path_indexes;
	int last_point = -1;
	if (path_index == -1) {
		last_point = trace_data.paths.front().points.front();
	}
	else {
		if (path_index >= 0 && path_index < trace_data.paths.size())
			last_point = trace_data.paths[path_index].points.back();
		else return path_indexes;
	}

	for (int i = path_index + 1; i < trace_data.paths.size(); ++i) {
		if (trace_data.paths[i].points.front() == last_point) {
			path_indexes.push_back(i);
		}
	}


	return path_indexes;
}

//std::vector<int> FindTheWay::getBackPathIndexes(int path_index)
//{
//    std::vector<int> path_indexes;
//    int first_point = -1;
//    if (path_index == -1) {
//
//    }
//    else {
//        if (path_index >= 0 && path_index < trace_data.paths.size())
//            first_point = trace_data.paths[path_index].points.front();
//        else return path_indexes;
//    }
//
//    for (int i = path_index + 1; i < trace_data.paths.size(); ++i) {
//        if (trace_data.paths[i].points.back() == first_point) {
//            path_indexes.push_back(i);
//        }
//    }
//    return path_indexes;
//}


void FindTheWay::onShowItem(Node * item, float delay, float duration, cocos2d::CallFunc * call_func)
{
	item->setScale(0);
	item->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::EaseElasticOut::create(cocos2d::ScaleTo::create(duration, 1)), call_func, NULL));
}

cocos2d::Color3B FindTheWay::getColor3B(std::string color)
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

void FindTheWay::drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2)
{
	character_start->runAction(MoveTo::create(0.1f, p2));
	if (_drawer) {
		_drawer->drawSegment(p1, p2, line_draw, color_line);
	}
}

void FindTheWay::findRightPath_DFS()
{
	m_right_path.clear();
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		m_right_path.insert({ i, false });
	}

	bool correct = false;
	bool *visited = new bool[trace_data.paths.size() + 1]();
	int end_path = trace_data.paths.size() - 1;
	std::set<int> path_list;
	for (auto start_path : getNextPathIndexes(-1)) {
		findRightPath_DFS(start_path, end_path, visited, path_list);
	}

}

void FindTheWay::findRightPath_DFS(int start_path, int end_path, bool * visited, std::set<int> path_list)
{
	visited[start_path] = true;
	path_list.insert(start_path);
	if (start_path == end_path) {
		for (int path : path_list) {
			m_right_path.at(path) = true;
		}
	}
	for (int w : getNextPathIndexes(start_path)) {
		if (!visited[w]) {
			findRightPath_DFS(w, end_path, visited, path_list);
		}
	}
	path_list.erase(start_path);
	visited[start_path] = false;
}


std::vector<FindTheWay::edge> FindTheWay::findNextEdges(int point, int path_index)
{
	std::vector<FindTheWay::edge> list_edge;
	for (auto e : graph.at(point)) {
		if (e.path_index == path_index) {
			list_edge.push_back(e);
		}
	}
	return list_edge;
}

void FindTheWay::addEvents()
{
	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(FindTheWay::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(FindTheWay::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(FindTheWay::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(FindTheWay::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, _panel_move);

	GuideCenter->startGuide(CC_CALLBACK_0(FindTheWay::onGuideStarted, this), CC_CALLBACK_0(FindTheWay::onGuideDone, this));
	GuideCenter->setIncorrectTime(3);
}

//Math_Object* FindTheWay::touchOnObject(cocos2d::Vec2 point) {
//    for (Math_Object* object : item_list) {
//        if (!object->isColoring() && object->isTouchedAt(point)) {
//            return object;
//        }
//    }
//    return nullptr;
//}

bool FindTheWay::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) {
	GuideCenter->setGuideDisabled();
	current_point = touch->getLocation();
	moveable = true;
	drawable = false;
	if (!isInPath(touch->getLocation())) {
		GuideCenter->increamentIncorrect();
		return false;
	}
	//last_point = character_start->getPosition();

	Math_Object * object = nullptr;
	if (map_objects.find(current_node) != map_objects.end()) {
		object = map_objects.at(current_node);
		if (object && object->getPosition().distance(current_point) <= range_detect) {
			drawable = true;
			if (!object->isColoring()) {
				activeItem(object);
			}

		}
	}

	return moveable;
}

std::vector<Math_Object*> FindTheWay::getTracingObject(bool is_tracing) {
	std::vector<Math_Object*> tracing_obj;

	auto tracing_point_list = trace_data.paths[current_edge.path_index].points;
	CCLOG("current path index : %d", current_edge.path_index);

	std::vector<Math_Object*> not_tracing_obj = {};
	not_tracing_obj.push_back(item_list[0]);

	for (auto point : tracing_point_list) {
		for (int i = 0; i < item_list.size(); i++) {
			auto item = item_list[i];
			if (item->getID() == point) {
				tracing_obj.push_back(item);
			}
		}
	}

	for (auto item : item_list) {
		if (std::find(tracing_obj.begin(), tracing_obj.end(), item) != tracing_obj.end()) {
			/* v contains x */
		}
		else {
			/* v does not contain x */
			not_tracing_obj.push_back(item);
		}

	}
	if (!is_tracing) {
		return not_tracing_obj;
	}

	return tracing_obj;
}

void FindTheWay::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
	if (moveable) {
		current_point = touch->getLocation();


		if (!isInPath(current_point)) {
			//nếu điểm chạm ko ở đúng đường
			moveable = false;
			drawable = false;
			/*if (_drawer) {
				_drawer->clear();
			}*/
			AUDIO_ENGINE->playSound("sounds/findTheWay/tl_dot_wrong.mp3");
			if (!path_done) {
				//                startTracing(false);
								//goStartPath();
				GuideCenter->increamentIncorrect();
			}
		}
		else {
			//nếu điểm chạm ở đúng đường
			//ẩn chấm:
//            hidePoint();

			//Vẽ màu:
			//drawSegment(last_point, current_point);
			/*if (_drawer) {
				_drawer->drawSegment(last_point, current_point, line_draw, color_line);
			}
			last_point = current_point;*/

			auto d0 = current_edge.line.distanceB(current_point);
			if (d0 <= range_active) {
				auto next_item = map_objects.at(current_edge.end);
				activeItem(next_item);
			}
		}

	}
	else {

	}
}

void FindTheWay::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {

	GuideCenter->setGuideEnabled();
}

void FindTheWay::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
	GuideCenter->setGuideEnabled();
}

void FindTheWay::showPoint() {
	auto tracing_point_list = trace_data.paths[current_edge.path_index].points;
	for (auto item : item_list) {
		item->setImageVisible(false);
	}
	for (auto point : tracing_point_list) {
		auto item = map_objects.at(point);
		if (!item->isColoring()) {
			item->setImageVisible(true);
		}
	}
}
void FindTheWay::hidePoint() {
	not_tracing_items = getTracingObject(false);
	for (auto obj : not_tracing_items) {
		obj->setImageVisible(false);
		obj->setColoring(true);
	}
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void FindTheWay::autoTesting() {

	this->scheduleOnce([=](float dt) {
		//this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}



