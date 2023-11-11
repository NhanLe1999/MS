#include "HouseOfWord.h"
#include "AspectR.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <audio/include/AudioEngine.h>
#include "json/document.h"
#include "json/stringbuffer.h"
#include "../external/json/prettywriter.h"
USING_NS_CC;
using namespace houseofwords;
using namespace rapidjson;

HouseOfWord* HouseOfWord::createGame(std::vector<math::resource::Image*> images)
{
    HouseOfWord* obj = new HouseOfWord();
    if(obj->init())
    {
        obj->autorelease();
        obj->initOptions(images);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}


#include "MJDefault.h"
#define key_count_guide_house_of_words "key_count_guide_house_of_words"

void HouseOfWord::initOptions(std::vector<math::resource::Image*> images)
{
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_house_of_words, 0);
    _is_guide_showed = !(count < 3);
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guide_house_of_words, count + 1);
    }, 20, "delay_update_count_guide_house_of_words");
    
    json_path = "config.json";
    
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        cocos2d::experimental::AudioEngine::play2d("sounds/house_of_words/ui/bg.mp3", true, 0.3);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(Size(visibleSize.width,visibleSize.height));

    //assign
    _allLevels = loadLevelData(images);

    //background///////////
    _background = ImageView::create("games/house_of_words/bg.png");
    _background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_background->setScale(visibleSize.width * 2 / _background->getContentSize().width);
    _background->setPosition(Vec2(-5, 0));
    _background->setTouchEnabled(false);
    _background->addClickEventListener([=](Ref*) {
        CCLOG("touch began");
        if (_enableTouchImage)
        {
            if (!_allLevels.empty())
                schedulePlaySoundGuiding();
        }
        }
        );
    this->addChild(_background);
    ///////////////////////

    _cloud1 = ImageView::create("games/house_of_words/cloud.png");
    _cloud1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _cloud1->setPosition(Vec2(0,AspectR::getHeight(700)));
    _cloud1->setTouchEnabled(false);
    _cloud1->addClickEventListener([=](Ref*) {
        CCLOG("touch began");
        if (_enableTouchImage)
        {
            if (!_allLevels.empty())
                schedulePlaySoundGuiding();

        }
        });
    this->addChild(_cloud1);

    _cloud2 = ImageView::create("games/house_of_words/cloud.png");
    _cloud2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _cloud2->setPosition(Vec2(_cloud1->getContentSize().width,AspectR::getHeight(700)));
    _cloud2->setTouchEnabled(false);
    _cloud2->addClickEventListener([=](Ref*) {
        CCLOG("touch began");
        if (_enableTouchImage)
        {
            if (!_allLevels.empty())
                schedulePlaySoundGuiding();
        }
        }
        );
    this->addChild(_cloud2);

    this->schedule(schedule_selector(HouseOfWord::scrollCloud));

    auto title = ImageView::create("games/house_of_words/title.png");
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY()));
    title->addClickEventListener([=](Ref*) {
        CCLOG("touch began");
        if (!_allLevels.empty())
            schedulePlaySoundGuiding();
        });
    this->addChild(title);

    _infoBar = Layout::create();
    _infoBar->setBackGroundImage("games/house_of_words/info.png");
    _infoBar->setBackGroundImageScale9Enabled(true);
    _infoBar->setContentSize(Size(AspectR::getWidth(450), AspectR::getHeight(120)));
    _infoBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _infoBar->setPosition(Vec2(AspectR::getWidth(30), AspectR::getHeight(768 - 30)));
    _infoBar->setCascadeOpacityEnabled(true);
    _infoBar->setOpacity(0);
    _infoBar->setTouchEnabled(true);
    _infoBar->addClickEventListener([=](Ref*) {
        CCLOG("touch began");
        if (!_allLevels.empty())
            schedulePlaySoundGuiding();
        });
    this->addChild(_infoBar);

    _lightButton = ImageView::create("games/house_of_words/light.png");
    _lightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lightButton->setPosition(Vec2(_infoBar->getPositionX() + _lightButton->getContentSize().width/2, _infoBar->getPositionY() - _infoBar->getContentSize().height/2));
    _lightButton->setTouchEnabled(true);
    _lightButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (!canTouch) return;
        if (type == Widget::TouchEventType::ENDED) {
            _infoBar->runAction(FadeIn::create(0.5));
            cocos2d::experimental::AudioEngine::stopAll();
            _infoAudioInt = cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].infoAudio);
            schedulePlaySoundGuiding();
        }
    });
    _lightButton->setVisible(false);
    this->addChild(_lightButton);

    _lightButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(RandomHelper::random_real(2.0f,5.0f)), RotateTo::create(0.2,-10), RotateTo::create(0.2,10), RotateTo::create(0.2,0), nullptr)));

    auto infoTxt = Label::createWithTTF(_allLevels[_currentIndex].text_info, "fonts/KGWHATTHETEACHERWANTS.TTF", AspectR::getFontSize(20));
    infoTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    infoTxt->setTextColor(Color4B::BLACK);
    infoTxt->setDimensions(_infoBar->getContentSize().width - _lightButton->getContentSize().width - AspectR::getWidth(20), _infoBar->getContentSize().height - AspectR::getHeight(10));
    infoTxt->setPosition(Vec2(_lightButton->getContentSize().width + AspectR::getWidth(10), _infoBar->getContentSize().height/2));
    infoTxt->setAlignment(TextHAlignment::LEFT,TextVAlignment::CENTER);
    infoTxt->setName("infoTxt");
    _infoBar->addChild(infoTxt);

    _question = makeQuestion();
    _question->setContentSize(this->getContentSize());
    _question->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _question->setPosition(Vec2(AspectR::getWidth(1024),0));
    this->addChild(_question);

	auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/house_of_words/gameName.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		//_background->runAction(Sequence::create(DelayTime::create(2), MoveTo::create(1, Vec2(AspectR::getWidth(-1024), 0)), nullptr));
		title->runAction(Sequence::create(DelayTime::create(1.0f), MoveTo::create(0.5f, Vec2(AspectR::getMidX() - AspectR::getWidth(1024), title->getPositionY())), nullptr));
		_question->runAction(Sequence::create(DelayTime::create(2), MoveTo::create(1, Vec2(0, 0)), CallFunc::create([=]{
			//canTouch = true;
            if (!_is_guide_showed) {
                _is_guide_showed = true;
                //mj::helper::showTap(_guide_point);
            }
		}), nullptr));
	});
}

void HouseOfWord::onEnter(){
    ms::BaseActivity::onEnter();
}

//void HouseOfWord::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::Director::getInstance()->getWinSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

vector<houseofwords::LevelData> HouseOfWord::loadLevelData(std::vector<math::resource::Image*> images)
{
    Document document;
    document.Parse(FileUtils::getInstance()->getStringFromFile(json_path).c_str());


#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    vector<houseofwords::LevelData> totalLevels;
    
    //overall array
    auto levelArray = document["data"].GetArray();

    for (auto i = 0; i < levelArray.Size(); i++) {
        auto level = houseofwords::LevelData();

        if (JSON_SAFE_CHECK(levelArray[i], "custom_question", String)){
            auto custom_question = math::resource::ResHelper::getImage(images, levelArray[i]["custom_question"].GetString());
            if (custom_question)
            {
                level.ques = custom_question->getText();
                level.quesAudio = custom_question->getAudio();
            }
            else {
                level.ques = houseofwords::QUESTION_TEXT_DEFAULT.at(GHW_CHARACTERS).c_str();
                level.quesAudio = houseofwords::QUESTION_AUDIO_PATH_DEFAULT.at(GHW_CHARACTERS).c_str();
            }
        }
        else {
            level.ques = houseofwords::QUESTION_TEXT_DEFAULT.at(GHW_CHARACTERS).c_str();
            level.quesAudio = houseofwords::QUESTION_AUDIO_PATH_DEFAULT.at(GHW_CHARACTERS).c_str();
        }

        if (JSON_SAFE_CHECK(levelArray[i], "main_image", String)) {
            auto main_image = math::resource::ResHelper::getImage(images, levelArray[i]["main_image"].GetString());
            if(main_image)          
                level.mainImage = main_image->path;
        }

        if (JSON_SAFE_CHECK(levelArray[i], "char_image", String)) {
            auto char_image = math::resource::ResHelper::getImage(images, levelArray[i]["char_image"].GetString());
            if(char_image) level.charImage = char_image->path;
        }

        if (JSON_SAFE_CHECK(levelArray[i], "read_audio_suggestions", Bool)) {
            auto enableSuggestion = levelArray[i]["read_audio_suggestions"].GetBool();
            if (enableSuggestion)
            {               
                if (JSON_SAFE_CHECK(levelArray[i], "custom_suggestions", String)) {
                    auto custom_suggestions = math::resource::ResHelper::getImage(images, levelArray[i]["custom_suggestions"].GetString());
                    if (custom_suggestions)
                    {
                        level.text_info = custom_suggestions->getText();
                        level.infoAudio = custom_suggestions->getAudio();
                        _enableSugesstion = true;
                    }
                } 
                else {
                    level.text_info = houseofwords::INSTRUCTION_TEXT_DEFAULT.at(GHW_CHARACTERS).c_str();
                    level.infoAudio = houseofwords::INSTRUCTION_AUDIO_PATH_DEFAULT.at(GHW_CHARACTERS).c_str();
                    _enableSugesstion = true;
                }
            }
        }
        level.answer = 0;

		auto true_ans_info = math::resource::ResHelper::getImage(images, levelArray[i]["true_ans_info"].GetString());
		std::vector<std::string> datalevel;
		datalevel.push_back(true_ans_info->getText());
		datalevel.push_back(true_ans_info->getAudio());
		level.options.push_back(datalevel);

        for (auto& v : levelArray[i]["answer_info"].GetArray()) {
			auto answer_info = math::resource::ResHelper::getImage(images, v.GetString());
			std::vector<std::string> datalevel1;
			datalevel1.push_back(answer_info->getText());
			datalevel1.push_back(answer_info->getAudio());
            level.options.push_back(datalevel1);
        }
		mj::helper::shuffle(level.options);

		for (int j = 0; j < level.options.size(); j++){
			if (true_ans_info->getText().compare(level.options.at(j).at(0)) == 0){
				level.answer = j;
			}
		}
        totalLevels.push_back(level);
    }
    return totalLevels;
}

void HouseOfWord::scrollCloud(float dt) {
    _cloud1->setPosition( Vec2(_cloud1->getPosition().x-1, _cloud1->getPosition().y) );
    _cloud2->setPosition( Vec2(_cloud2->getPosition().x-1, _cloud2->getPosition().y) );

    if( _cloud1->getPosition().x < -_cloud1->getBoundingBox().size.width){
        _cloud1->setPosition( Vec2( _cloud2->getPosition().x + _cloud2->getBoundingBox().size.width, _cloud1->getPosition().y));
    }
    if( _cloud2->getPosition().x < -_cloud2->getBoundingBox().size.width){
        _cloud2->setPosition( Vec2( _cloud1->getPosition().x + _cloud1->getBoundingBox().size.width, _cloud2->getPosition().y));
    }
}

Layout* HouseOfWord::makeQuestion()
{
    auto holder = Layout::create();

    auto frame = Sprite::create("games/house_of_words/frame.png");
    frame->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    frame->setPosition(Vec2(AspectR::getMidX() + AspectR::getWidth(35), AspectR::getHeight(50)));
    holder->addChild(frame);

   auto image = ImageView::create(_allLevels[_currentIndex].mainImage);
    CCLOG("image = %s", _allLevels[_currentIndex].mainImage.c_str());
    image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    image->setPosition(Vec2((frame->getContentSize().width/2), (frame->getContentSize().height/2) + AspectR::getHeight(48)));
    image->setScale(1.8);
    float scalex = image->getScaleX();
    float scaley = image->getScaleY();
    if (image->getBoundingBox().size.width + 20 > frame->getBoundingBox().size.width)
    {
        while (image->getBoundingBox().size.width + 20 > frame->getBoundingBox().size.width) {
            scalex = scalex - 0.01;
            image->setScaleX(scalex);
        }
        while (image->getBoundingBox().size.height > (frame->getBoundingBox().size.height - AspectR::getHeight(115))) {
            scaley = scaley - 0.01;
            image->setScaleY(scaley);
        }
    }

    image->setTouchEnabled(true);
    image->addTouchEventListener(CC_CALLBACK_2(HouseOfWord::onTouchImageAndWords, this));
    frame->addChild(image);



    auto house = ImageView::create("games/house_of_words/house.png");
    house->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    house->setPosition(Vec2(AspectR::getMidX(), AspectR::getHeight(50)));
    holder->addChild(house);

    auto quesTxt = Label::createWithTTF(_allLevels[_currentIndex].ques, "fonts/KGWHATTHETEACHERWANTS.TTF", AspectR::getFontSize(24));
    quesTxt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    quesTxt->setTextColor(Color4B::WHITE);
    //quesTxt->setPosition(Vec2(house->getContentSize().width/2, AspectR::getHeight(615)));
    quesTxt->setDimensions(AspectR::getWidth(250),AspectR::getHeight(200));
    quesTxt->setAlignment(TextHAlignment::CENTER,TextVAlignment::CENTER);
    _bound = Layout::create();
    _bound->setContentSize(quesTxt->getContentSize());
    _bound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bound->setTouchEnabled(false);
    _bound->setPosition(Vec2(house->getContentSize().width / 2, AspectR::getHeight(615)));
    _bound->addClickEventListener([=](Ref*) {
        if (_enableTouchImage)
        {
            if (_allLevels.size() != 0)
                cocos2d::experimental::AudioEngine::stopAll();
            cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].quesAudio);
            schedulePlaySoundGuiding();
        }
        }
        );
    _bound->addChild(quesTxt);
    _bound->setZOrder(99999);
    house->addChild(_bound);

    auto option1 = ImageView::create("games/house_of_words/window.png");
    //option1->setScale9Enabled(true);
    option1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option1->setPosition(Vec2(house->getContentSize().width/2,AspectR::getHeight(430)));
    option1->setTouchEnabled(true);
    option1->addTouchEventListener(CC_CALLBACK_2(HouseOfWord::onAnswerClick, this));
    option1->setTag(0);
    house->addChild(option1);
    if (_allLevels[_currentIndex].answer == 0) {
        auto bb = option1->getBoundingBox();
        _guide_point = Point(bb.origin.x + bb.size.width * 0.8, bb.getMidY());
        _guide_point = house->convertToWorldSpace(_guide_point);
    }

    auto overlay1 = Layout::create();
    overlay1->setBackGroundColorType(BackGroundColorType::SOLID);
    overlay1->setBackGroundColor(Color3B::RED);
    overlay1->setContentSize(option1->getContentSize());
    overlay1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    overlay1->setPosition(Vec2(option1->getContentSize().width/2,option1->getContentSize().height/2));
    overlay1->setOpacity(0);
    overlay1->setName("overlay");
    option1->addChild(overlay1);

    auto option1Caption = Label::createWithTTF(_allLevels[_currentIndex].options[0][0], "fonts/KGWHATTHETEACHERWANTS.TTF", AspectR::getFontSize(21));
    option1Caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option1Caption->setPosition(Vec2(option1->getContentSize().width/2,option1->getContentSize().height/2));
    option1Caption->setTextColor(Color4B::BLACK);
    option1Caption->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    option1Caption->setDimensions(option1->getContentSize().width - AspectR::getWidth(6), option1->getContentSize().height - AspectR::getHeight(6));
    option1->addChild(option1Caption);

    auto option2 = ImageView::create("games/house_of_words/window.png");
    option2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option2->setPosition(Vec2(house->getContentSize().width/2,AspectR::getHeight(270)));
    option2->setTouchEnabled(true);
    option2->addTouchEventListener(CC_CALLBACK_2(HouseOfWord::onAnswerClick, this));
    option2->setTag(1);
    house->addChild(option2);
    if (_allLevels[_currentIndex].answer == 1) {
        auto bb = option2->getBoundingBox();
        _guide_point = Point(bb.origin.x + bb.size.width * 0.8, bb.getMidY());
        _guide_point = house->convertToWorldSpace(_guide_point);
    }

    auto overlay2 = Layout::create();
    overlay2->setBackGroundColorType(BackGroundColorType::SOLID);
    overlay2->setBackGroundColor(Color3B::RED);
    overlay2->setContentSize(option2->getContentSize());
    overlay2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    overlay2->setPosition(Vec2(option2->getContentSize().width/2,option2->getContentSize().height/2));
    overlay2->setOpacity(0);
    overlay2->setName("overlay");
    option2->addChild(overlay2);

    auto option2Caption = Label::createWithTTF(_allLevels[_currentIndex].options[1][0], "fonts/KGWHATTHETEACHERWANTS.TTF", AspectR::getFontSize(21));
    option2Caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option2Caption->setPosition(Vec2(option2->getContentSize().width/2,option2->getContentSize().height/2));
    option2Caption->setTextColor(Color4B::BLACK);
    option2Caption->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    option2Caption->setDimensions(option2->getContentSize().width - AspectR::getWidth(6), option2->getContentSize().height - AspectR::getHeight(6));
    option2->addChild(option2Caption);

    auto option3 = ImageView::create("games/house_of_words/window.png");
    option3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option3->setPosition(Vec2(house->getContentSize().width/2,AspectR::getHeight(120)));
    option3->setTouchEnabled(true);
    option3->addTouchEventListener(CC_CALLBACK_2(HouseOfWord::onAnswerClick, this));
    option3->setTag(2);
    house->addChild(option3);
    if (_allLevels[_currentIndex].answer == 2) {
        auto bb = option3->getBoundingBox();
        _guide_point = Point(bb.origin.x + bb.size.width * 0.8, bb.getMidY());
        _guide_point = house->convertToWorldSpace(_guide_point);
    }

    auto overlay3 = Layout::create();
    overlay3->setBackGroundColorType(BackGroundColorType::SOLID);
    overlay3->setBackGroundColor(Color3B::RED);
    overlay3->setContentSize(option3->getContentSize());
    overlay3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    overlay3->setPosition(Vec2(option3->getContentSize().width/2,option3->getContentSize().height/2));
    overlay3->setOpacity(0);
    overlay3->setName("overlay");
    option3->addChild(overlay3);

    auto option3Caption = Label::createWithTTF(_allLevels[_currentIndex].options[2][0], "fonts/KGWHATTHETEACHERWANTS.TTF", AspectR::getFontSize(21));
    option3Caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    option3Caption->setPosition(Vec2(option3->getContentSize().width/2,option3->getContentSize().height/2));
    option3Caption->setTextColor(Color4B::BLACK);
    option3Caption->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    option3Caption->setDimensions(option3->getContentSize().width - AspectR::getWidth(6), option3->getContentSize().height - AspectR::getHeight(6));
    option3->addChild(option3Caption);

    this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create([=](){
        auto snd = cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].quesAudio);
        quesTxt->setTextColor(Color4B::RED);
        cocos2d::experimental::AudioEngine::setFinishCallback(snd, [=](int iVal, const std::string str){
            CCLOG("PLAY LATE ---- ----- >>>");
            quesTxt->setTextColor(Color4B::WHITE);
           
            PlayLate(0, {option1Caption, option2Caption, option3Caption});
        });
        /*float delay = cocos2d::experimental::AudioEngine::getDuration(snd);
        this->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=](){
            quesTxt->setTextColor(Color4B::WHITE);
            _lightButton->setVisible(true);
            PlayLate(0, {option1Caption, option2Caption, option3Caption});
        }), nullptr));*/
        }), nullptr));

    return holder;
}

void HouseOfWord::PlayLate(int index, vector<Label*> answers) {
    if(_allLevels[_currentIndex].options.size()<=index) {
        canTouch = true;
        return;
    }
    this->schedulePlaySoundGuiding();
    auto snd = cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].options[index][1]);
    answers[index]->setTextColor(Color4B::RED);
    cocos2d::experimental::AudioEngine::setFinishCallback(snd, [=](int iVal, const std::string str){
        CCLOG("PLAY LATE ---- ----- >>>");
        answers[index]->setTextColor(Color4B::BLACK);
        PlayLate(index + 1, answers);
		if (index + 1 == _allLevels[_currentIndex].options.size()){
            if(_enableSugesstion) _lightButton->setVisible(true);
            _enableTouchImage = true;
            _background->setTouchEnabled(true);
            _cloud1->setTouchEnabled(true);
            _cloud2->setTouchEnabled(true);
            _bound->setTouchEnabled(true);
            this->schedulePlaySoundGuiding();
			CCLOG("%s", _allLevels[_currentIndex].text_info.c_str());
            ((Label*)_infoBar->getChildByName("infoTxt"))->setString(StringUtils::toString(_allLevels[_currentIndex].text_info));
		}
    });
    /*float delay = cocos2d::experimental::AudioEngine::getDuration(snd);
    CCLOG("DELAY = %f",delay);
    this->runAction(Sequence::create(DelayTime::create(delay),CallFunc::create([=](){
        answers[index]->setTextColor(Color4B::BLACK);
        PlayLate(index + 1, answers);
    }), nullptr));*/
}

void HouseOfWord::schedulePlaySoundGuiding()
{
    CCLOG("start schedule");
    this->unschedule("PlaySoundGuiding");
    schedule([=](float dt) 
        {
            if (cocos2d::experimental::AudioEngine::getPlayingAudioCount() == 0)
            {
                int x = cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].quesAudio);
                cocos2d::experimental::AudioEngine::setFinishCallback(x, [=](int asd, const std::string& dsa) {
                    schedulePlaySoundGuiding();
                    });
            }
        }, 11.f, 100, 11.f, "PlaySoundGuiding");
}


void HouseOfWord::onAnswerClick(Ref *sender, ui::Widget::TouchEventType event) {
    this->schedulePlaySoundGuiding();
    if (!canTouch) return;
        if (event == Widget::TouchEventType::ENDED) {

            //stop info audio
            cocos2d::experimental::AudioEngine::stop(_infoAudioInt);
            _lightButton->setTouchEnabled(true);

            auto layout = (ImageView*) sender;
            if (layout->getTag() == _allLevels[_currentIndex].answer) {
                mj::helper::hideTap();
                this->unschedule("PlaySoundGuiding");
                //window
                const string windowPath = "gaf/house_of_words/Window_large/Window_large.gaf";
                GAFAsset* windowAsset = GAFAsset::create(windowPath);
                GAFObject* window = windowAsset->createObject();
                window->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                window->setPosition(Vec2(layout->getPositionX() - AspectR::getWidth(8), layout->getPositionY() - AspectR::getHeight(8)));
                layout->getParent()->addChild(window);
                window->start();

                //masking
                auto stencil = DrawNode::create();
                stencil->drawSolidRect(Vec2(-layout->getContentSize().width / 2, -layout->getContentSize().height / 2), Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2), Color4F::RED);
                stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                stencil->setPosition(layout->getPosition());

                auto clipper = ClippingNode::create();
                clipper->setStencil(stencil);

                //auto charImg = Sprite::create(_allLevels[_currentIndex].charImage);
                std::string imagePath;
                if (cocos2d::FileUtils::getInstance()->isFileExist("games/house_of_words/tickV.png")) {
                    imagePath = "games/house_of_words/tickV.png";
                }
                else {
                    imagePath = _allLevels[_currentIndex].charImage;
                }
                auto charImg = Sprite::create(imagePath);
                charImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                charImg->setPosition(Vec2(layout->getPositionX(), -charImg->getContentSize().height / 2));
                charImg->setScale(0.1);
                clipper->addChild(charImg);

                layout->getParent()->addChild(clipper);
                /////////

                //tick
                //const string tickPath = "gaf/house_of_words/tick/tick.gaf";
                //GAFAsset *tickAsset = GAFAsset::create(tickPath);
                //GAFObject *tick = tickAsset->createObject();
                //tick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                //tick->setPosition(Vec2( layout->getPositionX() + layout->getContentSize().width / 2, layout->getPositionY() + layout->getContentSize().height/2));
                ////layout->getParent()->addChild(tick);

                this->runAction(Sequence::create(DelayTime::create(0.3),
                    CallFunc::create([=] {
                        charImg->setPosition(charImg->getPositionX(), layout->getPositionY());
                        showNextLevel();
                        })
                    , nullptr));
                cocos2d::experimental::AudioEngine::stopAll();
                auto sound_id = cocos2d::experimental::AudioEngine::play2d("sounds/house_of_words/ui/correct.mp3");
                cocos2d::experimental::AudioEngine::setFinishCallback(sound_id, ([=](int au_id, std::string path) {                 
                    this->unschedule("PlaySoundGuiding");
                    }));
            } else {
                auto overlay = (Layout*)layout->getChildByName("overlay");
                overlay->runAction(Repeat::create(Sequence::create(FadeTo::create(0.2,128),FadeTo::create(0.2,0), nullptr),3));
                layout->runAction(Sequence::create(Repeat::create(Sequence::create(RotateTo::create(0.1,-2),RotateTo::create(0.1,2),RotateTo::create(0.1,0), nullptr),3), CallFunc::create([=](){
                    canTouch = true;
                }), nullptr));
                cocos2d::experimental::AudioEngine::play2d("sounds/house_of_words/ui/wrong.mp3");
            }

            canTouch = false;
        }

}

void HouseOfWord::onTouchImageAndWords(Ref* sender, ui::Widget::TouchEventType event)
{
    if (event == ui::Widget::TouchEventType::ENDED)
    {
        if (_enableTouchImage)
        {
            cocos2d::experimental::AudioEngine::stopAll();
            cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].quesAudio);
        }
        schedulePlaySoundGuiding();
    }  
}

void HouseOfWord::showNextLevel()
{
    if (!game_play_enable) {
        this->pauseGame();
        return;
    }
    
    _currentIndex++;
    _lightButton->setVisible(false);
    _enableTouchImage = false;
    _infoBar->setOpacity(0);
    this->unschedule("PlaySoundGuiding");

    if(_currentIndex >= (int)_allLevels.size()) {
		this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
            cocos2d::experimental::AudioEngine::stopAll();
			commonEndGame(ms::BaseActivity::EGT_COMMON);
		}), NULL));
        return;
    }

    auto tempQuestion = makeQuestion();
    tempQuestion->setContentSize(this->getContentSize());
    tempQuestion->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    tempQuestion->setPosition(Vec2(AspectR::getWidth(1024),0));
    this->addChild(tempQuestion);

    float dd = 1;
	
    tempQuestion->runAction(Sequence::create(DelayTime::create(dd), MoveTo::create(1,Vec2(0,0)), nullptr));
    //_background->runAction(Sequence::create(DelayTime::create(dd), MoveTo::create(1,Vec2(AspectR::getWidth(-1024),0)), nullptr));
    _question->runAction(Sequence::create(DelayTime::create(dd), MoveTo::create(1,Vec2(AspectR::getWidth(-1024),0)), CallFunc::create([=](){
        this->removeChild(_question); 
        _question = tempQuestion;
//        canTouch = true;
    }), nullptr));
}
