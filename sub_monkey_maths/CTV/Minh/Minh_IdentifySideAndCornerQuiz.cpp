//
//  Minh_IdentifySideAndCornerQuiz.cpp
//  MJQuiz
//
//  Created by Minh_Tzy on 7/3/18.
//
//

#include "Minh_IdentifySideAndCornerQuiz.h"

Minh_IdentifySideAndCornerQuiz* Minh_IdentifySideAndCornerQuiz::createQuiz()
{
	Minh_IdentifySideAndCornerQuiz* quiz = new Minh_IdentifySideAndCornerQuiz();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Minh_IdentifySideAndCornerQuiz::init() {
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
#define detect_size  cocos2d::Size(100, 100)
#define line_size    5

void Minh_IdentifySideAndCornerQuiz::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	rapidjson::Value& json_game = json_data.FindMember("game")->value;

	mode_side = getJsonBool(getJsonMember(json_game, "game_mode"), "side");
	mode_corner = getJsonBool(getJsonMember(json_game, "game_mode"), "corner");

	std::vector<math::resource::Image*> image_list;

	auto all_images = getImageList("icons");
	math::resource::Image* image = math::func::getRandomNode(all_images);
	img_list.push_back(image);
	/*---- PARSE DATA -----*/
	rapidjson::Document doc_tracing;
	doc_tracing.Parse<rapidjson::kParseStopWhenDoneFlag>(img_list.front()->tracing.c_str());
	rapidjson::Value& trace_positions = getJsonMember(doc_tracing, "position");
	for (rapidjson::SizeType i = 0; i < trace_positions.Size(); i++) {
		rapidjson::Value& json_point = trace_positions[i];
		Dot point;
		point.index = getJsonInt(json_point, "order", 1);
		point.x = getJsonInt(json_point, "horizontal_axis", 1);
		point.y = getJsonInt(json_point, "vertical_axis", 1);
		points.push_back(point);
	}

	image_center = cocos2d::Vec2(quiz_size.width * 0.5, quiz_size.height * 0.275 );
	image_size = cocos2d::Size(450, 450);
	drag_size = cocos2d::Size(200, 100);
	std::sort(points.begin(), points.end(), [=](Dot a, Dot b)->bool {
		return a.index < b.index;
	});

	//TODO: GENERATE QUIZ COMPONENTS
	// Gen Image
    object_victim = mpool->getObject();
    object_victim->setEnable(0, math::common::deny, img_list.front(), nullptr);
    object_victim->setObjectSize(image_size);
    object_victim->fixHeightObject();
    image_size = object_victim->getContentSize();

	// Dragers and Slots
    std::vector<math::resource::Image*> img_drags;
    img_drags.push_back(getTextByTextGroup("corner"));
    img_drags.push_back(getTextByTextGroup("side"));
    
    auto obj_bg = objectBG();
    // Drag
	corner_drag = mpool->getObject();
	corner_drag->setEnable(0, math::common::move, img_drags[0], nullptr);
    corner_drag->setObjectSize(drag_size);
    corner_drag->setImageVisible(false);
    corner_drag->setValueTextByKey("name_1");
    corner_drag->setValueTextPosition("middle");
    corner_drag->showValueTextWithAnimation(true);
	corner_drag->getLabelValue()->setFontSize(42);
    corner_drag->setBackground(obj_bg, 0.3);
	corner_drag->setMoveCallback(CC_CALLBACK_1(Minh_IdentifySideAndCornerQuiz::onTouchedObject, this), CC_CALLBACK_2(Minh_IdentifySideAndCornerQuiz::onMoveObjectEnded, this));
	_dragers.push_back(corner_drag);
	// Slot
	corner_slot = mpool->getSlot();
	corner_slot->setEndble(0, nullptr);
	corner_slot->setSlotSize(drag_size*0.95);
    corner_slot->setBackground(slotBG(), 0.3);
	corner_slot->setComparisionValue(corner_drag->getComparisionValue());
	_slots.push_back(corner_slot);

	// Drag
	side_drag = mpool->getObject();
	side_drag->setEnable(1, math::common::move, img_drags[1], nullptr);
    side_drag->setObjectSize(drag_size);
    side_drag->getLabelValue()->setFontSize(42);
    side_drag->setBackground(obj_bg, 0.3);
    side_drag->setImageVisible(false);
    side_drag->setValueTextByKey("name_1");
    side_drag->setValueTextPosition("middle");
    side_drag->showValueTextWithAnimation(true);
	side_drag->setMoveCallback(CC_CALLBACK_1(Minh_IdentifySideAndCornerQuiz::onTouchedObject, this), CC_CALLBACK_2(Minh_IdentifySideAndCornerQuiz::onMoveObjectEnded, this));
	_dragers.push_back(side_drag);
	// Slot
	side_slot = mpool->getSlot();
	side_slot->setEndble(1, nullptr);
	side_slot->setSlotSize(drag_size*0.95);
    side_slot->setBackground(slotBG(), 0.3);
	side_slot->setComparisionValue(side_drag->getComparisionValue());
	_slots.push_back(side_slot);
	
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		getQuestion()->setOptionalTexts(key, img_list);
	}
}

void Minh_IdentifySideAndCornerQuiz::onStart() {
	onShowBottomBar(0, 0.25, true, CC_CALLBACK_1(Minh_IdentifySideAndCornerQuiz::onAnswerDone,this));
    quizBegan(0.5, true);
    
    math::func::shuffle(_dragers);
    math::func::smartSetPosition(_dragers, drag_size, cocos2d::Rect(0, 0, quiz_size.width-300, quiz_size.height*0.25-30), 125);
    
    image_center = cocos2d::Vec2(quiz_size.width, quiz_size.height*0.75-getQuestion()->getContentSize().height-20) * 0.5;
    object_victim->setPosition(image_center);
    addChildNode(object_victim);
    object_victim->onShow(0, math::animation::ScaleUp);
    
    if(mode_corner) {
        corner_slot->setPosition(getLocation(1));
        addChildNode(corner_slot, 3);
        corner_slot->onShow(0.2f, math::animation::ScaleUp);
    }
    if (mode_side) {
        side_slot->setPosition(getLocation(2));
        addChildNode(side_slot, 3);
        side_slot->onShow(0.2f, math::animation::ScaleUp);
    }
	for (int i = 0; i < _dragers.size(); ++i) {
		addChildNodeToBottomPanel(_dragers[i], 5);
        _dragers[i]->onShow(0.1*i+0.5, math::animation::SlideUp);
	}
}

void Minh_IdentifySideAndCornerQuiz::startQuestion() {
	enableControlQuiz();
	for (int i = 0; i < _dragers.size(); ++i) {
		_dragers[i]->enableDrag(_panel_bot);
	}
}

void Minh_IdentifySideAndCornerQuiz::onExplanation(bool correct) {
	if(!correct) {
        disableControlQuiz(0.25);
        for (Math_Slot* slot : _slots) {
            if (!slot->isCorrect(math::comparision::name)) {
                if (slot->isUsed()) {
                    slot->getLinker()->backToStart();
                    slot->removeLinker();
                }
            }
        }
    } else {
        disableControlQuiz();
        nextQuestion(0.75);
    }
}

void Minh_IdentifySideAndCornerQuiz::onComplete() {

	// Show side
    for (int i=0; i<_dragers.size(); i++) {
        _dragers[i]->onHide(0.1*i, math::animation::SlideUp);
    }

    if (mode_side) {
        side_slot->onHide(0);
    }
    if (mode_corner) {
        corner_slot->onHide(0);
    }
    
    object_victim->onHide(0);
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
#define line_weight 10

cocos2d::Vec2 Minh_IdentifySideAndCornerQuiz::getLocation(int point_index) {
	Dot point;
	for (Dot mpoint : points) {
		if (mpoint.index == point_index) {
			point = mpoint;
			break;
		}
	}
	cocos2d::Vec2 origin = cocos2d::Vec2(image_center.x - image_size.width / 2, image_center.y - image_size.height / 2);
	float padding = 60;

	float fix_x = (0.5 - (float)point.x / 1000.0) * padding + point.x;
	float fix_y = (0.5 - (float)point.y / 1000.0) * padding + point.y;

	cocos2d::Vec2 point_in_object = cocos2d::Vec2(fix_x * image_size.width / 1000, fix_y * image_size.height / 1000);
	return  origin + point_in_object;
}

cocos2d::Vec2 Minh_IdentifySideAndCornerQuiz::getSlotLocation(int point)
{
	auto location = getLocation(point);
	float x_axis, y_axis;
	x_axis = location.x - image_center.x;
	y_axis = location.y - image_center.y;
	float radius = std::sqrt(powf(x_axis, 2) + powf(y_axis, 2));
	int order;
	if (x_axis > std::sqrt(3) / 2 * radius) {
		order = 1;
	}
	else if (x_axis > 0.5 * radius) {
		if (y_axis > 0) order = 2;
		else order = 8;
	}
	else if (x_axis > -0.5 * radius) {
		if (y_axis > 0) order = 3;
		else order = 7;
	}
	else if (x_axis > -std::sqrt(3) / 2 * radius) {
		if (y_axis > 0) order = 4;
		else order = 6;
	}
	else order = 5;

	cocos2d::Vec2 position;
	switch (order)
	{
	case 1: {
		position.x = location.x + drag_size.width / 2 + 10;
		position.y = location.y;
		break;
	}
	case 2: {
		position.x = location.x + 10;
		position.y = location.y + drag_size.height / 2 + 10;
		break;
	}
	case 3: {
		position.x = location.x;
		position.y = location.y + drag_size.height / 2 + 10;
		break;
	}
	case 4: {
		position.x = location.x - 10;
		position.y = location.y + drag_size.height / 2 + 10;
		break;
	}
	case 5: {
		position.x = location.x - drag_size.width / 2 - 10;
		position.y = location.y;
		break;
	}
	case 6: {
		position.x = location.x - 10;
		position.y = location.y - drag_size.height / 2 - 10;
		break;
	}
	case 7: {
		position.x = location.x;
		position.y = location.y - drag_size.height / 2 - 10;
		break;
	}
	case 8: {
		position.x = location.x - 10;
		position.y = location.y - drag_size.height / 2 - 10;
		break;
	}
	default:
		break;
	}
	return position;
}

void Minh_IdentifySideAndCornerQuiz::onAnswerDone(cocos2d::Node * node)
{
    auto answer = static_cast<Math_Answer*>(node);
	bool complete = true;
	for (auto slot : _slots) {
		if (!slot->isUsed()) {
			complete = false;
			break;
		}
	}
	if (complete) {
		bool done = true;
		for (Math_Slot* slot : _slots) {
			if (!slot->isCorrect(math::comparision::name)) {
				done = false;
				break;
			}
		}
        answer->onAnimationClick(done);
        this->onExplanation(done);
    } else {
        answer->onAnimationClick(false);
        onExplanation(false);
    }
}

float Minh_IdentifySideAndCornerQuiz::autoDragCorrect()
{ 
    return 0;
}

void Minh_IdentifySideAndCornerQuiz::onTouchedObject(cocos2d::Node * node)
{
}

void Minh_IdentifySideAndCornerQuiz::onMoveObjectEnded(cocos2d::Node * node, cocos2d::Vec2 position)
{
	disableControlQuiz(0.25);
	Math_Slot * current_slot = NULL;
	Math_Slot * olded_slot = NULL;
	Math_Object* object = (Math_Object*)node;

	//Doi tuong keo tha nam trong 1 slot nao do.
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed() && slot->getLinker()->isSame(object)) {
			olded_slot = slot;
			break;
		}
	}
	//Check slot dich.
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(_panel->convertToNodeSpace(position))) {
			current_slot = slot;
			break;
		}
	}
	// Neu khong nam trong slot nao
	if (current_slot == NULL) {
		// Neu truoc do nam trong 1 slot -> xoa object
		if (olded_slot != NULL) {
			olded_slot->removeLinker();
		}
		// Tro ve vi tri ban dau
		object->backToStart();


	}
	// Neu nam trong 1 slot nao do
	else {
		// Neu truoc do k nam trong slot nao
		if (olded_slot == NULL) {
			// neu slot hien tai dang duoc su dung
			if (current_slot->isUsed()) {
				current_slot->getLinker()->backToStart();
				current_slot->removeLinker();
			}
			current_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getWorldPosition()));

		}
		// Neu truoc do nam o slot nao do
		// Doi cho 2 slot
		else {
			if (current_slot == olded_slot) {
				object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getWorldPosition()));
			}
			else {
				Math_Object * ocs = current_slot->getLinker();
				Math_Object * oos = olded_slot->getLinker();
				current_slot->removeLinker();
				olded_slot->removeLinker();

				current_slot->setLinker(oos);
				oos->runAction(cocos2d::MoveTo::create(0.2, current_slot->getWorldPosition()));

				if (ocs != NULL) {
					olded_slot->setLinker(ocs);
					ocs->runAction(cocos2d::MoveTo::create(0.2, olded_slot->getWorldPosition()));
				}

			}
		}
	}
	object->setLocked(false);
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Minh_IdentifySideAndCornerQuiz::autoTesting() {

	this->scheduleOnce([=](float dt) {
		//this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}

