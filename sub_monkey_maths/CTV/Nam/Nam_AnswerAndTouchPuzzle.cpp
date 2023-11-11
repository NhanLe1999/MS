//
//	Answer And Touch Puzzle
//  MJQuiz
//
//  Created by Tunado on 25/9/2017
//
//
#include "Nam_AnswerAndTouchPuzzle.h"
Nam_AnswerAndTouchPuzzle* Nam_AnswerAndTouchPuzzle::createQuiz()
{
    Nam_AnswerAndTouchPuzzle* quiz=new Nam_AnswerAndTouchPuzzle();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_AnswerAndTouchPuzzle::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_AnswerAndTouchPuzzle::initQuiz(mj::model::Game game)
{
	colorVec = { Color4B::BLACK, Color4B::GREEN, Color4B::YELLOW, Color4B::BLUE, Color4B::GRAY, Color4B::MAGENTA, Color4B::ORANGE,
		Color4B::RED };
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());


	rapidjson::Value& json_data = document.FindMember("data")->value;
	
	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_result = json_game.FindMember("Caculate")->value.FindMember("result")->value;

	minResult = json_result.FindMember("min")->value.GetInt();
	maxResult = json_result.FindMember("max")->value.GetInt();
	rapidjson::Value& json_size_puzzle= json_data.FindMember("object")->value.FindMember("size")->value;
	puzzleSize = Size(json_size_puzzle.FindMember("width")->value.GetDouble(), json_size_puzzle.FindMember("height")->value.GetDouble());
	//TODO: get IMAGE
    auto allImage =getImageList("icons");
    math::resource::Image* image_model;
    int pair = 0;
    for (auto img : allImage){
        if (img->getComparisionValue("pair") == pair){
            if (img->getComparisionValue("role") == 0){
                image_model = img;
            }
            else{
                puzzleImg.push_back(img);
            }
            img->used = true;
        }
    }
    std::sort(puzzleImg.begin(), puzzleImg.end(), [=](math::resource::Image* a,math::resource::Image* b)->bool{
        return a->getComparisionValue("role") < b->getComparisionValue("role");
    });
    /*---- PARSE DATA -----*/
     std::vector<Dot> points;
    rapidjson::Document doc_tracing;
    doc_tracing.Parse<rapidjson::kParseStopWhenDoneFlag>(image_model->tracing.c_str());
    rapidjson::Value& trace_positions = getJsonMember(doc_tracing, "position");
    for (rapidjson::SizeType i = 0; i<trace_positions.Size(); i++) {
        rapidjson::Value& json_point = trace_positions[i];
        Dot point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
    std::sort(points.begin(), points.end(), [=](Dot a, Dot b)->bool{
        return a.index < b.index;
    });
	/*std::string strPuzzle = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	rapidjson::Document documentPuzzle;
	documentPuzzle.Parse<0>(strPuzzle.c_str());
	rapidjson::Value& json_puzzle = documentPuzzle.FindMember("icons")->value;
	
	for (int i = 0; i < json_puzzle.Size(); i++) {
		rapidjson::Value& j = json_puzzle[i].FindMember("positionOfNumber")->value;
		numberPosOfPuzzle.push_back(Vec2(j.FindMember("posx")->value.GetDouble(), j.FindMember("posy")->value.GetDouble()));
	}*/
	numberPuzzle = puzzleImg.size();
	////Gen phep tinh
	for (int i = 0; i < numberPuzzle; i++) {
		int result = RandomHelper::random_int(minResult, maxResult);
		int a, b;
		std::string key;
		if (RandomHelper::random_int(0, 1)) {
			a = RandomHelper::random_int(0, result);
			b = result - a;
			key = "+";
		}
		else {
			a = RandomHelper::random_int(result, maxResult);
			b = a - result;
			key = "-";
		}
		Calculation c;
		c.number1 = a;
		c.number2 = b;
		c.result = result;
		c.key = key;
		vecCaculation.push_back(c);
		vecResults.push_back(result);
	};
	int soCauHoi = 0;
	for (int i = 0; i < vecCaculation.size(); i++) {
		soCauHoi++;
		for (int j = 0; j < i; j++) {
			if (vecCaculation[i].result == vecCaculation[j].result) {
				soCauHoi--;
				break;
			}
		}
	}
	this->parseQuestion();
	
	std::random_shuffle(vecResults.begin(), vecResults.end());
	for (int i = 0; i < vecCaculation.size(); i++) {
		bool kt = true;
		for (int j = 0; j < i; j++) {
			if (vecCaculation[i].result == vecCaculation[j].result) {
				kt = false;
				break;
			}
		}
		if (kt) {
			vecStrPhepTinh.push_back(cocos2d::StringUtils::toString(vecCaculation[i].number1) + " " + vecCaculation[i].key + " " + cocos2d::StringUtils::toString(vecCaculation[i].number2) + " = ?");
		}
	}
	
	indexOfCurrentCaculation = 0;
	//lay mau
	for (int i = 8; i <= maxResult + 1; i++) {
		Color4B tmp;
		while (true) {
			bool kt = true;
			tmp = Color4B(RandomHelper::random_int(0, 255), RandomHelper::random_int(0, 255), RandomHelper::random_int(0, 255), 255);
			for (int j = 0; j < colorVec.size(); j++) {
				if (colorVec[j] == tmp) {
					kt == false;
					break;
				}
			}
			if (kt) break;
		}
		colorVec.push_back(tmp);
	}
	isUpdating = true;
	//Gen Puzzle
	std::vector <std::string> paths_image_result;
	paths_image_result.push_back(m_theme->getThemePath() + "gameui/Nam_AnswerAndTouchPuzzle/" + "cat.jpg");
	paths_image_result.push_back(m_theme->getThemePath() + "gameui/Nam_AnswerAndTouchPuzzle/" + "dog.jpg");
	paths_image_result.push_back(m_theme->getThemePath() + "gameui/Nam_AnswerAndTouchPuzzle/" + "dog2.jpg");
	float delay = 0;
	resultObjectOfPuzzle = cocos2d::ui::ImageView::create(paths_image_result[RandomHelper::random_int(0, (int)paths_image_result.size() - 1)]);
	resultObjectOfPuzzle->setScale9Enabled(true);
	resultObjectOfPuzzle->setCapInsets(Rect(1, 1, resultObjectOfPuzzle->getVirtualRendererSize().width - 2, resultObjectOfPuzzle->getVirtualRendererSize().height - 2));
	resultObjectOfPuzzle->setContentSize(puzzleSize);
	resultObjectOfPuzzle->setPosition(Vec2(quiz_size.width/2,quiz_size.height*0.4));
	resultObjectOfPuzzle->setVisible(false);
	//resultObjectOfPuzzle = Math_Pool::getInstance()->getObject();
	this->addChildNode(resultObjectOfPuzzle, 2);
	for (int i = 0; i < puzzleImg.size(); i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		obj->setEnable(i, math::common::TouchPermission::touch, puzzleImg[i], NULL);
		obj->setObjectSize(puzzleSize);
		obj->setTouchDetectedType(math::common::DetectedType::nontrans);
		obj->setTouchCallback(CC_CALLBACK_1(Nam_AnswerAndTouchPuzzle::onTouchedObject, this, i));
		obj->setName("0");
		obj->setPosition(Vec2(quiz_size.width/2,quiz_size.height*0.4));
		obj->setVisible(true);
		//obj->getImage()->setOpacity(255);
		//obj->getLabelValue()->setOpacity(255);
		this->addChildNode(obj, 2);
		obj->onShow(0);
		puzzleObject.push_back(obj);
	}
	for (int i = 0; i < numberPuzzle; i++) {
		this->setTextPuzzle(puzzleObject[i], vecResults[i], points[i]);
	}
	// outframe
	objOutFrame = cocos2d::ui::ImageView::create(m_theme->getThemePath() + "gameui/Nam_AnswerAndTouchPuzzle/" + "picture_outframe.png");
	objOutFrame->setScale9Enabled(true);
	objOutFrame->setCapInsets(Rect(1, 1, objOutFrame->getVirtualRendererSize().width - 2, objOutFrame->getVirtualRendererSize().height - 2));
	objOutFrame->setContentSize(puzzleSize*1.05);
	objOutFrame->setPosition(Vec2(quiz_size.width/2,quiz_size.height*0.4));
	this->addChildNode(objOutFrame, 2);
	objOutFrame->setScale(0);
	objOutFrame->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1))));
	bangPhepTinh = Math_Pool::getInstance()->getObject();
	bangPhepTinh->setEnable(0, math::common::TouchPermission::deny, puzzleImg[0], NULL);
	bangPhepTinh->setObjectSize(Size(300, 100));
	bangPhepTinh->setImageVisible(false);
	keyBangPhepTinh = 0;
	bangPhepTinh->setValueText(vecStrPhepTinh[keyBangPhepTinh]);
    auto T = bangPhepTinh->getLabelValue();
	T->setVisible(true);
    T->setTextColor(Color4B::BLACK);
    T->disableEffect();
	bangPhepTinh->getLabelValue()->setFontSize(70.f);
	bangPhepTinh->setPosition(Vec2(quiz_size.width/2, quiz_size.height*0.85));
	
	//Math_Pool::getInstance()->resetImages();
}

void Nam_AnswerAndTouchPuzzle::onStart() {
	
	//
	//Gen question
	
    showQuestionBox();
	
}

void Nam_AnswerAndTouchPuzzle::startQuestion() {
	scheduleOnce([=](float dt) {
		//this->hideQuestionInQuiz();
	},1, "hide_box");
	this->addChildNode(bangPhepTinh,2);
	bangPhepTinh->onShow(1.5);
	if (this->getQuestion()->getData().index == 0) {
		this->enableControlQuiz();
		resultObjectOfPuzzle->setVisible(true);
		objOutFrame->setVisible(true);
		this->scheduleUpdate();
	}
}

void Nam_AnswerAndTouchPuzzle::update(float dt) {
	if (isUpdating) {
		bool isNext = true, isEndGame = true;
		for (int i = 0; i < puzzleObject.size(); i++) {
			if (vecResults[i] == vecCaculation[indexOfCurrentCaculation].result) {
				if (puzzleObject[i]->getName()!="69") {
					isNext = false;
				}
			}
		}
		for (int i = 0; i < puzzleObject.size(); i++) {
			if (puzzleObject[i]->getName() != "69") {
				isEndGame = false;
			}
		}
		if (isEndGame) {
			this->nextQuestion();
			isUpdating = false;
		}
		else {
			if (isNext) {
				while (true) {
					bool kt = true;
					indexOfCurrentCaculation++;
					if (indexOfCurrentCaculation < numberPuzzle) {
						for (int i = 0; i < indexOfCurrentCaculation; i++) {
							if (vecCaculation[i].result == vecCaculation[indexOfCurrentCaculation].result) {
								kt = false;
								break;
							}
						}
					}
					if (kt) break;
				}
				this->nextStep();
			}
		}
	}
}

void Nam_AnswerAndTouchPuzzle::setTextPuzzle(Math_Object* obj,int number, Dot dot) {
    dot.x = dot.x/1000;
    dot.y = dot.y/1000;
	obj->setValueText(cocos2d::StringUtils::toString(number));
	auto label = obj->getLabelValue();
	label->setVisible(true);
	label->setFontSize(55.f);
	label->setTextColor(colorVec.at(number));
	label->setPosition(Vec2(dot.x*puzzleSize.width,dot.y*puzzleSize.height));
	
}
void Nam_AnswerAndTouchPuzzle::onExplanation(bool correct) {}
void Nam_AnswerAndTouchPuzzle::nextStep() {
	keyBangPhepTinh++;
	bangPhepTinh->setValueText(vecStrPhepTinh[keyBangPhepTinh]);
}
void Nam_AnswerAndTouchPuzzle::onComplete() {
	this->unscheduleUpdate();
    //TODO: Thuc hien them cac lenh o day
    //Math_Pool::getInstance()->resetAnswers();
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
    
	m_delay += 0.3;
    bangPhepTinh->onHide(0.3);
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
	}
	for (Math_Object* object : puzzleObject) {
		object->onHide(0.3);
	}
	objOutFrame->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay-0.1),
		cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)),
		cocos2d::CallFunc::create([=]() {
		objOutFrame->setVisible(false);
	}), NULL));
	resultObjectOfPuzzle->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay-0.1),
		cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)),
		cocos2d::CallFunc::create([=]() {
		resultObjectOfPuzzle->setVisible(false);
	}), NULL));
	//resultObjectOfPuzzle->onHide(m_delay);
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void Nam_AnswerAndTouchPuzzle::onTouchedObject(Node* node, int index) {
	CCLOG("TEST touched");
	Math_Object* object = (Math_Object*)node;
	if (vecResults[index] == vecCaculation[indexOfCurrentCaculation].result) {
		object->getImage()->runAction(FadeOut::create(0.3f));
		object->getLabelValue()->runAction(FadeOut::create(0.3f));
		object->setLocked(true);
		object->setName("69");
	}
	else {
		object->getImage()->runAction(Sequence::create(TintTo::create(0.2f, Color3B::RED), TintTo::create(0.2f, Color3B::WHITE),NULL));
		object->setName("0");
	}
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_AnswerAndTouchPuzzle::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Nam_AnswerAndTouchPuzzle::autoTestingRun(float delaytime)
{
	
}

