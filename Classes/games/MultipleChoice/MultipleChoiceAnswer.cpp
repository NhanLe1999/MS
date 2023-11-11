#include "MultipleChoiceAnswer.h"

#define GAF_ANSWER "gaf/saywhy/sw_answer/sw_answer.gaf"

MultipleChoiceAnswer::MultipleChoiceAnswer()
: _delegate{nullptr}
, m_lbl_answer{ nullptr }
, answer_size { cocos2d::Size(556, 84) }//
{
}

MultipleChoiceAnswer * MultipleChoiceAnswer::createAnswer(MultipleChoiceAnswerInfo info)
{
	auto answer = new MultipleChoiceAnswer();
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

void MultipleChoiceAnswer::setAnswerInfo(MultipleChoiceAnswerInfo info)
{
	m_info = info;
	m_lbl_answer->setString(m_info.answer);
	layout_root->setPosition(cocos2d::Vec2::ZERO);
}

void MultipleChoiceAnswer::showAnswer(float dt,std::function<void()> call_back)
{
	layout_root->setOpacity(0);
	m_lbl_answer->setOpacity(255);
	showImageAnswer(1);
	auto position = layout_root->getPosition();
	layout_root->setPosition(cocos2d::Vec2(position.x, position.y - 100));
	layout_root->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),
		cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position))),
		cocos2d::CallFunc::create([=] {
		if (call_back) {
			call_back();
		}
	}),NULL));

}

void MultipleChoiceAnswer::hideAnswer(float dt, std::function<void()> call_back, bool side_down)
{
	if (side_down) {
		layout_root->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),
			cocos2d::Spawn::createWithTwoActions(cocos2d::FadeOut::create(0.5f), cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, -200))), NULL));
	}
	else {
		layout_root->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeOut::create(0.5f), NULL));
	}
	this->scheduleOnce([=](float ldt) {
		if (call_back) {
			call_back();
		}
	}, dt + 0.5f, "hide answer call back");

}

void MultipleChoiceAnswer::showSpeaker()
{
	m_speaker->setTouchEnabled(true);
	m_speaker->runAction(cocos2d::FadeIn::create(0.25f));
}

void MultipleChoiceAnswer::hideSpeaker()
{
	m_speaker->setTouchEnabled(false);
	m_speaker->runAction(cocos2d::FadeOut::create(0.25f));
}

void MultipleChoiceAnswer::highlightText(cocos2d::Color3B color)
{
	if (m_lbl_answer) {
		m_lbl_answer->setTextColor(cocos2d::Color4B(color, 255));
	}
}


bool MultipleChoiceAnswer::isAnswerCorrect()
{
	return m_info.correct;
}

void MultipleChoiceAnswer::setDelegate(MultipleChoiceAnswerDelegate * delegate)
{
	_delegate = delegate;
}

cocos2d::ui::Button * MultipleChoiceAnswer::getSpeaker()
{
	return m_speaker;
}

void MultipleChoiceAnswer::showImageAnswer(int type)
{
	auto answer = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(layout_root, "img_answer");
	auto answer_correct = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(layout_root, "img_answer_correct");
	auto answer_wrong = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(layout_root, "img_answer_wrong");
	answer->setOpacity((255));
	answer_correct->setOpacity((type == 2 ? 255 : 0));
	answer_wrong->setOpacity((type == 3 ? 200 : 0));
}

void MultipleChoiceAnswer::didLoadFromCSB(MultipleChoiceAnswerInfo info)
{
	m_info = info;
	loadFrames();
}

void MultipleChoiceAnswer::loadFrames()
{
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setContentSize(answer_size);
	this->setCascadeOpacityEnabled(true);
	/*this->setBackGroundColor(cocos2d::Color3B::BLUE);
	this->setBackGroundColorOpacity(100);
	this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);*/

	layout_root = cocos2d::ui::Layout::create();
	layout_root->setContentSize(answer_size);
	layout_root->setCascadeOpacityEnabled(true);
	layout_root->setOpacity(0);
	this->addChild(layout_root);

	auto img_answer = cocos2d::ui::ImageView::create("games/multiplechoice/ts_text_a.png");
	img_answer->setPosition(answer_size / 2);
	img_answer->setOpacity(0);
	layout_root->addChild(img_answer, 1,"img_answer");
	img_answer->setScale9Enabled(true);
	auto img_answer_virtual_size = img_answer->getVirtualRendererSize();
	img_answer->setCapInsets(cocos2d::Rect(0.25 * img_answer_virtual_size.width, 1, 0.5*img_answer_virtual_size.width, img_answer_virtual_size.height - 2));
	img_answer->setContentSize(answer_size);

	auto img_answer_wrong = cocos2d::ui::ImageView::create("games/multiplechoice/ts_text_aw.png");
	img_answer_wrong->setPosition(answer_size / 2);
	img_answer_wrong->setOpacity(0);
	layout_root->addChild(img_answer_wrong, 2, "img_answer_wrong");
	img_answer_wrong->setScale9Enabled(true);
	auto img_answer_wrong_virtual_size = img_answer_wrong->getVirtualRendererSize();
	img_answer_wrong->setCapInsets(cocos2d::Rect(0.25 * img_answer_wrong_virtual_size.width, 1, 0.5*img_answer_wrong_virtual_size.width, img_answer_wrong_virtual_size.height - 2));
	img_answer_wrong->setContentSize(answer_size);


	auto img_answer_correct = cocos2d::ui::ImageView::create("games/multiplechoice/ts_text_ac.png");
	img_answer_correct->setPosition(answer_size / 2);
	img_answer_correct->setOpacity(0);
	layout_root->addChild(img_answer_correct, 3, "img_answer_correct");
	img_answer_correct->setScale9Enabled(true);
	auto img_answer_correct_virtual_size = img_answer_correct->getVirtualRendererSize();
	img_answer_correct->setCapInsets(cocos2d::Rect(0.25 * img_answer_correct_virtual_size.width, 1, 0.5*img_answer_correct_virtual_size.width, img_answer_correct_virtual_size.height - 2));
	img_answer_correct->setContentSize(answer_size);
	
	m_lbl_answer = cocos2d::ui::Text::create(m_info.answer,font_name, 22);
	m_lbl_answer->setTextAreaSize(cocos2d::Size(answer_size.width - 1.2 *answer_size.height, 0.9 * answer_size.height));
	m_lbl_answer->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_answer->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_lbl_answer->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_lbl_answer->setPosition(cocos2d::Vec2(answer_size.width/2.2 + 1.2 * answer_size.height/2,answer_size.height/2));
	layout_root->addChild(m_lbl_answer, 100,"lbl_text");

	this->addClickEventListener([=](cocos2d::Ref * sender) {
		CCLOG("on Clicked Answer");
		if (_delegate) {
			_delegate->onClickedAnswerDelegate(this);
		}
	});

	m_speaker = cocos2d::ui::Button::create("games/multiplechoice/loa icon.png");
	m_speaker->setContentSize(cocos2d::Size(answer_size.height, answer_size.height));
	m_speaker->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
	m_speaker->setOpacity(0);
	m_speaker->setPosition(cocos2d::Vec2(answer_size.height / 2, answer_size.height / 2));
	m_speaker->addClickEventListener([=] (cocos2d::Ref * sender){
		if (_delegate) {
			_delegate->onClickedSpeakerDelegate(this);
		}
	});
	this->addChild(m_speaker, 10);
}
