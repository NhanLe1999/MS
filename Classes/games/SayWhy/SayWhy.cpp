// Bug remain 
// when touched true answer still touch wrong answer
// 
#include "SayWhy.h"
#include "HSAudioEngine.h"
#include "SayWhyQuestion.h"

USING_NS_CC;
using namespace ui;

#define IMG_BG "games/saywhy/sw_bg.png"

#define SOUND_BGM "sounds/saywhy/sw_bgm.mp3"
#define SW_SOUND_DET_CORRECT "sounds/saywhy/sw_det_correct.mp3"
#define SW_SOUND_DET_FADEIN "sounds/saywhy/sw_det_fadein.mp3"
#define SW_SOUND_DET_INTERACT "sounds/saywhy/sw_det_interact.mp3"
#define SW_SOUND_DET_WRONG "sounds/saywhy/sw_det_wrong.mp3"
#define SW_SOUND_WRONG "sounds/saywhy/sw_wrong.mp3"
#define SW_SOUND_CORRECT "sounds/saywhy/sw_correct.mp3"
#define LEARNING_REMINDER "sounds/saywhy/answer4.mp3"
#define BG_MUSIC "sounds/saywhy/sw_bgm.mp3"


SayWhy* SayWhy::_sayWhyInstance;

SayWhy* SayWhy::getInstance()
{
	return _sayWhyInstance;
}

SayWhy::SayWhy()
	:m_detective{ nullptr }
	, m_config_path{ "" }
	, m_sw_book{ nullptr }
	, layout_root{ nullptr }
{
}

bool SayWhy::init()
{
	if (!MJMainGBase::init()) {
		return false;
	}
	return true;
}

SayWhy* SayWhy::createGame(std::string json_path)
{
	auto game = new SayWhy();
	if (game && game->init()) {
		game->autorelease();
		game->m_config_path = "config.json";
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	_sayWhyInstance = game;
	return game;
}

void SayWhy::generateData()
{
	if (m_config_path.size() < 6) {
		initSampleGameInfo();
		number_turnplay = (int)list_game_info.size();
		return;
	}
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
	//get images
	std::vector<math::resource::Image*> m_images = math::resource::ResHelper::getImages("icons.json");

	// get data
	rapidjson::Value game_config = document.FindMember("game_config")->value.GetObject();
	number_turnplay = game_config.FindMember("number_turnplay")->value.GetInt();
	rapidjson::Value data_config = document.FindMember("data")->value.GetArray();
	for (rapidjson::SizeType i = 0; i < data_config.Size(); ++i) {
		rapidjson::Value item = data_config[i].GetObject();
		SayWhyBook::SayWhyBookInfo book_info;
		std::string question_id = item.FindMember("question")->value.GetString();
		auto image_question = math::resource::ResHelper::getImage(m_images, question_id);
		book_info.question.image_path = image_question->path;
		book_info.question.question = image_question->getText("name_1");
		book_info.question.audio_path = image_question->getAudios("name_1").front();
		rapidjson::Value answers_data = item.FindMember("answers")->value.GetArray();
		cocos2d::experimental::AudioEngine::preload(book_info.question.audio_path);
		for (rapidjson::SizeType j = 0; j < answers_data.Size(); ++j) {
			SayWhyAnswer::SayWhyAnswerInfo answer_info;
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
			book_info.list_answer.push_back(answer_info);
		}
		list_game_info.push_back(book_info);
	}

	if (number_turnplay > list_game_info.size()) {
		number_turnplay = (int)list_game_info.size();
	};
	//TODO
	//number_turnplay = 2;
}
void SayWhy::onEnter()
{
	MJMainGBase::onEnter();
	lockTouch(true);
	loadFrames();
	generateData();
	initBook();
	playBackgroundMusic(SOUND_BGM);
	showIntroGames();

}

void SayWhy::loadFrames()
{
	isReading = true;
	this->setContentSize(screen_size);
	layout_root = cocos2d::ui::Layout::create();
	layout_root->setContentSize(screen_size);
	this->addChild(layout_root, 0, "layout_root");

	auto bg_img = cocos2d::ui::ImageView::create(IMG_BG);
	bg_img->setPosition(screen_size / 2);
	layout_root->addChild(bg_img);

	m_detective = GAFWrapper::createGAF("gaf/saywhy/sw_detective/sw_detective.gaf");
	m_detective->setPosition(Vec2(screen_size.width / 2 - 250, -50));
	layout_root->addChild(m_detective, 100);

	auto why_layout = cocos2d::ui::Layout::create();
	why_layout->setScale(0.75f);
	why_layout->setContentSize(cocos2d::Size(152, 136));
	why_layout->setPosition(Vec2(screen_size.width / 2 - 175, 275));
	why_layout->setCascadeOpacityEnabled(true);
	why_layout->setOpacity(0);
	auto chat_bubble = cocos2d::ui::ImageView::create("games/saywhy/sw_chat.png");
	chat_bubble->setPosition(why_layout->getContentSize() / 2);
	chat_bubble->setFlippedX(true);
	why_layout->addChild(chat_bubble);

	auto lbl_text = cocos2d::ui::Text::create("WHY?", "fonts/KGWHATTHETEACHERWANTS.TTF", 40);
	lbl_text->setTextColor(cocos2d::Color4B::BLACK);
	lbl_text->setPosition(why_layout->getContentSize() / 2);

	why_layout->addChild(lbl_text);
	layout_root->addChild(why_layout, 101, "why_layout");




	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	
}

void SayWhy::radio() {
	_speaker->addClickEventListener([=](Ref*) {
		m_sw_book->stopGuideGame();
		m_sw_book->guideGame();
		AUDIO_ENGINE->stopSound("sounds/saywhy/Choose the correct answer!.mp3");
		if (isReading) {
			return;
		}
		m_sw_book->pauseAudio();
		string a = m_sw_book->getInfo().question.audio_path;
		auto _radio = m_sw_book->getInfo().flag;
		if (a != "" && _radio == true) {
			this->runAction(Sequence::create(CallFunc::create([=] {
				_idRadio = AUDIO_ENGINE->play(m_sw_book->getInfo().question.audio_path);
				_speaker->setTouchEnabled(false);
				}), DelayTime::create(3.0f), CallFunc::create([=] {
					_speaker->setTouchEnabled(true);
					}), NULL));
		}
		});
}

void SayWhy::initBook()
{

	m_sw_book = SayWhyBook::createBook(list_game_info);
	m_sw_book->setPosition(screen_size / 2);
	m_sw_book->setDelegate(this);
	m_sw_book->setSayWhy1(this);
	layout_root->addChild(m_sw_book, 10);
	radio();
}

void SayWhy::showIntroGames()
{
	AUDIO_ENGINE->playEffect("sounds/saywhy/intro1.mp3");
	m_sw_book->setScale(2.0f);
	m_sw_book->setPosition(Vec2(screen_size.width / 2 - m_sw_book->getContentSize().width / 4 * m_sw_book->getScaleX(), screen_size.height / 2 - m_sw_book->getContentSize().height / 8));
	m_sw_book->runAction(Sequence::create(DelayTime::create(1.0f), Spawn::create(ScaleTo::create(0.5f, 1.0f), MoveTo::create(0.5f, screen_size / 2), NULL),
		//DelayTime::create(0.25f),
		CallFunc::create([=] {
			m_sw_book->openBook([=] {
				AUDIO_ENGINE->playEffect(SW_SOUND_DET_FADEIN);
				m_detective->playSequence("fade_in", false, [=](gaf::GAFObject* obj, const std::string sqN) {
					startGame();
					m_detective->playSequence("idle", true, [=](gaf::GAFObject* obj, const std::string sqN) {
						});
					});
				AUDIO_ENGINE->play("sounds/saywhy/intro2.mp3", false, 1.0, [=] {
					m_sw_book->showQuestion(0.5f);
					});
				});
			}), NULL));

}

void SayWhy::startGame()
{
	_speaker->setTouchEnabled(true);
	AUDIO_ENGINE->play(BG_MUSIC, true, 0.5f);
	lockTouch(false);
	m_detective->playSequence("look", false, [=](gaf::GAFObject* obj, const std::string sqN) {
		m_detective->playSequence("idle", true);
		});
	AUDIO_ENGINE->play(SW_SOUND_DET_INTERACT);
	auto why_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(layout_root, "why_layout");
	why_layout->setScale(0);
	why_layout->setOpacity(255);
	why_layout->runAction(ScaleTo::create(0.25f, 0.75f));
}

void SayWhy::onChooseRight(SayWhyAnswer* answer)
{
	m_sw_book->stopAudio();
	AUDIO_ENGINE->stopEffect(_idRadio);
	m_sw_book->stopGuideGame();
	lockTouch(true);
	AUDIO_ENGINE->stopEffect(_idAudio);
	auto why_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(layout_root, "why_layout");
	why_layout->runAction(ScaleTo::create(0.25f, 0.0f));
	answer->setTouchEnabled(false);
	m_detective->playSequence("correct", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		m_detective->playSequence("idle", true);
		});
	answer->onClickedAnswer([=] {});
	AUDIO_ENGINE->playEffect(SW_SOUND_CORRECT);
	AUDIO_ENGINE->play(SW_SOUND_DET_CORRECT, false, 1.0, [=] {
		auto audio_path = std::string(answer->m_info.audio_path);
		AUDIO_ENGINE->play(audio_path, false, 1.0, [=] {
			onNextQuestion();
			});
		});
}

void SayWhy::onChooseWrong(SayWhyAnswer* answer)
{
	m_sw_book->stopGuideGame();
	m_sw_book->guideGame();
	m_sw_book->pauseAudio();
	AUDIO_ENGINE->stopEffect(_idRadio);
	answer->setTouchEnabled(false);
	//std::vector<std::string> chain;
	//chain.push_back(SW_SOUND_WRONG);
	//chain.push_back(SW_SOUND_DET_WRONG);
	//AUDIO_ENGINE->playChain(chain);
	AUDIO_ENGINE->playEffect(SW_SOUND_WRONG);
	AUDIO_ENGINE->playEffect(SW_SOUND_DET_WRONG);
	m_detective->playSequence("wrong", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		m_detective->playSequence("idle", true);
		});
	answer->onClickedAnswer([answer] {
		answer->setTouchEnabled(true);
		});
}

void SayWhy::onNextQuestion()
{
	if (m_sw_book->getQuestionIndex() < number_turnplay - 1 && m_sw_book->onNextQuestion()) {
		lockTouch(false);
		auto why_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(layout_root, "why_layout");
		why_layout->setScale(0);
		why_layout->setOpacity(255);
		why_layout->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			AUDIO_ENGINE->playEffect(SW_SOUND_DET_INTERACT);
			m_detective->playSequence("look", false, [=](gaf::GAFObject* obj, const std::string sqN) {
				m_detective->playSequence("idle", true);
				});
			}), ScaleTo::create(0.25f, 0.75f), NULL));
	}
	else {
		onFinishGame();
	}
}

void SayWhy::onFinishGame()
{
	MJMainGBase::onFinishGame();
}

void SayWhy::lockTouch(bool is_lock)
{
	CCLOG("LOCK TOUCH: %d", is_lock);
	auto lockLayoutCheck = cocos2d::utils::findChild(this, "lock_layout");
	if (lockLayoutCheck) {
		lockLayoutCheck->setVisible(is_lock);
	}
	else {
		auto lockLayout = cocos2d::ui::Layout::create();
		lockLayout->setContentSize(screen_size);
		lockLayout->setTouchEnabled(true);
		this->addChild(lockLayout, 10000, "lock_layout");
		lockLayout->setVisible(is_lock);
	}

}

void SayWhy::initSampleGameInfo()
{
	SayWhyBook::SayWhyBookInfo book_info1;
	book_info1.question.image_path = "sounds/saywhy/1026_Diamond seeker.jpg";
	book_info1.question.question = "The mother started to cry when she touched a little flower because...";
	book_info1.question.audio_path = "sounds/saywhy/question1.mp3";

	SayWhyAnswer::SayWhyAnswerInfo answer1;
	answer1.answer = std::string("Its thorns hurt her.");
	answer1.audio_path = "sounds/saywhy/q1answer1.mp3";
	answer1.correct = false;

	SayWhyAnswer::SayWhyAnswerInfo answer2;
	answer2.answer = std::string("She knew it was her child.");
	answer2.audio_path = "sounds/saywhy/q1answer2.mp3";
	answer2.correct = true;

	SayWhyAnswer::SayWhyAnswerInfo answer3;
	answer3.answer = std::string("She loved flowers.");
	answer3.audio_path = "sounds/saywhy/q1answer3.mp3";
	answer3.correct = false;

	book_info1.list_answer.push_back(answer1);
	book_info1.list_answer.push_back(answer2);
	book_info1.list_answer.push_back(answer3);
	list_game_info.push_back(book_info1);

	SayWhyBook::SayWhyBookInfo book_info2;
	book_info2.question.image_path = "sounds/saywhy/1006_Diamond seeker.jpg";
	book_info2.question.question = "Death brought the child back to life because...";
	book_info2.question.audio_path = "sounds/saywhy/question2.mp3";

	SayWhyAnswer::SayWhyAnswerInfo answer4;
	answer4.answer = std::string("The child's love for his mother deeply touched him");
	answer4.audio_path = "sounds/saywhy/answer6.mp3";
	answer4.correct = false;

	SayWhyAnswer::SayWhyAnswerInfo answer5;
	answer5.answer = std::string("The child's strong will deeply touched him");
	answer5.audio_path = "sounds/saywhy/answer5.mp3";
	answer5.correct = true;

	SayWhyAnswer::SayWhyAnswerInfo answer6;
	answer6.answer = std::string("The mother's love for her child deeply touched him");
	answer6.audio_path = "sounds/saywhy/answer4.mp3";
	answer6.correct = false;

	book_info2.list_answer.push_back(answer4);
	book_info2.list_answer.push_back(answer5);
	book_info2.list_answer.push_back(answer6);
	list_game_info.push_back(book_info2);
}

void SayWhy::onExit()
{
	MJMainGBase::onExit();
}

void SayWhy::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void SayWhy::onClickedAnswer(cocos2d::Ref* sender)
{
	auto answer = static_cast<SayWhyAnswer*>(sender);
	if (answer->isAnswerCorrect()) {
		onChooseRight(answer);
	}
	else {
		onChooseWrong(answer);
	}
}

void SayWhy::stopRadio() {
	AUDIO_ENGINE->stopEffect(_idRadio);
}


// Game Lifecycle