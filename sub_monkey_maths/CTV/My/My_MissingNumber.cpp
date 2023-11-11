#include "My_MissingNumber.h"

My_MissingNumber* My_MissingNumber::createQuiz() {
	My_MissingNumber *quiz = new My_MissingNumber();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_MissingNumber::init() {
	return cocos2d::ui::Layout::init();
}

void My_MissingNumber::initQuiz(mj::model::Game game) {

	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	number_object = randomRangeJson(json_object, "number");
	object_size = getSizeJson(json_object, "size");
	object_value = json_object.FindMember("value")->value.FindMember("type")->value.GetString();
    std::string bg_code = getJsonString(json_object, "background", "none");

	//Dragger Config
	rapidjson::Value& json_dragger = json_data.FindMember("drager")->value;
	number_dragger = randomRangeJson(json_dragger, "number");
	dragger_size = getSizeJson(json_dragger, "size");
	int answer_min = json_dragger.FindMember("value")->value.FindMember("minimum")->value.GetInt();
	int answer_max = json_dragger.FindMember("value")->value.FindMember("maximum")->value.GetInt();

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
    count_style = getJsonString(json_game, "count_style", "forward");
    count_distance = getJsonInt(json_game, "step", 1);
    drag = getJsonBool(json_game, "drag_drop", true);
    number_slot = getJsonInt(json_game, "slot", 1);
	slot_size = object_size;
	if (!drag) {
		number_slot = 1;
		number_dragger = 4;
	}
    
	int start_min = json_game.FindMember("number")->value.FindMember("min")->value.GetInt();
	int start_max = json_game.FindMember("number")->value.FindMember("max")->value.GetInt();
	int start_number = 0;
    number_line = getJsonBool(json_game, "number_line", false);

	//Lấy ảnh nền cho slot
    slot_bg = slotBG();

	switch (count_distance) {
	case 1: {
		start_number = cocos2d::random(start_min, start_max);
	}
			break;
	case 2: {
		start_number = cocos2d::random(start_min, start_max);
	}
			break;
	case 5: {
		start_number = cocos2d::random(start_min, start_max) * 5;
		start_min *= 5;
		start_max *= 5;
	}
			break;
	default: {
		start_number = cocos2d::random(start_min, start_max) * 10;
		start_min *= 10;
		start_max *= 10;
	}
			 break;
	}

    int story_fix = getJsonInt(json_game, "story", -1);
    slots_pos = randomArrayIndexes(number_slot, number_object);
    if (story_fix >= 0 && story_fix < number_object && number_slot == 1) {
        slots_pos.clear();
        slots_pos.push_back(story_fix);
    }
	if (slots_pos.size() > 1) {
		for (int i = 0; i < slots_pos.size() - 1; i++) {
			for (int j = i + 1; j < slots_pos.size(); j++) {
				if (slots_pos.at(i) > slots_pos.at(j)) {
					int temp = slots_pos.at(i);
					slots_pos.at(i) = slots_pos.at(j);
					slots_pos.at(j) = temp;
				}
			}
		}
	}

	//Lấy hình ảnh cho object
	int max = answer_max > start_number + (number_object + 1) * count_distance ? answer_max : start_number + (number_object + 1) * count_distance;
	auto imgs = getNumbers(0, max);

	int object_value = start_number;
	while (object_imgs.size() < number_object) {
		for (math::resource::Image* img : imgs) {
			if (img->getComparisionValue("number") == object_value) {
				object_imgs.push_back(img);
				break;
			}
		}
		object_value += count_distance;
	}

	//Lấy hình ảnh cho dragger
	std::vector<int> dragger_values;
	for (int i = 0; i < slots_pos.size(); i++) {
		dragger_values.push_back(start_number + slots_pos.at(i) * count_distance);
	}

	while (dragger_values.size() < number_dragger) {
		bool is_insert = true;
		int value = cocos2d::random(answer_min, answer_max);
		for (int ans_val : dragger_values) {
			if (ans_val == value) {
				is_insert = false;
				break;
			}
		}
		if (is_insert) {
			dragger_values.push_back(value);
		}
	}

	for (int val : dragger_values) {
		for (math::resource::Image* img : imgs) {
			if (img->getComparisionValue("number") == val) {
				dragger_imgs.push_back(img);
				break;
			}
		}
	}

	//TODO: GENERATES QUIZ COMPONENTS
	//Tạo Object
	math::common::TouchPermission touch_per = math::common::deny;
	int obj_id = 0;
	for (int i = 0; i < number_object; i++) {
		Math_Object* object = mpool->getObject();
		object->setEnable(obj_id++, touch_per, object_imgs.at(i), NULL);
		object->setObjectSize(object_size);
		object->setImageVisible(false);
		object->setValueTextByKey("number");
		object->getLabelValue()->setFontSize(object_size.height*0.75);
		object->showValueTextWithAnimation(false);

		for (int slot_index : slots_pos) {
			if (i == slot_index) {
				Math_Slot* slot = mpool->getSlot();
				slot->setEndble(0, slot_bg);
				slot->setSlotSize(slot_size * 0.95);
				slot->setComparisionValue(object->getComparisionValue());
				_slots.push_back(slot);
				if (drag) {
					object->hideValueTextWithAnimation(false);
				}
				else {
					object->setValueText("?");
				}
			}
		}
		_objects.push_back(object);
	}

	//Tạo Dragger
    auto drag_bg = objectBG();
	if (drag) {
		math::func::setPositionList(_objects, number_object, cocos2d::Rect(0.05, 0.1, 0.9, 0.4));

		math::common::TouchPermission touch_per_dragger = math::common::touch;
		for (int i = 0; i < number_dragger; i++) {
			Math_Object* dragger = mpool->getObject();
			dragger->setEnable(i, touch_per_dragger, dragger_imgs[i], drag_bg);
			dragger->setObjectSize(object_size);
			dragger->setImageVisible(false);
			dragger->setValueTextByKey("number");
			dragger->getLabelValue()->setFontSize(object_size.height*0.6);
			dragger->showValueTextWithAnimation(true);
            dragger->setMoveCallback([=](cocos2d::Node* node){
                audio_helper->play("audio_drag.mp3");
            }, CC_CALLBACK_2(My_MissingNumber::onMoveObjectEnded, this));
			_dragers.push_back(dragger);
			test_draged.push_back(dragger);
		}
		math::func::shuffle(test_draged);

	}
	else {
		math::func::setPositionList(_objects, number_object, cocos2d::Rect(0.05, 0.05, 0.4, 0.4));
		result = _objects[slots_pos.front()]->getComparisionValue().number;
		//Tạo Các đáp án lựa chọn
		for (int i = 0; i < number_dragger; i++) {
			Math_Answer* answer = mpool->getAnswer();
			answer->setEnable("", math::common::answer, CC_CALLBACK_1(My_MissingNumber::onTouchAnswer, this), object_size);
            answer->setIcon(dragger_imgs[i]);
            answer->setTitleWithIcon("name_1");
			if (dragger_imgs[i]->getComparisionValue("number") == result) {
				answer->setCorrect(true);
			}
            answer->setCustomAnswer(slot_bg, object_size, object_size.height*0.6);
			_answers.push_back(answer);
			test_chose.push_back(answer);
		}
		math::func::shuffle(test_chose);
	}

    //Question Config
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, object_imgs);
    }
}

void My_MissingNumber::onStart() {
    startGuide();

    onShowBottomBar(0, 0.3);
    quizBegan(0.5, false);
    
    float delta_obj = (quiz_size.width - number_object * object_size.width) / (number_object+1);
    delta_obj = delta_obj > 100 ? 100 : delta_obj;
    
	//Vẽ trục số
	if (number_line) {
        math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height* 0.3, quiz_size.width, quiz_size.height* 0.7 - 60));
        //Show slot
        for (int i = 0; i < _slots.size(); i++) {
            int index = slots_pos[i];
            _slots[i]->setPosition(_objects[index]->getPosition());
            this->addChildNode(_slots[i], 2);
            _slots[i]->onShow(index * 0.1, math::animation::SlideUp);
        }

        //Show Object
        float delay = 0;
        for (Math_Object* object : _objects) {
            object->fitObjectSize();
            this->addChildNode(object, 3);
            object->onShow(delay, math::animation::SlideUp);
            delay += 0.1;
        }
        
        //number line
        float start_line = _objects[0]->getPosition().x;
        float delta_line = _objects[1]->getPosition().x - _objects[0]->getPosition().x;
        float line_y = _objects[0]->getPosition().y + object_size.height *0.5 + 30;
        
        //line
        auto img_line = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
        img_line->setScale9Enabled(true);
        auto line_render = img_line->getVirtualRendererSize();
        img_line->setContentSize(cocos2d::Size(delta_line*(number_object), line_render.height));
        img_line->setPosition(cocos2d::Vec2(quiz_size.width *0.5, line_y));
        addChildNode(img_line);
        ui_elements.push_back(img_line);
        
        //arrows
        auto img_arrow = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
        img_arrow->setPosition(cocos2d::Vec2((quiz_size.width + delta_line*number_object) * 0.5 - 7, line_y));
        addChildNode(img_arrow);
        ui_elements.push_back(img_arrow);
        
        auto img_arrow2 = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
        img_arrow2->setPosition(cocos2d::Vec2((quiz_size.width - delta_line*number_object) * 0.5 + 7, line_y));
        img_arrow2->setFlippedX(true);
        addChildNode(img_arrow2);
        ui_elements.push_back(img_arrow2);
        
        //sub lines
        for (int i=0; i<number_object; i++) {
            auto img_subline = cocos2d::ui::ImageView::create(getUIPath("common/numberline/point.png"));
            img_subline->setPosition(cocos2d::Vec2(start_line + delta_line*i, line_y));
            addChildNode(img_subline);
            ui_elements.push_back(img_subline);
        }
        
    } else {
        math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height* 0.3, quiz_size.width, quiz_size.height* 0.7));
        
        //Show Object
        auto img_comma1 = math::resource::ResHelper::createImage(getUIPath("gameui/missing_number/comma.png"));
        auto img_comma2 = math::resource::ResHelper::createImage(getUIPath("gameui/missing_number/comma2.png"));
        auto comma_size = cocos2d::Size(75, 75);

        float delta_comma = (_objects[1]->getPosition() - _objects[0]->getPosition()).x / 2;
        for (int i=0; i<_objects.size(); i++) {
            auto position = _objects[i]->getPosition();
            auto comma = mpool->getObject();
            comma->setEnable(1, math::common::deny, (i<number_object-1)?img_comma1:img_comma2, nullptr);
            comma->setObjectSize(comma_size);
            comma->setPosition(cocos2d::Vec2(position.x + delta_comma, position.y));
            addChildNode(comma);
            comma->onShow(0.5, math::animation::No_Effect);
            commas.push_back(comma);
        }
        
        for (int i = 0; i < _slots.size(); i++) {
            int index = slots_pos[i];
            _slots[i]->setPosition(_objects[index]->getPosition());
            this->addChildNode(_slots[i], 3);
            _slots[i]->onShow(index * 0.1, math::animation::SlideUp);
        }

        float delay = 0;
        for (Math_Object* object : _objects) {
            object->fitObjectSize();
            this->addChildNode(object, 3);
            object->onShow(delay, math::animation::SlideUp);
            delay += 0.1;
        }
    }

    float delta = (quiz_size.width - 40 - number_dragger * object_size.width) / (number_dragger+1);
    delta = delta > 100 ? 100 : delta;
    float m_delay = 0.5;
    if (drag) {
        math::func::smartSetPosition(_dragers, object_size, cocos2d::Rect(20, 0, quiz_size.width - 40, quiz_size.height*0.3 - 30), delta);
		for (Math_Object* dragger : _dragers) {
            addChildNodeToBottomPanel(dragger, 2);
            dragger->onShow(m_delay);
			m_delay += 0.1;
		}
	}
	else {
        math::func::smartSetPosition(_answers, object_size, cocos2d::Rect(20, 0, quiz_size.width - 40, quiz_size.height*0.3 - 30), delta);
		for (Math_Answer* answer : _answers) {
            answer->onShowInParent(_panel_bot, 2, m_delay, math::animation::SlideUp);
            m_delay += 0.1;
		}
	}
}

void My_MissingNumber::startQuestion() {
	enableControlQuiz();
	if (questionIndex() == 0) {
        if (drag) {
            for (Math_Object* drag : _dragers) {
                drag->enableDrag(_panel_bot);
            }
        }
	}
}

void My_MissingNumber::onExplanation(bool correct) {
	if (!correct) {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
		if (drag) {
			for (Math_Slot* slot : _slots) {
				if (slot->isUsed() && !slot->isCorrect(math::comparision::number)) {
                    auto linker = slot->getLinker();
                    slot->removeLinker();
                    linker->stopAllActions();
                    linker->backToStart();
				}
			}
            audio_helper->play("audio_moveback.mp3");
		}
	}
	else {
        disableControlQuiz();
		this->nextQuestion(0.25);
	}
}

void My_MissingNumber::onComplete() {
	if (number_line) {
        for (cocos2d::ui::Widget* imgview : ui_elements) {
            imgview->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.5), NULL));
        }
    } else {
        for (int i=0; i<commas.size(); i++) {
            commas[i]->onHide((i+0.5)*0.1, math::animation::No_Effect);
        }
    }
    
    if (drag) {
        for (int i=0; i<number_dragger; i++) {
            _dragers[i]->onHide(i*0.1, math::animation::SlideUp);
        }
    } else {
        for (int i=0; i<number_dragger; i++) {
            _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
        }
    }

    for (int i=0; i<number_object; i++) {
        _objects[i]->onHide(i*0.1, math::animation::ScaleUp);
    }

	for (int i = 0; i < _slots.size(); i++) {
        _slots[i]->onHide(slots_pos[i] * 0.1, math::animation::ScaleUp);
	}

    quizEnded(0.5, true);
}

void My_MissingNumber::onGuideStarted() {
    disableControlQuiz();
    
    //Find unused slot
    Math_Slot* slot = nullptr;
    
    auto slots_temp = _slots;
    math::func::shuffle(slots_temp);
    for (Math_Slot* i_slot : slots_temp) {
        if (!i_slot->isUsed()) {
            slot = i_slot;
            break;
        }
    }
    
    if (slot) {
        Math_Object* object = nullptr;
        for (Math_Object* i_object : _dragers) {
            slot->removeLinker();
            slot->setLinker(i_object);
            if (slot->isCorrect(math::comparision::number)) {
                object = i_object;
                break;
            }
        }
        slot->removeLinker();
        
        if (object) {
            GuideCenter->guideBySwipe(nullptr, object->getWorldPosition(), slot->getWorldPosition());
            return;
        }
    }
    
    GuideCenter->guideNone();
}

void My_MissingNumber::onGuideDone() {
    enableControlQuiz();
}


void My_MissingNumber::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {

    Math_Object* object = static_cast<Math_Object*>(node);
	Math_Slot* t_slot = nullptr;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			t_slot = slot;
			break;
		}
	}
    
    if (t_slot != nullptr) {
        if (t_slot->isUsed()) {
            object->backToStart();
            GuideCenter->increamentIncorrect();
            return;
        } else {
            t_slot->setLinker(object);
            if (t_slot->isCorrect(math::comparision::number)) {
                object->removeFromParentAndCleanup(false);
                object->setPosition(position);
                addChildNode(object, 10);
                object->runAction(cocos2d::MoveTo::create(0.1, t_slot->getPosition()));
                object->onDropObject(0.1);
                object->setLocked(true);
            } else {
                t_slot->removeLinker();
                object->backToStart();
                GuideCenter->increamentIncorrect();
                return;
            }
        }
    } else {
        //move start
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->backToStart();
        audio_helper->play("audio_moveback.mp3");
        return;
    }
    
    //Check done
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed()) {
            return;
        }
    }
    scheduleOnce([=](float){
        onExplanation(true);
    }, 1.0, "check done");
}

void My_MissingNumber::onAnswerDone(cocos2d::Node* node) {
    auto answer = static_cast<Math_Answer*>(node);
	bool done = true;
	for (Math_Slot* slot : _slots) {
		if (!slot->isCorrect(math::comparision::number)) {
			done = false;
			break;
		}
	}
    answer->onAnimationClick(done);
    onExplanation(done);
}

void My_MissingNumber::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* touch = (Math_Answer*)node;
	bool correct = touch->isCorrect();
	touch->onAnimationClick(correct);
	onExplanation(correct);
}

void My_MissingNumber::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(2.0);
	}, 5, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void My_MissingNumber::autoTestingRun(float delaytime) {
	if (drag) {
		Math_Object* dragger = test_draged.at(test_draged.size() - 1);
		test_draged.pop_back();
		dragger->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.2, _slots.at(0)->getPosition())));
		_slots.at(0)->setLinker(dragger);
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			_answers[0]->onAutoClick();
			if (_slots.at(0)->isCorrect(math::comparision::number)) {
				_answers[0]->onAnimationClick(true);
				this->nextQuestion();
			}
			else {
				_answers[0]->onAnimationClick(false);
				_slots.at(0)->getLinker()->backToStart();
				_slots.at(0)->removeLinker();
				this->scheduleOnce([=](float dt) {
					this->autoTestingRun(delaytime);
				}, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
			}
		}), NULL));
	}
	else {
		Math_Answer* touch = test_chose.at(test_chose.size() - 1);
		test_chose.pop_back();
		int x = 0;
		for (int i = 0; i < 4; i++) {
			if (_answers.at(i) == touch) {
				x = i;
				break;
			}
		}
		if (dragger_imgs.at(x)->getComparisionValue("number") == result) {
			touch->onAnimationClick(true);
			this->nextQuestion();
		}
		else {
			touch->onAnimationClick(false);
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, 2, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
		}
	}
}
