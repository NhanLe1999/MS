
#include "GameBuildWall_Item.h"
#include "HSAudioEngine.h"
#include "NativeBridge.h"
#include "MsHelperGame.h"
INITIALIZE_READER(MJGameBuildWall_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/build_wall/GameBuildWall_Item.csb"

MJGameBuildWall_Item * MJGameBuildWall_Item::createGame(ItemInfo item_info, bool is_crocodile_flip) {
    auto p = static_cast<MJGameBuildWall_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, is_crocodile_flip);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameBuildWall_Item::didLoadFromCSB(ItemInfo item_info, bool is_crocodile_flip) {
    _item_info = item_info;
    crocodile_flip = is_crocodile_flip;
    
    auto h_item = 108;
    this->setContentSize(cocos2d::Size(Director::getInstance()->getWinSize().width, h_item));
    ui::Helper::doLayout(this);
    _isTouchable = false;
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(MJGameBuildWall_Item::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(MJGameBuildWall_Item::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(MJGameBuildWall_Item::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(MJGameBuildWall_Item::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    size_text = 45;
    auto font_text = std::string("fonts/monkeystories_ultra.ttf");
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_text = PHONIC_MANAGER->getGameFontName();
    }
    text_full = cocos2d::ui::Text::create(_item_info.text_full, font_text, size_text);
    this->addChild(text_full, 10);
    text_full->setName(_item_info.text_full);
    text_full->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2+10));
    text_full->setTextColor(Color4B::YELLOW);
    if (text_full->getContentSize().width > Director::getInstance()->getWinSize().width-40) {
        text_full->setFontSize(40);
        size_text = 40;
        if (text_full->getContentSize().width > Director::getInstance()->getWinSize().width-40) {
            text_full->setFontSize(35);
            size_text = 35;
			if (text_full->getContentSize().width > Director::getInstance()->getWinSize().width - 40) {
				float width_text = text_full->getContentSize().width;
				float width_director = Director::getInstance()->getWinSize().width;
				while (text_full->getContentSize().width > Director::getInstance()->getWinSize().width - 40) {
					size_text -= 1;
					text_full->setFontSize(size_text);
				}
			}
        }
    }
//    auto text_full_test = std::string("abc 123 abc45 abc456.");
//    auto text_find = std::string("abc456");
//    auto list_find = NativeBridge::findStringOnString(NativeBridge::string_towlower(_item_info.text_full), NativeBridge::string_towlower(_item_info.text_true), true);
//    CCLOG("list_find = %d", (int)list_find.size());
    
    auto arr_index = NativeBridge::findStringOnString(NativeBridge::string_towlower(_item_info.text_full), NativeBridge::string_towlower(_item_info.text_true), true);
    if (arr_index.empty()){
        CCLOG("Không tìm thấy từ trong câu");
        return;
    }
    auto _start = arr_index[0];
    auto _end = _start + NativeBridge::utf8LengthOfString(_item_info.text_true);
    std::vector<Sprite *> _letters;
    for(int j = _start; j < _end ; j++) {
        auto letter = text_full->getLetter(j);
        if(letter != NULL) {
            letter->setOpacity(0);
            _letters.push_back(letter);
        }
        letter = nullptr;
    }
    rect = getRectOfLetters(_letters, _item_info.text_true);
    rect = Rect(rect.origin.x, rect.origin.y, rect.size.width+15, rect.size.height);    
    
    _layout_compare = Layout::create();
    _layout_compare->setBackGroundColor(Color3B::WHITE);
    _layout_compare->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    auto width_add = 400;
    _layout_compare->setContentSize(cocos2d::Size(rect.size.width + width_add,h_item));
    _layout_compare->setAnchorPoint(Vec2(0, 0));
    pos_layout = text_full->convertToWorldSpace(Vec2(rect.origin.x, 0));
    rect_convert = Rect(pos_layout.x, pos_layout.y, rect.size.width, rect.size.height);
    pos_layout.y = 0;
    pos_layout.x -= 7.5;
    _layout_compare->setPosition(Vec2(pos_layout.x - width_add/2, 0));
    this->addChild(_layout_compare, 9);
    _layout_compare->setOpacity(0);
    pos_goal = pos_layout + rect.size/2;
    
    auto img_left = cocos2d::ui::Scale9Sprite::create("games/build_wall/buildwall_gach.png");
    img_left->setPreferredSize(cocos2d::Size(pos_layout.x+2,h_item));
    img_left->setName("build_wall_left");
    img_left->setAnchorPoint(Vec2(0,0));
    this->addChild(img_left,9);
    
    auto img_right = cocos2d::ui::Scale9Sprite::create("games/build_wall/buildwall_gach.png");
    img_right->setPreferredSize(cocos2d::Size(this->getContentSize().width - (pos_layout.x + rect.size.width)+5,h_item));
    img_right->setAnchorPoint(Vec2(0,0));
    img_right->setPosition(Vec2(pos_layout.x + rect.size.width -2, 0));
    this->addChild(img_right,9);
}

void MJGameBuildWall_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameBuildWall_Item::onShow(bool isFlip){

    {
        using namespace rapidjson;
        auto jsonString = FileUtils::getInstance()->getStringFromFile("config.json");
        Document resultDocument;
        if (resultDocument.Parse(jsonString.c_str()).HasParseError())
            return;
        play_image = JSON_SAFE_CHECK(resultDocument, "play_image", Bool) ? resultDocument["play_image"].GetBool() : false;
    }

    crocodile_flip = isFlip;
    auto h_item = 108;
    auto size_screen_width = Director::getInstance()->getWinSize().width;
    auto font_text = std::string("fonts/monkeystories_ultra.ttf");
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_text = PHONIC_MANAGER->getGameFontName();
    }
    std::vector<Vec2> list_pos_answer;
    auto pos_item = pos_layout;
    list_pos_answer.push_back(Vec2(size_screen_width/2 + 55 - rect.size.width/2, 350));
    list_pos_answer.push_back(Vec2(size_screen_width/2 + 115 + rect.size.width/2, 350));
    
    mj::helper::shuffle(list_pos_answer);
    
    std::vector<std::string> list_str_answer = {_item_info.text_true, _item_info.text_false};
    
    for (int i = 0; i < 2; i++) {
        auto item_answer = cocos2d::ui::Scale9Sprite::create("games/build_wall/buildwall_gach.png");
        this->addChild(item_answer, 100);
        item_answer->setPreferredSize(cocos2d::Size(rect.size.width, h_item));
        if (crocodile_flip) {
            item_answer->setPosition(Vec2(list_pos_answer[i].x + pos_item.x + rect.size.width*2 + size_screen_width/2, list_pos_answer[i].y));
        }else {
            item_answer->setPosition(Vec2(list_pos_answer[i].x - pos_item.x - rect.size.width*2 - size_screen_width/2, list_pos_answer[i].y));
        }
        
        auto text = cocos2d::ui::Text::create(list_str_answer[i], font_text, size_text);
        text->setName(list_str_answer[i]);
        item_answer->addChild(text);
        text->setTextColor(Color4B::YELLOW);
        
        auto w_item = item_answer->getContentSize().width;
        auto value_compare = 10;
        if (text->getContentSize().width > w_item - value_compare) {
            item_answer->setPreferredSize(cocos2d::Size(text->getContentSize().width+value_compare, h_item));
            w_item = text->getContentSize().width+value_compare - w_item;
            if (i == 1) {
                if (list_pos_answer[1].x > list_pos_answer[0].x) {
                    list_pos_answer[i].x = list_pos_answer[i].x + w_item/2;
                }else
                    list_pos_answer[i].x = list_pos_answer[i].x - w_item/2;
            }
        }

        text->setPosition(Vec2(item_answer->getContentSize().width/2, item_answer->getContentSize().height/2+value_compare));
        _isPlayReadText = false;

        if (play_image)
        {
            item_answer->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                if (i == 1) {
                    readText(5.0);
                }
                }), DelayTime::create(5.0f), EaseBackOut::create(MoveTo::create(0.5f, list_pos_answer[i])), NULL));
        }
        else
        {
            item_answer->runAction(Sequence::create(DelayTime::create(1.0f), EaseBackOut::create(MoveTo::create(0.5f, list_pos_answer[i])), CallFunc::create([=] {
                if (i == 1) {
                    readText(0.5f);
                }
                }), NULL));
        }
        
        item_answer->setCascadeOpacityEnabled(true);
        list_answer.push_back(item_answer);
        
    }
    
}

void MJGameBuildWall_Item::readText(float delayTime){
    this->runAction(Sequence::create(
        CallFunc::create([=] {
            auto audio_path = std::string(_item_info.audio_path_full);
            audio_index = cocos2d::experimental::AudioEngine::play2d(audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_index, [=](int au_id, std::string path) {
                _isPlayReadText = true;
                _isTouchable = true;
                });
            }),
        DelayTime::create(1.5),
        CallFunc::create([=] {
                is_answer_done = false;
                runActionHand(5.0);
            }),
        NULL));
    

}

bool MJGameBuildWall_Item::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (!_isTouchable)
    {
        return true;
    }
    if (auto hand = utils::findChild(this, "hand_suggetion"))
    {
        hand->removeFromParent();
    }
    this->unschedule("show_guide_brick_layers");
    _isPlayAudio = true;
    AUDIO_ENGINE->stopAllAudio();
    AUDIO_ENGINE->stopAllSound();
    AUDIO_ENGINE->stopAllEffects();
    if (!is_choose && !is_answer_done) {
        cocos2d::Point location = this->convertToNodeSpace(touch->getLocation());
        is_choose = true;
        for (int i = 0; i < (int)list_answer.size(); i++) {
            if (list_answer[i]->getBoundingBox().containsPoint(location)) {
                is_choose = true;
                img_choose = list_answer[i];
                answer_old_pos = img_choose->getPosition();
                list_answer[i]->setLocalZOrder(101);
                index_answer = i;
                AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_touch.mp3");
                if (i == 0) {
                    AUDIO_ENGINE->playEffect(_item_info.audio_path_true);
                }
				else 
				{
				                    AUDIO_ENGINE->playEffect(_item_info.audio_path_false);
				} 
                break;
            }
        }
    }
    return true;
}
void MJGameBuildWall_Item::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    if (!_isTouchable)
    {
        return;
    }
    if (is_choose && img_choose && !is_answer_done) {
        cocos2d::Point location = this->convertToNodeSpace(touch->getLocation());
        img_choose->setPosition(location);
    }
}
void MJGameBuildWall_Item::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (!_isTouchable)
    {
        return;
    }
    if (img_choose && !is_answer_done) {
        img_choose->setLocalZOrder(100);
        is_answer_done = true;
        if (_layout_compare->getBoundingBox().intersectsRect(img_choose->getBoundingBox()) && index_answer == 0) {
            if (auto hand = utils::findChild(this, "hand_suggetion"))
            {
                hand->removeFromParent();
            }
            this->unschedule("show_guide_brick_layers");
            _isPlayAudio = true;
//            this->getEventDispatcher()->setEnabled(false);
            AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_correct.mp3");
            AUDIO_ENGINE->stopEffect(audio_index);
            img_choose->runAction(Sequence::create(MoveTo::create(0.05f, Vec2(pos_layout.x+img_choose->getContentSize().width/2, 108/2)),DelayTime::create(0.15f), MoveBy::create(0.2f, Vec2(0, 30)), CallFunc::create([=]{
                if (delegate) {
                    delegate->onChooseAnswerFinish();
                }
				img_choose->setLocalZOrder(9);
                list_answer[1]->runAction(FadeOut::create(0.25f));
            }), NULL));
        }else{
            AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_wrong.mp3");
            img_choose->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, answer_old_pos)), CallFunc::create([=]{
                if (index_answer == 1) {
                    AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_opps.mp3");
                    if (delegate) {
                        delegate->onChooseWrongAnswer();
                    }
                }
                
                is_choose = false;
                img_choose = nullptr;
                is_answer_done = false;
            }), NULL));
            runActionHand(5.0);
        }
    }
    else
    {
        is_choose = false;
    }
}

void MJGameBuildWall_Item::setDoneAnswer(){
    if (img_choose) {
        img_choose->runAction(MoveBy::create(0.05f, Vec2(0, -30)));
    }
}

cocos2d::Rect MJGameBuildWall_Item::getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find){
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


void MJGameBuildWall_Item::setDelegate(MJGameBuildWall_ItemDelegate *_delegate){
    this->delegate = _delegate;
}

ui::Widget::ccWidgetClickCallback MJGameBuildWall_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onTapFish") {
//        return CC_CALLBACK_1(MJGameBuildWall_Item::onTapFish, this);
    }
    return nullptr;
}

cocos2d::Vec2 MJGameBuildWall_Item::getGuideStartPosition() {
    auto bb = list_answer.front()->getBoundingBox();
    auto p = this->getPosition();
    p = this->getParent()->convertToWorldSpace(p);
    return p;
}

cocos2d::Vec2 MJGameBuildWall_Item::getGuideStopPosition() {
    auto bb = _layout_compare->getBoundingBox();
    auto p = Point(bb.getMidX(), bb.getMidY());
    p = _layout_compare->getParent()->convertToWorldSpace(p);
    return p;
}

void MJGameBuildWall_Item::runActionHand(float timeDelay)
{
    Director::getInstance()->getRunningScene()->unschedule("show_guide_brick_layers");
    this->schedule([this](float) {
        if (auto hand = utils::findChild(this, "hand_suggetion"))
        {
            hand->removeFromParent();
        }
        AUDIO_ENGINE->stopSound(_item_info.audio_path_full);
        _isPlayAudio = false;
        AUDIO_ENGINE->playSound(_item_info.audio_path_full, false, [this] {
            _isPlayAudio = true;
            });
        auto start = list_answer[0]->getPosition();
        auto stop = pos_layout + cocos2d::Vec2(rect.size.width / 2, 0);
        auto hand = cocos2d::ui::ImageView::create("mjstory/new_ui/hand.png");
        this->addChild(hand, INT_MAX - 1);
        hand->setName("hand_suggetion");
        hand->setPosition(start - Point(0, 25));

        hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
    }, 8, 100, timeDelay, "show_guide_brick_layers");
}

bool MJGameBuildWall_Item::checkIsPlayAudio()
{
    auto cc = _isPlayReadText;
    return _isPlayAudio && _isPlayReadText;
}

void MJGameBuildWall_Item::setCheckIsPlayAudio(bool isPlayAudio)
{
    _isPlayAudio = isPlayAudio;
}
