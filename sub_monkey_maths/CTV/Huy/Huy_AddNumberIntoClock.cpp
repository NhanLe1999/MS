//
//  Huy_AddNumberIntoClock.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "Huy_AddNumberIntoClock.h"

Huy_AddNumberIntoClock* Huy_AddNumberIntoClock::createQuiz() {
	Huy_AddNumberIntoClock* quiz = new Huy_AddNumberIntoClock();
	if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Huy_AddNumberIntoClock::init() {
	return cocos2d::ui::Layout::init();
}

void Huy_AddNumberIntoClock::initQuiz(mj::model::Game game) {
    Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    rapidjson::Value& json_play = getJsonMember(json_game, "game_play");
    if (json_play.Empty()) {
        is_add_number = true;
        is_learn_time = false;
    } else {
        for (rapidjson::SizeType i=0; i<json_play.Size(); i++) {
            std::string value = json_play[i].GetString();
            if (value == "add_number") {
                is_add_number = true;
            }
            if (value == "learn_time") {
                is_learn_time = true;
            }
        }
    }

	// Get all images
    std::vector<math::resource::Image*> all_images = getNumbers(1, 12);
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : all_images) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : all_images) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, all_images);
        }
    }
    
    m_clock = math::AnalogClock::createClock(1);
    m_clock->setTime(0, 0);
    m_clock->setClockSize(500);

    text_object = mpool->getObject();
    text_object->setEnable(0, math::common::deny, math::resource::ResHelper::createImage(""), nullptr);
    text_object->setObjectSize(cocos2d::Size(100, 100));
    text_object->setImageVisible(false);
    addChildNode(text_object);
    
}

void Huy_AddNumberIntoClock::onStart() {
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(Huy_AddNumberIntoClock::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    quizBegan(0, true);

    m_clock->setPosition(cocos2d::Vec2(quiz_size.width*0.5, (quiz_size.height * 0.85-getQuestion()->getContentSize().height-20) * 0.5+quiz_size.height * 0.15));
    addChildNode(m_clock);
    m_clock->show(0, math::animation::SlideUp);
    m_clock->hideAllNumbers();
    m_clock->hideHand(true);
    m_clock->hideHand(false);
    
    text_object->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height*0.09));
    text_object->onShow(0);
}

void Huy_AddNumberIntoClock::startQuestion() {
	
}

void Huy_AddNumberIntoClock::onExplanation(bool correct) {
	if (!correct) {
		
	}
	else {
        scheduleOnce([=](float dt) {
            this->nextQuestion();
        }, 1.0, "next__Question");
	}
}

void Huy_AddNumberIntoClock::onComplete() {
    m_clock->destroy(0, math::animation::SlideUp);
    quizEnded(0.75, true);
}

bool Huy_AddNumberIntoClock::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event){
    index_touch++;
    CCLOG("Touch on quiz: %d", index_touch);
    
    if (index_touch <=12) {
        disableControlQuiz(0.5);
        m_clock->showNumber(index_touch);
    } else if (index_touch == 13) {
        disableControlQuiz(0.5);
        m_clock->showHand(true);
    } else if (index_touch == 14) {
        m_clock->showHand(false);
        disableControlQuiz();
        scheduleOnce([=](float){
            minute = 0;
            runClock(0.02);
        }, 0.5, "start_clock");
    } else {
        if (hour < 12) {
            disableControlQuiz();
            minute = 0;
            runClock(0.02);
        }
    }
    
    return false;
}

void Huy_AddNumberIntoClock::runClock(float dt) {
    if (minute == 60) {
        enableControlQuiz();
        minute = 0;
        hour++;
        m_clock->setTime(hour, minute);
        
        audio_helper->play_sequence(m_clock->getTimeAudios(), nullptr, m_clock->getTimeNumber());
        
        text_object->setValueText(m_clock->getTimeSemiText());
        if (hour != 1) {
            text_object->hideValueTextWithAnimation(false);
        }
        text_object->showValueTextWithAnimation(true);
        
        if (hour == 12) {
            if (completed) {
                completed = false;
                this->onExplanation(true);
            }
        }
    } else {
        minute+=2;
        m_clock->setTime(hour, minute);
        scheduleOnce([=](float){
            this->runClock(dt);
        }, dt, m_clock->getTimeNumber());
    }
}

void Huy_AddNumberIntoClock::autoTesting() {

}

void Huy_AddNumberIntoClock::autoTestingRun(float delaytime) {

}
