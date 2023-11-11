
#include "GameMakeAMatch.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include "MSGradeManager.h"

#define audio_nhachoc "sounds/makeamatch/Make a match.mp3"
#define audio "sounds/makeamatch/intro_phonic.mp3"

INITIALIZE_READER(MJGameMakeAMatch);

USING_NS_CC;

std::once_flag make_a_match_reader;

mj::GameDataSet<mj::MakeAMatchDataSet> MJGameMakeAMatch::generateData(rapidjson::Value value_data, std::string resource) {
    auto images = math::resource::ResHelper::getImages("icons.json");

    mj::GameDataSet<mj::MakeAMatchDataSet> data;
    mj::MakeAMatchDataSet set;

    auto arrObjects = value_data["text_info"].GetArray();
    for (int i = 0; i < (int)arrObjects.Size(); i++) {
        auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
        mj::GameObject game_object = {
          img_nature->getText(),
          resource + img_nature->path,
          resource + img_nature->getAudio()
        };
        set.objects.push_back(game_object);
    }

    data.sets.push(set);

    return data;
}

mj::GameDataSet<mj::MakeAMatchDataSet> MJGameMakeAMatch::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::MakeAMatchDataSet> data;
    mj::MakeAMatchDataSet set;

    auto gamedict = value_map["game"].asValueMap();
    auto arrObjects = value_map["text_info"].asValueVector();
    for (auto it : arrObjects) {
        auto obj = it.asValueMap();
        mj::GameObject game_object = {
            obj["text"].asString(),
            resource + obj["img_path"].asString(),
            resource + obj["audio_path"].asString()
        };
        set.objects.push_back(game_object);
    }

    data.sets.push(set);

    return data;
}

mj::GameDataSet<mj::MakeAMatchDataSet> MJGameMakeAMatch::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    mj::GameDataSet<mj::MakeAMatchDataSet> data;
    mj::MakeAMatchDataSet set;

    auto arrObjects = value_data["text_info"].GetArray();
    int m = (int)arrObjects.Size();
    for (int i = 0; i < (int)arrObjects.Size(); i++) {
        auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
        mj::GameObject game_object = {
          img_nature->getText(),
          img_nature->path,
          img_nature->getAudio()
        };
        set.objects.push_back(game_object);
    }

    data.sets.push(set);

    return data;
}

MJGameMakeAMatch* MJGameMakeAMatch::createGame(mj::GameDataSet<mj::MakeAMatchDataSet> data) {
    std::call_once(make_a_match_reader, [] {
        REGISTER_CSB_READER(MJGameMakeAMatch);
        REGISTER_CSB_READER(MJGameMakeAMatch_Item);
        });
    auto p = static_cast<MJGameMakeAMatch*>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/game/makeamatch/GameMakeAMatch.csb"));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameMakeAMatch::didLoadFromCSB(mj::GameDataSet<mj::MakeAMatchDataSet> data) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _data = data;
    content_layout = this->getChildByName<Layout*>("content");
    bg = utils::findChild<ui::ImageView*>(this, "bg");
    _results = utils::findChild<ui::ImageView*>(this, "bg_0");
    _results->setZOrder(INT_MAX -1);
    results = _results->getPosition();
    _results->setVisible(false);
    panel_object = content_layout->getChildByName<Layout*>("panel_object");
    panel_object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panel_object->setPosition(visibleSize / 2);
    panel_results = content_layout->getChildByName<Layout*>("panel_results");
    panel_results->setZOrder(INT_MAX - 1);
    pan_results = panel_results->getPosition();
    panel_results->setVisible(false);

    _current_set = _data.sets.front();
    isTestGame = CONFIG_MANAGER->isTestGame();
    adjustDifficulity();
    win_size = Director::getInstance()->getWinSize();

}

void MJGameMakeAMatch::adjustDifficulity() {
    auto limit = 0;
    mj::helper::shuffle(_current_set.objects);

    if (_data.info.level <= mj::GameDifficulity::EASY) {
        limit = 4;
    }
    else if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
        limit = 4;
    }
    else {
        limit = 6;
    }

    if (isTestGame) {
        limit = 100;
    }

    if (limit <= (int)_current_set.objects.size()) {
        _current_set.objects.erase(_current_set.objects.begin() + limit, _current_set.objects.end());
    }
}

void MJGameMakeAMatch::setColumnRowItem() {
    _column = 4;
    if ((int)_current_set.objects.size() <= 4) {
        _row = 2;
    }
    else _row = 3;

    if (isTestGame && (int)_current_set.objects.size() > 6) {
        if ((int)_current_set.objects.size() >= 10) {
            _column = 5;
        }
        _row = (int)_current_set.objects.size() * 2 / _column + 1;
    }

    if (win_size.width > 1024) {//Màn hình dài > ipad thì giãn item ra theo tỉ lệ cho đẹp.
        auto do_chenh = win_size.width - 1024;
        do_chenh = do_chenh * 0.8;
        do_chenh = do_chenh / _column;
        item_size_w += do_chenh;
    }
    auto size_object = (int)_current_set.objects.size();
    CCLOG("num_tiem = %d", size_object);
    int count_pos = 0;
    //khởi tạo list toạ độ.
    for (int i = _row - 1; i >= 0; i--) {
        int offset_heigth = 100;
        if (i == 1)
        {
            offset_heigth = 50;
        }
        if (i == 1)
        {
            offset_heigth = 150;
        }
        for (int j = 0; j < _column; j++) {
            CCLOG("j = %d | i = %d", j, i);
            list_pos.push_back(Vec2(item_size_w / 2 + j * item_size_w + 150, item_size_h / 2 + i * item_size_h + offset_heigth));
            count_pos++;
            if (count_pos >= 2 * size_object) {
                break;
            }
        }
        if (count_pos >= 2 * size_object) {
            break;
        }
    }

    if (_column == 4 && (2 * size_object) % _column == 2) {
        list_pos[2 * size_object - 1].x += item_size_w * 1.25;
        list_pos[2 * size_object - 2].x += item_size_w * 0.75;
    }
}

void MJGameMakeAMatch::onEnter() {
    MJGameBase::onEnter();
    auto bg = cocos2d::utils::findChild(this, "bg");
    if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width)
        bg->setScaleX(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);

    setColumnRowItem();
    this->runAction(Sequence::create(DelayTime::create(_time_delay - 0.25f), CallFunc::create([=] {
        if (MJDEFAULT->getBoolForKey(key_background_music)) {
            AUDIO_ENGINE->playBackgroundMusic("sounds/makeamatch/makeamatch_bg.mp3");
        }
        runAnimStartGame();
        }), NULL));
}

void MJGameMakeAMatch::onExitTransitionDidStart() {
    Layout::onExitTransitionDidStart();
}


void MJGameMakeAMatch::loadFrames() {
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    this->addChild(_speaker);
    this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
        radio();
        }), NULL));

    guideGame();
    int tag = 100;
    int count_box_results = 0;  auto box_size = 165;
    auto type1 = 0, type2 = 0;
    if (_data.info.game_id == mj::MAKE_A_MATCH_TT) {
        type1 = type2 = TT_Type;
    }
    else if (_data.info.game_id == mj::MAKE_A_MATCH_TI) {
        type1 = TT_Type;
        type2 = II_Type;
    }

    auto list_object = _current_set.objects;
    _arrObjectSize = list_object.size();
    ValueMap map_type;
    for (auto object : _current_set.objects) {
        list_object.push_back(object);
        map_type[object.text] = type1;
        //Add box_results = Hàng kết quả bên dưới.
        auto box_results = ImageView::create("games/makeamatch/makeamatch_box3.png");
        panel_results->addChild(box_results); 
        auto pos_from = Vec2(win_size.width + 300, box_size / 2);
        auto pos_to = Vec2(box_size / 2 + box_size * count_box_results, box_size / 2);
        box_results->setPosition(pos_from);
        box_results->runAction(Sequence::create(DelayTime::create(0.05f * (tag - 101)), EaseSineOut::create(MoveTo::create(0.2f, pos_to)), NULL));
        box_results->setTag(200 + count_box_results++);
        _boxResultSizeWidth = box_results->getContentSize().width;
    }

    mj::helper::shuffle(list_object);
    
    std::vector<MJGameMakeAMatch_Item*> listItem = {};
    std::vector<MJGameMakeAMatch_Item*> listItemText = {};

    int idItem = 0;
    int idObj = 0;
    float fontSizeForText = 0;
    
    for (auto object : list_object) {
        auto item1 = MJGameMakeAMatch_Item::createFrame(object, map_type[object.text].asInt());

        if (map_type[object.text].asInt() != 2)
        {
            listItemText.push_back(item1);
            fontSizeForText = item1->getFontSizeText();
        }

        map_type[object.text] = type2;
        item1->setAnchorPoint(Vec2(0.5, 0.5));
        item1->setPosition(list_pos[tag - 100]);
        item1->setName(item1->getObjectName());
        item1->setDelegate(this);
        item1->setTag(tag);
        tag++;
        panel_object->addChild(item1);
        listItem.push_back(item1);
    }

    float maxItemSize = -1;

    float constSixz = 60;

    for (int i = 0; i < listItemText.size(); i++) {

        auto textItem = listItemText[i]->getText();

        if (textItem->getBoundingBox().size.width + constSixz > maxItemSize)
        {
            maxItemSize = textItem->getBoundingBox().size.width + constSixz;
            idItem = idObj;
        }
        idObj++;
    }

    Text* textItemWithLengthMax = listItemText[idItem]->getText();

    while (textItemWithLengthMax->getBoundingBox().size.width + constSixz > listItemText[idItem]->getBoundingBox().size.width)
    {
        textItemWithLengthMax->setFontSize(textItemWithLengthMax->getFontSize() - 1);

        fontSizeForText = textItemWithLengthMax->getFontSize();
    }

    for (auto object : listItem) {
        object->setFontSizeText(fontSizeForText);
        object->setScale(0);
    }
    
    int number = list_object.size() / 2;
    float space = 10;
    float scalee = 1.2f;
    float scaleee1 = listItem[0]->getScale();
    listItem[0]->setScale(1.2);
    
    float sizeObject = listItem[0]->getBoundingBox().size.width * number + (number - 1) * space;
    
    bool isSetPositonNew = false;
    
    while (sizeObject + 20 > win_size.width) {
        isSetPositonNew = true;
        listItem[0]->setScale(scalee - 0.005);
        sizeObject = listItem[0]->getBoundingBox().size.width * number + (number - 1) * space;
        scalee = scalee - 0.005;
    }
    listItem[0]->setScale(scalee);
    float sizeButton = listItem[0]->getBoundingBox().size.width;
    float posX = win_size.width - sizeButton * number - (number - 1) * space;
    posX = posX / 2;
    int count = 0;
    listItem[0]->setScale(scaleee1);
    for(auto item1 : listItem)
    {
        if(isSetPositonNew)
        {
            if(count == number)
            {
                posX = win_size.width - sizeButton * number - (number - 1) * space;
            }
            item1->setPositionX(posX + sizeButton / 2);
            count++;
        }
        
        posX = posX + space + sizeButton;
        item1->runAction(Sequence::create(DelayTime::create(0.15f * (tag - 102)), CallFunc::create([=] {
            AUDIO_ENGINE->playEffect("theendpage/theendpage_showword.mp3");
            }), EaseBackOut::create(ScaleTo::create(0.5f, scalee)), NULL));
    }
    
    listItem.clear();
    list_object.clear();

    auto min_h = 175;
    panel_results->setPositionX(win_size.width / 2 - (int)_current_set.objects.size() * box_size / 2);//căn giữa panel_results.

    if (isTestGame) {
        auto scale = 1.0f;
        if (_row * item_size_h > (win_size.height - 185)) {
            scale = (float)(win_size.height - 185) / (_row * item_size_h);
        }
        else if (_column * item_size_w > win_size.width) {
            scale = win_size.width / (_column * item_size_w);
        }
        panel_object->setScale(scale);
        panel_object->setPosition(Vec2((win_size.width - _column * item_size_w * scale) / 2, min_h + (win_size.height - 185 - _row * item_size_h * scale) / 2));//căn giữa panel_object


        panel_results->setScale(scale);
        panel_results->setPositionX(win_size.width / 2 - (int)_current_set.objects.size() * box_size * scale / 2);//căn giữa panel_results.
        panel_results->setPositionY(panel_results->getPositionY() + (box_size - box_size * scale) / 2);
    }

}


void MJGameMakeAMatch::runAnimStartGame() {
    auto title = ImageView::create("games/makeamatch/makeamatch_title.png");
    title->setPosition(win_size / 2);
    this->addChild(title, 9000000);
    title->setScale(0);

    title->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.5f, 1.0f)), DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([=] {
        title->removeFromParent();
        AUDIO_ENGINE->playSound("sounds/makeamatch/intro_phonic.mp3", false, [=]() {
            loadFrames();
            });
        // play intro game phonic
        if (ms::GradeManager::getInstance().getScope() == ms::GradeManager::LESSONS_PHONIC) {
            AUDIO_ENGINE->playEffect("sounds/makeamatch/intro_phonic.mp3");
        }
        }), NULL));
}


void MJGameMakeAMatch::onFlipCardWithName(Ref* sender) {
    AUDIO_ENGINE->stopSound(audio);
    stopGuideGame();
    guideGame();
    _speaker->setTouchEnabled(true);
    if (isRunAni) return;
    isRunAni = true;
    auto item = dynamic_cast<MJGameMakeAMatch_Item*>(sender);
    item->flipCard(item->box1, item->box2);

    if (item->_type == item->Text_Type || item->_type == item->Img_Type) {
        AUDIO_ENGINE->playEffect(item->_data.audio_path);
    }
    auto card_name = item->getObjectName();
    auto tag = (int)item->getTag();
    CCLOG("touch = %s | tag = %d", card_name.c_str(), tag);
    if (count_flip == 0) {
        count_flip = 1;
        tag_flip_recent = tag;
        isRunAni = false;
    }
    else {
        panel_object->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
            auto item_recent = panel_object->getChildByTag<MJGameMakeAMatch_Item*>(tag_flip_recent);
            if (item_recent->getObjectName() == card_name) {//Đúng cặp
                _results->setVisible(true);
                panel_results->setVisible(true);
                count_match_true++;
                CCLOG("count_match_true = %d", count_match_true);
                if (!game_play_enable && count_match_true == 2) {
                    pauseGame();
                    return;
                }
                this->mergeTwoItems(item_recent, item);//move 2 item lại với nhau tại trung điểm.

            }
            else {//sai
                item->flipCard(item->box2, item->box1);
                item_recent->flipCard(item_recent->box2, item_recent->box1);
                CCLOG("sai bét");

                panel_object->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=] {
                    isRunAni = false;
                    }), NULL));
            }

            count_flip = 0;
            }), NULL));
    }
}

void MJGameMakeAMatch::mergeTwoItems(MJGameMakeAMatch_Item* item1, MJGameMakeAMatch_Item* item2) {
    item1->setLocalZOrder(100);
    item2->setLocalZOrder(100);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    midle_pos = Vec2(visibleSize.width/2, visibleSize.height/2);//toạ độ move den sau khi ghep dung

    auto duration = 0.5f * item2->getPosition().distance(item1->getPosition()) / 768;

    item1->runAction(MoveTo::create(duration, midle_pos));
    item2->runAction(Sequence::create(MoveTo::create(duration, midle_pos), CallFunc::create([=] {
        auto type = item2->Merge_ImgText_Type;
        if (_data.info.game_id == mj::MAKE_A_MATCH_TT) {
            type = item2->Merge_TextText_Type;
        }

        auto merge_item = MJGameMakeAMatch_Item::createFrame(item2->_data, type);
        merge_item->setPosition(panel_results->convertToNodeSpace(panel_object->convertToWorldSpace(midle_pos)));
        panel_results->addChild(merge_item);
        this->moveItemMergeToResults(merge_item);

        item1->removeFromParent();
        item2->removeFromParent();

        }), DelayTime::create(0.5f), CallFunc::create([=] {

            }), NULL));
    if (count_match_true < (int)_current_set.objects.size()) {
        this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {
            auto cc = _results->getAnchorPoint();
            _results->setVisible(false);
            panel_results->setVisible(false);
            }), NULL));
    }
}

void MJGameMakeAMatch::moveItemMergeToResults(MJGameMakeAMatch_Item* item) {
    //    auto pos_results;
    item->text->setFontName("fonts/KGWHATTHETEACHERWANTS.TTF");
    item->setAnchorPoint(Vec2(0.5, 0.5));
    item->setPosition(item->getPosition());
    auto duration = 0.15f;
    auto box_results = panel_results->getChildByTag(200 + count_match_true - 1);//box_results;
    Vec2 pos_results;
    if (box_results) {
        pos_results = box_results->getPosition();//lấy toạ độ results cần move đến.
        pos_results.y += 5;
    }
    AUDIO_ENGINE->playEffect("sounds/makeamatch/makeamatch_merge.mp3");
    auto ac_merge_scale = Sequence::create(ScaleTo::create(duration + 0.1f, 3.0f), ScaleTo::create(duration, 2.8f), ScaleTo::create(duration, 2.9f), ScaleTo::create(duration + 0.05f, 2.8f), NULL);

    auto result_scale = 0.76f;
    auto duration_move = 1.0f * item->getPosition().distance(pos_results) / 768;
    auto ac_moveto_results = Sequence::create(DelayTime::create(1.0f), Spawn::create(MoveTo::create(duration_move, pos_results), ScaleTo::create(0.5f, result_scale), NULL), NULL);

    auto ac_results_scale = Sequence::create(DelayTime::create(0.1f), ScaleTo::create(0.1f, result_scale + 0.15f), ScaleTo::create(0.1f, result_scale), ScaleTo::create(0.1f, result_scale + 0.05f), ScaleTo::create(duration, result_scale), NULL);
    item->runAction(Sequence::create(ac_merge_scale, CallFunc::create([=] {
        //play sound
        item->setColorForLabel(Color3B::RED);
        AUDIO_ENGINE->playSound(item->_data.audio_path, false, [=]() {
            item->setColorForLabel(Color3B::BLACK);
        });
        
        }),DelayTime::create(0.2f), ac_moveto_results, CallFunc::create([=] {

            //item->setOnResults(); //move đến box_results, gọi hàm ẩn backgroud to, hiện background bé!
            AUDIO_ENGINE->playEffect("sounds/makeamatch/makeamatch_correct.mp3");

            }), ac_results_scale, CallFunc::create([=] {

                isRunAni = false;//Xong tất cả animation => trả biến check về false để cho click vào item tiếp.
                if (count_match_true == (int)_current_set.objects.size()) {
                    this->onFinishGame();
                }

                }), NULL));
}

void MJGameMakeAMatch::onFinishGame() {
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        _results->runAction(MoveTo::create(0.5f, Vec2(_results->getPosition().x - _results->getPosition().x/4, visibleSize.height/3)));//khung mau xanh chay ra giua man hinh
        _results->runAction(ScaleTo::create(0.5f, 1.5));   
        panel_results->runAction(ScaleTo::create(0.5f, 1.5));
        panel_results->runAction(MoveTo::create(0.5f, Vec2(panel_results->getPosition().x - _arrObjectSize * _boxResultSizeWidth / 4 , visibleSize.height / 3)));//dap an chay ra giua man hinh
        AUDIO_ENGINE->playEffect("sounds/makeamatch/makeamatch_win.mp3");
        }), DelayTime::create(1.25f),
            CallFunc::create([=] {
            auto text = Text::create("", "fonts/KGWHATTHETEACHERWANTS.TTF", 80);
            text->setPosition(this->getContentSize() / 2);
            text->setColor(Color3B::BLACK);
            this->addChild(text, 9000000);

            text->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.5f, 1.0f), DelayTime::create(0.5f), CallFunc::create([=] {
                AUDIO_ENGINE->stopAllEffects();
                AUDIO_ENGINE->stopBackgroundMusic();
                this->onGameOver();
                }), NULL));
                }), NULL));
}

mj::GameDataSet<mj::MakeAMatchDataSet> MJGameMakeAMatch::loadData() {
    mj::GameDataSet<mj::MakeAMatchDataSet> data;
    mj::MakeAMatchDataSet set;
    set.objects = {
        {"Ship", "games/sample/ship.png"},
        {"Ball", "games/sample/ball.png"},
        {"Truck", "games/sample/truck.png"},
        {"Block Shape", "games/sample/cube.png"},
        {"Turtle Car", "games/sample/car.png"},
        {"Cog", "games/sample/cog.png"}
    };

    data.sets.push(set);

    return data;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameMakeAMatch::onLocateClickCallback(const std::string& callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameMakeAMatch::onRefresh, this);
    }

    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJGameMakeAMatch::onClose, this);
    }
    return nullptr;
}

void MJGameMakeAMatch::onRefresh(cocos2d::Ref* sender) {
    auto data = this->loadData();
    this->removeFromParent();
    Director::getInstance()->getRunningScene()->addChild(MJGameBase::createGame(data));
}

void MJGameMakeAMatch::onClose(cocos2d::Ref* sender) {
    this->removeFromParent();
}

void MJGameMakeAMatch::radio() {
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        _speaker->setTouchEnabled(false);
        AUDIO_ENGINE->playSound(audio, false, [=] {
            _speaker->setTouchEnabled(true);
            });

        stopGuideGame();
        guideGame();
        });
}

void MJGameMakeAMatch::guideGame() {
    this->schedule([=](float) {
        _audio = AUDIO_ENGINE->playSound(audio_nhachoc);
        }, 10, 100, 10, "guideGame");
}

void MJGameMakeAMatch::stopGuideGame() {
    this->unschedule("guideGame");
    AUDIO_ENGINE->stopSound(audio_nhachoc);
}
