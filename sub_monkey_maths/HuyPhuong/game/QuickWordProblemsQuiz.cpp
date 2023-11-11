//
//  QuickWordProblemsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "QuickWordProblemsQuiz.h"

QuickWordProblemsQuiz* QuickWordProblemsQuiz::createQuiz()
{
    QuickWordProblemsQuiz* quiz=new QuickWordProblemsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool QuickWordProblemsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void QuickWordProblemsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    is_time_ask = getJsonBool(json_game, "time_ask", false);
    is_onething = getJsonBool(json_game, "one_thing", true);
    multiple = getJsonInt(json_game ,"multiple", 1);
    
    rapidjson::Value& answer_type_json = getJsonMember(json_game, "answer");
    std::string answer_type_str="number_only";
    if (!answer_type_json.Empty()) {
        answer_type_str = answer_type_json[cocos2d::random(0, (int)answer_type_json.Size()-1)].GetString();
    }
    if (answer_type_str == "number_only") {
        answer_type = AT_number_only;
    } else if (answer_type_str == "sentence_only") {
        answer_type = AT_sentence_only;
    } else {
        answer_type = AT_number_and_text;
    }
    
    std::string game_type_str = getJsonString(json_game, "game_type", "addition");
    if (game_type_str == "addition") {
        is_addition = true;
    } else if (game_type_str == "subtraction") {
        is_addition = false;
    } else {
        is_addition = cocos2d::random(0, 1) == 0;
    }
    
    //Gen number img
    int total_number = randomRangeJson(json_game, "total");
    if (total_number < 2) {
        total_number = 2;
    }
    int number_small = cocos2d::random(1, total_number-1);
    total_number*=multiple;
    number_small*=multiple;
    
    if (is_addition) {
        img_number.push_back(getNumbers(number_small, number_small).front());
        img_number.push_back(getNumbers(total_number - number_small, total_number - number_small).front());
        img_number.push_back(getNumbers(total_number, total_number).front());
    } else {
        img_number.push_back(getNumbers(total_number, total_number).front());
        img_number.push_back(getNumbers(number_small, number_small).front());
        img_number.push_back(getNumbers(total_number - number_small, total_number - number_small).front());
    }
    
    //Gen img time
    for (int i=0; i<2; i++) {
        auto clock = new math::ClockBase();
        clock->setTime(img_number[i]->getComparisionValue("number"), 0);
        img_time.push_back(clock->getImgOfClock());
    }
    
    //All image
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
    std::string same = getJsonString(json_icon, "same", "none");
    std::string diff = getJsonString(json_icon, "different", "id");
    
    auto all_img = getImageList("icons");
    for (math::resource::Image* img : all_img) {
        int role = img->getComparisionValue("role");
        if (role == 0) {
            img_thing.push_back(img);
        } else if (role == 1) {
            img_person.push_back(img);
        } else {
            img_unit.push_back(img);
        }
    }
    
    img_thing = math::resource::ResHelper::filterImages(img_thing, same);
    math::func::shuffle(img_unit);
    math::func::shuffle(img_thing);
    math::func::shuffle(img_person);
    
    //Tobe
    for (int i=0; i<img_number.size(); i++) {
        int number = img_number[i]->getComparisionValue("number");
        if (i==2) {
            number = 2;
        }
        if (number > 1) {
            auto _tobe = math::resource::ResHelper::createImage("");
            std::vector<std::string> audios;
            _tobe->addProperty("tobe", "are", audios);
            img_tobe.push_back(_tobe);
        } else {
            auto _tobe = math::resource::ResHelper::createImage("");
            std::vector<std::string> audios;
            _tobe->addProperty("tobe", "is", audios);
            img_tobe.push_back(_tobe);
        }
    }
    
    //One thing
    if (is_onething) {
        auto img_select = math::func::getRandomNode(img_thing);
        img_thing.clear();
        img_thing.push_back(img_select);
        img_thing.push_back(img_select);
        img_thing.push_back(img_select);
    } else {
        while (img_thing.size() > 2) {
            img_thing.pop_back();
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "time_24") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_time) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "time_12") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_time) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "person_name") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "people_name") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "the") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "use") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<img_number.size(); i++) {
                auto number = img_number[i]->getComparisionValue("number");
                if (i==2) {
                    number = 2;
                }
                if (number < 2) {
                    auto img_txt = img_unit[0]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_unit[0]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "tobe") {
            getQuestion()->setOptionalTexts(key, img_tobe);
        } else if (key == "name_n") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<img_number.size(); i++) {
                auto number = img_number[i]->getComparisionValue("number");
                if (i==2) {
                    number = 2;
                }
                if (number < 2) {
                    auto img_txt = img_thing[i]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_thing[i]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_thing);
        }
    }
    getQuestion()->setKeywordValue(is_addition?0:1);
    
    //Gen answer
    if (is_time_ask) {
        answer_type = AT_number_only;
    }
    
    if (answer_type != AT_sentence_only) {
        int min_number = number_small>(total_number-number_small)?(total_number-number_small):number_small;
        int max_number = total_number;
        if (is_addition) {
            max_number += multiple;
        } else {
            if (min_number > multiple) {
                min_number-=multiple;
            }
        }
        while (max_number - min_number < 4 * multiple) {
            if (min_number>multiple) {
                min_number-=multiple;
            } else {
                max_number+=multiple;
            }
        }
        auto numbers = getNumbers(min_number, max_number);
        img_answer.push_back(img_number.back());
        math::func::shuffle(numbers);
        
        if (is_time_ask) {
            std::vector<int> answer_number;
            int first_number = img_answer.front()->getComparisionValue("number");
            for (math::resource::Image* img : numbers) {
                int number = img->getComparisionValue("number");
                if (abs(number-first_number) % multiple == 0 && number != first_number) {
                    answer_number.push_back(number);
                }
                if (img_answer.size() == 4) {
                    break;
                }
            }
            for (int number : answer_number) {
                auto clock = new math::ClockBase();
                clock->setTime(number, 0);
                img_answer.push_back(clock->getImgOfClock());
            }
        } else {
            int first_number = img_answer.front()->getComparisionValue("number");
            for (math::resource::Image* img : numbers) {
                int number = img->getComparisionValue("number");
                if (abs(number-first_number) % multiple == 0 && number != first_number) {
                    img_answer.push_back(img);
                }
                if (img_answer.size() == 4) {
                    break;
                }
            }
            
            if (answer_type == AT_number_and_text) {
                std::string text_unit_1 = "";
                std::vector<std::string> voice_unit_1;
                std::string text_unit_n = "";
                std::vector<std::string> voice_unit_n;
                
                if (img_unit.empty()) {
                    if (is_onething) {
                        text_unit_1 = img_thing.front()->getText();
                        voice_unit_1 = img_thing.front()->getAudios();
                        text_unit_n = img_thing.front()->getText("name_n");
                        voice_unit_n = img_thing.front()->getAudios("name_n");
                    } else {
                        text_unit_1 = img_thing.front()->getText("kind");
                        voice_unit_1 = img_thing.front()->getAudios("kind");
                        text_unit_n = img_thing.front()->getText("family");
                        voice_unit_n = img_thing.front()->getAudios("family");
                    }
                } else {
                    text_unit_1 = img_unit.front()->getText();
                    voice_unit_1 = img_unit.front()->getAudios();
                    text_unit_n = img_unit.front()->getText("name_n");
                    voice_unit_n = img_unit.front()->getAudios("name_n");
                }
                
                for (math::resource::Image* img : img_answer) {
                    int number = img->getComparisionValue("number");
                    
                    math::resource::Text _text;
                    math::resource::Voice _voice;
                    _voice.voice_id=1;
                    auto voice_paths = img->getAudios("name_1");
                    if (number < 2) {
                        for (std::string audio : voice_unit_1) {
                            voice_paths.push_back(audio);
                        }
                        _text.text = cocos2d::StringUtils::format("%s %s", img->getText().c_str(), text_unit_1.c_str());
                    } else {
                        for (std::string audio : voice_unit_n) {
                            voice_paths.push_back(audio);
                        }
                        _text.text = cocos2d::StringUtils::format("%s %s", img->getText().c_str(), text_unit_n.c_str());
                    }
                    _voice.paths = voice_paths;
                    _text.voices.push_back(_voice);
                    
                    math::resource::Property prop;
                    prop.type = math::resource::Property::PROP;
                    prop.key = "full_name";
                    prop.value = _text;
                    img->properties.front().properties.push_back(prop);
                }
            }
        }
    } else {
        auto img_equal = getOperator("=");
        auto img_operator = getOperator("+");
        if (!is_addition) {
            img_operator = getOperator("-");
        }
        
        std::vector<int> number_1;
        std::vector<int> number_2;
        std::vector<int> number_3;
        
        number_1.push_back(img_number[0]->getComparisionValue("number"));
        number_2.push_back(img_number[1]->getComparisionValue("number"));
        number_3.push_back(img_number[2]->getComparisionValue("number"));
        
        number_1.push_back(img_number[0]->getComparisionValue("number"));
        number_2.push_back(img_number[1]->getComparisionValue("number"));
        number_3.push_back(img_number[2]->getComparisionValue("number")+multiple);
        
        number_1.push_back(img_number[0]->getComparisionValue("number"));
        number_2.push_back(img_number[1]->getComparisionValue("number"));
        number_3.push_back(img_number[2]->getComparisionValue("number")-multiple);
        
        number_1.push_back(img_number[0]->getComparisionValue("number")+cocos2d::random(-1, 1)*multiple);
        number_2.push_back(img_number[1]->getComparisionValue("number")+cocos2d::random(-1, 1)*multiple);
        number_3.push_back(img_number[2]->getComparisionValue("number"));
        
        for (int i=0; i<4; i++) {
            std::vector<math::resource::Image*> img_sentence;
            img_sentence.push_back(math::number::getNumber(number_1[i]));
            img_sentence.push_back(img_operator);
            img_sentence.push_back(math::number::getNumber(number_2[i]));
            img_sentence.push_back(img_equal);
            img_sentence.push_back(math::number::getNumber(number_3[i]));
            
            auto image = math::resource::ResHelper::createImage("");
            math::resource::Text _text;
            math::resource::Voice _voice;
            _voice.voice_id=1;
            
            std::string text_sentence = "";
            std::vector<std::string> audio_sentence;
            for (math::resource::Image*img : img_sentence) {
                auto audios = img->getAudios();
                auto text = img->getText();
                for (std::string audio : audios) {
                    audio_sentence.push_back(audio);
                }
                text_sentence = cocos2d::StringUtils::format("%s %s", text_sentence.c_str(), text.c_str());
            }

            _voice.paths = audio_sentence;
            _text.voices.push_back(_voice);
            _text.text = text_sentence;
            
            math::resource::PropertiesSet prop_set;
            prop_set.lang = m_lang->getLanguage();
            
            math::resource::Property prop;
            prop.type = math::resource::Property::PROP;
            prop.key = "full_name";
            prop.value = _text;
            prop_set.properties.push_back(prop);
            image->properties.push_back(prop_set);
            
            img_answer.push_back(image);
        }
    }
    
    int answer_size = 2;
    if (answer_type != AT_number_only) {
        answer_size = 5;
    }
    for (int i=0; i<img_answer.size(); i++) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(QuickWordProblemsQuiz::onAnswered, this), getAnswerSize(answer_size));
        answer->setIcon(img_answer[i]);
        if (answer_type == AT_number_only) {
            answer->setTitleWithIcon("name_1");
        } else {
            answer->setTitleWithIcon("full_name");
        }
        if (i==0) {
            answer->setCorrect(true);
        }
        _answers.push_back(answer);
    }
    math::func::shuffle(_answers);
}

void QuickWordProblemsQuiz::onStart() {
    onShowBottomBar(0, 0.4);
    Math_Quiz::onStart();
    
    float pos_x = 0.1;
    if (answer_type == AT_number_only) {
        pos_x = 0.15;
    }
    math::func::setPositionList(_answers, 2, cocos2d::Rect(quiz_size.width *pos_x, 0, quiz_size.width *(1-pos_x*2), quiz_size.height * 0.4 - 30));
}

void QuickWordProblemsQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    int i=0;
    for (Math_Answer* answer : _answers) {
        answer->onShowInParent(_panel_bot, 2, 0.1*(i++), math::animation::SlideUp);
    }
}

void QuickWordProblemsQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}


void QuickWordProblemsQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    //Show full number
    int i=0;
    for (Math_Answer* node : _answers) {
        node->setDisable(0.1*(i++), math::animation::SlideUp);
    }
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void QuickWordProblemsQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    answer->onAnimationClick(answer->isCorrect());
    
    if (answer->isCorrect()) {
        scheduleOnce([=](float){
            this->onExplanation(true);
        }, 1.5, "wait explain");
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void QuickWordProblemsQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void QuickWordProblemsQuiz::autoTestingRun(float delaytime) {
    
}

