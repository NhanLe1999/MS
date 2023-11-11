
#include "GamePhonics.h"
#include "HSAudioEngine.h"
#include "GamePhonics_Monkey.h"
#include "GamePhonics_TextVideo.h"
#include "JsonDump.h"
INITIALIZE_READER(MJGamePhonics);

USING_NS_CC;
std::once_flag phonics_reader;
#define CSB_NAME "csb/game/phonics/GamePhonics.csb"

MJGamePhonics * MJGamePhonics::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(phonics_reader, [] {
        REGISTER_CSB_READER(MJGamePhonics);
        REGISTER_CSB_READER(MJGamePhonics_Monkey);
        REGISTER_CSB_READER(MJGamePhonics_TextVideo);
    });
    auto p = static_cast<MJGamePhonics *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGamePhonics::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_danhvan.plist");
		//auto value_map = FileUtils::getInstance()->getValueMapFromFile("test/79_1_game.plist");
        buffer_string = JsonDump::getInstance()->MapToString(value_map);
    }
    
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    value_data.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    generateData(value_data.GetObject(),images);
    
    auto bg = cocos2d::utils::findChild(this, "bg");
    if (bg->getContentSize().width < Director::getInstance()->getWinSize().width) {
        bg->setScale(Director::getInstance()->getWinSize().width*1.0f/bg->getContentSize().width);
    }
    
}

void MJGamePhonics::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    
    /*auto list_object = value_data["content_info"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGamePhonics_TextVideo::ObjectInfo object;
        auto object_value = list_object[i].GetObject();
        object.text = object_value["text"].GetString();
        object.audio_path = object_value["audio_path"].GetString();
        object.img_path = object_value["img_path"].GetString();
        object.video_path = object_value["video_path"].GetString();
        object.video_duration = object_value["video_duration"].GetFloat();
        object.audio_duration = object_value["audio_duration"].GetFloat();

        game_dataset.list_objects.push_back(object);
    }*/

	auto list_object = value_data["content_info"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGamePhonics_TextVideo::ObjectInfo object;
        CCLOG("i = %d | size = %d", i, (int)list_object.Size());
		auto info = math::resource::ResHelper::getImage(images, list_object[i]["info_common"].GetString());
		object.text = info->getText();
		object.audio_path = info->getAudio();
		//object.img_path = info->path;
		object.video_path = info->path;
		//object.video_duration = info->getVideo().duration;
		object.audio_duration = 4.0f;

		game_dataset.list_objects.push_back(object);
	}
}

void MJGamePhonics::onEnter(){
    ms::BaseActivity::onEnter();
    loadFrames();
    this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
		AUDIO_ENGINE->playEffect("sounds/phonics/gameName.mp3");
		auto imgGameName = cocos2d::ui::ImageView::create("games/phonics/logo.png");
		this->addChild(imgGameName, 1000011);
		imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
		imgGameName->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(2.0f)
			, CallFunc::create([=](){
			onStartGame();
		}), RemoveSelf::create(true), nullptr));
       // onStartGame();
    }), NULL));
}
void MJGamePhonics::loadFrames() {
    setInputConfigGame();
    auto bg = this->getChildByName("bg");
    text_video = MJGamePhonics_TextVideo::createGame(game_dataset.list_objects[0]);
    this->addChild(text_video, 100000);
    text_video->setDelegate(this);
    
    auto layout_mc = cocos2d::ui::Layout::create();
    layout_mc->setContentSize(bg->getContentSize());
    layout_mc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layout_mc->setPosition(bg->getPosition());
    this->addChild(layout_mc, 200000);
    monkey_mc = MJGamePhonics_Monkey::createGame();
    layout_mc->addChild(monkey_mc);
    monkey_mc->setPosition(Vec2(60, 310));
    monkey_mc->setDelegate(this);
    monkey_mc->setScale(0.9);
    monkey_mc->setVisible(false);
}

void MJGamePhonics::setInputConfigGame(){
    mj::helper::shuffle(game_dataset.list_objects);
}

void MJGamePhonics::onStartGame(){
    monkey_mc->setVisible(true);
    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::JUMP);
    AUDIO_ENGINE->playEffect("sounds/phonics/phonics_monkeyjump.mp3");
    
}

//void MJGamePhonics::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(this->getContentSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(this->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(this->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void MJGamePhonics::onFinishGame(){
    this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
        //_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
    }), NULL));
    
}

//FINISH_ACTION_VIDEO
void MJGamePhonics::onFinishAction(int finish_type){
    switch (finish_type) {
        case MJGamePhonics_TextVideo::VIDEO_DONE:
        {
            CCLOG("VIDEO_DONE");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::TALK, true);
            auto audio_path = std::string("sounds/phonics/mokeyphonics_line3.mp3");
            audio_record_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_record_id, [=](int au_id, std::string path){
                if (object_index < 2) {
                    auto audio_path = std::string("sounds/phonics/mokeyphonics_line4.mp3");
                    audio_record_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_record_id, [=](int au_id, std::string path){
                        monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
                        text_video->runLayoutDisplay(text_video->btrecord, true);
                        mj::helper::showTap(text_video->btrecord->getParent()->convertToWorldSpace(text_video->btrecord->getPosition()));
                    });
                }else {
                    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
                    text_video->btrecord->setOpacity(255);
                    text_video->btrecord->setTouchEnabled(true);
                }
            });
        }
            break;
        case MJGamePhonics_TextVideo::RECORD_DONE:
        {
            CCLOG("RECORD_DONE");
            if (object_index < 2) {
                monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::TALK, true);
                auto audio_path = std::string("sounds/phonics/mokeyphonics_line5.mp3");
                audio_record_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_record_id, [=](int au_id, std::string path){
                    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
                });
            }else {
                monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
            }
        }
            break;
            
        case MJGamePhonics_TextVideo::AUDIO_DONE:
            CCLOG("AUDIO_DONE");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::MIC_HIDE);
            
            break;
        case MJGamePhonics_TextVideo::SHOW_TEXT_DONE:
            CCLOG("SHOW_TEXT_DONE");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::POINT_DOWN);
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                text_video->playVideo();
            }), NULL));
            break;
        case MJGamePhonics_TextVideo::HIGH_LIGHT_DONE:
            CCLOG("HIGH_LIGHT_DONE");
            break;
        default:
            break;
    }
    
}

//FINISH_ACTION_MONKEY
void MJGamePhonics::onFinishMonkey(int finish_type){
    switch (finish_type) {
        case MJGamePhonics_Monkey::JUMP:
        {
            CCLOG("MONKEY: JUMP_DONE");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::TALK, true);
            auto audio_path = std::string("sounds/phonics/mokeyphonics_line1.mp3");
            auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                auto audio_path2 = std::string("sounds/phonics/mokeyphonics_line2.mp3");
                auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path2);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::POINT_UP);
                    AUDIO_ENGINE->playEffect("sounds/phonics/phonics_huzz_pop.mp3");
                    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                        text_video->showText();
                    }), NULL));
                });
            });
        }
            
        break;
            
        case MJGamePhonics_Monkey::POINT_UP:
        {
            CCLOG("MONKEY: POINT_UP");
        }
            
            break;
        case MJGamePhonics_Monkey::POINT_DOWN:
        {
            CCLOG("MONKEY: POINT_DOWN");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
            
        }
            break;
        
        case MJGamePhonics_Monkey::LISTEN_HAND:
        {
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::LISTEN, true);
        }
            break;
            
        case MJGamePhonics_Monkey::MIC_SHOW:
        {
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::READ, true);
        }
            break;
            
        case MJGamePhonics_Monkey::MIC_HIDE:
        {
            CCLOG("MONKEY: MIC_HIDE");
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
        }
            break;
            
        default:
            break;
    }
}

void MJGamePhonics::onRecordClicked() {
    mj::helper::hideTap();
    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::LISTEN_HAND);
    AUDIO_ENGINE->playEffect("sounds/phonics/phonics_huzz_pop.mp3");
}
void MJGamePhonics::onPlayClicked() {
    monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::MIC_SHOW);
    AUDIO_ENGINE->playEffect("sounds/phonics/phonics_huzz.mp3");

}

void MJGamePhonics::onNextTurnClicked(){
    text_video->hideText();
    object_index++;
    if ((int)game_dataset.list_objects.size() == object_index) {
        monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::TALK, true);
        auto audio_path = std::string("sounds/phonics/mokeyphonics_line6.mp3");
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::IDLE, true);
            monkey_mc->setVisible(false);
            this->onFinishGame();
        });
    }else{
        monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::TALK, true);
        auto audio_path2 = std::string("sounds/phonics/mokeyphonics_line2.mp3");
        auto audio_id2 = cocos2d::experimental::AudioEngine::play2d(audio_path2);
        CCLOG("audio_id2 = %d", audio_id2);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id2, [=](int au_id, std::string path){
            if (!game_play_enable) {
                this->pauseGame();
                monkey_mc->gaf_monkey->pauseAnimation();
                return;
            }
            monkey_mc->playMonkeyWithAction(MJGamePhonics_Monkey::POINT_UP);
            CCLOG("object_index = %d", object_index);
            text_video->resetTextVideoData(game_dataset.list_objects[object_index]);
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                text_video->showText();
            }), NULL));
        });
    }
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGamePhonics::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGamePhonics::onRefresh, this);
    }
    return nullptr;
}

void MJGamePhonics::onRefresh(cocos2d::Ref *sender) {
    /*this->removeFromParent();
    auto phonics = MJGamePhonics::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
    
}

void MJGamePhonics::onCloseGame(){
    text_video->deleteLayerLock();
    text_video->removeVideo();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
}

void MJGamePhonics::onExitTransitionDidStart(){
    ms::BaseActivity::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}
