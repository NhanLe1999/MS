#include "My_OneMoreOneLessWhitCount.h"

My_OneMoreOneLessWhitCount* My_OneMoreOneLessWhitCount::createQuiz() {
	My_OneMoreOneLessWhitCount* quiz = new My_OneMoreOneLessWhitCount();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_OneMoreOneLessWhitCount::init() {
	return cocos2d::ui::Layout::init();
}

void My_OneMoreOneLessWhitCount::initQuiz(mj::model::Game game) {

	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("data")->value;

	//GAME Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	std::string game_mode = json_game.FindMember("compare_mode")->value.GetString();
	if (game_mode.compare("more") == 0) {
		game_type = GameType::More;
	}
	else if (game_mode.compare("less") == 0) {
		game_type = GameType::Less;
	}
	else if (game_mode.compare("equal") == 0) {
		game_type = GameType::Equal;
	}
	else if (game_mode.compare("more_less") == 0) {
		if (cocos2d::random(0, 1) == 0) {
			game_type = GameType::More;
		}
		else {
			game_type = GameType::More;
		}
	}
	else {
		int rand_val = cocos2d::random(0, 2);
		if (rand_val == 0) {
			game_type = GameType::More;
		}
		else if (rand_val == 1) {
			game_type = GameType::Less;
		}
		else {
			game_type = GameType::Equal;
		}
	}

	number_question = 0;

	int numberobject_min = json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt();
	int numberobject_max = json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt();
	number_object = cocos2d::random(numberobject_min, numberobject_max);

	if (game_type != GameType::Equal) {
		int number_ques_min = json_game.FindMember("number_question")->value.FindMember("minimum")->value.GetInt();
		int number_ques_max = json_game.FindMember("number_question")->value.FindMember("maximum")->value.GetInt();

		number_question = cocos2d::random(number_ques_min, number_ques_max);
		if (game_type == GameType::More) {
			while (number_object + number_question > numberobject_max) {
				number_object = cocos2d::random(numberobject_min, numberobject_max);
				number_question = cocos2d::random(number_ques_min, number_ques_max);
			}
		}
		else if (game_type == GameType::Less) {
			while (number_object <= number_question) {
				number_object = cocos2d::random(numberobject_min, numberobject_max);
				number_question = cocos2d::random(number_ques_min, number_ques_max);
			}
		}
	}

	//Group Config
	rapidjson::Value& json_group = json_data.FindMember("group")->value;
	std::string group_type = json_group.FindMember("group_type")->value.GetString();
	int number_group = 2;

	//Group Config
	std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
	std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();

	img_list = getImageList("icons", 1, icon_filter, icon_same);

	std::vector<int> numbers;

	switch (game_type) {
	case GameType::More:
	{
		numbers.push_back(number_object + number_question);
		std::vector<int> wrong_list;
		wrong_list.push_back(number_object);
		if (number_object > number_question) {
			wrong_list.push_back(number_object - number_question);
		}
		if (number_object + number_question + 1 <= numberobject_max) {
			wrong_list.push_back(number_object + number_question + 1);
		}
		if (number_question != 1) {
			wrong_list.push_back(number_object + 1);
			wrong_list.push_back(number_object - 1);
		}
		math::func::shuffle(wrong_list);
		while (numbers.size() < number_group) {
			numbers.push_back(wrong_list.back());
			wrong_list.pop_back();
		}
	}
	break;
	case GameType::Less:
	{
		numbers.push_back(number_object - number_question);

		std::vector<int> wrong_list;
		wrong_list.push_back(number_object);
		if (number_object + number_question < numberobject_max) {
			wrong_list.push_back(number_object + number_question);
		}
		if (number_question != 1) {
			wrong_list.push_back(number_object - 1);
		}
		else {
			wrong_list.push_back(number_object + 1);
		}
		math::func::shuffle(wrong_list);
		while (numbers.size() < number_group) {
			numbers.push_back(wrong_list.back());
			wrong_list.pop_back();
		}
	}
	break;
	default:
	{
		numbers.push_back(number_object);
		std::vector<int> wrong_list;
		if (number_object + 1 <= numberobject_max) {
			wrong_list.push_back(number_object + 1);
		}
		if (number_object > 1) {
			wrong_list.push_back(number_object - 1);
		}
		if (number_object + 2 <= numberobject_max) {
			wrong_list.push_back(number_object + 2);
		}
		if (number_object > 2) {
			wrong_list.push_back(number_object - 2);
		}

		math::func::shuffle(wrong_list);
		while (numbers.size() < number_group) {
			numbers.push_back(wrong_list.back());
			wrong_list.pop_back();
		}
	}
	break;
	}

	int number_max = 0;
	img_number_list = getNumbers(number_question, number_question);

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
				auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else {
			getQuestion()->setOptionalTexts(key, img_list);
		}
	}
	img_number_list.clear();
	auto all_number = getNumbers(0, number_max);
	for (int number : numbers) {
		if (number > number_max) {
			number_max = number;
		}
		for (math::resource::Image* img : all_number) {
			if (img->getComparisionValue("number") == number) {
				img_number_list.push_back(img);
				break;
			}
		}
	}

	//Lấy backgroud cho group
	group_bg = groupBG();

	int delta = 10;
	if (number_group == 2) {
		delta = 40;
	}

	cocos2d::Size object_size = cocos2d::Size(90, 90);
	cocos2d::Size group_size = cocos2d::Size(90 * 1.2 * 5 + delta, 100);

	if (number_max > 5) {
		object_size = cocos2d::Size(70, 70);
		group_size = cocos2d::Size(70 * 1.2 * 10 + delta, 90);
	}

	for (int i = 0; i < number_object; i++) {
		Math_Object* object = mpool->getObject();
		object->setEnable(i, math::common::touch, img_list[0], NULL);
		object->setObjectSize(object_size);
		object->setTouchCallback(CC_CALLBACK_1(My_OneMoreOneLessWhitCount::onTouchObject, this));
		_objects.push_back(object);
	}

	temp_group = mpool->getGroup();
	temp_group->setEnable(0, _objects, math::group::queue, number_max);
	temp_group->setObjectSize(object_size);
	temp_group->setGroupQueue(group_size);
	temp_group->setBackground(group_bg);
	temp_group->setPosition(cocos2d::Vec2(quiz_size.width / 2, quiz_size.height / 2));

	//Tạo các Groups
	for (int i = 0; i < number_group; i++) {
		std::vector<Math_Object*> _group_list;
		for (int j = 0; j < numbers.at(i); j++) {
			Math_Object* object = mpool->getObject();
			object->setEnable(i, math::common::deny, img_list[0], NULL);
			object->setObjectSize(object_size);
			_group_list.push_back(object);
		}
		Math_Group* group = mpool->getGroup();
		group->setEnable(i + 1, _group_list, math::group::custom, number_max);
		group->setObjectSize(object_size);
		group->setGroupSize(cocos2d::Size(object_size.width * 4, object_size.height * 3));
		group->setGroupQueue(cocos2d::Size(object_size.width * 4, object_size.height * 3));
		group->setBackground(group_bg);
		group->enableToch(CC_CALLBACK_1(My_OneMoreOneLessWhitCount::onTouchGroup, this));
		_groups.push_back(group);
	}
	math::func::shuffle(_groups);
}

void My_OneMoreOneLessWhitCount::onStart() {
	//Tạo Objects để đếm
	addChildNode(temp_group);
	temp_group->onShow();

	math::func::setPositionList(_groups, 2, cocos2d::Rect(0, 0.1, 1, 0.4));

	showQuestionBox();
	disableControlQuiz();
}

void My_OneMoreOneLessWhitCount::startQuestion() {
	CCLOG("Starting question");

	enableControlQuiz();
	int index = getQuestion()->getData().index;
	if (index == 1) {
		temp_group->runAction(cocos2d::MoveTo::create(0.2, cocos2d::Vec2(quiz_size.width / 2, quiz_size.height / 4 * 2.8)));
		for (Math_Group* group : _groups) {
			addChildNode(group);
			group->onShow();
		}
	}
}

void My_OneMoreOneLessWhitCount::onTouchObject(cocos2d::Node * node) {
	Math_Object* object = (Math_Object*)node;
	int vt = 0;
	for (int i = 0; i < number_object; i++) {
		if (_objects.at(i) == object) {
			vt = i;
			break;
		}
	}

	object->setValueText(cocos2d::StringUtils::format("%d", number_counting + 1));

	number_counting++;
	object->showValueTextWithAnimation(true);
	object->getLabelValue()->setFontSize(50);
	object->setLocked(true);

	if (number_counting == number_object) {
		counting = false;
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=]() {
			this->onExplanation(true);
		}), NULL));
	}
}

void My_OneMoreOneLessWhitCount::onTouchGroup(cocos2d::Node* node) {
	Math_Group* group = (Math_Group*)node;
	int result = number_object;
	if (game_type == GameType::More) {
		result = number_object + number_question;
	}
	else if (game_type == GameType::Less) {
		result = number_object - number_question;
	}

	if (group->getNumberObjects() == result) {
		group->onTouchedAnimation(true);
		onExplanation(true);
	}
	else {
		group->onTouchedAnimation(false);
	}
}

void My_OneMoreOneLessWhitCount::onExplanation(bool correct) {
	if (correct) {
		disableControlQuiz();
		this->nextQuestion();
	}
}

void My_OneMoreOneLessWhitCount::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}
	scheduleOnce([=](float dt) {
		temp_group->setDisable();
	}, m_delay, "hide_groups_temp");

	scheduleOnce([=](float dt) {
		for (Math_Group* group : _groups) {
			group->setDisable();
		}
	}, m_delay + 0.4, "hide_groups");

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}


void My_OneMoreOneLessWhitCount::autoTesting() {

}

