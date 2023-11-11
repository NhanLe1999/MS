//
//  Firefly.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/31/18.
//

#include "Firefly.h"
#define CSB_NAME "csb/game/FireFly/FireFly.csb"

USING_NS_CC;
INITIALIZE_READER(Firefly);

#define update_time 3.0f
//#define speed 150
#define delta_opacity 85
#define DELAY_TIME 7

std::once_flag Firefly_reader;

Firefly* Firefly::createGame(std::string json_file, bool isAutoPlay){
    
    std::call_once(Firefly_reader, []{
        REGISTER_CSB_READER(Firefly);
    });
    
    auto p = static_cast<Firefly*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void Firefly::didLoadFromCSB(std::string json_file, bool isAutoPlay){
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    _is_vocab = document.FindMember("is_vocab")->value.GetBool();
    max_word = document.FindMember("max_word")->value.GetInt();
    rapidjson::Value& json_data=document.FindMember("data")->value;
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    this->generateData(json_data.GetArray(), images);
    
    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;
    this->loadFrame();
}

void Firefly::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        rapidjson::Value& json_item = value_data[i].FindMember("game")->value.FindMember("item")->value;
        
        auto letter = json_item.FindMember("text")->value.GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);
        
        mj::PHONIC_INFO word;
        word.text_full = pImage->getText("name_1");// json_game[j].FindMember("item")->value.FindMember("text")->value.GetString();
        word.audio_full = pImage->getAudios("name_1").front();//json_game[j].FindMember("item")->value.FindMember("audio")->value.GetString();
        word.text_phonic = pImage->getText("name_n");
        word.audio_phonic = pImage->getAudios("name_n").front();
        word.image_path = pImage->path;
        word.video_path = "";
        word.highlights = {0};
        word.highlight_color = Color3B::RED;
        words.push_back(word);
    }
    
    number_basket = (int) words.size();
}

void Firefly::gameConfig(){
    
    mj::helper::shuffle(words);
    
    //positions
    for(int i = 0; i < area_51->getChildrenCount(); i++){
        std::pair<Vec2, int> pair;
        pair.first = area_51->getChildByName("Panel_" + cocos2d::StringUtils::toString(i))->getPosition();
        area_51->getChildByName<Layout*>("Panel_" + cocos2d::StringUtils::toString(i))->setBackGroundColorType(BackGroundColorType::NONE);
        pair.second = 0;
        position_list.push_back(pair);
    }
}

void Firefly::loadFrame(){
    root_layout = utils::findChild<Layout*>(this, "root_panel");
    area_51 = utils::findChild<Layout*>(root_layout, "area_51");
    area_51->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
    area_51->setLocalZOrder(98);
  
    auto weed = utils::findChild<ImageView*>(root_layout, "weed");
    weed->setLocalZOrder(99);
    weed->runAction(RepeatForever::create(Sequence::create(EaseOut::create( SkewTo::create(1, 10, 0), 2), SkewTo::create(2, -15, 0), EaseOut::create(SkewTo::create(1, 0, 0), 1), NULL)));
    // yeu cau giam khoang cach giua cac ngoi sao di 3 lan 
    auto start_x = (screen_size.width)/(max_word*number_basket*2)  ;
    //create stars
    for(int i = 0 ; i < max_word*number_basket; i++){
        Layout* star_holder = Layout::create();
        star_holder->setContentSize(Size(60, 60));
        root_layout->addChild(star_holder, 100);
        // cac ngoi sao se duoc them tu giua man hinh 
        star_holder->setPosition(Vec2((screen_size.width/2)+(start_x * (i - (max_word * number_basket/2))), screen_size.height - 50));

        ImageView* star = ImageView::create("games/firefly/6.png");
        star->ignoreContentAdaptWithSize(false);
        star_holder->addChild(star);
        star->setContentSize(star_holder->getContentSize()/2);
        star->setName("star_shadow");
        
        ImageView* star_light = ImageView::create("games/firefly/7.png");
        star_light->ignoreContentAdaptWithSize(false);
        star_holder->addChild(star_light);
        star_light->setContentSize(star_holder->getContentSize()/2);
        star_light->setOpacity(0);
        star_light->setName("starlight");
        
        stars.push_back(star_holder);
    }
    
    //create background firefly
    GAFWrapper* dom_1 = GAFWrapper::createGAF("gaf/firefly/domdom_bg/domdom_bg.gaf");
    root_layout->addChild(dom_1);
    dom_1->setPosition(Vec2(root_layout->getContentSize().width*0.15, root_layout->getContentSize().height*0.15));
    dom_1->play(true);

    GAFWrapper* dom_2 = GAFWrapper::createGAF("gaf/firefly/domdom_bg/domdom_bg.gaf");
    root_layout->addChild(dom_2);
    dom_2->setFlippedX(true);
    dom_2->setPosition(Vec2(root_layout->getContentSize().width*0.9, root_layout->getContentSize().height*0.2));
    dom_2->play(true);
    
    gameConfig();
    
    
    float space = root_layout->getContentSize().width/number_basket;
    float pos_x = space/2;
    //create glasses
    for(int i = 0; i < number_basket; i++){
        
        Layout* binh = Layout::create();
        binh->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
        root_layout->addChild(binh,1);
        binh->setPositionY(root_layout->getContentSize().height*random(1.68, 1.7));
        binh->setPositionX(pos_x + space*i);
        binh->setContentSize(Size(300, 500));
        binh->setTouchEnabled(true);
        binh->addClickEventListener(CC_CALLBACK_1(Firefly::onClickedBasket, this));
        
        ImageView* basket = ImageView::create("games/firefly/2.png");
        binh->addChild(basket);
        basket->setPosition(Vec2(binh->getContentSize()/2));
        basket->setScale(0.8);
        basket->setName("basket");
       
        ImageView* img = ImageView::create( words[i].image_path);
        basket->addChild(img);
        img->setPosition(Vec2(basket->getContentSize().width*0.5, basket->getContentSize().height*0.25));
        img->setName(words[i].text_full);
        img->setContentSize(Size (185, 160));
        img->ignoreContentAdaptWithSize(false);
        
        ImageView* basket_light = ImageView::create("games/firefly/3.png");
        basket_light->setOpacity(0);
        basket_light->setScale(0.8);
        binh->addChild(basket_light);
        basket_light->setName("light");
        basket_light->setPosition(Vec2(binh->getContentSize()/2));
        
        GAFWrapper* smoky = GAFWrapper::createGAF("gaf/firefly/effect/effect.gaf");
        binh->addChild(smoky);
        smoky->setName("smoky");
        smoky->setScale(0.8);
        smoky->setPosition(Vec2(binh->getContentSize()/2));
        smoky->setVisible(false);
        
        binh->setName("basket_" + words[i].text_full);
        binh->setTag(i);
        baskets.push_back(binh);
    }
    
     gaf_asset = gaf::GAFAsset::create("gaf/firefly/firefly/firefly.gaf");
    //create flies
    for(int i = 0; i < words.size(); i++){
        flies.push_back( createFly(words[i].text_full, i, i));
    }

    root_layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->setIgnoreAnchorPointForPosition(true);
    root_layout->setScale(2.5);
    root_layout->runAction(Sequence::create(ScaleTo::create(1, 1), CallFunc::create([=]{
        onGameStart();
    }), NULL));
}

void Firefly::onGameStart(){

    _speaker = mj::helper::addSpeaker(this);
    _speaker->setPosition(Vec2(40, Director::getInstance()->getVisibleSize().height - 40));
    _speaker->setScale(0.9f);
    _speaker->setTouchEnabled(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        this->hideGuideGame();
        _currentGuidingAudio = cocos2d::experimental::AudioEngine::play2d("sounds/firefly/instruction.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentGuidingAudio, [=](int id, string path) {
            inActiveMode();
        });
    });


    int bowl_id = AUDIO_ENGINE->play("sounds/firefly/bowls.mp3");
    cocos2d::experimental::AudioEngine::setFinishCallback(bowl_id, [=](int id, string path){
        AUDIO_ENGINE->play("sounds/firefly/wing.mp3", true);
        AUDIO_ENGINE->play("sounds/firefly/intro_question.mp3", false, 1.0f, [=]() {
            _speaker->setTouchEnabled(true);
            });
        this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=]{
			inActiveMode(10);
        }), DelayTime::create(1.0), CallFunc::create([=]{
                this->updateFlyPostion(update_time);
        }), NULL));
    });
	
    for(auto basket : baskets){
        int angle = random(-5, 5);
        while(angle == 0){
            angle = random(-5, 5);
        }
        float time = random(0.8, 1.0);
        basket->runAction(EaseOut::create(MoveBy::create(0.8, Vec2(0, -450)), random(2, 5)));
        basket->runAction(RepeatForever::create(Sequence::create(RotateTo::create(time, random(angle, angle)), RotateTo::create(time*2, -angle, -angle), RotateTo::create(time, 0, 0), NULL)));
    }

    root_layout->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(5, 1.05), ScaleTo::create(5, 1.0), NULL)));
}

void Firefly::onEnter(){
    MJMainGBase::onEnter();
    this->playBackgroundMusic("sounds/firefly/bgm.mp3", true);
    this->playBackgroundMusic("sounds/firefly/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
//    this->updateFlyPostion(update_time);

}

void Firefly::onFinishGame(){
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGame();
}

void Firefly::onExitTransitionDidStart(){
    //cocos2d::ui::Layout::onExitTransitionDidStart();
    //this->unscheduleAllCallbacks();
    //this->unscheduleUpdate();
    //this->stopAllActions();
    //AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onExitTransitionDidStart();
}

void Firefly::showGuideGame(std::string gaf_guide_path){
    _countWrongClick = 0;
    // phải kiểm tra xem có con đom đóm nào tồn tại không
	Firefly_Item* fly_guide = nullptr;
    for (int i = 0; i < flies.size(); i++) {
        auto fly = flies.at(i);
        if (fly && !fly->isDisable()){
			fly_guide = fly;
			break;
		}
	}
    
    if (fly_guide == nullptr) {
        return;
    }
    
    auto posStart = this->convertToWorldSpace(fly_guide->getPosition());
    
    auto basket = utils::findChild(root_layout, "basket_" + fly_guide->getName());
    auto posEnd = this->convertToWorldSpace( Vec2( basket->getPositionX(), basket->getPositionY() - 300 ));
    
	auto hand = GAFWrapper::createGAF(gaf_guide_path);
    hand->setScale(0.8f);
	this->addChild(hand, INT_MAX - 1);
	hand->setPosition(posStart);
    hand->setName("hand");
    hand->runAction(Sequence::create(CallFunc::create([=]() {fly_guide->pauseSchedulerAndActions(); }), DelayTime::create(0.5f), Spawn::create(MoveTo::create(1.5f, posEnd), CallFunc::create([=] {
	}), NULL), CallFunc::create([=] {
        fly_guide->resumeSchedulerAndActions();
		hand->removeFromParent();
	}), NULL));
}

void Firefly::onClickedBasket(cocos2d::Ref *sender){
    hideGuideGame();
    inActiveMode(10);
    Layout* basket = (Layout*) sender;
    auto basket_name = basket->getName();
    string basket_text = basket_name.substr( basket_name.find("_") +1);
    mj::PHONIC_INFO bk_word;
    for(auto w : words){
        if(w.text_full == basket_text){
            bk_word = w;
        }
    }
    AUDIO_ENGINE->play(bk_word.audio_full);
}

void Firefly::onTouchedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    
    Layout* fly = (Layout*)sender;
    Firefly_Item* fly_parent = (Firefly_Item*) fly->getParent();
    int zOrder = 1;
    int scale = 1;
    int id =-1;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            hideGuideGame();
            if (++_countWrongClick > 3 && !_guideShowed){
                    this->showGuideGame();
            }
            //CCLOG("began");
            scale = fly->getScale();
            fly->stopAllActions();
            fly->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4, 1.4), ScaleTo::create(0.4, 1), NULL)));
            zOrder = fly->getLocalZOrder();
            fly->setLocalZOrder(9);
            position_list[fly_parent->getTag()].second = 0;

            id = AUDIO_ENGINE->play("sounds/firefly/drag.mp3", false);

            for(auto w: words){
                if(w.text_full == fly_parent->getName()){
                    word = w;
                }
            }
            if(_is_vocab){
                cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, string path){
                    AUDIO_ENGINE->play(word.audio_full);
                });
            }
       
            this->unschedule("fly_fly") ;
            break;
            
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            //CCLOG("moving");
            fly_parent->setPosition(area_51->convertToNodeSpace(((Layout*)sender)->getTouchMovePosition()));
            break;
            
        default:
            //CCLOG("end/cancel");
            fly->stopAllActions();
            fly->setLocalZOrder(zOrder);
            fly->runAction(ScaleTo::create(0.2, scale));
            auto basket = utils::findChild<Layout*>(root_layout, "basket_" + fly_parent->getName());

            if(basket->getBoundingBox().containsPoint(area_51->convertToWorldSpace(fly_parent->getPosition())) ){
                _guideShowed = true;
                CCLOG("right");
				fly_parent->setDisable(true);
                fly_parent->retain();
                fly_parent->removeFromParent();
                fly->setTouchEnabled(false);
                root_layout->addChild(fly_parent,2);
                fly_parent->release();
                int rotate = (utils::findChild(fly, "gaf")->getTag()? 40: -40);
                auto final_pos = Vec2(basket->getPositionX(), basket->getPositionY() - basket->getContentSize().height*0.4 );
                
     

                int id = random(1, 2);
                AUDIO_ENGINE->play("sounds/firefly/correct" + cocos2d::StringUtils::toString(id) + ".mp3", false,1, [=]() {
                 });

             
                
                fly_parent->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.3, 0.1), MoveTo::create(0.3, final_pos), NULL), RotateTo::create(0.1, rotate, rotate), Spawn::create(MoveBy::create(0.2, Vec2(0, -100)), FadeOut::create(0.2), CallFunc::create([=]{ fly_parent->setLocalZOrder(0); }), NULL), CallFunc::create([=]{
                   
                    basket->runAction(EaseBounceOut::create(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.2), ScaleTo::create(0.1, 1))));
                    
                    auto root_action = (Sequence::create(MoveBy::create(0.1, Vec2(20, 0)) , EaseInOut::create(MoveBy::create(0.1, Vec2(-20, 0)), 5), NULL));
                    root_layout->runAction(Sequence::create(root_action, CallFunc::create([=]{
                        
                        AUDIO_ENGINE->play(word.audio_full);

                        total_count++;
                        
                        stars[total_count-1]->getChildByName("starlight")->runAction(Spawn::create(FadeIn::create(0.5), Sequence::create(ScaleTo::create(0.2, 1.2), ScaleTo::create(0.2, 1), NULL) , NULL));
                        
                        if(total_count == max_word * number_basket){
                            this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]{
                                    this->onFinishGame();
                            })));
                        }
                    }) , NULL));
                   
                    //dieu chinh do sang
                    auto light_index = 0;
                    if( basket->getTag() == 0){
                        light_index = count_word_1;
                    }else if (basket->getTag() == 1){
                        light_index = count_word_2;
                    }else{
                        light_index = count_word_3;
                    }
                    
                    if(light_index > 2 && light_index < 4){
                        light_level = 2;
                    }else if (light_index > 3){
                        light_level = 3;
                    }
                    
                    auto basket_light = utils::findChild(basket, "light");
                    basket_light->stopAllActions();
                    basket_light->runAction(Sequence::create(FadeIn::create(0.2f), FadeTo::create(0.5, delta_opacity* light_level), NULL));

                    auto smoky = utils::findChild<GAFWrapper*>(basket, "smoky");
                    smoky->setVisible(true);
                    smoky->play(false);
                    
                    fly_parent->removeFromParent();
                    
                    int w_id = findWordIndex(fly);
                    //create new fly
                    if(w_id != -1){
                        int fly_id = fly->getTag();
                        std::string word = words[w_id].text_full;
                        flies[fly_id] = createFly(word, fly_id, w_id);
                    } else {
                        int fly_id = fly->getTag();
                        flies[fly_id] = nullptr;
                    }
                }), NULL));
        
            }else{
                for(auto other_basket : baskets){
                    if(other_basket->getBoundingBox().containsPoint(area_51->convertToWorldSpace(fly_parent->getPosition()))){
                        AUDIO_ENGINE->play("sounds/firefly/wrong.mp3");
                        
                        utils::findChild<GAFWrapper*>(fly, "gaf")->runAction(Sequence::create(CallFunc::create([=]{
                            utils::findChild<GAFWrapper*>(fly, "gaf")->playSequence("lose");
                        }), DelayTime::create(0.5), CallFunc::create([=]{
                            utils::findChild<GAFWrapper*>(fly, "gaf")->playSequence("idle", true);
                        }), NULL));
                    }
                }
                changeFlyPosition(fly_parent, false);
            }
            
            
            inActiveMode(10);
            this->updateFlyPostion(update_time);
            
            break;
    }
}

int Firefly::findWordIndex(cocos2d::Node *fly){
    int w_id = -1;
    
    std::vector<int> left_word_id;
    if(number_basket > 3){
        if(count_word_4 < max_word){
            left_word_id.push_back(3);
        }
    }
    
    if(number_basket > 2){
        if(count_word_3 < max_word){
            left_word_id.push_back(2);
        }
    }
   
    if(number_basket > 1){
        if(count_word_2 < max_word){
            left_word_id.push_back(1);
        }
    }
    
    if(count_word_1 < max_word){
        left_word_id.push_back(0);
    }
    
    if(left_word_id.size() > 0){
        w_id = left_word_id[random(0, (int) left_word_id.size()-1)];
    }
    
    return w_id;
}

Firefly_Item* Firefly::createFly(std::string word, int fly_id, int word_id){

    if(word_id == 0){
        count_word_1++;
    }else if(word_id == 1){
        count_word_2++;
    }else if (word_id == 2){
        count_word_3++;
    }else{
        count_word_4++;
    }
    
    Firefly_Item* fly = Firefly_Item::createItem(word, fly_id, word_id, gaf_asset);
    fly->setAnchorPoint(Point::ANCHOR_MIDDLE);
    area_51->addChild(fly);
    fly->setDelegate(this);
    
    int flipped = utils::findChild(fly, "gaf")->getTag();
    auto start_pos_x = (flipped)? -area_51->getContentSize().width* random(0.2, 0.4) : area_51->getContentSize().width* random(1.2, 1.4);
    auto start_pos_y = area_51->getContentSize().height * random(-0.4, 0.4);
    
    fly->setPosition(Vec2(start_pos_x, start_pos_y));
    
    int index = changeFlyPosition(fly, false);
    fly->setName(word);
    fly->setTag(index);
    
    return fly;
}

void Firefly::updateFlyPostion(float time){
    // mảng flies lúc nào cũng có 3 phần tử nên phải kiểm tra xem có con đom đóm nào tồn tại không
    int count = 0;
    for (auto fly : flies) {
        if (fly == nullptr) {
            count++;
        }
    }
    
    if (count == flies.size()) {
        return;
    }
    
    this->schedule([=](float){
        // nếu đom đóm = nullptr thì phải lấy con khác
        int index = random(0, number_basket-1);
        while (flies[index] == nullptr) {
            index = random(0, number_basket-1);
        }
        changeFlyPosition(flies[index], true);
    }, time , 10000, 0.0, "fly_fly");
    
}

int Firefly::changeFlyPosition(Firefly_Item *fly, bool changeSpeed){
    
    if(fly->isMoving())
        return -1;
  
    float speed = 170;
    int index = random(0, (int) position_list.size()-1);
    
    while (true) {
        if (position_list[index].second != 0) {
            //nếu vị trí không trống, lấy vị trí mới
            index = random(0, (int) position_list.size()-1);
        } else {
            //vị trí đang trống
            if (fly->getPosition().getDistance(position_list[index].first) < 150) {
                // nếu khoảng cách quá ngắn, lấy vị trí mới
                index = random(0, (int) position_list.size()-1);
            } else {
                // khoảng cách đủ dài, break
                if (fly->getPosition().getDistance(position_list[index].first) > 700){
                    speed = 230;
                }
                break;
            }
        }
    }
    
    auto time_move = 1.5;
    if(changeSpeed){
        auto distance = fly->getPosition().getDistance(position_list[index].first);
        time_move = distance/speed;
    }
    
    fly->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(CallFunc::create([=]{
        fly->setMoving(true);
        utils::findChild<Layout*>(fly, "item_layout")->setTouchEnabled(false);
    }), EaseInOut::create(MoveTo::create(time_move, position_list[index].first), 5)), CallFunc::create([=]{
        fly->setMoving(false);
        utils::findChild<Layout*>(fly, "item_layout")->setTouchEnabled(true);
    })) );
    if(fly->getTag() >= 0 && fly->getTag() < position_list.size()) 
		position_list[fly->getTag()].second = 0;
    position_list[index].second = 1;
    fly->setTag(index);

    return index;
}
void Firefly::inActiveMode(float delay_time) {
    this->scheduleOnce([=](float) {
        if (!_guideShowed) {
            this->showGuideGame();
        }
        cocos2d::experimental::AudioEngine::stop(_currentGuidingAudio);
        _currentGuidingAudio = cocos2d::experimental::AudioEngine::play2d("sounds/firefly/instruction.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentGuidingAudio, [=](int, std::string) {
            inActiveMode(delay_time);
            });
        },delay_time, "active_mode");
}

void Firefly::hideGuideGame() {
    if (auto hand =utils::findChild(this, "hand")) {
        hand->removeFromParent();
    }
    this->unschedule("active_mode");
    if (_currentGuidingAudio != -1) {
        cocos2d::experimental::AudioEngine::stop(_currentGuidingAudio);
    }
}