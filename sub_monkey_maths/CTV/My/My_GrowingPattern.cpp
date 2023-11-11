#include "My_GrowingPattern.h"

#define line_width 1.5
My_GrowingPattern* My_GrowingPattern::createQuiz() {
	My_GrowingPattern *quiz = new My_GrowingPattern();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_GrowingPattern::init() {
	return cocos2d::ui::Layout::init();
}

void My_GrowingPattern::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	pattern_type = getJsonString(json_game, "pattern_type");
	min_number = randomRangeJson(json_game, "min_number");
	row_number = randomRangeJson(json_game, "row_number");
	drag = getJsonBool(json_game, "drag", true);

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	object_size = getSizeJson(json_object, "size");
	pattern_kind = getJsonString(json_object, "different", "id");

	if (pattern_type.compare("left") == 0 || pattern_type.compare("right") == 0) {
		pattern_piece = 2;
	}
	else {
		//pattern_type = left-right
		pattern_piece = 3;
	}

	//Lấy Image cho pattern
	auto all_imgs = getImageList("icons");
	pattern_imgs = getImageList(all_imgs, pattern_piece, pattern_kind);

	//Question Config
	parseQuestion();

	//GENERATE QUIZ COMPONENTS
	int first, last, mid;
	if (pattern_type.compare("left") == 0) {
		first = 0;
		last = 1;
		mid = 1;
	}
	else if (pattern_type.compare("right") == 0) {
		first = 0;
		mid = 0;
		last = 1;
	}
	else if (pattern_type.compare("left-right") == 0) {
		first = 0;
		mid = 1;
		last = 2;
	}
	//Gen Objects
	for (int i = 0; i <= row_number; i++) {
		std::vector<Math_Object*> temps;
		int obj_id = 0;
		for (int j = 0; j < min_number + i; j++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			if (j == 0) {
				object->setEnable(obj_id++, math::common::deny, pattern_imgs.at(first), nullptr);
			}
			else if (j == min_number + i - 1) {
				object->setEnable(obj_id++, math::common::deny, pattern_imgs.at(last), nullptr);
			}
			else {
				object->setEnable(obj_id++, math::common::deny, pattern_imgs.at(mid), nullptr);
			}
			object->setObjectSize(object_size*0.8);
			if (i == row_number) {
				object->setImageVisible(false);
				object->setBackgroundVisible(false);
			}
			temps.push_back(object);
		}
		objects.push_back(temps);
	}

	//Gen Slots
	int slot_id = 0;
	for (int i = 0; i < min_number + row_number; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(slot_id++, nullptr);
		slot->setSlotSize(cocos2d::Size(object_size.width - line_width, object_size.height - line_width));
		if (i == 0) {
			slot->setComparisionValue(objects.at(0).at(0)->getComparisionValue());
		}
		else if (i == min_number + row_number - 1) {
			slot->setComparisionValue(objects.at(0).back()->getComparisionValue());
		}
		else {
			slot->setComparisionValue(objects.at(1).at(1)->getComparisionValue());
		}
		_slots.push_back(slot);
	}

	//Gen Dragger
	int drag_id = 0;
	for (int i = 0; i < pattern_piece; i++) {
		Math_Object* dragger = Math_Pool::getInstance()->getObject();
		dragger->setEnable(drag_id++, math::common::touch, pattern_imgs.at(i), nullptr);
		dragger->setObjectSize(object_size*0.8);
		if (drag) {
			dragger->setMoveCallback(CC_CALLBACK_1(My_GrowingPattern::onTouchAnswer, this), CC_CALLBACK_2(My_GrowingPattern::onMoveObjectEnded, this));
		}
		else {
			dragger->setTouchCallback(CC_CALLBACK_1(My_GrowingPattern::onTouchAnswer, this));
		}
		_draggers.push_back(dragger);
	}

	//Question
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		getQuestion()->setOptionalTexts(key, pattern_imgs);
	}
}

void My_GrowingPattern::onStart() {
	onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_GrowingPattern::onTouchButtonDone, this));
	Math_Quiz::onStart();

	cocos2d::Vec2 center = cocos2d::Vec2(quiz_size.width / 2, (quiz_size.height * 0.75 - 20 - getQuestion()->getContentSize().height) / 2);

	background = math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"));
	auto img_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png"));
	auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/brown2.png"));

	auto bg = cocos2d::ui::ImageView::create(background->path);
	bg->setScale9Enabled(true);
	auto render_size = bg->getVirtualRendererSize();
	bg->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
	auto padding = render_size.width*0.1;
	bg->setContentSize(cocos2d::Size(padding * 2 + (row_number + min_number)*object_size.width, padding * 2 + (row_number + 1)*object_size.height));
	bg->setPosition(center);
	addChildNode(bg, 1);

	ui_elements.push_back(bg);
	bg->setOpacity(0);
	bg->runAction(cocos2d::FadeIn::create(0.5));

	float start_x = center.x - (row_number + min_number)*0.5*object_size.width + object_size.width*0.5;
	float start_y = center.y + (row_number + 1)*object_size.height*0.5 - object_size.height*0.5;
	for (int i = 0; i <= row_number; i++) {
		if (i < row_number) {
			for (int i2 = 0; i2 < row_number + min_number; i2++) {
				auto blank = cocos2d::ui::ImageView::create(img_bg->path);
				blank->setScale9Enabled(true);
				auto render_size_bl = blank->getVirtualRendererSize();
				blank->setCapInsets(cocos2d::Rect(render_size_bl.width*0.4, render_size_bl.height*0.4, render_size_bl.width*0.2, render_size_bl.height*0.2));
				blank->setContentSize(cocos2d::Size(object_size.width - line_width, object_size.height - line_width));
				blank->setPosition(cocos2d::Vec2(start_x + i2*object_size.width, start_y - i*object_size.height));
				addChildNode(blank, 1);

				ui_elements.push_back(blank);
				blank->setOpacity(0);
				blank->runAction(cocos2d::FadeIn::create(0.5));
			}
			auto list = objects[i];
			for (int i2 = 0; i2 < list.size(); i2++) {
				auto object = list[i2];
				object->setPosition(cocos2d::Vec2(start_x + i2*object_size.width, start_y - i*object_size.height));
				addChildNode(object);
				object->onShow(0.5 + i*0.1, math::animation::ScaleUp);
			}
		}
		else {
			for (int i2 = 0; i2 < row_number + min_number; i2++) {
				auto blank = cocos2d::ui::ImageView::create(slot_bg->path);
				blank->setScale9Enabled(true);
				auto render_size_bl = blank->getVirtualRendererSize();
				blank->setCapInsets(cocos2d::Rect(render_size_bl.width*0.4, render_size_bl.height*0.4, render_size_bl.width*0.2, render_size_bl.height*0.2));
				blank->setContentSize(cocos2d::Size(object_size.width - line_width, object_size.height - line_width));
				blank->setPosition(cocos2d::Vec2(start_x + i2*object_size.width, start_y - i*object_size.height));
				addChildNode(blank, 1);

				ui_elements.push_back(blank);
				blank->setOpacity(0);
				blank->runAction(cocos2d::FadeIn::create(0.5));
			}
			for (int i2 = 0; i2 < _slots.size(); i2++) {
				auto slot = _slots[i2];
				slot->setPosition(cocos2d::Vec2(start_x + i2*object_size.width, start_y - i*object_size.height));
				addChildNode(slot);
				slot->onShow(i*0.1, math::animation::ScaleUp);
			}
		}
	}

	//Show dragger
	int number_dr = (int)_draggers.size();
	float delta_dr = (quiz_size.width - 330 - number_dr * object_size.width) / (number_dr + 1);
	delta_dr = delta_dr > 100 ? 100 : delta_dr;
	math::func::smartSetPosition(_draggers, object_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height*0.25 - 30), delta_dr);
	float delay = 0.5;
	for (Math_Object* dragger : _draggers) {
		addChildNodeToBottomPanel(dragger, 2);
		dragger->onShow(delay, math::animation::SlideUp);
		delay += 0.1;
	}
}

void My_GrowingPattern::startQuestion() {
	enableControlQuiz();

	if (drag) {
		scheduleOnce([=](float dt) {
			for (Math_Object* dragger : _draggers) {
				dragger->enableDrag(_panel_bot);
			}
		}, 0.5, "Enable drag");

	}
}

void My_GrowingPattern::onExplanation(bool correct) {
	if (correct) {
		disableControlQuiz();
		this->nextQuestion();
	}
	else {
		disableControlQuiz(0.5);
	}
}

void My_GrowingPattern::onComplete() {
	for (int i = 0; i <= row_number; i++) {
		if (i < row_number) {
			auto list = objects[i];
			for (int i2 = 0; i2 < list.size(); i2++) {
				list[i2]->onHide(i*0.1, math::animation::ScaleUp);
			}
		}
		else {
			for (int i2 = 0; i2 < _slots.size(); i2++) {
				auto slot = _slots[i2];
				//addChildNode(slot);
				slot->onHide(i*0.1, math::animation::ScaleUp);
			}
		}
	}

	auto delay = 0;
	for (Math_Object* dragger : _draggers) {
		dragger->onHide(delay, math::animation::SlideUp);
		delay += 0.1;
	}

	scheduleOnce([=](float dt) {
		Math_Quiz::onComplete();
	}, 1.0, "complete_quiz");
}

void My_GrowingPattern::onTouchAnswer(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;

	touch_slot = false;
	for (Math_Slot* slot : _slots) {
		if (object->isSame(slot->getLinker())) {
			if (!drag) {
				slot->removeLinker();
			}
			touch_slot = true;
			break;
		}
	}

	if (drag) {
		if (touch_slot) {

		}
		else {
			for (int i = 0; i < _draggers.size(); i++) {
				if (object->isSame(_draggers[i])) {
					_draggers.erase(_draggers.begin() + 1);
					break;
				}
			}
			auto cloned = object->clone();
			cloned->onShow(0, math::animation::No_Effect);
			_draggers.push_back(cloned);
		}
	}
	else {
		if (touch_slot) {
			object->onHide(0, math::animation::ScaleUpAndRotation);
		}
		else {
			Math_Slot* t_slot = nullptr;
			for (Math_Slot* slot : _slots) {
				if (!slot->isUsed()) {
					t_slot = slot;
					break;
				}
			}

			if (t_slot == nullptr) {
				object->onTouchObject(false);
			}
			else {
				auto cloned = object->clone();
				auto position = cloned->getWorldPosition();
				cloned->removeFromParentAndCleanup(false);
				_panel_move->addChild(cloned, 100);
				cloned->setPosition(position);
				cloned->onShow(0, math::animation::No_Effect);

				/*cloned->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, t_slot->getWorldPosition())), cocos2d::CallFunc::create([=]() {
					cloned->removeFromParentAndCleanup(false);
					this->addChild(cloned, 3);
					cloned->setPosition(t_slot->getPosition());
				})));*/
				cloned->runAction(cocos2d::MoveTo::create(0.5, t_slot->getWorldPosition()));
				t_slot->setLinker(cloned);
			}
		}
	}
}

void My_GrowingPattern::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point) {
	Math_Object* object = (Math_Object*)node;

	auto position = _panel->convertToNodeSpace(point);

	Math_Slot* m_slot = nullptr;
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed() && object->isSame(slot->getLinker())) {
			m_slot = slot;
			break;
		}
	}

	Math_Slot* t_slot = nullptr;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			t_slot = slot;
			break;
		}
	}

	if (t_slot == nullptr) {
		object->onHide(0, math::animation::ScaleUpAndRotation);
	}
	else {
		if (m_slot == nullptr) {
			if (t_slot->isUsed()) {
				object->onHide(0, math::animation::ScaleUpAndRotation);
			}
			else {
				object->removeFromParentAndCleanup(false);
				object->setPosition(position);
				addChildNode(object, 10);

				t_slot->setLinker(object);
				object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			}
		}
		else {
			object->removeFromParentAndCleanup(false);
			object->setPosition(position);
			addChildNode(object, 10);

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
}

void My_GrowingPattern::onTouchButtonDone(cocos2d::Node* node) {
	auto btn_done = static_cast<Math_Answer*>(node);
	bool done = true;
	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed()) {
			done = false;
		}
	}
	if (done) {
		for (Math_Slot* slot : _slots) {
			CCLOG("Comparision Value: %s", pattern_kind.c_str());
			if (!slot->isCorrect(getComparisionKey(pattern_kind))) {
				CCLOG("Fail at: %d", slot->getId());
				//if (_slots.at(i)->getLinker()->getImageSource() != _groups.at(_groups.size() - 1)->getObjectAtIndex(i)->getImageSource()) {
				done = false;
				break;
			}
		}
	}

	if (!done) {
		for (Math_Slot* slot : _slots) {
			if (slot->isUsed()) {
				auto linker = slot->getLinker();
				slot->removeLinker();
				linker->onHide(0, math::animation::ScaleUpAndRotation);
			}
		}
	}
	btn_done->onAnimationClick(done);
	onExplanation(done);
}
