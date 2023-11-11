//
//  NAM_GameTest.cpp
//  MJQuiz
//
//  Created by tunado on 7/29/17.
//
//

#include "Nam_AddObjectAndAnswerQuiz.h"
#include "Nam_AddObjectAndAnswerQuiz_On_Touch.h"

Nam_AddObjectAndAnswerQuiz* Nam_AddObjectAndAnswerQuiz::createQuiz() {
	Nam_AddObjectAndAnswerQuiz* quiz = new Nam_AddObjectAndAnswerQuiz();

	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}
bool Nam_AddObjectAndAnswerQuiz::init() {
	return cocos2d::ui::Layout::init();
}
void Nam_AddObjectAndAnswerQuiz::initQuiz(mj::model::Game game) {
	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	sequence_line = json_game.FindMember("sequence_line")->value.GetBool();
    max_total = std::min(json_game.FindMember("number_icon")->value.FindMember("max")->value.GetInt(),8);
	total = cocos2d::RandomHelper::random_int(
        std::max(json_game.FindMember("number_icon")->value.FindMember("min")->value.GetInt(),2),
		max_total
	);
    game_mode = json_game.FindMember("activity")->value.GetString();
    //game_mode = "add";
	// RANDOM cho double add
    
    if (game_mode == "doubleadd"){
        number_icon_left = total / 2;
        number_icon_right = total / 2;
        total = number_icon_left * 2;
    }
    else{
        number_icon_left = random(1,std::min(4,total-1));
        number_icon_right = (total - number_icon_left) < 5 ? (total - number_icon_left) : 4;
        total =number_icon_left + number_icon_right;
    }
	numbers_show_begin = json_game.FindMember("display_numbers")->value.FindMember("show_begin")->value.GetBool();
	display_sequence_number = json_game.FindMember("display_numbers")->value.FindMember("sequence_numbers")->value.GetBool();
	style_display_icon = json_game.FindMember("style_display_icon")->value.FindMember("style")->value.GetString();
	divide_icon = json_game.FindMember("style_display_icon")->value.FindMember("divide")->value.GetBool();
	number_of_line = json_game.FindMember("style_display_icon")->value.FindMember("number_of_line")->value.GetInt();

	//TODO: lấy dữ liệu object
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	float object_height = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	float object_width = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();
	object_size.height = object_height;
	object_size.width = object_width;
	ratio_wild = (object_size.width / 900)*0.9;
	object_bg = json_object.FindMember("background")->value.GetString();
	//TODO: lấy tất cả ảnh trong media
	image_icon = getImageList("icons", 1, "name_1")[0];
	image_icon->used = true;
	math::resource::Image* img_slot = objectBG();
	img_slot->used = true;
	img_bg_number = math::resource::ResHelper::createImage((m_theme->getThemePath() + "common/frame/square2.png"));
	img_bg_number->used = true;
	img_frame_object = math::resource::ResHelper::createImage((m_theme->getThemePath() + "common/frame/square3.png"));
	img_frame_object->used = true;
	// TODO lay anh so va dau
    img_result_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    img_result_bg->used = true;
	name_icon = image_icon->path;
	image_number.push_back(this->getNumbers(number_icon_left, number_icon_left)[0]);
	image_number.push_back(this->getNumbers(number_icon_right, number_icon_right)[0]);
	image_number.push_back(this->getNumbers(total, total)[0]);
	for (auto img : image_number) {
		img->used = true;
	}
	image_equal = this->getOperator("=");
	image_plus = this->getOperator("+");
	image_plus->used = true;
	image_equal->used = true;
	// TODO: Xử lý dữ liệu cấu hình câu hỏi
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = image_number[1]->getTextValue("name_1", m_lang->getLanguage());
			t.text = cocos2d::StringUtils::toString(image_number[1]->getComparisionValue("number"));
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			if (number_icon_left != 1) {
				math::resource::Text t = image_icon->getTextValue("name_n", m_lang->getLanguage());
				texts.push_back(t);
			}
			else {
				math::resource::Text t = image_icon->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(t);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	
	// get number result
	for (int i = 1; i <= 3; i++) {
		int x;
		while (true)
		{
			int kt = true;
			x = RandomHelper::random_int(1, max_total);
			if (x == total) kt = false;
			for (int j : number_result) {
				if (x == j) {
					kt = false;
				}
			}
			if (kt) break;
		}
		number_result.push_back(x);
	}
	number_result.push_back(total);
	std::random_shuffle(number_result.begin(), number_result.end());
	// GEN LEN INIT
	//TODO: gen Object
	// gen object ben trai
	for (int i = 0; i < number_icon_left; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(i, math::common::deny, image_icon, NULL);
		object->getImage()->setScaleX(-1);
		object->setObjectSize(object_size);
		left_objects.push_back(object);
		_objects.push_back(object);
	}
	// gen object line
    imgline = math::resource::ResHelper::createImage(getUIPath("common/line.png"));
	linePath = m_theme->getThemePath() + "common/line.png";
	imgline->used = true;
	line = Math_Pool::getInstance()->getObject();
	line->setEnable(0, math::common::touch, imgline, NULL);
	line->setObjectSize(object_size);
	// khoi tao dem object ben phai
	countObject = 0;
	
	// gen answer

	for (int i = 1; i <= 4; i++) {
		Math_Answer* btn;
		btn = Math_Pool::getInstance()->getAnswer();
		btn->setEnable(cocos2d::StringUtils::toString(number_result[i - 1]), math::common::answer, CC_CALLBACK_1(Nam_AddObjectAndAnswerQuiz::onAnswerDone2, this, number_result[i - 1]), getAnswerSize());
		btn->setCustomAnswer(img_slot, Size(120, 120), 80, Color4B(Color3B(99,51,15)));
		btn_result.push_back(btn);
		_answers.push_back(btn);
	}
	if (style_display_icon != "v style") math::func::setPositionList(btn_result, _answers.size(), Rect(0.1, 0, 0.8, 0.22));
	else math::func::setPositionList(btn_result, _answers.size(), Rect(0.1, 0, 0.8, 0.22));
	//Gen lines neu o che do hien san
	if (!sequence_line) {
		for (int i = 0; i < number_icon_right; i++) {
			CCLOG("%s", imgline->path.c_str());
			Math_Object* l = Math_Pool::getInstance()->getObject();
			l->setEnable(0, math::common::touch, imgline, NULL);
			l->setObjectSize(object_size);
            l->getImage()->setColor(Color3B::WHITE);
            if (style_display_icon != "v style") l->setPosition(straight_right(number_icon_right).at(i));
            else {
                l->setPosition(v_style_right(number_icon_right).at(i));
            }
			lines.push_back(l);
			_objects.push_back(l);
			lines.at(i)->setImage(imgline);
			lines.at(i)->setTouchCallback(CC_CALLBACK_1(Nam_AddObjectAndAnswerQuiz::onTouchObject2, this, i));
		}
	}
	else {
		if (style_display_icon != "v style") line->setPosition(straight_right(1).at(0));
		else line->setPosition(v_style_right(1).at(0));
		///this->addChildNode(line, 2);
		line->setTouchCallback(CC_CALLBACK_1(Nam_AddObjectAndAnswerQuiz::AddObject, this));
		_objects.push_back(line);
	}
    // sắp xếp các object bên trái
    if (style_display_icon == "v style") {
        for (int i = 0; i < number_icon_left; i++) {
            left_objects[i]->setPosition(this->v_style_left(number_icon_left).at(i));
        }
    }
    else {
        for (int i = 0; i < number_icon_left; i++) {
            left_objects[i]->setPosition(this->straight_left(number_icon_left).at(i));
        }
    }
    // fix lai theo activity
    if (style_display_icon != "v style"){
        float length_two_objects = object_width*(number_icon_left+number_icon_right+1);
        float diem_bat_dau = quiz_size.width/2 - (length_two_objects/2);
        std::vector<Math_Object*> tam;
        for (Math_Object* obj : left_objects){
            tam.push_back(obj);
        }
        for (Math_Object* obj : lines){
            tam.push_back(obj);
        }
        for (int i = 0; i < tam.size(); i++){
            tam[i]->setPositionX(diem_bat_dau + i * object_width);
            tam[i]->setPositionY(450);
        }
        //math::func::setPositionList(tam, tam.size(), Rect(0.1,0.2,0.7,0.4));
        for (Math_Object* obj : lines){
            obj->setPositionX(obj->getPositionX()+100);
        }
    }
    //gen bieu thu ket qua
    Math_Object* _result1 = Math_Pool::getInstance()->getObject();
    this->gen_value_object(_result1, cocos2d::Size(70, 70), 50.f, "", cocos2d::Color3B::WHITE);
    _result1->setImage(image_number.at(0));
    _result1->setObjectSize(Size(100, 100));
    _result1->setImageVisible(false);
    _result1->setValueText(cocos2d::StringUtils::toString(number_icon_left));
    _result1->getLabelValue()->setVisible(true);
    _result1->getLabelValue()->setTextColor(Color4B::WHITE);
    _result1->getLabelValue()->disableEffect();
    _result1->getLabelValue()->setFontSize(70);
    result.push_back(_result1);
    Math_Object* _result2 = Math_Pool::getInstance()->getObject();
    this->gen_value_object(_result2, cocos2d::Size(70, 70), 50.f, "", cocos2d::Color3B::BLUE);
    _result2->setImage(image_plus);
    _result2->setBackgroundVisible(false);
    result.push_back(_result2);
    Math_Object* _result3 = Math_Pool::getInstance()->getObject();
    this->gen_value_object(_result3, cocos2d::Size(70, 70), 50.f, "", cocos2d::Color3B::WHITE);
    _result3->setImage(image_number.at(1));
    _result3->setObjectSize(Size(100, 100));
    _result3->setValueText(cocos2d::StringUtils::toString(number_icon_right));
    _result3->getLabelValue()->setVisible(true);
    _result3->getLabelValue()->setTextColor(Color4B::WHITE);
    _result3->getLabelValue()->setFontSize(70);
    _result3->getLabelValue()->disableEffect();
    _result3->setImageVisible(false);
    
    result.push_back(_result3);
    btn_done = Math_Pool::getInstance()->getAnswer();
    btn_done->setEnable("Done", math::common::answer, CC_CALLBACK_1(Nam_AddObjectAndAnswerQuiz::onAnswerDone1, this), this->getAnswerSize());
    btn_done->setPosition(cocos2d::Vec2(450, 55));
    cocos2d::ui::Button* b = btn_done->getChildByName<cocos2d::ui::Button*>("button");
    b->setTouchEnabled(true);
    _answers.push_back(btn_done);
    // gen box1, box2
    float delta; // khoang cach giua 2 object
    delta = (number_icon_left == 1) ? 0 : (left_objects[1]->getPositionX() - left_objects[0]->getPositionX() - object_width);
    box1 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/frame/square3.png"));
    box1->setScale9Enabled(true);
    box1->setContentSize(Size(object_size.width*number_icon_left*(number_icon_left == 1 ? 1.05 : 1.03) + (number_icon_left - 1) * delta, object_size.height*1.1));
    box1->setAnchorPoint(Vec2(1, 0.5));
    box1->setPosition(Vec2(left_objects.back()->getPositionX()+object_size.width*0.55,left_objects.back()->getPositionY()));
    _panel->addChild(box1);
    delta = (number_icon_right == 1) ? 0 : (lines[1]->getPositionX() - lines[0]->getPositionX() - object_width);
    box2 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/frame/square3.png"));
    box2->setScale9Enabled(true);
    box2->setContentSize(Size(object_size.width*number_icon_right*(number_icon_right == 1 ? 1.05 : 1.03)  + (number_icon_right - 1) * delta, object_size.height*1.1));
    box2->setAnchorPoint(Vec2(0, 0.5));
    box2->setPosition(Vec2(lines[0]->getPositionX()-object_size.width*0.55,lines[0]->getPositionY()));
    _panel->addChild(box2);
    line1 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/line/line_4.png"));
    line1->setScale9Enabled(true);
    line1->setContentSize(Size(10,80));
    _panel->addChild(line1);
    line2 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/line/line_4.png"));
    line2->setScale9Enabled(true);
    line2->setContentSize(Size(10,80));
    _panel->addChild(line2);
    line1->setOpacity(0);
    line2->setOpacity(0);
    box1->setOpacity(0);
    box2->setOpacity(0);
}
void Nam_AddObjectAndAnswerQuiz::onStart() {
	//add cac doi tuong len man hinh
	float delay = 0;
	delay += 0.3;
    box1->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
    box2->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(delay);
	}
	delay += 0.3;
	line1->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
    line2->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
	// show result
	//math::func::setPositionList(result, 10, cocos2d::Rect(0.1, 0.2, 0.8, 0.1));
    result[0]->setPositionX(box1->getPositionX() - box1->getContentSize().width/2);
    result[2]->setPositionX(box2->getPositionX() + box2->getContentSize().width/2);
    result[1]->setPositionX((result[0]->getPositionX()+result[2]->getPositionX())/2);
    //result[3]->setPositionX(result[2]->getPositionX()+150);
    for (Math_Object* obj : result){
        obj->setPositionY(300);
        this->addChildNode(obj,2);
        obj->onShow(delay);
    }
    line1->setPosition(Vec2(result[0]->getPositionX(),result[0]->getPositionY()+result[0]->getContentSize().height - 20));
    line2->setPosition(Vec2(result[2]->getPositionX(),result[2]->getPositionY()+result[2]->getContentSize().height - 20));
    
	// TODO: gen câu hỏi
    quizBegan(0, true);
    
}
void Nam_AddObjectAndAnswerQuiz::startQuestion() {

	// bắt đầu câu hỏi
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
		//result[0]->onShow(0);
		Math_Object* _result4 = Math_Pool::getInstance()->getObject();
		this->gen_value_object(_result4, cocos2d::Size(70, 70), 50.f, "", cocos2d::Color3B::BLUE);
		_result4->setImage(image_equal);
		_result4->setBackgroundVisible(false);
		result.push_back(_result4);
		Math_Object* _result5 = Math_Pool::getInstance()->getObject();
		this->gen_value_object(_result5, cocos2d::Size(70, 70), 50.f, "", cocos2d::Color3B::WHITE);
        _result5->setBackground(img_result_bg);
		result.push_back(_result5);
		_result5->setObjectSize(Size(100, 100));
        for (int i = 3; i < result.size(); i++) {
            result[i]->setPosition(Vec2(result[i-1]->getPositionX()+object_size.width,result[i-1]->getPositionY()));
            this->addChildNode(result[i], 2);
            result[i]->onShow(0);
        }
        for (int i = 0; i < result.size(); i++) {
            this->removeChild(result[i]);
        }
		////////////////////////////
		if (sequence_line)
		{
			// hien thi button done
			btn_done->onShowInParent(_panel_bot, 2);
			this->scheduleUpdate();
		}
		else {

		}
	}
	else {
		CCLOG("QUESTION 2");
        // fix hien cao tu dau
        for (auto obj : _objects){
            obj->setPositionY(obj->getPositionY()-100);
        }
        for (auto obj : result){
            obj->setPositionY(obj->getPositionY()-100);
        }
        box1->setPositionY(box1->getPositionY()-100);
        box2->setPositionY(box2->getPositionY()-100);
        line1->setPositionY(line1->getPositionY()-100);
        line2->setPositionY(line2->getPositionY()-100);
		onShowBottomBar();
		for (Math_Answer* btn : btn_result) {
			btn->onShowInParent(_panel_bot, 2);
		}
		for (Math_Answer* btn : btn_result) {
			cocos2d::ui::Button* b = btn->getChildByName<cocos2d::ui::Button*>("button");
			b->setTouchEnabled(true);
		}
		
	}
}

void Nam_AddObjectAndAnswerQuiz::onExplanation(bool correct) {
}
void Nam_AddObjectAndAnswerQuiz::onComplete() {
	//TODO: Thuc hien them cac lenh o day
	////Math_Pool::getInstance()->resetImages();
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
	//btn_next->onHide();
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Object* object : result) {
		object->onHide(m_delay);
		m_delay += 0.03;
	}
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.03;
	}
	for (Math_Object* object : right_objects) {
		object->onHide(m_delay);
		m_delay += 0.03;
	}
	for (Math_Answer* btn : btn_result) {
		btn->setDisable();
	}
    scheduleOnce([=](float dt) {
        box1->setVisible(false);
        box2->setVisible(false);
        line1->setVisible(false);
        line2->setVisible(false);
    }, m_delay, "");
	btn_done->setDisable();
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
void Nam_AddObjectAndAnswerQuiz::gen_value_object(Math_Object* object, cocos2d::Size object_size, float font_size, std::string text, cocos2d::Color3B color) {
	object->setEnable(0, math::common::touch, image_icon,img_bg_number);
	object->setObjectSize(object_size);
	//object->setBackgroundVisible(false);
	object->setImageVisible(false);
	object->setValueText(text);
	object->setLocked(true);
	cocos2d::ui::Text* t = object->getLabelValue();
	t->setFontSize(font_size);
	t->setColor(color);
	t->setVisible(true);
}

std::vector <Vec2> Nam_AddObjectAndAnswerQuiz::straight_left(int number) {
	std::vector <Vec2> vector;
	std::vector<Node*> nodes;
	for (int i = 0; i < number; i++) {
		Node* node = Node::create();
		node->retain();
		nodes.push_back(node);
	}
	if (divide_icon == false) {
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.48 - number*ratio_wild, 0.25, number*ratio_wild, 0.4));

	}
	else {
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.48 - number*ratio_wild, 0.25, number*ratio_wild, 0.4));
		for (int i = 0; i < number - number_of_line; i++) {
			nodes[i]->setPositionX(nodes[i + number_of_line]->getPositionX());
			nodes[i]->setPositionY(nodes[i + number_of_line]->getPositionY() + 100);
		}
	}
	for (int i = 0; i < number; i++) {
		vector.push_back(nodes.at(i)->getPosition());
	}
	for (int i = 0; i < number; i++) {
		vector[i].y+=20;
	}
	return vector;
}
std::vector <Vec2> Nam_AddObjectAndAnswerQuiz::straight_right(int number) {
	std::vector <Vec2> vector;
	std::vector<Node*> nodes;
	for (int i = 0; i < number; i++) {
		Node* node = Node::create();
		node->retain();
		nodes.push_back(node);
	}
	if (divide_icon == false) {
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.52, 0.25, number*ratio_wild, 0.4));

	}
	else {
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.52, 0.25, number*ratio_wild, 0.4));
		for (int i = number_of_line; i < number; i++) {
			nodes[i]->setPositionX(nodes[i%number_of_line]->getPositionX());
			nodes[i]->setPositionY(nodes[i%number_of_line]->getPositionY() + 100 * (int)(i / number_of_line));
		}
	}
	for (int i = 0; i < number; i++) {
		vector.push_back(nodes.at(i)->getPosition());
	}
	for (int i = 0; i < number; i++) {
		vector[i].y+=20;
	}
	return vector;
}
std::vector <Vec2> Nam_AddObjectAndAnswerQuiz::v_style_left(int number) {
	std::vector <Vec2> vector;
	std::vector<Node*> nodes;
	for (int i = 0; i < number; i++) {
		Node* node = Node::create();
		node->retain();
		nodes.push_back(node);
	}
	if (divide_icon == false) {
		float dtY = 80;
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.48 - number*ratio_wild, 0.25, number*ratio_wild, 0.4));
		for (int i = 0; i < number; i++) {
			nodes[i]->setPositionY(nodes[i]->getPositionY() + (number - i - 1) * 30 - dtY);
		}
	}
	else {
		float dtY = 80;
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.48 - number*ratio_wild, 0.25, number*ratio_wild, 0.4));
		for (int i = 0; i < number; i++) {
			nodes[i]->setPositionY(nodes[i]->getPositionY() + (number - i - 1) * 30 - dtY);
		}
		for (int i = 0; i < number - number_of_line; i++) {
			nodes[i]->setPositionX(nodes[i + number_of_line]->getPositionX());
			nodes[i]->setPositionY(nodes[i + number_of_line]->getPositionY() + 100);
		}

	}
	for (int i = 0; i < number; i++) {
		vector.push_back(nodes.at(i)->getPosition());
	}
	for (int i = 0; i < number; i++) {
		vector[i].y += 100;
	}
	return vector;
}
std::vector <Vec2> Nam_AddObjectAndAnswerQuiz::v_style_right(int number) {
	std::vector <Vec2> vector;
	std::vector<Node*> nodes;
	for (int i = 0; i < number; i++) {
		Node* node = Node::create();
		node->retain();
		nodes.push_back(node);
	}
	if (divide_icon == false) {
		float dtY = 80;
		//if (number_icon_left < 4 && number_icon_right < 4 && sequence_line == false) dtY = 0;
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.52, 0.25, number*ratio_wild, 0.4));
		for (int i = 0; i < number; i++) {
			nodes[i]->setPositionY(nodes[i]->getPositionY() + i * 30 - dtY);
		}
	}
	else {
		float dtY = 80;
		math::func::setPositionList(nodes, number, cocos2d::Rect(0.52, 0.25, number*ratio_wild, 0.4));
		for (int i = 0; i < number; i++) {
			nodes[i]->setPositionY(nodes[i]->getPositionY() + i * 30 - dtY);
		}
		for (int i = number_of_line; i < number; i++) {
			nodes[i]->setPositionX(nodes[i%number_of_line]->getPositionX());
			nodes[i]->setPositionY(nodes[i%number_of_line]->getPositionY() + 100.0*(int)(i / number_of_line));
		}
	}
	for (int i = 0; i < number; i++) {
		vector.push_back(nodes.at(i)->getPosition());
	}
	for (int i = 0; i < number; i++) {
		vector[i].y += 100;
	}
	return vector;
}

#define auto_testing_delay 2
void Nam_AddObjectAndAnswerQuiz::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", testing_step++));
}
void Nam_AddObjectAndAnswerQuiz::autoTestingRun(float delaytime) {
	int index_ques = this->getQuestion()->getData().index;
	if (index_ques == 0) {
		if (sequence_line == true) {
			testing_step++;
			if (testing_step < 3) {
				for (int i = 0; i < 20; i++)
				{
					this->runAction(Sequence::create(DelayTime::create(1.f*i), CallFunc::create([=] {
						if (i < 20) {
							int rand = RandomHelper::random_int(1, 10);
							if ((rand > 4 || countObject == 0) && countObject != max_total) {
								this->AddObject(line);
							}
							else {
								int idx = RandomHelper::random_int(0, countObject - 1);
								this->RemoveObject(right_objects[idx], idx);
							}
						}
						else this->AddObject(line);
					}), NULL));
				}
				this->runAction(Sequence::create(DelayTime::create(20.f), CallFunc::create([=] {
					this->onAnswerDone1(btn_done);
					this->autoTestingRun(delaytime);
				}), nullptr));
			}
			else
			{
				for (int i = 0; i < number_icon_right; i++)
				{
					this->runAction(Sequence::create(DelayTime::create(1.f*i), CallFunc::create([=] {
						this->AddObject(line);
					}), NULL));
				}
				this->runAction(Sequence::create(DelayTime::create(number_icon_right*1.f), CallFunc::create([=] {
					this->onAnswerDone1(btn_done);
					this->autoTestingRun(delaytime);
				}), nullptr));
			}
		}
		else {
			bool mang[100];
			for (int i = 0; i < number_icon_right; i++) mang[i] = false;
			int k;
			int i = 0;
			for (i = 0; i < 20 + number_icon_right; i++) {
				k = RandomHelper::random_int(0, number_icon_right - 1);
				if (i > 20)
					while (1) {
						k = RandomHelper::random_int(0, number_icon_right - 1);
						if (mang[k] == false) break;
					}
				if (mang[k]) mang[k] = false;
				else mang[k] = true;
				this->runAction(Sequence::create(DelayTime::create(1.f*i + 1.f), CallFunc::create([=] {
					this->onTouchObject2(lines[k], k);

				}), nullptr));
				bool kt = true;
				for (int j = 0; j < number_icon_right; j++) {
					if (mang[j] == false) {
						kt = false;
						break;
					}
				}
				if (kt) break;
			}
			this->runAction(Sequence::create(DelayTime::create(1.f*i + 1.f), CallFunc::create([=] {
				this->autoTestingRun(delaytime);
			}), nullptr));
		}
	}
	else {
		/// xu li question 2
		int i = 0;
		while (1) {
			int k = RandomHelper::random_int(0, max_total - 1);
			this->runAction(Sequence::create(DelayTime::create(1.f*i + 6.f), CallFunc::create([=] {
				this->onAnswerDone2(numbers_object[k], k);
			}), nullptr));
			if (k + 1 == total) break;
			i++;
		}
		this->runAction(Sequence::create(DelayTime::create(1.f*i + 11.f), CallFunc::create([=] {
			this->onAnswerDone1(btn_next);
		}), nullptr));
	}
	CCLOG("TEST testing");
}

/////////////////////////////////////////////////////////////////////////////
// các hàm set touch
void Nam_AddObjectAndAnswerQuiz::AddObject(cocos2d::Node* node) {
    if (this->getQuestion()->getData().index == 0) {
        if (countObject < max_total) {
            Math_Object* object = Math_Pool::getInstance()->getObject();
            object->setEnable(0, math::common::touch, image_icon, NULL);
            object->setObjectSize(object_size);
            
            if (style_display_icon != "v style")
            {
                object->setPosition(straight_right(countObject + 2).at(countObject));
                for (int i = 0; i < countObject; i++) {
                    right_objects[i]->runAction(MoveTo::create(0.2f, straight_right(countObject + 2).at(i)));
                }
                line->runAction(MoveTo::create(0.3f, straight_right(countObject + 2).at(countObject + 1)));
            }
            else {
                object->setPosition(v_style_right(countObject + 2).at(countObject));
                for (int i = 0; i < countObject; i++) {
                    right_objects[i]->runAction(MoveTo::create(0.2f, v_style_right(countObject + 2).at(i)));
                }
                line->runAction(MoveTo::create(0.3f, v_style_right(countObject + 2).at(countObject + 1)));
            }
            this->addChild(object);
            object->onShow(0);
            right_objects.push_back(object);
            countObject++;
            // truong hop click dup
            for (int i = 0; i < countObject; i++) {
                right_objects[i]->setLocked(true);
            }
            line->setLocked(true);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([&] {
                for (int i = 0; i < countObject; i++)
                    right_objects[i]->setLocked(false);
                line->setLocked(false);
            }), nullptr));
        }
    }
}

void Nam_AddObjectAndAnswerQuiz::RemoveObject(cocos2d::Node* node, int index) {
    if (this->getQuestion()->getData().index == 0) {
        // xóa object
        right_objects[index]->setDisable();
        this->removeChild(right_objects[index]);
        // không cho chạm liên tiếp
        for (int i = 0; i < countObject; i++)
            right_objects[i]->setLocked(true);
        line->setLocked(true);
        // di chuyển object
        int rows = (countObject - 1) / number_of_line;
        if (index >= (rows)*number_of_line) {
            for (int i = index; i < countObject - 1; i++) {
                right_objects[i] = right_objects[i + 1];
            }
            
        }
        else {
            int current_row = (index - 1) / number_of_line;
            
            for (int i = current_row; i < rows; i++) {
                if (index + number_of_line < countObject) {
                    index = index + number_of_line;
                    right_objects[index - number_of_line] = right_objects[index];
                }
                else {
                    right_objects[index] = right_objects[countObject - 1];
                    index = countObject - 1;
                }
            }
            for (int i = index; i < countObject - 1; i++) {
                right_objects[i] = right_objects[i + 1];
            }
        }
        countObject--;
        if (style_display_icon != "v style") {
            for (int i = 0; i < countObject; i++) {
                right_objects[i]->runAction(MoveTo::create(0.3f, straight_right(countObject + 1).at(i)));
            }
            line->runAction(MoveTo::create(0.3f, straight_right(countObject + 1).at(countObject)));
        }
        else {
            for (int i = 0; i < countObject; i++) {
                right_objects[i]->runAction(MoveTo::create(0.3f, v_style_right(countObject + 1).at(i)));
            }
            line->runAction(MoveTo::create(0.3f, v_style_right(countObject + 1).at(countObject)));
        }
        right_objects.pop_back();
        // mở khóa chạm
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([&] {
            for (int i = 0; i < countObject; i++)
                right_objects[i]->setLocked(false);
            line->setLocked(false);
        }), nullptr));
    }
}
void Nam_AddObjectAndAnswerQuiz::update(float dt) {
    for (int i = 0; i < countObject; i++) {
        right_objects[i]->setTouchCallback(CC_CALLBACK_1(Nam_AddObjectAndAnswerQuiz::RemoveObject, this, i));
    }
}

void Nam_AddObjectAndAnswerQuiz::onAnswerDone1(bool correct)
{
    if (this->getQuestion()->getData().index == 0) {
        if (countObject == number_icon_right) {
            CCLOG("WIN");
            btn_done->onAnimationClick(true);
            btn_done->setDisable();
            btn_done->disableClickAnswer();
            CCLOG("TOUCH");
            for (int i = 0; i < countObject; i++) {
                right_objects[i]->setLocked(true);
            }
            line->setLocked(true);
            this->nextQuestion();
            
        }
        else {
            btn_done->onAnimationClick(false);
            for (int i = 0; i < countObject; i++) {
                right_objects[i]->setDisable();
                this->removeChild(right_objects[i]);
                right_objects[i]->setLocked(true);
                if (style_display_icon == "straight") line->setPosition(straight_right(1).at(0));
                else line->setPosition(v_style_right(1).at(0));
            }
            while (!right_objects.empty()) {
                right_objects.pop_back();
            }
            countObject = 0;
        };
    }
    else {
        // TODO: fix lỗi addchild đã add
        
        btn_next->setDisable();
        btn_next->disableClickAnswer();
        this->nextQuestion();
        _answers.push_back(btn_next);
    }
}
void Nam_AddObjectAndAnswerQuiz::onAnswerDone2(cocos2d::Node* node, int index)
{
    Math_Answer* btn = (Math_Answer*)node;
    
    //numbers_object[index]->runAction(Sequence::create(ScaleTo::create(0.3f, 1.5f), DelayTime::create(0.2f), ScaleTo::create(0.3f, 1.f), nullptr));
    if (index == total) {
        btn->onAnimationClick(true);
        btn->disableClickAnswer();
        
        //////// chay hieu ung ket qua
        for (int i = 0; i < max_total; i++) {
            //numbers_object[i]->setLocked(true);
        }
        /*if (style_display_icon != "v style") math::func::setPositionList(result, 10, cocos2d::Rect(0.1, 0.1, 0.8, 0.1));
         else math::func::setPositionList(result, 10, cocos2d::Rect(0.1, 0.1, 0.8, 0.1));*/
        result.back()->setImage(image_number[2]);
        result.back()->setValueText(cocos2d::StringUtils::toString(total));
        result.back()->getLabelValue()->setVisible(true);
        result.back()->getLabelValue()->setTextColor(Color4B::WHITE);
        result.back()->getLabelValue()->setFontSize(70);
        result.back()->getLabelValue()->disableEffect();
        result.back()->setImageVisible(false);
        for (int i = 0; i < 5; i++) {
            result[i]->runAction(Sequence::create(DelayTime::create(i*1.f),
                                                  CallFunc::create([=] {
                
            })
                                                  , ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f),
                                                  nullptr));
        }
        // scale cac object khi doc ket qua
        for (int i = 0; i < left_objects.size(); i++) {
            left_objects[i]->runAction(Sequence::create(ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
        }
        for (int i = 0; i < number_icon_right; i++) {
            if (sequence_line == true) right_objects[i]->runAction(Sequence::create(DelayTime::create(2.f), ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
            else lines[i]->runAction(Sequence::create(DelayTime::create(2.f), ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
        }
        for (int i = 0; i < left_objects.size(); i++) {
            left_objects[i]->runAction(Sequence::create(DelayTime::create(4.f), ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
        }
        for (int i = 0; i < number_icon_right; i++) {
            if (sequence_line == true) right_objects[i]->runAction(Sequence::create(DelayTime::create(4.f), ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
            else lines[i]->runAction(Sequence::create(DelayTime::create(4.f), ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.f), nullptr));
        }
        this->runAction(Sequence::create(DelayTime::create(6.f), CallFunc::create([=] {
            this->nextQuestion();
        }), nullptr));
    }
    else {
        btn->onAnimationClick(false);
        CCLOG("FALSE");
    }
}
void Nam_AddObjectAndAnswerQuiz::onTouchObject2(cocos2d::Node* node, int index) {
    if (lines.at(index)->getImageSource()->path.compare(linePath))
        lines.at(index)->setImage(imgline);
    else
        lines.at(index)->setImage(image_icon);
    bool isNext = true;
    for (int i = 0; i < number_icon_right; i++) {
        if (lines.at(i)->getImageSource() != image_icon)
        {
            isNext = false;
            break;
        }
    }
    if (isNext) {
        for (int i = 0; i < lines.size(); i++) {
            lines.at(i)->setLocked(true);
        }
        this->nextQuestion();
    }
}
