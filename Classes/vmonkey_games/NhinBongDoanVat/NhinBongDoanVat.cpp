//
//  NhinBongDoanVat.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "NhinBongDoanVat.h"

#define CSB_NAME "csb/game/NhinBongDoanVat/NhinBongDoanVat.csb"
#define bg_music "sounds/NhinBongDoanVat/bg_music.mp3"

#define SOUND_CORRECT "sounds/NhinBongDoanVat/correct.mp3"
#define SOUND_WRONG "sounds/NhinBongDoanVat/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/NhinBongDoanVat/touch_%d.mp3"
#define SOUND_DROP "sounds/NhinBongDoanVat/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/NhinBongDoanVat/moveback_%d.mp3"


USING_NS_CC;
INITIALIZE_READER(NhinBongDoanVat);

std::once_flag NhinBongDoanVat_reader;

NhinBongDoanVat* NhinBongDoanVat::createGame(){
    
    std::call_once(NhinBongDoanVat_reader, []{
        REGISTER_CSB_READER(NhinBongDoanVat);
    });
    
    auto p = static_cast<NhinBongDoanVat*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void NhinBongDoanVat::didLoadFromCSB(){
    
    this->generateData();
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
}

void NhinBongDoanVat::generateData(){
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
    
//    // get data
    string json_file = "config.json";
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    auto q_id = document.FindMember("question")->value.GetString();
    auto q_img = math::resource::ResHelper::getImage(images, q_id);
	q_obj.text = q_img->getQuestionVMonkey("name_1", lang);
    q_obj.audio_path = q_img->getAudios("name_1", lang).front();
    
    
    rapidjson::Value& img_pieces = document.FindMember("img_pieces")->value;
    number_obj = img_pieces.GetArray().Size();
    
    for (int j = 0; j < number_obj ; ++j) {
        auto answer = img_pieces[j].GetString();
        mj::GameObject go;
        auto image_answer = math::resource::ResHelper::getImage(images, answer);
        go.text = image_answer->getText("name_1", lang);
        go.audio_path = image_answer->getAudios("name_1", lang).front();
        go.image_path = image_answer->path;
       imgs.push_back(go);
    }
    
}

#include "Math_Quiz.h"
void NhinBongDoanVat::loadFrame(){
    radio();
    auto speakerCSB = utils::findChild(this, "speaker");
    auto image1 = utils::findChild(this, "Image_1");
    speakerCSB->setVisible(false);
    image1->setVisible(false);
    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
    
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    root_layout->setBackGroundColorType(BackGroundColorType::NONE);
    auto frameQuestion = utils::findChild<Sprite*>(root_layout, "question");
    frameQuestion->setPosition(Vec2(521.2, 679.68));
    auto question = utils::findChild<Text*>(root_layout, "Text_1");
    question->setContentSize(Size(utils::findChild<Sprite*>(root_layout, "question")->getContentSize()*0.9));
    question->ignoreContentAdaptWithSize(false);
//    question->setString("Ghép hình vào bóng cho đúng.");
    question->setString(q_obj.text);
    question->setFontName(font_name);
    question->setFontSize(font_sz);
    
    auto size = Size(1024/(number_obj+1), 1024/(number_obj+1));
    std::vector<ImageView*> slots = {};
    std::vector<std::string> s = {};
    for(int i = 0; i < number_obj; i++){
        auto slot_obj = ImageView::create(imgs[i].image_path);
        auto name = StringUtils::format("Slot_%s", imgs[i].image_path.c_str());
        slot_obj->setName(name);
        slot_obj->setTag(i);
        s.push_back(imgs[i].image_path);
        auto old_color = slot_obj->getColor();
        slot_obj->runAction(TintTo::create(0, Color3B::BLACK));
        _colors[name] = old_color;
        slots.push_back(slot_obj);
//        slot_obj->setContentSize(size);
//        slot_obj->ignoreContentAdaptWithSize(false);
        
        root_layout->addChild(slot_obj);
    }
    
    mj::helper::shuffle(imgs);
    
    for(int i = 0; i < number_obj; i++){
        auto obj = ImageView::create(imgs[i].image_path);
        auto name = imgs[i].image_path;
        obj->setName(name);
        objs.push_back(obj);
//        obj->setContentSize(size);
//        obj->ignoreContentAdaptWithSize(false);
        obj->addTouchEventListener(CC_CALLBACK_2(NhinBongDoanVat::onTouchObject, this));
        obj->setTouchEnabled(true);
        root_layout->addChild(obj);
    }
    
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < objs.size(); j++) {
            if (objs[j]->getName() == s[i]) {
                objs[j]->setTag(i);
            }
        }
    }
    for (auto a : slots) {
        objs_copy.push_back(a);
    }

    math::func::setPositionList(slots, number_obj, Rect(0.05 * 1024, 0.3 * 768, 0.9 * 1024, 0.6 * 768));
    math::func::setPositionList(objs, number_obj, Rect(0.05 * 1024, 0.1 * 768, 0.9 * 1024, 0.4 * 768));
   audio_id_question = AUDIO_ENGINE->play(q_obj.audio_path, false, 1.0f, [=]{
       inActiveMode();
       showTutorial(10.0f);
        for (auto o: objs) {
            o->setTouchEnabled(true);
        }
    });

}

void NhinBongDoanVat::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/NhinBongDoanVat/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/NhinBongDoanVat/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void NhinBongDoanVat::onFinishGameVMonkey(){
//    AUDIO_ENGINE->stopAllEffects();
    MJMainGBase::onFinishGameVMonkey();
}

void NhinBongDoanVat::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void NhinBongDoanVat::onTouchObject(Ref* sender, TouchEventType type ){
    CCLOG("let's go");
    auto obj = (ImageView*)sender;
   auto slot = utils::findChild<ImageView*>(root_layout, StringUtils::format("Slot_%s", obj->getName().c_str()));
    
    CCLOG("ten %s:", obj->getName().c_str());
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            stopGuiding();
             CCLOG("let's begin");
			AUDIO_ENGINE->play(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)));
            pos = obj->getPosition();
            obj->setPosition(root_layout->convertToNodeSpace(obj->getTouchBeganPosition()));
            obj->setLocalZOrder(99);
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            
             CCLOG("let's move");
            obj->setPosition(root_layout->convertToNodeSpace( obj->getTouchMovePosition()));
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            
            CCLOG("let's end");
            if (slot->getBoundingBox().containsPoint(root_layout->convertToNodeSpace(obj->getTouchEndPosition()))) {
                count++;
                AUDIO_ENGINE->play(SOUND_CORRECT);
                for (auto img: imgs) {
                    if (obj->getName() == img.image_path) {
                        AUDIO_ENGINE->stopEffect(audio_id_question);
                        AUDIO_ENGINE->play(img.audio_path);
                        break;
                    }
                }
                
                obj->setPosition(slot->getPosition());
                slot->setColor(_colors[StringUtils::format("Slot_%s", obj->getName().c_str())]);
                obj->setVisible(false);
                obj->setTouchEnabled(false);

                
                for (int i = 0; i < objs_copy.size(); i++) {
                    if (obj->getTag() == objs_copy[i]->getTag()) {
                        objs_copy.erase(objs_copy.begin() + i);
                        stopGuiding();
                        inActiveMode();
                    }
                }
                
                if(count == number_obj){
                    
                    this->scheduleOnce([=](float dt){
                        onFinishGameVMonkey();
                    }, 1.5, "finish_game");
                }
            }else {
                if (count == 0) {
                    stopGuiding();
                    showTutorial(10.0f);
                    inActiveMode();
                }
                //return old pos
                obj->setPosition(pos);
				AUDIO_ENGINE->playSound(StringUtils::format(SOUND_WRONG, cocos2d::random(1, 3)));
            }
             obj->setLocalZOrder(10);
            break;
        default:
            break;
    }
}

void NhinBongDoanVat::radio(){   
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(60, Director::getInstance()->getWinSize().height - 60));
    this->addChild(_speaker);
    _speaker->addClickEventListener([=](Ref*) {
        stopGuiding();
        if (count == 0) {
            showTutorial(10.0f);
        }
        AUDIO_ENGINE->stopSound(q_obj.audio_path);
        audio_id_question = AUDIO_ENGINE->playSound(q_obj.audio_path, false, [=]() {
            inActiveMode();
            }, 1.0f);
        });
}

void NhinBongDoanVat::showGuide(Point start, Point stop) {

    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
    this->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    hand->setName("guidingHand");

    hand->runAction(Sequence::create(MoveTo::create(2, stop), RemoveSelf::create(), nullptr));
}

void NhinBongDoanVat::showTutorial(float dt) {
    Point start, stop;
    if (!objs_copy.empty()) {
        auto a = objs_copy.at(0);
        for (auto o : objs) {
            if (o->getTag() == a->getTag()) {
                auto bb_start = a->getBoundingBox();
                auto bb_stop = o->getBoundingBox();
                stop = Point(bb_start.getMidX(), bb_start.getMidY());
                stop = a->getParent()->convertToWorldSpace(stop);
                start = Point(bb_stop.getMidX(), bb_stop.getMidY());
                start = o->getParent()->convertToWorldSpace(start);
                break;
            }
        }
    }

    this->schedule([=](float) {
        this->showGuide(start, stop);
        }, dt, 100, 10.0f, "show_guide_matching");
}

void NhinBongDoanVat:: inActiveMode() {
    this->schedule([=](float) {
        AUDIO_ENGINE->stopSound(q_obj.audio_path);
        audio_id_question = AUDIO_ENGINE->playSound(q_obj.audio_path);
        }, 10.0f, 100, 10.0f, "inActiveMode");
}

void NhinBongDoanVat::stopGuiding() {
    this->unschedule("show_guide_matching");
    this->unschedule("inActiveMode");
    if (auto hand = utils::findChild<ImageView*>(this, "guidingHand")) {
        hand->stopAllActions();
        hand->removeFromParent();
        AUDIO_ENGINE->stopSound(q_obj.audio_path);
    }
}


