//
//  GameBirdieAnswerOverlay.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/5/17.
//

#include "GameBirdieAnswerOverlay.h"
#include "GameBirdieAnswer.h"
#include "GameBirdieConstants.h"

INITIALIZE_READER(MJGameBirdieAnswerOverlay);

USING_NS_CC;

MJGameBirdieAnswerOverlay * MJGameBirdieAnswerOverlay::createOverlay(mj::BirdieDataSet data) {
    auto overlay = static_cast<MJGameBirdieAnswerOverlay *>(CSLoader::createNodeWithVisibleSize("csb/game/birdie/game_birdie_answers.csb"));
    if (overlay) {
        overlay->didLoadFromCSB(data);
        return overlay;
    }

    CC_SAFE_DELETE(overlay);
    return nullptr;
}

void MJGameBirdieAnswerOverlay::didLoadFromCSB(mj::BirdieDataSet data) {
    _data = data;
    _delegate = nullptr;

    bool is_one_row = false;

    auto options_counter = 0;
    for (auto i = 2; i > 0; i--) {
        auto answer_row_name = StringUtils::format("answers_row_%d", i);
        auto answer_row = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, answer_row_name));

        auto ww = 0.f;
        auto margin = 15.f;

        for (auto c = 0; c < 2; c++) {
            if (options_counter >= _data.options.size()) {
                break;
            }
            auto answer = MJGameBirdieAnswer::createAnswer(_data.options[options_counter++]);
            answer->setAnchorPoint(Point::ANCHOR_MIDDLE);
            answer->setPosition(Point(ww,20) + answer->getContentSize() / 2);
            answer->setTouchEnabled(true);
            answer->addTouchEventListener(CC_CALLBACK_2(MJGameBirdieAnswerOverlay::onAnswerTouch, this));
            answer_row->addChild(answer);
            
            _answers.push_back(answer);

            ww += answer->getContentSize().width + margin;
        }

        ww -= margin;
        answer_row->setContentSize(Size(ww, answer_row->getContentSize().height));

        if (answer_row->getChildren().empty()) {
            is_one_row = true;
            answer_row->removeFromParent();
        }
    }

    ui::Helper::doLayout(this);

    if (is_one_row) {
        auto answer_row = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "answers_row_2"));
        answer_row->setPosition(cocos2d::Point(Director::getInstance()->getVisibleSize().width / 2, 90));
//        answer_row->setPositionPercent(Vec2(.5, .5));
    }    

}

void MJGameBirdieAnswerOverlay::onAnswerTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType event_type) {
    auto answer = static_cast<MJGameBirdieAnswer *>(sender);
    switch (event_type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(_delegate){
                _delegate->onAnswerTouchBegan();
            }
            answer->setStartedTouchPosition(answer->getPosition());
            experimental::AudioEngine::play2d(soundBirdieDrag);
            experimental::AudioEngine::play2d(answer->_data.audio_path);
            answer->setLocalZOrder(2);
            answer->getParent()->setLocalZOrder(2);
            break;

        case ui::Widget::TouchEventType::MOVED:
            answer->setPosition(answer->getParent()->convertToNodeSpace(answer->getTouchMovePosition()));
            break;

        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
            answer->setTouchEnabled(false);
            if (_delegate) {
                _delegate->onAnswerDropped(answer, answer->getTouchEndPosition());
                _delegate->onAnswerTouchEnded();
            }
            break;

        default:
            break;
    }
}

Point MJGameBirdieAnswerOverlay::getAnswerPosition(std::string word) {
    auto it = std::find_if(_answers.begin(), _answers.end(), [word](MJGameBirdieAnswer * a) {
        return a->getWord() == word;
    });
    
    if (it == _answers.end()) {
        return Point::ZERO;
    }
    
    auto bb = (*it)->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = (*it)->getParent()->convertToWorldSpace(p);
    return p;
}
