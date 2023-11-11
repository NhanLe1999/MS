//
//  MJGameBirdieTree.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#include "GameBirdieTree.h"
#include "GameBirdieAnswer.h"
#include "GAFWrapper.h"
#include "GameBirdieConstants.h"
#include "GameInstructionOverlay.h"

INITIALIZE_READER(MJGameBirdieTree);
USING_NS_CC;

MJGameBirdieTree * MJGameBirdieTree::createTree(mj::GameObject data) {
    auto tree = static_cast<MJGameBirdieTree *>(CSLoader::createNode("csb/game/birdie/tree.csb"));
    if (tree) {
        tree->didLoadFromCSB(data);
        return tree;
    }

    CC_SAFE_DELETE(tree);
    return nullptr;
}

void MJGameBirdieTree::didLoadFromCSB(mj::GameObject data) {
    _data = data;
    _is_answered = false;

    auto image = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "image"));
    image->loadTexture(_data.image_path);
}

void MJGameBirdieTree::addAnswer(MJGameBirdieAnswer *answer) {
//    auto text_audio_id = experimental::AudioEngine::play2d(_data.audio_path);
//    experimental::AudioEngine::setFinishCallback(text_audio_id, [=](int, std::string) {
        auto bird = ui::Helper::seekWidgetByName(this, "bird");
        auto bird_gaf = GAFWrapper::createGAF("gaf/tree_bird/bird.gaf", bird);
        bird_gaf->play(false);
//    });
    
    this->scheduleOnce([=](float dt) {
//        auto duration = experimental::AudioEngine::getDuration(text_audio_id);
//        this->scheduleOnce([](float dt) {
            experimental::AudioEngine::play2d(soundBirdieRight);
//        }, duration, "delay_bird_effect");
//        MJGameInstructionOverlay::playInstructionAfterDelay(duration, mj::GameType::SING_BIRDIE, soundBirdieRight);
    }, 0.1, "delay_duration");
    _is_answered = true;
    auto timeline = CSLoader::createTimeline("csb/game/birdie/tree.csb");
    timeline->play("right_answer", false);

    this->runAction(timeline);

    auto mouth = ui::Helper::seekWidgetByName(this, "mouth");
    auto destination = mouth->getPosition();

    destination = mouth->getParent()->convertToWorldSpace(destination);
    destination = answer->getParent()->convertToNodeSpace(destination);

    answer->setTouchEnabled(false);
    answer->runAction(EaseElasticOut::create(MoveTo::create(0.75, destination), 0.5));
//    answer->runAction(Sequence::create(EaseQuinticActionOut::create(ScaleTo::create(0.3, 1.5)), EaseQuinticActionOut::create(ScaleTo::create(0.7, 1)), NULL));

    auto eye = ui::Helper::seekWidgetByName(this, "eye");
    auto eye_gaf = GAFWrapper::createGAF("gaf/tree_eye/eyes.gaf", eye);
    eye_gaf->play(true);
}

void MJGameBirdieTree::onWrongAnswer() {
    experimental::AudioEngine::play2d(soundBirdieWrong);
    auto timeline = CSLoader::createTimeline("csb/game/birdie/tree.csb");
    timeline->play("wrong_answer", false);
    this->runAction(timeline);
}
