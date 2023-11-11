//
//	Drag Arrow To Measure
//  MJQuiz
//
//  Created by Tunado on 20/9/17.
//
//

#include "Nam_DragArrowToMeasure.h"

Nam_DragArrowToMeasure* Nam_DragArrowToMeasure::createQuiz()
{
    Nam_DragArrowToMeasure* quiz=new Nam_DragArrowToMeasure();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_DragArrowToMeasure::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_DragArrowToMeasure::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_result = json_game.FindMember("result")->value;
	int minResult = json_result.FindMember("min")->value.GetInt();
	int maxResult = json_result.FindMember("max")->value.GetInt();
	intResult = RandomHelper::random_int(minResult, maxResult);
	rapidjson::Value& json_dragerSize = json_data.FindMember("drager")->value.FindMember("size")->value;
	dragerSize = Size(json_dragerSize.FindMember("width")->value.GetDouble(), json_dragerSize.FindMember("height")->value.GetDouble());
    mode_game = json_game.FindMember("mode_game")->value.GetString();
	//TODO: GET IMAGE
	characterImg = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()),"",image_prefix).front();
	characterImg->used = true;
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_1") {
			std::vector<math::resource::Text> texts;
			auto img_txt = characterImg->getTextValue("name_1", m_lang->getLanguage());
			//img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	//Math_Pool::getInstance()->resetImages();
	//FIX GAME CHUNG
	
	isEnd = false;
	//result = RandomHelper::random_real(200.f, 500.f);
	//TODO: GEN DRAGER
    math::resource::Image* originImg = math::resource::ResHelper::createImage(getUIPath("gameui/Nam_DragArrowToMeasure/origin.png"));
	originImg->used = true;
	math::resource::Image* dragerImg = math::resource::ResHelper::createImage(getUIPath("gameui/Nam_DragArrowToMeasure/drager.png"));
	dragerImg->used = true;
    math::resource::Image* lineYellowImg = math::resource::ResHelper::createImage(getUIPath((mode_game == "width") ? "gameui/Nam_DragArrowToMeasure/line_yellow.png" : "gameui/Nam_DragArrowToMeasure/line_yellow_height.png"));
	lineYellowImg->used = true;
    math::resource::Image* lineBrown =math::resource::ResHelper::createImage(getUIPath((mode_game == "width") ? "gameui/Nam_DragArrowToMeasure/line_brown.png" : "gameui/Nam_DragArrowToMeasure/line_brown_height.png"));
	lineBrown->used = true;
	lineYellow = Math_Pool::getInstance()->getObject();
	lineYellow->setEnable(0, math::common::TouchPermission::deny, lineYellowImg, NULL);
    float originX= (mode_game == "width") ? (quiz_size.width/2- 400) : (quiz_size.width/2+250);
    if (mode_game == "width"){
        // width
        lineYellow->setAnchorPoint(Vec2(0, 0.5));
        lineYellow->setObjectSize(Size(0, 13));
        lineYellow->setPosition(Vec2(originX,550));
    }
    else{
        // height
        lineYellow->setAnchorPoint(Vec2(0.5, 0));
        lineYellow->setObjectSize(Size(13, 0));
        lineYellow->setPosition(Vec2(originX,60));
    }
    this->addChildNode(lineYellow, 2);
	lineYellow->onShow(0);
	_objects.push_back(lineYellow);
	lineBrownOrigin = Math_Pool::getInstance()->getObject();
	lineBrownOrigin->setEnable(0, math::common::TouchPermission::deny, lineBrown, NULL);
    if (mode_game == "width"){
        // width
        lineBrownOrigin->setObjectSize(Size(2, 300));
        lineBrownOrigin->setAnchorPoint(Vec2(0.5, 1));
        lineBrownOrigin->setPosition(lineYellow->getPosition());
    }
    else{
        // height
        lineBrownOrigin->setObjectSize(Size(500, 2));
        lineBrownOrigin->setAnchorPoint(Vec2(1, 0.5));
        lineBrownOrigin->setPosition(lineYellow->getPosition());
    }
	this->addChildNode(lineBrownOrigin, 2);
	lineBrownOrigin->onShow(0);
	_objects.push_back(lineBrownOrigin);
	origin = Math_Pool::getInstance()->getObject();
	origin->setEnable(0, math::common::TouchPermission::deny, originImg, NULL);
	origin->setObjectSize(dragerSize);
    if (mode_game == "width"){
        // width
        origin->setPosition(lineYellow->getPosition());
    }
    else{
        // height
        origin->setPosition(lineYellow->getPosition());
    }
	this->addChildNode(origin, 2);
	origin->onShow(0);
	_objects.push_back(origin);
	lineBrownDrager = Math_Pool::getInstance()->getObject();
	lineBrownDrager->setEnable(0, math::common::TouchPermission::deny, lineBrown, NULL);
    if (mode_game == "width"){
        // width
        lineBrownDrager->setObjectSize(Size(2, 300));
        lineBrownDrager->setAnchorPoint(Vec2(0.5, 1));
        lineBrownDrager->setPosition(lineYellow->getPosition());
    }
    else{
        // height
        lineBrownDrager->setObjectSize(Size(500, 2));
        lineBrownDrager->setAnchorPoint(Vec2(1, 0.5));
        lineBrownDrager->setPosition(lineYellow->getPosition());
    }
	this->addChildNode(lineBrownDrager, 2);
	lineBrownDrager->onShow(0);
	_objects.push_back(lineBrownDrager);
	drager = Math_Pool::getInstance()->getObject();
	drager->setEnable(0, math::common::TouchPermission::deny, dragerImg, NULL);
	drager->setObjectSize(dragerSize);
    if (mode_game == "width"){
        // width
        drager->setPosition(lineYellow->getPosition());
    }
    else{
        // height
        drager->setPosition(lineYellow->getPosition());
    }
	this->addChildNode(drager, 2);
	drager->onShow(0);
	_objects.push_back(drager);
	hiddenDrager = Math_Pool::getInstance()->getObject();
	hiddenDrager->setEnable(0, math::common::TouchPermission::move, dragerImg, NULL);
    hiddenDrager->setObjectSize(dragerSize*1.5);
	hiddenDrager->getImage()->setOpacity(0);
	hiddenDrager->setMoveCallback(CC_CALLBACK_1(Nam_DragArrowToMeasure::onTouchedObject, this), CC_CALLBACK_2(Nam_DragArrowToMeasure::onMoveObjectEnded, this));
	hiddenDrager->setPosition(lineYellow->getPosition());
	this->addChildNode(hiddenDrager, 2);
	hiddenDrager->onShow(0);
	_objects.push_back(hiddenDrager);
	//TODO: GEN RULER
    auto rulerImg = math::resource::ResHelper::createImage(getUIPath((mode_game == "width") ? "gameui/Nam_DragArrowToMeasure/ruler.png" : "gameui/Nam_DragArrowToMeasure/ruler_height.png"));
	rulerImg->used = true;
    auto lineRedImg = math::resource::ResHelper::createImage(getUIPath((mode_game == "width") ? "gameui/Nam_DragArrowToMeasure/line_red.png" : "gameui/Nam_DragArrowToMeasure/line_red_height.png"));
	lineRedImg->used = true;
	rulerObject = Math_Pool::getInstance()->getObject();
	rulerObject->setEnable(0, math::common::TouchPermission::deny, rulerImg, NULL);
    if (mode_game == "width"){
        // width
        rulerObject->setObjectSize(Size(890, 100));
        rulerObject->setAnchorPoint(Vec2(0, 0.5));
        rulerObject->setPosition(Vec2(originX - 45, 220));
    }
    else{
        // height
        rulerObject->getImage()->setScaleY(-1);
        rulerObject->setObjectSize(Size(100, 700));
        rulerObject->setAnchorPoint(Vec2(0.5, 0));
        rulerObject->setPosition(Vec2(originX - 500 , 60- 39));
    }
	this->addChildNode(rulerObject, 2);
	rulerObject->onShow(0);
	_objects.push_back(rulerObject);
	lineRed = Math_Pool::getInstance()->getObject();
	lineRed->setEnable(0, math::common::TouchPermission::deny, lineRedImg, NULL);
    if (mode_game == "width"){
        lineRed->setObjectSize(Size(0, 30));
        lineRed->setAnchorPoint(Vec2(0, 0.5));
        lineRed->setPosition(Vec2(lineYellow->getPositionX(), rulerObject->getPositionY() - 13));
    }
    else{
        lineRed->setObjectSize(Size(30, 0));
        lineRed->setAnchorPoint(Vec2(0.5, 0));
        lineRed->setPosition(Vec2(rulerObject->getPositionX() - 13, lineYellow->getPositionY()));
    }
	this->addChildNode(lineRed, 2);
	lineRed->onShow(0);
	_objects.push_back(lineRed);
    result = (mode_game == "width") ? (rulerObject->getContentSize().width / 22)*intResult : (700 / 22)*intResult;
	//TODO: Gen Character
	characterObj = Math_Pool::getInstance()->getObject();
	characterObj->setEnable(0, math::common::TouchPermission::deny, characterImg, NULL);
    if (mode_game == "width"){
        // width
        characterObj->setObjectSize(Size(result, 200));
        characterObj->setAnchorPoint(Vec2(0, 0.5));
        characterObj->setPosition(Vec2(lineYellow->getPositionX(), 400));
    }
    else{
        // height
        characterObj->setObjectSize(Size(200, result));
        characterObj->setAnchorPoint(Vec2(0.5, 0));
        characterObj->setPosition(Vec2(quiz_size.width/2, lineYellow->getPositionY()));
    }
	this->addChildNode(characterObj, 2);
	characterObj->onShow(0);
	_objects.push_back(characterObj);
}

void Nam_DragArrowToMeasure::onStart() {
	//Gen question
    showQuestionBox();
//    (mode_game == "width") ? showQuestionBox() : showQuestionBox(0, math::animation::No_Effect);
}

void Nam_DragArrowToMeasure::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
		this->scheduleUpdate();	
	}
}
void Nam_DragArrowToMeasure::update(float dt) {
	CCLOG("test");
	if (isMoving && !isEnd) {
        if (mode_game == "width"){
            // width
            Vec2 pos = hiddenDrager->getPosition();
            if (pos.x >= origin->getPositionX()) {
                float updateX = pos.x;
                if (pos.x - origin->getPositionX() > result) updateX = result+origin->getPositionX();
                drager->setPositionX(updateX);
                lineBrownDrager->setPositionX(updateX);
                lineYellow->setObjectSize(Size(drager->getPositionX() - origin->getPositionX(), 13));
                lineRed->setObjectSize(Size(drager->getPositionX() - origin->getPositionX(), 30));
                if ((pos.x - origin->getPositionX()) >= result) {
                    isEnd = true;
                    //scheduleOnce([=](float dt) {
                        this->nextQuestion();
                    //}, 0.5f, "");
                }
            }
        }
        else{
            // height
            Vec2 pos = hiddenDrager->getPosition();
            if (pos.y >= origin->getPositionY()) {
                float updateY = pos.y;
                if (pos.y - origin->getPositionY() > result) updateY = result+origin->getPositionY();
                drager->setPositionY(updateY);
                lineBrownDrager->setPositionY(updateY);
                lineYellow->setObjectSize(Size(13,drager->getPositionY() - origin->getPositionY()));
                lineRed->setObjectSize(Size(30, drager->getPositionY() - origin->getPositionY()));
                if ((pos.y - origin->getPositionY()) >= result) {
                    isEnd = true;
                    //scheduleOnce([=](float dt) {
                    this->nextQuestion();
                    //}, 0.5f, "");
                }
            }
        }
	}
}
void Nam_DragArrowToMeasure::onAnswerDone(Node* node) {
	this->nextQuestion();
}
void Nam_DragArrowToMeasure::onExplanation(bool correct) {}
void Nam_DragArrowToMeasure::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
    m_delay += 0.5;
	for (Math_Object* object : _objects) {
		this->fixAnchorPoint(object);
        object->onHide(m_delay,math::animation::SlideUp);
		m_delay += 0.015;
	}
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay+0.5f, "complete_quiz");

}

#pragma mark - object touch events callback
void Nam_DragArrowToMeasure::onTouchedObject(Node* node) {
	Math_Object* obj = (Math_Object*)node;
	obj->setLocked(true);
	isMoving = true;
}
void Nam_DragArrowToMeasure::onMoveObjectEnded(Node* node, Vec2 point) {
	hiddenDrager->runAction(MoveTo::create(0.3f,origin->getPosition()));
	Math_Object* obj = (Math_Object*)node;
	this->scheduleOnce([=](float dt) {
		isMoving = false;
		obj->setLocked(false);
	}, 0.31f, cocos2d::StringUtils::format("a"));
}
void Nam_DragArrowToMeasure::fixAnchorPoint(Math_Object* obj) {
	float deltaX = 0.5 - obj->getAnchorPoint().x;
	float deltaY = 0.5 - obj->getAnchorPoint().y;
	obj->setAnchorPoint(Vec2(0.5, 0.5));
	obj->setPosition(Vec2(deltaX*obj->getContentSize().width + obj->getPositionX(), deltaY*obj->getContentSize().height + obj->getPositionY()));
}



#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_DragArrowToMeasure::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_DragArrowToMeasure::autoTestingRun(float delaytime)
{
   
}

