#include "My_NumberLine.h"

My_NumberLine* My_NumberLine::createQuiz() {
	My_NumberLine *quiz = new My_NumberLine();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_NumberLine::init() {
	return cocos2d::ui::Layout::init();
}

void My_NumberLine::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;

	rapidjson::Value& json_style = json_game.FindMember("game_style")->value;
	std::vector<std::string> game_st;
	for (rapidjson::SizeType i = 0; i < json_style.Size(); i++) {
		game_st.push_back(json_style[i].GetString());
	}
	int rand = cocos2d::random(0, (int)game_st.size() - 1);
	game_style = game_st.at(rand);

	min_number = randomRangeJson(json_game, "min_number");
	distance = randomRangeJson(json_game, "distance");

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	number_object = randomRangeJson(json_object, "number");
	object_size = getSizeJson(json_object, "size");

	//Lấy dữ liệu phép toán
	if (game_style.compare("addition") == 0) {
		while (min_number > number_object + min_number - 1 - distance) {
			min_number = randomRangeJson(json_game, "min_number");
			distance = randomRangeJson(json_game, "distance");
		}

		operand = cocos2d::random(min_number, number_object + min_number - 1 - distance);
		result = operand + distance;
	}
	else {
		//Trường hợp phép trừ. game_style = subtraction
		while (min_number + distance > number_object + min_number - 1) {
			min_number = randomRangeJson(json_game, "min_number");
			distance = randomRangeJson(json_game, "distance");
		}
		operand = cocos2d::random(min_number + distance, min_number + number_object - 1);
		result = operand - distance;
	}

	//Lấy hình ảnh chữ số
	auto numbers = getNumbers(0, 20);
	int vt = -1;
	while (number_imgs.size() < number_object) {
		for (int i = 0; i < numbers.size(); i++) {
			if (numbers.at(i)->getComparisionValue("number") == min_number) {
				number_imgs.push_back(numbers.at(i));
				vt = i + 1;
				break;
			}
		}
		number_imgs.push_back(numbers.at(vt++));
	}

	//Tạo giá trị các đáp án
	std::string str1;
	if (game_style.compare("addition") == 0) {
		str1 = cocos2d::StringUtils::toString(operand) + " + " + cocos2d::StringUtils::toString(distance) + " = " + cocos2d::StringUtils::toString(result);
	}
	else {
		str1 = cocos2d::StringUtils::toString(operand) + " - " + cocos2d::StringUtils::toString(distance) + " = " + cocos2d::StringUtils::toString(result);
	}
	answer_values.push_back(str1);
	while (answer_values.size() < 4) {
		if (game_style.compare("addition") == 0) {
			int x = cocos2d::random(1, 5);
			std::string str2;
			str2 = cocos2d::StringUtils::toString(operand) + " + " + cocos2d::StringUtils::toString(x) + " = " + cocos2d::StringUtils::toString(operand + x);
			bool ok = true;
			for (int i = 0; i < answer_values.size(); i++) {
				if (answer_values.at(i).compare(str2) == 0) {
					ok = false;
					break;
				}
			}
			if (ok) {
				answer_values.push_back(str2);
			}
		}
		else {
			int x = cocos2d::random(1, 3);
			int y = cocos2d::random(operand - 1, operand + 1);
			std::string str2 = cocos2d::StringUtils::toString(y) + " - " + cocos2d::StringUtils::toString(x) + " = " + cocos2d::StringUtils::toString(y - x);
			bool ok = true;
			for (int i = 0; i < answer_values.size(); i++) {
				if (answer_values.at(i).compare(str2) == 0) {
					ok = false;
					break;
				}
			}
			if (ok) {
				answer_values.push_back(str2);
			}
		}
	}

	//Lấy background cho object
	bg_img = objectBG();

	//Question Config
	parseQuestion();
	if (game_style.compare("addition") == 0) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}
}

void My_NumberLine::onStart() {
	onShowBottomBar(0.5, 0.3);

	//Tạo Object
	int obj_id = 0;
	for (int i = 0; i < number_object; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		if (min_number + i == operand || min_number + i == result) {
			object->setEnable(obj_id++, math::common::deny, number_imgs.at(i), bg_img);
		}
		else {
			object->setEnable(obj_id++, math::common::deny, number_imgs.at(i), NULL);
		}
		object->setValueText(cocos2d::StringUtils::toString(min_number + i));
		object->setObjectSize(object_size*0.6);
		object->showValueTextWithAnimation(true);
		object->getLabelValue()->setFontSize(50);
		object->setImageVisible(false);
		_objects.push_back(object);
	}

	for (int i = 0; i < distance; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(0, math::common::deny, number_imgs.at(distance - 1), NULL);
		object->setObjectSize(object_size*0.6);
		object->setImageVisible(false);
		if (game_style.compare("addition") == 0) {
			object->setValueText("+1");
		}
		else {
			object->setValueText("-1");
		}
		object->showValueTextWithAnimation(true);
		object->getLabelValue()->setFontSize(30);
		operand_object.push_back(object);
	}

	//Tạo đáp án
	for (int i = 0; i < 4; i++) {
		Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
		answer->setEnable(answer_values.at(i), math::common::answer, CC_CALLBACK_1(My_NumberLine::onTouchAnswer, this), cocos2d::Size(250, 80));
		_answers.push_back(answer);
	}
	right_answer = _answers.at(0);
	std::random_shuffle(_answers.begin(), _answers.end());

	//Show object	
	math::func::setPositionList(_objects, number_object, cocos2d::Rect(0, 0.05, 1, 0.3));
	float delay = 0.5;
	for (Math_Object* object : _objects) {
		object->onShow(0);
		addChildNode(object, 2);
	}

	auto line_pos = _objects.at(0)->getPosition();

	//Show Answer
	math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.25));
	for (Math_Answer* answer : _answers) {
		answer->onShowInParent(_panel_bot, 2);
	}

	//Number Line
	float start_line = _objects[0]->getPosition().x;
	float delta_line = _objects[1]->getPosition().x - _objects[0]->getPosition().x;
	float line_y = _objects[0]->getPosition().y + object_size.height / 2 + 30;

	//line
	auto img_line = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
	img_line->setScale9Enabled(true);
	auto line_render = img_line->getVirtualRendererSize();
	img_line->setContentSize(cocos2d::Size(delta_line*(number_object - 0.5), line_render.height));
	img_line->setPosition(cocos2d::Vec2(quiz_size.width *0.5, line_y));
	addChildNode(img_line);
	ui_elements.push_back(img_line);

	//arrows
	auto img_arrow = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
	img_arrow->setPosition(cocos2d::Vec2((quiz_size.width + delta_line * (number_object - 0.5)) * 0.5 - 7, line_y));
	addChildNode(img_arrow);
	ui_elements.push_back(img_arrow);

	auto img_arrow2 = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
	img_arrow2->setPosition(cocos2d::Vec2((quiz_size.width - delta_line * (number_object - 0.5)) * 0.5 + 7, line_y));
	img_arrow2->setFlippedX(true);
	ui_elements.push_back(img_arrow2);
	addChildNode(img_arrow2);

	//sub lines
	for (int i = 0; i < number_object; i++) {
		auto img_subline = cocos2d::ui::ImageView::create(getUIPath("common/numberline/point.png"));
		img_subline->setPosition(cocos2d::Vec2(start_line + delta_line*i, line_y));
		addChildNode(img_subline);
		ui_elements.push_back(img_subline);
	}

	//curves
	if (game_style.compare("addition") == 0) {
		for (int i = operand - min_number; i < result - min_number; i++) {
			auto curve_arrow = cocos2d::ui::ImageView::create(getUIPath("common/line/curve.png"));
			curve_arrow->setFlippedX(true);
			curve_arrow->setScale9Enabled(true);
			auto render_size = curve_arrow->getVirtualRendererSize();
			cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
			curve_arrow->setCapInsets(cap);
			curve_arrow->setContentSize(cocos2d::Size(delta_line, object_size.height / 3));
			curve_arrow->setPosition(cocos2d::Vec2(_objects[i]->getPosition().x + delta_line / 2, img_line->getPosition().y + 35));
			addChildNode(curve_arrow);
			ui_elements.push_back(curve_arrow);
		}
	}
	else {
		for (int i = result - min_number; i < operand - min_number; i++) {
			auto curve_arrow = cocos2d::ui::ImageView::create(getUIPath("common/line/curve.png"));
			curve_arrow->setScale9Enabled(true);
			auto render_size = curve_arrow->getVirtualRendererSize();
			cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
			curve_arrow->setCapInsets(cap);
			curve_arrow->setContentSize(cocos2d::Size(delta_line, object_size.height / 3));
			curve_arrow->setPosition(cocos2d::Vec2(_objects[i]->getPosition().x + delta_line / 2, img_line->getPosition().y + 35));
			addChildNode(curve_arrow);
			ui_elements.push_back(curve_arrow);
		}
	}

	for (int i = 0; i < operand_object.size(); i++) {
		if (game_style.compare("addition") == 0) {
			operand_object.at(i)->setPosition(cocos2d::Vec2((_objects.at(operand - min_number + i)->getPosition().x + _objects.at(operand - min_number + i + 1)->getPosition().x) / 2, img_line->getPosition().y + object_size.height / 2 + 30));
		}
		else {
			operand_object.at(i)->setPosition(cocos2d::Vec2((_objects.at(result - min_number + i)->getPosition().x + _objects.at(result - min_number + i + 1)->getPosition().x) / 2, img_line->getPosition().y + object_size.height / 2 + 30));
		}
		operand_object.at(i)->onShow(0);
		addChildNode(operand_object.at(i), 2);
	}

	//Show question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_NumberLine::startQuestion() {
	CCLOG("Start Question!");
}

void My_NumberLine::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
}

void My_NumberLine::onComplete() {
	float delay = showConratulation(0.3);

	delay = 1;
	for (Math_Object* object : _objects) {
		object->onHide(2);
	}

	scheduleOnce([=](float dt) {
		for (cocos2d::ui::Widget* im : ui_elements) {
			im->removeFromParent();
		}
	}, 2, "hide ui");	

	for (Math_Answer* answer : _answers) {
		answer->setDisable();
	}

	for (Math_Object* object : operand_object) {
		object->onHide(0);
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, 2, "complete_quiz");
}

void My_NumberLine::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	bool done = false;
	if (answer == right_answer) {
		done = true;
	}

	answer->onAnimationClick(done);
	this->onExplanation(done);
}

void My_NumberLine::drawSemiCircle(cocos2d::DrawNode* node, cocos2d::Point origin, float radius, float degree, cocos2d::Color4F color, int num_points) {
	auto step = 2 * M_PI*degree / 360.f / num_points;
	std::vector<cocos2d::Point> vec_pos;
	for (int i = 0; i < num_points; i++) {
		auto rads = -step * i + (180 - degree) / 2;
		cocos2d::Point pos;
		pos.x = origin.x + radius * cosf(rads);
		pos.y = origin.y + radius * sinf(rads);
		vec_pos.push_back(pos);
	}

	for (int i = 0; i < num_points - 1; i++) {
		node->drawLine(vec_pos.at(i), vec_pos.at(i + 1), color);
	}
}
