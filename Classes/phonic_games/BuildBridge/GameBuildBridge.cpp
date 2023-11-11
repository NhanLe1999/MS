//
//  GameBuildBridge.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/5/18.
//

#include "GameBuildBridge.h"
#include "GAFWrapper.h"
#include "GameBuildBridge_Piece.h"
#include "GameBuildBridge_Mask.h"

#include "AudioEngine.h"

USING_NS_CC;
INITIALIZE_READER(GameBuildBridge);

std::once_flag game_build_bridge_reader_flag;

GameBuildBridge * GameBuildBridge::createGame(std::string config_path)
{
	std::call_once(game_build_bridge_reader_flag, [] {
		REGISTER_CSB_READER(GameBuildBridge);
	});

	auto view = reinterpret_cast<GameBuildBridge *>(CSLoader::createNodeWithVisibleSize("csb/game/build_bridge/build_bridge.csb"));
	if (view) {
		view->didLoadFromCSB(config_path);
		return view;
	}

	CC_SAFE_DELETE(view);
	return view;
}

void GameBuildBridge::generateData()
{
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_configPath);
	rapidjson::Document value_data;

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

	_isAudioIntro = JSON_SAFE_CHECK(value_data, "hide_text", Bool) ? value_data["hide_text"].GetBool() : false;

	if (JSON_SAFE_CHECK(value_data, "audio_intro", String))
	{
		auto keyAudioIntro = value_data["audio_intro"].GetString();
		auto pImage = math::resource::ResHelper::getImage(m_images, keyAudioIntro);
		_audioIntroPath = pImage->getAudios().front();
	}

	if (value_data.HasMember("number_turnplay") && value_data["number_turnplay"].IsInt()) {
		number_turnplay = value_data["number_turnplay"].GetInt();
	}


	_data.clear();
	if (value_data.HasMember("data") && value_data["data"].IsArray()) {
		auto list_item_data = value_data["data"].GetArray();
		for (int i = 0; i < (int)list_item_data.Size(); i++) {
			auto item_data = list_item_data[i]["item"].GetObject();

			RoundInfo item_info;
			auto letter = item_data["text_full"].GetString();
			auto pImage = math::resource::ResHelper::getImage(m_images, letter);
			if (pImage) {
				item_info.word = pImage->getText("name_1");
				item_info.audio = pImage->getAudios("name_1").front();
				item_info.slow_audio = pImage->getAudios("name_n").front();
				item_info.image = pImage->path;
				//TODO: audio and text phonic
				auto phonics = pImage->getTextPhonic("name_p");

				//Cho nay cho sua
				std:vector<std::string> phonic_audios;
				if (!_isAudioIntro)
				{
					phonic_audios = pImage->getAudioPhonic("name_p");
				}

				for (int j = 0; j < (int)phonics.size(); j++) {
					RoundPhonicInfo phonic;
					phonic.phonic = phonics[j];
					if (!_isAudioIntro)
					{
						phonic.audio_phonic = phonic_audios[j];
					}
					item_info.phonics.push_back(phonic);
				}
				_data.push_back(item_info);
			}
		}
	}
	//if (value_data.HasMember("is_shuffle") && value_data["is_shuffle"].IsBool() && value_data["is_shuffle"].GetBool()) {
	//	mj::helper::shuffle(_data);
	//}
	//if (number_turnplay < _data.size()) {
	//	_data.resize(number_turnplay);
	//}
}

void GameBuildBridge::didLoadFromCSB(std::string config_path) {

	m_configPath = config_path;
	is_first_round = true;
	addSpeaker();
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);

	
	auto img_bg = cocos2d::utils::findChild<ui::ImageView*>(this, "bg");
	img_bg->setLocalZOrder(-1);
	auto BtB_bg_cloud = GAFWrapper::createGAF("gaf/build_bridge/BtB_bg_cloud/BtB_bg_cloud.gaf");
	BtB_bg_cloud->setPosition(Vec2(screen_size / 2));
	BtB_bg_cloud->play(true);
	this->addChild(BtB_bg_cloud,- 1);

	auto BtB_bg_lands = GAFWrapper::createGAF("gaf/build_bridge/BtB_bg_lands/BtB_bg_lands.gaf");
	BtB_bg_lands->setPosition(Vec2(screen_size.width / 2,250) );
	BtB_bg_lands->play(true);
	this->addChild(BtB_bg_lands,  - 1);

	//auto img_tree = cocos2d::utils::findChild<ui::ImageView*>(this, "tree");
	//float fix_pos = 0;
	//if (screen_size.width > 1024) {
	//	fix_pos = 30;
	//	img_tree->setScaleX(1.0f);
	//}

	_nat = GAFWrapper::createGAF("gaf/build_bridge/nat/nat.gaf");
	this->addChild(_nat);

	auto treeL = GAFWrapper::createGAF("gaf/build_bridge/treeL/treeL.gaf");
	treeL->play(true);
	treeL->setPosition(Vec2(screen_size.width / 2 - 570, 600));
	this->addChild(treeL);

	auto treeR = GAFWrapper::createGAF("gaf/build_bridge/treeR/treeR.gaf");
	treeR->setPosition(Vec2(screen_size.width / 2 + 535, 590));
	treeR->play(true);
	this->addChild(treeR);		
	
	_camera = GAFWrapper::createGAF("gaf/build_bridge/camera/camera.gaf");
	_camera->setVisible(false);
	_camera->setPosition(screen_size / 2);
	this->addChild(_camera);

	_word = ui::Layout::create();
	this->addChild(_word, 1);
	_word->setPosition(Point(this->getContentSize().width / 2, 625));
	_word->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);

	auto list_top = utils::findChild<ui::ListView *>(this, "list_top");
	auto list_bottom = utils::findChild<ui::ListView *>(this, "list_bottom");
	list_top->setScrollBarEnabled(false);
	list_bottom->setScrollBarEnabled(false);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	touch_layout->setVisible(false);
	addChild(touch_layout, 1000);
    
}

void GameBuildBridge::onEnter() {
	MJMainGBase::onEnter();
	playBackgroundMusic("sounds/build_bridge/BtB_bgm.mp3");
	generateData();
	startRound();
}

void GameBuildBridge::startRound() {
	if (_data.empty()) {
        AUDIO_ENGINE->stopAllAudio();
		MJMainGBase::onFinishGame(/*CHARACTER::NAT*/);
		return;
	}
	wrong_count = 0;

	_current_round = _data.front(); _data.erase(_data.begin());
	_char_filled.clear();
	_char_filled.resize(_current_round.word.size(), false);
	_phonics_copy.clear();
	std::copy(_current_round.phonics.begin(), _current_round.phonics.end(), std::back_inserter(_phonics_copy));
	if (is_first_round) {
		is_first_round = false;
		showRound();
	}
	else {
		onTransitionRound();
	}
	
}

void GameBuildBridge::showRound() {

	auto nat_pos = Point(screen_size.width / 2 - 412, 290);
	auto mask = GameBuildBridge_Mask::createMask();
	mask->setPosition(nat_pos);
	this->addChild(mask, 10);

	AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_fade_in.mp3");
	_nat->setPosition(Point(-100, 290));
	_nat->runAction(Sequence::create(MoveTo::create(1, nat_pos), CallFunc::create([this, mask] {
		AUDIO_ENGINE->playSound("sounds/build_bridge/intro1.mp3", false);
		_nat->playSequence("run_stop", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("wrong", false);
		});
	}), DelayTime::create(1.25f), CallFunc::create([this, mask] {
		mask->focus(1);
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_binocular.mp3");
		_nat->playSequence("look_on", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("look_idle", false);
		});
	}), nullptr));
	_nat->playSequence("run", true);


    setCameraMaskVisible(true);
    _camera->setOpacity(255);
    _camera->playSequence("fade_in",false,[=](gaf::GAFObject * obj, const std::string & sequenceName){
        _camera->setOpacity(0);
        setCameraMaskVisible(false);
    });
	mask->runAction(Sequence::create(DelayTime::create(3.5), CallFunc::create([this] {
		initStoneLight();
	}), MoveTo::create(1, Vec2(screen_size.width / 2 + 450, 275)), DelayTime::create(0.5), CallFuncN::create([this](Node * n) {
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_binocular.mp3");
		_nat->playSequence("look_off", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("idle", false);
		});
		reinterpret_cast<GameBuildBridge_Mask *>(n)->unFocus(1);
		onTapStone(stone_light);
	}), //DelayTime::create(3), 
		CallFunc::create([this] {
		/*_nat->playSequence("wrong", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("idle", false);
		});*/
		AUDIO_ENGINE->playSound("sounds/build_bridge/intro2.mp3", false, [=] {
			std::vector<std::string> chain;
			std::string audioIntro = "sounds/build_bridge/Make the word.mp3";
			if (_audioIntroPath != "")
			{
				audioIntro = _audioIntroPath;
			}
			chain.push_back(audioIntro);
			
			chain.push_back(_current_round.audio);
			AUDIO_ENGINE->playChain(chain);
			showText();
		});

	}), RemoveSelf::create(), nullptr));
}

void GameBuildBridge::showText() {
	_underlines.clear();

	for (auto i = 0; i < (int)StringUtils::getCharacterCountInUTF8String(_current_round.word); ++i) {
		auto c = ui::Text::create(StringUtils::toString(_current_round.word[i]), "fonts/UVNVAN_0.TTF", 100);
		c->setTextColor(Color4B::WHITE);
		c->enableOutline(Color4B(0x27, 0x97, 0xD4, 0xdd), 5);
		c->setAnchorPoint(Point::ANCHOR_MIDDLE);
		c->setPosition(Point(50 + 90 * i + 10 * i, 50));
		c->setTag(i);
		c->setVisible(false);
		_word->addChild(c);

		auto underline = ui::Layout::create();
		underline->setBackGroundColor(Color3B::WHITE);
		underline->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
		underline->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
		underline->setContentSize(Size(0, 10));
		underline->setPosition(Point(c->getPositionX(), -10));

		underline->runAction(Sequence::create(DelayTime::create(i * 0.1), EaseBackOut::create(ResizeTo::create(0.15, Size(90, 10))), NULL));

		_word->addChild(underline);
		_underlines.push_back(underline);
	}
	_word->setContentSize(Size((int)_current_round.word.length() * 100 - 10, 0));

	text_lb = cocos2d::ui::Text::create(_current_round.word, "fonts/UVNVAN_0.TTF", 100);
	text_lb->setPosition(Vec2(_word->getContentSize().width / 2, 50));
	text_lb->setVisible(false);
	text_lb->setTextColor(Color4B::WHITE);
	text_lb->enableOutline(Color4B(0x27, 0x97, 0xD4, 0xdd), 5);
	_word->addChild(text_lb);

	this->scheduleOnce([this](float) {
		showBubbles();
	}, 0.5, "delay_bubbles");
}

#include "MJHelper.h"
void GameBuildBridge::showBubbles() {
	//    mj::helper::shuffle(_current_round.phonics);

	std::map<int, std::vector<int>> piece_map;

	piece_map.emplace(0, std::vector<int>({ 0, 1, 2 }));
	auto average = 10 / ((int)_current_round.phonics.size() - 1);
	auto remain = 10 % ((int)_current_round.phonics.size() - 1);
	auto last_piece = 3;
	for (auto i = 1; i < (int)_current_round.phonics.size(); ++i) {
		std::vector<int> piece;
		for (auto c = 0; c < average; ++c) {
			piece.push_back(last_piece++);
		}
		if (remain) {
			piece.push_back(last_piece++);
			remain--;
		}

		piece_map.emplace(i, piece);
	}

	bubbles.clear();
	for (auto i = 0; i < (int)_current_round.phonics.size(); ++i) {
		auto bubble = GAFWrapper::createGAF("gaf/build_bridge/bubble/bubble.gaf");
		bubble->setPosition(this->getContentSize() / 2);

		auto layout = ui::Layout::create();
		layout->setContentSize(Size(150, 150));
		layout->setPosition(bubble->getContentSize() / 2);
		layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
		layout->setTouchEnabled(true);
		layout->setOpacity(0x55);
		layout->setName("touch_layout");
		layout->addClickEventListener([=](Ref * sender) {
			auto bubble = reinterpret_cast<Node *>(sender)->getParent();
			this->onBubbleClicked(bubble);
		});
		bubble->addChild(layout);

		auto c = ui::Text::create(_current_round.phonics[i].phonic, "fonts/UVNVAN_0.TTF", 100);
		c->setTextColor(Color4B::WHITE);
		c->enableOutline(Color4B(0x27, 0x97, 0xD4, 0xdd), 6);
		c->setAnchorPoint(Point::ANCHOR_MIDDLE);
		c->setPosition(Point(bubble->getContentSize() / 2) + Point(0, 15));
		c->setName("text");
		c->setVisible(false);
		bubble->addChild(c, 1);
		bubble->setScale(1.2);

		auto size = Size(150, 150);
		auto label_size = c->getContentSize();
		float fSize = c->getFontSize();
		while (label_size.width > size.width * 0.75 && label_size.height > size.height * 0.75) {
			fSize -= 2;
			c->setFontSize(fSize);
			label_size = c->getContentSize();
		}

		auto pieces = piece_map[i];
		for (auto u = 0; u < (int)pieces.size(); ++u) {
			auto piece = GameBuildBridge_Piece::createPiece(pieces[u]);
			piece->setPosition(bubble->getContentSize() / 2);
			piece->setName("piece");
			piece->setTag(u);
			bubble->addChild(piece, -1);
		}

		bubbles.push_back(bubble);
	}

	mj::helper::shuffle(bubbles);

	if ((int)_current_round.phonics.size() <= 2) {
		auto list_top = utils::findChild<ui::ListView *>(this, "list_top");
		list_top->setItemsMargin(-75);

		auto w = 0;
		for (auto i = 0; i < (int)_current_round.phonics.size(); ++i) {
			list_top->pushBackCustomItem(bubbles[i]);
			w += bubbles[i]->getContentSize().width + list_top->getItemsMargin();
		}
		w -= list_top->getItemsMargin();
		list_top->setContentSize(Size(w, 150));

		list_top->doLayout();
	}
	else {
		auto list_top = utils::findChild<ui::ListView *>(this, "list_top");
		list_top->setItemsMargin(-75);
		auto list_bottom = utils::findChild<ui::ListView *>(this, "list_bottom");
		list_bottom->setItemsMargin(-75);

		auto w = 0;
		for (auto i = 0; i < ((int)_current_round.phonics.size() + 1) / 2; ++i) {
			list_top->pushBackCustomItem(bubbles[i]);
			w += bubbles[i]->getContentSize().width + list_top->getItemsMargin();
		}
		w -= list_top->getItemsMargin();
		list_top->setContentSize(Size(w, 150));

		w = 0;
		for (auto i = ((int)_current_round.phonics.size() + 1) / 2; i < (int)_current_round.phonics.size(); ++i) {
			list_bottom->pushBackCustomItem(bubbles[i]);
			w += bubbles[i]->getContentSize().width + list_bottom->getItemsMargin();
		}
		w -= list_bottom->getItemsMargin();

		list_bottom->setContentSize(Size(w, 150));

		list_top->doLayout();
		list_bottom->doLayout();
	}

	auto counter = 0;
	for (auto b : bubbles) {
		b->setVisible(false);
		b->runAction(Sequence::create(DelayTime::create(counter++ * 0.15), Show::create(), CallFuncN::create([](Node * n) {
			n->enumerateChildren("piece", [](Node * n) {
				reinterpret_cast<GameBuildBridge_Piece *>(n)->show();
				return false;
			});
			//            auto piece = n->getChildByName<GameBuildBridge_Piece *>("piece");
			//            piece->show();
		}), DelayTime::create(0.25), CallFuncN::create([](Node * n) {
			auto gaf = reinterpret_cast<GAFWrapper *>(n);
			auto text = gaf->getChildByName("text");
			text->setVisible(true);

			text->setScale(0);
			text->setOpacity(0x00);
			text->runAction(ScaleTo::create(0.5, 1));
			text->runAction(FadeIn::create(0.25));

			gaf->playSequence("fade_in", false, [](gaf::GAFObject * gaf, std::string) {
				gaf->playSequence("idle", true);
			});
		}), NULL));
	}

	inactiveMode();
}

void GameBuildBridge::initStoneLight()
{
	stone_light = GAFWrapper::createGAF("gaf/build_bridge/stone_light/stone_light.gaf");
	this->addChild(stone_light);
	stone_light->addClickEventListener(CC_CALLBACK_1(GameBuildBridge::onTapStone, this));
	stone_light->setPosition(Vec2(screen_size.width / 2 + 480, 275));
	stone_light->play(true);
	stone_light->setScale(2.0);

	auto image = ui::ImageView::create(_current_round.image);
	image->setScale9Enabled(true);
	image->setCapInsets(Rect(Point::ZERO, image->getContentSize()));
	image->setContentSize(Size(70, 70));
	image->setPosition(Vec2(stone_light->getContentSize() / 2) + Vec2(10, 0));
	stone_light->addChild(image,1,"image");
}

void GameBuildBridge::setCameraMaskVisible(bool visible)
{

}

void GameBuildBridge::onBubbleClicked(cocos2d::Ref *sender) {
	activeMode();
	inactiveMode();
	auto bubble = reinterpret_cast<GAFWrapper *>(sender);
	bubble->stopAllActions();
	bubble->setScale(1.0f);
	CCLOG("%f, %f", bubble->getTouchBeganPosition().x, bubble->getTouchEndPosition().y);
	auto text = bubble->getChildByName<ui::Text *>("text");
	auto phonic = text->getString();
	bubble->playSequence("tap");
	AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_bubble_tap.mp3");
	auto current_phonic = _current_round.phonics.front();

	// true
	if (phonic == current_phonic.phonic) {
		wrong_count = 0;
		// sound effects
		bool is_first_letter = (_current_round.word.find_first_of(phonic) == 0);
		AUDIO_ENGINE->playSound(current_phonic.audio_phonic);

		bubble->removeChildByName("touch_layout");

		// bubble effect
		bubble->enumerateChildren("piece", [is_first_letter](Node * n) {
			n->runAction(Sequence::create(DelayTime::create(n->getTag() * 0.2), CallFuncN::create([is_first_letter](Node * n) {
				reinterpret_cast<GameBuildBridge_Piece *>(n)->onTapRight([=] {
					if (is_first_letter && n->getTag() == 0) {
						AUDIO_ENGINE->playSound("sounds/build_bridge/BtB_correct_pin.mp3");
					}
					else if (is_first_letter && n->getTag() == 1) {
						AUDIO_ENGINE->playSound("sounds/build_bridge/BtB_correct_rope.mp3");
					}
					else {
						AUDIO_ENGINE->playSound("sounds/build_bridge/BtB_correct_parts.mp3");
					}
				});

			}), NULL));
			return false;
		});

		bubble->runAction(Sequence::create(DelayTime::create(0.15), CallFunc::create([bubble] {
			AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_correct_stars.mp3");
			auto fx = GAFWrapper::createGAF("gaf/build_bridge/effect_stars/effect_stars.gaf");
			fx->setPosition(bubble->getContentSize() / 2);
			bubble->addChild(fx);
			fx->play(false);
		}), DelayTime::create(0.15), CallFunc::create([text] {
			text->runAction(EaseBackIn::create(ScaleTo::create(0.25, 0)));
		}), NULL));

		// nat effect
		AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/build_bridge/BtB_nat_correct%d.mp3", cocos2d::random(1, 3)));
		_nat->playSequence("correct", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			_nat->playSequence("idle");
		});

		// merge text
		for (auto u = 0; u < (int)phonic.size(); ++u) {
			for (auto i = 0; i < (int)_char_filled.size(); ++i) if (!_char_filled[i] && _current_round.word[i] == phonic[u]) {
				_char_filled[i] = true;
				auto char_img = _word->getChildByTag(i);
				char_img->runAction(Sequence::create(DelayTime::create(0.5), Show::create(), CallFuncN::create([](Node * n) {
					n->setScale(0);
					n->setOpacity(0x00);
					auto fx = GAFWrapper::createGAF("gaf/build_bridge/text_effect/text_effect.gaf");
					fx->setPosition(n->getContentSize() / 2);
					n->addChild(fx);
					fx->playSequence("fade_outv");
					AUDIO_ENGINE->playSound("sounds/build_bridge/BtB_text_transition.mp3");
				}), Spawn::create(EaseBackOut::create(ScaleTo::create(0.25, 1)), FadeIn::create(0.15), NULL), CallFunc::create([=] {
				}), NULL));
				_underlines[i]->runAction(FadeOut::create(0.25));
				break;
			}
		}
		_current_round.phonics.erase(_current_round.phonics.begin());
		// checck next game
		if (_current_round.phonics.empty()) {

			scheduleOnce([=](float dt) {
				onMergeText();
			}, 1.0f, "merge text");
		}
	}
	// false
	else {
		auto saved_position = text->getPosition();
		bubble->enumerateChildren("piece", [](Node * n) {
			reinterpret_cast<GameBuildBridge_Piece *>(n)->onTapWrong();
			return false;
		});
		AUDIO_ENGINE->playSound("sounds/build_bridge/BtB_nat_wrong.mp3");
		_nat->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			_nat->playSequence("idle");
		});
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_sfx_wrong.mp3");
		text->runAction(EaseQuinticActionIn::create(MoveBy::create(0.5, Vec2(0, -1500))));
		bubble->runAction(Sequence::create(DelayTime::create(0.5), CallFuncN::create([text, saved_position](Node * n) {
			text->stopAllActions();
			text->setPosition(Point(n->getContentSize() / 2) + Point(0, 15));
			text->setScale(0);
			text->setOpacity(0x00);
			text->runAction(ScaleTo::create(0.5, 1));
			text->runAction(FadeIn::create(0.25));
			reinterpret_cast<GAFWrapper *>(n)->playSequence("fade_in", false, [](gaf::GAFObject * g, std::string) {
				g->playSequence("idle", true);
			});
		}), NULL));

		// hint 
		if (++wrong_count >= 3) {

			// show bubble hint
			//wrong_count = 0;
			for (auto item : bubbles) {
				// bubble chua duoc cham
				if (item->getChildByName("touch_layout")) {
					auto text = item->getChildByName<ui::Text *>("text");
					auto phonic = text->getString();
					if (current_phonic.phonic == phonic) {
						item->stopAllActions();
						item->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.5f, 1.25f), ScaleTo::create(0.5f, 1.0f), NULL), 5));
						return;
					}
				}
			}
		}

	}
}

void GameBuildBridge::onTransitionGame()
{
	activeMode();
	AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_fade_out.mp3");
	stone_light->stopAllActions();
	stone_light->setTouchEnabled(false);
	_nat->playSequence("runpre", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		_nat->playSequence("run", true);
	});
	ccBezierConfig path;
	path.controlPoint_1 = Vec2(screen_size.width / 2 - 100, 150);
	path.controlPoint_2 = Vec2(screen_size.width / 2 + 100, 150);
	path.endPosition = Vec2(screen_size.width / 2 + 368, 295);

	_nat->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(100, 0)), CallFunc::create([=] {
		_camera->setOpacity(255);
        setCameraMaskVisible(true);
        _camera->playSequence("fade_out");
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_win_act.mp3");
        
	}), cocos2d::BezierTo::create(1.40625f, path), MoveTo::create(0.5f, Vec2(screen_size.width + 100, 290)), CallFunc::create([=] {
		_nat->playSequence("run_stop", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			_nat->playSequence("idle");
		});
		_word->runAction(EaseQuinticActionOut::create(MoveTo::create(1, Vec2(screen_size.width / 2, 550))));

		// hide gaf object
		for (auto child : stone_light->getChildren()) {
			if (child->getName() != "image") {
				child->setVisible(false);
			}
		}
		stone_light->runAction(ScaleTo::create(0.75, 6));
		stone_light->runAction(EaseQuinticActionOut::create(MoveTo::create(1, Vec2(this->getContentSize().width / 2 - 50, 300))));
		this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
			playSyncText();
		}), NULL));
	}), NULL));

}

void GameBuildBridge::onTapStone(cocos2d::Ref * sender) {
	activeMode();
	auto stone_light = reinterpret_cast<GAFWrapper *>(sender);
	stone_light->stopAllActions();
	// do nguoi dung cham vao 
	if (stone_light->isTouchEnabled()) {
		AUDIO_ENGINE->playSound(_current_round.audio);
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_binocular.mp3");
		_nat->playSequence("look_on", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("look_idle", false);
		});

		_nat->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([this] {
			AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_binocular.mp3");
			_nat->playSequence("look_off", false, [](gaf::GAFObject * g, std::string) {
				g->playSequence("idle", false);
			});
		}), NULL));
	}

	disableTouch();
	stone_light->setTouchEnabled(false);
	stone_light->runAction(ScaleTo::create(0.75, 6));
	stone_light->runAction(EaseQuinticActionOut::create(MoveTo::create(0.5f, Vec2(screen_size.width / 2 - 50, screen_size.height / 2))));

	auto back_pos = Vec2(screen_size.width / 2.0f + 480, 275);
	if ((screen_size.width / 2.0f) < 580) {
		back_pos.x = screen_size.width - 100;
	}
		
	stone_light->runAction(Sequence::create(DelayTime::create(2.5),
		Spawn::create(EaseQuinticActionOut::create(MoveTo::create(0.5f, back_pos)), ScaleTo::create(0.75, 2.0f), nullptr),
		CallFunc::create([stone_light, this] {
		stone_light->setTouchEnabled(true);
		enableTouch();
		inactiveMode();
	}), NULL));
}

void GameBuildBridge::onResetRound()
{
	stone_light->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=] {
		startRound();
	}), RemoveSelf::create(), NULL));
	_word->removeAllChildren();
	_word->setScale(1.0f);
	_word->setPosition(Point(this->getContentSize().width / 2, 625));
	_camera->runAction(FadeOut::create(0.5f));
	auto list_top = utils::findChild<ui::ListView *>(this, "list_top");
	list_top->removeAllItems();
	auto list_bottom = utils::findChild<ui::ListView *>(this, "list_bottom");
	list_bottom->removeAllItems();
}

void GameBuildBridge::onTransitionRound()
{
	auto nat_pos = Point(screen_size.width / 2 - 412, 290);

	AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_fade_in.mp3");
	_nat->setPosition(Point(-100, 290));
	_nat->playSequence("run", true);
	_nat->runAction(Sequence::create(MoveTo::create(1, nat_pos), CallFunc::create([this] {
		_nat->playSequence("run_stop", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("wrong", false);
		});

		initStoneLight();

		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_binocular.mp3");
		_nat->playSequence("look_off", false, [](gaf::GAFObject * g, std::string) {
			g->playSequence("idle", false);
		});

		onTapStone(stone_light);

		std::vector<std::string> chain;
		std::string audioIntro = "sounds/build_bridge/Make the word.mp3";

		if (_audioIntroPath != "")
		{
			audioIntro = _audioIntroPath;
		}
		chain.push_back(audioIntro);
		chain.push_back(_current_round.audio);
		AUDIO_ENGINE->playChain(chain);
		showText();

	}), NULL));
}

void GameBuildBridge::enableTouch()
{
	touch_layout->setVisible(false);
}

void GameBuildBridge::disableTouch()
{
	touch_layout->setVisible(true);
}

void GameBuildBridge::inactiveMode(float dt)
{
	this->schedule([=](float) {
		showHandSuggetionGame();
		std::string sound_guide = "sounds/build_bridge/Make the word.mp3";
		if (_audioIntroPath != "")
		{
			sound_guide = _audioIntroPath;
		}
		std::vector<std::string> chain;
		chain.push_back(sound_guide);
		chain.push_back(_current_round.audio);
		AUDIO_ENGINE->playChain(chain);
	}, 12.0f, 10000, dt, "inactive_mode");
}

void GameBuildBridge::activeMode()
{
    this->hideTapGuideGame();
	this->unschedule("inactive_mode");
	AUDIO_ENGINE->stopSound("sounds/build_bridge/Make the word.mp3");
	AUDIO_ENGINE->stopSound(_audioIntroPath);
	AUDIO_ENGINE->stopSound(_current_round.audio);
}

void GameBuildBridge::onMergeText()
{
	AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_nat_wingame.mp3");
	_word->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5f, screen_size / 2), ScaleTo::create(0.5f, 1.5f), NULL),
		DelayTime::create(0.5f), CallFunc::create([=] {
		auto word_size = (int)StringUtils::getCharacterCountInUTF8String(_current_round.word);
		for (int i = 0; i < word_size; ++i) {
			auto text = _word->getChildByTag(i);
			text->runAction(Sequence::create(MoveBy::create(0.25f, Point(((word_size) / 2.0f - i - 0.5) * 40, 0)), Hide::create(), NULL));
		}
	}), /*DelayTime::create(0.25f),*/ CallFunc::create([=] {
		AUDIO_ENGINE->playEffect("sounds/build_bridge/BtB_correct_stars.mp3");
		auto fx = GAFWrapper::createGAF("gaf/build_bridge/effect_stars/effect_stars.gaf");
		fx->setPosition(_word->getPosition() + Vec2(0, 50));
		fx->setScale(3.0f);
		this->addChild(fx, 10);
		fx->play(false, [=](gaf::GAFObject * obj) {
			fx->removeFromParent();
		});
	}), DelayTime::create(0.5f), CallFunc::create([=] {
		onTransitionGame();
	}), NULL));

	text_lb->runAction(cocos2d::Sequence::create(DelayTime::create(1.25f), Show::create(), NULL));

}

void GameBuildBridge::playSyncText()
{
	_char_filled.clear();
	_char_filled.resize(_current_round.word.size(), false);
	float dt = 0;
	
	if (_isAudioIntro)
	{
		for (auto phonic : _phonics_copy) {
			this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
				AUDIO_ENGINE->playSound(phonic.audio_phonic);
				}), NULL));
			for (auto u = 0; u < (int)phonic.phonic.size(); ++u) {
				for (auto i = 0; i < (int)_char_filled.size(); ++i) if (!_char_filled[i] && _current_round.word[i] == phonic.phonic[u]) {
					_char_filled[i] = true;
					auto char_img = text_lb->getLetter(i);
					if (char_img) {
						char_img->runAction(Sequence::create(ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
							char_img->setColor(Color3B::WHITE);
							}), NULL));
					}
					break;
				}
			}
		}
		this->scheduleOnce([=](float delay) {
			playSlowText();
			}, 0.5, "playSlowText");
	}
	else
	{
		for (auto phonic : _phonics_copy) {
			this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
				AUDIO_ENGINE->playSound(phonic.audio_phonic);
				}), NULL));
			for (auto u = 0; u < (int)phonic.phonic.size(); ++u) {
				for (auto i = 0; i < (int)_char_filled.size(); ++i) if (!_char_filled[i] && _current_round.word[i] == phonic.phonic[u]) {
					_char_filled[i] = true;
					auto char_img = text_lb->getLetter(i);
					if (char_img) {
						char_img->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
							char_img->setColor(Color3B::RED);
							}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
								char_img->setColor(Color3B::WHITE);
								}), NULL));
					}
					break;
				}
			}
			dt += 0.75f;
		}
		dt += 0.25f;
		this->scheduleOnce([=](float delay) {
			playSlowText();
			}, dt, "playSlowText");
	}
}

void GameBuildBridge::playSlowText()
{
	auto audio_id = AUDIO_ENGINE->play(_current_round.slow_audio);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = 1.0f;
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		for (auto i = 0; i < (int)_char_filled.size(); ++i) {
			auto char_img = text_lb->getLetter(i);
			if (char_img) {
				char_img->setColor(Color3B::RED);
				char_img->runAction(Sequence::create(scale1->clone(), Spawn::create(scale2->clone(), move1->clone(), NULL),
					Spawn::create(scale3->clone(), move2->clone(), NULL), CallFunc::create([=] {
					char_img->setColor(Color3B::WHITE);
				}), NULL));
			}
		}
		this->scheduleOnce([=](float delay) {
			playNormalText();
		}, (duration + duration0 + 0.5f), "playNormalText");
	}), NULL));
}

void GameBuildBridge::playNormalText()
{
	auto audio_id = AUDIO_ENGINE->play(_current_round.audio);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = 1.0f;
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		for (auto i = 0; i < (int)_char_filled.size(); ++i) {
			auto char_img = text_lb->getLetter(i);
			if (char_img) {
				char_img->setColor(Color3B::RED);
				char_img->runAction(Sequence::create(scale1->clone(), Spawn::create(scale2->clone(), move1->clone(), NULL),
					Spawn::create(scale3->clone(), move2->clone(), NULL), CallFunc::create([=] {
					char_img->setColor(Color3B::WHITE);
				}), DelayTime::create(1.0f), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), NULL));
			}
		}
		this->scheduleOnce([=](float delay) {
			onResetRound();
		}, (duration + duration0 + 1.0f), "onResetRound");
	}), NULL));
}

void GameBuildBridge::addSpeaker()
{
	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	auto screenSize = cocos2d::Director::getInstance()->getWinSize();
	this->addChild(speaker, 10000000);
	speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	speaker->setPosition(Vec2(50, Director::getInstance()->getWinSize().height - 50));
	speaker->addClickEventListener([=](Ref* sender) {
		auto current_phonic = _current_round.phonics.front();

		std::vector<std::string> chain;
		std::string audioIntro = "sounds/build_bridge/Make the word.mp3";
		std::string audioPhonic = current_phonic.audio_phonic;

		if (_audioIntroPath != "")
		{
			audioIntro = _audioIntroPath;
		}
		chain.push_back(audioIntro);

		if (audioPhonic == "")
		{
			audioPhonic = _current_round.audio;
		}

		chain.push_back(audioPhonic);
		AUDIO_ENGINE->playChain(chain);
	});
}

ui::Widget::ccWidgetClickCallback GameBuildBridge::onLocateClickCallback(const std::string &name) {
//    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//
//    };
//
//    if (function_map.find(name) != function_map.end()) {
//        return function_map.at(name);
//    }

	return nullptr;
}

void GameBuildBridge::showHandSuggetionGame()
{
	if (auto handGaf = utils::findChild(this, "name_sugget_game"))
	{
		handGaf->removeFromParent();
	}
	std::string pathHand = "gaf/guidegame/hand_tap/hand_tap.gaf";
	auto hand = GAFWrapper::createGAF(pathHand);
	hand->setName("name_sugget_game");
	auto currentPhonic = _current_round.phonics.front();;

	for (int i = 0; i < bubbles.size(); i++)
	{
		auto bubble = reinterpret_cast<GAFWrapper*>(bubbles[i]);
		if (bubble)
		{
			auto textBuble = utils::findChild<ui::Text *>(bubble, "text");
			std::string text = textBuble->getString();
			if (text == currentPhonic.phonic)
			{
				if (auto parentBubble = bubble->getParent())
				{
					hand->setPosition(bubble->getBoundingBox().size / 2);
					hand->setAnchorPoint(Point(0.5, 0.5));
					bubble->addChild(hand, INT_MAX - 1);
					hand->runAction(Sequence::create(
						CallFunc::create([=] {
							hand->play(true);
						}),
						DelayTime::create(1),
						RemoveSelf::create(),
					NULL));
				}
				break;
			}
		}
		
	}
}