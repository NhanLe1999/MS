//
//  Nam_SortAndDisplayResult
//  MJQuiz
//
//  Created by Tunado on 18/9/17.
//
//

#include "Nam_SortAndDisplayResult.h"

Nam_SortAndDisplayResult* Nam_SortAndDisplayResult::createQuiz()
{
    Nam_SortAndDisplayResult* quiz=new Nam_SortAndDisplayResult();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_SortAndDisplayResult::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_SortAndDisplayResult::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	
	//TODO: object config
	numberOfObjectType = json_game.FindMember("objectTypeNumber")->value.GetInt();
	maxRow = json_game.FindMember("heightOfGraph")->value.GetInt();
	int maxResult = json_game.FindMember("resultNumber")->value.FindMember("max")->value.GetInt();
	int minResult = json_game.FindMember("resultNumber")->value.FindMember("min")->value.GetInt();
	for (int i = 0; i < numberOfObjectType; i++) {
		objectsNumber.push_back(RandomHelper::random_int(minResult, maxResult));
	}
	// 
	rapidjson::Value& json_objSize = json_data.FindMember("object")->value.FindMember("size")->value;
	//get image
	boxObject = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	boxObject->path = m_theme->getThemePath()+"gameui/Nam_SortAndDisplayResult/"+"box_object.png";
	boxObject->used = true;
	boxBlue = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	boxBlue->path = m_theme->getThemePath() + "gameui/Nam_SortAndDisplayResult/"+"box1.png";
	boxBlue->used = true;
	boxYellow = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	boxYellow->path = m_theme->getThemePath() + "gameui/Nam_SortAndDisplayResult/"+"box2.png";
	boxYellow->used = true;
	//auto bg_list = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sbackground.json", _game_path.c_str()), "box");
	imgSlot = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/drag.png");
	imgSlot->used = true;
	objectSize = Size(json_objSize.FindMember("width")->value.GetDouble(), json_objSize.FindMember("height")->value.GetDouble());
    auto allImageObject = getImageList("icons",numberOfObjectType,"name_1");
    for (auto img : allImageObject){
        
        objectImages.push_back(img);
        img->used = true;
    }
//    for (int i = 0; i < numberOfObjectType; i++) {
//        while (1) {
//            bool kt = true;
//            math::resource::Image* img = allImageObject[RandomHelper::random_int(0, (int)allImageObject.size() - 1)];
//            for (int j = 0; j < i; j++) {
//                if (objectImages[j]->getText("name_1").compare(img->getText("name_1")) == 0) {
//                    kt = false;
//                    break;
//                }
//                if (objectImages[j]->getText("color").compare(img->getText("color")) == 0) {
//                    kt = false;
//                    break;
//                }
//            }
//            if (kt) {
//                img->used = true;
//                objectImages.push_back(img);
//                break;
//            }
//        }
//    }
	//Math_Pool::getInstance()->resetImages();
	this->parseQuestion();
	// FIX GAME CHUNG
	float posX = 60;
	float posY = 200;
	//Gen objects
	int itemSize = 0;
	for (int i = 0; i < numberOfObjectType; i++) {
		itemSize += objectsNumber[i];
		for (int j = 0; j < objectsNumber[i]; j++) {
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			obj->setEnable(i, math::common::TouchPermission::deny, objectImages[i], NULL);
			obj->setObjectSize(objectSize);
			_objects.push_back(obj);
		}
	}
	std::vector <Vec2> vecPos = Math_Quiz::randomPositionsInRect(objectSize, cocos2d::Rect(500, 100, 450 - posX / 2 - 50, 100 + 80 * maxRow - 50), itemSize);
	int index = 0;
	for (Math_Object* obj : _objects) {
		obj->setPosition(vecPos[index]);
		index++;
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	// Gen graph

	for (int i = 0; i < maxRow; i++) {
		for (int j = 0; j < numberOfObjectType + 1; j++) {
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			// fix tam image
			obj->setEnable(i*numberOfObjectType + j, math::common::TouchPermission::touch, boxObject, boxObject);
			obj->setObjectSize(graphSize*1.02f);
			obj->setPosition(Vec2(posX + graphSize.width*(0.5 + j), posY + graphSize.height*(0.5 + i)));
			//obj->getImage()->setScale(2.f);
			if (j != 0) {
				obj->setImage(objectImages[j - 1]);
				obj->setTouchCallback(CC_CALLBACK_1(Nam_SortAndDisplayResult::onTouchedObject, this));
				// draw point
				auto point = cocos2d::DrawNode::create();
				point->drawDot(obj->getPosition(), 6.f, Color4F::ORANGE);
				this->addChild(point);
				points[i][j] = point;
				pointsVec.push_back(point);
			}
			obj->setImageVisible(false);
			if (j == 0) {
				obj->setLocked(true);
				obj->setValueText(cocos2d::StringUtils::toString(i + 1));
				obj->getLabelValue()->setFontSize(50);
				obj->getLabelValue()->setVisible(true);
				obj->setBackground(boxBlue);
			}
			graphObject[i][j] = obj;
			this->addChildNode(obj, 2);
			obj->onShow(0);
			_objects.push_back(obj);
		}
	}
	Math_Object* obj_origin = Math_Pool::getInstance()->getObject();
	obj_origin->setEnable(0, math::common::TouchPermission::deny, boxYellow, NULL);
	obj_origin->setObjectSize(graphSize*1.02f);
	obj_origin->setPosition(Vec2(graphObject[0][0]->getPositionX(), graphObject[0][0]->getPositionY() - graphSize.height));
	this->addChildNode(obj_origin, 2);
	objFooterGraph.push_back(obj_origin);
	obj_origin->onShow(0);
	for (int i = 0; i < numberOfObjectType; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		obj->setEnable(i, math::common::TouchPermission::deny, objectImages[i], boxYellow);
		obj->setObjectSize(graphSize*1.02f);
		obj->setPosition(Vec2(graphObject[0][i + 1]->getPositionX(), graphObject[0][i + 1]->getPositionY() - graphSize.height));
		this->addChildNode(obj, 2);
		objFooterGraph.push_back(obj);
		obj->onShow(0);
	}
}

void Nam_SortAndDisplayResult::onStart() {
	//
	//Gen button done
	btn_done = Math_Pool::getInstance()->getAnswer();
	btn_done->setEnable("Done", math::common::answer, CC_CALLBACK_1(Nam_SortAndDisplayResult::onAnswerDone, this),this->getAnswerSize(1));
	btn_done->setPosition(cocos2d::Vec2(450, 55));
	btn_done->onShowInParent(_panel, 2);
	_answers.push_back(btn_done);
    showQuestionBox();
	// ket qua hien tai
	for (int i = 0; i < numberOfObjectType; i++) {
		currentAnswer.push_back(0);
	}
}

void Nam_SortAndDisplayResult::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_SortAndDisplayResult::onExplanation(bool correct) {}
void Nam_SortAndDisplayResult::onComplete() {
    //TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
    
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (cocos2d::DrawNode* point : pointsVec) {
		point->setVisible(false);
	}
	btn_done->setDisable();
	for (Math_Object* obj : objFooterGraph) {
		obj->onHide(m_delay);
		m_delay += 0.015f;
	}
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.015;
	}
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback
void Nam_SortAndDisplayResult::onTouchedObject(Node* node) {
	Math_Object* object = (Math_Object*)node;
	int currentX, currentY;
	for (int i = 0; i < maxRow; i++) {
		for (int j = 1; j < numberOfObjectType + 1; j++) {
			if (object == graphObject[i][j]) {
				currentX = j-1;
				currentY = i;
				break;
			}
		}
	}
	currentAnswer[currentX] = currentY+1;
	for (int i = 0; i < maxRow; i++) {
		if (i <= currentY) {			
			graphObject[i][currentX + 1]->setImageVisible(true);
			points[i][currentX + 1]->setVisible(false);
			graphObject[i][currentX + 1]->getImage()->setScale(0.1f);
			graphObject[i][currentX + 1]->getImage()->runAction(ScaleTo::create(0.2f, 1.0f));
		}
		else {
			graphObject[i][currentX + 1]->setImageVisible(false);
			points[i][currentX + 1]->setVisible(true);
		}
	}
}

void Nam_SortAndDisplayResult::onAnswerDone(Node* node) {
	Math_Answer* button = (Math_Answer*)(node);
	bool isDone = true;
	for (int i = 0; i < numberOfObjectType; i++) {
		if (currentAnswer[i] != objectsNumber[i]) {
			for (int j = 0; j < maxRow; j++) {
				graphObject[j][i+1]->setImageVisible(false);
				points[j][i + 1]->setVisible(true);
				currentAnswer[i] = 0;
				isDone = false;
			}
		}
	}
	if (isDone) {
        button->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		button->onAnimationClick(false);
	}
}




#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_SortAndDisplayResult::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_SortAndDisplayResult::autoTestingRun(float delaytime)
{
   
}

