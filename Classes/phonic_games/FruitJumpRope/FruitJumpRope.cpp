#include "FruitJumpRope.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"
INITIALIZE_READER(FruitJumpRope);

std::once_flag FruitJumpRope_reader;
#define CSB_NAME "csb/game/FruitJumpRope/FruitJumpRope.csb"

#define GAF_AVOCADO "gaf/FruitJumpRope/jumprope_avocado/jumprope_avocado.gaf"
#define GAF_DRAGON "gaf/FruitJumpRope/jumprope_dragon/jumprope_dragon.gaf"
#define GAF_DURIAN "gaf/FruitJumpRope/jumprope_durian/jumprope_durian.gaf"
#define GAF_LEMON "gaf/FruitJumpRope/jumprope_lemon/jumprope_lemon.gaf"
#define GAF_MANGOSTEEN "gaf/FruitJumpRope/jumprope_mangosteen/jumprope_mangosteen.gaf"
#define GAF_PAPAYA "gaf/FruitJumpRope/jumprope_papaya/jumprope_papaya.gaf"
#define GAF_PASSION "gaf/FruitJumpRope/jumprope_passion/jumprope_passion.gaf"
#define GAF_WATERMELON "gaf/FruitJumpRope/jumprope_watermelon/jumprope_watermelon.gaf"
#define GAME_NAME "hdr/games/FruitJumpRope/background.png"
#define GAF_EFFECT "gaf/FruitJumpRope/jumprope_effects/jumprope_effects.gaf"
#define BUTTON_WORD "hdr/games/FruitJumpRope/TextButton.png"

#define SOUND_BUBBLESHOW "sounds/FruitJumpRope/jr_bubbleshow.mp3"
#define SOUND_CORRECT "sounds/FruitJumpRope/jr_correct.mp3"
#define SOUND_JUMP "sounds/FruitJumpRope/jr_jump.mp3"
#define SOUND_SWING "sounds/FruitJumpRope/jr_swing.mp3"
#define SOUND_WRONG "sounds/FruitJumpRope/jr_wrong.mp3"
#define SOUND_INTRUC1 "sounds/FruitJumpRope/jr_intruction1.mp3"
#define SOUND_INTRUC1_VI_AM "sounds/FruitJumpRope/jr_intruction1_vi_am.mp3"
#define SOUND_INTRUC1_VI_VAN "sounds/FruitJumpRope/jr_intruction1_vi_van.mp3"
#define SOUND_INTRUC2 "sounds/FruitJumpRope/jr_intruction2.mp3"
#define SOUND_INTRUC2_VI_AM "sounds/FruitJumpRope/jr_intruction2_vi_am.mp3"
#define SOUND_INTRUC2_VI_VAN "sounds/FruitJumpRope/jr_intruction2_vi_van.mp3"
#define SOUND_BGM "sounds/FruitJumpRope/jr_bgm.mp3"
#define SOUND_GAME_NAME "sounds/FruitJumpRope/Fruit Jump Rope.mp3"
FruitJumpRope* FruitJumpRope::createGame(std::string json_file)
{
	std::call_once(FruitJumpRope_reader, [] {
		REGISTER_CSB_READER(FruitJumpRope);
		});

	auto p = static_cast<FruitJumpRope*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void FruitJumpRope::informationGame()
{
	this->runAction(Sequence::create(FadeOut::create(0.0), FadeIn::create(0.5f),
		DelayTime::create(0.5f),
		CallFunc::create([=] {
			screen_size = Director::getInstance()->getVisibleSize();
			if (auto infoGame = Sprite::create(GAME_NAME)) {
				infoGame->setPosition(screen_size / 2);
				infoGame->setName("FruitJumbRope");
				infoGame->setAnchorPoint(Vec2(0.5, 0.5));
				root->getParent()->addChild(infoGame);
			}
			
			}),
		DelayTime::create(0.5f),
				CallFunc::create([=] {
				AUDIO_ENGINE->playSound(SOUND_GAME_NAME, false);})
					,DelayTime::create(2.5f),CallFunc::create([=] {
					if (auto child = utils::findChild(this, "FruitJumbRope"))
					{
						child->removeFromParent();
					}
					triggerReady();
					}),
				NULL));
}

void FruitJumpRope::didLoadFromCSB(std::string json_file)
{
	this->question = nullptr;

	string str = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	rapidjson::Value& json_data = document.FindMember("data")->value;
	for (rapidjson::SizeType i = 0; i < json_data.Size(); i++)
	{
		FruitQuestion* question = new FruitQuestion();
		question->phase1 = json_data[i].FindMember("number")->value.GetInt();
		auto letter = json_data[i].FindMember("text")->value.GetString();
		question->hoc_am_hoc_van = "";
		if (json_data[i].HasMember("option_vm_phonic") && json_data[i].FindMember("option_vm_phonic")->value.GetString()) {
			question->hoc_am_hoc_van = json_data[i].FindMember("option_vm_phonic")->value.GetString();
		}
		auto pImage = math::resource::ResHelper::getImage(images, letter);
		question->word = pImage->getText("name_1");
		question->audio = pImage->getAudios("name_1").front();
		rapidjson::Value& json_question = json_data[i].FindMember("answer")->value;
		for (rapidjson::SizeType j = 0; j < json_question.Size(); j++)
		{
			auto img = math::resource::ResHelper::getImage(images, json_question[j].FindMember("item")->value.FindMember("text")->value.GetString());
			question->questions.push_back({ img->getText("name_1"),img->getAudios("name_1").front() });
		}

		this->questions.push_back(question);
	}
}

void FruitJumpRope::onEnter()
{
	MJMainGBase::onEnter();
	playBackgroundMusic(SOUND_BGM, 1.0f);
	this->setContentSize(screen_size);

	root = cocos2d::ui::Layout::create();
	root->setContentSize(screen_size);
	root->setTouchEnabled(true);
	addChild(root);
	//init fruit
	fruitList.clear();
	fruitList.push_back(GAF_AVOCADO);
	fruitList.push_back(GAF_DRAGON);
	fruitList.push_back(GAF_DURIAN);
	fruitList.push_back(GAF_LEMON);
	fruitList.push_back(GAF_MANGOSTEEN);
	fruitList.push_back(GAF_PAPAYA);
	fruitList.push_back(GAF_PASSION);
	//	fruitList.push_back(GAF_WATERMELON);

		//create final fruits
	for (int i = 0; i < fruitList.size(); i++)
	{
		Fruit* fruit1 = Fruit::Init(fruitList[i]);
		fruits.push_back(fruit1);
	}

	preFruit = -1;

	touchable = true;
	left = false;

	//game name
	/*auto gameName = cocos2d::ui::Text::create("Fruit Jump Rope", PHONIC_MANAGER->getGameFontName(), 100);
	root->addChild(gameName, 100);
	gameName->setColor(Color3B::BLACK);
	gameName->setPosition(screen_size / 2);
	gameName->setAnchorPoint(Vec2(0.5, 0.5));
	gameName->runAction(Sequence::create(FadeOut::create(0.0), FadeIn::create(0.5f),DelayTime::create(1.0f), FadeOut::create(0.5), CallFunc::create([=] {*/
	level = -1;
	informationGame();
	//gameName->removeFromParent();
//}), NULL));
//gameName->setLocalZOrder(10000);

}

void FruitJumpRope::onExit()
{
	for (Fruit* fruit : fruits)
	{
		CC_SAFE_DELETE(fruit);
	}
	for (FruitQuestion* question : questions)
	{
		CC_SAFE_DELETE(question);
	}
	CC_SAFE_DELETE(fruit);
	MJMainGBase::onExit();
	// AUDIO_ENGINE->stopAllEffects();
}

void FruitJumpRope::triggerReady()
{	 
	screen_size = Director::getInstance()->getVisibleSize();

	//get data
	level++;
	question = questions[level];

	//create word
	this->word = Word::Init(question->word);
	root->addChild(word);
	word->setPosition(Vec2(screen_size.width * 0.5, screen_size.height * 0.8));
	word->setDelegate(this);
	word->setVisible(false);
	word->setAudio(question->audio);
	word->setName("Word");
	if (word->getTextField()->getContentSize().width > screen_size.width * 0.3)
	{
		word->getTextField()->setScale((screen_size.width * 0.3) / (word->getTextField()->getContentSize().width));
	}
	

	_buttonWord = Button::create(BUTTON_WORD);
	_buttonWord->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_buttonWord->setPosition(Vec2(0, -10));
	word->addChild(_buttonWord);
	_buttonWord->setScale9Enabled(true);
	_buttonWord->ignoreContentAdaptWithSize(false);
	_buttonWord->setContentSize(Size(screen_size.width * 0.3 + 100,screen_size.height/5));
	_buttonWord->setZOrder(word->getZOrder() - 1);
	_buttonWord->addClickEventListener([=](Ref*) {
		word->addClick();
		});
	//create fruit
	int numOfFruit = fruitList.size();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, numOfFruit - 1);
	int ran = dis(gen);

	while (ran == preFruit)
	{
		ran = dis(gen);
	}

	preFruit = ran;
	this->fruit = Fruit::Init(fruitList[ran]);
	root->addChild(fruit);
	fruit->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
	fruit->setOpacity(0);
	fruit->setLocalZOrder(1000);

	//set up Game State


	gameState = FruitGameState::Ready;
	count = 0;
	timeCount = 0;
	tapped = 0;
	point = 0;
	rightAnswer = -1;
	touchable = true;
	fruit->idle(true);
	justJump = false;



	// play intruction
	std::vector<std::string> chain;
	if (CONFIG_MANAGER->isGameVMonkey()) {
		if (question->hoc_am_hoc_van == "hoc_am") {
			chain.push_back(SOUND_INTRUC1_VI_AM);
		}
		else {
			chain.push_back(SOUND_INTRUC1_VI_VAN);
		}
	}
	else {
		chain.push_back(SOUND_INTRUC1);
	}
	chain.push_back(this->question->audio);
	AUDIO_ENGINE->playChain(chain);
	//start a new game
	this->runAction(Sequence::create(CallFunc::create([=] {
		fruit->runAction(FadeIn::create(1.0));
		}), DelayTime::create(2.5f), CallFunc::create([=] {
			triggerStart();
			}), NULL));
}

void FruitJumpRope::triggerStart()
{
	gameState = FruitGameState::Phase1;
	phun(0);


	this->scheduleUpdate();
}

void FruitJumpRope::update(float dt)
{
	this->timeCount++;
	if (timeCount == 10000)
		timeCount = 0;

	if (!justJump && count != 0)
	{
		//this->runAction(Sequence::create(CallFunc::create([=] {
		if (tapped == 0)
			count--;
		else tapped--;

		if (count == 0 && gameState == FruitGameState::Phase2)
		{
			//				log("Hello");
			this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
				//					log("Phase 2");
				phase2();
				}), NULL));
		}

		auto call_back = [=] {
			if (count == 0)
			{
				this->fruit->idle(true);
			}
			justJump = false;
		};
		if (tapped >= 3)
			this->fruit->fast(false, call_back);
		else
			this->fruit->start(false, call_back);
		//this->fruit->start(false);
		AUDIO_ENGINE->playSound(SOUND_JUMP, false);
		AUDIO_ENGINE->playSound(SOUND_SWING, false);
		justJump = true;
		//}), DelayTime::create(25.0f/32), CallFunc::create([=] {
		//	if (count == 0)
		//	{
		//		this->fruit->idle(true);
		//	}
		//	justJump = false;
		//}), NULL));
	}
}

void FruitJumpRope::phun(float rad)
{
	screen_size = Director::getInstance()->getVisibleSize();

	float pi = 3.14159265359;
	rad = (rad * pi) / 180;

	float s = sin(rad);
	float c = cos(rad);

	float d = 300 * s;
	float k = 300 * c;

	float x = this->word->getPositionX();
	float y = this->word->getPositionY();

	float u = x + k;
	float v = y + d;
	
	if (d >=  screen_size.height * 0.25) {
		d = screen_size.height * 0.25;
	}

	//	this->word->runAction(MoveBy::create(0.25,Vec2(k,d)));
	this->word->runAction(Sequence::create(EaseOut::create(MoveBy::create(0.25, Vec2(k, d)), 0.5)
		,DelayTime::create(0.25f)
		,CallFunc::create([=] {
		word->touchable = true;
		_buttonWord->setVisible(true);
		this->word->setVisible(true);
		}), nullptr));

	showTutorial();

}

void FruitJumpRope::setUpTouchHandling()
{

}

void FruitJumpRope::phase2()
{
	//create new word bubble
	p2Word[0] = Word::Init(question->word);
	p2Word[0]->setAudio(question->audio);
	p2Word[0]->fixFit();
	p2Word[1] = Word::Init(question->questions[0].first);
	p2Word[1]->setAudio(question->questions[0].second);
	p2Word[1]->fixFit();
	p2Word[2] = Word::Init(question->questions[1].first);
	p2Word[2]->setAudio(question->questions[1].second);
	p2Word[2]->fixFit();

	for (int i = 0; i < 3; i++)
	{
		root->addChild(p2Word[i]);
		p2Word[i]->setDelegate(this);
		p2Word[i]->setVisible(true);
		p2Word[i]->showTB(1);
		Vec2 end_pos;
		switch (i)
		{
		case 0:
			//p2Word[i]->setPosition(Vec2(screen_size.width / 7 * 1, screen_size.height / 5 * 3));
			end_pos = Vec2(screen_size.width / 2 - 305, screen_size.height / 5 * 2.5);
			break;
		case 1:
			//p2Word[i]->setPosition(Vec2(screen_size.width / 2, screen_size.height / 5 * 4));
			end_pos = Vec2(screen_size.width / 2, screen_size.height / 5 * 4);
			break;
		case 2:
			//p2Word[i]->setPosition(Vec2(screen_size.width / 7 * 6, screen_size.height / 5 * 3));
			end_pos = Vec2(screen_size.width / 2 + 305, screen_size.height / 5 * 2.5);
			break;
		}
		auto start_pos = fruit->getPosition();
		p2Word[i]->setPosition(start_pos);
		p2Word[i]->setScale(0);
		p2Word[i]->runAction(Spawn::create(ScaleTo::create(0.3f, 1.0f), MoveTo::create(0.3f, end_pos), NULL));
	}

	//random question
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 2);
	rightAnswer = dis(gen);
	if (rightAnswer != 0)
	{
		p2Word[0]->setText(p2Word[rightAnswer]->text);
		p2Word[0]->setAudio(p2Word[rightAnswer]->wordAudio);
		p2Word[rightAnswer]->setText(question->word);
		p2Word[rightAnswer]->setAudio(question->audio);
	}

	//change text Color
	/*p2Word[0]->changeColor(Color3B::BLACK);
	p2Word[1]->changeColor(Color3B::BLACK);
	p2Word[2]->changeColor(Color3B::BLACK);*/
	auto path_audio = SOUND_INTRUC2;
	if (CONFIG_MANAGER->isGameVMonkey()) {
		if (question->hoc_am_hoc_van == "hoc_am") {
			path_audio = SOUND_INTRUC2_VI_AM;
		}
		else {
			path_audio = SOUND_INTRUC2_VI_VAN;
		}
	}
	AUDIO_ENGINE->playSound(path_audio, false, [=] {
		AUDIO_ENGINE->playSound(question->audio);
		});
	//animation
	AUDIO_ENGINE->playSound(SOUND_BUBBLESHOW, false);
	fruit->mchoice(false);
	showTutorial();

}

void FruitJumpRope::WordClick(Ref* ref)
{
	
	unschedule("guide");
	
	//this->hideTapGuideGame();
	if (!touchable) return;
	
	screen_size = Director::getInstance()->getVisibleSize();

	Word* word = static_cast<Word*>(ref);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 70);
	uniform_int_distribution<> dis1(111, 180);
	int ran = 0;

	if (gameState == FruitGameState::Phase1)
	{
		count = 1;

		point++;

		if (point == question->phase1)
		{
			gameState = FruitGameState::Phase2;
			word->touchable = false;
		}
		
		if (left)
		{
			ran = dis1(gen);
			left = false;
		}
		else
		{
			ran = dis(gen);
			left = true;
		}

		tapped = min(tapped + 1, 2);
		word->runAction(Sequence::create(CallFunc::create([=] {
			_buttonWord->setVisible(false);
			word->touchable = false;
			})
			,ScaleTo::create(0.2f, 1.3f)
			,DelayTime::create(0.3f)
			,CallFunc::create([=] {
				word->changeColor(Color3B::RED);
				})
			,DelayTime::create(0.5f)
			,ScaleTo::create(0.2,0)
			,CallFunc::create([=] {
				word->setVisible(false);
				word->changeColor(Color3B::BLACK);
				word->setPosition(Vec2(screen_size.width * 0.5, screen_size.height * 0.65));
				})
			, ScaleTo::create(0, 1)
			,CallFunc::create([=] {
				if (gameState == FruitGameState::Phase1) this->phun(ran);
				})
			, NULL));
		
	}

	if (gameState == FruitGameState::Phase2 && rightAnswer != -1)
	{
		if (word->text == question->word)
		{
			word->touchable = false;
			this->runAction(Sequence::create(CallFunc::create([=] {
				AUDIO_ENGINE->playSound(question->audio);
				}),
				CallFunc::create([=] {
					AUDIO_ENGINE->stopAllSound();
				AUDIO_ENGINE->playSound(word->wordAudio, false);
				AUDIO_ENGINE->playSound(SOUND_CORRECT, false);
				fruit->correct(true);
				
				playEffectStars();
				}),NULL));		
			for (int i = 0; i < 3; i++)
			{
				if (i == rightAnswer)
				{
					p2Word[i]->correct();
				}
				else p2Word[i]->wrong();
			}
			//word->touchable = false;
			if (level == questions.size() - 1)
				this->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([=] {
				triggerWin();
					}), NULL));
			else {
				this->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([=] {
					clearSence();
					}), DelayTime::create(2.0), CallFunc::create([=] {
						this->triggerReady();
						}), NULL));
			}

		}
		else
		{
			showTutorial();
			AUDIO_ENGINE->playSound(word->wordAudio, false);
			AUDIO_ENGINE->playSound(SOUND_WRONG, false, [=]() {
				},1.0f);
			fruit->wrong(false);
			this->runAction(Sequence::create(CallFunc::create([=] {
				word->showTB(2);
				}), DelayTime::create(0.25), CallFunc::create([=] {
					word->showTB(1);
					}), NULL));
		}
	}
}

void FruitJumpRope::clearSence()
{
	fruit->runAction(Sequence::create(FadeOut::create(1.5), CallFunc::create([=] {
		fruit->removeFromParent();
		}), NULL));
	for (int i = 0; i < 3; i++)
		p2Word[i]->removeFromParent();
	word->removeFromParent();
}

void FruitJumpRope::playEffectStars()
{
	int number_star = 8;
	auto location = randomPositionsInRect(cocos2d::Size(100, 100), Rect(200, 200, screen_size.width - 400, screen_size.height - 300), number_star);
	float dt = 0.0f;

	for (auto loc : location) {
		auto gaf_star = GAFWrapper::createGAF(GAF_EFFECT);
		gaf_star->setPosition(loc + Vec2(150, -200));
		root->addChild(gaf_star, 999);
		gaf_star->setVisible(false);
		gaf_star->runAction(Sequence::create(DelayTime::create(dt), Show::create(), CallFunc::create([gaf_star] {
			gaf_star->play(false, [=](gaf::GAFObject* obj) {
				gaf_star->removeFromParent();
				});
			}), NULL));
		dt += 0.2f;
	}
}

std::vector<cocos2d::Vec2> FruitJumpRope::randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item)
{
	std::vector<cocos2d::Vec2> list;

	float delta = 1.4;
	int count_rand = 0;

	for (int i = 0; i < number_item; i++) {
		if (count_rand >= 10) {
			count_rand = 0;
			delta -= 0.1;
		}
		cocos2d::Vec2 point;
		bool avail = true;
		int count = 0;
		do {
			count++;
			if (count > 50) {
				break;
			}

			avail = true;
			int rand_x = cocos2d::random(0, (int)(box_rect.size.width - item_size.width)) + (int)item_size.width * 0.6;
			int rand_y = cocos2d::random(0, (int)(box_rect.size.height - item_size.height)) + (int)item_size.height * 0.6;
			point = cocos2d::Vec2(rand_x + box_rect.origin.x, rand_y + box_rect.origin.y);

			for (cocos2d::Vec2 _point : list) {
				if (point.distance(_point) < item_size.width * delta) {
					avail = false;
					break;
				}
			}
		} while (!avail);

		if (count > 50) {
			i = -1;
			list.clear();
			count_rand++;
			continue;
		}
		else {
			list.push_back(point);
		}
	}
	return list;
}

void FruitJumpRope::triggerWin()
{
	AUDIO_ENGINE->stopAllSound();
	AUDIO_ENGINE->playSound(SOUND_JUMP, true);
	AUDIO_ENGINE->playSound(SOUND_SWING, true);

	touchable = false;
	for (int i = 0; i < 3; i++)
		p2Word[i]->removeFromParent();
	word->removeFromParent();

	fruit->fast(true);

	int cnt = -1;
	for (int i = 0; i < fruitList.size(); i++)
	{
		if (i != preFruit)
		{
			cnt++;
			placeFruit(cnt, fruits[i]);
		}
	}

	this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=] {
		AUDIO_ENGINE->stopAllAudio();
		AUDIO_ENGINE->stopAllSound();
		MJMainGBase::onFinishGame();
		}), NULL));
}

void FruitJumpRope::placeFruit(int position, Fruit* fruit)
{
	screen_size = Director::getInstance()->getVisibleSize();

	//auto root = cocos2d::utils::findChild(this, "bg");

	root->addChild(fruit);

	fruit->fast(true);

	switch (position)
	{
	case 0:
		//		fruit->setPosition(Vec2(screen_size.width / 2 - 300, screen_size.height / 2 + 300));
		fruit->setPosition(Vec2(-200, screen_size.height / 2 + 300));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 - 300, screen_size.height / 2 + 300)));
		break;
	case 1:
		//		fruit->setPosition(Vec2(screen_size.width / 2 + 300, screen_size.height / 2 + 300));
		fruit->setPosition(Vec2(screen_size.width + 200, screen_size.height / 2 + 300));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 + 300, screen_size.height / 2 + 300)));
		break;
	case 2:
		//		fruit->setPosition(Vec2(screen_size.width / 2 - 300, screen_size.height / 2 - 100));
		fruit->setPosition(Vec2(-200, screen_size.height / 2 - 100));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 - 300, screen_size.height / 2 - 100)));
		break;
	case 3:
		//		fruit->setPosition(Vec2(screen_size.width / 2 + 300, screen_size.height / 2 - 100));
		fruit->setPosition(Vec2(screen_size.width + 200, screen_size.height / 2 - 100));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 + 300, screen_size.height / 2 - 100)));
		break;
	case 4:
		//		fruit->setPosition(Vec2(screen_size.width / 2 - 600, screen_size.height / 2 + 100));
		fruit->setPosition(Vec2(-200, screen_size.height / 2 + 100));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 - 520, screen_size.height / 2 + 100)));
		break;
	case 5:
		//		fruit->setPosition(Vec2(screen_size.width / 2 + 600, screen_size.height / 2 + 100));
		fruit->setPosition(Vec2(screen_size.width + 200, screen_size.height / 2 + 100));
		fruit->runAction(MoveTo::create(0.5, Vec2(screen_size.width / 2 + 520, screen_size.height / 2 + 100)));
		break;
	}
}

void FruitJumpRope::showTutorial() {
	this->schedule([=](float) {
		inActiveMode();
		}, 10.0f, CC_REPEAT_FOREVER, 0.0f, "guide");
}

void FruitJumpRope::inActiveMode() {
	this->runAction(Sequence::create(CallFunc::create([=] {
					AUDIO_ENGINE->playSound(SOUND_INTRUC1, false, [=]() {
						AUDIO_ENGINE->playEffect(question->audio);
						},1.0f);
					}), NULL));
}
