//
//  GameNinja.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/3/18.
//
//edited by Cuong Cao Nguyen 4/25/2022

#include "GameNinja.h"
#include "GAFWrapper.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace cocos2d::experimental;

INITIALIZE_READER(GameNinja);

#define VELOCITY_JUNGLE 1500
#define VELOCITY_GROUND 1573
#define font_name "fonts/KGWHATTHETEACHERWANTS.TTF"
#define key_ninja_inactive_mode "key_ninja_inactive_mode"

#define music_game_name "sounds/ninja/Ninja_escape.mp3"

std::once_flag game_ninja_reader_flag;

GameNinja * GameNinja::createView(std::string config_path) {
	std::call_once(game_ninja_reader_flag, [] {
		REGISTER_CSB_READER(GameNinja);
	});

	auto view = reinterpret_cast<GameNinja *>(CSLoader::createNodeWithVisibleSize("csb/game/ninja/ninja.csb"));
	if (view) {
		view->didLoadFromCSB(config_path);
		return view;
	}

	CC_SAFE_DELETE(view);
	return view;
}

void GameNinja::didLoadFromCSB(std::string config_path) {
	this->setTouchEnabled(true);
	this->config_path = config_path;
	_jungle = utils::findChild<Layout *>(this, "jungle");
	_ground = utils::findChild<Layout *>(this, "ground");

	_current_turn = -1;

	_word = Layout::create();
	_word->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	_word->setPositionX(50);
	_word->setPositionY(this->getContentSize().height - 50);
	this->addChild(_word, 1);
}

void GameNinja::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	AudioEngine::play2d("sounds/ninja/forest.mp3", true, 0.2f);

	auto logo = ImageView::create("games/ninja/ninja.png");
	logo->setPosition(this->getContentSize() / 2);
	this->addChild(logo);

	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(music_game_name);
		}), NULL));

	logo->runAction(Sequence::create(DelayTime::create(3), FadeOut::create(0.25), CallFunc::create([this] {
		this->introGame();
	}), RemoveSelf::create(), NULL));
}

void GameNinja::onExit() {
    MJMainGBase::onExit();
}

void GameNinja::introGame() {
	auto timeline = CSLoader::createTimeline("csb/game/ninja/ninja.csb");
	this->runAction(timeline);
	timeline->play("appear", false);
	timeline->setTag(1010);

	_gaf = GAFWrapper::createGAF("gaf/ninja/ninja/ninja.gaf");
	_gaf->setFps(30);
	auto ninja = this->getChildByName("ninja");
	_gaf->setPosition(Vec2(ninja->getContentSize() / 2) + Vec2(50, 15) /* offset */);
	ninja->addChild(_gaf);
	_gaf->setScale(2);

	ninja->setPosition(Point(150, 135));
	ninja->runAction(JumpTo::create(1, Point(512, 425), 400, 1));

	//    CCLOG("time: %lld", utils::getTimeInMilliseconds());
	_gaf->playSequence("air", false, [=](gaf::GAFObject * g, std::string) {
		AudioEngine::play2d("sounds/ninja/front_flip.mp3");
		_gaf->playSequence("flip", false, [=](gaf::GAFObject * g, std::string) {
			_gaf->setFps(4);
			_gaf->playSequence("fall", false, [=](gaf::GAFObject * g, std::string) {
				_gaf->setFps(25);
				//                _gaf->playSequence("fall", false, [=](gaf::GAFObject * g, std::string) {

				//                    _gaf->playSequence("land", false, [=](gaf::GAFObject * g, std::string) {
				this->scheduleOnce([=](float dt) {
					//                           AudioEngine::play2d("sounds/ninja/jump.mp3");

					_gaf->playSequence("sit", false, [=](gaf::GAFObject * g, std::string) {
						_gaf->runAction(Sequence::create(DelayTime::create(0.0), CallFuncN::create([=](Node * n) {
							_gaf->playSequence("sit_open_eyes");
							AudioEngine::play2d("sounds/ninja/drum.mp3");
							auto new_anchor_point = Point(512 / this->getContentSize().width, 0.5);
							this->setAnchorPoint(new_anchor_point);
							this->setPosition(Point(new_anchor_point.x * this->getContentSize().width, new_anchor_point.y * this->getContentSize().height));
							this->runAction(Sequence::create(DelayTime::create(0.25), EaseElasticOut::create(ScaleTo::create(1, 2)), CallFunc::create([this, timeline, ninja] {

								this->runAction(ScaleTo::create(1, 1));
								timeline->play("appear_end", false);
								_gaf->runAction(ScaleTo::create(1, 1));
								timeline->setLastFrameCallFunc(nullptr);

								ninja->runAction(JumpTo::create(1, Point(150, 235), 300, 1));

								AudioEngine::play2d("sounds/ninja/jump.mp3");

								_gaf->playSequence("air", false, [=](gaf::GAFObject * g, std::string) {
									AudioEngine::play2d("sounds/ninja/front_flip.mp3");
									_gaf->playSequence("flip", false, [=](gaf::GAFObject * g, std::string) {
										_gaf->playSequence("fall", false, [=](gaf::GAFObject * g, std::string) {
											_audio_id_run = AudioEngine::play2d("sounds/ninja/run.mp3", true, 0.25);
											AUDIO_ENGINE->playSound("sounds/ninja/music.mp3", true, nullptr, 0.2f);

											_gaf->playSequence("run", true);
											_jungle->removeChildByName("gate");
											this->schedule(schedule_selector(GameNinja::updateBackground), 0, CC_REPEAT_FOREVER, 0.25);
											this->runAction(Sequence::create(DelayTime::create(0.25), CallFunc::create([this] {
												_jungle->runAction(RepeatForever::create(MoveBy::create(1, Vec2(-VELOCITY_JUNGLE, 0))));
												_jungle->removeChildByName("gate");
												_ground->runAction(RepeatForever::create(MoveBy::create(1, Vec2(-VELOCITY_GROUND, 0))));
											}), nullptr));

											auto id_intro = AudioEngine::play2d("sounds/ninja/intro.mp3");
											AudioEngine::setFinishCallback(id_intro, [this](int, std::string) {
												startGame();
											});
										});
									});
								});
							}), nullptr));
						}), NULL));
					});
				}, 0.0, "delay_animation_land");
			});
			//                });
			//            });
		});
	});
}

#include "Math_Libs.h"
void GameNinja::generateData() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(config_path);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	rapidjson::Value& value_data = document.FindMember("data")->value;
	for (int i = 0; i < value_data.Size(); i++) {
		WordInfo word_info;
		auto word = value_data[i].FindMember("text_full")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, word);
		word_info.word = pImage->getText("name_1");
		word_info.sound_normal = pImage->getAudios("name_1").front();
		word_info.sound_slow = pImage->getAudios("name_n").front();
		word_info.image = pImage->path;
		word_info.characters = pImage->getTextPhonic("name_p");
		word_info.voice = pImage->getAudioPhonic("name_p");

		rapidjson::Value& noices = value_data[i].FindMember("list_distracting")->value;
		_noicesSize.push_back(noices.Size());
		for (int j = 0; j < noices.Size(); j++) {
			auto noice = noices[j].FindMember("text")->value.GetString();
			auto noice_img = math::resource::ResHelper::getImage(images, noice);
			word_info.noise.push_back(noice_img->getText("name_1"));
		}
		_data.push_back(word_info);
	}
}

#include "MJHelper.h"
void GameNinja::startGame() {
	generateData();
	startTurn();
}

void GameNinja::newRound() {
	_current_round = _data.front();
	_data.erase(_data.begin());
	_current_turn = 0;
	_char_filled.clear();
	_char_filled.resize(_current_round.word.length(), false);

	for (auto c : _underline) c->removeFromParent();
	_underline.clear();
	_word->removeAllChildren();
	_space.clear();

	auto label = Text::create(_current_round.word, font_name, 100);
	for (auto i = 0; i < (int)_current_round.word.length(); ++i) {
		auto bb = label->getLetter(i)->getBoundingBox();
		_space.push_back(bb.getMinX());
	}

	for (auto i = 0; i < _current_round.word.length(); ++i) {
		auto c = Text::create(StringUtils::toString(_current_round.word[i]), font_name, 100);
		c->setTextColor(Color4B::BLACK);
		c->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
		c->setPosition(Point(40 + 80 * i, 0));
		c->setTag(i);
		_word->addChild(c);
	}
	_word->setPositionX(50);
	_word->setPositionY(this->getContentSize().height - 200);

	for (auto i = 0; i < (int)StringUtils::getCharacterCountInUTF8String(_current_round.word); ++i) {
		auto c = _word->getChildByTag(i);
		if (c) {
			c->setVisible(false);
			auto bb = c->getBoundingBox();

			//create underline for this character
			auto underline = Layout::create();
			underline->setBackGroundColor(Color3B::BLACK);
			underline->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
			underline->setContentSize(Size(70, 10));
			underline->setPosition(Point(80 * i + 55 /*+ 10 * i*/, this->getContentSize().height - 210));
			underline->addClickEventListener([=](Ref* sender) {
				AudioEngine::setVolume(_audio_id_run, 0.25);
				auto id_voice = AudioEngine::play2d(_current_round.voice[_current_turn]);
				AudioEngine::setFinishCallback(id_voice, [this](int, std::string) {
					AudioEngine::setVolume(_audio_id_run, 0.25);
				});
			});
			this->addChild(underline);
			_underline.push_back(underline);
		}
	}
}

void GameNinja::inactiveMode(float dt)
{
	this->schedule([=](float) {
		AudioEngine::play2d(_current_round.voice[_current_turn]);
	}, 11.5f, 100000, dt, key_ninja_inactive_mode);
}

void GameNinja::startTurn() {
	_shurikens.clear();
	if (_current_turn == -1) {
		//first turn
		newRound();
	}
	else {
		_underline[_current_turn]->setTouchEnabled(false);
		_current_turn++;
	}

	if (_current_turn == (int)_current_round.characters.size()) {
		//finish turn
		_countTurn++;
		auto w = 0.f;
		for (auto i = 0; i < (int)_current_round.word.length(); ++i) {
			auto c = _word->getChildByTag(i);
			c->runAction(MoveTo::create(0.15, Vec2(_space[i] + c->getContentSize().width / 2, 0)));

			if (i == (int)_current_round.word.length() - 1) {
				w = _space[i] + c->getContentSize().width;
			}
		}
		_word->runAction(EaseQuinticActionOut::create(MoveTo::create(1, Point(this->getContentSize().width / 2 - w / 2, _word->getPositionY()))));

		auto frame = ImageView::create("games/ninja/bamboo_frame.png");
		frame->setPosition(this->getContentSize() / 2);
		frame->setPositionY(-1000);
		frame->setName("frame");

		auto frame_bg = Layout::create();
		frame_bg->setContentSize(Size(715, 430));
		frame_bg->setBackGroundColor(Color3B::WHITE);
		frame_bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
		frame_bg->setBackGroundColorType(BackGroundColorType::SOLID);
		frame->addChild(frame_bg, -1);
		frame_bg->setPosition(frame->getContentSize() / 2);

		float frame_scale = 1.0f;
		auto image = ImageView::create(_current_round.image);
		image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		float scale = 380.0f / image->getContentSize().height;
		if (image->getContentSize().width * scale > 715)
			scale = 695.0f / image->getContentSize().width ;

		CCLOG("Image Size : %f %f", image->getContentSize().width, image->getContentSize().height);
		scale *= frame_scale;
		image->setScale(scale);
		image->setPosition(frame->getContentSize() / 2);
		frame->addChild(image, -1);
		frame->setScale(frame_scale);
		frame->runAction(Sequence::create(EaseQuinticActionOut::create(MoveTo::create(0.5, this->getContentSize() / 2 - Size(0, 100))), CallFunc::create([this] {
			auto id_normal = AudioEngine::play2d(_current_round.sound_normal);
			AudioEngine::setFinishCallback(id_normal, [this](int, std::string) {
				this->scheduleOnce([this](float) {
					_word->runAction(MoveBy::create(0.5, Vec2(0, 300)));

					auto f = this->getChildByName("frame");
					f->runAction(Sequence::create(EaseQuinticActionIn::create(MoveBy::create(0.5, Vec2(0, -this->getContentSize().height))), RemoveSelf::create(), NULL));

					if (_data.empty()) {
						_gaf->getParent()->runAction(Sequence::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 - _gaf->getParent()->getContentSize().width / 2, _gaf->getParent()->getPositionY())), CallFunc::create([this] {

							AudioEngine::stop(_audio_id_run);
                            AudioEngine::play2d("sounds/ninja/Great job!.mp3");
							// chay het nhac end roi moi ket game
							int audio_id_end = AudioEngine::play2d("sounds/ninja/end_music.mp3", false, 0.5f);
							AudioEngine::setFinishCallback(audio_id_end, [this](int, std::string) {
								//MJMainGBase::onCloseGame();
							});
							this->unscheduleAllCallbacks();
							_jungle->stopAllActions();
							_ground->stopAllActions();

							auto new_anchor_point = Point(0.5, 200 / this->getContentSize().height);
							this->setPosition(Point(new_anchor_point.x * this->getContentSize().width, new_anchor_point.y * this->getContentSize().height));
							this->setAnchorPoint(new_anchor_point);

							this->runAction(Sequence::create(ScaleTo::create(0.5, 2), CallFunc::create([this] {
								AudioEngine::stop(_audio_id_background);
								this->scheduleOnce([](float dt) {
									AudioEngine::play2d("sounds/ninja/throw_bomb.mp3");
								}, 0.55, "delay_audio_bomb");
								_gaf->playSequence("throw_bomb");

								auto bomb = ImageView::create("games/ninja/bomb.png");
								bomb->setPosition(Point(180, 150));
								bomb->setScale(0.5);
								bomb->setVisible(false);
								_gaf->getParent()->addChild(bomb);

								bomb->runAction(Sequence::create(DelayTime::create(0.55), Show::create(), JumpTo::create(0.5, Point(300, 0), 100, 1), Hide::create(), CallFunc::create([this] {
									auto bomb_fx = GAFWrapper::createGAF("gaf/ninja/bomb/bomb.gaf");
									_gaf->getParent()->addChild(bomb_fx);
									bomb_fx->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
									bomb_fx->setPosition(Point(-75, -150));
									bomb_fx->play(true);
									_gaf->runAction(Sequence::create(DelayTime::create(0.1), Hide::create(), NULL));
									bomb_fx->play(false, [this](gaf::GAFObject *) {
										_gaf->getParent()->removeFromParent();
										this->scheduleOnce([=](float) {
											onFinishGameVMonkey();
										}, 1.5f, "delay_endgame");
										
									});

								}), NULL));
							}), NULL));
						}), nullptr));
						return;
					}
					else {
						_current_turn = -1;
						_gaf->getParent()->runAction(Sequence::create(MoveBy::create(2, Vec2(-Director::getInstance()->getVisibleSize().width, 0)), CallFunc::create([this] {
							startTurn();
						}), nullptr));
					}

				}, 0.5, "daludiko");
			});
		}), nullptr));
		this->addChild(frame);
		_gaf->getParent()->runAction(MoveBy::create(2, Vec2(Director::getInstance()->getVisibleSize().width, 0)));
		return;
	}

	mj::helper::shuffle(_current_round.noise);

	//create shuriken
	const std::vector<int> shurikens_x = { 250, 465, 680 };
	int wrongAnswer = 0;
	int answer = -1;
	for (auto i = 0; i < 3; ++i) {
		auto shuriken = ImageView::create("games/ninja/shuriken.png");
		_shurikens.push_back(shuriken);
		shuriken->setName("shuriken");
		shuriken->setTouchEnabled(false);
		if (i == 0) {
			auto label = Text::create(_current_round.characters[_current_turn], font_name, 80);
			label->setTextColor(Color4B::BLACK);
			label->setPosition(shuriken->getContentSize() / 2 - Size(10, 0));
			shuriken->addChild(label);
			shuriken->addClickEventListener(CC_CALLBACK_1(GameNinja::onRightAnswer, this));
		}
		else {
			if (_countTurn < _noicesSize.size()) {
				wrongAnswer = rand() % _noicesSize[_countTurn];
				if (wrongAnswer == answer) {
					wrongAnswer = rand() % _noicesSize[_countTurn];
				}
			}
			auto label = Text::create(_current_round.noise[wrongAnswer], font_name, 80);
			label->setTextColor(Color4B::BLACK);
			label->setPosition(shuriken->getContentSize() / 2 - Size(10, 0));
			shuriken->addChild(label);
			answer = wrongAnswer;
			shuriken->addClickEventListener(CC_CALLBACK_1(GameNinja::onWrongAnswer, this));
		}
	}

	auto id_swish = AudioEngine::play2d("sounds/ninja/swish.mp3");
	AudioEngine::setFinishCallback(id_swish, [this](int, std::string) {
		AudioEngine::setVolume(_audio_id_run, 0.25);
		auto id_voice = AudioEngine::play2d(_current_round.voice[_current_turn]);
		AudioEngine::setFinishCallback(id_voice, [this](int, std::string) {
			AudioEngine::setVolume(_audio_id_run, 0.25);
			for (auto item : _shurikens) {
				item->setTouchEnabled(true);
			}
		});
	});
	//    _underline[_current_turn]->setTouchEnabled(true);

	auto it = std::find(_char_filled.begin(), _char_filled.end(), false);
	auto p = std::distance(_char_filled.begin(), it);
	for (auto i = 0; i < _current_round.characters[_current_turn].length(); ++i) {
		if (_current_round.characters[_current_turn][i] != '_') {
			_underline[p + i]->setTouchEnabled(true);
			_underline[p + i]->setBackGroundColor(Color3B::RED);
			_underline[p + i]->runAction(Blink::create(100, 120));
		}
	}

	mj::helper::shuffle(_shurikens);
	for (auto i = 0; i < 3; ++i) {
		auto vary = (i == 1) ? 250 : 200;
		_shurikens[i]->setPosition(Point(cocos2d::random(Director::getInstance()->getVisibleSize().width - vary, Director::getInstance()->getVisibleSize().width - vary), shurikens_x[i]));

		_shurikens[i]->setPositionX(_shurikens[i]->getPositionX() + 400);
		_shurikens[i]->runAction(
			Sequence::create(
				DelayTime::create(cocos2d::random(0.0, 0.5)),
				EaseQuarticActionOut::create(MoveBy::create(0.5, Vec2(-400, 0))),
				CallFuncN::create([](Node * n) {
			n->runAction(RepeatForever::create(Sequence::create(
				MoveBy::create(cocos2d::random(0.4, 0.6), Vec2(10, 0)),
				MoveBy::create(cocos2d::random(0.4, 0.6), Vec2(-10, 0)), nullptr)));
		}), NULL));
		this->addChild(_shurikens[i]);
	}

	inactiveMode(11.5f);
}

void GameNinja::randAnwser(int dt) {
	int wrongAnswer = rand() % dt;
}

void GameNinja::onRightAnswer(Ref * sender) {
	// inactivemode 
	unschedule(key_ninja_inactive_mode);

	//right answer
	for (auto c : this->getChildren()) if (c->getName() == "shuriken" && c != sender) {
		reinterpret_cast<Widget *>(c)->setTouchEnabled(false);
		c->runAction(Sequence::create(MoveBy::create(0.5, Vec2(400, 0)), RemoveSelf::create(), NULL));
	}

	auto shu = reinterpret_cast<ImageView *>(sender);
	shu->setTouchEnabled(false);
	auto action_scale = ScaleTo::create(0.5, 0.15); action_scale->setFlags(0xffee);
	auto action_rotate = RotateBy::create(0.5, -360); action_rotate->setFlags(0xffee);
	shu->runAction(action_scale);
	shu->runAction(action_rotate);
	AudioEngine::play2d("sounds/ninja/correct.mp3");
	AudioEngine::play2d("sounds/ninja/throw_suriken.mp3");

	shu->runAction(Sequence::create(JumpTo::create(0.5, Vec2(300, 225), 10, 1), CallFunc::create([this, shu] {
		auto eff = GAFWrapper::createGAF("gaf/ninja/effect/effect.gaf");
		eff->setPosition(Point(300, 225));
		this->addChild(eff);
		auto character = _word->clone();;
		//        character->setTextColor(Color4B::BLACK);
		character->setPosition(Point(300, 225));
		this->addChild(character);
		character->setScale(0.5);
		character->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		character->runAction(ScaleTo::create(0.15, 1));
		character->setTag(0xffff);

		for (auto i = 0; i < StringUtils::getCharacterCountInUTF8String(_current_round.word); ++i) {
			auto c = character->getChildByTag(i);
			c->setVisible(false);
            
		}

		//find destination
		auto it = std::find(_char_filled.begin(), _char_filled.end(), false);
		auto p = std::distance(_char_filled.begin(), it);
		auto bb = _word->getBoundingBox();
		//        bb.origin = _word->convertToWorldSpace(bb.origin);
		auto destination = bb.origin;


		for (auto i = 0; i < _current_round.characters[_current_turn].length(); ++i) {
			if (_current_round.characters[_current_turn][i] != '_') {
				_char_filled[p + i] = true;
				character->getChildByTag(p + i)->setVisible(true);
			}
		}

		auto ease = EaseElasticOut::create(MoveTo::create(1.5, destination), 0.66);
		character->runAction(Sequence::create(ease, CallFunc::create([this] {
			for (auto i = 0; i < (int)_char_filled.size(); ++i) {
				if (_char_filled[i]) {
					_word->getChildByTag(i)->setVisible(true);
					_underline[i]->stopAllActions();
					_underline[i]->runAction(FadeOut::create(0.25));
				}
			}
			this->startTurn();
		}), RemoveSelf::create(), NULL));
		eff->play(false, [this](gaf::GAFObject *) {
		});
	}), nullptr));

	_gaf->runAction(Sequence::create(DelayTime::create(0.4), CallFunc::create([this, shu] {
		const std::vector<std::string> ACTION = { "swing_up", "swing_down", "side_swing" };
		AudioEngine::play2d("sounds/ninja/sword_swing.mp3");

		shu->removeAllChildren();
		shu->stopActionsByFlags(0xffee);

		auto action_id = cocos2d::random(0, 2);

		_gaf->playSequence(ACTION[action_id], false, [this](gaf::GAFObject *, std::string) {
			_gaf->playSequence("run", true);
		});

		switch (action_id) {
		case 0:
			shu->runAction(Sequence::create(EaseQuinticActionOut::create(MoveBy::create(2, Vec2(300, 1000))), RemoveSelf::create(), nullptr));
			shu->runAction(ScaleTo::create(1, 0.5));
			shu->runAction(RotateBy::create(2, 720));
			break;

		case 1:
			shu->runAction(Sequence::create(MoveBy::create(0.5, Vec2(700, -100)), RemoveSelf::create(), nullptr));
			shu->runAction(ScaleTo::create(0.5, 1));
			shu->runAction(RotateBy::create(2, 360));
			shu->runAction(FadeOut::create(0.5));
			break;

		case 2:
			shu->runAction(Sequence::create(RotateBy::create(2, 360), RemoveSelf::create(), nullptr));
			shu->runAction(JumpBy::create(1, Vec2(400, -500), 500, 1));
			break;

		default:
			break;
		}

	}), nullptr));
}

void GameNinja::onWrongAnswer(Ref * sender) {
	//wrong answer
	unschedule(key_ninja_inactive_mode);
	inactiveMode(11.0f);
	AudioEngine::play2d("sounds/ninja/newwrong.mp3");
	auto shu = reinterpret_cast<ImageView *>(sender);
	shu->stopActionsByFlags(0x1001);
	auto action_tint = Sequence::create(TintTo::create(0.25, 255, 0, 0), DelayTime::create(1), TintTo::create(0.25, 0xff, 0xff, 0xff), NULL);
	action_tint->setFlags(0x1001);
	auto action_shake = Repeat::create(Sequence::create(MoveBy::create(0.15, Vec2(0, -20)), MoveBy::create(0.15, Vec2(0, 20)), NULL), 5);
	shu->runAction(action_tint);
	shu->runAction(action_shake);
}

void GameNinja::updateBackground(float dt) {
	auto jungle_children = _jungle->getChildren();
	for (auto c : jungle_children) {
		auto bb = c->getBoundingBox();
		bb.origin = _jungle->convertToWorldSpace(bb.origin);
		if (bb.getMaxX() < 0) {
			c->setPositionX(c->getPositionX() + c->getContentSize().width * 2);
		}
	}

	auto ground_children = _ground->getChildren();
	for (auto c : ground_children) {
		auto bb = c->getBoundingBox();
		bb.origin = _ground->convertToWorldSpace(bb.origin);
		if (bb.getMaxX() < 0) {
			c->setPositionX(c->getPositionX() + c->getContentSize().width * 5);
		}
	}
}

Widget::ccWidgetClickCallback GameNinja::onLocateClickCallback(const std::string &name) {
	return nullptr;
}
