//
//  ChoosePerfectObjectsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "ChoosePerfectObjectsQuiz.h"

ChoosePerfectObjectsQuiz* ChoosePerfectObjectsQuiz::createQuiz()
{
    ChoosePerfectObjectsQuiz* quiz=new ChoosePerfectObjectsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ChoosePerfectObjectsQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void ChoosePerfectObjectsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::vector<std::string> cmp_props;
    rapidjson::Value& json_prop_cmp = getJsonMember(json_game, "prop_cmp");
    for (rapidjson::SizeType i=0; i<json_prop_cmp.Size(); i++) {
        std::string prop_str = json_prop_cmp[i].GetString();
        if (prop_str == "size") {
            is_cmp_size = true;
            is_big = cocos2d::random(0, 1) == 1;
        } else {
            cmp_props.push_back(prop_str);
        }
    }
    is_fuz_size = getJsonBool(json_game, "size_fuzzy", false);
    revert_answer = getJsonBool(json_game, "answer_type", false);
    
    rapidjson::Value& json_out_cmp = getJsonMember(json_game, "out_cmp");
    is_out_cmp = getJsonBool(json_out_cmp, "enable", false);
    if (is_out_cmp) {
        out_cmp_prop = getJsonString(json_out_cmp, "prop_cmp", "length");
    }
    
    object_size = getSizeJson(json_game, "size");
    display_object = getJsonString(json_game, "display", "inline");
    number_total = randomRangeJson(json_game, "number_total");
    number_correct = randomRangeJson(json_game, "number_correct");
    if (number_total <= number_correct) {
        number_total = number_correct + 2;
    }
    
    //Icon đúng là icon thỏa mãn tất cả yêu cầu có trong cmp_prop (có thể thừa)
    //Icon sai là icon k thỏa mãn (dù chỉ một) yêu cầu.
    auto all_imgs = getImageList("icons");
    std::string filter_prop = getJsonString(json_game, "filter_icon", "none");
    bool filter_prop_check = true;
    for (math::resource::Property prop : all_imgs.front()->comparisions) {
        if (prop.key == filter_prop) {
            filter_prop_check = false;
            break;
        }
    }
    
    if (is_out_cmp) {
        std::vector<math::resource::Image*> out_imgs;
        for (math::resource::Image* img : all_imgs) {
            int role = img->getComparisionValue("role");
            if (role == 0) {
                img_list.push_back(img);
            } else {
                out_imgs.push_back(img);
            }
        }
        out_img = math::func::getRandomNode(out_imgs);
        math::func::shuffle(img_list);
        while (img_list.size() > number_total) {
            img_list.pop_back();
        }
        number_total = (int)img_list.size();
        number_correct = 1;
        is_cmp_size = false;
        is_fuz_size = false;
    } else {
        rapidjson::Value& json_fix_icon = getJsonMember(json_game, "fix_icon");
        bool is_fix_icon = getJsonBool(json_fix_icon, "enable");
        if (is_fix_icon) {
            int index = getJsonInt(json_fix_icon, "index", 0);
            if (index >= all_imgs.size()) {
                index = 0;
            }
            img_list.push_back(all_imgs[index]);
        } else {
            img_list.push_back(math::func::getRandomNode(all_imgs));
        }
        
        if (filter_prop_check) {
            all_imgs = math::resource::ResHelper::filterImages(all_imgs, filter_prop, cocos2d::Value(img_list.front()->getText(filter_prop)));
        } else {
            all_imgs = math::resource::ResHelper::filterImages(all_imgs, filter_prop, cocos2d::Value(img_list.front()->getComparisionValue(filter_prop)));
        }
        
        for (std::string prop : cmp_props) {
            std::string value = img_list.front()->getText(prop);
            prop_value_list.push_back(std::make_pair(prop, value));
            if (value == "") {
                CCLOG("ERROR: Thuoc tinh chua duoc khai bao trong icon");
            }
        }
        
        //add correct icon
        for (math::resource::Image* img : all_imgs) {
            if (img->code != img_list.front()->code) {
                bool valid = true;
                for (std::pair<std::string, std::string> prop_value : prop_value_list) {
                    std::string value = img->getText(prop_value.first);
                    if (value == "" || value != prop_value.second) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    img_list.push_back(img);
                }
                if (img_list.size() >= number_correct) {
                    break;
                }
            }
        }
        while (img_list.size() < number_correct) {
            img_list.push_back(math::func::getRandomNode(img_list));
        }

        //add wrong icon
        if (is_cmp_size) {
            int number_added = 0;
            math::func::shuffle(all_imgs);
            for (math::resource::Image* img : all_imgs) {
                bool valid = true;
                for (std::pair<std::string, std::string> prop_value : prop_value_list) {
                    std::string value = img->getText(prop_value.first);
                    if (value == "" || value != prop_value.second) {
                        valid = true;
                        break;
                    }
                }
                
                if (valid) {
                    img_list.push_back(img);
                } else {
                    img_list_wrong.push_back(img);
                }
                number_added++;
                if (number_added >= number_total - number_correct) {
                    break;
                }
            }
            
            while (number_added < number_total - number_correct) {
                if (img_list_wrong.empty()) {
                    img_list.push_back(math::func::getRandomNode(img_list));
                    number_added++;
                } else {
                    if (cocos2d::random(0, 1)==0) {
                        img_list.push_back(math::func::getRandomNode(img_list));
                        number_added++;
                    } else {
                        img_list_wrong.push_back(math::func::getRandomNode(img_list_wrong));
                        number_added++;
                    }
                }
            }
        } else {
            for (math::resource::Image* img : all_imgs) {
                bool valid = true;
                for (std::pair<std::string, std::string> prop_value : prop_value_list) {
                    std::string value = img->getText(prop_value.first);
                    if (value == prop_value.second) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    img_list_wrong.push_back(img);
                }
                if (img_list_wrong.size() >= number_total - number_correct) {
                    break;
                }
            }
            while (img_list_wrong.size() < number_total - number_correct) {
                img_list_wrong.push_back(math::func::getRandomNode(img_list_wrong));
            }
        }
    }
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    if (is_out_cmp) {
        for (std::string key : opt_keys) {
            if (key == "like") {
                std::vector<math::resource::Text> texts;
                auto img_txt = out_img->getTextValue("name_1");
                texts.push_back(img_txt);
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_list);
            }
        }
    } else {
        for (std::string key : opt_keys) {
            getQuestion()->setOptionalTexts(key, img_list);
        }
        getQuestion()->setKeywordValue(is_big?0:1);
    }
    
    //Size objects
    std::vector<int> sizes;
    if (is_cmp_size) {
        for (int i=0; i<number_total; i++) {
            if (i<number_correct) {
                sizes.push_back(is_big?0:2);
            } else {
                if (i<img_list.size()) {
                    sizes.push_back(is_big?2:0);
                } else {
                    sizes.push_back(cocos2d::random(0, 1)*2);
                }
            }
        }
    } else {
        if (is_fuz_size) {
            for (int i=0; i<number_total; i++) {
                sizes.push_back(cocos2d::random(0, 1)*2);
            }
        } else {
            for (int i=0; i<number_total; i++) {
                sizes.push_back(0);
            }
        }
    }
    
    //Gen object
    for (int i=0; i<number_total; i++) {
        math::resource::Image* img = nullptr;
        if (i<img_list.size()) {
            img = img_list[i];
        } else {
            img = img_list_wrong[i - (int)img_list.size()];
        }
        
        auto object = mpool->getObject();
        object->setEnable(i, math::common::touch, img, nullptr);
        object->setComparisionValue(sizes[i]);
        object->setObjectSize(object_size);
        object->setTouchCallback(CC_CALLBACK_1(ChoosePerfectObjectsQuiz::onTouchObject, this));
        _objects.push_back(object);
    }
    
    math::func::shuffle(_objects);
}

void ChoosePerfectObjectsQuiz::onStart() {
    Math_Quiz::onStart();
    
    std::vector<cocos2d::Vec2> locations;
    if (display_object == "inline") {
        int number_inline = 5;
        if (number_total <= 5) {
            number_inline = number_total;
        } else if (number_total <= 10) {
            number_inline = (number_total + 1)/2;
        }
        
        int delta = 180;
        float objs_width = number_inline*(object_size.width + delta) - delta;
        while (objs_width>quiz_size.width * 0.9) {
            delta-=5;
            objs_width = number_inline*(object_size.width + delta) - delta;
        }
        int number_line = (number_total + number_inline - 1)/number_inline;
        
        float start_x = (quiz_size.width-objs_width) /2.0;
        float delta_y = object_size.height + 50;
        float start_y = (quiz_size.height - getQuestion()->getContentSize().height - 25 - delta_y * number_line - object_size.height/2) / 2;
        
        locations = math::func::getLocations(number_total, number_inline, cocos2d::Rect(start_x, start_y, quiz_size.width - start_x*2, start_y + delta_y * number_line), quiz_size);
    } else {
        locations = randomPositionsInRect(object_size*1.2, cocos2d::Rect(quiz_size.width * 0.05, quiz_size.height * 0.05, quiz_size.width*0.9, quiz_size.height*0.9 - getQuestion()->getContentSize().height-25), number_total);
    }
    
    std::vector<int> random_show;
    for (int i=0; i<_objects.size(); i++) {
        random_show.push_back(i);
    }
    int number_div = ((int)_objects.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->setPosition(locations[i]);
        addChildNode(object, 2);
        object->onShow((random_show[i]/number_div)*0.1);
    }
}

void ChoosePerfectObjectsQuiz::startQuestion() {
    enableControlQuiz();
}

void ChoosePerfectObjectsQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void ChoosePerfectObjectsQuiz::onComplete() {
    
    std::vector<int> random_hide;
    for (int i=0; i<_objects.size(); i++) {
        random_hide.push_back(i);
    }
    int number_div = ((int)_objects.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide((random_hide[i]/number_div)*0.1);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void ChoosePerfectObjectsQuiz::onTouchObject(Node* node) {
    disableControlQuiz(0.5);
    Math_Object* object = static_cast<Math_Object*>(node);
    bool correct = true;
    
    if (is_out_cmp) {
        int out_value = out_img->getComparisionValue(out_cmp_prop);
        int object_value = object->getImageSource()->getComparisionValue(out_cmp_prop);
        correct = (out_value == object_value);
    } else {
        if (is_cmp_size) {
            int object_size = object->getComparisionValue().size;
            if ((is_big && object_size>0) || (!is_big && object_size==0)) {
                correct = false;
            }
        }
        
        if (correct) {
            for (auto pair : prop_value_list) {
                std::string value = object->getImageSource()->getText(pair.first);
                if (value != pair.second) {
                    correct = false;
                    break;
                }
            }
        }
    }
    
    if (revert_answer) {
        correct = !correct;
        object->onTouchObject(correct, 0, 100);
        if (correct) {
            object->setLocked(true);
            number_touch++;
            if (number_touch == (number_total - number_correct)) {
                this->onExplanation(true);
            }
        }
    } else {
        object->onTouchObject(correct, 0, 100);
        if (correct) {
            object->setLocked(true);
            number_touch++;
            if (number_touch == number_correct) {
                this->onExplanation(true);
            }
        }
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ChoosePerfectObjectsQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ChoosePerfectObjectsQuiz::autoTestingRun(float delaytime) {
    
}
