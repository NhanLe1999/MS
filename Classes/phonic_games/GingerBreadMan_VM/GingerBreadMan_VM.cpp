//
//  GingerBreadMan_VM.h
//


#include "GingerBreadMan_VM.h"
#include "JsonDump.h"
#include "AudioHelper.h"
#include "MJHelper.h"
#define num_egg 3

using namespace cocos2d;
GingerBreadMan_VM * GingerBreadMan_VM::createGame(std::string json_file) {

	GingerBreadMan_VM * obj = new GingerBreadMan_VM();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(json_file);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void GingerBreadMan_VM::initHighlightIndexs(GBM_INFO & info)
{
	
	// Gen character index	
	int start_index = 0;
	std::queue<int> underline_index;
	for (int i = 0; i < info.list_character.size(); ++i) {
		auto phonic_data = info.list_character[i].character;
		int index = 0;
        for (int j = 0; j < phonic_data.size(); ++j) {
            if (phonic_data[j] == '_') {
                //underline_index
                underline_index.push(start_index + index);
                index++;
            }
            else {
                if (underline_index.empty() || underline_index.front() > start_index) {
                    CCLOG("test %c",phonic_data[j]);
                    if (!checkRomaji(phonic_data[j])){
                        if (j == 0){
                            info.list_character[i].list_index.push_back(start_index + index);
                            index++;
                        }else{
                            if(checkRomaji(phonic_data[j-1])){
                                info.list_character[i].list_index.push_back(start_index + index);
                                index++;
                            }
                        }
                    }else{
                        info.list_character[i].list_index.push_back(start_index + index);
                        index++;
                    }

                }
                else {
                    info.list_character[i].list_index.push_back(underline_index.front());
                    underline_index.pop();
                }
            }
        }
        start_index += index;
    }
}

void GingerBreadMan_VM::didLoadFromCSB(std::string json_file) {
    state_dau = false;
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document value_data;
	bool is_convert_plist_2_json = false;
	if (is_convert_plist_2_json) {
		auto value_map = FileUtils::getInstance()->getValueMapFromFile(json_file);
		buffer_string = JsonDump::getInstance()->MapToString(value_map);
	}

	value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
	if (value_data.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	// log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	value_data.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif

	generateData(value_data.GetObject());

}

void GingerBreadMan_VM::generateData(rapidjson::Value value_data) {
	rapidjson::Value& game_mode = value_data.FindMember("game_mode")->value;
	auto value_info = value_data["data"].GetArray();

	if (game_mode.HasMember("number_turnplay") && game_mode["number_turnplay"].IsInt()) {
		number_turnplay = game_mode["number_turnplay"].GetInt();
		number_turnplay = min(number_turnplay, (int)value_info.Size());
	}
	else number_turnplay = (int)value_info.Size();

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
	for (int i = 0; i < (int)value_info.Size(); i++) {
		GBM_INFO gbm_info;
		auto letter = value_info[i].FindMember("text_full")->value.GetString();
        gbm_info.co_dau = false;
        if (value_info[i].HasMember("co_dau")){
            gbm_info.co_dau = value_info[i].FindMember("co_dau")->value.GetBool();
        }
        
		auto pImage = math::resource::ResHelper::getImage(images, letter);
		gbm_info.text = pImage->getText("name_1");
		gbm_info.audio_path_normal = pImage->getAudios("name_1").front();
		gbm_info.audio_path_slow = pImage->getAudios("name_n").front();
        gbm_info.image_path = pImage->path;

		auto audio_phonic = pImage->getAudioPhonic("name_p");
		auto list_charater = pImage->getTextPhonic("name_p");
        if (gbm_info.co_dau){
            gbm_info.text_dau = list_charater.back();
            if (gbm_info.text_dau == "/") gbm_info.text_dau = "´";
            list_charater.pop_back();
            gbm_info.audio_dau = audio_phonic.back();
            audio_phonic.pop_back();
            
            gbm_info.text_khong_dau = list_charater.back();
            list_charater.pop_back();
            gbm_info.audio_khong_dau = audio_phonic.back();
            audio_phonic.pop_back();
            
            gbm_info.pos_dau = pImage->getComparisionValue("position_p");
        }
		for (int index = 0; index < (int)list_charater.size(); index++) {
			GingerBreadMan_VM_Item::GBM_CHARACTER_INFO character_info;
			auto character_data = list_charater[index];
			character_info.character = list_charater[index];
			character_info.audio_path = audio_phonic[index];

			gbm_info.list_character.push_back(character_info);
		}

		auto list_distracting = value_info[i]["list_distracting"].GetArray();
		for (int index = 0; index < (int)list_distracting.Size(); index++) {
			GingerBreadMan_VM_Item::GBM_CHARACTER_INFO character_info;
			auto l = list_distracting[index]["text"].GetString();
			auto lImage = math::resource::ResHelper::getImage(images, l);
			character_info.character = lImage->getText("name_1");
			character_info.audio_path = lImage->getAudios("name_1").front();
			gbm_info.list_distracting.push_back(character_info);
		}
        
		initHighlightIndexs(gbm_info);
		list_item_data.push_back(gbm_info);
	}
	//    mj::helper::shuffle(list_item_data);
}

void GingerBreadMan_VM::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->playBackgroundMusic("sounds/GingerBreadMan_VM/gbm_bg.mp3", 0.8f);
	this->loadFrames();
	this->showIntroGame();
}

void GingerBreadMan_VM::loadFrames() {
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setTouchEnabled(true);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	this->addChild(touch_layout, 1000000);
	touch_layout->setTouchEnabled(true);
	touch_layout->setVisible(false);


	auto background = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_background.jpg");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	background->setPosition(Vec2(screen_size.width / 2, 0));
	this->addChild(background);

	table = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_table.png");
	table->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	table->setPosition(Vec2(screen_size.width / 2, -80 - table->getContentSize().height));
	//    table->setPosition(Vec2(screen_size.width/2, -80));
	this->addChild(table, 10000);

	auto khay = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_khay.png");
	khay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	khay->setPosition(Vec2(table->getContentSize().width / 2, table->getContentSize().height));
	table->addChild(khay);

	banhgung = GAFWrapper::createGAF("gaf/GingerBreadMan_VM/banhgung/banhgung.gaf");
	khay->addChild(banhgung);
	banhgung->setPosition(Vec2(khay->getContentSize().width*0.625, khay->getContentSize().height*1.36));
	banhgung->setVisible(false);

	box = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_box.png");
	this->addChild(box);
	box->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	box->setPosition(Vec2(screen_size.width / 2, screen_size.height*1.1 + box->getContentSize().height));
    box->ignoreContentAdaptWithSize(false);
    box->setContentSize(Size(800, 150));
	//    box->setPosition(Vec2(screen_size.width/2, screen_size.height*0.95));

	pie_layout = cocos2d::ui::Layout::create();
	box->addChild(pie_layout);
	pie_layout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pie_layout->setPosition(Vec2(0, -50));

	gachngang_layout = cocos2d::ui::Layout::create();
	box->addChild(gachngang_layout);
	gachngang_layout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	gachngang_layout->setPosition(Vec2(0, 30));

	text_lb = cocos2d::ui::Text::create("", "fonts/danhvan.ttf", 100);
	this->addChild(text_lb, 1000);
	text_lb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	text_lb->setColor(Color3B::BLACK);

	speaker = cocos2d::ui::Button::create("games/GingerBreadMan_VM/gbm_speaker.png");
	this->addChild(speaker, 10000000);
	speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	speaker->setPosition(Vec2(5, screen_size.height - 5));
	speaker->setScale(1.5f);
	speaker->addClickEventListener(CC_CALLBACK_1(GingerBreadMan_VM::onSpeakerClicked, this));

	phaohoa1 = GAFWrapper::createGAF("gaf/GingerBreadMan_VM/phaohoa/phaohoa.gaf");
	this->addChild(phaohoa1, INT_MAX - 100);
	phaohoa1->setPosition(Vec2(screen_size.width*0.125, screen_size.height*0.2));
	phaohoa1->setVisible(false);

	phaohoa2 = GAFWrapper::createGAF("gaf/GingerBreadMan_VM/phaohoa/phaohoa.gaf");
	this->addChild(phaohoa2, INT_MAX - 100);
	phaohoa2->setPosition(Vec2(screen_size.width*0.875, screen_size.height*0.2));
	phaohoa2->setVisible(false);
	phaohoa2->setFlippedX(true);

	phaohoa3 = GAFWrapper::createGAF("gaf/GingerBreadMan_VM/phaohoa/phaohoa.gaf");
	this->addChild(phaohoa3, INT_MAX - 100);
	phaohoa3->setPosition(Vec2(screen_size.width / 2, screen_size.height*0.2));
	phaohoa3->setVisible(false);
    
}

void GingerBreadMan_VM::showIntroGame() {
    audio_helper->play("sounds/GingerBreadMan_VM/gbm_whop.mp3");
    auto duration_move = 1.0f;
    box->runAction(EaseBounceOut::create(MoveTo::create(duration_move, Vec2(screen_size.width / 2, screen_size.height*0.95))));
    table->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(duration_move, Vec2(screen_size.width / 2, -80))), CallFunc::create([=] {
        auto touch_layout = cocos2d::ui::Layout::create();
        touch_layout->setContentSize(screen_size);
        touch_layout->setTouchEnabled(true);
        this->addChild(touch_layout, INT_MAX - 100);

        this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {this->showText(); }), NULL));
        audio_helper->play("sounds/GingerBreadMan_VM/gbm_start.mp3");
        banhgung->playSequence("start", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
            banhgung->playSequence("talk", true);
            auto audio_path = question_vi;//std::string("sounds/GingerBreadMan_VM/gbm_mc.mp3");
            auto audio_id = AudioEngine::play2d(audio_path);
            AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                banhgung->playSequence("idle", true);
                this->runEffectGachNgang(current_character.list_index);
                auto audio_id = AudioEngine::play2d(current_character.audio_path);
                speaker_audio_path = current_character.audio_path;
                AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                    touch_layout->removeFromParent();
                    if (is_auto_play) {
                        
                    }
                });
            });

        });
        banhgung->setVisible(true);
    }), NULL));
}

void GingerBreadMan_VM::showText() {
	current_item_info = list_item_data[index_item];
	list_gachngang.clear();
	list_pos_letter.clear();
	list_letter.clear();
	AudioEngine::preload(current_item_info.audio_path_slow);
	gachngang_layout->removeAllChildren();

	auto width_gachngang = 81.0f, w_add_gachngang = 5.0f ;
	auto num_gachngang = StringUtils::StringUTF8(current_item_info.text).length();
    if(num_gachngang * width_gachngang > 800)
    {
        width_gachngang *= 720.0f / (num_gachngang * width_gachngang);
    }
	for (int i = 0; i < num_gachngang; i++) {
		auto gachngang = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_gachchan.png");
        auto capInsets = Rect(1,1,gachngang->getVirtualRendererSize().width-2,gachngang->getVirtualRendererSize().height-2);
        gachngang->setScale9Enabled(true);
        gachngang->setCapInsets(capInsets);
        gachngang->setContentSize(Size(width_gachngang,gachngang->getContentSize().height));
		gachngang_layout->addChild(gachngang);
		gachngang->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		gachngang->setPositionX(i*(width_gachngang + w_add_gachngang));
		gachngang->setColor(Color3B::BLACK);
		gachngang->setScale(0, 1.0f);
		gachngang->runAction(ScaleTo::create(0.25f, 1.0f));
		list_gachngang.push_back(gachngang);
	}
	gachngang_layout->setContentSize(cocos2d::Size(num_gachngang*(width_gachngang + w_add_gachngang) - w_add_gachngang, 50));
	gachngang_layout->setPositionX(box->getContentSize().width / 2 - gachngang_layout->getContentSize().width / 2);

	text_lb->setScale(1.0f);
    text_lb->setString(current_item_info.co_dau?current_item_info.text_khong_dau:current_item_info.text);
	auto pos_text = gachngang_layout->convertToWorldSpace(Vec2(0, list_gachngang[0]->getPositionY()));
	text_lb->setPosition(Vec2(screen_size.width / 2, pos_text.y));
	for (int i = 0; i < (int)list_gachngang.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			auto pos_convert = text_lb->convertToNodeSpace(gachngang_layout->convertToWorldSpace(list_gachngang[i]->getPosition()));
			list_pos_letter.push_back(letter->getPosition() + Vec2(0, 0));
			CCLOG("y = %f", letter->getPositionY() - letter->getContentSize().height*0.5);
			letter->setPosition(Vec2(pos_convert.x + width_gachngang * 0.5, letter->getPositionY() - 23));
			letter->setVisible(false);
			list_letter.push_back(letter);
			letter->setColor(Color3B::BLACK);
		}
	}

	list_letter_save = list_letter;
	this->runAction(Sequence::create(DelayTime::create(0.35f), CallFunc::create([=] {
		this->showPie();
	}), NULL));
}

void GingerBreadMan_VM::showPie() {
	touch_layout->setVisible(true);
	speaker->setVisible(true);
	list_pie.clear();
	pie_layout->removeAllChildren();
    
    auto list_distracting = current_item_info.list_distracting;
    auto list_character = current_item_info.list_character;
    current_character = current_item_info.list_character[index_charater];
    list_character.erase(list_character.begin()+index_charater);
    list_distracting.insert(list_distracting.begin(), current_character);
    auto num_pie = 4;
    while (list_distracting.size() < num_pie) {
        list_distracting.push_back(list_character.at(0));
        list_character.erase(list_character.begin());
    }
    
    mj::helper::shuffle(list_distracting);

    
	auto width = 182, height = 189;
	auto w_add = 30;
	auto delaytime = 0.2f;
	
	for (int i = 0; i < num_pie; i++) {
		auto pie = GingerBreadMan_VM_Item::createItem(list_distracting[i]);
		pie_layout->addChild(pie);
		pie->setPosition(Vec2(i*(width + w_add) + width / 2, height / 2));
		pie->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pie->setDelegate(this);
		list_pie.push_back(pie);
		pie->setScale(0);
		pie->runAction(Sequence::create(DelayTime::create(i*delaytime), CallFunc::create([=] {
			audio_helper->play("sounds/GingerBreadMan_VM/gbm_showpie.mp3");
		}), EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)), CallFunc::create([=] {
			if (i == num_pie - 1) {
				if ((index_charater > 0 || index_item > 0)) {
					this->runEffectGachNgang(current_character.list_index);
					audio_helper->play(current_character.audio_path);
					speaker_audio_path = current_character.audio_path;
				}
				touch_layout->setVisible(false);
			}
		}), NULL));
		if (list_distracting[i].character == current_character.character) {
			index_right = i;
		}
	}
	pie_layout->setContentSize(cocos2d::Size(num_pie*(width + w_add) - w_add, height));
	pie_layout->setPositionX(box->getContentSize().width / 2 - pie_layout->getContentSize().width / 2);
    this->showTapGuideGame(pie_layout->convertToWorldSpace(list_pie[index_right]->getPosition()), (index_charater==0)?10:6, 10.0f);
	auto num_character = (int)StringUtils::getCharacterCountInUTF8String(current_character.character);
	list_pos_letter_goal.clear();
	auto start_index = current_character.list_index[0];
	for (int i = start_index; i < start_index + num_character;++i) {
		auto gach_ngang = list_gachngang[i];
		list_pos_letter_goal.push_back(gachngang_layout->convertToWorldSpace(gach_ngang->getPosition() + Vec2(40.5, 0)));
	}
}

void GingerBreadMan_VM::runEffectGachNgang(std::vector<int> indexs) {
	for (int i : indexs) {
		auto gach_ngang = list_gachngang[i];
		gach_ngang->runAction(Repeat::create(Sequence::create(CallFunc::create([=] {
			gach_ngang->setColor(Color3B::RED);
		}), DelayTime::create(0.3f), CallFunc::create([=] {
			gach_ngang->setColor(Color3B::BLACK);
		}), DelayTime::create(0.3f), NULL), 100000));
	}
}

void GingerBreadMan_VM::onItemClicked(cocos2d::Ref *sender) {
     this->hideTapGuideGame();
	auto pie = static_cast<GingerBreadMan_VM_Item *>(sender);
	//audio_helper->play(pie->item_info.audio_path); ;
	if (pie->item_info.character == current_character.character) {
		for (int i = 0; i < (int)list_pie.size(); i++) {
			if (pie == list_pie[i]) {
				pie->setChooseRight();
				this->onChoosePieRight(pie);
			}
			else list_pie[i]->setVisible(false);
		}
	}
	else {
		//audio_helper->play("sounds/GingerBreadMan_VM/gbm_wrong.mp3");
        if (state_dau){
            if (pie->item_info.character == current_item_info.text_dau) {
                for (int i = 0; i < (int)list_pie.size(); i++) {
                    if (pie == list_pie[i]) {
                        pie->setChooseRight();
                        this->onChoosePieRight(pie);
                    }
                    else list_pie[i]->setVisible(false);
                }
                return;
            }
        }
		std::vector<std::string> chain;
		chain.push_back(pie->item_info.audio_path);
		chain.push_back("sounds/GingerBreadMan_VM/gbm_wrong.mp3");
		audio_helper->play_sequence(chain,nullptr,"wrong_effect");
		banhgung->playSequence("lose", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			banhgung->playSequence("idle", true);
		});
		pie->onChooseWrong();
        this->showTapGuideGame(pie_layout->convertToWorldSpace(list_pie[index_right]->getPosition()), (index_charater==0)?10:6, 10.0f);
	}
}

void GingerBreadMan_VM::onChoosePieRight(GingerBreadMan_VM_Item *pie) {
	speaker_audio_path = "";
	std::vector<std::string> chain;
	chain.push_back(pie->item_info.audio_path);
	chain.push_back("sounds/GingerBreadMan_VM/gbm_right.mp3");
	audio_helper->play_sequence(chain, nullptr, "right_effect");
	//audio_helper->play("sounds/GingerBreadMan_VM/gbm_right.mp3");
	
	phaohoa1->setVisible(true);
	phaohoa2->setVisible(true);
	phaohoa3->setVisible(true);
	phaohoa1->play(false, [=](gaf::GAFObject *obj) {
		phaohoa1->setVisible(false);
		phaohoa2->setVisible(false);
		phaohoa3->setVisible(false);
	});
	phaohoa2->play(false);
	phaohoa3->play(false);

	banhgung->playSequence("win", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		banhgung->playSequence("idle", true);
	});
	this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
		this->onMergeLetter();
	}), NULL));

	for (int i = 0; i < (int)list_pos_letter_goal.size(); i++) {
		auto letter_index = current_character.list_index[0] + i;
		auto letter = pie->text_lb->getLetter(i);
		if (letter) {
            if (!state_dau){
                list_gachngang[letter_index]->stopAllActions();
                list_gachngang[letter_index]->setColor(Color3B::BLACK);
            }else{
                ganhngang_dau->stopAllActions();
                ganhngang_dau->setColor(Color3B::BLACK);
            }
			auto pos_goal = pie->text_lb->convertToNodeSpace(list_pos_letter_goal[i]);
			pos_goal.y += letter->getContentSize().height / 2;
			auto duration = 0.75*letter->getPosition().distance(pos_goal) / 1024.0f;
			letter->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=] {
				letter->setVisible(false);
				if (i == (int)list_pos_letter_goal.size() - 1) {
					this->onShowLetter();
				}
			}), NULL));
		}
	}
}

void GingerBreadMan_VM::onShowLetter() {
    if (state_dau) {
        ganhngang_dau->removeFromParent();
        text_dau->setOpacity(255);
        text_dau->runAction(Sequence::create(ScaleTo::create(0.1f, 1.05f, 0.9f), ScaleTo::create(0.1f, 1.0f, 1.1f), ScaleTo::create(0.1f, 1.0f, 0.9f), ScaleTo::create(0.1f, 1.0f, 1.0f), CallFunc::create([=] {
        }), NULL));
    }else{
        for (int i : current_character.list_index) {
            auto letter = list_letter[i];
            //list_letter.erase(list_letter.begin());
            letter->setVisible(true);
            list_gachngang[i]->removeFromParent();
            //list_gachngang.erase(list_gachngang.begin());
            //letter->setColor(cocos2d::Color3B::WHITE);
            letter->runAction(Sequence::create(ScaleTo::create(0.1f, 1.05f, 0.9f), ScaleTo::create(0.1f, 1.0f, 1.1f), ScaleTo::create(0.1f, 1.0f, 0.9f), ScaleTo::create(0.1f, 1.0f, 1.0f), CallFunc::create([=] {
            }), NULL));
            //audio_helper->play(current_character.audio_path);
        }
    }
}


void GingerBreadMan_VM::onNextTurnPlay() {
	index_charater = 0;
	index_item++;
    state_dau = false;
    if (text_dau) text_dau->removeFromParent();
    text_dau = nullptr;
	if (index_item >= number_turnplay) {//end_game
		this->onFinishGame();
	}
	else {
		auto duration_move = 1.0f;
		box->runAction(EaseBounceOut::create(MoveTo::create(duration_move, Vec2(screen_size.width / 2, screen_size.height*0.95))));
		table->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(duration_move, Vec2(screen_size.width / 2, -80))), CallFunc::create([=] {
			this->showText();
		}), NULL));

	}
}
void GingerBreadMan_VM::onNextTurnPie() {
	this->showPie();
}

void GingerBreadMan_VM::onMergeLetter() {
	// this->hideTapGuideGame();
	index_charater++;
	if (index_charater >= (int)current_item_info.list_character.size()) {
        if (current_item_info.co_dau && !state_dau){
            state_dau = true;
            touch_layout->setVisible(true);
            speaker->setVisible(true);
            list_pie.clear();
            pie_layout->removeAllChildren();
            text_dau = cocos2d::ui::Text::create(current_item_info.text_dau, "fonts/danhvan.ttf", 100);
            //text_dau->setTextColor(cocos2d::Color4B::BLACK);
            //text_dau->enableOutline(cocos2d::Color4B::WHITE, 5);
            auto lb = text_lb->getLetter(current_item_info.pos_dau);
            auto pos = (current_item_info.text_dau==".") ? lb->getPosition() - Vec2(0,lb->getContentSize().height/2 - 5): lb->getPosition() + Vec2(0,lb->getContentSize().height/2);
            text_dau->setColor(Color3B::BLACK);
            text_dau->setPosition(pos);
            text_dau->setOpacity(0);
            text_lb->addChild(text_dau);
            
            ganhngang_dau = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_gachchan.png");
            auto capInsets = Rect(1,1,ganhngang_dau->getVirtualRendererSize().width-2,ganhngang_dau->getVirtualRendererSize().height-2);
            ganhngang_dau->setScale9Enabled(true);
            ganhngang_dau->setCapInsets(capInsets);
            //gachngang->setContentSize(Size(width_gachngang,gachngang->getContentSize().height));
            gachngang_layout->addChild(ganhngang_dau);
            ganhngang_dau->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            ganhngang_dau->setColor(Color3B::BLACK);
            ganhngang_dau->setScale(0.25f, 1.0f);
            auto x = (current_item_info.text_dau == ".")?Vec2(0,-18):Vec2(0,5);
            auto pos_dau = gachngang_layout->convertToNodeSpace(text_dau->getParent()->convertToWorldSpace(text_dau->getPosition()))+x;
            
            ganhngang_dau->setPosition(pos_dau);
            ganhngang_dau->runAction(Repeat::create(Sequence::create(CallFunc::create([=] {
                ganhngang_dau->setColor(Color3B::RED);
            }), DelayTime::create(0.5f), CallFunc::create([=] {
                ganhngang_dau->setColor(Color3B::BLACK);
            }), DelayTime::create(0.5f), NULL), 100000));
            list_gachngang.push_back(ganhngang_dau);
            
            std::vector<std::pair<std::string, std::string>> list_dau = {
                {"´","dau/sac.mp3"},
                {"`","dau/huyen.mp3"},
                {"~","dau/nga.mp3"},
                {"?","dau/hoi.mp3"},
                {".","dau/nang.mp3"}
            };
            std::vector<GingerBreadMan_VM_Item::GBM_CHARACTER_INFO> list_distracting;
            GingerBreadMan_VM_Item::GBM_CHARACTER_INFO correct_info;
            correct_info.character = current_item_info.text_dau;
            correct_info.audio_path = current_item_info.audio_dau;
            list_distracting.push_back(correct_info);
            for (auto dau : list_dau){
                if (dau.first != current_item_info.text_dau){
                    GingerBreadMan_VM_Item::GBM_CHARACTER_INFO info;
                    info.character = dau.first;
//                    if (info.character == "´"){
//                        info.character = "/";
//                    }
                    info.audio_path = dau.second;
                    list_distracting.push_back(info);
                    if (list_distracting.size() == 4) break;
                }
            }

            mj::helper::shuffle(list_distracting);

            auto num_pie = 4;
            auto width = 182, height = 189;
            auto w_add = 30;
            auto delaytime = 0.2f;
            
            for (int i = 0; i < num_pie; i++) {
                auto pie = GingerBreadMan_VM_Item::createItem(list_distracting[i]);
                pie_layout->addChild(pie);
                pie->setPosition(Vec2(i*(width + w_add) + width / 2, height / 2));
                pie->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                pie->setDelegate(this);
                list_pie.push_back(pie);
                pie->setScale(0);
                pie->runAction(Sequence::create(DelayTime::create(i*delaytime), CallFunc::create([=] {
                    audio_helper->play("sounds/GingerBreadMan_VM/gbm_showpie.mp3");
                }), EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)), CallFunc::create([=] {
                    if (i == num_pie - 1) {
                        if ((index_charater > 0 || index_item > 0)) {
                            //this->runEffectGachNgang(current_character.list_index);
                            audio_helper->play(current_item_info.audio_dau);
                            speaker_audio_path = current_item_info.audio_dau;
                        }
                        touch_layout->setVisible(false);
                    }
                }), NULL));
                if (list_distracting[i].character == current_item_info.text_dau) {
                    index_right = i;
                }
            }
            pie_layout->setContentSize(cocos2d::Size(num_pie*(width + w_add) - w_add, height));
            pie_layout->setPositionX(box->getContentSize().width / 2 - pie_layout->getContentSize().width / 2);
            this->showTapGuideGame(pie_layout->convertToWorldSpace(list_pie[index_right]->getPosition()), (index_charater==0)?10:6, 10.0f);
            auto num_character = (int)1;
            list_pos_letter_goal.clear();
            auto start_index = current_character.list_index.back()+1;
            for (int i = start_index; i < start_index + num_character;++i) {
                auto gach_ngang = list_gachngang[i];
                list_pos_letter_goal.push_back(gachngang_layout->convertToWorldSpace(gach_ngang->getPosition() + Vec2(40.5, 0)));
            }
            
        }else{
            speaker->setVisible(false);
            this->runAction(Sequence::create(DelayTime::create(0.35f), CallFunc::create([=] {
                audio_helper->play("sounds/GingerBreadMan_VM/gbm_whop.mp3");
                box->runAction(EaseBackIn::create(MoveTo::create(0.75f, Vec2(screen_size.width / 2, screen_size.height*1.1 + box->getContentSize().height))));
                table->runAction(EaseBackIn::create(MoveTo::create(0.75f, Vec2(screen_size.width / 2, -88 - table->getContentSize().height*1.5))));
                auto duration = 0.2f;
                if (current_item_info.co_dau){
                    if (text_dau) text_dau->setOpacity(255);
                }
                text_lb->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(MoveTo::create(duration, Vec2(screen_size.width / 2, screen_size.height*0.4)), ScaleTo::create(duration, 1.7f), NULL), CallFunc::create([=] {
                    setColorForText(cocos2d::Color3B::BLACK);
                }), DelayTime::create(0.5f), CallFunc::create([=] {
                    //audio_helper->play_sequence(current_item_info.audio_path_phonic, nullptr, "play_phonic");
                    this->playSyncText();
                    //
                }), NULL));
                for (int i = 0; i < (int)list_pos_letter.size(); i++) {
                    list_letter_save[i]->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(duration, list_pos_letter[i]), CallFunc::create([=] {
                        if (current_item_info.co_dau){
                            if (text_dau){
                                auto lb = text_lb->getLetter(current_item_info.pos_dau);
                                auto pos = (current_item_info.text_dau==".") ? lb->getPosition() - Vec2(0,lb->getContentSize().height/2 - 5): lb->getPosition() + Vec2(0,lb->getContentSize().height/2 - 10);
                                if (current_item_info.text_dau=="`") pos += Vec2(-3,+5);
                                if (current_item_info.text_dau=="´") pos += Vec2(0,-5);
                                if (current_item_info.text_dau=="?") pos += Vec2(0,5);
                                auto utf8 = StringUtils::StringUTF8(current_item_info.text_khong_dau);
                                auto ee = utf8.getAsCharSequence(current_item_info.pos_dau,1);
                                if (ee == "ê" || ee == "â" || ee == "ô" || ee == "ă"){
                                    pos+= Vec2(0,2);
                                }
                                text_dau->setPosition(pos);
                            }
                        }
                    }), NULL));
                }
            }), NULL));
        }
	}
	else {
		this->onNextTurnPie();
	}

}
#define GBM_HIGHTLIGHT_COLOR cocos2d::Color3B::WHITE
void GingerBreadMan_VM::playSyncText()
{
	float dt = 0.0f;
	for (int i = 0; i < current_item_info.list_character.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			audio_helper->play(current_item_info.list_character[i].audio_path);
			for (int index : current_item_info.list_character[i].list_index) {
				text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(GBM_HIGHTLIGHT_COLOR);
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::BLACK);
				}), NULL));
			}
		}), NULL));
		dt += 1.f;
	}
    if (current_item_info.co_dau){
        scheduleOnce([=](float dt) {
            audio_helper->play(current_item_info.audio_khong_dau);
            auto scale = text_lb->getScale();
            text_lb->runAction(Sequence::create(CallFunc::create([=] {
                this->setColorForText(GBM_HIGHTLIGHT_COLOR);
            }), ScaleTo::create(0.5f, scale,scale*1.2f), ScaleTo::create(0.5f, scale, scale), CallFunc::create([=] {
                this->setColorForText(Color3B::BLACK);
                audio_helper->play(current_item_info.audio_dau);
                text_dau->runAction(Sequence::create(CallFunc::create([=] {
                    text_dau->setColor(GBM_HIGHTLIGHT_COLOR);
                }), ScaleTo::create(0.5f, 1, 1.2f), ScaleTo::create(0.5f, 1, 1), CallFunc::create([=] {
                    text_dau->setColor(Color3B::BLACK);
                    playSlowText();
                }), NULL));
            }), NULL));
        }, dt, "synctext finish");
    }else{
        scheduleOnce([=](float dt) {
            playSlowText();
        }, dt, "synctext finish");
    }
	
}

void GingerBreadMan_VM::playSlowText() {
	auto audio_path = std::string(current_item_info.audio_path_slow);
	auto audio_id = AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		this->setColorForText(GBM_HIGHTLIGHT_COLOR);
        if (text_dau) text_dau->setColor(GBM_HIGHTLIGHT_COLOR);
		auto duration = AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 20));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -20));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::BLACK);
            if (text_dau) text_dau->setColor(Color3B::BLACK);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));


}
void GingerBreadMan_VM::playNormalText() {
	auto audio_id = audio_helper->play(current_item_info.audio_path_normal);
	this->setColorForText(GBM_HIGHTLIGHT_COLOR);
    if (text_dau) text_dau->setColor(GBM_HIGHTLIGHT_COLOR);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::BLACK);
            if (text_dau) text_dau->setColor(Color3B::BLACK);
		}), DelayTime::create(1.0f), CallFunc::create([=] {
			this->showImage();
		}), NULL));
	}), NULL));

}

void GingerBreadMan_VM::setColorForText(Color3B color_text) {
	for (int i = 0; i < (int)list_letter_save.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
//    if (current_item_info.co_dau){
//       if (text_dau){
//           text_dau->setColor(color_text);
//       }
//    }
}

void GingerBreadMan_VM::showImage() {
	if (current_item_info.image_path != "") {//có image

		cocos2d::Size layout_size = cocos2d::Size(600, 400);
		auto img_layout = cocos2d::ui::Layout::create();
		this->addChild(img_layout);
		img_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		img_layout->setPosition(Vec2(screen_size.width / 2, screen_size.height*1.1));
		img_layout->setContentSize(layout_size + cocos2d::Size(10, 10));
		img_layout->setBackGroundColor(Color3B::BLACK);
		img_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);

		auto bound_layout = cocos2d::ui::Layout::create();
		img_layout->addChild(bound_layout);
		bound_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bound_layout->setPosition(img_layout->getContentSize() / 2);
		bound_layout->setContentSize(layout_size);
		bound_layout->setBackGroundColor(Color3B::WHITE);
		bound_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);

		auto img = cocos2d::ui::ImageView::create(current_item_info.image_path);
		img_layout->addChild(img);
		img->setPosition(img_layout->getContentSize() / 2);

		float scale = bound_layout->getContentSize().height * 0.9 / img->getContentSize().height;
		img->setScale(scale);

		if (img->getBoundingBox().size.width > bound_layout->getContentSize().width) {
			img->setScale(img->getScale()*bound_layout->getContentSize().width / img->getBoundingBox().size.width);
		}
		//if (img->getContentSize().height > bound_layout->getContentSize().height) {
		//	img->setScale(bound_layout->getContentSize().height*1.0 / img->getContentSize().height);
		//}

		//if (img->getBoundingBox().size.width > bound_layout->getContentSize().width) {
		//	img->setScale(img->getScale()*bound_layout->getContentSize().width / img->getBoundingBox().size.width);
		//}

		

		auto duration_move = 0.25f;
		auto y_goal = screen_size.height*0.3f;
		text_lb->runAction(MoveTo::create(duration_move, Vec2(screen_size.width / 2, y_goal / 2 - text_lb->getBoundingBox().size.height / 2 - 20)));
		img_layout->runAction(Sequence::create(MoveTo::create(duration_move, Vec2(screen_size.width / 2, y_goal)), DelayTime::create(1.0f), CallFunc::create([=] {
			audio_helper->play(current_item_info.audio_path_normal);
			auto audio_path = std::string(current_item_info.audio_path_normal);
			auto audio_id = AudioEngine::play2d(audio_path);
			this->setColorForText(GBM_HIGHTLIGHT_COLOR);
            if (text_dau) text_dau->setColor(GBM_HIGHTLIGHT_COLOR);
			AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
				this->setColorForText(Color3B::BLACK);
                if (text_dau) text_dau->setColor(Color3B::BLACK);
			});
		}), DelayTime::create(2.0f), CallFunc::create([=] {
			text_lb->runAction(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))));
		}), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, screen_size.height))), CallFunc::create([=] {
			//            text_lb->removeFromParent();
			//            text_lb = nullptr;
			this->onNextTurnPlay();
			img_layout->removeFromParent();
		}), NULL));

	}
	else {
		text_lb->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, screen_size.height))), CallFunc::create([=] {
			//            text_lb->removeFromParent();
			//            text_lb = nullptr;
		}), NULL));
	}
}

void GingerBreadMan_VM::onSpeakerClicked(cocos2d::Ref *sender) {
	audio_helper->play(speaker_audio_path);
}

void GingerBreadMan_VM::onExitTransitionDidStar() {
	MJMainGBase::onExitTransitionDidStart();
}
bool GingerBreadMan_VM::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    return false;
}
