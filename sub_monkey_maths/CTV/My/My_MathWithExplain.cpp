#include "My_MathWithExplain.h"

My_MathWithExplain* My_MathWithExplain::createQuiz() {
	My_MathWithExplain* quiz = new My_MathWithExplain();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_MathWithExplain::init() {
	return cocos2d::ui::Layout::init();
}

void My_MathWithExplain::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	std::vector<std::string> game_st_vt;
	std::vector<std::string> answer_st;
	rapidjson::Value& json_style = json_game.FindMember("game_style")->value;
	for (rapidjson::SizeType i = 0; i < json_style.Size(); i++) {
		game_st_vt.push_back(json_style[i].GetString());
	}
	int temp = cocos2d::random(0, (int)game_st_vt.size() - 1);
	game_style = game_st_vt.at(temp);
	rapidjson::Value& answer_style = json_game.FindMember("answer_style")->value;
	for (rapidjson::SizeType i = 0; i < answer_style.Size(); i++) {
		answer_st.push_back(answer_style[i].GetString());
	}
	int ran = cocos2d::random(0, (int)answer_st.size() - 1);
	if (answer_st.at(ran).compare("answer") == 0) answer_display = true;
	else answer_display = false;
	//answer_display = json_game.FindMember("answer_display")->value.GetBool();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	number_object = cocos2d::random(json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();

	if (game_style.compare("addition") == 0) {
		int x;
		if (number_object > 15) x = 10;
		else if (number_object > 10) x = cocos2d::random(6, 10);
		else x = cocos2d::random(1, number_object - 1);
		operand_1 = x;
		operand_2 = number_object - operand_1;
		result = number_object;
	}
	else {
		operand_1 = cocos2d::random(2, 10);
		operand_2 = cocos2d::random(1, operand_1 - 1);
		result = operand_1 - operand_2;
	}

	//Lấy hình ảnh hiển thị cho Object
	auto imgs = getImageList("icons");
	int vt = cocos2d::random(0, (int)imgs.size() - 1);
	object_image = imgs.at(vt);

	auto numbers = getNumbers(0, 20);

	//Lấy giá trị cho các đáp án
	if (answer_display) {
		answer_values.push_back(result);
		while (answer_values.size() < 4) {
			int temp;
			if (game_style.compare("addition") == 0) temp = cocos2d::random(1, 20);
			else temp = cocos2d::random(1, 10);
			bool ok = true;
			for (int i = 0; i < answer_values.size(); i++) {
				if (answer_values.at(i) == temp) {
					ok = false;
					break;
				}
			}
			if (ok) answer_values.push_back(temp);
		}
	}
	else {
		std::string str1;
		if (game_style.compare("addition") == 0) str1 = cocos2d::StringUtils::toString(operand_1) + " + " + cocos2d::StringUtils::toString(operand_2) + " = " + cocos2d::StringUtils::toString(result);
		else str1 = cocos2d::StringUtils::toString(operand_1) + " - " + cocos2d::StringUtils::toString(operand_2) + " = " + cocos2d::StringUtils::toString(result);
		answer_strings.push_back(str1);
		std::vector<int> temp_values;
		temp_values.push_back(result);
		while (answer_strings.size() < 4) {
			int temp;
			if (result < 4) temp = cocos2d::random(result + 1, result + 4);
			else temp = cocos2d::random(result - 3, result + 3);
			bool ok = true;
			for (int i = 0; i < temp_values.size(); i++) {
				if (temp_values.at(i) == temp) {
					ok = false;
					break;
				}
			}
			if (ok) {
				temp_values.push_back(temp);
				int x;
				if (game_style.compare("addition") == 0) x = cocos2d::random(0, temp - 1);
				else x = cocos2d::random(temp + 1, 10);
				std::string str2;
				if (game_style.compare("addition") == 0) str2 = cocos2d::StringUtils::toString(x) + " + " + cocos2d::StringUtils::toString(temp - x) + " = " + cocos2d::StringUtils::toString(temp);
				else str2 = cocos2d::StringUtils::toString(x) + " - " + cocos2d::StringUtils::toString(temp) + " = " + cocos2d::StringUtils::toString(x - temp);
				answer_strings.push_back(str2);
			}
		}
	}

	auto img1 = getNumbers(operand_1, operand_1);
	auto img2 = getNumbers(operand_2, operand_2);
	auto img3 = getNumbers(result, result);
	std::vector<math::resource::Image*> img_number_list;
	img_number_list.push_back(img1.at(0));
	img_number_list.push_back(img2.at(0));
	img_number_list.push_back(img3.at(0));
	auto names = getPeopleNames(2);

	//Question Conifg
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

		else if (key == "people_name") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : names) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "person_name") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : names) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "kind") {
			std::vector<math::resource::Text> texts;
			auto img_txt = object_image->getTextValue("kind", m_lang->getLanguage());
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "family") {
			std::vector<math::resource::Text> texts;
			auto img_txt = object_image->getTextValue("family", m_lang->getLanguage());
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "color") {
			std::vector<math::resource::Text> texts;
			auto img_txt = object_image->getTextValue("color", m_lang->getLanguage());
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			for (int i = 0; i < img_number_list.size(); i++) {
				auto number = img_number_list[i]->getComparisionValue("number");
				if (i == 2) {
					number = 3;
				}
				if (number < 2) {
					auto img_txt = object_image->getTextValue("name_1", m_lang->getLanguage());
					texts.push_back(img_txt);
				}
				else {
					auto img_txt = object_image->getTextValue("name_n", m_lang->getLanguage());
					texts.push_back(img_txt);
				}
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}

	if (game_style.compare("addition") == 0) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}
}

void My_MathWithExplain::onStart() {
	//Gen Object 1;
	int obj_id_1 = 0;
	int obj_id_2 = 0;
	int x;
	if (game_style.compare("addition") == 0) x = operand_1;
	else x = result;
	for (int i = 0; i < x; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id_1++, math::common::deny, object_image, NULL);
		object->setObjectSize(object_size);
		object->setOpacity(225);
		_objects_1.push_back(object);
	}

	for (int i = 0; i < operand_2; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id_2++, math::common::deny, object_image, NULL);
		object->setObjectSize(object_size);
		object->setOpacity(225);
		_objects_2.push_back(object);
	}

	//Gen Answers
	if (answer_display) {
		for (int i = 0; i < 4; i++) {
			Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
			answer->setEnable(cocos2d::StringUtils::toString(answer_values.at(i)), math::common::answer, CC_CALLBACK_1(My_MathWithExplain::onTouchAnswer, this), cocos2d::Size(200, 60));
			_answers.push_back(answer);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
			answer->setEnable(answer_strings.at(i), math::common::answer, CC_CALLBACK_1(My_MathWithExplain::onTouchAnswer, this), cocos2d::Size(300, 60));
			_answers.push_back(answer);
		}
	}
	ans = _answers.at(0);
	std::random_shuffle(_answers.begin(), _answers.end());

	//Show Question
	showQuestionBox();
	disableControlQuiz(5);

	//Show object
	disableControlQuiz(5);
	if (game_style.compare("addition") == 0) {
		math::func::setPositionList(_objects_1, 10, cocos2d::Rect(0, 0.3, 1, 0.3));
	}
	else math::func::setPositionList(_objects_1, 10, cocos2d::Rect(0, 0.5, 1, 0.3));
	math::func::setPositionList(_objects_2, 10, cocos2d::Rect(0, 0.3, 1, 0.3));
	math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.2));
	float delay = 0;
	for (Math_Object* object : _objects_1) {
		addChildNode(object, 2);
		object->onShow(delay);
		delay += 0.15;
	}

	float delay_1 = 0;
	if (game_style.compare("addition") == 0) delay_1 = 5;
	else delay_1 = 0;
	for (Math_Object* object : _objects_2) {
		addChildNode(object, 2);
		object->onShow(delay_1);
		delay_1 += 0.15;
	}

	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(5), cocos2d::CallFunc::create([=]() {
		if (game_style.compare("subtraction") == 0) {
			/*for (int i = result; i < _objects_1.size(); i++) {
				_objects_1.at(i)->setOpacity(0);
			}*/

			for (Math_Object* obj : _objects_2) {
				obj->setOpacity(100);
			}
		}
		else math::func::setPositionList(_objects_1, 10, cocos2d::Rect(0, 0.5, 1, 0.3), true);
		this->nextQuestion();
	}), cocos2d::DelayTime::create(3), cocos2d::CallFunc::create([=]() {
		this->nextQuestion();
		if (game_style.compare("subtraction") == 0) {
			/*for (int i = result; i < _objects_1.size(); i++) {
			_objects_1.at(i)->setOpacity(0);
			}*/
			math::func::setPositionList(_objects_1, 10, cocos2d::Rect(0, 0.25, 1, 0.3));
			for (Math_Object* obj : _objects_2) {
				obj->setOpacity(100);
			}
		}
		else math::func::setPositionList(_objects_1, 10, cocos2d::Rect(0, 0.25, 1, 0.3));
		math::func::setPositionList(_objects_2, 10, cocos2d::Rect(0, 0.05, 1, 0.3));
		onShowBottomBar(0.5);
		for (Math_Answer* answer : _answers) {
			answer->onShowInParent(_panel_bot, 2);
		}
	}), NULL));
}

void My_MathWithExplain::startQuestion() {

}

void My_MathWithExplain::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	if (answer == ans) {
		this->onExplanation(true);
		answer->onAnimationClick(true);
		return;
	}
	answer->onAnimationClick(false);
}

void My_MathWithExplain::onExplanation(bool correct) {
	if (correct) this->nextQuestion();
}

void My_MathWithExplain::onComplete() {
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	for (Math_Object* obj : _objects_1) {
		obj->onHide(m_delay);
		m_delay += 0.1;
	}

	for (Math_Object* obj : _objects_2) {
		obj->onHide(m_delay);
		m_delay += 0.1;
	}

	for (Math_Answer* answer : _answers) {
		answer->setDisable();
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, m_delay, "complete_quiz");
}
