#include "Ufo.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"

std::once_flag Ufo_Reader;
INITIALIZE_READER(Ufo);

using namespace cocos2d;

#define CSB_NAME "csb/game/UFO/ufo.csb"

#define SOUND_BG_START "sounds/ufo/sfx/ufo_bgstart.mp3"
#define SOUND_BG_LOOP "sounds/ufo/sfx/ufo_bgloop.mp3"
#define SOUND_BGM "sounds/ufo/sfx/ufo_bgm.mp3"
#define SOUND_QUESTION "sounds/ufo/Choose the correct sheep to complete the word .mp3"
#define SOUND_ACTION "sounds/ufo/sfx/ufo_action.mp3"
#define SOUND_CORRECT "sounds/ufo/sfx/ufo_correct.mp3"
#define SOUND_FADE_OUT "sounds/ufo/sfx/ufo_fadeout.mp3"
#define SOUND_WRONG "sounds/ufo/sfx/ufo_wrong.mp3"

#define GAF_BLACKHOLE "gaf/ufo/UFO_blackhole/UFO_blackhole.gaf"
#define GAF_ALIEN "gaf/ufo/UFO_alien/UFO_alien.gaf"
#define GAF_ALIEN_LIGHT "gaf/ufo/UFO_alienlight/UFO_alienlight.gaf"
#define GAF_TEXTHOLDER "gaf/ufo/UFO_textholder/UFO_textholder.gaf"
#define GAF_LIGHT1 "gaf/ufo/UFO_light1/UFO_light1.gaf"
#define GAF_LIGHT2 "gaf/ufo/UFO_light2/UFO_light2.gaf"
#define GAF_SHEEP "gaf/ufo/UFO_sheep/UFO_sheep.gaf"
#define GAF_FIREWORK "gaf/ufo/DtD_stars/DtD_stars.gaf"

Ufo* Ufo::createGame(std::string json_file, bool is_auto_play) {
	std::call_once(Ufo_Reader, [] {
		REGISTER_CSB_READER(Ufo);
	});
	auto p = static_cast<Ufo*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p)
	{
		p->didLoadFromCSB(json_file);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

Ufo * Ufo::createGame(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type) {
	std::call_once(Ufo_Reader, [] {
		REGISTER_CSB_READER(Ufo);
	});
	auto p = static_cast<Ufo *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p)
	{
		p->didLoadFromCSB(list_phonic_info, is_auto_play);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void Ufo::didLoadFromCSB(std::string json_file) {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && document.HasMember("audio_intro_vi")){
        auto audio_intro = document["audio_intro_vi"].GetArray();
        for (int i = 0; i < (int)audio_intro.Size(); i++) {
            auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
            question_vi = img_audio->getAudios("name_1").front();
        }
    }
    
	rapidjson::Value& json_data = document.FindMember("data")->value;
	for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
		Ufo_Data ufo_data;
		auto letter = json_data[i].FindMember("text")->value.FindMember("text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		ufo_data.text = pImage->getText("name_1");
		ufo_data.image_link = pImage->path;
		ufo_data.n_sound = pImage->getAudios("name_1").front();
		ufo_data.p_sound = pImage->getAudios("name_n").front();


		rapidjson::Value& json_list_answer = json_data[i].FindMember("answers")->value;
		auto right_ans = json_list_answer.FindMember("right_answer")->value.GetString();
		auto right_img = math::resource::ResHelper::getImage(images, right_ans);
		ufo_data.right_answer.text = right_img->getText("name_1");
		ufo_data.right_answer.audio_path = right_img->getAudios("name_1").front();
		ufo_data.right_sub_text = (right_img->getText("name_n") != "") ? right_img->getText("name_n") : right_img->getText("name_1");
		ufo_data.answer.push_back(ufo_data.right_answer);

		rapidjson::Value& wrong_answer = json_list_answer.FindMember("wrong_answers")->value;
		for (rapidjson::SizeType j = 0; j < wrong_answer.Size(); ++j) {
			auto wrong_ans = wrong_answer[j].GetString();
			auto aImage = math::resource::ResHelper::getImage(images, wrong_ans);
			mj::GameObject go;
			go.text = aImage->getText("name_1");
			go.audio_path = aImage->getAudios("name_1").front();
			go.image_path = aImage->path;
			ufo_data.answer.push_back(go);
		}

		ufo_data.number = (int)ufo_data.answer.size();

		//for position list
		auto pos_t = ufo_data.text.find(ufo_data.right_sub_text);
		auto sub_str = ufo_data.text.substr(pos_t);
        auto sub_str_utf8 = StringUtils::StringUTF8(sub_str);
        auto right_answer_utf8 = StringUtils::StringUTF8(ufo_data.right_answer.text);
        for (int k = 0; k < sub_str_utf8.length(); k++) {
			for (int j = 0; j < right_answer_utf8.length(); j++) {
                auto letter = right_answer_utf8.getAsCharSequence(j,1);
                if (sub_str_utf8.getAsCharSequence
                    (k,1) == letter) {
					ufo_data.position.push_back(pos_t + k);
				}
			}
		}

		list_data.push_back(ufo_data);
	}

	mj::helper::shuffle(list_data);

	number_stage = list_data.size();
	this->loadFrame();
}

void Ufo::didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play) {
	_list_phonic_info = list_phonic_info;
	this->is_auto_play = is_auto_play;
	screen_size = Director::getInstance()->getVisibleSize();

	this->loadFrame();
}

void Ufo::loadFrame() {
	screen_size = Director::getInstance()->getVisibleSize();
	auto ufo = utils::findChild(this, "ufo");
	auto text_pos = utils::findChild(this, "text");
	root_layout = utils::findChild<ui::Layout*>(this, "root_layout");
	panel = utils::findChild<ui::Layout*>(this, "panel");
	panel->setVisible(false);

    auto bg_width = utils::findChild(this,"UFO_bg_1")->getContentSize().width;
    if(bg_width < screen_size.width)
    {
        utils::findChild(this, "UFO_bg_1")->setScaleX(bg_width/screen_size.width);
        utils::findChild(this, "UFO_trees_2")->setScaleX(bg_width/screen_size.width);
    }
    
	blackhole = GAFWrapper::createGAF(GAF_BLACKHOLE);
	root_layout->addChild(blackhole);
	blackhole->setPosition(ufo->getPosition());
	blackhole->play(true);
	blackhole->setScale(0);

	alien = GAFWrapper::createGAF(GAF_ALIEN);
	root_layout->addChild(alien, 10000);
	alien->setPosition(Vec2(ufo->getPosition().x - 80, ufo->getPosition().y - 150));


	alien_light = GAFWrapper::createGAF(GAF_ALIEN_LIGHT);
	root_layout->addChild(alien_light, 100);

	textholder = GAFWrapper::createGAF(GAF_TEXTHOLDER);
	root_layout->addChild(textholder);
	textholder->setPosition(Vec2(screen_size.width / 2, text_pos->getPosition().y - 50));
	textholder->setVisible(false);

	trans_layout = cocos2d::ui::Layout::create();
	trans_layout->setContentSize(screen_size);
	trans_layout->setTouchEnabled(true);
	root_layout->addChild(trans_layout, 500);
	trans_layout->setBackGroundColor(Color3B::BLACK);
	trans_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	trans_layout->setOpacity(150);
	trans_layout->setVisible(false);

	onBeginStage();

}

void Ufo::onEnter() {
	MJMainGBase::onEnter();
	
	this->playBackgroundMusic(SOUND_BGM);
	
}

void Ufo::onBeginStage() {
	auto text_pos = utils::findChild(this, "text");
	panel->setVisible(false);

	alien->setLocalZOrder(10000);

	list_text.clear();
    
    StringUtils::StringUTF8 textStr = StringUtils::StringUTF8(list_data[stage_count].text);

    for (int i = 0; i < textStr.getString().size(); i++)
    {
        auto newChar = textStr.getString().at(i)._char;
        
        //MS hien ko co font -> nen h xai chung font VM luon, ko xai font mac dinh cua he thong nua.
        ui::Text* text = ui::Text::create(newChar,PHONIC_MANAGER->getGameFontName(),120);
        list_text.push_back(text);
        root_layout->addChild(text, 1000);
        text->setVisible(false);
        text->addClickEventListener(CC_CALLBACK_1(Ufo::onClickText, this));
        text->setTouchEnabled(true);
	}
    
	setTextPosition(list_text, Vec2(screen_size.width / 2, text_pos->getPosition().y));

	list_light.clear();
	for (int i = 0; i < list_text.size(); i++) {
		GAFWrapper* light = GAFWrapper::createGAF(GAF_LIGHT1);
		root_layout->addChild(light);
		light->setPosition(list_text[i]->getPosition());
		list_light.push_back(light);
	}

	list_firework.clear();
	for (int i = 0; i < list_text.size(); i++) {
		GAFWrapper* firework = GAFWrapper::createGAF(GAF_FIREWORK);
		root_layout->addChild(firework);
		firework->setPosition(list_text[i]->getPosition());
		firework->setVisible(false);
		list_firework.push_back(firework);
	}

	list_light_2.clear();
	for (int i = 0; i < list_data[stage_count].position.size(); i++) {
		GAFWrapper* light_2 = GAFWrapper::createGAF(GAF_LIGHT2);
		light_2->setScale(0.3, 1);
		root_layout->addChild(light_2);
		light_2->setPosition(Vec2(list_text[list_data[stage_count].position[i]]->getPosition().x, list_text[list_data[stage_count].position[i]]->getPosition().y - 60));
		list_light_2.push_back(light_2);
	}


	sheeps.clear();
	number_answer = list_data[stage_count].number;
	for (int i = 0; i < list_data[stage_count].answer.size(); i++) {
		auto gaf = GAFWrapper::createGAF(GAF_SHEEP);
		Ufo_Sheep* sheep = new Ufo_Sheep(list_data[stage_count].answer[i].text, gaf);
		sheep->getLayout()->setTouchEnabled(true);
		sheep->getLayout()->addClickEventListener(CC_CALLBACK_1(Ufo::onClickSheep, this));
		sheep->setAudio(list_data[stage_count].answer[i].audio_path);
		if (i == 0) {
			right_answer = sheep;
		}
		sheeps.push_back(sheep);
	}
	mj::helper::shuffle(sheeps);

	for (Ufo_Sheep* sheep : sheeps) {
		root_layout->addChild(sheep);
	}
	setSheepsPosition(sheeps);
	for (int i = 0; i < sheeps.size(); i++) {
		if (i < sheeps.size() / 2) {
			sheeps[i]->setPosition(Vec2(sheeps[i]->getPosition().x - 700, sheeps[i]->getPosition().y));
		}
		else {
			sheeps[i]->setPosition(Vec2(sheeps[i]->getPosition().x + 700, sheeps[i]->getPosition().y));
			sheeps[i]->getSheep()->setFlippedX(true);
			sheeps[i]->getText()->setPosition(Vec2(20, 0));
		}
	}

	this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
		for (int i = 0; i < sheeps.size(); i++) {
			if (i < sheeps.size() / 2) {
				sheeps[i]->runAction(MoveBy::create(2, Vec2(700, 0)));
			}
			else {
				sheeps[i]->runAction(MoveBy::create(2, Vec2(-700, 0)));
			}
			sheeps[i]->getSheep()->playSequence("walk", true);
		}
		blackhole->runAction(ScaleTo::create(0.5, 1));
	}), DelayTime::create(0.5), CallFunc::create([=] {
		alien->playSequence("fade_in");
		AUDIO_ENGINE->play((CONFIG_MANAGER->isGameVMonkey() && !question_vi.empty()) ? question_vi:SOUND_QUESTION);
	}), DelayTime::create(0.8), CallFunc::create([=] {
		//image->runAction(ScaleTo::create(0.5, 1));
		textholder->setVisible(true);
		textholder->playSequence("fade_in");
	}), DelayTime::create(0.25), CallFunc::create([=] {
		for (int i = 0; i < list_text.size(); i++) {
			list_text[i]->setVisible(true);
			this->runAction(Sequence::create(DelayTime::create(i * 0.1), CallFunc::create([=] {
				list_light[i]->playSequence("light");
			}), NULL));

			for (int j = 0; j < list_data[stage_count].position.size(); j++) {
				if (i == list_data[stage_count].position[j]) {
					list_light_2[j]->playSequence("light");
					list_text[i]->setVisible(false);
					this->runAction(Sequence::create(DelayTime::create(i * 0.1), CallFunc::create([=] {
						list_light[i]->playSequence("light_red", true);
					}), NULL));
				}
			}
		}
		textholder->playSequence("light_fade_in");
	}), DelayTime::create(0.25), CallFunc::create([=] {
		for (int i = 0; i < list_light_2.size(); i++) {
			list_light_2[i]->playSequence("light_red", true);
		}
		for (Ufo_Sheep* sheep : sheeps) {
			sheep->getSheep()->playSequence("look_start");
		}
		//image->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), MoveBy::create(1, Point(10, 0)), MoveBy::create(1, Point(-10, 0)), MoveBy::create(1, Point(-10, 0)), MoveBy::create(1, Point(10, 0)), NULL)));
		//image->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), RotateTo::create(1, 5), RotateTo::create(1, 0), RotateTo::create(1, -5), RotateTo::create(1, 0), NULL)));
		textholder->playSequence("light_red");
		alien->playSequence("idle", true);
		clickable = true;
		alien->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), RotateTo::create(1, 10), RotateTo::create(1, 0), RotateTo::create(1, -10), RotateTo::create(1, 0), NULL)));
		alien->runAction(Repeat::create(Sequence::create(MoveTo::create(1.5, Vec2(screen_size.width / 2 + 150, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2 - 150, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2, screen_size.height / 2)), NULL), 10000));
	}), DelayTime::create(0.25), CallFunc::create([=] {
		for (Ufo_Sheep* sheep : sheeps) {
			sheep->getSheep()->playSequence("look_end");
		}
	}), DelayTime::create(0.25), CallFunc::create([=] {
		for (Ufo_Sheep* sheep : sheeps) {
			sheep->getSheep()->playSequence("idle", true);
		}
		clickedText = true;
        inactiveMode(true);
		//showTapGuideGame(right_answer->getPosition());
	}), NULL));

}

std::vector<cocos2d::Vec2> Ufo::getTextPosition(std::vector<cocos2d::ui::Text*> texts, cocos2d::Vec2 point, float space)
{
	std::vector<cocos2d::Vec2> locs;
	std::vector <char> thinner = { 'f', 'i', 'j', 'l', 't', 'r', '.', ',','\'' };
	std::vector <char> thicker = { 'H', 'W', 'w', 'M', 'm', 'G', 'K', 'Q', 'O' };
	int num_thin;
	int num_thick;
	float offset = 0;
	int size = texts.size();
	for (int i = 0; i < size; ++i) {
		for (char c : thinner) {
			if (texts[i]->getString().find(c) != std::string::npos) {
				offset += space * 0.3;
				break;
			}
		}
		for (char c : thicker) {
			if (texts[i]->getString().find(c) != std::string::npos) {
				offset -= space * 0.3;
				break;
			}
		}
	}
	auto start_pos_x = point.x - (float)size / 2.0f * space + offset/2;
	
	for (int i = 0; i < size; ++i) {
		offset = space;
		for (char c : thinner) {
			if (texts[i]->getString().find(c) != std::string::npos) {
				offset -= space * 0.3;
				break;
			}
		}
		for (char c : thicker) {
			if (texts[i]->getString().find(c) != std::string::npos) {
				offset += space * 0.3;
				break;
			}
		}
		start_pos_x += offset / 2;
		locs.push_back(Vec2(start_pos_x, point.y));
		start_pos_x += offset / 2;
	}
	return locs;
}

void Ufo::setTextPosition(std::vector<ui::Text*> texts, Vec2 point) {

	auto locs = getTextPosition(texts, point, 90);
	int size = texts.size();
	for (int i = 0; i < size; i++) {
		//float x = point.x + (i - (float)size / 2 + 0.5) * 90;
		//texts[i]->setPosition(Vec2(x, point.y));
		texts[i]->setPosition(locs[i]);
	}
}

void Ufo::moveText(std::vector<ui::Text*> texts, Vec2 point) {
	auto locs = getTextPosition(texts, point, 110);
	int size = texts.size();
	for (int i = 0; i < size; i++) {
		//float x = point.x + (i - (float)size / 2 + 0.5) * 110;
		//texts[i]->runAction(MoveTo::create(0.5, Vec2(x, point.y)));
		texts[i]->runAction(MoveTo::create(0.5, locs[i]));
		texts[i]->runAction(ScaleTo::create(0.5, 1.5));
	}
}

void Ufo::setSheepsPosition(std::vector<Ufo_Sheep*> sheeps) {
	switch (sheeps.size()) {
	case 2: {
		sheeps[0]->setPosition(Vec2(screen_size.width * 0.35, screen_size.height * 0.2));
		sheeps[1]->setPosition(Vec2(screen_size.width * 0.65, screen_size.height * 0.2));
		break;
	}
	case 3: {
		sheeps[0]->setPosition(Vec2(screen_size.width * 0.25, screen_size.height * 0.25));
		sheeps[1]->setPosition(Vec2(screen_size.width * 0.5, screen_size.height * 0.2));
		sheeps[2]->setPosition(Vec2(screen_size.width * 0.75, screen_size.height * 0.25));
		break;
	}
	default:
		sheeps[0]->setPosition(Vec2(screen_size.width * 0.2, screen_size.height * 0.25));
		sheeps[1]->setPosition(Vec2(screen_size.width * 0.4, screen_size.height * 0.2));
		sheeps[2]->setPosition(Vec2(screen_size.width * 0.6, screen_size.height * 0.25));
		sheeps[3]->setPosition(Vec2(screen_size.width * 0.8, screen_size.height * 0.2));
		break;
	}
}

void Ufo::onClickText(Ref* sender) {
	CCLOG("On Clicked Text!");

	if (clickedText) {
		clickedText = false;
		int id = AUDIO_ENGINE->play(list_data[stage_count].n_sound);
        cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, std::string path) {
			clickedText = true;
		});
	}	
}

void Ufo::onClickSheep(cocos2d::Ref * sender) {
	//hideTapGuideGame();
	CCLOG("On Clicked Sheep!");
	auto choose = (ui::Layout*) sender;
	Ufo_Sheep* sheep;

	if (!clickable) {
		return;
	}
	else {
        inactiveMode(false);
		alien->stopAllActions();
		//image->setTouchEnabled(false);
		alien_light->setVisible(true);
		clickable = false;

		for (Ufo_Sheep* s : sheeps) {
			if (s->getLayout() == choose) {
				sheep = s;
				break;
			}
		}
		AUDIO_ENGINE->play(sheep->getAudio());

		this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
			alien->runAction(MoveTo::create(0.5, Vec2(sheep->getPosition().x, sheep->getPosition().y + 150)));
			alien->runAction(RotateTo::create(0.5, 0));
			alien_light->runAction(MoveTo::create(0.5, Vec2(sheep->getPosition().x, sheep->getPosition().y + 150)));
		}), DelayTime::create(0.5), CallFunc::create([=] {
			AUDIO_ENGINE->play(SOUND_ACTION);
			alien_light->playSequence("action");
			for (Ufo_Sheep* sheep : sheeps) {
				sheep->getSheep()->playSequence("look_start");
			}
		}), DelayTime::create(0.2), CallFunc::create([=] {
			sheep->getSheep()->playSequence("fly");
			sheep->runAction(MoveBy::create(0.4, Vec2(0, 50)));
		}), DelayTime::create(0.5), CallFunc::create([=] {
			for (Ufo_Sheep* sheep : sheeps) {
				sheep->getSheep()->playSequence("look_end");
			}
			if (sheep == right_answer) {
				clickedText = false;
				alien_light->playSequence("correct");
				sheep->runAction(MoveTo::create(0.5, alien->getPosition()));
				sheep->runAction(ScaleTo::create(0.5, 0));
				this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
					float x = (float)list_data[stage_count].position.size() / 2;
					Vec2 point;
					point.y = list_text[list_data[stage_count].position[0]]->getPosition().y - 250;
					if (x == (int)x) {
						point.x = (list_text[list_data[stage_count].position[x - 1]]->getPosition().x + list_text[list_data[stage_count].position[x]]->getPosition().x) / 2;
					}
					else {
						point.x = list_text[list_data[stage_count].position[x - 0.5]]->getPosition().x;
					}
					alien->runAction(MoveTo::create(0.5, point));
					alien_light->setVisible(false);
				}), DelayTime::create(1), CallFunc::create([=] {
					for (int i = 0; i < list_data[stage_count].position.size(); i++) {
						list_text[list_data[stage_count].position[i]]->setVisible(true);
						list_text[list_data[stage_count].position[i]]->setTextColor(Color4B(255, 45, 50, 255));
						list_light[list_data[stage_count].position[i]]->stopAllActions();
						list_light[list_data[stage_count].position[i]]->playSequence("light");
					}
					AUDIO_ENGINE->play(SOUND_CORRECT);
				}), DelayTime::create(1), CallFunc::create([=] {
					for (int i = 0; i < list_firework.size(); i++) {
						this->runAction(Sequence::create(DelayTime::create(i * 0.1), CallFunc::create([=] {
							list_firework[i]->setVisible(true);
							list_firework[i]->play(false);
						}), NULL));
					}
				}), DelayTime::create(1), CallFunc::create([=] {
					for (int i = 0; i < list_light_2.size(); i++) {
						list_light_2[i]->removeFromParentAndCleanup(true);
					}
					alien->setLocalZOrder(100);
					textholder->setVisible(false);
					moveText(list_text, Vec2(screen_size.width/ 2, screen_size.height* 0.8));
                    
                    image = ui::ImageView::create(list_data[stage_count].image_link);
                    image->addClickEventListener(CC_CALLBACK_1(Ufo::onClickImage, this));
                    image->setTouchEnabled(true);
                    root_layout->addChild(image, 5000);
                    image->setOpacity(1);
                    image->ignoreContentAdaptWithSize(false);
                    image->setContentSize(Size(400, 400));
                    image->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2 - 100));
                    image->runAction(FadeIn::create(0.5));
                    
					trans_layout->setVisible(true);
					trans_layout->setOpacity(255);
					for (Ufo_Sheep* sheep : sheeps) {
						sheep->removeFromParentAndCleanup(true);
					}
				}), DelayTime::create(0.5), CallFunc::create([=] {
					//image->stopAllActions();
					int id = AUDIO_ENGINE->play(list_data[stage_count].right_answer.audio_path);
					cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, std::string path) {
						AUDIO_ENGINE->play(list_data[stage_count].n_sound);
					});

				}), DelayTime::create(2.5), CallFunc::create([=] {
					for (ui::Text* text : list_text) {
						text->removeFromParentAndCleanup(true);
					}

					for (GAFWrapper* firework : list_firework) {
						firework->removeFromParentAndCleanup(true);
					}

                    image->runAction(ScaleTo::create(1, 0));
                    image->runAction(Repeat::create(Sequence::create(RotateBy::create(0.15, 180), RotateBy::create(0.15, 180), NULL), 4));
				}), DelayTime::create(1), CallFunc::create([=] {
					trans_layout->runAction(FadeOut::create(0.5));
					trans_layout->setVisible(false);
					alien->runAction(MoveTo::create(0.5, Vec2(utils::findChild(this, "ufo")->getPosition().x - 200, utils::findChild(this, "ufo")->getPosition().y - 200)));
					alien->playSequence("fade_out");
					onNextStage();
				}), NULL));
			}
			else {
				AUDIO_ENGINE->play(SOUND_WRONG);
                inactiveMode(10.0f);
				alien_light->playSequence("wrong");
				sheep->getSheep()->playSequence("fall");
				sheep->runAction(MoveBy::create(0.25, Vec2(0, -50)));
				this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
					clickable = true;
					for (int i = 0; i < sheeps.size(); i++) {
						sheeps[i]->getSheep()->playSequence("idle", true);
					}
					//image->setTouchEnabled(true);
					alien->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), RotateTo::create(1, 10), RotateTo::create(1, 0), RotateTo::create(1, -10), RotateTo::create(1, 0), NULL)));
					alien->runAction(Repeat::create(Sequence::create(MoveTo::create(1.5, Vec2(screen_size.width / 2 + 150, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2 - 150, screen_size.height / 2)), MoveTo::create(1.5, Vec2(screen_size.width / 2, screen_size.height / 2)), NULL), 10000));
				}), NULL));
			}
		}), NULL));
	}
}

void Ufo::onClickImage(Ref* sender) {
	//image->setTouchEnabled(false);
	int id = AUDIO_ENGINE->play(list_data[stage_count].n_sound);
	cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, std::string path) {
		AUDIO_ENGINE->play(list_data[stage_count].p_sound);
	});
	this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
		//image->setTouchEnabled(true);
	}), NULL));
}

void Ufo::onNextStage() {
	stage_count++;
    inactiveMode(false);
	if (stage_count == number_stage) {
        if (!CONFIG_MANAGER->isVietnameseStories())
            AUDIO_ENGINE->play(SOUND_FADE_OUT);
	}
	this->runAction(Sequence::create(DelayTime::create(4.5), CallFunc::create([=] {
		blackhole->runAction(ScaleTo::create(0.5, 0));

	}), DelayTime::create(2), CallFunc::create([=] {
		if (stage_count == number_stage) {
			onFinishGame();
		}
		else {
			onBeginStage();
		}
	}), NULL));
}

void Ufo::onFinishGame() {
    AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onFinishGame();
}

void Ufo::inactiveMode(bool enabled,float delay)
{
    if(enabled)
    {
        if(this->isScheduled("inactive_mode")){
            this->unschedule("inactive_mode");
            AUDIO_ENGINE->stopSound((CONFIG_MANAGER->isGameVMonkey() && !question_vi.empty()) ? question_vi:SOUND_QUESTION);
        }
        this->schedule([=](float dt)
        {
            AUDIO_ENGINE->playSound((CONFIG_MANAGER->isGameVMonkey() && !question_vi.empty()) ? question_vi:SOUND_QUESTION);
        },10.0f,1000,delay,"inactive_mode");
    }
    else
    {
        this->unschedule("inactive_mode");
        AUDIO_ENGINE->stopSound((CONFIG_MANAGER->isGameVMonkey() && !question_vi.empty()) ? question_vi:SOUND_QUESTION);
    }
}
