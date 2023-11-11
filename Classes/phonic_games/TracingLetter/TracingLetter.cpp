//
//  TracingLetter.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "TracingLetter.h"
#define detect_size cocos2d::Size(100, 100)
#define detect_size_level 3

#define step_draw 1.f
#define line_draw 30.f
#define range_detect 60.f
#define range_active 30.f

TracingLetter * TracingLetter::createGame(std::string configPath)
{
	TracingLetter* quiz = new TracingLetter();
	if (quiz->init()) {
        quiz->autorelease();
		quiz->didLoadFromCSB(configPath);
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

void TracingLetter::didLoadFromCSB(std::string configPath)
{
	m_configPath = configPath;
    loadFrames();
}

bool TracingLetter::init() {
	return MJMainGBase::init();
}


#pragma mark - Quiz Life-cycle

void TracingLetter::generateData()
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(m_configPath);
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	//std::string choose_icon = "name";
	//if (json_data.HasMember("icon") && json_data["icon"].IsString()) {
	//	choose_icon = json_data["icon"].GetString();
	//}
	//
	//std::string choose_icon_name = "";
	//if (json_data.HasMember("icon_choose") && json_data["icon_choose"].IsString()) {
	//	choose_icon = json_data["icon_choose"].GetString();
	//}

	//int pair_value = -1;
	//if (choose_icon == "name") {
	//	for (math::resource::Image* img : m_images) {
	//		if (!img->used && img->getText("name_1") == choose_icon_name) {
	//			pair_value = img->getComparisionValue("pair");
	//			break;
	//		}
	//	}
	//}
	//if (pair_value == -1) {
	//	pair_value = math::func::getRandomNode(m_images)->getComparisionValue("pair");
	//}

	tracing_size = Size(650, 650);//cocos2d::Size(json_data.FindMember("size")->value.FindMember("width")->value.GetInt(), json_data.FindMember("size")->value.FindMember("height")->value.GetInt());

	std::vector<std::string> colors;
	colors.push_back("red");
	colors.push_back("pink");
	colors.push_back("purple");
	colors.push_back("orange");
	colors.push_back("blue");
	color_tracing = math::func::getRandomNode(colors);
	color_line = cocos2d::Color4F(getColor3B(color_tracing));

	//TODO: GAME CONFIG
	std::vector<math::resource::Image*> image_list;
	img_item = nullptr;
	img_tracing = nullptr;
	//auto all_images = getImageList("icons");
	std::sort(m_images.begin(), m_images.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool {
		return a->getComparisionValue("pair") < b->getComparisionValue("pair");
	});
	for (math::resource::Image* img : m_images) {
		if (img->getComparisionValue("pair") == pair_value) {
			int role = img->getComparisionValue("role");
			if (role == -1) {
				//img_item = img;
			}
			else if (role == 0) {
				img_tracing = img;
			}
			else {
				img_item = img;
				//image_list.push_back(img);
			}
		}
	}

	if (img_tracing == nullptr) {
		onFinishGame();
		return;
	}


	/*---- PARSE DATA -----*/
	trace_data = math::JSON::MMTrace::create(img_tracing->tracing);

	/*---- GEN OBJECT -----*/
	tracing_center = screen_size / 2;

	game_object = TracingLetter_Item::createItem(0, math::common::deny, img_tracing);
	game_object->setItemSize(tracing_size);
	game_object->setPosition(tracing_center);
	game_object->setImageColor(cocos2d::Color3B::WHITE);
	_panel->addChild(game_object);
	game_object->setSuperNode(_panel_move);

	_drawers.clear();
	point_objects.clear();

	std::unordered_map<int, math::JSON::MMPoint> points;
	for (auto point : trace_data.points) {
		points.insert({ point.index,point });
	}
	for (int i = 0; i < trace_data.paths.size(); ++i) {
		for (auto index : trace_data.paths[i].points) {
			auto point = points.at(index);
			auto object = TracingLetter_Item::createItem(point.index, math::common::deny, nullptr, "");
			object->setItemSize(detect_size);
			object->setImageVisible(false);
			cocos2d::Vec2 location = trace_data.getLocation(point, tracing_center, tracing_size);
			object->setPosition(location);
			_panel->addChild(object, 3);
			object->setSuperNode(_panel_move);
			point_objects.push_back(object);
		}
	}

	startQuestion();
}

void TracingLetter::finishQuestion()
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


void TracingLetter::onEnter()
{
    MJMainGBase::onEnter();
	playBackgroundMusic("sonds/tracingletter/tl_bgm.mp3");
	addEvents();
	pair_value = -1;
	onStart();
}

void TracingLetter::loadFrames()
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
	_panel = Layout::create();
	_panel->setContentSize(screen_size);
	_panel->setLayoutType(Layout::Type::ABSOLUTE);
	_panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel->setPosition(screen_size / 2);
	_panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_panel->setBackGroundColor(cocos2d::Color3B::BLUE);
	_panel->setOpacity(0);
	_panel->setSwallowTouches(true);
	this->addChild(_panel, 2);

	/*---- Layout ngăn tương tác: ----*/
	_panel_disable = Layout::create();
	_panel_disable->setContentSize(screen_size);
	_panel_disable->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_panel_disable->setPosition(screen_size / 2);
	_panel_disable->setVisible(false);
	this->addChild(_panel_disable, INT_MAX - 1);


	// gui

	auto background = cocos2d::ui::ImageView::create("games/tracingletter/tl_bg.png");
	background->setPosition(screen_size / 2);
	_panel->addChild(background);

	gaf_balloon = GAFWrapper::createGAF("gaf/tracingletter/tl_balloon/tl_balloon.gaf");
	gaf_balloon->setVisible(false);
	gaf_balloon->setCascadeOpacityEnabled(true);
	gaf_balloon->setScale(0.5f);
	_panel->addChild(gaf_balloon, 5);
}

void TracingLetter::onStart() {

	//quizBegan(0, false);
	disableControlQuiz();
	++pair_value;
	generateData();
}

void TracingLetter::startQuestion() {
	AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_appear.mp3");
	game_object->onShow(0, math::animation::No_Effect);
	game_object->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	game_object->setPosition(Vec2(-200, tracing_center.y - tracing_size.height / 2));
	//game_object->setPosition(Vec2(-200, tracing_center.y));

	float duration = 42.0f / 32;//1.0f;
	auto playJumpSound = CallFunc::create([] {
		AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_jumpmove.mp3");
	});
	auto jumpAction1 = JumpTo::create(duration * 0.2, Vec2(tracing_center.x / 2, tracing_center.y - tracing_size.height / 2), 50, 2);
	auto jumpAction2 = JumpTo::create(duration * 0.2, Vec2(tracing_center.x, tracing_center.y - tracing_size.height / 2), 50, 2);

	auto skew1 = SkewTo::create(duration * 0.2, 10, 0);
	auto skew2 = SkewTo::create(duration * 0.2, -10, 0);
	auto scale1 = ScaleTo::create(duration * 0.2, 0.92f, 1.18f);
	auto scale2 = ScaleTo::create(duration * 0.2, 1.12f, 0.92f);
	auto toNormal = Spawn::create(SkewTo::create(duration * 0.1, 0.0f, 0.0f), ScaleTo::create(duration * 0.1, 1.0f), NULL);
	auto introTracing = CallFunc::create([=] {
		speakQuestion();
		index_path = -1;
		startTracing(true, 0.25f);
	});

	auto skewAndScale = Sequence::create(Spawn::createWithTwoActions(skew1, scale1), Spawn::createWithTwoActions(skew2, scale2), NULL);
	auto jump = Sequence::create(
		DelayTime::create(duration * 0.1), jumpAction1, playJumpSound,
		DelayTime::create(duration * 0.2), jumpAction2, playJumpSound,
		DelayTime::create(duration * 0.1), toNormal,
		DelayTime::create(0.25f), introTracing, NULL);
	//auto jump = Sequence::create( jumpAction1, playJumpSound, jumpAction2, playJumpSound, DelayTime::create(duration * 0.1), introTracing, NULL);
	game_object->runAction(Spawn::create(
		jump,
		Sequence::create(skewAndScale, skewAndScale, NULL),
		NULL));


}

void TracingLetter::nextQuestion(float delay_time)
{
	finishQuestion();
	this->scheduleOnce([=](float dt) {
		onStart();
	}, delay_time, "next question");
}

void TracingLetter::onExitTransitionDidStart()
{
	GuideCenter->stopGuide();
	MJMainGBase::onExitTransitionDidStart();
}

void TracingLetter::onExplanation(bool correct) {
	disableControlQuiz();
	_drawer = nullptr;
	for (auto drawer : _drawers) {
		drawer->clear();
		drawer->removeFromParentAndCleanup(true);
		drawer = nullptr;
	}
	_drawers.clear();
	auto duration = 1.0f;
	auto jump = JumpBy::create(duration, Vec2(0, 0), 50, 2);
	auto scale1 = ScaleTo::create(duration / 4, 0.88f, 1.12f);
	auto scale2 = ScaleTo::create(duration / 4, 1.12f, 0.88f);
	auto scaleNormal = ScaleTo::create(duration * 0.1, 1.0f);
	auto playAudioJump = CallFunc::create([] {
		AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_jumpidle.mp3");
	});
	auto showEndQuiz = CallFunc::create([=] {
		if (img_item == nullptr) {
			nextQuestion();
		}
		else {
			showImageEndQuiz();
		}

	});
	auto effect = GAFWrapper::createGAF("gaf/tracingletter/tl_sparkle/tl_sparkle.gaf");
	effect->setPosition(game_object->getPosition() + Vec2(0, tracing_size.height / 2));
	effect->play(false, [effect](gaf::GAFObject * obj) {
		effect->removeFromParent();
	});
	_panel->addChild(effect);

	AUDIO_ENGINE->playSound("sounds/tracingletter/tl_correct.mp3");
	AUDIO_ENGINE->playSound(img_tracing->getAudios("name_1").front());
	game_object->setImageColor(getColor3B(color_tracing));
	game_object->runAction(Spawn::create(
		jump,
		Sequence::create(
			scale1, scale2, playAudioJump,
			scale1, scale2, playAudioJump,
			scaleNormal, showEndQuiz, NULL), NULL));

}

void TracingLetter::showImageEndQuiz()
{
	img_character = nullptr;

	AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_imageshow.mp3");
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

	auto img_text = img_item->getText("name_1");
	auto tracing_text = img_tracing->getText("name_1");

	// to lower case
	std::transform(img_text.begin(), img_text.end(), img_text.begin(), ::tolower);
	std::transform(tracing_text.begin(), tracing_text.end(), tracing_text.begin(), ::tolower);

	// get text before and after tracing text, ignore tracing text
	auto pos = img_text.find(tracing_text);
	if (pos == std::string::npos) pos = 0;
	auto pos1 = pos + (size_t)tracing_text.size();
	auto text_front = img_text.substr(0, pos);
	auto text_back = img_text.substr(pos1);
	CCLOG("Text end quiz : %s  %s %s", text_front.c_str(), tracing_text.c_str(), text_back.c_str());

	float img_minX = screen_size.width;
	float img_minY = screen_size.height;
	float img_maxX = 0;


	for (auto point : trace_data.points) {
		cocos2d::Vec2 location = trace_data.getLocation(point, tracing_center, tracing_size);
		if (location.x > img_maxX) img_maxX = location.x;
		if (location.x < img_minX) img_minX = location.x;
		if (location.y < img_minY) img_minY = location.y;
	}

	float minX = screen_size.width;
	float minY = screen_size.height;
	float maxX = 0;

	auto text_center = Vec2(tracing_center.x, tracing_center.y - tracing_size.height * 0.5f);
	tracing_text = img_tracing->getText("name_1");
	lbl_tracing = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%s%s%s", text_front.c_str(), tracing_text.c_str(), text_back.c_str()), font_name, font_size);
	lbl_tracing->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbl_tracing->setTextVerticalAlignment(TextVAlignment::BOTTOM);
	lbl_tracing->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	lbl_tracing->setPosition(text_center);
	_panel->addChild(lbl_tracing);

	for (int i = text_front.size(); i < text_front.size() + tracing_text.size(); ++i) {
		auto letter = lbl_tracing->getLetter(i);
		if (letter) {
			letter->setColor(Color3B(color_line));
			auto loc = _panel->convertToNodeSpace(lbl_tracing->convertToWorldSpace(letter->getPosition()));
			if (loc.x > maxX) maxX = loc.x;
			if (loc.x < minX) minX = loc.x;
		}
	}

	minY = text_center.y;

	lbl_tracing->setVisible(false);
	Vec2 fix_pos = Vec2((minX + maxX) / 2 - text_center.x, 0);
	game_object->runAction(
		Sequence::create(
			EaseQuarticActionIn::create(Spawn::create(MoveBy::create(duration, fix_pos), ScaleTo::create(duration, 0.3f), NULL)),
			CallFunc::create([this] {
		lbl_tracing->setVisible(true);
	}),
			Hide::create(), NULL));

	if (text_front.size() > 0) {
		auto text_f = cocos2d::ui::Text::create(text_front, font_name, font_size);
		text_f->setTextHorizontalAlignment(TextHAlignment::RIGHT);
		text_f->setTextVerticalAlignment(TextVAlignment::BOTTOM);
		text_f->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		text_f->setScale(2.0f);
		_panel->addChild(text_f);
		// run action
		auto front_x = minX - 25;
		auto start_pos = Vec2(img_minX - 50, img_minY - 50);
		auto end_pos = Vec2(front_x, minY);
		text_f->setPosition(start_pos);
		text_f->runAction(
			Sequence::create(
				EaseQuarticActionIn::create(Spawn::create(MoveTo::create(duration, end_pos), ScaleTo::create(duration, 1.0f), NULL)),
				RemoveSelf::create(), NULL));
	}
	if (text_back.size() > 0) {
		auto text_b = cocos2d::ui::Text::create(text_back, font_name, font_size);
		text_b->setTextHorizontalAlignment(TextHAlignment::LEFT);
		text_b->setTextVerticalAlignment(TextVAlignment::BOTTOM);
		text_b->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		_panel->addChild(text_b);
		text_b->setScale(2.0f);

		// runaction
		auto back_x = maxX + 25;
		auto start_pos = Vec2(img_maxX + 50, img_minY - 50);
		auto end_pos = Vec2(back_x, minY);
		text_b->setPosition(start_pos);
		text_b->runAction(
			Sequence::create(
				EaseQuarticActionIn::create(Spawn::create(MoveTo::create(duration, end_pos), ScaleTo::create(duration, 1.0f), NULL)),
				RemoveSelf::create(), NULL));
	}

	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(img_item->getAudios("name_1").front(), false, [=] {
			this->scheduleOnce([=](float) {
				nextQuestion();
			}, 1.0f, "next question");
		});
	}, duration, "play image audio");

}

void TracingLetter::onFinishGame()
{
	this->scheduleOnce([=](float dt) {
        MJMainGBase::onFinishGame(/*CHARACTER::DAN*/);
	}, 0.5f, "finish game");

}


void TracingLetter::onGuideStarted() {
	disableControlQuiz();
	std::vector<TracingLetter_Item*> not_actives;
	for (TracingLetter_Item* object : item_list) {
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
			for (TracingLetter_Item* object : not_actives) {
				vec2points.push_back(object->getWorldPosition());
			}
			GuideCenter->guideBySwipe(vec2points);
		}
	}
}

void TracingLetter::onGuideDone()
{
	enableControlQuiz();
}

#pragma mark - object touch events callback

bool TracingLetter::isInPath(cocos2d::Vec2 point) {
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

void TracingLetter::startTracing(bool next_path, float dt)
{
	CCLOG(__FUNCTION__);
	disableControlQuiz();
	scheduleOnce([=](float) {
		if (next_path) {
			index_path++;
			_drawer = cocos2d::DrawNode::create();
			_panel->addChild(_drawer, 2);
			_drawers.push_back(_drawer);
			GuideCenter->resetIncorrectCount();
		}
		else {
			_drawer->clear();
		}

		path_reload = true;
		path_done = false;
		is_completed = false;

		if (index_path == trace_data.paths.size() - 1) {
			/*chest->setPosition(cocos2d::Vec2(0, INT_MAX));
			chest_eff->setPosition(cocos2d::Vec2(0, INT_MAX));*/
		}
		float delay = 0;
		float delta = 0.11f;
		if (item_list.size() > 0) {
			CCLOG("Reset items");
			for (auto item : item_list) {
				if (item != nullptr && item->isColoring()) {
					this->scheduleOnce([](float) {
						AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_dot_fadein.mp3");
					}, delay, cocos2d::StringUtils::format("play fadein effect %d", item->getId()));

					item->onShowGAF(delay);
					delay += delta;
					item->setColoring(false);
				}
			}
		}
		else {
			CCLOG("show items");
			int index = 0;
			TracingLetter_Item* last_item = nullptr;
			auto points = trace_data.paths[index_path].points;
			for (int i : points) {
				for (auto it = point_objects.begin(); it != point_objects.end(); ++it) {
					auto item = *(it);
					if (item != nullptr && item->getId() == i) {
						this->scheduleOnce([](float) {
							AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_dot_fadein.mp3");
						}, delay, cocos2d::StringUtils::format("play fadein effect %d %p", item->getId(),item));

						bool is_arrow = false;// (index & 1);
						auto gaf_file = is_arrow ? "tl_arrow" : "tl_cloud";
						item->setGAF(cocos2d::StringUtils::format("gaf/tracingletter/%s/%s.gaf", gaf_file, gaf_file));
						if (is_arrow) {
							item->getGAF()->setScale(0.75f);
							auto arrow = item->getPosition() - last_item->getPosition();
							item->getGAF()->setRotation(-CC_RADIANS_TO_DEGREES(arrow.getAngle()));
						}
						else {
							item->getGAF()->setScale(0.6f);
						}

						item->setColoring(false);
						item->setImageVisible(true);
						item_list.push_back(item);
						//item->onShow(delay);
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
		}
		if (item_list.size() > 2) {
			current_line.setup(item_list[0]->getPosition(), item_list[1]->getPosition());
		}
		else if (item_list.size() == 1) {
			current_line.setup(item_list[0]->getPosition(), item_list[0]->getPosition());
		}
		if (next_path && end_of_last_path == item_list.front()->getId()) {
			this->activeItem(item_list.front());
			end_of_last_path = item_list.back()->getId();
			return;
		}
		end_of_last_path = item_list.back()->getId();

		//Add balloon
		auto item_point = item_list.front()->getPosition();

		auto balloon_show = Sequence::create(
			CallFunc::create([=] {
			AUDIO_ENGINE->playSound("sounds/tracingletter/tl_char_fadein.mp3");
			enableControlQuiz();
		}),
			EaseElasticOut::create(cocos2d::ScaleTo::create(0.25f, 0.5f)), NULL);

		if (next_path) {
			gaf_balloon->setVisible(true);
			gaf_balloon->setOpacity(255);
			gaf_balloon->play(true);
			gaf_balloon->setPosition(item_point);
			gaf_balloon->setScale(0);
			gaf_balloon->stopAllActions();
			gaf_balloon->runAction(Sequence::create(
				DelayTime::create(delay),
				balloon_show, NULL));
		}
		else {
			shake_balloon = false;
			gaf_balloon->stopAllActions();
			gaf_balloon->setOpacity(255);
			gaf_balloon->setScale(0.5f);
			gaf_balloon->setRotation(0);
			gaf_balloon->runAction(
				Sequence::create(
					MoveBy::create(0.5f, Vec2(0, screen_size.height)),
					ScaleTo::create(0, 0.0f), Place::create(item_point),
					DelayTime::create(max(0.0f, delay - 0.5f)),
					balloon_show, NULL));
		}
	}, dt, "starting path");
}

void TracingLetter::disableControlQuiz(float time)
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

void TracingLetter::enableControlQuiz()
{
	unschedule("disable_touch_quiz");
	//_panel_move->getEventDispatcher()->setEnabled(true);
	_panel_disable->setVisible(false);
}

TracingLetter_Item* TracingLetter::nextItem() {
	if (item_list.empty()) {
		return nullptr;
	}
	for (TracingLetter_Item* item : item_list) {
		if (!item->isColoring()) {
			return item;
		}
	}
	return nullptr;
}

void TracingLetter::activeItem(TracingLetter_Item* object) {
	if (object == NULL) {
		return;
	}
	if (object->isColoring()) {
		return;
	}
	if (!object->correct_sequence.empty()) {
		AUDIO_ENGINE->playSound("sounds/tracingletter/tl_dot_action.mp3");
		AUDIO_ENGINE->play(img_tracing->getAudios("name_1").front());
	}
	object->setColoring(true);
	//object->onTouchItem(true, 0, 0);
	object->onCorrectGAF(0);

	// hide gaf
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

void TracingLetter::speakQuestion()
{
	if (img_tracing) {
		std::vector<std::string> chain;
		chain.push_back("sounds/tracingletter/tl_question.mp3");
		chain.push_back(img_tracing->getAudios("name_1").front());
		AUDIO_ENGINE->playChain(chain);
	}

}

void TracingLetter::goNextPath()
{

	disableControlQuiz();
	for (auto item : item_list) {
		if (item) item->onHide(0.0f);
	}
	item_list.clear();
	if (is_completed) {
		//stopSpeakQuestion();
		//setState(Math_Quiz::QuizState::PAUSING);
		AUDIO_ENGINE->playEffect("sounds/tracingletter/tl_tracingdone.mp3");
		AUDIO_ENGINE->play("sounds/tracingletter/tl_good_job.mp3");
		CCLOG("next game");
		gaf_balloon->stopAllActions();
		gaf_balloon->runAction(Sequence::create(
			RotateTo::create(0.1, 5), RotateTo::create(0.2, -5), RotateTo::create(0.1, 0),
			ScaleTo::create(0.1f, 0.0f), NULL));
		scheduleOnce([=](float) {
			this->onExplanation(true);
		}, 0.5f, "go_completed");
	}
	else {
		int begin_of_next_path = trace_data.paths[index_path + 1].points.front();
		if (begin_of_next_path == end_of_last_path) {
			startTracing(true);
		}
		else {
			startTracing(true,0.25f);
		}
	}
}


void TracingLetter::onShowItem(Node * item, float delay, float duration, cocos2d::CallFunc * call_func)
{
	item->setScale(0);
	item->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::EaseElasticOut::create(cocos2d::ScaleTo::create(duration, 1)), call_func, NULL));
}

cocos2d::Color3B TracingLetter::getColor3B(std::string color)
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

void TracingLetter::drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2)
{
	if (_drawer) {
		gaf_balloon->setPosition(p2);
		_drawer->drawSegment(p1, p2, line_draw, color_line);
	}
}

void TracingLetter::shakingBalloon()
{
	moveable = false;
	drawable = false;
	if (!shake_balloon && !path_done) {
		shake_balloon = true;
		gaf_balloon->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.1, 5), cocos2d::RotateTo::create(0.2, -5), cocos2d::RotateTo::create(0.1, 0), NULL)));
	}
}

void TracingLetter::addEvents()
{
	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(TracingLetter::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(TracingLetter::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(TracingLetter::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(TracingLetter::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, _panel_move);

	GuideCenter->startGuide(CC_CALLBACK_0(TracingLetter::onGuideStarted, this), CC_CALLBACK_0(TracingLetter::onGuideDone, this));
	GuideCenter->setIncorrectTime(3);
}

TracingLetter_Item* TracingLetter::touchOnObject(cocos2d::Vec2 point) {
	for (TracingLetter_Item* object : item_list) {
		if (!object->isColoring() && object->isTouchedAt(point)) {
			return object;
		}
	}
	return nullptr;
}

bool TracingLetter::isTraceDone() {
	for (TracingLetter_Item* object : item_list) {
		if (!object->isColoring()) {
			return false;
		}
	}
	return true;
}

bool TracingLetter::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) {
	current_point = touch->getLocation();
	moveable = true;
	drawable = false;
	return moveable;
}

void TracingLetter::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
	if (moveable) {
		current_point = touch->getLocation();
		if (!drawable) {
			if (path_reload) {
				if (current_line.p1.distance(current_point) <= range_active) {
					drawable = true;
					last_point = current_point;
					activeItem(item_list.front());
				}
			}
			else {
				if (gaf_balloon->getPosition().distance(current_point) <= range_active) {
					drawable = true;
					last_point = current_point;
					drawSegment(gaf_balloon->getPosition(), current_point);
				}
			}
			if (drawable && !is_completed) {
				//TODO: Stop shake boat
				shake_balloon = false;
				gaf_balloon->stopAllActions();
				gaf_balloon->setOpacity(255);
				gaf_balloon->setScale(0.5f);
				gaf_balloon->setRotation(0);
			}
		}
		else {
			if (!isInPath(current_point)) {
				moveable = false;
				drawable = false;
				AUDIO_ENGINE->playSound("sounds/tracingletter/tl_dot_wrong.mp3");
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
		shakingBalloon();
	}
}

void TracingLetter::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
	shakingBalloon();
}

void TracingLetter::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
	shakingBalloon();
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void TracingLetter::autoTesting() {

	this->scheduleOnce([=](float dt) {
		//this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}



