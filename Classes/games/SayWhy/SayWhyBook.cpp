#include "SayWhyBook.h"
#include "MJHelper.h"
#include "HSAudioEngine.h"
#include "SayWhy.h"

#define SW_SOUND_OPENBOOK "sounds/saywhy/sw_bookflap.mp3"
#define SW_SOUND_ANSWER "sounds/saywhy/sw_answer.mp3"
#define SW_SOUND_QUESTION "sounds/saywhy/sw_question.mp3"
#define LEARNING_REMINDER "sounds/saywhy/answer4.mp3"

SayWhyBook::SayWhyBook()
	:_delegate{ nullptr }
	, m_img_book{ nullptr }
	, m_book_back{ nullptr }
	, m_book_front{ nullptr }
	, m_picture_bg{ nullptr }
	, m_picture_ref{ nullptr }
	, book_size{ cocos2d::Size(1000.0f,700.0f) }
	, question_index{ 0 }
{
}

SayWhyBook* SayWhyBook::createBook(std::vector<SayWhyBook::SayWhyBookInfo> info)
{
	auto book = new SayWhyBook();
	if (book && book->init()) {
		book->autorelease();
		book->didLoadFromCSB(info);
	}
	else {
		CC_SAFE_DELETE(book);
		book = nullptr;
	}
	return book;
}

SayWhyBook::SayWhyBookInfo SayWhyBook::getInfo() const
{
	return m_list_info[question_index];
}



void SayWhyBook::openBook(std::function<void()> call_back)
{

	AUDIO_ENGINE->playSound(SW_SOUND_OPENBOOK);
	m_book_back->play(false, [=](gaf::GAFObject* obj) {
		m_book_back->setOpacity(0);
		m_img_book->setOpacity(255);
		for (auto answer : m_answers) {
			answer->setVisible(true);
		}
		});
	m_book_front->play(false, [=](gaf::GAFObject* obj) {
		m_picture_bg->setOpacity(255);
		m_picture_ref->setOpacity(255);
		m_book_front->setOpacity(0);
		cocos2d::utils::findChild(this, "tape_1")->setOpacity(255);
		cocos2d::utils::findChild(this, "tape_2")->setOpacity(255);
		if (call_back) {
			call_back();
		}
		});
}

void SayWhyBook::showQuestion(float delay_time)
{
	this->scheduleOnce([=](float dt) {
		CCLOG("Show Question at index : %d", question_index);
		m_picture->setOpacity(255);
		//m_question->setOpacity(255);
		AUDIO_ENGINE->play(SW_SOUND_QUESTION, false, 1.0, [=] {
			m_question->typeQuestion([=] {
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0f), cocos2d::CallFunc::create([=] {
					showAnswers();
					}), NULL));
				});
			});
		}, delay_time, cocos2d::StringUtils::format("show question at index %d", question_index));
}

void SayWhyBook::radioQuestion() {
	m_picture->addClickEventListener([=](cocos2d::Ref* sender) {
		SayWhy::getInstance()->stopRadio();
		AUDIO_ENGINE->stopEffect(_idAudioText);
		AUDIO_ENGINE->stopSound("sounds/saywhy/Choose the correct answer!.mp3");
		stopGuideGame();
		guideGame();
		this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=] {
			_idAudioImage = AUDIO_ENGINE->playEffect(getInfo().question.audio_path);
			m_picture->setTouchEnabled(false);
			}), cocos2d::DelayTime::create(3.0f), cocos2d::CallFunc::create([=] {
				m_picture->setTouchEnabled(true);
				}), NULL));
		});
	m_question->addClickEventListener([=](cocos2d::Ref* sender) {
		SayWhy::getInstance()->stopRadio();
		AUDIO_ENGINE->stopEffect(_idAudioImage);
		AUDIO_ENGINE->stopSound("sounds/saywhy/Choose the correct answer!.mp3");
		stopGuideGame();
		guideGame();
		this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=] {
			_idAudioText = AUDIO_ENGINE->playEffect(getInfo().question.audio_path);
			m_question->setTouchEnabled(false);
			}), cocos2d::DelayTime::create(3.0f), cocos2d::CallFunc::create([=] {
				m_question->setTouchEnabled(true);
				}), NULL));
		});
}


void SayWhyBook::stopAudio()
{
	AUDIO_ENGINE->stopEffect(_idAudioImage);
	AUDIO_ENGINE->stopEffect(_idAudioText);
	m_picture->setTouchEnabled(false);
	m_question->setTouchEnabled(false);
	count_answer = 0;
}

void SayWhyBook::pauseAudio() {
	AUDIO_ENGINE->stopEffect(_idAudioImage);
	AUDIO_ENGINE->stopEffect(_idAudioText);
	count_answer = 0;
}


void SayWhyBook::showAnswers(int index)
{
	
	if (count_answer < m_answers.size()) {
		auto answer = m_answers[count_answer];
		if (!answer->isVisible()) {
			answer->setVisible(true);
		}
		AUDIO_ENGINE->playSound(SW_SOUND_ANSWER);
		answer->runAction(cocos2d::ScaleTo::create(0.25f, 1.1f));
		answer->showAnswer();
		//        m_answers[index]->showAnswer();
		//        m_answers[index]->highlightText(cocos2d::Color3B::RED);
		AUDIO_ENGINE->playSound(std::string(answer->m_info.audio_path), false, [=] {
			//            m_answers[index]->highlightText(cocos2d::Color3B::BLACK);
			count_answer++;
			if (count_answer == 3) {
				m_picture->setTouchEnabled(true);
				m_question->setTouchEnabled(true);
				radioQuestion();
				m_list_info[_flag].flag = true;
				_flag++;
				guideGame();
			}
			this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=] {
				answer->runAction(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.25f, 1.0f), cocos2d::CallFunc::create([=] {
					}), NULL));
				this->showAnswers(count_answer);
				})));
			});
	}
	else {
		setAnswersTouchable(true);
		if (_sayWhy) {
			_sayWhy->setReading(false);
		}

	}
}

void SayWhyBook::showTapGuideGame(cocos2d::Vec2 pos, int loop, float delay_time, std::string gaf_guide_path) {
	auto guide_layout_check = cocos2d::Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	cocos2d::Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		auto hand = GAFWrapper::createGAF(gaf_guide_path);
		guide_layout->addChild(hand, 1900000000);
		hand->setPosition(pos);
		hand->play(false, [=](gaf::GAFObject* obj) {
			hand->removeFromParent();
			});
		}, 5, loop, delay_time, "animated_guide");
}

void SayWhyBook::setDelegate(SayWhyBookDelegate* delegate)
{
	_delegate = delegate;
}

void SayWhyBook::didLoadFromCSB(std::vector<SayWhyBook::SayWhyBookInfo> info)
{
	m_list_info = info;
	loadFrames();
	initQuestion();
	initAnswers();
}

void SayWhyBook::loadFrames()
{
	this->setCascadeOpacityEnabled(true);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setContentSize(book_size);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	m_img_book = cocos2d::ui::ImageView::create("games/saywhy/sw_book.png");
	m_img_book->setPosition(cocos2d::Vec2(book_size.width / 2, 400));
	m_img_book->setOpacity(0);
	this->addChild(m_img_book);

	m_picture_bg = cocos2d::ui::ImageView::create("games/saywhy/sw_picture.png");
	m_picture_bg->setPosition(cocos2d::Vec2(280, book_size.height * 0.65));
	m_picture_bg->setOpacity(0);
	this->addChild(m_picture_bg, 1);

	m_picture_ref = cocos2d::ui::ImageView::create("games/saywhy/sw_picture_ref.png");
	m_picture_ref->setPosition(cocos2d::Vec2(280, book_size.height * 0.65));
	m_picture_ref->setOpacity(0);
	this->addChild(m_picture_ref, 2);
	if (question_index < m_list_info.size()) {
		m_picture = cocos2d::ui::ImageView::create(m_list_info[question_index].question.image_path);
		m_picture->setPosition(cocos2d::Vec2(277.5f, book_size.height * 0.645));
		m_picture->setContentSize(cocos2d::Size(360, 260));
		m_picture->setRotation(-3.0f);
		m_picture->ignoreContentAdaptWithSize(false);
		m_picture->setOpacity(0);
		this->addChild(m_picture, 3);
	}

	auto tape1 = cocos2d::ui::ImageView::create("games/saywhy/sw_tape1.png");
	tape1->setPosition(cocos2d::Vec2(114, 584));
	tape1->setRotation(-30.0f);
	tape1->setOpacity(0);
	this->addChild(tape1, 4, "tape_1");

	auto tape2 = cocos2d::ui::ImageView::create("games/saywhy/sw_tape2.png");
	tape2->setPosition(cocos2d::Vec2(448, 322));
	tape2->setRotation(-30.0f);
	tape2->setOpacity(0);
	this->addChild(tape2, 4, "tape_2");

	m_book_back = GAFWrapper::createGAF("gaf/saywhy/sw_book_fadein_under/sw_book_fadein_under.gaf");
	m_book_back->setPosition(cocos2d::Vec2(book_size.width / 2, 400));
	this->addChild(m_book_back, 4);

	m_book_front = GAFWrapper::createGAF("gaf/saywhy/sw_book_fadein_over/sw_book_fadein_over.gaf");
	m_book_front->setPosition(cocos2d::Vec2(book_size.width / 2, 400));
	this->addChild(m_book_front, 7);
}

void SayWhyBook::initQuestion()
{
	if (question_index < m_list_info.size()) {
		m_question = SayWhyQuestion::createQuestion(m_list_info[question_index].question);
		m_question->setName("saywhyquestion");
		m_question->setPosition(cocos2d::Vec2(book_size.width * 0.75f, 480));
		m_question->setOpacity(0);
		this->addChild(m_question, 6);
	}

}

void SayWhyBook::initAnswers()
{
	if (question_index < m_list_info.size()) {
		SayWhyBook::SayWhyBookInfo m_info = m_list_info[question_index];
		mj::helper::shuffle(m_info.list_answer);
		m_answers.clear();
		for (int i = 0; i < 3 && i < m_info.list_answer.size(); ++i) {
			auto answer = SayWhyAnswer::createAnswer(m_info.list_answer[i]);
			answer->setPosition(cocos2d::Vec2(book_size.width * 0.74f, 330 - i * 120));
			answer->setVisible(false);
			this->addChild(answer, 6);
			m_answers.push_back(answer);
			answer->setTouchEnabled(false);
			answer->addClickEventListener([=](cocos2d::Ref* sender) {
				CCLOG("onClickedAnswer");
				if (_delegate) {
					_delegate->onClickedAnswer(answer);
				}
				});
		}
	}

}

void SayWhyBook::onLoadNextQuestion()
{
	initQuestion();
	initAnswers();
	m_picture->loadTexture(m_list_info[question_index].question.image_path);
}

bool SayWhyBook::onNextQuestion()
{
	if (_sayWhy) {
		_sayWhy->setReading(true);
	}
	unschedule("guideGame");
	m_picture->setTouchEnabled(false);
	m_question->setTouchEnabled(false);
	++question_index;
	count_answer = 0;
	if (question_index < m_list_info.size()) {
		auto dt = 0.25f;
		m_picture->runAction(cocos2d::FadeOut::create(0.25f));
		m_question->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.25f), cocos2d::CallFunc::create([=] {
			m_question->removeFromParent();
			}), NULL));
		for (auto answer : m_answers) {
			answer->setTouchEnabled(false);
			answer->hideAnswer(dt);
			dt += 0.25f;
		}
		this->scheduleOnce([=](float delay) {
			onLoadNextQuestion();
			m_picture->runAction(cocos2d::FadeIn::create(0.25f));
			showQuestion(0.5f);
			}, dt, "next_question");
		return true;
	}
	return false;
}

int SayWhyBook::getQuestionIndex() const
{
	return question_index;
}

void SayWhyBook::setAnswersTouchable(bool touch)
{
	for (auto answer : m_answers) {
		answer->setTouchEnabled(touch);
	}
}

void SayWhyBook::guideGame() {
	schedule([=](float) {
		_audio  = AUDIO_ENGINE->playSound("sounds/saywhy/Choose the correct answer!.mp3");
	},10.0f,"guideGame");
}
void SayWhyBook::stopGuideGame() {
	unschedule("guideGame");
}
