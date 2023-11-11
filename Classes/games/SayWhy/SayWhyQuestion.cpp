#include "SayWhyQuestion.h"
#include "HSAudioEngine.h"


SayWhyQuestion::SayWhyQuestion()
	:m_lbl_question{ nullptr }
	, question_size{ cocos2d::Size(350,180) }
{
}

SayWhyQuestion* SayWhyQuestion::createQuestion(SayWhyQuestionInfo question_info)
{
	auto question = new SayWhyQuestion();
	if (question && question->init()) {
		question->autorelease();
		question->didLoadFromCSB(question_info);
	}
	else {
		CC_SAFE_DELETE(question);
		question = nullptr;
	}

	return question;
}

void SayWhyQuestion::setQuestionInfo(SayWhyQuestionInfo question_info)
{
	m_question_info = question_info;
	m_lbl_question->setString(question_info.question);
}

void SayWhyQuestion::typeQuestion(std::function<void()> call_back)
{
	//    m_lbl_question->setOpacity(0);
	if (m_question_info.question.empty()) return;
	auto audio_path = std::string(m_question_info.audio_path);

	AUDIO_ENGINE->play(audio_path, false, 1.0, [=] {
		if (call_back) {
			call_back();
		}

		});

	int audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path, false, 0.0f);
	auto question_length = m_question_info.question.size();
	if (question_length == 0) question_length = 1;
	this->scheduleOnce([=](float delayt) {
		//        m_lbl_question->setOpacity(255);
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		//        if (duration <= 0.0f) {
		duration = question_length * 0.025f;
		//        }

		auto dt = duration * 0.8 / question_length;
		float letter_dt = 0;
		/*for (int i = 0; i < question_length; i++) {
			auto letter = m_lbl_question->getLetter(i);
			if(letter){
					letter->runAction(cocos2d::FadeOut::create(0.01f));
			}
		}*/

		for (int i = 0; i < question_length; ++i) {
			auto letter = m_lbl_question->getLetter(i);
			if (letter) {
				letter->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.05 * i), cocos2d::FadeIn::create(0.01f), NULL));
				letter_dt += dt;
			}
		}
		}, 0.1f, "get duration");
}

void SayWhyQuestion::didLoadFromCSB(SayWhyQuestionInfo question_info)
{
	m_question_info = question_info;
	this->setContentSize(question_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setCascadeOpacityEnabled(true);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);

	m_lbl_question = cocos2d::ui::Text::create(question_info.question, font_name, 32);
	m_lbl_question->setTextAreaSize(question_size);
	m_lbl_question->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	m_lbl_question->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_lbl_question->setPosition(question_size / 2);
	//m_lbl_question->setOpacity(255);
	m_lbl_question->setCascadeOpacityEnabled(true);
	this->addChild(m_lbl_question);
}