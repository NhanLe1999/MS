//
//  GameBirdieQuestion.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#include "GameBirdieQuestion.h"
#include "GameBirdieTree.h"
#include "GameBirdieAnswer.h"
#include <numeric>
#include "GameBirdie.h"
INITIALIZE_READER(MJGameBirdieQuestion);

USING_NS_CC;

MJGameBirdieQuestion * MJGameBirdieQuestion::createQuestion(mj::BirdieDataSet data, bool is_enable /*= true*/,MJGameBirdieQuestion_ItemDelegate* dele_ /*= nullptr*/) {
    auto question = static_cast<MJGameBirdieQuestion *>(CSLoader::createNodeWithVisibleSize("csb/game/birdie/game_birdie_one_scene.csb"));
    if (question) {
		question->didLoadFromCSB(data, is_enable, dele_);
        return question;
    }

    CC_SAFE_DELETE(question);
    return nullptr;
}

void MJGameBirdieQuestion::didLoadFromCSB(mj::BirdieDataSet data, bool is_enable/* = true*/, MJGameBirdieQuestion_ItemDelegate* dele_ /*= nullptr*/) {
    _data = data;
	game_play_enable = is_enable;
	setDelegate(dele_);
    auto ww = 0.f;
    auto margin = 20.f;
    auto trees = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "tree"));
    trees->setTouchEnabled(true);
    for (auto picture : _data.pictures) {
        auto tree = MJGameBirdieTree::createTree(picture);
        tree->setPosition(Point(ww, 20));
        trees->addChild(tree);
        ww += tree->getContentSize().width + margin;
        _trees.push_back(tree);
    }

    ww -= margin;
    trees->setContentSize(Size(ww, trees->getContentSize().height));
    ui::Helper::doLayout(this);
}

void MJGameBirdieQuestion::onAnswerDropped(MJGameBirdieAnswer *answer, cocos2d::Point position) {
    for (auto tree : _trees) {
        auto bb = tree->getBoundingBox();
        bb.origin = tree->getParent()->convertToWorldSpace(bb.origin);

        if (bb.containsPoint(position)) {
            if (tree->getWord() == answer->getWord()) {
                GAMEBIRDIE_INSTANCE->setHandShowed(true);
                //remove schedule from parent, dirty work
                Director::getInstance()->getScheduler()->unschedule("show_guide_birdie", this->getParent()->getParent()->getParent());
                Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

                answer->setLocalZOrder(0);
                answer->getParent()->setLocalZOrder(0);
                tree->addAnswer(answer);
                auto is_all_answered = std::accumulate(_trees.begin(), _trees.end(), true, [](bool r, MJGameBirdieTree * tree) {
                    return r && tree->getAnswered();
                });

                if (is_all_answered) {
                    this->getEventDispatcher()->dispatchCustomEvent(kEventBirdieOneSetDone);
                }
				if (!game_play_enable) {
					_delegate->onPauseQuestion();
				}
                return;
            } else {
                tree->onWrongAnswer();
                answer->moveBackToOrigin();
                return;
            }
        }
    }
    answer->moveBackToOrigin();

}

Point MJGameBirdieQuestion::getFirstTreePosition() {
    if (_trees.empty()) {
        return Point::ZERO;
    }
    
    auto bb = _trees.front()->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = _trees.front()->getParent()->convertToWorldSpace(p);
    return p;
}

Point MJGameBirdieQuestion::getSecondTreePosition() {
    if (_trees.empty() || _trees.size()<2) {
        return Point::ZERO;
    }
    auto bb = _trees.at(1)->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = _trees.front()->getParent()->convertToWorldSpace(p);
    return p;
}

std::string MJGameBirdieQuestion::getFirstTreeWord() {
    if (_trees.empty()) {
        return "";
    }
    
    return _trees.front()->getWord();
}

MJGameBirdieTree* MJGameBirdieQuestion::getUnanswerTree(bool getSecondTree){
    for(auto tree: _trees){
        if(!tree->getAnswered()){
            if (getSecondTree) {
                getSecondTree = false;
                continue;
            }
            else {
                getSecondTree = false;
                return tree;
            }
        }
    }
    return nullptr;
}

Point MJGameBirdieQuestion::getTreePosition(MJGameBirdieTree* tree) {
    if (!tree) {
        return Point::ZERO;
    }
    
    auto bb = tree->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = tree->getParent()->convertToWorldSpace(p);
    return p;
}

std::string MJGameBirdieQuestion::getTreeWord(MJGameBirdieTree* tree) {
    if (!tree) {
        return "";
    }
    
    return tree->getWord();
}

void MJGameBirdieQuestion::onAnswerTouchBegan() {
    _delegate->onAnswerBegan();
}

void MJGameBirdieQuestion::onAnswerTouchEnded() {
    _delegate->onAnswerEnded();
}
