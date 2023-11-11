//
//  Thanh_Matching2C.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_Matching2C.h"
#include "Math_Libs.h"
#include "RomanNumeralsUtils.h"
#define line_color cocos2d::Color4F(cocos2d::Color3B(243, 104, 35))
#define line_width 6

Thanh_Matching2C* Thanh_Matching2C::createQuiz()
{
    Thanh_Matching2C* quiz=new Thanh_Matching2C();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_Matching2C::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_Matching2C::initQuiz(mj::model::Game game)
{

    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;

    str = json_game.FindMember("option")->value.GetString();
    if(str == "row") option = Option::row;
    else if(str == "column") option = Option::column;
    else if(str == "random") {
        if(cocos2d::random(0, 1)) option = Option::row;
        else option = Option::column;
    }

    rapidjson::Value& game_type_json = json_game.FindMember("game_type")->value;
    auto index_type = cocos2d::random(0, (int)game_type_json.Size()-1);
    str = game_type_json[index_type].GetString();
    if(str == "OO") gameType = GameType::OO;
    else if(str == "GO") gameType = GameType::GO;
    else if(str == "OG") gameType = GameType::OG;
    else if(str == "GG") gameType = GameType::GG;
    number_pair = json_game.FindMember("number_pair")->value.GetInt();

    object_size = getSizeJson(json_game, "object_size");
    group_size = getSizeJson(json_game, "group_size");
    img_bg = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));
    group_bg= groupBG();

    std::string compare_str = json_game.FindMember("compare_key")->value.GetString();
    if (compare_str.compare("total")==0) {
        make_sum = true;
    } else {
        compare_key = getComparisionKey(compare_str);
        if (gameType != GameType::OO) {
            compare_key = math::comparision::number;
        }
    }
    min_range = json_game.FindMember("range")->value.FindMember("minimum")->value.GetInt();
    max_range = json_game.FindMember("range")->value.FindMember("maximum")->value.GetInt();

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_reuse = json_object.FindMember("resource_reuse")->value.GetBool();

    str = json_object.FindMember("object_1")->value.FindMember("display")->value.GetString();
    if(str == "name") object_option_1 = ObjectOption::text;
    else if(str == "number") object_option_1 = ObjectOption::number;
    else if(str == "ordinal") object_option_1 = ObjectOption::ordinal;
	else if (str == "roman") object_option_1 = ObjectOption::roman;
    else  object_option_1 = ObjectOption::image;

    str = json_object.FindMember("object_2")->value.FindMember("display")->value.GetString();
    if(str == "name") object_option_2 = ObjectOption::text;
    else if(str == "number") object_option_2 = ObjectOption::number;
    else if(str == "ordinal") object_option_2 = ObjectOption::ordinal;
	else if(str == "roman") object_option_2 = ObjectOption::roman;
    else  object_option_2 = ObjectOption::image;

    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    group_reuse = json_group.FindMember("resource_reuse")->value.GetBool();

    rapidjson::Value& group_1 = json_group.FindMember("group_1")->value;
    ele_group_1_size = getSizeJson(group_1, "object_size");
    str = group_1.FindMember("group_type")->value.GetString();
    if(str == "frame") type_group_1 = math::group::GroupType::frame;
    else type_group_1 = math::group::GroupType::custom;
    frame_col_1 = group_1.FindMember("frame")->value.FindMember("column")->value.GetInt();
    frame_row_1 = group_1.FindMember("frame")->value.FindMember("row")->value.GetInt();

    rapidjson::Value& group_2 = json_group.FindMember("group_2")->value;
    ele_group_2_size = getSizeJson(group_2, "object_size");
    str = group_2.FindMember("group_type")->value.GetString();
    if(str == "frame") type_group_2 = math::group::GroupType::frame;
    else type_group_2 = math::group::GroupType::custom;
    frame_col_2 = group_2.FindMember("frame")->value.FindMember("column")->value.GetInt();
    frame_row_2 = group_2.FindMember("frame")->value.FindMember("row")->value.GetInt();

    //TODO:
    //lay tat ca icon trong media_set
    auto all_imgs = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);

    switch (gameType) {
        case OO:
        {
            for (math::resource::Image* img : all_imgs) {
                int role = img->getComparisionValue("role");
                if (role == 0) {
                    icons_1.push_back(img);
                }
            }
            if(!object_reuse) {
                for (math::resource::Image* img : all_imgs) {
                    int role = img->getComparisionValue("role");
                    if (role == 1) {
                        icons_2.push_back(img);
                    }
                }
            } else {
                for (math::resource::Image*img : icons_1) {
                    icons_2.push_back(img);
                }
            }
        }
            break;
        case GG:
        {
            for (math::resource::Image* img : all_imgs) {
                int role = img->getComparisionValue("role");
                if (role == 2) {
                    icons_1.push_back(img);
                }
            }
            if(!group_reuse) {
                for (math::resource::Image* img : all_imgs) {
                    int role = img->getComparisionValue("role");
                    if (role == 3) {
                        icons_2.push_back(img);
                    }
                }
            }else {
                for (math::resource::Image*img : icons_1) {
                    icons_2.push_back(img);
                }
            }
        }
            break;
        default:
        {
            for (math::resource::Image* img : all_imgs) {
                int role = img->getComparisionValue("role");
                if (role==0) {
                    icons_1.push_back(img);
                }
            }
            for (math::resource::Image* img : all_imgs) {
                int role = img->getComparisionValue("role");
                if (role == 2) {
                    icons_2.push_back(img);
                }
            }
        }
            break;
    }

	if (gameType == GG) {
		if (group_reuse) {
			img_group_1 = math::func::getRandomNode(icons_1);
			do {
				img_group_2 = math::func::getRandomNode(icons_2);
			} while (img_group_1 == img_group_2);
		}
		else {
			img_group_1 = math::func::getRandomNode(icons_1);
			img_group_2 = math::func::getRandomNode(icons_2);
		}
	}
	else {
		if (icons_1.size()>0) {
			img_group_1 = math::func::getRandomNode(icons_1);
		}
	}

    if (make_sum) {
        sum_value = cocos2d::random(min_range, max_range);
        value_list = randomArrayIndexes(number_pair, sum_value);
        for (int val : value_list) {
            value_list2.push_back(sum_value - val);
        }
        for (int value : value_list) {
            for (math::resource::Image* img : icons_1) {
                if (img->getComparisionValue("number") == value) {
                    img->used=true;
                    image_list_1.push_back(img);
                    break;
                }
            }
        }
        for (int value : value_list2) {
            for (math::resource::Image* img : icons_2) {
                if (img->getComparisionValue("number") == value) {
                    img->used=true;
                    image_list_2.push_back(img);
                    break;
                }
            }
        }
        if (image_list_1.size()==0) {
            image_list_2.clear();
            auto icon_number = getNumbers(0, sum_value);
            for (int value : value_list) {
                for (math::resource::Image* img : icon_number) {
                    if (img->getComparisionValue("number") == value) {
                        img->used=true;
                        image_list_1.push_back(img);
                        break;
                    }
                }
            }
			for (int value : value_list2) {
				for (math::resource::Image* img : icon_number) {
					if (img->getComparisionValue("number") == value) {
						img->used = true;
						image_list_2.push_back(img);
						break;
					}
				}
			}
        } 
    } else {
        if (compare_key == math::comparision::number) {
            auto list_num = randomArrayIndexes(number_pair, max_range-min_range+1);
            for (int val : list_num) {
                value_list.push_back(val + min_range);
            }
            for (int value : value_list) {
                for (math::resource::Image* img : icons_1) {
                    if (img->getComparisionValue("number") == value) {
                        img->used=true;
                        image_list_1.push_back(img);
                        image_list_2.push_back(img);
                        break;
                    }
                }
            }
            if (image_list_1.size()==0) {
                image_list_2.clear();
                auto icon_number = getNumbers(0, max_range+1);
                for (int value : value_list) {
                    for (math::resource::Image* img : icon_number) {
                        if (img->getComparisionValue("number") == value) {
                            img->used=true;
                            image_list_1.push_back(img);
                            image_list_2.push_back(img);
                            break;
                        }
                    }
                }
            }
            if (gameType==GG) {
                if (group_reuse) {
                    img_group_1 = math::func::getRandomNode(icons_1);
                    do {
                        img_group_2 = math::func::getRandomNode(icons_2);
                    } while (img_group_1 == img_group_2);
                } else {
                    img_group_1 = math::func::getRandomNode(icons_1);
                    img_group_2 = math::func::getRandomNode(icons_2);
                }
            } else {
                if (icons_2.size()>0) {
                    img_group_1 = math::func::getRandomNode(icons_2);
                }
            }

        } else {
            switch (compare_key) {
                case math::comparision::exactly :
                {
                    while (image_list_1.size() < number_pair) {
                        auto img = math::func::getRandomNode(icons_1);
                        bool add = true;
                        for (math::resource::Image* img2 : image_list_1) {
                            if (img2->code == img->code) {
                                add = false;
                                break;
                            }
                        }
                        if (add) {
                            image_list_1.push_back(img);
                        }
                    }
                    image_list_2 = image_list_1;
                }
                    break;
                case math::comparision::name :
                {
                    while (image_list_1.size() < number_pair) {
                        auto img = math::func::getRandomNode(icons_1);
						bool add = true;
						for (math::resource::Image *image : image_list_1) {
							if (image == img) add = false;
						}
						if (add) {
							image_list_1.push_back(img);
							for (math::resource::Image* img2 : icons_2) {
								if (img2->getText() == img->getText()) {
									image_list_2.push_back(img2);
									break;
								}
							}
						}
                    }

                }
                    break;
                case math::comparision::kind :
                {
					while (image_list_1.size() < number_pair) {
						auto img = math::func::getRandomNode(icons_1);
						bool add = true;
						for (math::resource::Image *image : image_list_1) {
							if (image == img) add = false;
						}
						if (add) {
							image_list_1.push_back(img);
							for (math::resource::Image* img2 : icons_2) {
								if (img2->getText("kind") == img->getText("kind")) {
									image_list_2.push_back(img2);
									break;
								}
							}
						}
					}

                }
                    break;
                case math::comparision::color :
                {
					while (image_list_1.size() < number_pair) {
						auto img = math::func::getRandomNode(icons_1);
						bool add = true;
						for (math::resource::Image *image : image_list_1) {
							if (image == img) add = false;
						}
						if (add) {
							image_list_1.push_back(img);
							for (math::resource::Image* img2 : icons_2) {
								if (img2->getText("color") == img->getText("color")) {
									image_list_2.push_back(img2);
									break;
								}
							}
						}
					}
                }
                    break;
                default:
                    break;
            }

        }
    }
    /*---> Lay xong data cua game*/


    //TODO: Xử lý dữ liệu cấu hình câu hỏi

    std::vector<math::resource::Image*> img_number_list;
    if (make_sum) {
        img_number_list = getNumbers(sum_value, sum_value);
    }

    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_number_list);
        }
    }

    //TODO: GENERATE QUIZ COMPONENTS
    genObjectGroup(); 
}

void Thanh_Matching2C::onStart() {

    math_statistic->setCurrentGame(_game);

    switch (gameType) {
        case OO:
        {
            for (Math_Object* obj : objects_1) {
                addChildNode(obj, 2);
                obj->onShow(0);
                MyPair *mp = new MyPair();
                mp->node = obj;
                mp->panel_1 = true;
                my_pair.push_back(mp);
            }

            for (Math_Object* obj : objects_2) {
                addChildNode(obj, 2);
                obj->onShow(0.2);
                MyPair *mp = new MyPair();
                mp->node = obj;
                mp->panel_1 = false;
                my_pair.push_back(mp);
            }
        }
            break;
        case OG:
        {
            for (Math_Object* obj : objects_1) {
                addChildNode(obj, 2);
                obj->onShow(0);
                MyPair *mp = new MyPair();
                mp->node = obj;
                mp->panel_1 = true;
                my_pair.push_back(mp);
            }
            for (Math_Group* group : groups_1) {
                addChildNode(group, 2);
				group->onShow(0, math::animation::No_Effect);
                MyPair *mp = new MyPair();
                mp->node = group;
                mp->panel_1 = false;
                my_pair.push_back(mp);
            }
        }
            break;
        case GO:
        {
            for (Math_Group* group : groups_1) {
                addChildNode(group, 2);
                group->onShow(0, math::animation::No_Effect);
                MyPair *mp = new MyPair();
                mp->node = group;
                mp->panel_1 = true;
                my_pair.push_back(mp);
            }
            for (Math_Object* obj : objects_1) {
                addChildNode(obj, 2);
                obj->onShow(0.2);
                MyPair *mp = new MyPair();
                mp->node = obj;
                mp->panel_1 = false;
                my_pair.push_back(mp);
            }
        }
            break;
        default:
        {
            for (Math_Group* group : groups_1) {
                addChildNode(group, 2);
                group->onShow(0, math::animation::No_Effect);
                MyPair *mp = new MyPair();
                mp->node = group;
                mp->panel_1 = true;
                my_pair.push_back(mp);
            }

            for (Math_Group* group : groups_2) {
                addChildNode(group, 2);
                group->onShow(0, math::animation::No_Effect);
                MyPair *mp = new MyPair();
                mp->node = group; 
                mp->panel_1 = false;
                my_pair.push_back(mp);
            }
        }
            break;
    }
    for(MyPair *mp: my_pair){
		mp->light = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/2.png"));
		mp->light->setScale9Enabled(true);
		//mp->light->setCapInsets(cocos2d::Rect(1, 1, mp->light->getVirtualRendererSize().width - 2, mp->light->getVirtualRendererSize().height - 2));
		//mp->light->setContentSize(mp->light->getContentSize()*0.8);
		addChildNode(mp->light, 0);
		if(mp->panel_1) mp->plug = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/3.png"));
		else mp->plug = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/4.png"));
		mp->plug->setScale9Enabled(true);
		//mp->plug->setCapInsets(cocos2d::Rect(1, 1, mp->plug->getVirtualRendererSize().width - 2, mp->plug->getVirtualRendererSize().height - 2));
		//mp->plug->setContentSize(mp->plug->getContentSize()*0.8);
		addChildNode(mp->plug, 1); 
		mp->draw = cocos2d::DrawNode::create();
		addChildNode(mp->draw, 1); 
		mp->draw->setPosition(cocos2d::Vec2(0, 0));
		mp->start = cocos2d::DrawNode::create();
		addChildNode(mp->start, 0); 
		mp->start->drawDot(cocos2d::Vec2(0, 0), line_width / 2, cocos2d::Color4F(line_color));
		mp->end = cocos2d::DrawNode::create();
		addChildNode(mp->end, 0); 
		mp->end->drawDot(cocos2d::Vec2(0, 0), line_width / 2, line_color);
		mp->avg = cocos2d::DrawNode::create();
		addChildNode(mp->avg, 0);
		mp->disable = false;
		if (option == Option::column) {
			if (mp->panel_1) {
				mp->light->setPosition(mp->node->getPosition() - cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->light->getContentSize().width / 2, 0));
				mp->plug->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug->getContentSize().width / 2, 0));
				mp->start->setPosition(mp->plug->getPosition() - cocos2d::Vec2(mp->plug->getContentSize().width / 2, 0));
				mp->end->setPosition(mp->plug->getPosition() - cocos2d::Vec2(mp->plug->getContentSize().width / 2, 0));
				mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			}
			else {
				mp->light->setRotation(180);
				mp->light->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->light->getContentSize().width / 2, 0));
				mp->plug->setPosition(mp->node->getPosition() - cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug->getContentSize().width / 2, 0));
				mp->start->setPosition(mp->plug->getPosition() + cocos2d::Vec2(mp->plug->getContentSize().width / 2, 0));
				mp->end->setPosition(mp->plug->getPosition() + cocos2d::Vec2(mp->plug->getContentSize().width / 2, 0));
				mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			}
		}
		else if(option == Option::row) {
			if (mp->panel_1) {
				mp->light->setRotation(90);
				mp->light->setPosition(mp->node->getPosition() + cocos2d::Vec2(0, mp->node->getContentSize().height / 2 + mp->light->getContentSize().width / 2));
				mp->plug->setRotation(90);
				mp->plug->setPosition(mp->node->getPosition() - cocos2d::Vec2(0, mp->node->getContentSize().height / 2 + mp->plug->getContentSize().width / 2));
				mp->start->setPosition(mp->plug->getPosition() + cocos2d::Vec2(0, mp->plug->getContentSize().width / 2));
				mp->end->setPosition(mp->plug->getPosition() + cocos2d::Vec2(0, mp->plug->getContentSize().width / 2));
				mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			}
			else {
				mp->light->setRotation(-90);
				mp->light->setPosition(mp->node->getPosition() - cocos2d::Vec2(0, mp->node->getContentSize().height / 2 + mp->light->getContentSize().width / 2));
				mp->plug->setRotation(90);
				mp->plug->setPosition(mp->node->getPosition() + cocos2d::Vec2(0, mp->node->getContentSize().height / 2 + mp->plug->getContentSize().width / 2));
				mp->start->setPosition(mp->plug->getPosition() - cocos2d::Vec2(0, mp->plug->getContentSize().width / 2));
				mp->end->setPosition(mp->plug->getPosition() - cocos2d::Vec2(0, mp->plug->getContentSize().width / 2));
				mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			}
		}
		mp->start_plug_position = mp->plug->getPosition();
    }

    // touch
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(Thanh_Matching2C::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(Thanh_Matching2C::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(Thanh_Matching2C::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(Thanh_Matching2C::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

	this->scheduleUpdate();

    //drawer
    color = {
        {cocos2d::Color4F(cocos2d::Color3B::BLUE)},
        {cocos2d::Color4F(cocos2d::Color3B::GREEN)},
        {cocos2d::Color4F(cocos2d::Color3B::MAGENTA)},
        {cocos2d::Color4F(cocos2d::Color3B::BLACK)},
        {cocos2d::Color4F(cocos2d::Color3B::RED)},
        {cocos2d::Color4F(cocos2d::Color3B::GRAY)}
    };
    math::func::shuffle(color);

    //gen question
    showQuestionBox(0);
}


void Thanh_Matching2C::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (Math_Answer* answer : _answers) {
            answer->onShowInParent(_panel, 10);
        }
		touch_disable = false;
		game_end = false;
    }
}

void Thanh_Matching2C::onExplanation(bool correct) {
    if(correct)
    {
        disableControlQuiz();
        scheduleOnce([=](float dt){
            this->nextQuestion();
        }, 0.5, "complete_match");
    }
}

void Thanh_Matching2C::onComplete() {

    float delay = showConratulation(0.3);
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();;
        }
    }

    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }

    //m_delay= delay;
    CCLOG("%f delay", delay);

	for (MyPair *mp : my_pair) {
		if (mp->panel_1) {
			if (gameType == GameType::OG || gameType == GameType::OO) {
				((Math_Object*)mp->node)->onHide(m_delay+0.2, math::animation::ScaleUpAndRotation);
			}
			else {
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=]() {
					((Math_Group*)mp->node)->setDisable();
				}), nullptr));
			}
		}else {
			if (gameType == GameType::GO || gameType == GameType::OO) {
				((Math_Object*)mp->node)->onHide(m_delay + 0.2, math::animation::ScaleUpAndRotation);
			}
			else {
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=]() {
					((Math_Group*)mp->node)->setDisable();
				}), nullptr));
			}
		}
		mp->draw->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
			mp->draw->clear();
			mp->draw->removeFromParentAndCleanup(true);
		}), nullptr));
		mp->start->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
			mp->start->clear();
			mp->start->removeFromParentAndCleanup(true);
		}), nullptr));
		mp->avg->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
			mp->avg->clear();
			mp->avg->removeFromParentAndCleanup(true);
		}), nullptr));
		mp->end->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
			mp->end->clear();
			mp->end->removeFromParentAndCleanup(true);
		}), nullptr));
		mp->light->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.6), cocos2d::CallFunc::create([=]() {
			mp->light->removeFromParentAndCleanup(true);
		}), nullptr));
		mp->plug->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.6), cocos2d::CallFunc::create([=]() {
			mp->plug->removeFromParentAndCleanup(true);
		}), nullptr));
	} 

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void Thanh_Matching2C::onAnswerDone(bool correct)
{
    bool done=true;
    for (int i = 0; i < status.size(); i++) {
        if(status[i] != 0) done = false;
    }
    this->onExplanation(true);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_Matching2C::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Thanh_Matching2C::autoTestingRun(float dt){

}

#pragma mark - gen Group & Object
void Thanh_Matching2C::genObjectGroup()
{
    cocos2d::Rect left_rect = cocos2d::Rect(0, 0.05, 0.5, 0.75);
    cocos2d::Rect right_rect = cocos2d::Rect(0.5, 0.05, 0.5, 0.75);
    switch (gameType) {
        case GameType::OG:
        {
            float delta = group_size.width - object_size.width;
            if (type_group_1 == math::group::frame) {
                delta = frame_col_1*ele_group_1_size.width*1.2 - object_size.width;
            }
            delta = delta/900.0;
            left_rect.origin.x = left_rect.origin.x - delta/4;
            right_rect.origin.x = right_rect.origin.x - delta/4;
        }
            break;
        case GameType::GO:
        {
            float delta = group_size.width - object_size.width;
            if (type_group_1 == math::group::frame) {
                delta = frame_col_1*ele_group_1_size.width*1.2 - object_size.width;
            }
            delta = delta/900.0;
            left_rect.origin.x = left_rect.origin.x + delta/4;
            right_rect.origin.x = right_rect.origin.x + delta/4;
        }
            break;
        default:
            break;
    }

    cocos2d::Rect top_rect = cocos2d::Rect(0, 0.6, 1, 0.3);
    cocos2d::Rect bot_rect = cocos2d::Rect(0, 0.1, 1, 0.3);

    switch (gameType) {
        case GameType::OO:
        {
            objects_1 = createObjectList(image_list_1, object_option_1);
            if(option == Option::row) math::func::setPositionList(objects_1, number_pair, top_rect);
            else math::func::setPositionList(objects_1, 1, left_rect);

            // column b
            objects_2 = createObjectList(image_list_2, object_option_2);
            if(option == Option::row) math::func::setPositionList(objects_2, number_pair, bot_rect);
            else math::func::setPositionList(objects_2, 1, right_rect);
        }
            break;
        case GameType::GG:
        {
            // column a
            groups_1 = createGroupList(image_list_1, img_group_1, type_group_1, ele_group_1_size, frame_col_1, frame_row_1);
            if(option == Option::row) math::func::setPositionList(groups_1, number_pair, top_rect);
            else math::func::setPositionList(groups_1, 1, left_rect);

            // column b
            groups_2 = createGroupList(image_list_2, img_group_2, type_group_2, ele_group_2_size, frame_row_2, frame_col_2);
            if(option == Option::row) math::func::setPositionList(groups_2, number_pair, bot_rect);
            else math::func::setPositionList(groups_2, 1, right_rect);
        }
            break;
        case GameType::OG:
        {
            objects_1 = createObjectList(image_list_1, object_option_1);
            if(option == Option::row) math::func::setPositionList(objects_1, number_pair, top_rect);
            else math::func::setPositionList(objects_1, 1, left_rect);

            // column b
            groups_1 = createGroupList(image_list_1, img_group_1, type_group_1, ele_group_1_size, frame_col_1, frame_row_1);
            if(option == Option::row) math::func::setPositionList(groups_1, number_pair, bot_rect);
            else math::func::setPositionList(groups_1, 1, right_rect);
        }
            break;
        default:
        {
            // column a
            groups_1 = createGroupList(image_list_1, img_group_1, type_group_1, ele_group_1_size, frame_col_1, frame_row_1);
            if(option == Option::row) math::func::setPositionList(groups_1, number_pair, top_rect);
            else math::func::setPositionList(groups_1, 1, left_rect);

            objects_1 = createObjectList(image_list_1, object_option_1);
            if(option == Option::row) math::func::setPositionList(objects_1, number_pair, bot_rect);
            else math::func::setPositionList(objects_1, 1, right_rect);
        }
            break;
    }
}

std::vector<Math_Group*> Thanh_Matching2C::createGroupList(std::vector<math::resource::Image*> list_num, math::resource::Image* img, math::group::GroupType type, cocos2d::Size element_size, int col, int row)
{
    std::vector<Math_Group*> vector_group;
    for(int k = 0; k < list_num.size(); k++)
    {
        Math_Group* group=mpool->getGroup();
        std::vector<Math_Object*> _group_list;
        for(int i = 0; i < list_num[k]->getComparisionValue("number"); i++){
            Math_Object* object = mpool->getObject();
            object->setEnable(obj_id++, math::common::deny , img, img_bg);
            object->setBackgroundVisible(false);
			object->getLabelValue()->disableEffect();
            object->setObjectSize(element_size);
            _group_list.push_back(object);
        }

        group->setEnable(gr_id++, _group_list, type, col*row);
        group->setObjectSize(element_size);

        if(type == math::group::frame) {
            group->setGroupFrames(col, row, math::resource::ResHelper::createImage(getUIPath("")));
			group->setBackground(group_bg, 20);
        }
		else if (type == math::group::custom) {
			group->setAlignObjects(math::group::GroupAlign::random);
			group->setGroupSize(cocos2d::Size(group_size.width + 30, group_size.height+30));
			group->setBackground(group_bg, 20);
			group->updatePosition( false, false );
		}
		else if (type == math::group::queue) { 
			group->setGroupQueue(cocos2d::Size(element_size.width*1.2*col + 30, element_size.height*1.2*row + 30));
			group->setBackground(group_bg, 20);
		}
        _groups.push_back(group);
        vector_group.push_back(group);
        objects.push_back(group);
        status.push_back(1);
    }
    math::func::shuffle(vector_group);
    return vector_group;
}

std::vector<Math_Object*> Thanh_Matching2C::createObjectList(std::vector<math::resource::Image*> img_list, ObjectOption obj_option)
{
    std::vector<Math_Object*> vector_object;
    for(math::resource::Image* img: img_list)
    {
        Math_Object* obj=mpool->getObject();
        obj->setEnable(obj_id++, math::common::deny, img, img_bg);
        obj->setObjectSize(object_size); 

        switch (obj_option) {
            case ObjectOption::text:
            {
                std::string key = "name_1";
                if (obj->getComparisionValue().number >= 0) {
                    key = "name_n";
                }
                obj->setImageVisible(false);
                obj->setValueText(img->getText(key, m_lang->getLanguage()));
                obj->fitValueTextSize();
                obj->showValueTextWithAnimation(true);
                break;
            }
            case ObjectOption::number:
            {
                obj->setImageVisible(false);
                obj->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
                obj->fitValueTextSize();
                obj->showValueTextWithAnimation(true);
                obj->getLabelValue()->setFontSize(object_size.height*0.75);
                break;
            }
            case ObjectOption::ordinal:
            {
                obj->setImageVisible(false);
                obj->setValueText(getOrdinals(img->getComparisionValue("number"), img->getComparisionValue("number")).at(0)->getText("name_n", m_lang->getLanguage()));
                obj->fitValueTextSize();
                obj->showValueTextWithAnimation(true);
                obj->getLabelValue()->setFontSize(object_size.height*0.5);
                break;
            }
			case ObjectOption::roman:
			{ 
				obj->setImageVisible(false);
				obj->setValueText(math::RomanNumeralsUtils::int_2_roman(img->getComparisionValue("number")));
				obj->fitValueTextSize();
				obj->showValueTextWithAnimation(true);
				obj->getLabelValue()->setFontSize(object_size.height*0.5);
				break;
			}
                // image k can lam gi

            default:
                break;
        }

		obj->getLabelValue()->disableEffect();
        _objects.push_back(obj);
        vector_object.push_back(obj);
        objects.push_back(obj);
        status.push_back(1);
    }
    math::func::shuffle(vector_object);
    return vector_object;
}

#pragma mark - Add Event

void Thanh_Matching2C::update(float dt) { 
	if (game_end) return; 
	for (MyPair *mp : my_pair) {
		if (mp != nullptr && mp->panel_1 == true) {
			mp->draw->clear();
			mp->draw->setLineWidth(line_width);
			cocos2d::Vec2 end_pos;
			if (option == Option::column) { 
				end_pos = mp->plug->getPosition() - cocos2d::Vec2(mp->plug->getContentSize().width / 2, 0);
			}
			else { 
				end_pos = mp->plug->getPosition() + cocos2d::Vec2(0, mp->plug->getContentSize().width / 2);
			}
			if (mp->start->getPosition() + end_pos == 2 * mp->avg->getPosition()) CCLOG("true");
			else CCLOG("false");
			mp->draw->drawQuadBezier(mp->start->getPosition(), mp->avg->getPosition(), end_pos, 50, line_color);
		}
	}
}

bool Thanh_Matching2C::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);
	if (touch_disable) return false;
    touch_to_plug = false;
	touch_to_image = false;
    for(MyPair *mp: my_pair){
		if (mp->panel_1 == true && mp->disable == false) {
			cocos2d::Vec2 locationInNode = mp->plug->convertToNodeSpace(touch->getLocation());
			cocos2d::Size size = mp->plug->getBoundingBox().size;
			cocos2d::Rect rect;
			if (option == Option::column) {
				rect = cocos2d::Rect(0, 0, size.width, size.height);
			}
			else {
				rect = cocos2d::Rect(0, 0, size.height, size.width);
			}
			if (rect.containsPoint(locationInNode)) {
				current_mp = mp;
				mp->plug->setLocalZOrder(4);
				mp->draw->setLocalZOrder(3);
				touch_to_plug = true;
				return true;
			}
		}
		if (mp->disable == false) {
			cocos2d::Vec2 locationInNode = mp->node->convertToNodeSpace(touch->getLocation());
			cocos2d::Size size = mp->node->getBoundingBox().size;
			cocos2d::Rect rect;
			if (option == Option::column) {
				rect = cocos2d::Rect(0, 0, size.width, size.height);
			}
			else {
				rect = cocos2d::Rect(0, 0, size.height, size.width);
			}
			if (rect.containsPoint(locationInNode)) {
				current_mp = mp;
				mp->node->setScale(1.1); 
				touch_to_image = true;
				return true;
			}
		}
    } 
    return false;
}

void Thanh_Matching2C::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (touch_disable) return;
	if (touch_to_plug) {
		current_mp->plug->setPosition(_panel->convertToNodeSpace(touch->getLocation()));
		if (option == Option::column) {
			if (current_mp->panel_1) {
				current_mp->end->setPosition(current_mp->plug->getPosition() - cocos2d::Vec2(current_mp->plug->getContentSize().width / 2, 0));
				current_mp->avg->setPosition((current_mp->start->getPosition() + current_mp->end->getPosition()) / 2);
			}
			else {
				current_mp->end->setPosition(current_mp->plug->getPosition() + cocos2d::Vec2(current_mp->plug->getContentSize().width / 2, 0));
				current_mp->avg->setPosition((current_mp->start->getPosition() + current_mp->end->getPosition()) / 2);
			}
		}
		else if (option == Option::row) {
			if (current_mp->panel_1) {
				current_mp->end->setPosition(current_mp->plug->getPosition() + cocos2d::Vec2(0, current_mp->plug->getContentSize().width / 2));
				current_mp->avg->setPosition((current_mp->start->getPosition() + current_mp->end->getPosition()) / 2);
			}
			else {
				current_mp->end->setPosition(current_mp->plug->getPosition() - cocos2d::Vec2(0, current_mp->plug->getContentSize().width / 2));
				current_mp->avg->setPosition((current_mp->start->getPosition() + current_mp->end->getPosition()) / 2);
			}
		}
		return;
	} 
}


void Thanh_Matching2C::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (touch_disable) return;
    if(touch_to_plug){
		touch_disable = true;
		MyPair *end_pair = nullptr;
		for (MyPair *mp : my_pair) {
			if (mp->panel_1 == false) {
				cocos2d::Vec2 locationInNode = mp->node->convertToNodeSpace(touch->getLocation());
				cocos2d::Size size = mp->node->getBoundingBox().size;
				cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
				if (rect.containsPoint(locationInNode)) {
					end_pair = mp;
					break;
				}
				locationInNode = mp->plug->convertToNodeSpace(touch->getLocation());
				size = mp->plug->getBoundingBox().size;
				rect = cocos2d::Rect(0, 0, size.width, size.height);
				if (rect.containsPoint(locationInNode)) {
					end_pair = mp;
					break;
				}
			}
		}
		if (end_pair != nullptr && current_mp != end_pair) {
			int a = -1, b = -2;
			bool ok = false;
			if (gameType == GameType::OO) { 
				Math_Object *obja = (Math_Object*)current_mp->node;
				Math_Object *objb = (Math_Object*)end_pair->node;
				if (obja->isEqual(objb, compare_key)) {
					ok = true;
				}
			}
			switch (gameType)
			{
			case Thanh_Matching2C::OO:
				a = ((Math_Object*)current_mp->node)->getComparisionValue().number;
				b = ((Math_Object*)end_pair->node)->getComparisionValue().number;
				break;
			case Thanh_Matching2C::GO:
				a = ((Math_Group*)current_mp->node)->getNumberObjects();
				b = ((Math_Object*)end_pair->node)->getComparisionValue().number;
				break;
			case Thanh_Matching2C::OG:
				a = ((Math_Object*)current_mp->node)->getComparisionValue().number;
				b = ((Math_Group*)end_pair->node)->getNumberObjects();
				break;
			case Thanh_Matching2C::GG:
				a = ((Math_Group*)current_mp->node)->getNumberObjects();
				b = ((Math_Group*)end_pair->node)->getNumberObjects();
				break;
			default:
				break;
			} 
			if ((make_sum && a + b == sum_value) || (!make_sum && gameType != GameType::OO && a == b) || (!make_sum && ok)) {
				cocos2d::Vec2 pos;
				if (option == Option::column) {
					pos = end_pair->plug->getPosition() - cocos2d::Vec2(end_pair->plug->getContentSize().width / 2 + current_mp->plug->getContentSize().width / 4, 0);
				}
				else {
					pos = end_pair->plug->getPosition() + cocos2d::Vec2(0, end_pair->plug->getContentSize().width / 2 + current_mp->plug->getContentSize().width / 4);
				}
				float dt = cocos2d::ccpDistance(touch->getLocation(), pos) / 400;
				cocos2d::EaseElasticOut *action;
				if(option == Option::column){
					auto avg_pos = (current_mp->start->getPosition() + end_pair->plug->getPosition() - cocos2d::Vec2(end_pair->plug->getContentSize().width / 2 + 3* current_mp->plug->getContentSize().width / 4, 0)) / 2;
					action = cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(3*dt, avg_pos), 0.3);
				}else {
					auto avg_pos = (current_mp->start->getPosition() + end_pair->plug->getPosition() + cocos2d::Vec2(0, end_pair->plug->getContentSize().width / 2 + 3* current_mp->plug->getContentSize().width / 4)) / 2;
					action = cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(3*dt, avg_pos), 0.3); 
				} 
				current_mp->avg->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.2), action, nullptr));
				current_mp->plug->runAction(cocos2d::MoveTo::create(dt/2, pos)); 
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=]() {
					touch_disable = false;
					touch_to_plug = false;
					current_mp->draw->setLocalZOrder(0);
					current_mp->plug->setLocalZOrder(0); 
					current_mp->light->loadTexture(getUIPath("gameui/electriclight/1.png"));
					end_pair->light->loadTexture(getUIPath("gameui/electriclight/1.png")); 
					//current_mp->light->setCapInsets(cocos2d::Rect(1, 1, current_mp->plug->getVirtualRendererSize().width/0.8 - 2, current_mp->plug->getVirtualRendererSize().height / 0.8 - 2));
					//end_pair->light->setCapInsets(cocos2d::Rect(1, 1, end_pair->plug->getVirtualRendererSize().width / 0.8 - 2, end_pair->plug->getVirtualRendererSize().height / 0.8 - 2));
					current_mp->disable = true;
					end_pair->disable = true; 
				}), cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() { 
					bool done = true;
					for (MyPair *mp : my_pair) {
						if (mp->disable == false) {
							done = false;
						}
					}
					if (done) {
						onExplanation(true);
						game_end = true;
					}
				}), nullptr));
				return;
			} 
		}
		float dt = cocos2d::ccpDistance(touch->getLocation(), current_mp->start_plug_position) / 400;
		current_mp->avg->setPosition(current_mp->start->getPosition());
		current_mp->end->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(dt, current_mp->start->getPosition()), 0.5));
		current_mp->plug->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(dt, current_mp->start_plug_position), 0.5));
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt/2), cocos2d::CallFunc::create([=]() {
			current_mp->plug->setLocalZOrder(1);
			current_mp->draw->setLocalZOrder(0);
			touch_disable = false;
		}), nullptr));
		return; 
    }
    if(touch_to_image){
		cocos2d::Vec2 locationInNode = current_mp->node->convertToNodeSpace(touch->getLocation());
		cocos2d::Size size = current_mp->node->getBoundingBox().size;
		cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(locationInNode)) {
			if (mp1 == nullptr) mp1 = current_mp;
			else mp2 = current_mp;
		}
		else {
			current_mp->node->setScale(1);
		}
		if (mp1 != nullptr && mp2 != nullptr) {
			mp1->node->setScale(1);
			mp2->node->setScale(1);
			if (mp1 == mp2 || mp1->panel_1 == mp2->panel_1) return;
			if (mp2->panel_1) std::swap(mp1, mp2); 
			int a = -1, b = -2;
			bool ok = false;
			switch (gameType)
			{
			case Thanh_Matching2C::OO:
				a = ((Math_Object*)mp1->node)->getComparisionValue().number;
				b = ((Math_Object*)mp2->node)->getComparisionValue().number;
				if (((Math_Object*)mp1->node)->isEqual(((Math_Object*)mp2->node), compare_key)) ok = true;
				break;
			case Thanh_Matching2C::GO:
				a = ((Math_Group*)mp1->node)->getNumberObjects();
				b = ((Math_Object*)mp2->node)->getComparisionValue().number;
				break;
			case Thanh_Matching2C::OG:
				a = ((Math_Object*)mp1->node)->getComparisionValue().number;
				b = ((Math_Group*)mp2->node)->getNumberObjects();
				break;
			case Thanh_Matching2C::GG:
				a = ((Math_Group*)mp1->node)->getNumberObjects();
				b = ((Math_Group*)mp2->node)->getNumberObjects();
				break;
			default:
				break;                                                                                                                                                                                                                                                                                                                                                                                                                                       
			}
			if ((make_sum && a + b == sum_value) || (!make_sum && a == b) || (!make_sum && ok)) {
				current_mp = mp1;
				cocos2d::Vec2 pos;
				if (option == Option::column) {
					pos = mp2->plug->getPosition() - cocos2d::Vec2(mp2->plug->getContentSize().width / 2 + mp1->plug->getContentSize().width / 4, 0);
				}
				else {
					pos = mp2->plug->getPosition() + cocos2d::Vec2(0, mp2->plug->getContentSize().width / 2 + mp1->plug->getContentSize().width / 4);
				}
				float dt = cocos2d::ccpDistance(touch->getLocation(), pos) / 500;
				if (option == Option::column) {
					auto avg_pos = (mp1->start->getPosition() + mp2->plug->getPosition() - cocos2d::Vec2(mp2->plug->getContentSize().width / 2 + 3* mp1->plug->getContentSize().width / 4, 0)) / 2;
					mp1->avg->runAction(cocos2d::MoveTo::create(dt, avg_pos));
				}
				else {
					auto avg_pos = (mp1->start->getPosition() + mp2->plug->getPosition() - cocos2d::Vec2(0, mp2->plug->getContentSize().width / 2 + 3* mp1->plug->getContentSize().width / 4)) / 2;
					mp1->avg->runAction(cocos2d::MoveTo::create(dt, avg_pos));
				}
				mp1->end->setVisible(false);
				mp1->plug->runAction(cocos2d::MoveTo::create(dt, pos));
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=]() {
					touch_disable = false;
					touch_to_image = false;
					mp1->draw->setLocalZOrder(0);
					mp1->plug->setLocalZOrder(0);
					mp1->light->loadTexture(getUIPath("gameui/electriclight/1.png"));
					mp2->light->loadTexture(getUIPath("gameui/electriclight/1.png")); 
					//mp1->light->setCapInsets(cocos2d::Rect(1, 1, mp1->plug->getVirtualRendererSize().width / 0.8 - 2, mp1->plug->getVirtualRendererSize().height / 0.8 - 2));
					//mp2->light->setCapInsets(cocos2d::Rect(1, 1, mp2->plug->getVirtualRendererSize().width / 0.8 - 2, mp2->plug->getVirtualRendererSize().height / 0.8 - 2));
					mp1->disable = true;
					mp2->disable = true;
					mp1 = nullptr;
					mp2 = nullptr; 
				}), cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=]() { 
					bool done = true;
					for (MyPair *mp : my_pair) {
						if (mp->disable == false) {
							done = false;
						}
					}
					if (done) {
						onExplanation(true);
						game_end = true;
					}
				}), nullptr));
				return;
			}
			mp1 = nullptr;
			mp2 = nullptr;
		} 
    }  
}


void Thanh_Matching2C::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event)
{
    drawer[index]->clear();
    setSelected(false, objects[index]);
    if(pass >= 0) {
        setSelected(false, objects[pass]);
        pass = -1;
    }
}

#pragma mark - on this game
bool Thanh_Matching2C::isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node *object){
    cocos2d::Vec2 locationInNode = object->convertToNodeSpace(position);
    cocos2d::Size size = object->getBoundingBox().size;
    cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

    Math_Object* _obj = (Math_Object*)object;
    _obj->setSelected(false);
    if (rect.containsPoint(locationInNode)) return true;
    return false;
}

void Thanh_Matching2C::setSelected(bool selected, cocos2d::Node* object)
{
    if(selected == true)
    {
        object->setScale(1.1);
        object->setLocalZOrder(4);
    }
    else
    {
        object->setScale(1.0);
        object->setLocalZOrder(2);
    }
}

void Thanh_Matching2C::connect(int i, int j)
{
    cocos2d::Vec2 pos_a;
    cocos2d::Vec2 pos_b;
    if(i>j) std::swap(i,j);
    switch (option) {
        case Option::column:
        {
            pos_a = getPositionDraw(i, Direction::right);
            pos_b = getPositionDraw(j, Direction::left);
            break;
        }
        case Option::row:
        {
            pos_a = getPositionDraw(i, Direction::bottom);
            pos_b = getPositionDraw(j, Direction::top);
            break;
        }
        default:
            break;
    }
    int index_color = i/number_pair!=1?i:j;
    drawer[i]->clear();
    drawer[j]->clear();
    drawer[index_color]->drawSegment(pos_a, pos_b, line_width, color[index_color%number_pair]);
    //    drawer[index_color]->drawDot(pos_a, 5, color[index_color%number_pair]);
    //    drawer[index_color]->drawDot(pos_b, 5, color[index_color%number_pair]);
    drawer[index_color]->setLineWidth(5);
    drawer[index_color]->setLocalZOrder(1);
    status[i]--;
    status[j]--;

    number_line_drawed++;
}

cocos2d::Vec2 Thanh_Matching2C::getPositionDraw(int i, Direction direction)
{
    switch(direction)
    {
        case Direction::left:
            return objects[i]->getPosition() - cocos2d::Vec2(objects[i]->getContentSize().width/2, 0);
        case Direction::right:
            return objects[i]->getPosition() + cocos2d::Vec2(objects[i]->getContentSize().width/2, 0);
        case Direction::top:
            return objects[i]->getPosition() + cocos2d::Vec2(0, objects[i]->getContentSize().height/2);
        case Direction::bottom:
            return objects[i]->getPosition() - cocos2d::Vec2(0, objects[i]->getContentSize().height/2);
        default:
            return cocos2d::Vec2(0, 0);
    }
}

bool Thanh_Matching2C::isCorrect(int i, int j)
{
    if(i == j) return false;
    if (gameType == OO) {
        Math_Object* obj_a = (Math_Object*)objects[i];
        Math_Object* obj_b = (Math_Object*)objects[j];
        if (make_sum) {
            return (obj_a->getComparisionValue().number + obj_b->getComparisionValue().number) == sum_value;
        } else {
            return obj_a->isEqual(obj_b, compare_key);
        }
    } else {
        int a, b;
        // lay gia tri node i
        a = getValue(i);

        // lay gia tri node j
        b = getValue(j);

        if (make_sum) {
            return a + b ==sum_value;
        } else {
            return (a == b);
        }
    }
}

int Thanh_Matching2C::getValue(int i)
{
    for(Math_Group *gr : _groups)
    {
        if(gr == (Math_Group*)objects[i]) return gr->getNumberObjects();
    }
    for(Math_Object *obj : _objects)
    {
        if(obj == (Math_Object*)objects[i]) return obj->getComparisionValue().number;
    }
    return -1;
}
