// remain
// sounds effect
// hide effect

#include "MultipleChoice.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
USING_NS_CC;
using namespace ui;

#define IMG_BG "games/multiplechoice/ts_bg.png"
#define IMG_FG "games/multiplechoice/ts_fg.png"

#define SOUND_BGM "sounds/multiplechoice/ts_bgm.mp3"
#define MC_SOUND_CORRECT "sounds/multiplechoice/ts_correct.mp3"
#define MC_SOUND_NAVY_FADEIN "sounds/multiplechoice/ts_navyfadein.mp3"
#define MC_SOUND_FLAG_START "sounds/multiplechoice/ts_flagstart.mp3"
#define MC_SOUND_FLAG_LOOP "sounds/multiplechoice/ts_flagloop.mp3"
#define MC_SOUND_WRONG "sounds/multiplechoice/ts_wrong.mp3"
#define MC_SOUND_FADE_OUT "sounds/multiplechoice/ts_fadeout.mp3"
#define MC_SOUND_QFADEIN "sounds/multiplechoice/ts_qfadein.mp3"
#define MC_SOUND_EXCELLENT "sounds/multiplechoice/Excellent.mp3"

MultipleChoice::MultipleChoice()
	:m_navy{ nullptr }
	, m_config_path{ "" }
	, layout_root{ nullptr }
	, question_index{ 0 }
{
}

bool MultipleChoice::init()
{
	if (!MJMainGBase::init()) {
		return false;
	}
	return true;
}

MultipleChoice * MultipleChoice::createGame()
{
	auto game = new MultipleChoice();
	if (game && game->init()) {
		game->autorelease();
		game->m_config_path = "config.json";
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	return game;
}

void MultipleChoice::generateData()
{
//    if (m_config_path.size() < 6) {
//        initSampleGameInfo();
//        number_turnplay = (int) list_game_info.size();
//        return;
//    }
	auto buffer_string = cocos2d::FileUtils::getInstance()->getStringFromFile(m_config_path);
	rapidjson::Document document;
	document.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

	if (document.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	// log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	document.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    std::vector<math::resource::Image*> m_images = math::resource::ResHelper::getImages("icons.json");
	// get data
	rapidjson::Value game_config = document.FindMember("game_config")->value.GetObject();
	number_turnplay = game_config.FindMember("number_turnplay")->value.GetInt();
    
	rapidjson::Value data_config = document.FindMember("data")->value.GetArray();
	for (rapidjson::SizeType i = 0; i < data_config.Size(); ++i) {
		rapidjson::Value item = data_config[i].GetObject();
		MultipleChoiceInfo game_info;
		std::string question_id = item.FindMember("question")->value.GetString();
		auto image_question = math::resource::ResHelper::getImage(m_images, question_id);
		game_info._question.image_path = image_question->path;
		game_info._question.question = image_question->getText("name_1");
		game_info._question.audio_path = image_question->getAudios("name_1").front();
		if (item.HasMember("show_image") && item["show_image"].IsBool()) {
			game_info._question.show_image = item.FindMember("show_image")->value.GetBool();
		}
		rapidjson::Value answers_data = item.FindMember("answers")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < answers_data.Size(); ++j) {
			MultipleChoiceAnswer::MultipleChoiceAnswerInfo answer_info;
			auto answer_id = answers_data[j]["image_id"].GetString();
			auto image_answer = math::resource::ResHelper::getImage(m_images, answer_id);
			answer_info.answer = image_answer->getText("name_1");
			answer_info.audio_path = image_answer->getAudios("name_1").front();
			if (answers_data[j].HasMember("is_correct") && answers_data[j]["is_correct"].IsBool()) {
				answer_info.correct = answers_data[j]["is_correct"].GetBool();
			}
			else {
				answer_info.correct = false;
			}
			image_answer->getSyncText();
			game_info._answers.push_back(answer_info);
		}
		mj::helper::shuffle(game_info._answers);
		list_game_info.push_back(game_info);
	}

	if (number_turnplay >  (int) list_game_info.size()) {
		number_turnplay =  (int) list_game_info.size();
	};
}
void MultipleChoice::onEnter()
{
	MJMainGBase::onEnter();
	lockTouch(true);
	loadFrames();
	generateData();
	initQuestionAndAnswers();
	playBackgroundMusic(SOUND_BGM);
	showIntroGames();
}

void MultipleChoice::loadFrames()
{
	this->setContentSize(screen_size);
	layout_root = cocos2d::ui::Layout::create();
	layout_root->setContentSize(screen_size);
	this->addChild(layout_root, 0, "layout_root");

	auto bg_img = cocos2d::ui::ImageView::create(IMG_BG);
	bg_img->setPosition(screen_size / 2);
	layout_root->addChild(bg_img, 0);

	auto bg_fr = cocos2d::ui::ImageView::create(IMG_FG);
	bg_fr->setPosition(screen_size / 2);
	layout_root->addChild(bg_fr, 2);

	m_navy = GAFWrapper::createGAF("gaf/multiplechoice/ts_navy/ts_navy.gaf");
	m_navy->setPosition(Vec2(screen_size.width / 2 + 450, 0));
	m_navy->setOpacity(0);
	m_navy->play(true);
	layout_root->addChild(m_navy, 100);

	auto ts_flag = GAFWrapper::createGAF("gaf/multiplechoice/ts_flag/ts_flag.gaf");
	ts_flag->setPosition(Vec2(screen_size.width / 2 - 425, screen_size.height / 2));
	ts_flag->play(true);
	layout_root->addChild(ts_flag, 1);
}

void MultipleChoice::initQuestionAndAnswers()
{
	m_question = MultipleChoiceQuestion::createQuestion();
	
	layout_root->addChild(m_question, 10);
	m_question->setQuestionTouchable(false);
	m_question->setDelegate(this);
	for (int i = 0; i < 3; ++i) {
		auto answer = MultipleChoiceAnswer::createAnswer();
		answer->setDelegate(this);
		answer->setTouchEnabled(true);
		layout_root->addChild(answer, 11 + i);
		m_answers.push_back(answer);
	}
	setQAPosition();
}

void MultipleChoice::setGamePlay(int index)
{
	m_question->setQuestionInfo(list_game_info[index]._question);
	for (int i = 0; i < 3 || i < list_game_info[index]._answers.size(); ++i) {
		m_answers[i]->setAnswerInfo(list_game_info[index]._answers[i]);
		m_answers[i]->showImageAnswer(1);
	}
	setQAPosition();
}

void MultipleChoice::showIntroGames()
{
	AUDIO_ENGINE->playSound(MC_SOUND_FLAG_START, false, [=] {
		AUDIO_ENGINE->playSound(MC_SOUND_FLAG_LOOP, true);
	});
	layout_root->setAnchorPoint(cocos2d::Vec2(0.5f - 700.0f / screen_size.width, 0.5f));
	//layout_root->setPosition(screen_size / 2);
	layout_root->ignoreAnchorPointForPosition(true);
	layout_root->setScale(1.35f);
	layout_root->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
		showGamename();
	}), ScaleTo::create(0.5f, 1.0f), NULL));

	m_navy->setOpacity(0);
	auto navy_pos = m_navy->getPosition();
	m_navy->setPosition(Vec2(navy_pos.x, navy_pos.y - 150));
	m_navy->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(MC_SOUND_NAVY_FADEIN);
	}), Spawn::createWithTwoActions(MoveTo::create(0.5f, navy_pos), FadeIn::create(0.5f)), NULL));

}

void MultipleChoice::showGamename()
{
	auto game_name = cocos2d::ui::ImageView::create("games/multiplechoice/ts_gamename.png");
	game_name->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	if (game_name->getContentSize().width / 2 < 0.3 * screen_size.width) {
		game_name->setPosition(Vec2(screen_size.width * 0.7f, screen_size.height / 2));
	}
	else {
		game_name->setPosition(Vec2(screen_size.width * 0.675f, screen_size.height / 2));
	}
	layout_root->addChild(game_name,100);
	game_name->runAction(Sequence::create(cocos2d::FadeIn::create(1.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound("sounds/multiplechoice/intro.mp3", false, [=] {
			game_name->runAction(Sequence::create(FadeOut::create(1.5f), CallFunc::create([=] {
				game_name->removeFromParent();
			}), NULL));
			this->scheduleOnce([=](float dt) {
				startGame();
			}, 0.75f, "start game");
		});
	}), NULL));
}

void MultipleChoice::startGame()
{
	if (question_index < number_turnplay) {
		setGamePlay(question_index);
		showQuestion();
	}
	m_navy->playSequence("idle_start", false, [=](gaf::GAFObject * obj, const std::string sqN) {
		m_navy->playSequence("idle", true);
	});
}

void MultipleChoice::onChooseRight(MultipleChoiceAnswer * answer)
{
	lockTouch(true);
	for (int i = 0; i < 3 || i < list_game_info[question_index]._answers.size(); ++i) {
		m_answers[i]->hideSpeaker();
	}
	m_navy->playSequence("correct", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		m_navy->playSequence("idle", true);
	});
	answer->showImageAnswer(2);
	float amp = 10.0f;
	float dt = 0.05f;
	auto shake = cocos2d::Sequence::create(cocos2d::MoveBy::create(dt, Vec2(-amp, 0)), cocos2d::MoveBy::create(2 * dt, Vec2(2 * amp, 0)), cocos2d::MoveBy::create(dt, Vec2(-amp, 0)), NULL);
	answer->runAction(Sequence::create(cocos2d::Repeat::create(shake, 3), CallFunc::create([=] {
		answer->setTouchEnabled(true);
		answer->showImageAnswer(1);
	}), NULL));

	auto gaf_confetti1 = GAFWrapper::createGAF("gaf/multiplechoice/confetti/confetti.gaf");
	gaf_confetti1->setFlippedX(true);
	gaf_confetti1->setPosition(Vec2(screen_size.width / 2 - 300, 200));
	layout_root->addChild(gaf_confetti1, 10000);
	gaf_confetti1->play(false, [=](gaf::GAFObject * obj) {
		gaf_confetti1->removeFromParent();
	});
	auto gaf_confetti2 = GAFWrapper::createGAF("gaf/multiplechoice/confetti/confetti.gaf");
	gaf_confetti2->setPosition(Vec2(screen_size.width / 2 + 300, 200));
	layout_root->addChild(gaf_confetti2, 10000);
	gaf_confetti2->play(false, [=](gaf::GAFObject * obj) {
		gaf_confetti2->removeFromParent();
	});
	hideWrongAnswers();
	AUDIO_ENGINE->playSound(MC_SOUND_CORRECT, false, [=] {
		auto audio_path = std::string(answer->m_info.audio_path);
        auto audio_id = AUDIO_ENGINE->playSound(audio_path, false, [=]{
            this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=]{
                    onNextQuestion();
            }), NULL));
        });
	});

}

void MultipleChoice::onChooseWrong(MultipleChoiceAnswer * answer)
{
	answer->setTouchEnabled(false);
	answer->showImageAnswer(3);
	float amp = 10.0f;
	float dt = 0.05f;
	auto shake = cocos2d::Sequence::create(cocos2d::MoveBy::create(dt, Vec2(-amp, 0)), cocos2d::MoveBy::create(2 * dt, Vec2(2 * amp, 0)), cocos2d::MoveBy::create(dt, Vec2(-amp, 0)), NULL);
	answer->runAction(Sequence::create(cocos2d::Repeat::create(shake, 3), CallFunc::create([=] {
		answer->setTouchEnabled(true);
		answer->showImageAnswer(1);
	}), NULL));
	AUDIO_ENGINE->stopSound(MC_SOUND_WRONG);
	AUDIO_ENGINE->playSound(MC_SOUND_WRONG);
	m_navy->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		m_navy->playSequence("idle", true);
	});

}

void MultipleChoice::onNextQuestion()
{
	++question_index;
    count_answer = 0;
	if (question_index < number_turnplay) {
		nextQuestion();
	}
	else {
		AUDIO_ENGINE->playSound(MC_SOUND_FADE_OUT);

		auto navy_pos = m_navy->getPosition();
		m_navy->runAction(Spawn::createWithTwoActions(MoveTo::create(0.4f, Vec2(navy_pos.x, navy_pos.y - 200)), FadeOut::create(0.4f)));

		m_question->hideQuestion(0.0f, [=] {
			endGame();
		});
		for (int i = 0; i < m_answers.size(); ++i) {
			m_answers[i]->hideAnswer(0.0f, nullptr, true);
		}
	}
}

void MultipleChoice::endGame() {
	MJMainGBase::onFinishGame();
}

void MultipleChoice::showQuestion(std::function<void()> call_back)
{
//    auto chain = std::vector<std::string>({ MC_SOUND_QFADEIN, m_question->m_question_info.audio_path });
    AUDIO_ENGINE->play(MC_SOUND_QFADEIN, false, 1.0, [=]{
        this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
            m_question->showQuestion();
            AUDIO_ENGINE->play(m_question->m_question_info.audio_path, false, 1.0, [=]{
                showAnswers();
            });
        }), NULL) );
    });
}

void MultipleChoice::showAnswers(int index, float dt)
{
    if(count_answer < list_game_info[question_index]._answers.size()) {
        m_answers[index]->showAnswer();
        m_answers[index]->highlightText(cocos2d::Color3B::RED);
        answer_id =  AUDIO_ENGINE->play(m_answers[index]->m_info.audio_path, false, 1.0, [=]{
            m_answers[index]->highlightText(cocos2d::Color3B::BLACK);
            count_answer++;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([=]{
                this->showAnswers(count_answer);
            })));

        });
    }else{

        lockTouch(false);
        m_question->setQuestionTouchable(true);
        for (int i = 0; i < 3 || i < list_game_info[question_index]._answers.size(); ++i) {
            m_answers[i]->showSpeaker();
        }
    }
}

void MultipleChoice::hideWrongAnswers()
{
	for (int i = 0; i < list_game_info[question_index]._answers.size(); ++i) {
		if (!m_answers[i]->isAnswerCorrect()) {
			m_answers[i]->hideAnswer();
		}
	}
}

void MultipleChoice::nextQuestion(std::function<void()> call_back)
{
	AUDIO_ENGINE->playSound(MC_SOUND_FADE_OUT);
	m_question->hideQuestion(0.0f, [=] {
		setGamePlay(question_index);
		showQuestion();
	});
    for (int i = 0; i < list_game_info[question_index]._answers.size(); ++i) {
        m_answers[i]->hideAnswer(0.0f, nullptr, true);
    }

}

void MultipleChoice::lockTouch(bool lock)
{
	CCLOG("LOCK TOUCH: %d", lock);
	auto lockLayoutCheck = cocos2d::utils::findChild(this, "lock_layout");
	if (lockLayoutCheck) {
		lockLayoutCheck->setVisible(lock);
	}
	else {
		auto lockLayout = cocos2d::ui::Layout::create();
		lockLayout->setContentSize(screen_size);
		lockLayout->setTouchEnabled(true);
		this->addChild(lockLayout, 10000, "lock_layout");
		lockLayout->setVisible(lock);
	}

}

void MultipleChoice::setQAPosition()
{
	if (m_question->m_question_info.show_image) {
		m_question->setPosition(Vec2(screen_size.width * 0.7f, 550));
		for (int i = 0; i < m_answers.size(); ++i) {
			auto answer = m_answers[i];
			auto pos = Vec2(m_question->getPositionX(), m_question->getPositionY() - m_question->getContentSize().height / 2 - (i + 0.5) *answer->getContentSize().height - (i + 1) * 15);
			answer->setPosition(pos);
		}
	}
	else {
		m_question->setPosition(Vec2(screen_size.width * 0.7f, 750));
		for (int i = 0; i < m_answers.size(); ++i) {
			auto answer = m_answers[i];
			auto pos = Vec2(m_question->getPositionX(), m_question->getPositionY() - m_question->getContentSize().height / 2 - (i + 0.5) *answer->getContentSize().height - (i + 1) * 20 - 50);
			answer->setPosition(pos);
		}
	}
	
}

void MultipleChoice::initSampleGameInfo()
{
	MultipleChoiceInfo game_info1;
	game_info1._question.image_path = "sounds/saywhy/1026_Diamond seeker.jpg";
	game_info1._question.question = "How was Tiny diffirent from two other character?";
	game_info1._question.audio_path = "sounds/multiplechoice/q1.mp3";
	game_info1._question.show_image = true;

	MultipleChoiceAnswer::MultipleChoiceAnswerInfo answer1;
	answer1.answer = std::string("He was taller than them.");
	answer1.audio_path = "sounds/multiplechoice/q1a1.mp3";
	answer1.correct = true;

	MultipleChoiceAnswer::MultipleChoiceAnswerInfo answer2;
	answer2.answer = std::string("He was smaller than them.");
	answer2.audio_path = "sounds/multiplechoice/q1a2.mp3";
	answer2.correct = false;

	MultipleChoiceAnswer::MultipleChoiceAnswerInfo answer3;
	answer3.answer = std::string("He was bigger than them.");
	answer3.audio_path = "sounds/multiplechoice/q1a3.mp3";
	answer3.correct = false;

	game_info1._answers.push_back(answer1);
	game_info1._answers.push_back(answer2);
	game_info1._answers.push_back(answer3);
	mj::helper::shuffle(game_info1._answers);
	list_game_info.push_back(game_info1);
	mj::helper::shuffle(game_info1._answers);
	game_info1._question.show_image = false;
	list_game_info.push_back(game_info1);
}

void MultipleChoice::onExit()
{
	MJMainGBase::onExit();
}

void MultipleChoice::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void MultipleChoice::onClickedAnswerDelegate(cocos2d::Ref * sender)
{
    AUDIO_ENGINE->stopEffect(q_id);
	auto answer = static_cast<MultipleChoiceAnswer *>(sender);
	if (answer->isAnswerCorrect()) {
		onChooseRight(answer);
	}
	else {
		onChooseWrong(answer);
	}
}

void MultipleChoice::onClickedSpeakerDelegate(cocos2d::Ref * sender)
{
    AUDIO_ENGINE->stopEffect(q_id);
    AUDIO_ENGINE->stopEffect(ans_id);
//    AUDIO_ENGINE->stopSound(m_question->m_question_info.audio_path);
//    for (int i = 0; i < 3 || i < list_game_info[question_index]._answers.size(); ++i) {
//        AUDIO_ENGINE->stopSound(m_answers[i]->m_info.audio_path);
//    }
	auto answer = static_cast<MultipleChoiceAnswer *>(sender);
	ans_id = AUDIO_ENGINE->playEffect(answer->m_info.audio_path);
}

void MultipleChoice::onClickedQuestionDelegate(cocos2d::Ref * sender)
{
    AUDIO_ENGINE->stopEffect(q_id);
	auto question = static_cast<MultipleChoiceQuestion *>(sender);
//    for (int i = 0; i < 3 || i < list_game_info[question_index]._answers.size(); ++i) {
//        AUDIO_ENGINE->stopSound(m_answers[i]->m_info.audio_path);
//    }
	q_id = AUDIO_ENGINE->playEffect(question->m_question_info.audio_path);
}

// Game Lifecycle
