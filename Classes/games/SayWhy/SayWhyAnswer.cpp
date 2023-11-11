#include "SayWhyAnswer.h"

#define GAF_ANSWER "gaf/saywhy/sw_answer/sw_answer.gaf"

SayWhyAnswer::SayWhyAnswer()
:m_gaf_answer{ nullptr }
,m_lbl_answer{ nullptr }
,answer_size {cocos2d::Size(360,110)}
{
}

SayWhyAnswer * SayWhyAnswer::createAnswer(SayWhyAnswerInfo info)
{
	auto answer = new SayWhyAnswer();
	if (answer && answer->init()) {
		answer->autorelease();
		answer->didLoadFromCSB(info);
	}
	else {
		CC_SAFE_DELETE(answer);
		answer = nullptr;
	}
	return answer;
}

void SayWhyAnswer::showAnswer(float dt)
{
	m_lbl_answer->setOpacity(0);
	m_gaf_answer->playSequence("dafe_in",false,[=](gaf::GAFObject * obj,const std::string &sqN){
		m_lbl_answer->runAction(cocos2d::FadeIn::create(dt));
	});
}

void SayWhyAnswer::hideAnswer(float dt)
{
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),cocos2d::FadeOut::create(0.25f),NULL));
}

void SayWhyAnswer::onClickedAnswer(std::function<void()> call_back)
{
	if (m_info.correct) {
		m_gaf_answer->playSequence("correct", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			if (call_back) {
				call_back();
			}
		});
	}
	else {
		m_gaf_answer->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			if (call_back) {
				call_back();
			}
		});
	}
}

bool SayWhyAnswer::isAnswerCorrect()
{
	return m_info.correct;
}


void SayWhyAnswer::didLoadFromCSB(SayWhyAnswerInfo info)
{
	m_info = info;
	loadFrames();
}

void SayWhyAnswer::loadFrames()
{
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setContentSize(answer_size);
	this->setCascadeOpacityEnabled(true);
	/*this->setBackGroundColor(cocos2d::Color3B::BLUE);
	this->setBackGroundColorOpacity(100);
	this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);*/

	m_gaf_answer = GAFWrapper::createGAF(GAF_ANSWER);
	m_gaf_answer->setPosition(cocos2d::Vec2(175,65));
	this->addChild(m_gaf_answer);

	m_lbl_answer = cocos2d::ui::Text::create(m_info.answer,font_name,26);
	m_lbl_answer->setTextAreaSize(cocos2d::Size(0.85 * answer_size.width, 0.9 * answer_size.height));
	m_lbl_answer->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_answer->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_lbl_answer->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_lbl_answer->setPosition(cocos2d::Vec2(0.575 * answer_size.width, 50));
	m_lbl_answer->setOpacity(0);
	this->addChild(m_lbl_answer, 100);
}
