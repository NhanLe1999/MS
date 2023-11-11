//
//  Nam_PartitionShapeColor
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_PartitionShapeColor.h"

Nam_PartitionShapeColor* Nam_PartitionShapeColor::createQuiz()
{
    Nam_PartitionShapeColor* quiz=new Nam_PartitionShapeColor();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_PartitionShapeColor::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_PartitionShapeColor::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);
    
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	//game_mode = json_game.FindMember("mode")->value.GetString();
	mau_so = RandomHelper::random_int(2, 4);
	tu_so = RandomHelper::random_int(1, mau_so - 1);
	std::string str_result_kind = "1/" + cocos2d::StringUtils::toString(mau_so);
	// GET IMAGE
	auto allImage = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	for (math::resource::Image* img : allImage) {
		if (img->getText("kind") == str_result_kind) {
			images_shape.push_back(img);
			img->used = true;
		}
	}
	math::resource::Image* image_tu_so = this->getNumbers(tu_so, tu_so)[0];
	image_tu_so->used = true;
	math::resource::Image* image_mau_so = this->getNumbers(mau_so, mau_so)[0];
	image_mau_so->used = true;
	// get question
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (int i = 0; i < opt_keys.size(); i++) {
		if (opt_keys[i] == "number") {
			std::vector<math::resource::Text> texts;
			int x = image_tu_so->getComparisionValue("number");
			math::resource::Text t;
			t.text = cocos2d::StringUtils::toString(x) + " /";
			texts.push_back(t);
			x = image_mau_so->getComparisionValue("number");
			t.text = cocos2d::StringUtils::toString(x);
			texts.push_back(t);
			getQuestion()->setOptionalTexts(opt_keys[i], texts);
			
		}
	}
	//TODO: GEN
	for (int i = 0; i < images_shape.size(); i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		obj->setEnable(i, math::common::TouchPermission::touch, images_shape[i], NULL);
		obj->setObjectSize(Size(500, 500));
		obj->setTouchDetectedType(math::common::DetectedType::nontrans);
		obj->setTouchCallback(CC_CALLBACK_1(Nam_PartitionShapeColor::onTouchedObject, this));
		obj->setPosition(Vec2(450, 350));
		obj->setName("0");
		_objects.push_back(obj);
	}
	//Math_Pool::getInstance()->resetImages();
}

void Nam_PartitionShapeColor::onStart() {
	math_statistic->setCurrentGame(_game);
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	//Gen submit
	Math_Answer* btn_done = Math_Pool::getInstance()->getAnswer();
	btn_done->setEnable("Submit", math::common::answer, CC_CALLBACK_1(Nam_PartitionShapeColor::onAnswerDone, this), this->getAnswerSize(2));
	btn_done->setPosition(cocos2d::Vec2(450, 55));
	_answers.push_back(btn_done);
	btn_done->onShowInParent(_panel,2);
	
	//Gen question
    showQuestionBox();
}

void Nam_PartitionShapeColor::startQuestion() {
    CCLOG("%s override", __func__);
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
		/*auto stencil = DrawNode::create();
		stencil->drawSolidCircle(Vec2(450,400), 50.0f, 0.0f, 32.0f, Color4F::GREEN);
		// create the clipping node and set the stencil 
		auto clipper = ClippingNode::create();
		clipper->setStencil(stencil);

		// create the sprite, which should be clipped
		auto sprite = Sprite::create();
		sprite->setSpriteFrame("icons/animal/animal_0.png");
		sprite->setPosition(Vec2(450, 400));

		// add the sprite to the clipping node, so it can be clipped
		clipper->addChild(sprite);

		// add the clipping node to the main node
		this->addChild(clipper);*/
	}
}
void Nam_PartitionShapeColor::onExplanation(bool correct) {}
void Nam_PartitionShapeColor::onComplete() {
    CCLOG("%s override", __func__);
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
	}

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback

void Nam_PartitionShapeColor::onAnswerDone(Node* node) {
	Math_Answer* btn = (Math_Answer*)node;
	int count = 0;
	for (Math_Object* obj : _objects) {
		if (obj->getName() == "1") {
			count++;
		}
	}
	if (count == tu_so) {
		for (Math_Object* obj : _objects) {
			obj->setLocked(true);
		}
		btn->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		btn->onAnimationClick(false);
	}
	
}
void Nam_PartitionShapeColor::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)(node);
	if (object->getName() == "0") {
		object->getImage()->setColor(Color3B::BLUE);
		object->setName("1");
	}
	else {
		object->getImage()->setColor(Color3B::WHITE);
		object->setName("0");
	}
	//object->getImage()->runAction()
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_PartitionShapeColor::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Nam_PartitionShapeColor::autoTestingRun(float delaytime)
{
   
}

