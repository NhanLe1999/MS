//
//  GameLazerFrameBottom.cpp
//
//  Created by To Ngoc Linh on 6/23/17.
//  Cloned by MinhTzy on 29/05/2019
//

#include "GameNoiFrameBottom.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
INITIALIZE_READER(GameNoiFrameBottom);

USING_NS_CC;

GameNoiFrameBottom * GameNoiFrameBottom::createFrame(mj::GameObject object, bool is_image) {
    auto p = static_cast<GameNoiFrameBottom *>(CSLoader::createNode("csb/game/vm_game_noi/end_point.csb"));
    if (p) {
        p->didLoadFromCSB(object, is_image);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameNoiFrameBottom::didLoadFromCSB(mj::GameObject object, bool is_image) {
    _object = object;

    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    auto image = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "image"));

    text->setString(object.text);
    image->loadTexture(object.image_path);

    text->setVisible(!is_image);
    image->setVisible(is_image);
    
}

std::string GameNoiFrameBottom::getName() {
    return _object.text;
}

void GameNoiFrameBottom::addConnectedPlug(Point position) {
	ui::Helper::seekWidgetByName(this, "correct")->setVisible(true);

    position = this->convertToNodeSpace(position);
    auto connected_plug = ui::ImageView::create("games/vm_game_noi/plug_out.png");
    connected_plug->setAnchorPoint(Point::ANCHOR_MIDDLE);
    connected_plug->setPosition(position);

    this->addChild(connected_plug);

    connected_plug->runAction(ScaleTo::create(0.5, 1, 1.3));
    connected_plug->runAction(Sequence::create(
            EaseCircleActionIn::create(MoveTo::create(0.5, Point(92.5, 245))),
            CallFunc::create([=] {
                connected_plug->loadTexture("games/vm_game_noi/plug_out_none.png");
                auto electric = GAFWrapper::createGAF("gaf/vm_game_noi/electric/electric.gaf");
                electric->setPosition(Point(92.5, 330));
                electric->play(false, [](gaf::GAFObject * object) {
                    object->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([=]{
                        object->removeFromParent();
                    }), NULL));
                });
                this->addChild(electric);
            }),
            EaseElasticOut::create(ScaleTo::create(1, 1), 0.5),
            nullptr));
}


void GameNoiFrameBottom::turnBulbOn() {
	ui::Helper::seekWidgetByName(this, "correct")->setVisible(true);
}

void GameNoiFrameBottom::onBgFrame(cocos2d::Ref *sender){
    cocos2d::experimental::AudioEngine::play2d(_object.audio_path);
}
#pragma mark - Callback Functions
cocos2d::ui::Widget::ccWidgetClickCallback GameNoiFrameBottom::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(GameNoiFrameBottom, onBgFrame),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
