//
//  GameWordMachinePipe.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/27/17.
//
//

#include "GameWordMachinePipe.h"
#include "MJHelper.h"
#include "GameWordMachineConstants.h"
#include "StoryDataModel.h"
#include "MsHelperGame.h"
using namespace cocos2d;

INITIALIZE_READER(MJGameWordMachinePipe);

USING_NS_CC;

void MJGameWordMachinePipe::loadSentence(mj::GameObject sentence, std::string answer, bool isVisible, std::vector<math::resource::Sync_Text> syncText, std::string audoPath) {
    _answer = answer;
    _sentence = sentence;
    _syncText = syncText;
    _audoPath = audoPath;
    _indexGame = 0;
    auto ww = 0.f;
    _textFullAnswer.clear();
    sizeTextBegin.clear();
    auto splited_sentence = mj::helper::split(_sentence.text, '#');
    if(_sentence.text.length() > 15){
        font_sz = 40;
    }


    auto space = ui::Text::create(" ", "fonts/monkeystories_medium.ttf", font_sz);
    auto margin = space->getContentSize().width;

    auto sentence_placeholder = utils::findChild(this, "sentence_holder");

    auto tag = 1;
    auto is_first_word = true;

     _sumSizeText = 0.0;
     _sumSizeTextFull = 0.0;
   
    for (auto &s : splited_sentence) {
        if (s.empty() && !is_first_word) {
            //answer placeholder
            auto text = ui::Text::create(answer, "fonts/monkeystories_medium.ttf", font_sz);
            text->setTextColor(Color4B(255, 0, 0, 0));
            text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            text->setPosition(Point(ww, 0));
            text->setCascadeOpacityEnabled(true);
            ww += text->getContentSize().width + margin;
            text->setName("answer");  
            _sumSizeTextFull = _sumSizeTextFull + text->getBoundingBox().size.width;
            sentence_placeholder->addChild(text);

            auto placeholder = ui::Text::create("____", "fonts/monkeystories_medium.ttf", 40);
            auto underline = ui::Layout::create();
            underline->setContentSize(Size(placeholder->getContentSize().width, 5));
            underline->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            underline->setBackGroundColor(Color3B::BLACK);
            underline->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
            underline->setPosition(Point(text->getContentSize().width / 2, -3));
            underline->setName("underline");
            _sumSizeText = underline->getBoundingBox().size.width;

            text->addChild(underline);

            text->setTag(tag++);

            _text_translation_factor = (text->getContentSize().width - placeholder->getContentSize().width) / 2;
        } else {
            auto text = ui::Text::create(s, "fonts/monkeystories_medium.ttf", font_sz);
            _textFullAnswer.push_back(s);
            text->setName(s);
            text->setColor(Color3B::BLACK);
            text->setTextColor(Color4B::BLACK);
            text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);        
            text->setPosition(Point(ww, 0));
            ww += text->getContentSize().width + margin;
            sentence_placeholder->addChild(text);
            text->setTag(tag++);
            sizeTextBegin.push_back(text->getBoundingBox().size.width);
            _sumSizeTextFull = _sumSizeTextFull + text->getBoundingBox().size.width;
        }
        is_first_word = false;
    }
    
    ww -= margin;
    sentence_placeholder->setContentSize(Size(ww, sentence_placeholder->getContentSize().height));
    auto sizeScreen = Director::getInstance()->getWinSize();
    int index = 0;
    while (_sumSizeTextFull > sizeScreen.width)
    {
        index++;
        float sizeText = 0;
        int fontSize = 0;
        for (int i = 0; i < splited_sentence.size(); i++)
        {
            cocos2d::ui::Text* text = nullptr;
            if (!splited_sentence[i].empty())
            {
                text = utils::findChild<ui::Text*>(this, splited_sentence[i]);
            }
            else 
            {
                text = utils::findChild<ui::Text*>(this, "answer");
            }
            if (text)
            {
                if (fontSize == 0)
                {
                    fontSize = text->getFontSize() - 1;
                }
                text->setFontSize(fontSize);
                text->setPositionX(sizeText);
                sizeText = sizeText + text->getContentSize().width + margin;
            }
        }
        sizeText -= margin;
        _sumSizeTextFull = sizeText;
    }
    sentence_placeholder->setContentSize(Size(_sumSizeTextFull, sentence_placeholder->getContentSize().height));

    float sizePlaceholder = 0;
    if (index != 0)
    {
        for (int i = 0; i < _textFullAnswer.size(); i++)
        {
            if (auto text = utils::findChild<ui::Text*>(this, _textFullAnswer[i]))
            {
                if (i == 0)
                {
                    auto cc = text->getBoundingBox().size.width;
                    sizePlaceholder = sizePlaceholder + cc + margin;
                    text->setPositionX(text->getPositionX() + (sizeTextBegin[i] - text->getBoundingBox().size.width));
                    //CCLOG("Pos: %d", text->getPositionX() + (sizeTextBegin[i] - text->getBoundingBox().size.width));
                }
            }
        }
        if (auto textAnswer = utils::findChild(this, "answer"))
        {
            sizePlaceholder = sizePlaceholder + textAnswer->getBoundingBox().size.width + margin;
        }
    }
    adjust(isVisible);
}

void MJGameWordMachinePipe::adjust(bool isVisible) {
    auto underline = utils::findChild(this, "underline");
    auto underline_position_x = (underline->getParent()->convertToWorldSpace(underline->getPosition())).x;

    auto sprout = utils::findChild(this, "sprout");
    auto sprout_position_x = (sprout->getParent()->convertToWorldSpace(sprout->getPosition())).x + sprout->getContentSize().width - 30;

    auto sentence = utils::findChild(this, "sentence_holder");

    auto translate_sync_vector = Vec2(- underline_position_x + sprout_position_x, 0);

    sentence->setPosition(sentence->getPosition() + translate_sync_vector);

    auto sentence_bb = sentence->getBoundingBox();
    sentence_bb.origin = sentence->getParent()->convertToWorldSpace(sentence_bb.origin);

    auto centered_position = this->getParent()->getParent()->convertToWorldSpace(Vec2(705, 0));

    auto translate_center_vector = Vec2(- sentence_bb.origin.x - sentence_bb.size.width / 2 + centered_position.x, 0);
    sentence->setPosition(sentence->getPosition() + translate_center_vector);
    sprout->setPosition(sprout->getPosition() + translate_center_vector);
//
    sentence_bb = sentence->getBoundingBox();
    sentence_bb.origin = sentence->getParent()->convertToWorldSpace(sentence_bb.origin);

    auto sentence_max_x = sentence_bb.origin.x + sentence_bb.size.width;
    if (sentence_max_x > Director::getInstance()->getVisibleSize().width - 25) {
        auto translate_back_vector = Vec2((Director::getInstance()->getVisibleSize().width - 1024) / 2.f + 1024 - 25 - sentence_max_x, 0);
        sentence->setPosition(sentence->getPosition() + translate_back_vector);
        sprout->setPosition(sprout->getPosition() + translate_back_vector);

    }

    auto answer = utils::findChild(this, "answer");
    for (auto c : sentence->getChildren()) if (c->getTag() != answer->getTag()){
        if (c->getTag() < answer->getTag()) {
            c->setPosition(c->getPosition() + Point(_text_translation_factor, 0));
        } else {
            c->setPosition(c->getPosition() - Point(_text_translation_factor, 0));
        }
    }

    int pos = sentence->getPositionX();
    if (pos < 350)
    {
        int offset = 350 - pos;
        sentence->setPositionX(350);
        sprout->setPositionX(sprout->getPositionX() + offset);
    }

    cocos2d::Vec2 postSentencePlaceholder = sentence->getPosition();
    
    sentence->setPosition(cocos2d::Vec2(sentence->getPositionX(), -250));

    sentence->runAction(Sequence::create(
        DelayTime::create(isVisible ? 2.0f : 0),
         MoveTo::create(0.5, postSentencePlaceholder),
        nullptr
    ));

    //construct pipe

    sprout_position_x = sprout->getPositionX();

    auto pipe_container = utils::findChild(this, "pipes");
    pipe_container->removeAllChildren();
    auto pipe_container_x = pipe_container->getPositionX();

    auto pipe_length = sprout_position_x - pipe_container_x;

    pipe_container->setContentSize(Size(pipe_length, pipe_container->getContentSize().height));

    auto number_of_pipe = int(floor(pipe_length / 100) + 1);
    auto piece_length = pipe_length / number_of_pipe;

    auto ww = 0.f;

    for (auto i = 0; i < number_of_pipe; i++) {
        auto pipe = ui::ImageView::create("games/word_machine/pipe.png");
        pipe->setScale(0);
        pipe->setScale9Enabled(true);
        pipe->setCapInsets(Rect(20, 11, 30, 13));
        pipe->setContentSize(Size(piece_length, pipe->getContentSize().height));

        pipe->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        pipe->setPosition(Point(ww, 0) + Point(0, pipe->getContentSize().height / 2));

        ww += pipe->getContentSize().width;

        pipe_container->addChild(pipe);

        _pipes.push_back(pipe);
    }
}

double MJGameWordMachinePipe::runAnimation() {
    CCLOG("Load tank");
    auto tank = utils::findChild(this, "tank");
    tank->runAction(Sequence::create(ScaleTo::create(0.2, 1, 1.3), EaseElasticOut::create(ScaleTo::create(1, 1, 1), 0.2), nullptr));

    auto counter = 1;
    auto step = 0.2;
    for (auto pipe : _pipes) {
        auto delaytime = step * counter;
        counter++;
        pipe->runAction(Sequence::create(DelayTime::create(delaytime), ScaleTo::create(0.3, 1, 1.7), EaseBackOut::create(ScaleTo::create(0.7, 1, 1)), nullptr));
    }

    CCLOG("Load sprout");
    auto sprout = utils::findChild(this, "sprout");
    sprout->runAction(Sequence::create(DelayTime::create(counter * step), Spawn::create(Sequence::create(ScaleTo::create(0.3, 1, 1.3), EaseElasticOut::create(ScaleTo::create(1, 1, 1)), nullptr), NULL), NULL));

    auto text = ui::Text::create(_answer, "fonts/monkeystories_medium.ttf", font_sz);
    text->setColor(Color3B::BLACK);
    text->setTextColor(Color4B::BLACK);

    auto sprout_position = sprout->getParent()->convertToWorldSpace(sprout->getPosition());

    text->setScale(50.0 / text->getContentSize().width);
    text->setPosition(sprout->getPosition() + Point(96, -50));
    text->setName("final_answer");
    this->addChild(text);
    sprout->setLocalZOrder(1);

    CCLOG("Load answer");
    auto answer_text = utils::findChild(this, "answer");
    auto answer_position = answer_text->getPosition() + answer_text->getContentSize() / 2;
    answer_position = answer_text->getParent()->convertToWorldSpace(answer_position);

    auto text_position = this->convertToNodeSpace(answer_position);

    CCLOG("Load underline");
    auto underline = utils::findChild(this, "underline");
    text->runAction(Sequence::create(DelayTime::create(counter * step + 0.1), CallFunc::create([] {
       cocos2d::experimental::AudioEngine::play2d(soundWMRight);
    }), Spawn::create(EaseBackOut::create(EaseCubicActionOut::create(MoveTo::create(1, text_position))), EaseBackOut::create(ScaleTo::create(1, 1)), nullptr), NULL));

    underline->runAction(Sequence::create(DelayTime::create(counter * step + 0.3), FadeOut::create(0.5), NULL));

    EaseOut::create(ScaleTo::create(0.25, 1), 1.5);

    CCLOG("Load sentence_holder");
    auto sentence = utils::findChild(this, "sentence_holder");
    for (auto c : sentence->getChildren()) if (c->getTag() != answer_text->getTag()){
        if (c->getTag() < answer_text->getTag()) {
            c->runAction(Sequence::create(DelayTime::create(counter * step + 0.1), EaseBackOut::create(MoveBy::create(0.3, Vec2(-_text_translation_factor, 0))), NULL));
        } else {
            c->runAction(Sequence::create(DelayTime::create(counter * step + 0.1), EaseBackOut::create(MoveBy::create(0.3, Vec2(+_text_translation_factor, 0))), NULL));
        }
    }

    float timeDelay = _syncText.empty() ? 0.75f : 0.0f;

    this->runAction(Sequence::create(DelayTime::create(counter * step), 
        CallFunc::create([=] {
        auto tint_action = Sequence::create(DelayTime::create(0.5), TintTo::create(1, 255, 0, 0), NULL);

        text->runAction(tint_action->clone());
        for (auto c : sentence->getChildren()) {
            if (dynamic_cast<ui::Text *>(c)) {
                c->runAction(tint_action->clone());
            }
        }
        if (!_syncText.empty())
        {
            for (int i = 0; i < _textFullAnswer.size(); i++)
            {
                if (_textFullAnswer[i].length() >= 1)
                {
                    if (auto textAnswer = utils::findChild<ui::Text*>(this, _textFullAnswer[i]))
                    {
                        if (_indexGame == 0)
                        {
                            _indexGame++;
                            ms::game::startHighlightText(_syncText, textAnswer);
                            text->setVisible(false);
                        }
                        if (textAnswer)
                        {
                            textAnswer->removeFromParent();
                        }
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < _textFullAnswer.size(); i++)
            {
                if (_textFullAnswer[i].length() >= 1)
                {
                    if (auto textAnswer = utils::findChild<ui::Text*>(this, _textFullAnswer[i]))
                    {
                        text->setTextColor(Color4B::RED);
                        textAnswer->setTextColor(Color4B::RED);
                    }
                }
            }
        }
    }),
        DelayTime::create(timeDelay),
        CallFunc::create([this] {
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(_sentence.audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
                if (_delegate) {
                    _delegate->onReadSentence();
                }
            });
        }),
        NULL));

    return counter * step + 0.3;
}

double MJGameWordMachinePipe::runAnimationReload() {
    auto sprout = utils::findChild(this, "sprout");
    auto delay_counter = 0;
    auto step = 0.2;

    sprout->runAction(Sequence::create(DelayTime::create(step * delay_counter++), ScaleTo::create(0.05, 1.5), ScaleTo::create(0.2, 0), NULL));

    for (auto it = _pipes.rbegin(); it != _pipes.rend(); it++) {
        auto pipe = *it;
        pipe->runAction(Sequence::create(DelayTime::create(step * delay_counter++), ScaleTo::create(0.05, 1.5), ScaleTo::create(0.2, 0), nullptr));
    }

    auto sentence_holder = utils::findChild(this, "sentence_holder");// ui::Helper::seekWidgetByName(this, "sentence_holder");
    for (auto c : sentence_holder->getChildren()) {
        c->runAction(EaseQuarticActionIn::create(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 2, 0))));
        c->runAction(FadeOut::create(0.5));
    }

    auto answer = this->getChildByName("final_answer");
    if (answer) {
        answer->runAction(EaseQuarticActionIn::create(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 2, 0))));
        answer->runAction(FadeOut::create(0.5));
    }

    this->scheduleOnce([=](float) {
        sentence_holder->removeAllChildren();
        _pipes.clear();
        if (answer) {
            answer->removeFromParent();
        }
    }, step * delay_counter, "cleanup");

    return delay_counter * step;

}

double MJGameWordMachinePipe::runAnimationEnter() {
    auto sprout = utils::findChild(this, "sprout");// ui::Helper::seekWidgetByName(this, "sprout");
    auto pipe_container = utils::findChild(this, "pipes");// ui::Helper::seekWidgetByName(this, "pipes");

    sprout->setScale(0);

    auto delay_counter = 0;
    auto step = 0.2;
    for (auto pipe : _pipes) {
        pipe->runAction(Sequence::create(DelayTime::create(step * delay_counter), ScaleTo::create(0.2, 1.5), ScaleTo::create(0.05, 1), nullptr));
        delay_counter ++;
    }

    sprout->runAction(Sequence::create(DelayTime::create(step * delay_counter), ScaleTo::create(0.2, 1.5), ScaleTo::create(0.05, 1), NULL));

    auto sentence_holder = utils::findChild(this, "sentence_holder");//ui::Helper::seekWidgetByName(this, "sentence_holder");
    for (auto c : sentence_holder->getChildren()) {
        c->setPosition(c->getPosition() + Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));
        c->runAction(EaseQuarticActionIn::create(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 2, 0))));
        c->runAction(FadeIn::create(0.5));
    }

    pipe_container->setContentSize(Size(sprout->getPositionX() - pipe_container->getPositionX(), pipe_container->getContentSize().height));

    return delay_counter * step;

}
