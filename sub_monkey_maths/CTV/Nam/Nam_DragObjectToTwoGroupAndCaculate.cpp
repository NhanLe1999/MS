//
//  DragObjectToTwoGroupAndCaculate
//  MJQuiz
//
//  Created by Tunado on 7/9/17.
//
//
#include "Nam_DragObjectToTwoGroupAndCaculate.h"

Nam_DragObjectToTwoGroupAndCaculate* Nam_DragObjectToTwoGroupAndCaculate::createQuiz()
{
	Nam_DragObjectToTwoGroupAndCaculate* quiz = new Nam_DragObjectToTwoGroupAndCaculate();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	
	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Nam_DragObjectToTwoGroupAndCaculate::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_DragObjectToTwoGroupAndCaculate::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_total = json_game.FindMember("total")->value;
	result = RandomHelper::random_int(json_total.FindMember("min")->value.GetInt(), json_total.FindMember("max")->value.GetInt());
	number1 = RandomHelper::random_int(1, result - 1);
	number2 = result - number1;
	//TODO drager config
	rapidjson::Value& json_drager_size = json_data.FindMember("drager")->value.FindMember("size")->value;
	dragerSize = Size(json_drager_size.FindMember("width")->value.GetDouble(), json_drager_size.FindMember("height")->value.GetDouble());
	//TODO: GROUP CONFIG
	rapidjson::Value& json_group = json_data.FindMember("group")->value;
	alignGroup = json_group.FindMember("align")->value.GetString();
	typeGroup = json_group.FindMember("type")->value.GetString();
	if (typeGroup == "frame") {
		gr_type = math::group::GroupType::frame;
	}
	else gr_type = math::group::GroupType::custom;
	colFrame = json_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
	rowFrame = json_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
	groupSize = cocos2d::Size(json_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("size")->value.FindMember("height")->value.GetInt());
	// image
	slot_bg = groupBG();
	slot_bg->used = true;
	drag_bg = objectBG();
	drag_bg->used = true;
	img_drager = getImageList("icons", 1, "name_1")[0];
	img_drager->used = true;
	img_bg_number = math::resource::ResHelper::createImage((m_theme->getThemePath() + "common/frame/square2.png"));
	img_bg_number->used = true;
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = img_drager->getTextValue("name_n", m_lang->getLanguage());
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	//Math_Pool::getInstance()->resetImages();
	// get number result
	for (int i = 1; i <= 3; i++) {
		int x;
		while (true)
		{
			int kt = true;
			x = RandomHelper::random_int(1, result*2);
			if (x == result) kt = false;
			for (int j : number_result) {
				if (x == j) {
					kt = false;
				}
			}
			if (kt) break;
		}
		number_result.push_back(x);
	}
	number_result.push_back(result);
	std::random_shuffle(number_result.begin(), number_result.end());
	//FIX GAME CHUNG
	//Gen group
	for (int i = 1; i <= 2; i++) {
		Math_Group* group = Math_Pool::getInstance()->getGroup();
		if (i == 1) this->genGroup(group, number1);
		else this->genGroup(group, number2);
		_groups.push_back(group);
	}
	math::func::setPositionList(_groups, 2, Rect(0, 0.15, 1, 0.5));
	//fix ui
	_groups[0]->setPositionX(_groups[0]->getPositionX() - 15);
	_groups[1]->setPositionX(_groups[1]->getPositionX() + 15);
	for (Math_Group* group : _groups) {
		this->addChildNode(group, 2);
		group->onShow();
	}
	//hien phep tinh tam thoi
	std::vector <std::string> strcal;
	strcal.push_back(cocos2d::StringUtils::toString(number1));
	strcal.push_back(cocos2d::StringUtils::toString(number2));
	for (int i = 1; i <= 2; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		this->createCaculationObject(obj, strcal[i - 1], true);
		objectsCaculation.push_back(obj);
	}
	float delay = 0;
	//math::func::setPositionList(objectsCaculation, 10, Rect(0.2, 0.15, 0.6, 0.2));
	objectsCaculation[0]->setPosition(Vec2(_groups[0]->getPositionX()+100, _groups[0]->getPositionY() - 200));
	objectsCaculation[1]->setPosition(Vec2(_groups[1]->getPositionX()+100, _groups[1]->getPositionY() - 200));
	for (Math_Object* obj : objectsCaculation) {
		this->addChildNode(obj, 2);
		obj->onShow(delay);
		delay += 0.2f;
	}
    line1 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/line/line_4.png"));
    line1->setScale9Enabled(true);
    line1->setContentSize(Size(10,80));
    _panel->addChild(line1);
    line2 = cocos2d::ui::ImageView::create((m_theme->getThemePath() + "common/line/line_4.png"));
    line2->setScale9Enabled(true);
    line2->setContentSize(Size(10,80));
    _panel->addChild(line2);
    line1->setPosition(Vec2(objectsCaculation[0]->getPositionX(),objectsCaculation[0]->getPositionY()+objectsCaculation[0]->getContentSize().height - 20));
    line2->setPosition(Vec2(objectsCaculation[1]->getPositionX(),objectsCaculation[1]->getPositionY()+objectsCaculation[1]->getContentSize().height - 20));
    line1->setOpacity(0);
    line1->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
    line2->setOpacity(0);
    line2->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
	Math_Object* dragger_object = Math_Pool::getInstance()->getObject();
	this->createGroupObject(dragger_object);
	this->addChildNodeToBottomPanel(dragger_object, 2);
	dragger_object->onShow(delay);
	delay += 0.2;
	objects_drager.push_back(dragger_object);
}

void Nam_DragObjectToTwoGroupAndCaculate::onStart() {
	onShowBottomBar(0, 0.25, true, CC_CALLBACK_1(Nam_DragObjectToTwoGroupAndCaculate::onAnswerDone, this));
	//Gen question
	showQuestionBox();
	
	// btn done
	
}

void Nam_DragObjectToTwoGroupAndCaculate::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
	else {
		objects_drager[objects_drager.size() - 1]->onHide(0);
		
		for (Math_Object* obj : objectsCaculation) {
			obj->onHide(0);
            line1->setVisible(false);
            line2->setVisible(false);
		}
		this->scheduleOnce([=](float dt) {
			
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			this->createCaculationObject(obj, cocos2d::StringUtils::toString(number1) +" + " + cocos2d::StringUtils::toString(number2) +" = " , true);
			objectsCaculation.push_back(obj);
			obj->setPosition(Vec2(quiz_size.width*0.45, 100));
			obj->setBackgroundVisible(false);
			obj->getLabelValue()->setTextColor((Color4B)Color3B(139, 87, 35));
			//fix UI
			this->addChildNode(obj, 2);
			// gen slot
			slotResult = Math_Pool::getInstance()->getSlot();
			slotResult->setEndble(0, slot_bg);
			slotResult->setSlotSize(Size(120, 120));
			slotResult->setPosition(Vec2(obj->getPositionX() + 190, obj->getPositionY()));
			this->addChildNode(slotResult, 2);
			slotResult->onShow(0);
			_slots.push_back(slotResult);
			// gen answer

			for (int i = 1; i <= 4; i++) {
				Math_Object* obj;
				obj = Math_Pool::getInstance()->getObject();
				this->createCaculationObject(obj, cocos2d::StringUtils::toString(number_result[i - 1]), false);
				obj->setObjectSize(Size(110, 110));
				obj->setBackground(drag_bg);
				obj->getLabelValue()->setTextColor(Color4B(Color3B(99,51,15)));
				_objects.push_back(obj);
				obj->setMoveCallback(CC_CALLBACK_1(Nam_DragObjectToTwoGroupAndCaculate::onTouchedObject, this), CC_CALLBACK_2(Nam_DragObjectToTwoGroupAndCaculate::onMoveObjectEndedSlot, this));
				//->setEnable(cocos2d::StringUtils::toString(number_result[i - 1]), math::common::answer, CC_CALLBACK_1(Nam_DragObjectToTwoGroupAndCaculate::onAnswerResult, this, number_result[i - 1]), cocos2d::Size(180, 60));
				//_answers.push_back(btn);	
			}
			math::func::setPositionList(_objects, 4, cocos2d::Rect(0.08, 0, 0.6, 0.22));
			for (Math_Object* obj : _objects) {
				this->addChildNodeToBottomPanel(obj, 2);
				obj->onShow(0);
				obj->enableDrag(_panel_bot);
			}
		}, 1.f, cocos2d::StringUtils::format("a%d"));
	}
}

void Nam_DragObjectToTwoGroupAndCaculate::genGroup(Math_Group* gr, int number) {
	if (gr_type == math::group::GroupType::custom) {
		std::vector<Math_Object*> _group_list;
		gr->setEnable(0, _group_list, gr_type, 10);
		gr->setObjectSize(cocos2d::Size(dragerSize));
		gr->setGroupSize(groupSize);
		gr->setBackground(slot_bg);
		//gr->updatePosition(false, true);
	}
	else {
		std::vector<Math_Object*> _group_list;
		gr->setEnable(number, _group_list, gr_type, colFrame*rowFrame);
		gr->setObjectSize(cocos2d::Size(dragerSize));
		gr->setGroupFrames(colFrame, rowFrame, slot_bg, NULL);
		if (alignGroup == "right") {
			gr->setAlignObjects(math::group::GroupAlign::right);
			gr->updatePosition(false, true);
		}
		else
			if (alignGroup == "left") {
				gr->setAlignObjects(math::group::GroupAlign::left);
				gr->updatePosition(false, true);
			}
			else
			{
				gr->setAlignObjects(math::group::GroupAlign::random);
			}
	}
}
void Nam_DragObjectToTwoGroupAndCaculate::createGroupObject(Math_Object* object) {
	object->setEnable(0, math::common::TouchPermission::move, img_drager, NULL);
	object->setObjectSize(dragerSize);
	object->setVisible(true);
	object->getImage()->setVisible(true);
	object->setPosition(Vec2(quiz_size.width*0.38,quiz_size.height*0.1));
	object->enableDrag(_panel_bot);
	object->setMoveCallback(CC_CALLBACK_1(Nam_DragObjectToTwoGroupAndCaculate::onTouchedObject, this), CC_CALLBACK_2(Nam_DragObjectToTwoGroupAndCaculate::onMoveObjectEnded, this));
}
void Nam_DragObjectToTwoGroupAndCaculate::createCaculationObject(Math_Object* obj, std::string valueText, bool looked) {
	// fix tam image
	obj->setEnable(0, math::common::TouchPermission::move, slot_bg,img_bg_number);
	obj->setObjectSize(Size(80, 80));
	obj->setImageVisible(false);
	obj->setValueText(valueText);
	obj->getLabelValue()->setVisible(true);
	obj->getLabelValue()->setFontSize(70);
	obj->getLabelValue()->setTextColor(Color4B::WHITE);
    obj->getLabelValue()->disableEffect();
	obj->setValueTextPosition("middle");
	obj->setLocked(looked);
}

void Nam_DragObjectToTwoGroupAndCaculate::onAnswerDone(Node* node){
	Math_Answer* btn = (Math_Answer*)node;
	if (this->getQuestion()->getData().index == 0) {
		if (_groups[0]->getNumberObjects() == number1 && _groups[1]->getNumberObjects() == number2) {
			for (Math_Object* obj : objects_drager) {
				obj->setLocked(true);
			}
            btn->onAnimationClick(true);
			this->nextQuestion();
		}
		else {
			btn->onAnimationClick(false);
		}
	}
	else {
		if (_slots.size() > 0) {
			if (_slots[0]->isUsed()) {
				if (_slots[0]->getLinker()->getValueText() == cocos2d::StringUtils::toString(result)) {
                    btn->onAnimationClick(true);
					this->nextQuestion();
				}
				else {
					_slots[0]->getLinker()->backToStart();
					_slots[0]->removeLinker();
					btn->onAnimationClick(false);
				}
			}
			else {
				btn->onAnimationClick(false);
			}
		}
	}
}
void Nam_DragObjectToTwoGroupAndCaculate::onAnswerResult(cocos2d::Node* node, int index){
	Math_Answer* btn = (Math_Answer*)node;

	if (index == result) {
                btn->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		btn->onAnimationClick(false);
	}
}
void Nam_DragObjectToTwoGroupAndCaculate::onTouchedObject(Node* node) {
	Math_Object* object = (Math_Object*)node;
	if (selected_object == NULL) {
		selected_object = object;
		//first_pos = object->getPosition();
	}
	else {
		object->setAllowMove(false);
	}
}
void Nam_DragObjectToTwoGroupAndCaculate::onMoveObjectEnded(Node* node, Vec2 point) {
	
	//remove object khoi group hien tai
	bool isInGroup = false;
	Math_Object* object = (Math_Object*)node;
	//Doi tuong keo tha nam trong 1 slot nao do.
	point = _panel->convertToNodeSpace(point);
	object->removeFromParentAndCleanup(false);
	object->setPosition(point);
	addChildNode(object, 2);
	for (Math_Group* _group : _groups) {
		if (_group->hasObject(object)) {
			if (_group->isTouchOnGroup(point) && gr_type == math::group::frame && alignGroup != "random") {
				object->backToLast(2);
				selected_object = NULL;
				return;
			}
			_group->removeObject(object);
			isInGroup = true;
			break;
		}
	}
	Math_Group* group = NULL;
	for (Math_Group* _group : _groups) {
		if (_group->isTouchOnGroup(point)) {
			group = _group;
			break;
		}
	}
	if (group != NULL) {
		//if (gr_type == math::group::GroupType::custom) object->removeFromParent();
		if (group->addObject(object,Vec2(point.x,point.y-0.25*quiz_size.height))) {
			if (!isInGroup) {
				Math_Object* drager_object = Math_Pool::getInstance()->getObject();
				this->createGroupObject(drager_object);
				this->addChildNodeToBottomPanel(drager_object, 2);
				drager_object->onShow(0);
				objects_drager.push_back(drager_object);
			}
			//group->updatePosition(false, true);
			//Check answer
			selected_object = NULL;
			return;
		}
	}
	if (isInGroup) {
		object->setDisable();
	}
	else {
		object->backToLast(2);
	}
	selected_object = NULL;
}
void Nam_DragObjectToTwoGroupAndCaculate::onMoveObjectEndedSlot(Node* node, Vec2 position) {
	//CODE ANH PHUONG
	Math_Object* object = (Math_Object*)node;
	position = _panel->convertToNodeSpace(position);
	object->removeFromParentAndCleanup(false);
	object->setPosition(position);
	addChildNode(object, 2);
	//Doi tuong keo tha nam trong 1 slot nao do.
	Math_Slot* m_slot = NULL;
	for (Math_Slot* slot : _slots) {
		if (object->isSame(slot->getLinker())) {
			m_slot = slot;
			break;
		}
	}

	Math_Slot* t_slot = NULL;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			t_slot = slot;
			break;
		}
	}

	if (m_slot != NULL) {
		if (t_slot != NULL) {
			//swap
			if (m_slot == t_slot) {
				object->backToLast(2);
			}
			else {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					m_slot->removeLinker();
					t_slot->removeLinker();

					t_slot->setLinker(object);
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

					m_slot->setLinker(linker);
					linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
				}
				else {
					m_slot->removeLinker();
					t_slot->setLinker(object);
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
				}
			}
		}
		else {
			//move to start
			m_slot->removeLinker();
			object->backToStart();
		}
	}
	else {
		if (t_slot != NULL) {
			if (t_slot->isUsed()) {
				auto linker = t_slot->getLinker();
				t_slot->removeLinker();
				linker->backToStart();
			}
			t_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
		}
		else {
			//move start
			object->backToStart();
		}
	}
	selected_object = NULL;
}

void Nam_DragObjectToTwoGroupAndCaculate::onExplanation(bool correct) {
}
void Nam_DragObjectToTwoGroupAndCaculate::onComplete() {
	//TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Group* group : _groups) {
		group->setDisable();
	}
	for (Math_Object* obj : objects_drager) {
		obj->onHide(0);
	}
	for (Math_Object* obj : objectsCaculation) {
		obj->onHide(m_delay += 0.01);
	}
	
	_slots[0]->onHide(m_delay += 0.01);
	for (Math_Object* obj : _objects) {
		obj->onHide(m_delay += 0.01);
	}
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");

}



#define auto_testing_delay 2
void Nam_DragObjectToTwoGroupAndCaculate::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_DragObjectToTwoGroupAndCaculate::autoTestingRun(float delaytime)
{
		
}

