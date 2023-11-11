//
//  BreakTheEgg.h
//  Created by HoiSung on 02/02/18.
//
//

#include "BreakTheEgg.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"
#define num_egg 3

#define GAME_NAME "hdr/games/breaktheegg/knock the egg.png"
#define SOUND_GAME_NAME "sounds/breaktheegg/Knock_the_Egg.mp3"
#define SOUND_TAP_THE_EGG "sounds/breaktheegg/Tap_the_egg_to_get_the_word.mp3"

using namespace cocos2d;
BreakTheEgg * BreakTheEgg::createBreakTheEgg(std::string json_file, std::string game_type) {

	BreakTheEgg * obj = new BreakTheEgg();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(json_file);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void BreakTheEgg::didLoadFromCSB(std::string json_file) {

	m_json_config = json_file;
    m_images = math::resource::ResHelper::getImages("icons.json");

}

void BreakTheEgg::generateData() {
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_json_config);
	rapidjson::Document value_data;
	bool is_convert_plist_2_json = false;
	if (is_convert_plist_2_json) {
		auto value_map = FileUtils::getInstance()->getValueMapFromFile(m_json_config);
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

	//generateData(value_data.GetObject());
	auto data_info = value_data["data"].GetArray();
	int game_index = cocos2d::random(0, (int)data_info.Size() - 1);
	auto game_info = data_info[game_index]["game"].GetArray();
	for (auto i = 0; i < game_info.Size(); ++i) {
		mj::PHONIC_INFO phonic_info;
		auto phonic_data = game_info[i].GetObject();
		auto image_id = phonic_data["image_id"].GetString();
		auto image = math::resource::ResHelper::getImage(m_images, image_id);
		phonic_info.image_path = image->path;
		phonic_info.text_full = image->getText("name_1");
		phonic_info.audio_full = image->getAudios("name_1").front();
		phonic_info.text_phonic = image->getText("name_n");
		phonic_info.audio_phonic = image->getAudios("name_n").front();
		if (phonic_data.HasMember("video_id") && phonic_data["video_id"].IsString()) {
			auto video_id = phonic_data["video_id"].GetString();
			auto video = math::resource::ResHelper::getImage(m_images, video_id);
//            phonic_info.video_path = video->getVideo().path;
		}
		if (phonic_data.HasMember("highlight") && phonic_data["highlight"].IsArray()) {
			auto highlight = phonic_data["highlight"].GetArray();
			for (auto j = 0; j < highlight.Size(); ++j) {
				phonic_info.highlights.push_back(highlight[j].GetInt());
			}
		}
		phonic_info.highlight_color = cocos2d::Color3B::RED;
		phonic_info.normal_color = cocos2d::Color3B::BLACK;
		_list_phonic_info.push_back(phonic_info);
	}
}
BreakTheEgg * BreakTheEgg::createBreakTheEgg(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type) {

	BreakTheEgg * obj = new BreakTheEgg();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(list_phonic_info, is_auto_play);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void BreakTheEgg::didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play) {
	_list_phonic_info = list_phonic_info;
	this->is_auto_play = is_auto_play;
}

void BreakTheEgg::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	if (m_json_config.size() > 5) {
		generateData();
	}
	this->showInformationGame();
	

}

void BreakTheEgg::loadFrame() {
	radio();

	float h_egg = 0.6;
	std::vector<Vec2> list_pos = { Vec2(screen_size.width*0.225, screen_size.height*h_egg), Vec2(screen_size.width / 2, screen_size.height*(h_egg + 0.1)), Vec2(screen_size.width*0.775, screen_size.height*h_egg) };
	mj::helper::shuffle(_list_phonic_info);
	std::vector<int> list_type_egg = { 1, 2, 3 };
	mj::helper::shuffle(list_type_egg);

	for (int i = 0; i < num_egg; i++) {
		auto egg_item = BreakTheEgg_EggItem::createItem(_list_phonic_info[i], list_type_egg[i], is_auto_play);
		egg_item->setPosition(list_pos[i]);
		this->addChild(egg_item);
		egg_item->setDelegate(this);
		egg_item->setTag(101 + i);
		list_egg.push_back(egg_item);
		egg_item->setLocalZOrder(10 - i % 2);
	}
	this->playBackgroundMusic("sounds/breaktheegg/breaktheegg_background.mp3");

	pos_guide = Vec2(screen_size.width * 0.5, screen_size.height * 0.45);
	egg_state = BreakTheEgg_EggItem::RUNNING;
	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		auto rand_egg = random(0, 2);
		current_egg = list_egg[rand_egg];
		current_egg->showEgg([=] {
			this->showIntroGame();
			});
		}), NULL));

}

void BreakTheEgg::radio()
{
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(60, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	_speaker->addClickEventListener([=](Ref*) {
		unschedule("showTutorial");
		AUDIO_ENGINE->stopSound(SOUND_TAP_THE_EGG);
		AUDIO_ENGINE->playSound(SOUND_TAP_THE_EGG, false, [=]() {
			showTutorial();
			});
		});
}

void BreakTheEgg::showIntroGame() {
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 1900000000);

	auto audio_path = std::string("sounds/breaktheegg/breaktheegg_guidemc.mp3");
    if (CONFIG_MANAGER->isGameVMonkey()) audio_path = std::string("sounds/breaktheegg/breaktheegg_guidemc_vi.mp3");
	auto audio_id = AUDIO_ENGINE->playSound(audio_path, false, [=]() {
		showTutorial();
		},1.0f);
//	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
//		touch_layout->removeFromParent();
//		if (is_auto_play) {
//			this->onClickedEggDelegate(current_egg);
//		}
//	});
    /* fix lỗi nếu act trước là story và bấm ghi âm, sau đó bật đoạn ghi âm thì sang game này ko touch được quả trứng
     * Nguyên nhân: khi bấm bật ghi âm thì callback finish không được gọi nên touch_layout không đc remove
     * Xử lý: thay bằng action khác viết ở dưới
     */
    this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=](){
        auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
        
        this->runAction(Sequence::create(DelayTime::create(duration - 0.1f), CallFunc::create([=](){
            touch_layout->removeFromParent();
            if (is_auto_play) {
                this->onClickedEggDelegate(current_egg);
            }
        }), NULL));
    }), NULL));
    
	
}

void BreakTheEgg::onClickedEggDelegate(cocos2d::Ref *sender) {
	unschedule("showTutorial");
	AUDIO_ENGINE->stopSound(SOUND_TAP_THE_EGG);
	showTutorial();
	if (!current_egg) return;
	auto egg_choose = static_cast<BreakTheEgg_EggItem *> (sender);
	if (egg_state == BreakTheEgg_EggItem::RUNNING || egg_state == BreakTheEgg_EggItem::BREAKING) return;
	if (egg_state == BreakTheEgg_EggItem::SHOWED || egg_state == BreakTheEgg_EggItem::ACTION) {
		if (current_egg == egg_choose) {
			current_egg->breakEggAction();
			if (is_auto_play) {
				this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
					this->onClickedEggDelegate(current_egg);
				}), NULL));
			}
			else this->showTutorial();
		}
		else {
			current_egg->hideEgg();
			egg_choose->showEgg([=] {
				showTutorial();
			});
			current_egg = egg_choose;
		}
		return;
	}

	//    if (egg_state == BreakTheEgg_EggItem::FINISH) {
	//        current_egg = egg_choose;
	//        current_egg->showEgg();
	//        return;
	//    }
}

void BreakTheEgg::onChangeEggStateDelegate(int state) {
	egg_state = state;
	if (egg_state == BreakTheEgg_EggItem::TEXT_SHOWED) {
		if (!current_egg) return;
		mj::PHONIC_BOARD_INFO board_info;
		board_info.video_path = current_egg->_item_info.video_path;
		board_info.image_path = current_egg->_item_info.image_path;
		board_info.text_full = current_egg->_item_info.text_full;
		board_info.audio_full = current_egg->_item_info.audio_full;
		board = BreakTheEgg_Board::createItem(board_info);
		board->setDelegate(this);
		this->addChild(board, 10000);
	}
}

void BreakTheEgg::onBoardStartHide() {
	count_phonic_done++;
	current_egg->setEggFinish();
	int index_egg = current_egg->egg_type;
	current_egg = nullptr;
	this->scheduleOnce([=](float dt) {
		if (count_phonic_done >= 3) {
			current_egg = nullptr;
			this->onFinishGame();
		}
		else {
			for (int i = 0; i < (int)list_egg.size(); i++) {
				if (index_egg == list_egg[i]->egg_type) {
					list_egg.erase(list_egg.begin() + i);
					break;
				}
			}
			for (int i = 0; i < (int)list_egg.size(); i++) {
				CCLOG("list_index = %d", list_egg[i]->egg_type);

			}
			auto rand_egg = random(0, (int)list_egg.size() - 1);
			current_egg = list_egg[rand_egg];
			CCLOG("list_egg size = %d | index_egg = %d | new_index = %d", (int)list_egg.size(), index_egg, current_egg->egg_type);

			current_egg->showEgg([=] {
				if (is_auto_play) {
					this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
						this->onClickedEggDelegate(current_egg);
						}), NULL));
				}
				else showTutorial();
			});
		}
	}, 1.05f, "onBoardStartHide");
}

void BreakTheEgg::onFinishGame() {
	CCLOG("onFinishGame Override");
	MJMainGBase::onFinishGame();
}

void BreakTheEgg::onExitTransitionDidStart() {
	MJMainGBase::onExitTransitionDidStart();
}

void BreakTheEgg::showInformationGame() {

	screen_size = cocos2d::Director::getInstance()->getVisibleSize();

	auto orgin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto bg = cocos2d::ui::ImageView::create("games/breaktheegg/breaktheegg_backgroud.jpg");
	this->addChild(bg);
	bg->setPosition(orgin + screen_size / 2);

	const float k_currScreenRatio = (screen_size.width / screen_size.height);

	if (k_currScreenRatio >= 19.f / 9.f) //20/9,21/9
	{
		bg->setScale((1.1f * screen_size.width) / bg->getContentSize().width);//110%
	}
	this->runAction(Sequence::create(DelayTime::create(0.5f)
		, CallFunc::create([=]() {
			if (auto infoGame = Sprite::create(GAME_NAME)) {
				auto visibleSize = Director::getInstance()->getVisibleSize();
				infoGame->setAnchorPoint(Vec2(0.5, 0.5));
				infoGame->setPosition(Vec2(-500, visibleSize.height / 2));
				if (visibleSize.width / visibleSize.height < 1.5) {
					infoGame->setScale(0.6);
				}
				else {
					if (visibleSize.width / visibleSize.height < 1.8) {
						infoGame->setScale(0.8);
					}
					else {
						infoGame->setScale(1);
					}
				}
				infoGame->setName("gameName");
				this->addChild(infoGame);

				infoGame->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.0f, Vec2(visibleSize / 2)))
					, DelayTime::create(0.5f)
					, CallFunc::create([=]() {
						AUDIO_ENGINE->playSound(SOUND_GAME_NAME);
						})
					, DelayTime::create(3.0f)
							, MoveTo::create(0.5f, Vec2(visibleSize.width + 500, visibleSize.height / 2)), NULL));
			}
			})
		, DelayTime::create(5.5f)
				, CallFunc::create([=]() {
				if (auto child = utils::findChild(this, "gameName")) {
					child->removeFromParent();
				}
				this->loadFrame();
					}), NULL));
}

void BreakTheEgg::showTutorial() {
	schedule([=](float) {
		AUDIO_ENGINE->playSound(SOUND_TAP_THE_EGG);
		},10.0f,100, 10.0f, "showTutorial");
}
