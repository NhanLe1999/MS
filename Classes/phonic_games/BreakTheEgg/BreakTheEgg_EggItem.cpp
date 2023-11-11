//
//  MJ_BreakTheEgg_EggItem.h
//  Created by HoiSung on 02/02/18.
//
//

#include "BreakTheEgg_EggItem.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
using namespace cocos2d;
BreakTheEgg_EggItem * BreakTheEgg_EggItem::createItem(mj::PHONIC_INFO item_info, int type_egg, bool is_auto_play) {
    BreakTheEgg_EggItem * obj = new BreakTheEgg_EggItem();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info, type_egg, is_auto_play);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void BreakTheEgg_EggItem::didLoadFromCSB(mj::PHONIC_INFO item_info, int type_egg, bool is_auto_play){
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    _item_info = item_info;
    egg_type = type_egg;
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    
    auto gio1 = cocos2d::ui::ImageView::create("games/breaktheegg/breaktheegg_gio1.png");
    this->addChild(gio1,1);
    
    gio2 = cocos2d::ui::ImageView::create("games/breaktheegg/breaktheegg_gio2.png");
    this->addChild(gio2,10);
    gio2->setPosition(Vec2(4.5,-4.5));
    
    egg_layout_root = cocos2d::ui::Layout::create();
    egg_layout_root->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    egg_layout_root->setBackGroundColor(Color3B::WHITE);
    egg_layout_root->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    egg_layout_root->setOpacity(0);
    this->addChild(egg_layout_root, 9);
    egg_layout_root->setContentSize(cocos2d::Size(290,390));
    
    egg_gaf = GAFWrapper::createGAF(StringUtils::format("gaf/breaktheegg/egg%d/egg%d.gaf", type_egg, type_egg));
    egg_layout_root->addChild(egg_gaf, 3);
    egg_gaf->playSequence("idle", true);
    egg_gaf->pauseAnimation();
    
//    egg_star = GAFWrapper::createGAF(StringUtils::format("gaf/breaktheegg/star%d/star%d.gaf", type_egg, type_egg));
//    egg_layout_root->addChild(egg_star, 4);
//    egg_star->playSequence("idle", true);
//    egg_star->pauseAnimation();
    
    if (type_egg == 1) {
        egg_gaf->setPosition(cocos2d::Size(egg_layout_root->getContentSize().width/2,egg_layout_root->getContentSize().height*0.4));
        
    }else if (type_egg == 2){
        egg_gaf->setPosition(cocos2d::Size(egg_layout_root->getContentSize().width/2,egg_layout_root->getContentSize().height*0.35));
    }
    else{
        egg_gaf->setPosition(cocos2d::Size(egg_layout_root->getContentSize().width/2,egg_layout_root->getContentSize().height*0.24));
    }
//    egg_star->setPosition(egg_gaf->getPosition());
    egg_layout_root->setScale(0.5);
    egg_layout_root->setTouchEnabled(!is_auto_play);
    egg_layout_root->addClickEventListener(CC_CALLBACK_1(BreakTheEgg_EggItem::onClickedEgg, this));
    egg_layout_root->setPosition(Vec2(-8, -30));
    int font_size = 250;
    auto font_name = PHONIC_MANAGER->getGameFontName();
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getGameFontName();
    }
    text_full = cocos2d::ui::Text::create(_item_info.text_full, font_name, font_size);
    egg_layout_root->addChild(text_full, 2);
    text_full->setAnchorPoint(Vec2(0.5, 0));
    text_full->setPosition(Vec2(egg_layout_root->getContentSize().width/2, egg_layout_root->getContentSize().height*0.4 - text_full->getContentSize().height/2));
    text_full->setColor(Color3B::BLACK);
    for (int i = 0; i < (int)_item_info.highlights.size(); i++) {
        auto letter = text_full->getLetter(_item_info.highlights[i]);
        if (letter) {
            letter->setColor(Color3B::RED);
        }
    }
    text_full->setVisible(false);
    
    font_size = 200;
    text_phonic = cocos2d::ui::Text::create(_item_info.text_phonic, font_name, font_size);
    while (1){
        if (text_phonic->getContentSize().width > 220){
            font_size-=10;
            text_phonic->setFontSize(font_size);
            if (font_size<=100) break;
        }else{
            break;
        }
    }
    egg_layout_root->addChild(text_phonic, 5);
    text_phonic->setPosition(Vec2(egg_layout_root->getContentSize().width/2, egg_layout_root->getContentSize().height*0.45));
    pos_origin = egg_layout_root->getPosition();
    
}

void BreakTheEgg_EggItem::onEnter(){
    cocos2d::ui::Layout::onEnter();
//    auto ani_phonic_audio =
//    this->
    
}

void BreakTheEgg_EggItem::showEgg(std::function<void()> callback){
    CCLOG("showEgg - %d", egg_type);
    AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_switch.mp3");
    this->onChangeEggState(RUNNING);
    auto pos_goal = this->convertToNodeSpace(Vec2(screen_size.width/2, screen_size.height*0.1));
    auto duration_jump = 0.5f;
    
    egg_layout_root->runAction(Spawn::create(Sequence::create(MoveBy::create(duration_jump*0.3, Vec2(0, 100)), CallFunc::create([=]{
        gio2->setLocalZOrder(9);
        egg_layout_root->setLocalZOrder(10);
    }), JumpTo::create(duration_jump*0.7, pos_goal, 200, 1), CallFunc::create([=]{
        if (callback) {
            callback();
        }
        this->onChangeEggState(SHOWED);
        egg_layout_root->stopAllActions();
        auto duration_scale = 0.2f;
        egg_layout_root->runAction(Repeat::create(Sequence::create(ScaleTo::create(duration_scale, 0.97f), ScaleTo::create(duration_scale, 1.0f), NULL), 100000));
        
    }), NULL), ScaleTo::create(duration_jump, 1.0f), NULL));

//    if (this->getPositionX() == screen_size.width/2){
//        egg_layout_root->runAction(Spawn::create(Sequence::create(MoveBy::create(duration_jump*0.3, Vec2(0, 100)), CallFunc::create([=]{
//            gio2->setLocalZOrder(9);
//            egg_layout_root->setLocalZOrder(10);
//        }), JumpTo::create(duration_jump*0.7, pos_goal, 200, 1), CallFunc::create([=]{
//            egg_gaf->resumeAnimation();
//            this->onChangeEggState(SHOWED);
//        }), NULL), ScaleTo::create(duration_jump, 1.0f), NULL));
//        //
//    }else {
//        egg_layout_root->runAction(Spawn::create(Sequence::create(JumpTo::create(duration_jump, pos_goal, 200, 1), NULL),  ScaleTo::create(duration_jump, 1.0f), NULL));
//    }
}

void BreakTheEgg_EggItem::hideEgg(std::function<void()> callback){
    CCLOG("hideEgg - %d", egg_type);
    egg_layout_root->stopAllActions();
    egg_layout_root->setScale(1.0f);
//    egg_gaf->pauseAnimation();
//    egg_star->pauseAnimation();
//    AUDIO_ENGINE->pauseEffects(break_audio_id);
//    AUDIO_ENGINE->pauseEffects(phonic_audio_id);
//    egg_star->setVisible(false);
    this->onChangeEggState(RUNNING);
    auto pos_goal = pos_origin;
    auto duration_jump = 0.5f;
    egg_layout_root->runAction(Spawn::create(Sequence::create(JumpTo::create(duration_jump*0.7, pos_goal+Vec2(0, 100), 200, 1), CallFunc::create([=]{
        egg_layout_root->setLocalZOrder(9);
        gio2->setLocalZOrder(10);
    }), MoveBy::create(duration_jump*0.3, Vec2(0, -100)), CallFunc::create([=]{
        if (callback) {
            callback();
        }
        this->onChangeEggState(HIDDEN);
    }), NULL), ScaleTo::create(duration_jump*0.7, 0.5f), NULL));
    
//    if (this->getPositionX() == screen_size.width/2){
//        egg_layout_root->runAction(Spawn::create(Sequence::create(JumpTo::create(duration_jump*0.7, pos_goal+Vec2(0, 100), 200, 1), CallFunc::create([=]{
//            egg_layout_root->setLocalZOrder(9);
//            gio2->setLocalZOrder(10);
//        }), MoveBy::create(duration_jump*0.3, Vec2(0, -100)), CallFunc::create([=]{
//            this->onChangeEggState(HIDDEN);
//        }), NULL), ScaleTo::create(duration_jump*0.7, 0.5f), NULL));
//    }else {
//        egg_layout_root->runAction(Spawn::create(Sequence::create(JumpTo::create(duration_jump, pos_goal, 200, 1), NULL),  ScaleTo::create(duration_jump*0.7, 0.5f), NULL));
//    }
}

void BreakTheEgg_EggItem::breakEggAction(){
//    egg_star->setVisible(true);
    egg_layout_root->stopAllActions();
    int number_break = CONFIG_MANAGER->isVietnameseStories()?5:10;
    if (count_break < number_break){
        AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_break.mp3");
        AUDIO_ENGINE->play(_item_info.audio_phonic);
        egg_gaf->playSequence(StringUtils::format("break%d", count_break), false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            if (egg_state != RUNNING && egg_state != BREAKING && egg_state != HIDDEN){
                auto duration_scale = 0.2f;
                egg_layout_root->runAction(Repeat::create(Sequence::create(ScaleTo::create(duration_scale, 0.97f), ScaleTo::create(duration_scale, 1.0f), NULL), 100000));
            }
            
        });
        count_break++;
        is_break_start = true;
        this->onChangeEggState(ACTION);
    }
    else if (count_break == number_break){
        egg_layout_root->setScale(1.0f);
        egg_layout_root->setTouchEnabled(false);
        text_phonic->setVisible(false);
        this->onChangeEggState(BREAKING);
        AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_break.mp3");
        AUDIO_ENGINE->play(_item_info.audio_phonic);
        egg_gaf->playSequence(StringUtils::format("break%d", count_break), false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            text_full->setVisible(true);
            text_full->setScale(0);
            egg_gaf->playSequence("broken", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                egg_gaf->runAction(FadeOut::create(0.25f));
                this->onChangeEggState(BROKEN);
            });
            text_full->runAction(Sequence::create(ScaleTo::create(0.4f, 1.0f), CallFunc::create([=]{
                this->showTextFull();
            }), NULL));
        });
    }
}

void BreakTheEgg_EggItem::breakEggActionJustOne(){
//    egg_star->setVisible(true);
    egg_layout_root->stopAllActions();
    egg_layout_root->setScale(1.0f);
    this->onChangeEggState(ACTION);
    if (is_break_start) {
        egg_gaf->resumeAnimation();
        egg_star->resumeAnimation();
        AUDIO_ENGINE->resumEffects(break_audio_id);
        AUDIO_ENGINE->resumEffects(phonic_audio_id);
    }else {
        break_audio_id = AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_break.mp3", false);
        phonic_audio_id = AUDIO_ENGINE->play(_item_info.audio_phonic, true);
//        egg_star->playSequenceWithPause("action", false);
        egg_gaf->playSequenceWithPause("action", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            AUDIO_ENGINE->stopEffect(break_audio_id);
            AUDIO_ENGINE->stopEffect(phonic_audio_id);
            text_phonic->setVisible(false);
            this->onChangeEggState(BREAKING);
            AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_textshow.mp3");
            text_full->setVisible(true);
            egg_gaf->playSequence("break", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                egg_gaf->runAction(FadeOut::create(0.25f));
                this->onChangeEggState(BROKEN);
                this->showTextFull();
            });
        });
    }
    is_break_start = true;
}

void BreakTheEgg_EggItem::showTextFull(){
    AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_textshow.mp3");
    auto text_star = GAFWrapper::createGAF(StringUtils::format("gaf/breaktheegg/Big_star/Big_star.gaf"));
    egg_layout_root->addChild(text_star, 1);
    text_star->setPosition(Vec2(egg_layout_root->getContentSize().width*0.3, egg_layout_root->getContentSize().height*0.6));
    text_star->play(false, [=] (gaf::GAFObject *obj){
        text_star->removeFromParent();
    });
    
    AUDIO_ENGINE->play(_item_info.audio_full);
    auto duration_scale = 0.25f;
    auto scale_x = 0.8f;
    auto scale_y = 1.25f;
    auto move_y = 15.0f;
    auto action1 = Spawn::create(ScaleTo::create(duration_scale, scale_x, scale_y), MoveBy::create(duration_scale, Vec2(0, move_y)), NULL);
    auto action2 = Spawn::create(ScaleTo::create(duration_scale, 1.0f, 1.0f), MoveBy::create(duration_scale, Vec2(0, -move_y)), NULL);
    auto seq = Sequence::create(action1, action2, NULL);
    text_full->runAction(Sequence::create(Repeat::create(seq, 4), DelayTime::create(0.2f), CallFunc::create([=]{
        this->onChangeEggState(TEXT_SHOWED);
        text_full->runAction(Sequence::create(Spawn::create(MoveBy::create(duration_scale*2, Vec2(0, -70)), ScaleTo::create(duration_scale*2, 0.6f), NULL), CallFunc::create([=]{
            
        }), NULL));
    }), NULL));
//    text_full->runAction(Repeat::create(seq1, 1000));
}

void BreakTheEgg_EggItem::setEggFinish(){
    text_full->runAction(Spawn::create(ScaleTo::create(1.0f, 0.4f), MoveBy::create(1.0f, Vec2(0, 50)), NULL));
    egg_layout_root->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(1.0f, pos_origin+Vec2(0, 50))), DelayTime::create(0.2f), CallFunc::create([=]{
    }), NULL));
}

void BreakTheEgg_EggItem::onClickedEgg(cocos2d::Ref *sender){
    CCLOG("onClickedEgg");
    if (_delegate){
        _delegate->onClickedEggDelegate(this);
    }
}

void BreakTheEgg_EggItem::onChangeEggState(int state){
    egg_state = state;
    if (_delegate){
        _delegate->onChangeEggStateDelegate(state);
    }
}

void BreakTheEgg_EggItem::setDelegate(BreakTheEgg_EggItemDelegate *delegate){
    _delegate = delegate;
}
