#include "WaterPipeGame.h"
#include "MsHelperGame.h"
#include "APProfileManager.h"

#define COLOR_HILIGHT cocos2d::Color3B(134, 255, 54)

USING_NS_CC;

using namespace cocostudio::timeline;

WaterPipe::WaterPipe()
{
	//    // 'scene' is an autorelease object
	//    auto scene = Scene::create();
	//
	//    // 'layer' is an autorelease object
	//    auto layer = WaterPipe::create();
	//
	//    // add layer as a child to scene
	//    scene->addChild(layer);
	//
	//    // return the scene
	//    return scene;
}

WaterPipe::~WaterPipe()
{
	textVec.clear();
}

// on "init" you need to initialize your instance
bool WaterPipe::init()
{
	//////////////////////////////
	// 1. super init first
	if (!MJMainGBase::init())
	{
		return false;
	}
	screen_size = Director::getInstance()->getWinSize();
	_numberAge = ap::ProfileManager::getInstance().getInformationOfProfile().age;
	this->setContentSize(screen_size);
	CSLoader* instance = CSLoader::getInstance();
	instance->registReaderObject("PipeReader", (ObjectFactory::Instance) PipeReader::getInstance);
	instance->registReaderObject("ChoiceReader", (ObjectFactory::Instance) ChoiceReader::getInstance);

	this->rootNode = CSLoader::createNode("csb/game/WaterPipe/MainScene.csb");
	this->pipeLayout = this->rootNode->getChildByName<ui::Layout*>("pipe");
	this->pipeLayout->setVisible(false);

	this->questionPipe = this->pipeLayout->getChildByName<Pipe*>("pipePanel");
	this->questionPipeOriginalPosition = this->questionPipe->getPosition();

	this->choiceNode = this->pipeLayout->getChildByName("choiceNode");
	this->choiceNodeOriginalPosition = this->choiceNode->getPosition();

	//this->firstLength = this->questionPipe->getChildByName<ui::ImageView*>("long2")->getContentSize().width * this->itemsScaleX;

	this->addChild(this->rootNode);

	auto steam = utils::findChild(this, "steam");
	
    auto speaker = Button::create("games/WaterPipe/loa.png");
    speaker->setOpacity(0);
    speaker->setName("speaker");
    speaker->setTouchEnabled(false);
    this->rootNode->addChild(speaker);
    speaker->setPosition(Vec2(140, screen_size.height*0.92));
    speaker->addClickEventListener([=](Ref* sender){
        if(!speakerEnabled) return;
        speakerEnabled = false;
        inactiveMode(false);
		std::vector<std::string> vecAudio = {};
		if (_listSyncText.empty())
		{
			vecAudio = this->question->getSentenceSound();
			std::vector<int> answer_indexes = { };
			//            auto answer_index = 0;
			for (int i = 0; i < vecAudio.size(); i++) {
				if (vecAudio[i] == "#") {
					//                    answer_index = i;
					answer_indexes.push_back(i);
				}
			}
			for (int i = 0; i < answer_indexes.size(); i++) {
				vecAudio.insert(vecAudio.begin() + answer_indexes[i], this->question->getAnswerSoundAt(i));
			}

		}
		else
		{
			vecAudio.push_back(question->getPathAudioSentence());
		}
       
        AUDIO_ENGINE->playChain(vecAudio,[=]{
            speakerEnabled = true;
            inactiveMode(true,12.0f);
        });
    });
	return true;
}

void WaterPipe::onEnter() {
	MJMainGBase::onEnter();
    AUDIO_ENGINE->stopAllSound();
	_isPlayAudioCorrect = true;

	this->gaf = GAFWrapper::createGAF("gaf/WaterPipe/baby.gaf");
	this->rootNode->addChild(this->gaf);
	this->gaf->setScale(0.8f);
	this->gaf->setAnchorPoint(Vec2(0.5, 1));
	this->gaf->setLocalZOrder(1);


	this->audio = new audioPlayer;
    if(MJDEFAULT->getBoolForKey(key_background_music)){
//         this->audio->playBgSound();
        MJMainGBase::playBackgroundMusic("sounds/waterpipe/bg_music.mp3");
    }
	this->createDataParser();
	this->level = 0;

	auto bgIntro = cocos2d::ui::ImageView::create("games/WaterPipe/fixing_pipes.png");
	bgIntro->setAnchorPoint(Point(0.5, 0.5));
	bgIntro->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
	this->addChild(bgIntro);

	AUDIO_ENGINE->playSound("sounds/waterpipe/fixing_pipes.mp3", false, [this, bgIntro]() {
		bgIntro->removeFromParent();
		this->triggerStart();
		});
}

void WaterPipe::onExit()
{
    CC_SAFE_DELETE(this->question);
    CC_SAFE_DELETE(this->dataParser);
    CC_SAFE_DELETE(this->audio);
    MJMainGBase::onExit();
}

void WaterPipe::introduction() {

	Vector<FiniteTimeAction*> actions;


	auto showQuest = CallFunc::create([=]() {
		/*
		float yyy = this->pipeLayout->getContentSize().height;
		auto thisPosition = this->questionPipe->getPosition();
		this->questionPipe->setPosition(thisPosition.x, yyy + 50);
		this->questionPipe->runAction(MoveTo::create(0.75f, thisPosition));
		*/
		this->questionPipe->setPosition(this->questionPipeOriginalPosition);
		this->questionPipe->showUp();
		this->audio->playPipeShowSound();
		CCLOG("Show OK");
	});
	actions.pushBack(showQuest);
	actions.pushBack(DelayTime::create(2.0f));

	if (this->level == 0) {

		auto ar = this->dataParser->getIntros();

		auto cur = CallFunc::create([=]() {
            if(!ar.empty())
            {
                this->audio->playCustomSound(ar[0]);
            }
		});

		auto cur2 = CallFunc::create([=]() {
            if(ar.size()>=2)
            {
                this->audio->playCustomSound(ar[1]);
            }
		});
		actions.pushBack(cur);
		actions.pushBack(DelayTime::create(5.0f));
		actions.pushBack(cur2);
		actions.pushBack(DelayTime::create(3.5f));
	}

	auto readSentence = CallFunc::create([=]() {
		CCLOG("Start syncing");
		this->syncText();
		/*
		for (auto text : this->textVec) {
			auto fadeIn = FadeIn::create(1.8f);
			text->runAction(fadeIn);
		}
		*/
		CCLOG("Finish syncing");
	});
	actions.pushBack(readSentence);
    auto finish_read = CallFunc::create([=]{
        auto speaker = utils::findChild(this, "speaker");
        if(speaker)
            speaker->runAction(FadeIn::create(0.5));
        inactiveMode(true,12.0f);
    });
    actions.pushBack(finish_read);

	auto seq = Sequence::create(actions);

	this->runAction(seq);

}

void WaterPipe::createDataParser() {
	this->dataParser = new DataParser();
	this->dataParser->init();
}

void WaterPipe::createLevel(const int &level) {

	this->question = new QuestionInfo;
	this->dataParser->parseDataIntoQuestionInfoInLevel(this->question, level);
	this->pipeLayout->setVisible(true);

	this->choiceNode->setVisible(true);
	this->questionPipe->setPosition(this->questionPipeOriginalPosition);

	Size sz = Director::getInstance()->getVisibleSize();
	this->gaf->setPosition(Vec2(sz.width / 2, -1));
	this->gaf->runAction(MoveTo::create(1.0f, Vec2(sz.width / 2, sz.height * 0.55)));

}

void WaterPipe::triggerStart() {

	this->rightItems = 0;
	this->numWrong = 0;

	this->gaf->playSequence("sad", true);

	this->createLevel(this->level);


	this->choiceNode->removeAllChildren();

	this->makeChoices();

	this->showQuestion();

	this->introduction();
}

void WaterPipe::makeChoices() {

	this->choices.clear();
	for (int i = 0; i < this->question->getNumChoice(); i++) {

		Choice* node = reinterpret_cast<Choice*>(CSLoader::createNode("csb/game/WaterPipe/Choice.csb"));

		Sprite* part = node->getChildByName<Sprite*>("choice");

		ui::Text* text = part->getChildByName<ui::Text*>("key");

		text->setFontName("fonts/Montserrat-Bold.ttf");
		text->setFontSize(35);
		text->setTextColor(Color4B::WHITE);
		//text->enableOutline(Color4B::WHITE, 3);
		text->setString(this->question->choiceAt(i));

		node->setText(this->question->choiceAt(i));
		//---------------
		auto allPipes = this->pipeLayout->getChildByName<Node*>("pipePanel");
		auto originalLongPipe = allPipes->getChildByName<ui::Layout*>("long1");
		auto son = node->getChildByName<ui::ImageView*>("choice");
		son->setContentSize(Size(Vec2(son->getContentSize() * this->itemsScaleX)));
		son->setContentSize(Size(Vec2(son->getContentSize().width, originalLongPipe->getContentSize().height)));
		text->setPosition(0.5f * son->getContentSize());
		node->setTag(i);
		//---------------
		node->setIsAnswer(false);
		int k = 0;
		for (auto xxx : this->question->answersss) {
			if (this->question->choiceAt(i) == xxx) {
				node->setIsAnswer(true);
				node->setTag(k + this->question->getNumChoice());
			}
			k++;
		}

		// fit value text size
		float pipe_length = son->getContentSize().width;
		float label_width = text->getContentSize().width;
		float fSize = node->getRealWidth() - 22;
		while (pipe_length < label_width) {
			fSize -= 2;
			text->setFontSize(fSize);
			label_width = text->getContentSize().width;
		}
		text->setPosition(0.5f * son->getContentSize());

		node->setOriginal(this->choiceNode->getPositionX() + this->pipeLayout->getPositionX(), this->choiceNode->getPositionY() + this->pipeLayout->getPositionY());
		node->setPosition(Vec2(-300.0f, 0));

		node->setOriginalPosition(
			Size(
				node->getRealWidth() * i + 10.0f * i + this->choiceNode->getPositionX(),
				this->choiceNode->getPositionY() + 0//son->getContentSize().height
			)
		);

		node->setOriginalLocalPosition(
			Size(
				200 + node->getRealWidth() * i + 10.0f * i,
				//son->getContentSize().height
				0
			)
		);

		//node->setPosition(node->getOriginalLocalPosition());
		//CCLOG("%f %f", node->getOriginalLocalPosition().width, node->getOriginalLocalPosition().height);

		node->setupDelegate(this);

		node->makeUntouchable();

		node->makeCantTouchAnyMore(false);

		this->choices.pushBack(node);
		this->choiceNode->addChild(node);

	}

}

void WaterPipe::makeChoiceMoves() {

	this->audio->playSlideSound();
	for (auto choice : this->choices) {
		auto Move = MoveTo::create(0.5f, Vec2(choice->getOriginalLocalPosition()));
		auto delay = DelayTime::create(0.01f);
		auto moveBy = MoveBy::create(0.1f, Vec2(-25, 0));
		auto moveBy2 = MoveBy::create(0.1f, Vec2(25, 0));
		Sequence* seq = Sequence::create(Move, delay, moveBy, delay, moveBy2, nullptr);
		choice->runAction(seq);
	}
    utils::findChild<Button*>(rootNode, "speaker")->setTouchEnabled(true);
}

void WaterPipe::runRightAnimation(Choice* movingChoice, Blank* _blank) {

    AUDIO_ENGINE->stopAllSound();
	_isPlayAudioCorrect = true;
	movingChoice->makeCantTouchAnyMore(true);
	_listChoice.push_back(movingChoice);

	auto moving = movingChoice;

	++this->rightItems;

	Vector<FiniteTimeAction*> actions;

	auto callFuncMove = CallFunc::create([=]() {
		auto move = MoveTo::create(_isClickObject ? 1.5f : 0.5, _blank->getRealPosition() - this->choiceNodeOriginalPosition - Vec2(16, -19));
		_blank->setVisible(false);
		moving->runAction(move);
	});
	actions.pushBack(callFuncMove);
	actions.pushBack(DelayTime::create(0.05f));

	auto callFunc = CallFunc::create([=]() {
		auto choiceTimeline = CSLoader::createTimeline("csb/game/WaterPipe/Choice.csb");
		moving->runAction(choiceTimeline);
		choiceTimeline->play("right", false);
	});
	actions.pushBack(callFunc);
	actions.pushBack(DelayTime::create(_isClickObject ? 1.5f : 0.5));

	auto callFuncFin = CallFunc::create([=]() {

		this->audio->playPipeRunSound();
		Vector<FiniteTimeAction*> vecs;
		this->questionPipe->viberate();
	});
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(callFuncFin);
		actions.pushBack(DelayTime::create(1.5f));
	}

	auto makeZoomInEffect = CallFunc::create([=]() {
		Size sz = Director::getInstance()->getVisibleSize();
		auto scaleTo = ScaleTo::create(1.0f, 1.8f);
		auto moveTo = MoveTo::create(1.0f, Vec2(-1.0f * 0.38f * sz.width, 0));
		Spawn* zooming = Spawn::createWithTwoActions(scaleTo, moveTo);
		this->rootNode->runAction(zooming);
	});
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(makeZoomInEffect);
		actions.pushBack(DelayTime::create(0.25f));
	}

	auto makeGafAnimating = CallFunc::create([=]() {
		this->gaf->playSequence("happy", false);
	});
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(makeGafAnimating);
		actions.pushBack(DelayTime::create(2.5f));
	}

	auto makeZoomOutEffect = CallFunc::create([=]() {

		auto zooming = Spawn::createWithTwoActions(
			MoveTo::create(1.0f, Vec2(0, 0)),
			ScaleTo::create(1.0f, 1.0f)
		);

		this->rootNode->runAction(zooming);
	});
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(makeZoomOutEffect);
		actions.pushBack(DelayTime::create(1.25f));
	}

	auto allVanish = CallFunc::create([=]() {
		this->audio->playHuzzSound();
		Size sz = Director::getInstance()->getVisibleSize();
		this->questionPipe->runAction(MoveTo::create(0.4f, Vec2(this->questionPipe->getPositionX(), 2000)));
		this->choiceNode->setVisible(false);
		this->gaf->runAction(MoveTo::create(0.4f, Vec2(sz.width / 2, -1.0f)));
	});
	
	auto action1 = CallFunc::create([=] {
		for (int i = 0; i < _listChoice.size(); i++)
		{
			auto cc = _listChoice[i]->getTag();
			int kk = 0;
		}
		_listChoice = sortChoice(_listChoice);
		syncText(_listChoice);
		_listChoice.clear();	});
	
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(action1);
		actions.pushBack(DelayTime::create(_timeDelay));
		actions.pushBack(allVanish);
		actions.pushBack(DelayTime::create(0.45f));
	}
	else
	{
		if (!_listSyncText.empty())
		{
			actions.pushBack(CallFunc::create([=] {
				_valueIndexOfListPostBlank.erase(_valueIndexOfListPostBlank.begin() + _valueIndex);
				_indexBlank = -1;
				_valueIndex = -1;
				inactiveMode(true);
			}));
		}
	}

	auto changeLevel = CallFunc::create([=]() {
		AUDIO_ENGINE->stopAllAudio();
		_isPlayAudioCorrect = true;

		if (this->increaseLevel()) {
			this->triggerStart();
		}
		else {
			this->triggerGameOver();
		}

	});
	if (this->rightItems == this->question->answersss.size()) {
		actions.pushBack(DelayTime::create(0.5f));
		actions.pushBack(changeLevel);
	}


	Sequence* seq = Sequence::create(actions);

	moving->runAction(seq);
	_isClickObject = false;

}

bool WaterPipe::increaseLevel() {

	this->level += 1;
	if (this->level == this->dataParser->getNumQuest()) {
		return false;

	}
	return true;
}

void WaterPipe::runWrongAnimation(Choice* movingChoice, Blank* _blank) {
	auto moving = movingChoice;

	Vector<FiniteTimeAction*> vecs;

	auto callFuncMove = CallFunc::create([=]() {
		CCLOG("%f %f", movingChoice->getOriginalLocalPosition().width, movingChoice->getOriginalLocalPosition().height);
		auto move = MoveTo::create(0.1f, Vec2(movingChoice->getOriginalLocalPosition()));
		moving->runAction(move);
	});
	vecs.pushBack(callFuncMove);
	//EaseElasticOut

	auto callFuncChoice = CallFunc::create([=]() {
		auto choiceTimeline = CSLoader::createTimeline("csb/game/WaterPipe/Choice.csb");
		moving->runAction(choiceTimeline);
		choiceTimeline->play("wrong", false);
	});
	vecs.pushBack(callFuncChoice);

	auto makeTouchableAll = CallFunc::create([=]() {
		for (auto choice : this->choices) {
			choice->makeTouchable();
			choice->canBeClicked = true;
		}
	});
	vecs.pushBack(makeTouchableAll);

    auto c2 = CallFunc::create([=]() {

        if(!this->question->textsAndSounds.empty() && this->question->textsAndSounds.count(movingChoice->getText()))
        {
          this->audio->playCustomSound(this->question->textsAndSounds.at(movingChoice->getText()));
        }
        else
        {
            for (int i = 0; i < (int)this->question->answersss.size(); i++)
            {
                if (this->question->answersss[i] == movingChoice->getText())
                {
					if (_isPlayAudioCorrect)
					{
						_isPlayAudioCorrect = false;
						AUDIO_ENGINE->playSound(this->question->getAnswerSoundAt(i), false, [this] {
							_isPlayAudioCorrect = true;
							});
					}
                    break;
                }
            }
        }
   
    });
    
    vecs.pushBack(c2);

	Sequence * seq = Sequence::create(vecs);

	moving->runAction(seq);

}

float WaterPipe::getRealPositionX(ui::ImageView* Long) {
	return this->questionPipe->convertToWorldSpace(Long->getPosition()).x;
}

float WaterPipe::getRealPositionY(ui::ImageView* Long) {
	return this->questionPipe->convertToWorldSpace(Long->getPosition()).y;
}

std::vector <char> thinner = { 'f', 'i', 'j', 'l', 't', 'r', '.', ',' };
std::vector <char> thicker = { 'H', 'W', 'w', 'M', 'm', 'G', 'K', 'Q', 'O' };

float maximum(float a, float b) {
	if (a > b) return a;
	return b;
}

void WaterPipe::showQuestion() {

	auto allPipes = this->pipeLayout->getChildByName<Node*>("pipePanel");
	auto originalLongPipe = allPipes->getChildByName<ui::Layout*>("long1");

	originalLongPipe->setVisible(true);
	this->textVec.clear();
	this->blanks.clear();

	auto vecWords = this->question->getParts();
	int numBlank = this->question->getNumBlank();

	originalLongPipe->removeAllChildren();

	auto spriteBlank = Blank::createView()->getChildByName<Sprite*>("blank");
	float blankPadding = 28;
	float blankLength = spriteBlank->getContentSize().width * this->itemsScaleX + blankPadding;
	float blankHeight = spriteBlank->getContentSize().height;
	float realLength = originalLongPipe->getContentSize().width;

	realLength -= numBlank * blankLength + blankPadding;

	int blank_index = 0;
	int word_index = 0;

	Vec2 start_pos = Vec2(15, originalLongPipe->getContentSize().height);
	auto questionPaths = question->getQuestionParts();

	auto listHilight = question->getValueListHilightTextSentence();
	std::vector<std::string> listText = {};

	for(int i = 0; i < listHilight.size(); i++)
	{
		listText.push_back(listHilight[i].w);
	}

	float words_width= 0;
	float padding = 50;
	for (auto word : vecWords) {
		auto text = ui::Text::create(" ","fonts/Montserrat-Bold.ttf", 35);
		text->setTextColor(Color4B::WHITE);
		text->setAnchorPoint(Vec2(0.5, 0.5));
		text->setString(word);
		text->setOpacity(0);
		text->runAction(FadeIn::create(1.0f));
		words_width += (text->getContentSize().width + padding);
		this->textVec.pushBack(text);
	}

	bool need_fix_pipe = false;
	if (words_width > realLength) {
		padding = 30;
		words_width -= 20 * vecWords.size();
		need_fix_pipe = true;
	}
	_listPostBlank.clear();
	for (int i = 0; i < questionPaths.size(); ++i) {

		std::string path = questionPaths[i];
		if (path == "#" && blank_index < numBlank) {
			auto longPipe = CSLoader::createNode("csb/game/WaterPipe/LongPipe.csb");
			auto son = longPipe->getChildByName<ui::ImageView*>("pipe");
			Blank* blank = Blank::createView();
			blank->setPosition(Vec2(start_pos.x + blankPadding / 2, son->getContentSize().height * 0.2f));
			blank->setScale(this->itemsScaleX);
			blank->setLocalZOrder(0);
			this->blanks.pushBack(blank);
			originalLongPipe->addChild(blank);

			Vec2 pos = blank->getPosition() + originalLongPipe->getPosition() + this->questionPipe->getPosition();
			blank->setRealPosition(pos - Vec2(0, son->getContentSize().height * 0.2) - Vec2(0, blankHeight));
			auto postBlank = blank->getPosition();

			postBlank = Vec2(postBlank.x, originalLongPipe->getPositionY());
			_listPostBlank.push_back(postBlank);
			blank->setTag(i);
			blank->setItsAnswer(this->question->answersss[blank_index]);

			start_pos.x += blankLength;
			++blank_index;

		}
		else {
			auto longPipe = CSLoader::createNode("csb/game/WaterPipe/LongPipe.csb");
			auto son = longPipe->getChildByName<ui::ImageView*>("pipe");

			auto text = this->textVec.at(word_index);
			son->addChild(text);
			float pipe_length = text->getContentSize().width + padding;
			float pipe_length_max = pipe_length/ words_width  * realLength;
			words_width -= pipe_length;
			if (word_index == vecWords.size() - 1 && blank_index == numBlank ) {
				pipe_length = realLength;
			}
			else {
				pipe_length = min(pipe_length, realLength);
				if (need_fix_pipe) {
					pipe_length = min(pipe_length_max, pipe_length);
				}

			}

			realLength -= pipe_length;
			//text->setPosition(Vec2(pipe_length / 2, start_pos.y / 2));
			text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			float fix_padding = 0;
			if (!need_fix_pipe && !word_index && !i ) {
				fix_padding = 5;
			}

			text->setPosition(Vec2(padding / 2 + fix_padding, start_pos.y / 2));
			//

			son->setPositionX(maximum(0, start_pos.x));
			son->setPositionY(originalLongPipe->getContentSize().height);
			son->setContentSize(Size(pipe_length, originalLongPipe->getContentSize().height));
			//son->setScale9Enabled(true);
			start_pos.x += pipe_length;
			float sizeToSet = pipe_length;

			int maxActs = 20;
			Vector<FiniteTimeAction*> longgerr;
			for (int j = 0; j < maxActs; j++) {
				float each = sizeToSet / maxActs;
				auto setLength = CallFunc::create([=]() {
					son->setContentSize(Size((j + 1) * each, son->getContentSize().height));
				});
				longgerr.pushBack(setLength);
				longgerr.pushBack(DelayTime::create(0.75f / maxActs));
			}
			son->runAction(Sequence::create(longgerr));

			originalLongPipe->addChild(longPipe);

			// fit value text size
			float label_width = text->getContentSize().width;
			float fSize = text->getFontSize();
			while ((pipe_length - padding/2) < label_width) {
				fSize -= 2;
				text->setFontSize(fSize);
				label_width = text->getContentSize().width;
			}

			++word_index;

			CCLOG("%f length", pipe_length);

		}
	}
	_valueIndexOfListPostBlank.clear();
	_indexBlank = -1;
	_valueIndex = -1;
	for (int i = 0; i < _listPostBlank.size(); i++)
	{
		_valueIndexOfListPostBlank.push_back(i);
	}

	if (realLength > 0) {
		auto longPipe = CSLoader::createNode("csb/game/WaterPipe/LongPipe.csb");
		auto son = longPipe->getChildByName<ui::ImageView*>("pipe");

		son->setPositionX(maximum(0, start_pos.x));
		son->setPositionY(originalLongPipe->getContentSize().height);
		son->setContentSize(Size(realLength, originalLongPipe->getContentSize().height));

		float sizeToSet = realLength;
		int maxActs = 20;
		Vector<FiniteTimeAction*> longgerr;
		for (int j = 0; j < maxActs; j++) {
			float each = sizeToSet / maxActs;
			auto setLength = CallFunc::create([=]() {
				son->setContentSize(Size((j + 1) * each, son->getContentSize().height));
			});
			longgerr.pushBack(setLength);
			longgerr.pushBack(DelayTime::create(0.75f / maxActs));
		}
		son->runAction(Sequence::create(longgerr));
		originalLongPipe->addChild(longPipe);
	}
}

void WaterPipe::madeRight(Choice* movingChoice, Blank* _blank) {

	_blank->setVisible(false);

	for (auto choice : this->choices) {
		choice->makeUntouchable();
	}

	this->audio->playDropRightSound();
	this->runRightAnimation(movingChoice, _blank);

	auto c1 = CallFunc::create([=]() {
		this->audio->playWinSound();
	});
	auto c2 = CallFunc::create([=]() {
		int index = 0;
		for (int i = 0; i < (int)this->question->answersss.size(); i++) {
			if (this->question->answersss[i] == movingChoice->getText()) {
				index = i;
			}
		}
		this->audio->playCustomSound(this->question->getAnswerSoundAt(index));
	});

	auto c3 = CallFunc::create([=]() {
		for (auto choice : this->choices) {
			choice->makeTouchable();
		}
	});

	auto seq = Sequence::create(c1, DelayTime::create(1.2f), c2, DelayTime::create(1.0f), c3, nullptr);
	this->runAction(seq);

}

void WaterPipe::madeWrong(Choice* movingChoice, Blank* _blank) {
	if (movingChoice != nullptr) {
		++this->numWrong;
		this->audio->playWrongSound();
		this->runWrongAnimation(movingChoice, _blank);
	}
}

void WaterPipe::cancled(Choice* movingChoice) {
	auto cur = this->inRectt(movingChoice);
	if (this->inRectt(movingChoice)) {
		this->madeRight(movingChoice, cur);
	}
	else {
		this->madeWrong(movingChoice, cur);
        inactiveMode(true);
	}
}

void WaterPipe::clickedd(Choice* movingChoice) {
	if (this->isScheduled("sugget_game_pipe"))
	{
		this->unschedule("sugget_game_pipe");
	}
	if (auto hand = utils::findChild(this, "hand_sugget_game"))
	{
		hand->removeFromParent();
	}
    inactiveMode(false);
	for (auto choice : this->choices) {
		choice->makeUntouchable();
	}
	movingChoice->makeTouchable();
	this->audio->playDragSound();
}

void WaterPipe::triggerGameOver() {
    inactiveMode(false);
    AUDIO_ENGINE->stopSound("sounds/waterpipe/bg_music.mp3");
	MJMainGBase::onFinishGame();
}

void WaterPipe::changeRed(Blank* _blank) {
	if (!_blank->getRed()) {
		//this->stopAllActions();
		_blank->setRed(true);
		auto timeLine = CSLoader::createTimeline("csb/game/WaterPipe/MainScene.csb");
		timeLine->play("color", false);
		this->runAction(timeLine);
	}
}

void WaterPipe::changeWhite(Blank* _blank) {
	if (_blank->getRed()) {
		//this->stopAllActions();
		_blank->setRed(false);
		auto timeLine = CSLoader::createTimeline("csb/game/WaterPipe/MainScene.csb");
		timeLine->play("recolor", false);
		this->runAction(timeLine);
	}
}

Blank* WaterPipe::inRectt(Choice* movingChoice) {
	if (movingChoice == nullptr) {
		return nullptr;
	}
	float xnho = movingChoice->getPositionX() + this->choiceNode->getPositionX() - 50;
	float ynho = movingChoice->getPositionY() + this->choiceNode->getPositionY() - 50;
	float xlon = xnho + movingChoice->getRealWidth() + 50;
	float ylon = ynho + movingChoice->getRealHeight() + 50;

	CCLOG("%f %f %f %f", xnho, ynho, xlon, ylon);
	for (auto blank : this->blanks) {
		if (_numberAge < 4)
		{
			if (blank->getItsAnswer() == movingChoice->getText()) {
				_isClickObject = true;
				return blank;
			}
		}

		CCLOG("%s_bl-%s_ch", blank->getItsAnswer().c_str(), movingChoice->getText().c_str());

		Vec2 point1 = Vec2(xnho, ynho);
		if (blank->inBlank(point1)) {
			this->changeRed(blank);
			if (blank->getItsAnswer().c_str() == movingChoice->getText().c_str()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point2 = Vec2(xnho, ylon);
		if (blank->inBlank(point2)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point3 = Vec2(xlon, ynho);
		if (blank->inBlank(point3)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point4 = Vec2(xlon, ylon);
		if (blank->inBlank(point4)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point5 = Vec2((xlon + xnho) / 2.0f, (ylon + ynho) / 2.0f);
		if (blank->inBlank(point5)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point6 = Vec2((xlon + xnho) / 2, ylon);
		if (blank->inBlank(point6)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point7 = Vec2((xlon + xnho) / 2, ynho);
		if (blank->inBlank(point7)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point8 = Vec2(xnho, (ynho + ylon) / 2);
		if (blank->inBlank(point8)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}

		Vec2 point9 = Vec2(xlon, (ynho + ylon) / 2);
		if (blank->inBlank(point9)) {
			this->changeRed(blank);
			if (blank->getItsAnswer() == movingChoice->getText()) {
				return blank;
			}
			else {
				return nullptr;
			}
		}
	}

	for (auto blank : this->blanks) {
		this->changeWhite(blank);
	}

	return nullptr/* false*/;
}

void WaterPipe::syncText(std::vector<Choice*> listChoice) {

    speakerEnabled = false;
	std::vector<cocos2d::Vector<Sprite*> > parts;

	for (auto Long : this->textVec) {
		if (Long != nullptr) {
            
            StringUtils::StringUTF8 curLong = StringUtils::StringUTF8(Long->getString() +" ");

			Vector<Sprite*> poses;
                       
			for (int i = 0; i <  Long->getStringLength(); i++)
            {
                if (curLong.getString().at(i)._char == " " ||
                    curLong.getString().at(i)._char == "," /*|| curLong[i] == '.'*/) {
					if (poses.size() > 0)
                    {
                        CCLOG("WaterPipe::syncText parts.push_back(poses) char:%s,i=%d",curLong.getString().at(i)._char.c_str(),i);

						parts.push_back(poses);

						poses.clear();
					}
				}
				else
                {
                    cocos2d::Sprite *charSprite = Long->getLetter(i);
                    
                    if (!charSprite) continue;
                    
                    CCLOG("WaterPipe::syncText char:%s pushback",curLong.getString().at(i)._char.c_str());
                    
                    poses.pushBack(Long->getLetter(i));
				}
			}
		}
		else {
			CCLOG("NULL");
		}
	}

	auto vecAudio = this->question->getSentenceSound();
	CCLOG("%d", parts.size());

	_part = parts;
	this->unschedule("sugget_game_pipe");
	if (auto hand = utils::findChild(this, "hand_sugget_game"))
	{
		hand->removeFromParent();
	}
	syncText(parts, vecAudio, 0, 0, listChoice);
}

void WaterPipe::syncText(std::vector<cocos2d::Vector<Sprite*> > parts, std::vector<std::string> audios, int part_index /* = 0*/, int blank_index /*= 0*/, std::vector<Choice*> listChoice)
{
	speakerEnabled = false;
	_listSyncText = question->getValueListHilightTextSentence();
	if (!_listSyncText.empty())
	{
		syncText2(parts, audios, part_index, blank_index, listChoice);
		return;
	}
	int i = part_index + blank_index;

	if (i >= audios.size()) {
		if (this->isScheduled("sugget_game_pipe"))
		{
			this->unschedule("sugget_game_pipe");
		}
		if (this->isScheduled("inactive_mode")) {
			this->unschedule("inactive_mode");
		}
		handSuggetionGame();
        speakerEnabled = true;
		for (auto choice : this->choices) {
			choice->makeTouchable();
		}
		this->makeChoiceMoves();
		return;
	}

	if (audios[i] != "#") {
		CCLOG("%s", audios[i].c_str());

		auto vecNow = parts[part_index];
		for (auto x : vecNow) {
			if (x != nullptr) {
				x->setColor(Color3B(COLOR_HILIGHT));
			}
			else {
				CCLOG("NULL");
			}
		}
		AUDIO_ENGINE->playSound(audios[i], false, [=] {
			auto vecNow = parts[part_index];
			for (auto x : vecNow) {
				if (x != nullptr) {
					x->setColor(Color3B::WHITE);
				}
				else {
					CCLOG("NULL");
				}
			}
			syncText(parts, audios, part_index + 1, blank_index);
		});

	}
	else {
		auto blank = this->blanks.at(blank_index);
		blank->setColor(COLOR_HILIGHT);
		AUDIO_ENGINE->playSound(this->question->getAnswerSoundAt(blank_index), false, [=]() {
			blank->setColor(Color3B::WHITE);
			syncText(parts, audios, part_index, blank_index + 1);
		});

	}

}


void WaterPipe::syncText2(std::vector<cocos2d::Vector<Sprite*> > parts, std::vector<std::string> audios, int part_index , int blank_index, std::vector<Choice*> listChoice)
{
	this->unschedule("sugget_game_pipe");
	if (auto hand = utils::findChild(this, "hand_sugget_game"))
	{
		hand->removeFromParent();
	}
	std::vector<float> listTimeDelay = {};
	_actions.clear();
	_timeDelay = 0;

	for (int k = 0; k < _listSyncText.size(); k++)
	{
		auto sync = _listSyncText[k];
		listTimeDelay.push_back((sync.e - sync.s) / 1000.f);
		_timeDelay = _timeDelay + listTimeDelay[k];
	}

	std::vector<int> listIndexTextSpace = {};
	speakerEnabled = false;

	_partIndex = part_index;
	int iCheck = part_index + blank_index;

	int i = part_index + blank_index;

	int index = 0;
	int indexChoice = 0;

	std::vector<Choice*> listChoiceCopy = {};

	for (int i = 0; i < listChoice.size(); i++)
	{
		int tagChoice = listChoice[i]->getTag() - this->question->getNumChoice();
		int cc = 0;
	}

	while (i < audios.size())
	{
		if (audios[i] != "#") {
			CCLOG("%s", audios[i].c_str());

			auto action1 = CallFunc::create([=] {
				auto vecNow = parts[part_index];
				for (auto x : vecNow) {
					if (x != nullptr) {
						x->setColor(COLOR_HILIGHT);
					}
					else {
						CCLOG("NULL");
					}
				}
			});

			_actions.pushBack(action1);
			_actions.pushBack(DelayTime::create(listTimeDelay[i]));

			auto action2 = CallFunc::create([=] {
				auto vecNow = parts[part_index];
				for (auto x : vecNow) {
					if (x != nullptr) {
						x->setColor(Color3B::WHITE);
					}
					else {
						CCLOG("NULL");
					}
				}
			});
			_actions.pushBack(action2);
			part_index = part_index + 1;
		}
		else {
			if (listChoice.empty())
			{
				auto action3 = CallFunc::create([=] {
					auto blank = this->blanks.at(blank_index);
					blank->setColor(COLOR_HILIGHT);
					});
				_actions.pushBack(action3);
				_actions.pushBack(DelayTime::create(listTimeDelay[i]));
				auto action4 = CallFunc::create([=] {
					auto blank = this->blanks.at(blank_index);
					blank->setColor(Color3B::WHITE);
					});
				_actions.pushBack(action4);
			}
			else
			{
				auto choiceComplete = listChoice[indexChoice];
				if (auto textChoice = utils::findChild<ui::Text*>(choiceComplete, "key"))
				{
					auto action3 = CallFunc::create([=] {
						textChoice->setColor(COLOR_HILIGHT);
					});
					_actions.pushBack(action3); 
					_actions.pushBack(DelayTime::create(listTimeDelay[i]));
					auto action4 = CallFunc::create([=] {
						textChoice->setColor(Color3B::WHITE);
						});
					_actions.pushBack(action4);
				}

				indexChoice++;
			}
			blank_index = blank_index + 1;
		}
		i = part_index + blank_index;
	}
	auto action5 = CallFunc::create([=] {
		speakerEnabled = true;
		for (auto choice : this->choices) {
			choice->makeTouchable();
		}
		this->makeChoiceMoves();
	});
	_actions.pushBack(action5);
	auto audioPath = question->getPathAudioSentence();
	AUDIO_ENGINE->playSound(audioPath, false, [this]() {
		if (this->isScheduled("sugget_game_pipe"))
		{
			this->unschedule("sugget_game_pipe");
		}
		if (this->isScheduled("inactive_mode")) {
			this->unschedule("inactive_mode");
		}
		handSuggetionGame();
		});
	this->runAction(Sequence::create(_actions));
	
	
}

void WaterPipe::handSuggetionGame(float timeDlay)
{
	if (auto hand = utils::findChild(this, "hand_sugget_game"))
	{
		hand->removeFromParent();
	}
	if (_valueIndex == -1 && _indexBlank == -1)
	{
		_valueIndex = cocos2d::random(0, (int)_valueIndexOfListPostBlank.size() - 1);
		_indexBlank = _valueIndexOfListPostBlank[_valueIndex];
	}

	this->scheduleOnce([=](float)
	{
		auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");

		
		for (int i = 0; i < this->choices.size(); i++)
		{
			auto choice = this->choices.at(i);
			if (_indexBlank == (choice->getTag() - this->question->getNumChoice()))
			{
				AUDIO_ENGINE->playSound(this->dataParser->getIntros().at(1));
				if (_numberAge < 4)
				{
					auto to = ScaleTo::create(0.4f, 1.2f);
					auto nho = ScaleTo::create(0.4f, 1.0f);

					auto cc = Sequence::createWithTwoActions(to, nho);

					if (choice != nullptr) {
						choice->runAction(Sequence::create(
							cc,
							DelayTime::create(0.8f),
							cc,
							DelayTime::create(0.8f),
							cc,
							CallFunc::create([this] {
								inactiveMode(true);
								}),
							NULL
						));
					}
				}
				else {
					_postPart = choice->getPosition();
					hand->setPosition(_postPart + Vec2(50, -hand->getBoundingBox().size.height / 2));
					hand->setAnchorPoint(choice->getAnchorPoint());
					hand->setName("hand_sugget_game");
					choice->getParent()->addChild(hand, INT_MAX - 1);
					hand->runAction(Sequence::create(
						DelayTime::create(1.0f),
						MoveTo::create(2.5f, Vec2(_listPostBlank[_indexBlank].x + 20, _listPostBlank[_indexBlank].y - 35)),
						CallFunc::create([this] {
							inactiveMode(true);
							}),
						RemoveSelf::create(),
						NULL));
				}
			
				break;
			}
		}
	}, timeDlay, "sugget_game_pipe");

}


void WaterPipe::inactiveMode(bool enabled, float delay)
{
    if(CONFIG_MANAGER->isGameVMonkey())
    {
        if(enabled)
          {
              if(this->isScheduled("inactive_mode")){
                  this->unschedule("inactive_mode");
                  AUDIO_ENGINE->stopSound(this->dataParser->getIntros().front());
              }
              this->schedule([=](float dt)
              {
                  AUDIO_ENGINE->playSound(this->dataParser->getIntros().front());
              },12.0f,1000,delay,"inactive_mode");
          }
          else
          {
              this->unschedule("inactive_mode");
              AUDIO_ENGINE->stopSound(this->dataParser->getIntros().front());
          }
    }
    else
    {
        if(enabled)
        {
            if(this->isScheduled("inactive_mode")){
                this->unschedule("inactive_mode");
                AUDIO_ENGINE->stopSound(this->dataParser->getIntros().at(1));
            }
            this->schedule([=](float dt)
            {
				if (this->isScheduled("sugget_game_pipe"))
				{
					this->unschedule("sugget_game_pipe");
				}
				handSuggetionGame(0.5);
            },12.0f,1000,delay,"inactive_mode");
        }
        else
        {
            this->unschedule("inactive_mode");
            AUDIO_ENGINE->stopSound(this->dataParser->getIntros().at(1));
        }
    }
   
}

std::vector<Choice*> WaterPipe::sortChoice(std::vector<Choice*> listChoice)
{
	std::vector<int> listTag = {};
	std::vector<Choice*> listChoiceCopy = {};
	for (int i = 0; i < listChoice.size(); i++)
	{
		listTag.push_back(listChoice[i]->getTag());
	}
	std::sort(listTag.begin(), listTag.end());

	for (int i = 0; i < listTag.size(); i++)
	{
		for (int j = 0; j < listChoice.size(); j++)
		{
			if (listTag[i] == listChoice[j]->getTag())
			{
				listChoiceCopy.push_back(listChoice[j]);
			}
		}
	}
	listChoice.clear();
	return listChoiceCopy;
}
