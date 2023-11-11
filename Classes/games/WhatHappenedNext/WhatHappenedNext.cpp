#include "WhatHappenedNext.h"
#include "MJHelper.h"
std::once_flag WhatHappenedNext_Reader;
INITIALIZE_READER(WhatHappenedNext);

using namespace cocos2d;

#define CSB_NAME "csb/game/WhatHappenedNext/WhatHappenedNext.csb"

#define gaf_title "gaf/whathappenednext/whn_title/whn_title.gaf"
#define gaf_star "gaf/whathappenednext/whn_stars/DtD_stars.gaf"

#define sfx_answer "sounds/whathappenednext/whn_answer.mp3"
#define sfx_carriage "sounds/whathappenednext/whn_carriage.mp3"
#define sfx_correct "sounds/whathappenednext/whn_correct.mp3"
#define sfx_fadein "sounds/whathappenednext/whn_fadein_o.mp3"
#define sfx_train "sounds/whathappenednext/whn_train.mp3"
#define sfx_fadeout "sounds/whathappenednext/whn_fadeout.mp3"
#define sfx_ghost_carriage "sounds/whathappenednext/whn_ghost_carriage.mp3"
#define sfx_interact "sounds/whathappenednext/whn_interact.mp3"
#define sfx_wrong "sounds/whathappenednext/whn_wrong.mp3"
#define sfx_command "sounds/whathappenednext/order.mp3"
#define gamename "sounds/whathappenednext/Gamename.mp3"
#define audio_whn "sounds/whathappenednext/What happened next.mp3"
#define audio_title "sounds/whathappenednext/intro.mp3"
#define bg_music "sounds/whathappenednext/bg_music.mp3"

WhatHappenedNext* WhatHappenedNext::createGame() {
	std::call_once(WhatHappenedNext_Reader, [] {
		REGISTER_CSB_READER(WhatHappenedNext);
	});
	auto p = static_cast<WhatHappenedNext*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p)
	{
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void WhatHappenedNext::didLoadFromCSB() {
    generateData();
    loadFrame();
    if (_billboardWhite)
    {
        _billboardWhite->setTouchEnabled(false);
    }
}

void WhatHappenedNext::onEnter() {
	MJMainGBase::onEnter();

}

void WhatHappenedNext::generateData() {
    
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

    std::vector<math::resource::Image*> m_images = math::resource::ResHelper::getImages("icons.json");
    
	rapidjson::Value& json_data = document.FindMember("data")->value;
	for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
		whn_data game_dat;
		rapidjson::Value& json_answer = json_data[i].FindMember("answer")->value;

        std::string right_answer = json_answer.FindMember("right_answer")->value.GetString();
        auto right_image = math::resource::ResHelper::getImage(m_images, right_answer);
        game_dat.right_answer_audio = right_image->getAudios("name_1").front();
        game_dat.right_answer_text = right_image->getText("name_1");

        std::string wrong_answer = json_answer.FindMember("wrong_answer")->value.GetString();
        auto wrong_image = math::resource::ResHelper::getImage(m_images, wrong_answer);
        game_dat.wrong_answer_text = wrong_image->getText("name_1");
        game_dat.wrong_answer_audio = wrong_image->getAudios("name_1").front();

        std::string json_question = json_data[i].FindMember("question")->value.GetString();
        auto question_image = math::resource::ResHelper::getImage(m_images, json_question);
        game_dat.question = question_image->getText("name_1");
        game_dat.question_image = question_image->path;
        game_dat.question_audio = question_image->getAudios("name_1").front();
    
		
		list_game_data.push_back(game_dat);
	}

	number_stage = list_game_data.size();
	mj::helper::shuffle(list_game_data);
}

void WhatHappenedNext::loadFrame() {
    
    AUDIO_ENGINE->playEffect(bg_music, true, 0.5);
    
	screen_size = Director::getInstance()->getVisibleSize();
	root_layout = utils::findChild<ui::Layout*>(this, "root_layout");
	train = utils::findChild<ui::Layout*>(root_layout, "train");
	station = utils::findChild(root_layout, "station");
    billboard = utils::findChild(root_layout, "billboard");
    _billboardWhite = utils::findChild<ui::Layout*>(billboard, "billboard_white");
	carriage = utils::findChild(train, "carriage");
    _imgCarriage = ui::ImageView::create("games/whathappenednext/whn_carriage_norm.png");
    _imgCarriage->setPosition(carriage->getPosition());
    _imgCarriage->setAnchorPoint(Point(0.5, 0.5));
    train->addChild(_imgCarriage, 99999);
    carriage->removeFromParent();
	carriage_white = utils::findChild(train, "carriage_white");
    carriage_dash = utils::findChild(train, "whn_carriage_dashes_18");

    _imageView = ui::ImageView::create();
    billboard->addChild(_imageView,1);
    _imageView->setPosition(_billboardWhite->getPosition());
    _imageView->setContentSize(Size(_billboardWhite->getContentSize().width*0.95, _billboardWhite->getContentSize().height*0.9));
    _imageView->ignoreContentAdaptWithSize(false);

    auto size = _imageView->getContentSize();
    train_pos = Vec2(train->getPositionX() + 1000, train->getPositionY()) ;
    train->setPosition(train_pos);


    _imgCarriage->setVisible(false);
    carriage_white->setVisible(false);
    carriage_dash->setVisible(false);

	for (int i = 0; i < 2; i++) {
        std::string str = "a_" + StringUtils::toString(i+1);
		auto carriage_light = utils::findChild<ui::Layout*>(this, str);
		list_carriage.push_back(carriage_light);
	}

	star = GAFWrapper::createGAF(gaf_star);
	star->setVisible(false);
    train->addChild(star, 9999);
    star->setPosition(carriage_white->getPosition());
    

	title = GAFWrapper::createGAF(gaf_title);
	title->setScale(0.5);
	_billboardWhite->addChild(title);
	title->setPosition(_billboardWhite->getContentSize() / 2);

    this->setAnchorPoint(Point(0.05, 1));
    this->setIgnoreAnchorPointForPosition(true);
    this->setScale(1.8);
    
	this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
		title->play(false);
		AUDIO_ENGINE->playSound(sfx_fadein);
        int id = cocos2d::experimental::AudioEngine::play2d(gamename);
		cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, std::string path) {
            AUDIO_ENGINE->playSound(sfx_train);
		});
	}), DelayTime::create(2.5), CallFunc::create([=] {
        this->runAction(ScaleTo::create(0.5, 1));
	}), DelayTime::create(2), CallFunc::create([=] {
                _speakerBtn->setVisible(true);
				onStageBegin();
	}), NULL));
    _speakerBtn = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speakerBtn->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    _speakerBtn->setVisible(false);
    _speakerBtn->addClickEventListener([=](cocos2d::Ref* sender) {
        removeAndStopActionHand();
        if (item_correct)
        {

            AUDIO_ENGINE->stopSound(item_correct->getAudio());
        }
        playChooseItem(NULL, sfx_command);
        });
    this->addChild(_speakerBtn);
}

void WhatHappenedNext::onStageBegin() {
    _speakable = false;
    _speakerBtn->setTouchEnabled(_speakable);

    _imgCarriage->setVisible(false);
    carriage_white->setVisible(false);
    _deplayForShowGuideline = 0;
    _firstTimeGuiding = 0;
	list_item.clear();
	for (int i = 0; i < 2; i++) {
		Item* item = new Item("");
		if (i == 0) {
			item->setText(list_game_data[stage_count].right_answer_text);
			item->setAudio(list_game_data[stage_count].right_answer_audio);
		}
		else {
			item->setText(list_game_data[stage_count].wrong_answer_text);
			item->setAudio(list_game_data[stage_count].wrong_answer_audio);
		}
		root_layout->addChild(item, 2000);
		if (i == 0) {
			item->setIsRight(true);
		}
		else {
			item->setIsRight(false);
		}
		item->getLayout()->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				onClickItem(sender);
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				onMoveItem(sender);
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				onMoveItemEnd(sender);
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
                onMoveItemEnd(sender);
				break;
			default:
				break;
			}
		});
		list_item.push_back(item);
	}

	mj::helper::shuffle(list_item);
	for (int i = 0; i < list_item.size(); i++) {
		list_item[i]->setPosition(list_carriage[i]->getPosition());
		list_item[i]->setStartPos(list_carriage[i]->getPosition());
		list_item[i]->setVisible(false);
	}
    _billboardWhite->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            onClickToImage(sender, type);
        }
        });

    train->runAction(EaseOut::create(MoveTo::create(3.5, Vec2(station->getPositionX(), station->getPositionY() - 30)), 3));

    AUDIO_ENGINE->play(sfx_train);
	this->runAction(Sequence::create(DelayTime::create(3.5), CallFunc::create([=] {
		title->setVisible(false);
		_imageView->loadTexture(list_game_data[stage_count].question_image);

        question = ui::Text::create(list_game_data[stage_count].question, font_name, font_sz);
        question->setTextAreaSize(Size(_imgCarriage->getContentSize().width - 20, _imgCarriage->getContentSize().height - 10));
        question->setTextColor(Color4B::BLACK);
        question->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        question->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        _imgCarriage->addChild(question, 99);
        question->setPosition(_imgCarriage->getContentSize() / 2);
        _imgCarriage->setVisible(true);
        _imgCarriage->setTouchEnabled(false);
        _imgCarriage->addClickEventListener([=](cocos2d::Ref* sender) {
            removeAndStopActionHand();
            AUDIO_ENGINE->stopSound(_pathAudio);
            if (item_correct)
            {
                AUDIO_ENGINE->stopSound(item_correct->getAudio());
            }
            AUDIO_ENGINE->stopSound(_audioCommand);
            AUDIO_ENGINE->playSound(_audioCommand, false, [=]() {
                _speakable = true;
                readCommandAndShowGuide();
                });
            });

        _audioCommand = list_game_data[stage_count].question_audio;
		AUDIO_ENGINE->playSound(sfx_carriage);
		AUDIO_ENGINE->playSound(list_game_data[stage_count].question_audio, false, [=]() {
            AUDIO_ENGINE->playSound(audio_whn, false, [=](){
                
                carriage_white->setVisible(true);
                carriage_dash->setVisible(true);
                AUDIO_ENGINE->playSound(sfx_ghost_carriage);
                
                list_item[0]->setVisible(true);
                AUDIO_ENGINE->playSound(sfx_ghost_carriage);
                list_item[0]->runAction(ScaleTo::create(0.2, 1.2));
                
                AUDIO_ENGINE->playSound(list_item[0]->getAudio(), false, [=]() {
                    list_item[0]->runAction(ScaleTo::create(0.2, 1));
                    list_item[1]->setVisible(true);
                    AUDIO_ENGINE->playSound(sfx_ghost_carriage);
                    list_item[1]->runAction(ScaleTo::create(0.2, 1.2));
                    AUDIO_ENGINE->playSound(list_item[1]->getAudio(), false, [=]() {
                        list_item[1]->runAction(ScaleTo::create(0.2, 1));
                        
                        if(stage_count == 0){
                            //TODO: menh lenh
                            AUDIO_ENGINE->playSound(sfx_command, false, [=](){
                                for (Item* item : list_item) {
                                    item->setTouchEnable(true);
                                }
                                _speakable = true;
                                _speakerBtn->setTouchEnabled(_speakable);
                                _next_state = false;

                                _billboardWhite->setTouchEnabled(true);
                                _imgCarriage->setTouchEnabled(true);
                                readGafBlinklink();
                                });
                        }
                        else {
                            for (Item* item : list_item) {
                                item->setTouchEnable(true);
                            }
                            _speakable = true;
                            _speakerBtn->setTouchEnabled(_speakable);
                            _next_state = false;
                            _billboardWhite->setTouchEnabled(true);
                            _imgCarriage->setTouchEnabled(true);
                            readGafBlinklink();

                        }

                        
                    });
                });
            });
		});
	}), DelayTime::create(0.5), CallFunc::create([=] {
		CCLOG("Train pos 1 = %f, %f", train->getPosition().x, train->getPosition().y);
	}), NULL));
}

void WhatHappenedNext::onClickItem(Ref* sender) {
	CCLOG("On Clicked Item!");
    AUDIO_ENGINE->play("sounds/whathappenednext/Pop.wav");
	auto choose = (ui::Layout*) sender;
	for (Item* item : list_item) {
		if (item->getLayout() == choose) {
			item_choose = item;
		}
	}
    playChooseItem(item_choose, item_choose->getAudio());
}
void WhatHappenedNext::playChooseItem(Item* chooseItem, std::string audio_path ) {
    
    AUDIO_ENGINE->stopSound(_pathAudio);
    AUDIO_ENGINE->stopSound(item_correct->getAudio());
    AUDIO_ENGINE->stopSound(_audioCommand);
    removeAndStopActionHand();
    if (audio_path == "")
        return;
    
    _speakable = false;
    _pathAudio = audio_path;
    AUDIO_ENGINE->playSound(audio_path, false, [=]() {
        _speakable = true;
        readCommandAndShowGuide();
        });
}


void WhatHappenedNext::onMoveItem(Ref* sender) {

	item_choose->setLocalZOrder(3000);
	Vec2 move_pos = item_choose->getLayout()->getTouchMovePosition();
	item_choose->setPosition(root_layout->convertToNodeSpace(move_pos));
    item_choose->runAction(ScaleTo::create(0.2, 1));
    CCLOG("item pos: %f, %f", item_choose->getPositionX(), item_choose->getPositionY());
    AUDIO_ENGINE->stopSound(item_choose->getAudio());
}

void WhatHappenedNext::onMoveItemEnd(Ref* sender) {

    removeAndStopActionHand();
    auto pos = item_choose->getParent()->convertToWorldSpace(item_choose->getPosition());

    auto carriage_pos = carriage_white->getParent()->convertToWorldSpace( carriage_white->getPosition());
	auto size = carriage_white->getContentSize();
    item_choose->runAction(ScaleTo::create(0.2, 1));
    
    
    
    CCLOG("distance: %f", pos.getDistance(carriage_pos));
    if( pos.getDistance(carriage_pos) <= size.width/2){
        if(item_choose->isRight()){
            _speakable = false;
            _speakerBtn->setTouchEnabled(_speakable);
            for (Item* item : list_item) {
                item->getLayout()->setTouchEnabled(false);
            }
            item_choose->retain();
            auto pos = item_choose->getParent()->convertToWorldSpace(item_choose->getPosition());
            item_choose->removeFromParent();
            train->addChild(item_choose);
            item_choose->setPosition(train->convertToNodeSpace(pos));
            item_choose->runAction(MoveTo::create(0.2, carriage_white->getPosition()));
            _billboardWhite->setTouchEnabled(false);
            _imgCarriage->setTouchEnabled(false);
            star->setLocalZOrder(3001);
            star->setVisible(true);
            star->play(false, [=](gaf::GAFObject* ){
                star->setVisible(false);
            });
            if (item_choose)
            {
                AUDIO_ENGINE->stopSound(item_choose->getAudio());
            }
            
            AUDIO_ENGINE->playSound(sfx_correct);
            AUDIO_ENGINE->playSound(StringUtils::format("sounds/whathappenednext/khen_%d.mp3", random(1, 3)));
            
            
            auto pos_1 = _imgCarriage->getPosition();
            auto pos_2 = carriage_white->getPosition();
            this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=]() {
                train->runAction(MoveTo::create(1, Vec2(-1500, train_pos.y)));
                AUDIO_ENGINE->play(sfx_train);
                stage_count++;
            }), DelayTime::create(1.5), CallFunc::create([=]() {
                question->removeFromParent();
                    _imgCarriage->setPosition(pos_1);
                carriage_white->setPosition(pos_2);
                train->setPosition(train_pos);
                
                item_choose->retain();
                item_choose->removeFromParent();
                root_layout->addChild(item_choose);
                item_choose = nullptr;
                if (stage_count < number_stage) {
                    onNextStage();
                }
                else {
                        _imgCarriage->setVisible(false);
                    carriage_white->setVisible(false);
                    for (Item* item : list_item) {
                        item->removeFromParentAndCleanup(true);
                    }
                    onFinishGame();
                }
                
            }), NULL));
            return;
        }
        else {
            item_choose->wrongItem();
            AUDIO_ENGINE->playSound(sfx_wrong);
            readCommandAndShowGuide();
        }
    }
    else {
        readCommandAndShowGuide();
    }

	item_choose->backToStart();
	item_choose->setLocalZOrder(2000);
    
    this->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create([=]{
    }), NULL));
}
void WhatHappenedNext::onClickToImage(Ref* pSender, Widget::TouchEventType type) {
    CCLOG("On Clicked Item!");
    AUDIO_ENGINE->play("sounds/whathappenednext/Pop.wav");
    AUDIO_ENGINE->stopSound(_pathAudio);
    if (item_correct)
    {
        AUDIO_ENGINE->stopSound(item_correct->getAudio());
    }
    AUDIO_ENGINE->stopSound(_audioCommand);
    if (_audioCommand == "")
        return;
    _speakable = false;
    billboard->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create([=] {
        if (auto gafStar = utils::findChild(_imageView, "star_bink_bink"))
        {
            gafStar->removeFromParent();
        }

        }), NULL));
    removeAndStopActionHand();
    AUDIO_ENGINE->playSound(_audioCommand, false, [=]() {
        readCommandAndShowGuide();
        _speakable = true;
        });
}
void WhatHappenedNext::onNextStage() {

    _imgCarriage->setVisible(false);
	carriage_white->setVisible(false);
    carriage_dash->setVisible(false);
    _next_state = true;
	for (Item* item : list_item) {
		item->removeFromParentAndCleanup(true);
	}
    _billboardWhite->setTouchEnabled(false);
    _imgCarriage->setTouchEnabled(false);
	onStageBegin();
}

void WhatHappenedNext::onFinishGame() {
	MJMainGBase::onFinishGame();
}

void WhatHappenedNext::readGafBlinklink()
{
   
    _imageView->schedule([=](float) {
        auto gafStar = GAFWrapper::createGAF("khung/sao.gaf");
        gafStar->setAnchorPoint(Point(0.5, 0.5));
        gafStar->setPosition(cocos2d::Vec2(_imageView->getBoundingBox().size / 2));
        gafStar->setName("star_bink_bink");
        gafStar->runAction(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node* n) {
            reinterpret_cast<GAFWrapper*>(n)->play(true);
            }), DelayTime::create(1.25), NULL));
        _imageView->addChild(gafStar, 10000);
        }, 1, 0, 2, "animated_blink");
    readCommandAndShowGuide();
}
void WhatHappenedNext::readCommandAndShowGuide()
{
    for (Item* item : list_item) {
        if (item->isRight()) {
            item_correct = item;
        }
    }

        auto correctPos = item_correct->getParent()->convertToWorldSpace(item_correct->getPosition());

        auto targetPos = carriage_white->getParent()->convertToWorldSpace(carriage_white->getPosition());

    int audioLength = std::count(list_game_data[stage_count].right_answer_text.begin(), list_game_data[stage_count].right_answer_text.end(), ' ');

    _firstTimeGuiding++;
    suggetionGame(correctPos, targetPos);
}

void WhatHappenedNext::suggetionGame(Vec2 posStart, Vec2 posEnd)
{
    removeAndStopActionHand();
    this->schedule([=](float) {
        auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");
        hand->setName("hand_suggetion_game");
        this->addChild(hand, INT_MAX);
        AUDIO_ENGINE->stopSound(_pathAudio);
        if (item_correct)
        {
            AUDIO_ENGINE->playSound(sfx_command, false, [=]() {
                suggetionGame(posStart, posEnd);
                });
        }
        hand->setPosition(posStart);
        hand->play(false, [=](gaf::GAFObject* obj) {
            hand->runAction(Sequence::create(MoveTo::create(1.5f, posEnd),
                RemoveSelf::create()
                    , NULL));
            });
        }, 8, 1000, 10.0f, "readCommandAgain");
}

void WhatHappenedNext::removeAndStopActionHand()
{
    this->unschedule("readCommandAgain");
    if (auto hand = utils::findChild(this, "hand_suggetion_game"))
    {
        hand->stopAllActions();
        hand->removeFromParent();
    }

}
