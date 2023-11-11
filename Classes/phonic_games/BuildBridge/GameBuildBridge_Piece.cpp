//
//  GameBuildBridge_Piece.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/5/18.
//

#include "GameBuildBridge_Piece.h"
#include "GAFWrapper.h"

USING_NS_CC;

GameBuildBridge_Piece * GameBuildBridge_Piece::createPiece(int idx) {
    auto p = new (std::nothrow) GameBuildBridge_Piece();
    if (p && p->initPiece(idx)) {
        p->autorelease();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GameBuildBridge_Piece::initPiece(int idx) {
    if (!ui::Layout::init()) {
        return false;
    }
    
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setContentSize(Size(100, 100));
    this->setScale(0.9);

    _gaf= GAFWrapper::createGAF(StringUtils::format("gaf/build_bridge/b%d/b%d.gaf", idx, idx));
    _gaf->playSequence("idle", true);
    _gaf->setPosition(Point(50, -310));
    this->addChild(_gaf, 1);
    
//    auto position = wrapper->convertToWorldSpace(Point(50, -310));
//    auto translated = Point(512, -125) - position;
    
    return true;
}

void GameBuildBridge_Piece::onTapRight(std::function<void()> callback) {

	auto screen_size = cocos2d::Director::getInstance()->getWinSize();
    auto position = Point(50, -310);
    auto translated = this->convertToNodeSpace(Point(screen_size.width/2, -85)) - position;
    
    _gaf->playSequence("tap", false, [translated,callback, this](gaf::GAFObject *, std::string) {
        _gaf->setFps(20);
        _gaf->runAction(MoveBy::create(6.f / 20, translated));
        _gaf->playSequence("correct", false, [this,callback](gaf::GAFObject *, std::string) {
            _gaf->setFps(32);
            auto pos = _gaf->getParent()->convertToWorldSpace(_gaf->getPosition());
            CCLOG("position: %f, %f", pos.x, pos.y);
            pos = _gaf->getParent()->getPosition();
            CCLOG("position: %f, %f", pos.x, pos.y);
			if (callback) {
				callback();
			}
            _gaf->playSequence("post_correct", false, [this](gaf::GAFObject *, std::string) {
                _gaf->playSequence("built_idle");
            });
        });
    });
}

void GameBuildBridge_Piece::onTapWrong() {
    _gaf->playSequence("tap", false, [this](gaf::GAFObject *, std::string) {
        _gaf->playSequence("wrong", false, [this](gaf::GAFObject *, std::string) {
            _gaf->playSequence("fade_in", false);
        });
    });
}

void GameBuildBridge_Piece::show() {
    _gaf->playSequence("fade_in", false, [](gaf::GAFObject * g, std::string) {
        g->playSequence("idle", true);
    });
}
