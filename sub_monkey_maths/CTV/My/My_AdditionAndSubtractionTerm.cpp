#include "My_AdditionAndSubtractionTerm.h"

My_AdditionAndSubtractionTerm* My_AdditionAndSubtractionTerm::createQuiz() {
	My_AdditionAndSubtractionTerm* quiz = new My_AdditionAndSubtractionTerm();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_AdditionAndSubtractionTerm::init() {
	return cocos2d::ui::Layout::init();
}

void My_AdditionAndSubtractionTerm::initQuiz(mj::model::Game game) {
    Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	operation_type = json_game.FindMember("operation_type")->value.GetString();
	rapidjson::Value& json_range = json_data.FindMember("range")->value;
	int minimum = json_range.FindMember("minimum")->value.GetInt();
	int maximum = json_range.FindMember("maximum")->value.GetInt();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();

	//Vector lưu tên gọi các thành phần trong phép toán
	term_values.push_back("addend");
	term_values.push_back("sum");
	term_values.push_back("minuend");
	term_values.push_back("subtrahend");
	term_values.push_back("difference");

	//Tạo phép toán
	if (operation_type.compare("addition") == 0) {
		operand_1 = cocos2d::random(1, maximum - 1);
		do {
			operand_2 = cocos2d::random(1, maximum - operand_1);
		} while (operand_2 == operand_1 || operand_1 - operand_2 == operand_2);

		if (operand_1 + operand_2 > maximum) {
			result = maximum;
			operand_2 = result - operand_1;
		}
		if (operand_1 + operand_2 < minimum) {
			result = minimum;
			operand_1 = cocos2d::random(0, minimum);
			operand_2 = result - operand_1;	
		}
		result = operand_1 + operand_2;
	}
	else {
		//operation_type = "subtraction"
		operand_1 = cocos2d::random(maximum - 2, maximum);
		do {
			operand_2 = cocos2d::random(1, operand_1 - 1);
		} while (operand_2 == operand_1 || operand_1 - operand_2 == operand_2);
		result = operand_1 - operand_2;		
	}

	auto numbers = getNumbers(0, maximum);
	auto opera = getOperators();

	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == operand_1) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 2) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == operand_2) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 1) {
			if (operation_type.compare("addition") == 0) {
				for (int j = 0; j < opera.size(); j++) {
					if (opera.at(j)->getText("name_n").compare("plus") == 0) {
						icons.push_back(opera.at(j));
						break;
					}
				}
			}
			else {
				for (int j = 0; j < opera.size(); j++) {
					if (opera.at(j)->getText("name_n").compare("minus") == 0) {
						icons.push_back(opera.at(j));
						break;
					}
				}
			}
		}
		else if (i == 3) {
			for (int j = 0; j < opera.size(); j++) {
				if (opera.at(j)->getText("name_n").compare("equal") == 0) {
					icons.push_back(opera.at(j));
					break;
				}
			}
		}
		else if (i == 4) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == result) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
	}

	int x = cocos2d::random(1, 3);
	if (operation_type.compare("subtraction") == 0) {
		if (x == 1) {
			term_value = operand_1;
			term = "minuend";
		}
		if (x == 2) {
			term_value = operand_2;
			term = "subtrahend";
		}
		if (x == 3) {
			term_value = result;
			term = "difference";
		}
	}
	else {
		//operation_type = "addition"
		if (x == 1) {
			term_value = operand_1;
			term = "addend";
		}
		if (x == 2) {
			term_value = operand_2;
			term = "addend";
		}
		if (x == 3) {
			term_value = result;
			term = "sum";
		}
	}

	answer_values.push_back(term);
	while (answer_values.size() < 4) {
		int y = cocos2d::random(0, (int)term_values.size() - 1);
		bool ok = true;
		for (int i = 0; i < answer_values.size(); i++) {
			if (answer_values.at(i).compare(term_values.at(y)) == 0) {
				ok = false;
				break;
			}
		}
		if (ok) {
			answer_values.push_back(term_values.at(y));
		}
	}

	auto img_number_list = getNumbers(term_value, term_value);
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : img_number_list) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : img_number_list) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
}


void My_AdditionAndSubtractionTerm::onStart() {
	onShowBottomBar(0.25, 0.3);

	//Tạo Object
	int obj_id = 0;
	std::string s;
	if (operation_type.compare("addition") == 0) {
		s = "+";
	}
	else {
		s = "-";
	}
	for (int i = 0; i < 5; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, icons.at(i), NULL);
		object->setObjectSize(object_size);
		if (i == 0) {
			object->setValueText(cocos2d::StringUtils::toString(operand_1));
		}
		if (i == 1) {
			object->setValueText(s);
		}
		if (i == 2) {
			object->setValueText(cocos2d::StringUtils::toString(operand_2));
		}
		if (i == 3) {
			object->setValueText("=");
		}
		if (i == 4) {
			object->setValueText(cocos2d::StringUtils::toString(result));
		}
		object->setImageVisible(false);
		object->showValueTextWithAnimation(true);
		_objects.push_back(object);
	}

	//Tạo đáp án lựa chọn
	for (int i = 0; i < 4; i++) {
		Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
		answer->setEnable(answer_values.at(i), math::common::answer, CC_CALLBACK_1(My_AdditionAndSubtractionTerm::onTouchAnswer, this), getAnswerSize(2));
		_answers.push_back(answer);
	}
	right_answer = _answers.at(0);
	std::random_shuffle(_answers.begin(), _answers.end());

	//Show Object
	float delay = 0;
	math::func::setPositionList(_objects, 5, cocos2d::Rect(0.15, 0.15, 0.7, 0.3));
	for (Math_Object* object : _objects) {
		object->onShow(delay);
		delay += 0.15;
		addChildNode(object, 2);
	}

	//Show Answer
	math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.25));
	for (Math_Answer* answer : _answers) {
		answer->onShowInParent(_panel_bot, 2);
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_AdditionAndSubtractionTerm::startQuestion() {
	CCLOG("Start Question");
}

void My_AdditionAndSubtractionTerm::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
}

void My_AdditionAndSubtractionTerm::onComplete() {
	float delay = showConratulation(0.3);

	for (Math_Object* object : _objects) {
		object->onHide(delay);
		delay += 0.15;
	}

	for (Math_Answer* answer : _answers) {
		answer->setDisable();
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_AdditionAndSubtractionTerm::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;

	bool done;
	if (answer == right_answer) {
		done = true;
	}
	else {
		done = false;
	}
	onExplanation(done);
	answer->onAnimationClick(done);
}