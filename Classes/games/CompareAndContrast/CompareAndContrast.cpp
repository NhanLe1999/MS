#include "CompareAndContrast.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
INITIALIZE_READER(CompareAndContrast);

std::once_flag CompareAndContrast_reader;
#define CSB_NAME "csb/game/CompareAndContrast/CompareAndContrast.csb"
#define _game_path "json/CompareAndContrast/"

//gaf path
#define path_gaf_title "gaf/compareandcontrast/cnc_title/cnc_title.gaf"
#define path_gaf_guild "gaf/compareandcontrast/cnc_guild_%s/cnc_guild.gaf"
#define path_gaf_scorestar "gaf/compareandcontrast/cnc_scorestar/cnc_scorestar.gaf"
#define path_gaf_charbg "gaf/compareandcontrast/cnc_charbg/cnc_charbg.gaf"
#define path_gaf_effect_correct "gaf/compareandcontrast/cnc_effect/cnc_effect.gaf"
//sounds path
#define path_sound_title "sounds/compareandcontrast/cnc_title.mp3"
#define path_sound_textname "sounds/compareandcontrast/cnc_textname.mp3"
#define path_sound_answer "sounds/compareandcontrast/cnc_answer.mp3"
#define path_sound_correct "sounds/compareandcontrast/cnc_correct.mp3"
#define path_sound_fadeout "sounds/compareandcontrast/cnc_fadeout.mp3"
#define path_sound_guild "sounds/compareandcontrast/cnc_guild.mp3"
#define path_sound_interact "sounds/compareandcontrast/cnc_interact.mp3"
#define path_sound_paper "sounds/compareandcontrast/cnc_paper.mp3"
#define path_sound_score "sounds/compareandcontrast/cnc_score.mp3"
#define path_sound_scorebar "sounds/compareandcontrast/cnc_scorebar.mp3"
#define path_sound_text "sounds/compareandcontrast/cnc_text.mp3"
#define path_sound_wrong "sounds/compareandcontrast/cnc_wrong.mp3"

#define path_sound_gamename "sounds/compareandcontrast/Compare and Contrast.mp3"
#define path_sound_guild_text "sounds/compareandcontrast/cnc_guild_%s.mp3"
#define bg_music "sounds/compareandcontrast/bg_music.mp3"

CompareAndContrast * CompareAndContrast::createGame() {
	std::call_once(CompareAndContrast_reader, [] {
		REGISTER_CSB_READER(CompareAndContrast);
		REGISTER_CSB_READER(CompareAndContrast_Item);
	});
	auto p = static_cast<CompareAndContrast *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void CompareAndContrast::didLoadFromCSB() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	so_luot_choi = document.FindMember("game_mode")->value.GetInt();
	gaf_guild_type = document.FindMember("guild_type")->value.GetString();
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();
//    autoPlay = isAutoPlay;
	this->loadFrames();
}

void CompareAndContrast::generateData(rapidjson::Value value_data) {
	data_game.clear();
     std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        PairCharacter pc_tmp;
        auto character_data = value_data[i].GetArray();
        std::vector<Answer> data_answer;
        for (rapidjson::SizeType j = 0; j < character_data.Size(); j++) {
            auto image_id = character_data[j].FindMember("text")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, image_id);
          
            Character c_tmp;
            c_tmp.text = pImage->getText();
            c_tmp.audio = pImage->getAudios().front();
            c_tmp.image_path = pImage->path;
            c_tmp.compare_1 = pImage->getText("name_n") ;
            c_tmp.compare_2 = pImage->getText("color");
            c_tmp.compare_3 = pImage->getText("number_text");
            c_tmp.audio_1 = pImage->getAudios("name_n").front();
            c_tmp.audio_2 = pImage->getAudios("color").front();
            c_tmp.audio_3 = pImage->getAudios("number_text").front();
            //c_tmp.audio = character_data[j].FindMember("audio")->value.GetString();
            pc_tmp.characters.push_back(c_tmp);
            Answer a = { c_tmp.compare_1,c_tmp.audio_1, static_cast<int>(j) };
            Answer b = { c_tmp.compare_2,c_tmp.audio_2, static_cast<int>(j) };
            Answer c = { c_tmp.compare_3,c_tmp.audio_3, static_cast<int>(j) };
            data_answer.push_back(a);
            data_answer.push_back(b);
            data_answer.push_back(c);
        }
        mj::helper::shuffle(data_answer);
        data_answers.push_back(data_answer);
        data_game.push_back(pc_tmp);
    }
}

void CompareAndContrast::onEnter() {
	MJMainGBase::onEnter();
	disableTouch();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	showIntroGame();
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {
        
		//         this->inActiveMode();

	}), NULL));

	if (autoPlay) {
		this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=] {
			this->onAutoPlay();
		}), NULL));

	}
}

void CompareAndContrast::loadFrames() {
    playBackgroundMusic(bg_music);
    
	//root layout
	current_index = 0;
	current_answer_index = 0;

	//auto bg = utils::findChild<ImageView*>(this, "bg");
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	root_layout->setTouchEnabled(false);
	root_layout->addClickEventListener([=](Ref*) {
		if (auto guidingLayout = this->getChildByName("guide_layout")) {
			_guide_layout->removeFromParent();
		}	
		AUDIO_ENGINE->stopAllSound();
		CCLOG("clicked root_layout ...");
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});

	
	image_scorebar = ImageView::create("games/compareandcontrast/cnc_scorebar.png");
	image_scorebar->setPositionX(screen_size.width / 2);
	image_scorebar->setPositionY(screen_size.height * 0.9);
	root_layout->addChild(image_scorebar, 2);

	for (int i = 0; i < 6; i++) {
		GAFWrapper* score_star = GAFWrapper::createGAF(path_gaf_scorestar);
		image_scorebar->addChild(score_star, 2);
		score_star->setPositionY(image_scorebar->getContentSize().height*0.62);
		score_star->setPositionX(23 + i * (35));
		score_stars.push_back(score_star);
	}
	image_scorebar->setScale(0.7);
	image_scorebar->setVisible(false);
	
	gaf_title = GAFWrapper::createGAF(path_gaf_title);
	root_layout->addChild(gaf_title,2);
	gaf_title->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_title->setPositionX(screen_size.width/2);
	gaf_title->setPositionY(screen_size.height*0.6);
	
	gaf_guild = GAFWrapper::createGAF(cocos2d::StringUtils::format(path_gaf_guild,gaf_guild_type.c_str()));
	root_layout->addChild(gaf_guild, 2);
	gaf_guild->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_guild->setPositionX(screen_size.width*0.51);
	gaf_guild->setPositionY(screen_size.height*0.45);
	gaf_guild->setVisible(false);

	//
	this->genBoards();
	//
	gaf_effect_correct = GAFWrapper::createGAF(path_gaf_effect_correct);
	root_layout->addChild(gaf_effect_correct, 2);
	gaf_effect_correct->setVisible(false);
	gaf_effect_correct->setAnchorPoint(Vec2(0.5, 0.5));

	// touch layout
	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	touch_layout->setVisible(false);
//    touch_layout->addClickEventListener(CC_CALLBACK_0(<#__selector__#>, this));
	this->addChild(touch_layout, 10000000);

	back_layout = cocos2d::ui::Layout::create();
	back_layout->setTouchEnabled(false);
	back_layout->addClickEventListener([=](Ref*) {
		CCLOG("clicked back_layout ...");
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});
	back_layout->setContentSize(screen_size);
	back_layout->setBackGroundColor(cocos2d::Color3B::BLACK );
	back_layout->setBackGroundColorOpacity(180);
	back_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	back_layout->setVisible(false);
	this->addChild(back_layout, 500);

	// create speaker
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	_speaker->setVisible(false);
	_speaker->setTouchEnabled(false);
	_speaker->addClickEventListener([=](cocos2d::Ref* sender)
		{
			if (auto guidingLayout = this->getChildByName("guide_layout")) {
				_guide_layout->removeFromParent();
			}
			AUDIO_ENGINE->stopAllSound();
			CCLOG("clicked speaker ...");
			if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/compareandcontrast/cnc_guild_picture.mp3")) {
				AUDIO_ENGINE->playSound("sounds/compareandcontrast/cnc_guild_picture.mp3", false, ([=] {
					scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
					}));
			}		
		});
	this->addChild(_speaker);

	// play background music
	if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/compareandcontrast/bgm.mp3")) {
		AUDIO_ENGINE->playBackgroundMusic("sounds/compareandcontrast/bgm.mp3");
	}
}

void CompareAndContrast::genBoards() {
	for (auto s : score_stars) {
		s->reset();
	}
	boards.clear();
	// xu li bang 1
	BoardCharacter b1;
	b1.info = data_game[current_index].characters[0];
	gaf_charbg_1 = GAFWrapper::createGAF(path_gaf_charbg);
	auto char_panel_1 = utils::findChild<Layout*>(this, "Panel_1");
	char_panel_1->setVisible(true);
	
	b1.t_compare_1 = (Text*)char_panel_1->getChildByName("compare_1");
	b1.t_compare_2 = (Text*)char_panel_1->getChildByName("compare_2");
	b1.t_compare_3 = (Text*)char_panel_1->getChildByName("compare_3");
	b1.current_number = 0;
	b1.image_panel = (Layout *)char_panel_1->getChildByName("image_layout");
	b1.t_name = (Text*)b1.image_panel->getChildByName("Text");
	b1.image = (ImageView*)b1.image_panel->getChildByName("image");
	b1.bg1 = (ImageView *)b1.image_panel->getChildByName("image_bg1");
	b1.bg2 = (Layout *)b1.image_panel->getChildByName("image_bg2");
	b1.bg2->setTouchEnabled(false);
	b1.bg2->addClickEventListener([=](Ref*) {
		CCLOG("clicked table1_Layout ...");
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});

	b1.t_name->setString(b1.info.text);
	b1.image->loadTexture(b1.info.image_path);
	char_panel_1->addChild(gaf_charbg_1, b1.image->getZOrder() - 1);
	gaf_charbg_1->setPositionX(char_panel_1->getContentSize().width / 2 - 38);
	gaf_charbg_1->setPositionY(char_panel_1->getContentSize().height / 2 + 30);
	gaf_charbg_1->setVisible(false);
	b1.t_name->setVisible(false);
	b1.t_compare_1->setVisible(false);
	b1.t_compare_2->setVisible(false);
	b1.t_compare_3->setVisible(false);
	b1.image->setVisible(false);
	
	b1.bg1->setVisible(false);
	
	b1.bg2->setVisible(false);
	b1.panel = char_panel_1;
	boards.push_back(b1);
    pos_1 = b1.image_panel->getPosition();
    

	// xu li bang 2
	BoardCharacter b2;
	b2.info = data_game[current_index].characters[1];
	gaf_charbg_2 = GAFWrapper::createGAF(path_gaf_charbg);
	auto char_panel_2 = utils::findChild<Layout*>(this, "Panel_2");
	char_panel_2->setVisible(true);
	
	b2.t_compare_1 = (Text*)char_panel_2->getChildByName("compare_1");
	b2.t_compare_2 = (Text*)char_panel_2->getChildByName("compare_2");
	b2.t_compare_3 = (Text*)char_panel_2->getChildByName("compare_3");
	b2.current_number = 0;
	b2.image_panel = (Layout *)char_panel_2->getChildByName("image_layout");
	b2.t_name = (Text*)b2.image_panel->getChildByName("Text");
	b2.image = (ImageView*)b2.image_panel->getChildByName("image");
	b2.bg1 = (ImageView *)b2.image_panel->getChildByName("image_bg1");
	b2.bg2 = (Layout *)b2.image_panel->getChildByName("image_bg2");
	b2.bg2->setTouchEnabled(false);
	b2.bg2->addClickEventListener([=](Ref*) {
		CCLOG("clicked table1_Layout ...");
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});

	b2.t_name->setString(b2.info.text);
	b2.image->loadTexture(b2.info.image_path);
	b2.image->removeAllChildrenWithCleanup(true);
	
	char_panel_2->addChild(gaf_charbg_2, b2.image->getZOrder() - 1);
	gaf_charbg_2->setPositionX(char_panel_2->getContentSize().width / 2 - 38);
	gaf_charbg_2->setPositionY(char_panel_2->getContentSize().height / 2 + 30);
	gaf_charbg_2->setVisible(false);
	b2.t_name->setVisible(false);
	b2.t_compare_1->setVisible(false);
	b2.t_compare_2->setVisible(false);
	b2.t_compare_3->setVisible(false);
	b2.image->setVisible(false);
	
	b2.bg1->setVisible(false);

	b2.bg2->setVisible(false);
	b2.panel = char_panel_2;
	boards.push_back(b2);
    pos_2 = b2.image_panel->getPosition();
	
	current_answer_index = 0;

    //text scale
    font_sz = b1.t_name->getFontSize();

//    b1.t_name->setScale(0.5);
//    b2.t_name->setScale(0.5);
    
	// add board click
	b1.image->setTouchEnabled(false);
	b1.image->addClickEventListener([=](cocos2d::Ref* sender) {
		CCLOG("CLICK BOARD");
		onTapBoardImage(b1);
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});
	b2.image->setTouchEnabled(false);
	b2.image->addClickEventListener([=](cocos2d::Ref* sender) {
		CCLOG("CLICK BOARD");
		onTapBoardImage(b2);
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
		});
}

void CompareAndContrast::showGame() {
	float time_1 = 0.5f;
	this->showBoards();
	gaf_title->runAction(EaseBackOut::create(MoveBy::create(time_1*2, Vec2(0, screen_size.height*0.4))));
	image_scorebar->runAction(EaseBackOut::create(MoveBy::create(time_1, Vec2(0, screen_size.height*0.2))));
	image_scorebar->runAction(ScaleTo::create(time_1, 1.f));

	// create item
	
}

void CompareAndContrast::showBoards() {
	// show board 1
	this->scheduleOnce([=](float) {
		gaf_charbg_1->setVisible(true);
		this->scheduleOnce([=](float) {
			boards[0].bg1->setVisible(true);
			boards[0].bg2->setVisible(true);
			boards[0].image->setVisible(true);
			boards[0].image->setOpacity(0);
			boards[0].image->runAction(FadeIn::create(0.15));
			showTextLBL(boards[0].t_compare_1, 0.5f, path_sound_text);
			showTextLBL(boards[0].t_compare_2, 0.5f, path_sound_text);
			showTextLBL(boards[0].t_compare_3, 0.5f, path_sound_text);
			showTextLBL(boards[0].t_name, 1.f, path_sound_textname);

			this->scheduleOnce([=](float) {
				AUDIO_ENGINE->playSound(boards[0].info.audio);
			}, 0.5f, "play audio ten nhan vat1");
		}, 0.5f, "show charactera 1");
		AUDIO_ENGINE->playSound(path_sound_paper);
		gaf_charbg_1->play(false);
	}, 1.f, "show character 1");
	// show board 1
	this->scheduleOnce([=](float) {
		gaf_charbg_2->setVisible(true);
		this->scheduleOnce([=](float) {
			boards[1].bg1->setVisible(true);
			boards[1].bg2->setVisible(true);
			boards[1].image->setVisible(true);
			boards[1].image->setOpacity(0);
			boards[1].image->runAction(FadeIn::create(0.15));
			showTextLBL(boards[1].t_compare_1, 0.5f, path_sound_text);
			showTextLBL(boards[1].t_compare_2, 0.5f, path_sound_text);
			showTextLBL(boards[1].t_compare_3, 0.5f, path_sound_text);
			showTextLBL(boards[1].t_name, 1.f, path_sound_textname);
			this->scheduleOnce([=](float) {
				AUDIO_ENGINE->playSound(boards[1].info.audio, false, [=] {
					this->scheduleOnce([=](float) {
						enableTouch();
						this->showQuestion();
					}, 0.5f, "show question");
				});
			}, 1.2f, "play audio ten nhan vat2");
		}, 0.6f, "show charactera 2");
		AUDIO_ENGINE->playSound(path_sound_paper);
		gaf_charbg_2->play(false);
	}, 2.9f, "show character 2");

	for (int i = 0; i < 2; i++) {
		boards[i].panel->setTouchEnabled(true);
		boards[i].panel->addClickEventListener([=](Ref*) {
			if (auto guidingLayout = this->getChildByName("guide_layout")) {
				_guide_layout->removeFromParent();
			}
			CCLOG("cliked board panel");
			if (_speaker->isTouchEnabled()) {
				AUDIO_ENGINE->stopAllSound();
			}
			scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
			});
	}
}

void CompareAndContrast::playGuidingBySound()
{
	CCLOG("Play Sound guiding ...");
	if (cocos2d::experimental::AudioEngine::getPlayingAudioCount() == 1) {
		AUDIO_ENGINE->playSound("sounds/compareandcontrast/cnc_guild_picture.mp3", false, ([=] {
			scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
			}));
	}
	else {
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
	}
}

void CompareAndContrast::enableTouch()
{
	touch_layout->setVisible(false);
}

void CompareAndContrast::disableTouch()
{
	touch_layout->setVisible(true);
}

void CompareAndContrast::showQuestion() {
	this->unschedule("guidingByHand");
	CompareAndContrast_Item* currentItem = CompareAndContrast_Item::createItem(data_answers[current_index][current_answer_index].text, data_answers[current_index][current_answer_index].key, data_answers[current_index][current_answer_index].audio);
	currentItem->setPositionX(screen_size.width / 2);
	currentItem->setPositionY(screen_size.height *0.7);
	root_layout->addChild(currentItem, 2);
	currentItem->setDelegate(this);
    tap = true;
    AUDIO_ENGINE->play(path_sound_answer, false, 1.0, [=]{
        tap = false;
    });
	disableTouch();
	currentItem->showItem();
	AUDIO_ENGINE->playSound(data_answers[current_index][current_answer_index].audio, false, ([=] {
		_speaker->setTouchEnabled(true);
		_speaker->setVisible(true);
		root_layout->setTouchEnabled(true);
		back_layout->setTouchEnabled(true);
		enableTouch();
		for (int i = 0; i < 2; i++) {
			boards[i].image->setTouchEnabled(true);
		}
		}));
	currentItem->setTouchHandleType(CompareAndContrastItemTouchType::TOUCH);
	current_answer_index++;

	for (int i = 0; i < 2; i++) {
		if (i == currentItem->id) {
			_answer_id = i;
			scheduleGuideByHand(currentItem->getPosition(), boards[i].panel->getPosition());
		}
	}
}

void CompareAndContrast::showTextLBL(Text* text, float time, std::string path_sound) {
	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(path_sound);
	}, time, cocos2d::StringUtils::format("play sound show text delay theo %f",time));
	text->setVisible(true);
	int length = text->getStringLength();
	for (int i = 0; i < length; i++) {
		//CCLOG("TEST %d",text->getLetter(i)->getOpacity());
		if (text->getLetter(i)!=nullptr)
			text->getLetter(i)->setOpacity(0);
	}
	for (int i = 0; i < length; i++) {
		if (text->getLetter(i) != nullptr)
		text->getLetter(i)->runAction(Sequence::create(DelayTime::create(time + i*0.01),FadeIn::create(0.01),NULL));
	}
}

void CompareAndContrast::onExitTransitionDidStart() {
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void CompareAndContrast::onClickedBox(cocos2d::Ref * sender)
{
	auto obj = static_cast<CompareAndContrast_Item *>(sender);
	//AUDIO_ENGINE->playSound(obj->getInfo().audio);
	//SoundManager::getInstance()->playSound(obj->getInfo().audio_phonic);
	//hideGuideGame();
}

void CompareAndContrast::onMoveBeginBox(cocos2d::Ref * sender)
{
	// remove guiding
	this->unschedule("guidingByHand");
	if (auto guidingLayout = this->getChildByName("guide_layout")) {
		_guide_layout->removeFromParent();
	}
	auto obj = static_cast<CompareAndContrast_Item *>(sender);
	//AUDIO_ENGINE->stopEffect(audio_id);
	AUDIO_ENGINE->stopAllSound();
	AUDIO_ENGINE->playSound(obj->getAudio());
	old_zorder = obj->getZOrder();
	obj->setZOrder(100000);
	this->hideTapGuideGame();
	kt_sound_interact = false;
    
    for(auto b: boards){
        b.image->setTouchEnabled(false);
    }
}
void CompareAndContrast::onMovingBox(cocos2d::Ref * sender)
{
	auto obj = static_cast<CompareAndContrast_Item *>(sender);
	Vec2 pos = obj->getPosition();

	for (int i = 0; i < 2; i++) {
		if (boards[i].panel->getBoundingBox().containsPoint(pos) ) {
			boards[i].panel->setScale(1.1);
			for (auto u : boards[i].items) {
				u->setScale(0.99);
			}
			if (kt_sound_interact == false) {
				AUDIO_ENGINE->playSound(path_sound_interact);
				kt_sound_interact = true;
				key_interact = i;
			}
		}
		else {
			boards[i].panel->setScale(1);
			for (auto u : boards[i].items) {
				u->setScale(0.9);
			}
			if (i == key_interact) kt_sound_interact = false;
		}
	}

}

void CompareAndContrast::onTapBoardImage(BoardCharacter board)
{
	if (auto guidingLayout = this->getChildByName("guide_layout")) {
		_guide_layout->removeFromParent();
	}
	AUDIO_ENGINE->stopAllSound();
	_isTapBoardImage = true;
    showed_board = board;
    int index = -1;
    for(int i = 0; i < boards.size(); i++){
        if(showed_board.image == boards[i].image ){
            index = i;
        }
    }
    if(index == 0){
        start_pos = pos_1;
    }else{
        start_pos = pos_2;
    }
     clone_pos = board.panel->convertToWorldSpace(start_pos);
    
//     start_pos = board.image_panel->getPosition();
//     clone_pos = board.panel->convertToWorldSpace(start_pos);

    if(!tap){
        tap = true;
        auto hide_img_layout = Layout::create();
        addChild(hide_img_layout, INT_MAX-1, "hide_img");
        hide_img_layout->setContentSize(screen_size);
      
        hide_img_layout->addClickEventListener(CC_CALLBACK_0(CompareAndContrast::onHideImage, this));
        
        board.image_panel->retain();
        board.image_panel->removeFromParentAndCleanup(false);
        board.image_panel->setPosition(clone_pos);
        board.t_name->setFontSize(font_sz*2);
        
        this->addChild(board.image_panel, 501);
        back_layout->setVisible(true);
        board.image_panel->runAction(cocos2d::Sequence::create( cocos2d::Spawn::create(MoveTo::create(0.5f, screen_size/2), ScaleTo::create(0.5f, 2.5f), CallFunc::create([=]{
            board.t_name->setScale(0.5);
        }), NULL), CallFunc::create([=] {
            AUDIO_ENGINE->playSound(board.info.audio);
        }),CallFunc::create([=]{
              hide_img_layout->setTouchEnabled(true);
        }), NULL));
        for(auto b: boards){
            b.image->setTouchEnabled(false);
        }
    }
}

void CompareAndContrast::onHideImage(){
	_isTapBoardImage = false;
	AUDIO_ENGINE->stopAllSound();
    utils::findChild(this, "hide_img")->removeFromParent();
    auto board = showed_board;
    board.image_panel->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(MoveTo::create(0.5f, clone_pos), ScaleTo::create(0.5f, 1.0f), NULL), CallFunc::create([=] {
        back_layout->setVisible(false);
        board.image_panel->removeFromParentAndCleanup(false);
        board.panel->addChild(board.image_panel, 1000);
         board.image_panel->setPosition(start_pos);
        board.t_name->setFontSize(font_sz);
         board.t_name->setScale(1);
        tap = false;
    }), CallFunc::create([=]{
        for(auto b: boards){
            b.image->setTouchEnabled(true);
        }
    }),
		CallFunc::create([=] {
		CCLOG("omHideImage!");
		scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), boards[_answer_id].panel->getPosition());
			}),
		NULL));
}

void CompareAndContrast::trueAction(int index_board, CompareAndContrast_Item* item) {
	item->setTouchEnable(false);
	item->setAnchorPoint(Vec2(0.5, 0.5));
	boards[index_board].panel->setScale(1);
	for (auto i : boards[index_board].items) {
		i->setScale(0.9);
	}
	
	Vec2 pos_text_compare;
	if (boards[index_board].current_number == 0) {
		pos_text_compare = boards[index_board].panel->convertToWorldSpace(boards[index_board].t_compare_1->getPosition());
	}
	if (boards[index_board].current_number == 1) {
		pos_text_compare = boards[index_board].panel->convertToWorldSpace(boards[index_board].t_compare_2->getPosition());
	}
	if (boards[index_board].current_number == 2) {
		pos_text_compare = boards[index_board].panel->convertToWorldSpace(boards[index_board].t_compare_3->getPosition());
	}
	item->runAction(MoveTo::create(0.1, pos_text_compare));
	item->runAction(ScaleTo::create(0.1,0.9));
	gaf_effect_correct->setPositionX(pos_text_compare.x - 100);
	gaf_effect_correct->setPositionY(pos_text_compare.y + 100);
	gaf_effect_correct->setVisible(true);
	gaf_effect_correct->setZOrder(item->getZOrder() + 1);
	gaf_effect_correct->play(false);
	boards[index_board].current_number++;
	boards[index_board].items.push_back(item);
	
	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(path_sound_score);
		score_stars[current_answer_index - 1]->play(false);
		this->nextGame();
	}, 0.5f, cocos2d::StringUtils::format("update scoreboard"));
}

void CompareAndContrast::onMoveEndedBox(cocos2d::Ref * sender)
{
	CCLOG("onMoveEndedBox");
	bool kt = false;
	auto object = static_cast<CompareAndContrast_Item *>(sender);
	auto pos = object->getPosition();
	for (int i = 0; i < 2; i++) {
		if (boards[i].panel->getBoundingBox().containsPoint(pos)) {
			if (i == object->id) {
				CCLOG("DUNG");
				_speaker->setTouchEnabled(false);
				disableTouch();
				AUDIO_ENGINE->stopSound(path_sound_correct);
				AUDIO_ENGINE->playSound(path_sound_correct);
				trueAction(i, object);
				object->setZOrder(old_zorder);
				_needGuidingByHand = false;
				return;
			}
			else {
				CCLOG("SAI");
				AUDIO_ENGINE->stopSound(path_sound_wrong);
				AUDIO_ENGINE->playSound(path_sound_wrong);
				boards[i].panel->setScale(1);
				for (auto u : boards[i].items) {
					u->setScale(0.9);
				}	
				CCLOG("sellect wrong!");
				scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height*0.7), Vec2(boards[object->id].panel->getPosition()));
                
			}
		}
		else {
			scheduleGuideByHand(Vec2(screen_size.width / 2, screen_size.height * 0.7), Vec2(boards[object->id].panel->getPosition()));
		}
	}
	object->setZOrder(old_zorder);
	object->backToStart();
    for(auto b: boards){
        b.image->setTouchEnabled(true);
    }

}

void CompareAndContrast::onNextStage() {
	this->unscheduleAllCallbacks();

}

void CompareAndContrast::onFinishGame() {
	MJMainGBase::onFinishGame();
}
void CompareAndContrast::nextStage() {
	for (auto b : boards) {
		b.panel->setVisible(false);
		for (auto i : b.items) {
			i->setVisible(false);
		}
	}
	gaf_charbg_1->setVisible(false);
	gaf_charbg_2->setVisible(false);
	current_index++;
	this->genBoards();
	this->showBoards();
}
void CompareAndContrast::nextGame() {
	if (auto guidingLayout = this->getChildByName("guide_layout")) {
		_guide_layout->removeFromParent();
	}
	this->unschedule("guidingByHand");
	this->unscheduleAllCallbacks();
	if (current_answer_index >= 6) {
		disableTouch();
        
		if (current_index + 1  < so_luot_choi ) {
			this->scheduleOnce([=](float dt) {
				this->nextStage();
			}, 1.5f, cocos2d::StringUtils::format("nextStage"));
		}
		else {
			this->scheduleOnce([=](float dt) {
				this->onFinishGame();
			}, 0.8f, cocos2d::StringUtils::format("finish"));
		}
	}
	else {
		this->scheduleOnce([=](float dt) {
			this->showQuestion();
		}, 1.0f, cocos2d::StringUtils::format("next show answer"));
	}
	
}


void CompareAndContrast::showIntroGame()
{
	AUDIO_ENGINE->playSound(path_sound_title);
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(path_sound_gamename);
	}, 0.5f, "show game");
	/*AudioEngine::setFinishCallback(id_title, [=](int au_id, std::string path) {
	AUDIO_ENGINE->playSound(path_sound_gamename);
	});*/
	gaf_title->play(false, [=](gaf::GAFObject* obj) {
		gaf_title->runAction(ScaleTo::create(0.3, 0.4));
		gaf_title->runAction(EaseBackOut::create(MoveBy::create(0.3, Vec2(0, screen_size.height*0.3))));
		this->scheduleOnce([=](float) {
			image_scorebar->setVisible(true);
			AUDIO_ENGINE->playSound(path_sound_scorebar);
			image_scorebar->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -screen_size.height*0.15))));
		}, 1.f, "show scrorebar");
		AUDIO_ENGINE->playSound(path_sound_guild);
		
		gaf_guild->setVisible(true);
		gaf_guild->playSequence("fade_in", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			AUDIO_ENGINE->play(cocos2d::StringUtils::format(path_sound_guild_text, gaf_guild_type.c_str()), false, 1.0, [=] {
                this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    AUDIO_ENGINE->playSound(path_sound_fadeout);
                    gaf_guild->playSequence("face_out", false, [=](gaf::GAFObject* obj, string path){
                        gaf_guild->setVisible(false);
                    });
                   
                    this->showGame();
               
                }), NULL));
			});
		});
	});

}

void CompareAndContrast::scheduleGuideByHand(Vec2 pos1, Vec2 pos2, std::string gaf_guide_path) {
	CCLOG("on schedule Guiding by hand");
	if (_needGuidingByHand) {
			CCLOG("start schedule guiding by hand");
			this->unschedule("guidingByHand");			
			this->schedule([=](float) {
				if (_isTapBoardImage == false) {
					_guide_layout = cocos2d::ui::Layout::create();
					_guide_layout->setContentSize(screen_size);
					this->addChild(_guide_layout);
					_guide_layout->setName("guide_layout");

					auto hand = GAFWrapper::createGAF(gaf_guide_path);
					_guide_layout->addChild(hand, 1900000000);
					hand->setPosition(pos1);
					hand->setScale(0.5);
					playGuidingBySound();
					hand->play(false, [=](gaf::GAFObject* obj) {
						hand->runAction(Sequence::create(MoveTo::create(0.6f, pos2),
							CallFunc::create([=]() {
								hand->play(false, [=](gaf::GAFObject* obj) {
									hand->removeFromParent();
									if (auto guidingLayout = this->getChildByName("guide_layout")) {
										_guide_layout->removeFromParent();
									}
									});
								}),
							CallFunc::create([=] {scheduleGuideByHand(pos1, pos2); })
									, NULL));
						});
				}				
			}, 12.0f, 100, 12.0f, "guidingByHand");	
	}
	else {
		CCLOG("Start schedule guiding by sound");
		this->scheduleOnce([=](float) {
			playGuidingBySound();
			}, 10.0f, "schedulePlayGuidingSound");
	}
}

void CompareAndContrast::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
	}, 7, 100, delay_time, "animated_guide");

}



ui::Widget::ccWidgetClickCallback CompareAndContrast::onLocateClickCallback(const std::string &callback_name) {
	return nullptr;
}
//void CompareAndContrast::showIntroGame(){
//    auto touch_layout = cocos2d::ui::Layout::create();
//    touch_layout->setContentSize(screen_size);
//    touch_layout->setTouchEnabled(true);
//    this->addChild(touch_layout, 1900000000);
//
//    auto audio_path = std::string(title);
//    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
//    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
//        touch_layout->removeFromParent();
//    });
//    this->showTapGuideGame(pos_guide);
//}

#define auto_testing_delay 1.5f
void CompareAndContrast::onAutoPlay()
{
	this->scheduleOnce([=](float dt) {
		this->autoPlayRun(1.5f);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CompareAndContrast::autoPlayRun(float dt)
{
	this->scheduleOnce([=](float dt) {
		this->onAutoPlay();
	}, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}
