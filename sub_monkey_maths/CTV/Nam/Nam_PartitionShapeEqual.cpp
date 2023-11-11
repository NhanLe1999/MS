//
//  Nam_PartitionShapeEqual
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_PartitionShapeEqual.h"

Nam_PartitionShapeEqual* Nam_PartitionShapeEqual::createQuiz()
{
    Nam_PartitionShapeEqual* quiz=new Nam_PartitionShapeEqual();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_PartitionShapeEqual::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_PartitionShapeEqual::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	game_mode = json_game.FindMember("mode")->value.GetString();
	if (game_mode == "halves_fourths") {
		if (RandomHelper::random_int(0, 1)) {
			str_result = "halves";
		}
		else {
			str_result = "fourths";
		}
	}
	else {
		int k = RandomHelper::random_int(0, 2);
		if (k == 0) {
			str_result = "halves";
		}
		else {
			if (k == 1) {
				str_result = "fourths";
			}
			else {
				str_result = "thirds";
			}
		}
	}
    auto allImage =math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()),"",image_prefix);
    math::func::shuffle(allImage);
    img_result = allImage.front();
	//GET QUESTION
	this->parseQuestion();
	int key = 0;
	if (str_result == "halves") {
		this->getQuestion()->setKeywordValue(0);
		key = 2;
	}
    if (str_result == "thirds") {
        this->getQuestion()->setKeywordValue(1);
        key = 3;
    }
	if (str_result == "fourths") {  
		this->getQuestion()->setKeywordValue(2);
		key = 4;
	}
	std::vector <int> randomAnswers = { 2,3,4 };
	int_result = RandomHelper::random_int(0, 3);
	//randomAnswers.push_back(int_result);
	std::random_shuffle(randomAnswers.begin(), randomAnswers.end());
	// GEN
	int j = 0;
	for (int i = 0; i < 4; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		if (i == int_result) {
			obj->setEnable(i, math::common::TouchPermission::touch, img_result, NULL);obj->setEnable(i, math::common::TouchPermission::touch, img_result, NULL);
            obj->setObjectSize(Size(250, 250));
			this->genDrawNode(obj, key, true);
		}
		else {
			obj->setEnable(i, math::common::TouchPermission::touch, img_result, NULL); obj->setEnable(i, math::common::TouchPermission::touch, img_result, NULL);
            obj->setObjectSize(Size(250, 250));
			if (randomAnswers[j] == key) this->genDrawNode(obj, key, false);
			else this->genDrawNode(obj, randomAnswers[j], true);
			j++;
		}
		obj->setTouchCallback(CC_CALLBACK_1(Nam_PartitionShapeEqual::onTouchedObject, this));
		_objects.push_back(obj);
	}
	math::func::setPositionList(_objects, 2, Rect(0.15, 0.05, 0.7, 0.75));
	//Math_Pool::getInstance()->resetImages();
}
void Nam_PartitionShapeEqual::genDrawNode(Math_Object* obj, int intType, bool isTrue) {
	auto draw = obj->getDrawer();
	draw = DrawNode::create();
	if (obj->getImageSource()->getText("name_1") == "square") {
		// hinh vuong
        draw->drawSegment(Point(0, 0), Point(obj->getContentSize().width, 0), 4, Color4F::BLACK);
        draw->drawSegment(Point(0, 0), Point(0, obj->getContentSize().height), 4, Color4F::BLACK);
        draw->drawSegment(Point(obj->getContentSize().width, obj->getContentSize().height), Point(0, obj->getContentSize().height), 4, Color4F::BLACK);
        draw->drawSegment(Point(obj->getContentSize().width, obj->getContentSize().height), Point(obj->getContentSize().width, 0), 4, Color4F::BLACK);
//        draw->drawRect(Vec2(0,0), Vec2(obj->getContentSize().width,obj->getContentSize().height), Color4F::BLACK);
//        draw->setLineWidth(6.f);
		if (isTrue) {
			for (int i = 1; i < intType; i++) {
				draw->drawSegment(Point((obj->getContentSize().width / intType)*i, 0), Point((obj->getContentSize().width / intType)*i, obj->getContentSize().height), 4, Color4F::BLACK);
			}
		}
		else {
			if (intType == 2) {
				draw->drawSegment(Point((obj->getContentSize().width / 3), 0), Point((obj->getContentSize().width / 3), obj->getContentSize().height), 4, Color4F::BLACK);
			}
			if (intType == 3) {
				draw->drawSegment(Point((obj->getContentSize().width / 2), 0), Point((obj->getContentSize().width / 2), obj->getContentSize().height), 4, Color4F::BLACK);
				draw->drawSegment(obj->convertToNodeSpace(obj->getPosition()), Point(obj->getContentSize().width, obj->getContentSize().height / 2), 4, Color4F::BLACK);
			}
			if (intType == 4) {
				draw->drawSegment(Point((obj->getContentSize().width / 3), 0), Point((obj->getContentSize().width / 3), obj->getContentSize().height), 4, Color4F::BLACK);
				draw->drawSegment(Point(5, obj->getContentSize().height / 2), Point(obj->getContentSize().width, obj->getContentSize().height / 2), 4, Color4F::BLACK);
			}
		}
	}
	else {
        if (obj->getImageSource()->getText("name_1") == "circle"){
            // hinh tron
            float r = obj->getContentSize().height / 2;
            Vec2 tam = obj->convertToNodeSpace(obj->getPosition());
            // ve hinh trong vie
            draw->drawCircle(tam, r, 0, 200000, false, 1, 1, Color4F::BLACK);
            draw->setLineWidth(6.f);
            /*DrawPrimitives::setDrawColor4B(0, 0, 0, 255);
            DrawPrimitives::setPointSize(10);
            DrawPrimitives::drawCircle(tam, r, 0, 100, false);*/
            if (isTrue) {
                if (intType == 2) {
                    draw->drawSegment(Point((obj->getContentSize().width / intType), 0), Point((obj->getContentSize().width / intType), obj->getContentSize().height), 4, Color4F::BLACK);
                }
                if (intType == 3) {
                    //auto drawCircle = DrawNode::create();
                    //drawCircle->drawCircle(tam, r, 360, 5000, false, Color4F::BLUE);
                    //obj->addChild(drawCircle);
                    draw->drawSegment(tam, Point((obj->getContentSize().width / 2), obj->getContentSize().height), 4, Color4F::BLACK);
                    draw->drawSegment(tam, Point(tam.x - (sqrt(3) / 2)*r, 0.5*r), 4, Color4F::BLACK);
                    draw->drawSegment(tam, Point((tam.x - (sqrt(3) / 2)*r) + obj->getContentSize().width - 2 * (tam.x - (sqrt(3) / 2)*r), 0.5*r), 4, Color4F::BLACK);
                }
                if (intType == 4) {
                    draw->drawSegment(Point((obj->getContentSize().width / 2), 0), Point((obj->getContentSize().width / 2), obj->getContentSize().height), 4, Color4F::BLACK);
                    draw->drawSegment(Point(0, obj->getContentSize().height / 2), Point(obj->getContentSize().width, obj->getContentSize().height / 2), 4, Color4F::BLACK);
                }
            }
            else {
                if (intType == 2) {
                    draw->drawSegment(tam, Point((obj->getContentSize().width / 2), obj->getContentSize().height), 4, Color4F::BLACK);
                    draw->drawSegment(tam, Point((tam.x - (sqrt(3) / 2)*r) + obj->getContentSize().width - 2 * (tam.x - (sqrt(3) / 2)*r), 0.5*r), 4, Color4F::BLACK);
                }
                if (intType == 3) {
                    draw->drawSegment(tam, Point((obj->getContentSize().width / 2), obj->getContentSize().height), 4, Color4F::BLACK);
                    draw->drawSegment(Point(0, obj->getContentSize().height / 2), Point(obj->getContentSize().width, obj->getContentSize().height / 2), 4, Color4F::BLACK);
                }
                if (intType == 4) {
                    draw->drawSegment(Point(0, obj->getContentSize().height / 2), Point(obj->getContentSize().width, obj->getContentSize().height / 2), 4, Color4F::BLACK);
                    draw->drawSegment(tam, Point(tam.x - (sqrt(3) / 2)*r, 0.5*r), 4, Color4F::BLACK);
                    draw->drawSegment(tam, Point((tam.x - (sqrt(3) / 2)*r) + obj->getContentSize().width - 2 * (tam.x - (sqrt(3) / 2)*r), 0.5*r), 4, Color4F::BLACK);
                }
            }
        }
        else{
            if (obj->getImageSource()->getText("name_1") == "triangle"){
                // hinh tam giac
                float r = obj->getContentSize().height / 2;
                Vec2 tam = obj->convertToNodeSpace(obj->getPosition());
                draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point((obj->getContentSize().width),0), 2, Color4F::BLACK);
                draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point(0,0), 2, Color4F::BLACK);
                draw->drawSegment(Point(0,0), Point(obj->getContentSize().width,0), 2, Color4F::BLACK);
                if (isTrue){
                    if (intType == 2) {
                        draw->drawSegment(Point((obj->getContentSize().width / intType), 0), Point((obj->getContentSize().width / intType), obj->getContentSize().height), 2, Color4F::BLACK);
                    }
                    if (intType == 3) {
                        draw->drawSegment(tam, Point((obj->getContentSize().width / 2), obj->getContentSize().height), 2, Color4F::BLACK);
                        draw->drawSegment(tam, Point(0,0), 2, Color4F::BLACK);
                        draw->drawSegment(tam, Point((obj->getContentSize().width), 0), 2, Color4F::BLACK);
                    }
                    if (intType == 4) {
                        draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point((obj->getContentSize().width*0.25),0), 2, Color4F::BLACK);
                        draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point((obj->getContentSize().width*0.5),0), 2, Color4F::BLACK);
                        draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point((obj->getContentSize().width*0.75),0), 2, Color4F::BLACK);
                        
                    }
                }
                else{
                    draw->drawSegment(Point((obj->getContentSize().width / 2), obj->getContentSize().height), Point((obj->getContentSize().width*0.25),0), 2, Color4F::BLACK);
                }
            }
        }
	}
	obj->addChild(draw);
}
void Nam_PartitionShapeEqual::onStart() {
	math_statistic->setCurrentGame(_game);
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	for (Math_Answer* ans : _answers) {
		ans->onShowInParent(_panel, 1);

	}
    showQuestionBox();
}

void Nam_PartitionShapeEqual::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_PartitionShapeEqual::onExplanation(bool correct) {}
void Nam_PartitionShapeEqual::onComplete() {
	
    //TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.02;
		
	}
    scheduleOnce([=](float dt) {
		for (auto d : draws) {
			d->removeFromParent();
		}
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback

void Nam_PartitionShapeEqual::onAnswerDone(Node* node, std::string str_ans) {
	Math_Answer* btn = (Math_Answer*)node;
	this->nextQuestion();
	if (str_ans == str_result) {
    btn->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		btn->onAnimationClick(false);
	}
}
void Nam_PartitionShapeEqual::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
	if ((object->getID() == int_result)) {
		object->getImage()->runAction(Sequence::create(TintTo::create(0.25, Color3B::GREEN), TintTo::create(0.25, Color3B::WHITE), TintTo::create(0.25, Color3B::GREEN), TintTo::create(0.25, Color3B::WHITE), nullptr));
		this->nextQuestion();
		for (Math_Object* obj : _objects) {
			obj->setLocked(true);
		}
	}
	else {
		object->getImage()->runAction(Sequence::create(TintTo::create(0.25, Color3B::BLACK), TintTo::create(0.25, Color3B::WHITE),nullptr));
	}

}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_PartitionShapeEqual::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_PartitionShapeEqual::autoTestingRun(float delaytime)
{
   
}

