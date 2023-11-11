
#include "GameWordTrain_Item.h"
#include "HSAudioEngine.h"
#include "NativeBridge.h"
#include "APProfileManager.h"
#include "MJDefinitions.h"
using namespace cocos2d;
INITIALIZE_READER(MJGameWordTrain_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/word_train/GameWordTrain_Item.csb"

MJGameWordTrain_Item * MJGameWordTrain_Item::createGame(ItemInfo item_info, ItemDistracting item_distracting) {
    auto p = static_cast<MJGameWordTrain_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, item_distracting);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameWordTrain_Item::didLoadFromCSB(ItemInfo item_info, ItemDistracting item_distracting) {
    
    

    _item_info = item_info;
//    _item_info.character = std::string("i");
//    _item_info.text = std::string("girl");
    _item_distracting = item_distracting;
    text_full = utils::findChild<ui::Text *>(this, "text");
    text_full->setString(_item_info.text);
    auto size_text = 130;
    if (CONFIG_MANAGER->isGameVMonkey()){
        text_full->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
        utils::findChild<ui::Text *>(this, "text_character1")->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
        utils::findChild<ui::Text *>(this, "text_character2")->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
    }
    text_full->setFontSize(size_text);
   
    layout_text = this->getChildByName<ui::Layout *>("layout_text");
    layout_text->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
    
    list_layout_charaters.push_back(utils::findChild<ui::Layout *>(this, "layout_character1"));
    list_layout_charaters.push_back(utils::findChild<ui::Layout *>(this, "layout_character2"));
    index_charater_correct = random(0,1);
    if (index_charater_correct == 0){
        utils::findChild<ui::Text *>(this, "text_character1")->setString(_item_info.character);
        list_audio_path.push_back(_item_info.character_audio_path);
        
        utils::findChild<ui::Text *>(this, "text_character2")->setString(_item_distracting.text);
        list_audio_path.push_back(_item_distracting.audio_path);
    }else{
        utils::findChild<ui::Text *>(this, "text_character1")->setString(_item_distracting.text);
        list_audio_path.push_back(_item_distracting.audio_path);
        
        utils::findChild<ui::Text *>(this, "text_character2")->setString(_item_info.character);
        list_audio_path.push_back(_item_info.character_audio_path);
    }
    utils::findChild<ui::Text *>(this, "text_character1")->setFontSize(size_text);
    utils::findChild<ui::Text *>(this, "text_character2")->setFontSize(size_text);
    list_layout_charaters[0]->setOpacity(0);
    list_layout_charaters[1]->setOpacity(0);
    text_full->setOpacity(0);
    
}

void MJGameWordTrain_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(MJGameWordTrain_Item::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(MJGameWordTrain_Item::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(MJGameWordTrain_Item::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(MJGameWordTrain_Item::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    auto list_pos = NativeBridge::findStringOnString(_item_info.text, _item_info.character, false);
    _item_info.start = list_pos[0];
    _item_info.end = _item_info.start + NativeBridge::utf8LengthOfString(_item_info.character);
    std::vector<Sprite *> _letters;
    for(int j = _item_info.start; j < _item_info.end ; j++) {
        auto letter = text_full->getLetter(j);
        if(letter != NULL) {
            letter->setOpacity(0);
            _letters.push_back(letter);
        }
        letter = nullptr;
    }
    
    rect = getRectOfLetters(_letters, _item_info.text);
    
    auto gach_ngang = ui::Layout::create();
    gach_ngang->setBackGroundColor(Color3B::BLACK);
    gach_ngang->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    gach_ngang->setContentSize(cocos2d::Size(rect.size.width, 8));
    gach_ngang->setAnchorPoint(Vec2(0, 0));
    gach_ngang->setPosition(Vec2(rect.origin.x, 37.0));
    gach_ngang->setOpacity(255);
    gach_ngang->setName("gach_ngang");
    text_full->addChild(gach_ngang);
    
    CCLOG("rect.origin.y = %f", rect.origin.y);
    
}

void MJGameWordTrain_Item::onShow(std::function<void()> finishCallback /*= nullptr*/ ){
    AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_showup.mp3");
    for (int i = 0; i < (int)list_layout_charaters.size(); i++) {
        list_layout_charaters[i]->runAction(Sequence::create(FadeIn::create(0.25f), CallFunc::create([=]{
            if (i == 1) {
                text_full->runAction(Sequence::create(FadeIn::create(0.25f), CallFunc::create([=]{
                    is_answer_done = false;
                    if (finishCallback) {
                        finishCallback();
                    }
                }),NULL));
            }
        }), NULL));
    }
}

bool MJGameWordTrain_Item::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    cocos2d::Point location = this->convertToNodeSpace(touch->getLocation());
    if(delegate){
        delegate->onTouchItemBegan();
    }
    CCLOG("touch");
    if (is_choose || is_answer_done) return true;
    for (int i = 0; i < (int) list_layout_charaters.size(); i++) {
        if (list_layout_charaters[i]->getBoundingBox().containsPoint(location)) {
            is_choose = true;
            layout_choose = list_layout_charaters[i];
            index_choose = i;
            old_pos_layout = layout_choose->getPosition();
            AUDIO_ENGINE->playSound(list_audio_path[i]);
            AUDIO_ENGINE->playSound("sounds/word_train/wordtrain_drag.mp3");
          
        }
    }
    return true;
}
void MJGameWordTrain_Item::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    if (is_choose && layout_choose && !is_answer_done) {
        cocos2d::Point location = this->convertToNodeSpace(touch->getLocation());
        layout_choose->setPosition(location);
    }
}
void MJGameWordTrain_Item::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (layout_choose && !is_answer_done) {
        layout_choose->setLocalZOrder(100);
        is_answer_done = true;

        // only touch for children under 3 year old 
        auto age = ap::ProfileManager::getInstance().getInformationOfProfile().age;
        if (age < CHILDREN_AGED && index_choose == index_charater_correct) {
            cocos2d::Point location = this->convertToNodeSpace(getGuideStoppingPoint());
            layout_choose->setPosition(location+ Point(0,42));
        }
        if (layout_text->getBoundingBox().intersectsRect(layout_choose->getBoundingBox()) && index_choose == index_charater_correct) {
            AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_correct.mp3");
            list_layout_charaters[1-index_choose]->runAction(Sequence::create(FadeOut::create(0.25f), NULL));
            if (delegate) {
                delegate->onAnswerTrue();
            }
            auto pos_goal = Vec2(rect.origin.x + rect.size.width/2, 0);
            pos_goal = layout_text->convertToNodeSpace(text_full->convertToWorldSpace(pos_goal));
            pos_goal.y = text_full->getPositionY();
            layout_choose->runAction(Sequence::create(MoveTo::create(0.25f, pos_goal), CallFunc::create([=]{
                auto gach_ngang = text_full->getChildByName("gach_ngang");
                if (gach_ngang) {
                    gach_ngang->runAction(Sequence::create(FadeOut::create(0.5f), DelayTime::create(0.5f), CallFunc::create([=]{
                        bool index_wrong = !index_choose;
                        list_layout_charaters[int(index_wrong)]->runAction(MoveBy::create(0.2f, Vec2(0, -20)));
                        layout_text->runAction(MoveBy::create(0.2f, Vec2(0, 20)));
                        text_full->setTextColor(Color4B::RED);
                        
                        auto audio_path = std::string(_item_info.audio_path);
                        auto audio_index = cocos2d::experimental::AudioEngine::play2d(audio_path);
                        cocos2d::experimental::AudioEngine::setFinishCallback(audio_index, [=](int au_id, std::string path){
                            list_layout_charaters[int(index_wrong)]->runAction(MoveBy::create(0.2f, Vec2(0, 20)));
                            layout_text->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, -20)), CallFunc::create([=]{
                                text_full->setTextColor(Color4B::BLACK);
                            }), CallFunc::create([=]{
                                if (delegate) {
                                    delegate->onFinishTurn();
                                }
                                layout_text->runAction(FadeOut::create(0.5f));
                            }), NULL));
                        });
                        gach_ngang->removeFromParent();
                    }), NULL));
                }
                
            }), NULL));
            
        }else {
            layout_choose->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5, old_pos_layout)), CallFunc::create([=]{
                if (index_choose != index_charater_correct) {
                    AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_wrong.mp3");
                }
                if (delegate) {
                    delegate->onTouchItemEnded();
                }
                is_choose = false;
                layout_choose = nullptr;
                is_answer_done = false;
            }), NULL));
        }
        
    }else
        is_choose = false;
}

cocos2d::Rect MJGameWordTrain_Item::getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find){
    auto minX = 0, maxX = 0, minY = 0, maxY = 0;
    minX = _letter_arr[0]->getPositionX() - _letter_arr[0]->getContentSize().width/2;
    minY = _letter_arr[0]->getPositionY() - _letter_arr[0]->getContentSize().height/2;
    
    maxX = _letter_arr[0]->getPositionX() + _letter_arr[0]->getContentSize().width/2;
    maxY = _letter_arr[0]->getPositionY() + _letter_arr[0]->getContentSize().height/2;
    
    
    for (int i = 1; i < (int)_letter_arr.size(); i++) {
        if (NativeBridge::hsStringSub(str_find, i, 1) == std::string(" ")) {
            continue;
        }
        auto _posX_min = _letter_arr[i]->getPositionX() - _letter_arr[i]->getContentSize().width/2;
        if (minX > _posX_min) {
            minX = _posX_min;
            //            CCLOG("letter = %d | posletter = %f | sizeletter = %f", i, _letter_arr[i]->getPositionX(), _letter_arr[i]->getContentSize().width);
        }
        
        auto _posX_max = _letter_arr[i]->getPositionX() + _letter_arr[i]->getContentSize().width/2;
        
        if (maxX < _posX_max) {
            maxX = _posX_max;
        }
        
        auto _posY_min = _letter_arr[i]->getPositionY() - _letter_arr[i]->getContentSize().height/2;
        if (minY > _posY_min) {
            minY = _posY_min;
        }
        auto _posY_max = _letter_arr[i]->getPositionY() + _letter_arr[i]->getContentSize().height/2;
        if (maxY < _posY_max) {
            maxY = _posY_max;
        }
        
        //        CCLOG("content = %f", _letter_arr[i]->getContentSize().height);
    }
    _letter_arr.clear();
    auto rect = Rect(minX, minY, maxX - minX, maxY - minY);
    return rect;
}

void MJGameWordTrain_Item::setDelegate(MJGameWordTrain_ItemDelegate *_delegate){
    this->delegate = _delegate;
}

void MJGameWordTrain_Item::onChoose(cocos2d::Ref *sender){
}

ui::Widget::ccWidgetClickCallback MJGameWordTrain_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onChoose") {
        return CC_CALLBACK_1(MJGameWordTrain_Item::onChoose, this);
    }
    return nullptr;
}

Point MJGameWordTrain_Item::getGuideStartingPoint() {
    auto p = list_layout_charaters[index_charater_correct]->getPosition();
    p = list_layout_charaters[index_charater_correct]->getParent()->convertToWorldSpace(p);
    return p;
}

Point MJGameWordTrain_Item::getGuideStoppingPoint() {
    auto hyphen = text_full->getChildByName("gach_ngang");
    if (!hyphen) {
        return Point::ZERO;
    }
    auto bb = hyphen->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = hyphen->getParent()->convertToWorldSpace(p);
    return p;
}
