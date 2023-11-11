#include "MultipleChoiceQuestion.h"
#include "HSAudioEngine.h"
MultipleChoiceQuestion::MultipleChoiceQuestion()
:m_lbl_question {nullptr}
,question_size{cocos2d::Size(556,460)}
,_delegate{nullptr}
{
}

MultipleChoiceQuestion * MultipleChoiceQuestion::createQuestion(MultipleChoiceQuestionInfo question_info)
{
	auto question = new MultipleChoiceQuestion();
	if (question && question->init()) {
		question->didLoadFromCSB(question_info);
        question->autorelease();
	}
	else {
		CC_SAFE_DELETE(question);
		question = nullptr;
	}
	return question;
}

void MultipleChoiceQuestion::setQuestionInfo(MultipleChoiceQuestionInfo question_info)
{
	m_question_info = question_info;
	m_lbl_question->setString(question_info.question);
	m_img_question->loadTexture(question_info.image_path);
	auto layout_image = cocos2d::utils::findChild(this, "layout_image");
	auto layout_question = cocos2d::utils::findChild(this, "layout_question");
	layout_image->setPosition(cocos2d::Vec2(0, 120));
	layout_image->setOpacity(0);
	layout_question->setPosition(cocos2d::Vec2(0, 0));
	layout_question->setOpacity(0);
}

void MultipleChoiceQuestion::typeQuestion(std::function<void()> call_back)
{
	m_lbl_question->setOpacity(0);
	if (m_question_info.question.empty()) return;
	auto audio_path = std::string(m_question_info.audio_path);
	auto audio_id = AUDIO_ENGINE->playSound(audio_path);
	this->scheduleOnce([=](float sdt) {
		m_lbl_question->setOpacity(255);
		auto duration = AUDIO_ENGINE->getDuration(audio_id);
		if (duration < 0.0f) {
			duration = 1.75f;
		}

		auto question_length = m_question_info.question.size();
		auto dt = duration * 0.9 / question_length;
		float letter_dt = 0;
		for (int i = 0; i < question_length; ++i) {
			auto letter = m_lbl_question->getLetter(i);
			if (letter) {
				letter->setOpacity(0);
				letter->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(letter_dt), cocos2d::FadeIn::create(0.0f), NULL));
				letter_dt += dt;
			}
		}
		this->scheduleOnce([=](float dt) {
			if (call_back) {
				call_back();
			}
		}, duration, "call_back");
	},0.1f,"fix get duaration");
}

void MultipleChoiceQuestion::showQuestion(float delay, std::function<void()> call_back)
{
	this->setOpacity(255);
	auto layout_image = cocos2d::utils::findChild(this, "layout_image");
	auto layout_question = cocos2d::utils::findChild(this, "layout_question");
	layout_image->setOpacity(0);
	layout_question->setOpacity(0);

	if (m_question_info.show_image) {
		auto li_position = layout_image->getPosition();
		layout_image->setPosition(cocos2d::Vec2(li_position.x, li_position.y + 100));
		layout_image->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay),
			cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, li_position))), NULL));
	}

	auto lq_position = layout_question->getPosition();
	layout_question->setPosition(cocos2d::Vec2(lq_position.x, lq_position.y - 100));
	layout_question->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay),
		cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, lq_position))), NULL));

	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, delay + 0.5f, "show question call back");
}

void MultipleChoiceQuestion::hideQuestion(float delay, std::function<void()> call_back)
{
	auto layout_image = cocos2d::utils::findChild(this, "layout_image");
	auto layout_question = cocos2d::utils::findChild(this, "layout_question");
	
	layout_image->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay),
		cocos2d::Spawn::createWithTwoActions(cocos2d::FadeOut::create(0.5f), cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, 200))), NULL));
	layout_question->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay),
		cocos2d::Spawn::createWithTwoActions(cocos2d::FadeOut::create(0.5f), cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, -200))), NULL));

	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	},delay + 0.5f,"hide question call back");
}

void MultipleChoiceQuestion::setQuestionTouchable(bool touchable)
{
	auto layout_question = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_question");
	layout_question->setTouchEnabled(touchable);
}

void MultipleChoiceQuestion::setDelegate(MultipleChoiceQuestionDelegate * delegate)
{
	_delegate = delegate;
}

void MultipleChoiceQuestion::didLoadFromCSB(MultipleChoiceQuestionInfo question_info)
{
	m_question_info = question_info;
	this->setContentSize(question_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setCascadeOpacityEnabled(true);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	auto layout_image = cocos2d::ui::Layout::create();
	layout_image->setContentSize(cocos2d::Size(556,320));
	layout_image->setPosition(cocos2d::Vec2(0, 120));
	layout_image->setCascadeOpacityEnabled(true);
	layout_image->setOpacity(0);
	this->addChild(layout_image, 0, "layout_image");

	auto m_img_question_bg = cocos2d::ui::ImageView::create("games/multiplechoice/ts_frame.png");
//    m_img_question_bg->setContentSize(cocos2d::Size(500, 320));
	m_img_question_bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_img_question_bg->ignoreContentAdaptWithSize(false);
	m_img_question_bg->setPosition(cocos2d::Vec2( layout_image->getContentSize().width / 2, layout_image->getContentSize().height * 0.52 ));
	layout_image->addChild(m_img_question_bg, 1000);

	m_img_question = cocos2d::ui::ImageView::create(m_question_info.image_path);
	m_img_question->setContentSize(cocos2d::Size(475, 290));
	m_img_question->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_img_question->ignoreContentAdaptWithSize(false);
	m_img_question->setPosition(layout_image->getContentSize()/2);	
	layout_image->addChild(m_img_question, 1001);

	auto text_size = cocos2d::Size(556, 120);
	auto layout_question = cocos2d::ui::Layout::create();
	layout_question->setCascadeOpacityEnabled(true);
	layout_question->setContentSize(text_size);
	layout_question->setOpacity(0);
	this->addChild(layout_question, 0, "layout_question");

	auto question_bg = cocos2d::ui::ImageView::create("games/multiplechoice/ts_text_s.png");
	question_bg->setPosition(layout_question->getContentSize() / 2);
    question_bg->setScaleX(1.1);
    question_bg->setScaleY(1.2);
	layout_question->addChild(question_bg);

	m_lbl_question = cocos2d::ui::Text::create(m_question_info.question,font_name, 28);
	m_lbl_question->setTextAreaSize(cocos2d::Size(text_size.width* 0.98,text_size.height* 0.95));
	m_lbl_question->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	m_lbl_question->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_lbl_question->setPosition(layout_question->getContentSize() / 2);
	m_lbl_question->setCascadeOpacityEnabled(true);
	layout_question->addChild(m_lbl_question,10);

	layout_question->addClickEventListener([=](cocos2d::Ref * sender) {
		CCLOG("click question");
		if (_delegate) {
			_delegate->onClickedQuestionDelegate(this);
		}
	});
}
