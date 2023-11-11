
#include "WordTv_Tv.h"
#include "HSAudioEngine.h"
#include "PhonicInfoModel.h"
#include "PhonicConfig.h"

WordTv_Tv * WordTv_Tv::createItem(mj::PHONIC_WORD data) {
    WordTv_Tv * obj = new WordTv_Tv();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(data);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void WordTv_Tv::didLoadFromCSB(mj::PHONIC_WORD data) {
    this->_data = data;
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    text = Text::create("",PhonicConfig::getInstance()->getGameFontName(), 100);
    //text->setFontName(PhonicConfig::getInstance()->getGameFontName());
    text->setString(data.text);
    text->setVisible(false);
    text->setTextColor(cocos2d::Color4B::BLACK);
    this->addChild(text,100);
    
    
    tv = GAFWrapper::createGAF("gaf/WordTv/wtv_tv/wtv_tv.gaf");
    this->addChild(tv);
    tv->setPosition(tv->getContentSize()*0.7f/2 + cocos2d::Size(0,100));
    tv->setScale(1.2f);
    text->setPosition(tv->getContentSize()*0.7f/2);
    text->setPositionY(text->getPositionY()+48);
    //tv->setVisible(false);
    this->setContentSize(tv->getContentSize()*0.7f);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    //this->setBackGroundColor(cocos2d::Color3B::RED);
    this->setTouchEnabled(true);
}

void WordTv_Tv::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
}

std::string WordTv_Tv::getColorChar(){
    switch (color) {
        case 0:
            return "r";
        case 1:
            return "g";
        case 2:
            return "y";
        case 3:
            return "b";
        default:
            break;
    }
    return "g";
}

void WordTv_Tv::voice(){
    this->setTouchEnabled(false);
    auto gameNameId = cocos2d::experimental::AudioEngine::play2d(_data.audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(gameNameId, [=](int au_id, std::string path){
        this->setTouchEnabled(true);
    });
}

void WordTv_Tv::playSoundVoice(bool* mybool) {
    if (!(*mybool))
    {
        AUDIO_ENGINE->playSound(_data.audio_path, false, [=] {
            (*mybool) = true;
            }, 1.0f);
    }
    else {
        AUDIO_ENGINE->playSound(_data.audio_path, false, [=] {}, 1.0f);
    }
    
}

void WordTv_Tv::playTv(WORD_TV_STATE state, const std::function<void()> afunc){
    _state = state;
    if (state==WORD_TV_STATE::LOCK) {
        this->tv->playSequence(StringUtils::format("tv%s_locked",this->getColorChar().c_str()),true);
    }
    else if (state == WORD_TV_STATE::ACTIVE){
        this->tv->playSequence(StringUtils::format("tv%s_activated",this->getColorChar().c_str()),true);
    }
    else if (state ==WORD_TV_STATE::ACTION){
        AUDIO_ENGINE->play(StringUtils::format("sounds/WordTv/wtv_action_%s.mp3", this->getColorChar().c_str()));
        this->scheduleOnce([=](float dt){
            text->setVisible(true);
            AUDIO_ENGINE->play(_data.audio_path);
        }, 1.1f,"active_tv");
        this->tv->playSequence(StringUtils::format("tv%s_action",this->getColorChar().c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
            this->playTv(WordTv_Tv::WORD_TV_STATE::IDLE);
            if (afunc)
                afunc();
        });
    }
    else if (state == WORD_TV_STATE::IDLE){
        this->tv->playSequence(StringUtils::format("tv%s_idle",this->getColorChar().c_str()),true);
    }
    else if ( state == WORD_TV_STATE::WRONG){
        this->tv->playSequence(StringUtils::format("tv%s_wrong",this->getColorChar().c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
            this->playTv(WordTv_Tv::WORD_TV_STATE::IDLE);
            if (callBackWrong)
                callBackWrong();
        });
    }
    else if ( state == WORD_TV_STATE::CORRECT){
        this->tv->playSequence(StringUtils::format("tv%s_correct_loop",this->getColorChar().c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
            this->playTv(WordTv_Tv::WORD_TV_STATE::IDLE);
            if (callBackCorrect)
                callBackCorrect();
        });
    }
}

void WordTv_Tv::setCallBackW(const std::function<void()> afunc){
    callBackWrong = afunc;
}

void WordTv_Tv::setCallBackC(const std::function<void()> afunc){
    callBackCorrect = afunc;
}

