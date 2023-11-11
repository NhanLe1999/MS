//
//  GameLazerFrameBottom.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/23/17.
//
//

#include "GameLazerFrameBottom.h"
#include "GAFWrapper.h"
#include "audio/include/AudioEngine.h"
#include "HSAudioEngine.h"
#include "MJDefault.h"
#include "GameLazer.h"
INITIALIZE_READER(MJGameLazerFrameBottom);

USING_NS_CC;

MJGameLazerFrameBottom * MJGameLazerFrameBottom::createFrame(mj::GameObject object, bool is_image, bool isTextLength) {
    auto p = static_cast<MJGameLazerFrameBottom *>(CSLoader::createNode("csb/game/lazer/end_point.csb"));
    if (p) {
        p->didLoadFromCSB(object, is_image, isTextLength);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameLazerFrameBottom::didLoadFromCSB(mj::GameObject object, bool is_image, bool isTextLength) {
    _object = object;

    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    auto image = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "image"));

    if (isTextLength)
    {
        text->setFontSize(text->getFontSize() - 10);
    }

    text->setString(object.text);
    image->loadTexture(object.image_path);

    text->setVisible(!is_image);
    image->setVisible(is_image);
    image->setContentSize(cocos2d::Size(190, 145));
    
}

std::string MJGameLazerFrameBottom::getName() {
    return _object.text;
}

void MJGameLazerFrameBottom::addConnectedPlug(Point position) {
    this->getChildByName("light_off")->setVisible(false);
    this->getChildByName("light_on")->setVisible(true);

    position = this->convertToNodeSpace(position);
    auto connected_plug = ui::ImageView::create("games/lazer/plug_out.png");
    connected_plug->setAnchorPoint(Point::ANCHOR_MIDDLE);
    connected_plug->setPosition(position);

    this->addChild(connected_plug);

    connected_plug->runAction(ScaleTo::create(0.5, 1, 1.3));
    connected_plug->runAction(Sequence::create(
            EaseCircleActionIn::create(MoveTo::create(0.5, Point(92.5, 330))),
            CallFunc::create([=] {
                connected_plug->loadTexture("games/lazer/plug_out_none.png");
                auto electric = GAFWrapper::createGAF("gaf/electric/electric.gaf");
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


void MJGameLazerFrameBottom::turnBulbOn() {
    this->getChildByName("light_off")->setVisible(false);
    this->getChildByName("light_on")->setVisible(true);
}

void MJGameLazerFrameBottom::onBgFrame(cocos2d::Ref *sender){
    if (MJDEFAULT->getBoolForKey("key_play_audio_gameLazer", true))
    {
        if (auto gameLazer = dynamic_cast<MJGameLazer*>(this->getParent()->getParent()->getParent()->getParent()))
        {
            if (auto hand = utils::findChild(this, "hand_suggetion"))
            {
                hand->removeFromParent();
            }
            gameLazer->unschedule("show_guide_bulbs_1");
            gameLazer->stopSoundAudioSuggetion();
        }
        MJDEFAULT->setBoolForKey("key_play_audio_gameLazer", false);
        AUDIO_ENGINE->play(_object.audio_path, false, 1.0f, [this]() {
            if (auto gameLazer = dynamic_cast<MJGameLazer*>(this->getParent()->getParent()->getParent()->getParent()))
            {
                gameLazer->showGuide(8.5f);
            }
            MJDEFAULT->setBoolForKey("key_play_audio_gameLazer", true);
            });
    }
    
}
#pragma mark - Callback Functions
cocos2d::ui::Widget::ccWidgetClickCallback MJGameLazerFrameBottom::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameLazerFrameBottom, onBgFrame),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
