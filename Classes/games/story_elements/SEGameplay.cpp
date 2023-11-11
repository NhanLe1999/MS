//
//  SEGameplay.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#include "SEGameplay.h"
#include "Constants.h"
#include "AspectR.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include <audio/include/AudioEngine.h>
#include <ctime>
#include "Math_Libs.h"
#include "HSAudioEngine.h"

using namespace story_elements;
using namespace rapidjson;
using namespace cocos2d::experimental;

GamePlay* GamePlay::createGame(std::vector<math::resource::Image*> images)
{
    GamePlay* obj = new GamePlay();
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
#define key_count_guide_story_elements "key_count_guide_story_elements"

void GamePlay::initOptions(std::vector<math::resource::Image*> images) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_story_elements, 0);
    
    this->scheduleOnce([](float) {
        auto count = MJDEFAULT->getIntegerForKey(key_count_guide_story_elements, 0);
        MJDEFAULT->setIntegerForKey(key_count_guide_story_elements, count + 1);
    }, 15, "update_count_guide_story_elements");
    
    _json_file = "config.json";
    
    list_game_info = {
        {"Main Character", "The main character is the most important person or animal in the story.", "story_elements/instructions/info1.mp3"},
        {"Problem", "The problem is what goes wrong in the story.", "story_elements/instructions/info2.mp3"},
        {"Setting", "The setting of the story is where and when the story happens.", "story_elements/instructions/info3.mp3"},
        {"Solution", "The solution is how the problem gets solved.", "story_elements/instructions/info4.mp3"},
        {"Supporting character", "The supporting character is a person or animal that is less important to the story.", "story_elements/instructions/info5.mp3"},
    };
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AudioEngine::play2d("story_elements/music_bg.mp3", true);
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(Size(visibleSize.width,visibleSize.height));

    //assign
    _levelData = loadLevelData(images);

    //background///////////
    auto background = Sprite::create("games/story_elements/bg.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(AspectR::getMidX(),AspectR::getMidY());
	if (background->getContentSize().width<Director::getInstance()->getVisibleSize().width){
		background->setScale(Director::getInstance()->getVisibleSize().width / background->getContentSize().width);
	}
    this->addChild(background);
    ///////////////////////

    //game name///////////
    auto gameName = Sprite::create("games/story_elements/name.png");
    gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gameName->setPosition(AspectR::getMidX(),AspectR::getMidY());
    this->addChild(gameName);
    ///////////////////////

    _bookImage = ImageView::create("games/story_elements/book.png");
    _bookImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bookImage->setScale(0.9);
    _bookImage->setPosition(Vec2(AspectR::getMidX() + AspectR::getWidth(80), AspectR::getMidY() - _bookImage->getContentSize().height - AspectR::getHeight(768)));
    this->addChild(_bookImage);

    auto titleTxt = Text::create();
    titleTxt->setString(story_name);
    titleTxt->setFontName("fonts/KGWHATTHETEACHERWANTS.TTF");
    titleTxt->setFontSize(22);
    titleTxt->setScale(1.5);
    titleTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    titleTxt->setTextColor(Color4B::BLACK);
    titleTxt->setPosition(Vec2((_bookImage->getContentSize().width / 2)/2, AspectR::getHeight(600)));
    titleTxt->setTextHorizontalAlignment(TextHAlignment::CENTER);
    titleTxt->setTextVerticalAlignment(TextVAlignment::CENTER);
    titleTxt->setOpacity(255);
    titleTxt->ignoreContentAdaptWithSize(false);
    titleTxt->setContentSize(Size(_bookImage->getContentSize().width / 2 - _bookImage->getContentSize().width / 4, titleTxt->getContentSize().height*2.5));
    //titleTxt->setContentSize(Size(50, 50));
    _bookImage->addChild(titleTxt);

    //Main Character//////////////////
    auto frame1 = ImageView::create("games/story_elements/main_char_frame.png");
    frame1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame1->setPosition(Vec2((_bookImage->getContentSize().width / 2)/2, AspectR::getHeight(450)));
    frame1->setOpacity(0);
    frame1->setCascadeOpacityEnabled(true);
    frame1->setTag(0);
    _bookImage->addChild(frame1);

    auto frame1_top = ImageView::create("games/story_elements/main_char_frame_top.png");
    frame1_top->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame1_top->setPosition(Vec2(frame1->getContentSize().width/2, frame1->getContentSize().height));
    frame1_top->setCascadeOpacityEnabled(true);
    frame1_top->setName("top");
    frame1->addChild(frame1_top);

    auto frame1_title = Label::createWithTTF(list_game_info[0].title, "fonts/KGWHATTHETEACHERWANTS.TTF",22);
    frame1_title->setScale(1.4);
    frame1_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame1_title->setTextColor(Color4B::BLACK);
    frame1_title->setPosition(Vec2((frame1_top->getContentSize().width/2) - AspectR::getWidth(15), frame1_top->getContentSize().height/2));
    frame1_title->setAlignment(TextHAlignment::CENTER);
    frame1_top->addChild(frame1_title);
	
    auto frame1_info = ImageView::create("games/story_elements/icon_question.png");
    frame1_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame1_info->setPosition(Vec2(frame1_top->getContentSize().width - AspectR::getWidth(20),
                                  frame1_top->getContentSize().height - AspectR::getHeight(20)));
    frame1_top->addChild(frame1_info);

    auto frame1_info_bar = ImageView::create("games/story_elements/main_char_dialog.png");
    frame1_info_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame1_info_bar->setPosition(Vec2(frame1->getContentSize().width/2, frame1->getContentSize().height - AspectR::getHeight(45)));
    frame1_info_bar->setVisible(false);
    frame1_info_bar->setName("info_bar");
    frame1_info_bar->setScale9Enabled(true);
    frame1_info_bar->setContentSize(Size(AspectR::getWidth(200), AspectR::getHeight(150)));
    frame1->addChild(frame1_info_bar);

    auto frame1_info_bar_txt = Label::createWithTTF(list_game_info[0].text, "fonts/KGWHATTHETEACHERWANTS.TTF", 23);
    frame1_info_bar_txt->setScale(1);
    frame1_info_bar_txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame1_info_bar_txt->setTextColor(Color4B::BLACK);
    frame1_info_bar_txt->setPosition(Vec2(frame1_info_bar->getContentSize().width/2, (frame1_info_bar->getContentSize().height/2) - AspectR::getHeight(5)));
    frame1_info_bar_txt->setDimensions(frame1_info_bar->getContentSize().width - AspectR::getWidth(10), frame1_info_bar->getContentSize().height - AspectR::getHeight(20));
    frame1_info_bar_txt->setAlignment(TextHAlignment::LEFT,TextVAlignment::TOP);
    frame1_info_bar->addChild(frame1_info_bar_txt);

    _holder.push_back(frame1);
    /////////////////////////////////

    //Problem//////////////////
    auto frame2 = ImageView::create("games/story_elements/problem_frame.png");
    frame2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame2->setPosition(Vec2((_bookImage->getContentSize().width / 2)/2, AspectR::getHeight(250)));
    frame2->setOpacity(0);
    frame2->setCascadeOpacityEnabled(true);
    frame2->setTag(0);
    _bookImage->addChild(frame2);

    auto frame2_top = ImageView::create("games/story_elements/problem_frame_top.png");
    frame2_top->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame2_top->setPosition(Vec2(frame2->getContentSize().width/2, frame2->getContentSize().height));
    frame2_top->setCascadeOpacityEnabled(true);
    frame2_top->setName("top");
    frame2->addChild(frame2_top);

    auto frame2_title = Label::createWithTTF(list_game_info[1].title, "fonts/KGWHATTHETEACHERWANTS.TTF",22);
    frame2_title->setScale(1.4);
    frame2_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame2_title->setTextColor(Color4B::BLACK);
    frame2_title->setPosition(Vec2((frame2_top->getContentSize().width/2), frame2_top->getContentSize().height/2));
    frame2_title->setAlignment(TextHAlignment::CENTER);
    frame2_top->addChild(frame2_title);

    auto frame2_info = ImageView::create("games/story_elements/icon_question.png");
    frame2_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame2_info->setPosition(Vec2(frame2_top->getContentSize().width - AspectR::getWidth(20),
                                  frame2_top->getContentSize().height - AspectR::getHeight(20)));
    frame2_top->addChild(frame2_info);

    auto frame2_info_bar = ImageView::create("games/story_elements/problem_dialog.png");
    frame2_info_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame2_info_bar->setPosition(Vec2(frame2->getContentSize().width/2, frame2->getContentSize().height - AspectR::getHeight(45)));
    frame2_info_bar->setVisible(false);
    frame2_info_bar->setName("info_bar");
    frame2_info_bar->setScale9Enabled(true);
    frame2_info_bar->setContentSize(Size(AspectR::getWidth(200), AspectR::getHeight(150)));
    frame2->addChild(frame2_info_bar);

    auto frame2_info_bar_txt = Label::createWithTTF(list_game_info[1].text, "fonts/KGWHATTHETEACHERWANTS.TTF", 23);
    frame2_info_bar_txt->setScale(1);
    frame2_info_bar_txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame2_info_bar_txt->setTextColor(Color4B::BLACK);
    frame2_info_bar_txt->setPosition(Vec2(frame2_info_bar->getContentSize().width/2, (frame2_info_bar->getContentSize().height/2) - AspectR::getHeight(5)));
    frame2_info_bar_txt->setDimensions(frame2_info_bar->getContentSize().width - AspectR::getWidth(10), frame2_info_bar->getContentSize().height - AspectR::getHeight(20));
    frame2_info_bar_txt->setAlignment(TextHAlignment::LEFT,TextVAlignment::TOP);
    frame2_info_bar->addChild(frame2_info_bar_txt);

    _holder.push_back(frame2);
    /////////////////////////////////

    //Setting//////////////////
    auto frame3 = ImageView::create("games/story_elements/settings_frame.png");
    frame3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame3->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(550)));
    frame3->setOpacity(0);
    frame3->setCascadeOpacityEnabled(true);
    frame3->setTag(0);
    _bookImage->addChild(frame3);

    auto frame3_top = ImageView::create("games/story_elements/settings_frame_top.png");
    frame3_top->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame3_top->setPosition(Vec2(frame3->getContentSize().width/2, frame3->getContentSize().height));
    frame3_top->setCascadeOpacityEnabled(true);
    frame3_top->setName("top");
    frame3->addChild(frame3_top);

    auto frame3_title = Label::createWithTTF(list_game_info[2].title, "fonts/KGWHATTHETEACHERWANTS.TTF",22);
    frame3_title->setScale(1.4);
    frame3_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame3_title->setTextColor(Color4B::BLACK);
    frame3_title->setPosition(Vec2((frame3_top->getContentSize().width/2), frame3_top->getContentSize().height/2));
    frame3_title->setAlignment(TextHAlignment::CENTER);
    frame3_top->addChild(frame3_title);

    auto frame3_info = ImageView::create("games/story_elements/icon_question.png");
    frame3_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame3_info->setPosition(Vec2(frame3_top->getContentSize().width - AspectR::getWidth(20),
                                  frame3_top->getContentSize().height - AspectR::getHeight(20)));
    frame3_top->addChild(frame3_info);

    auto frame3_info_bar = ImageView::create("games/story_elements/settings_dialog.png");
    frame3_info_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame3_info_bar->setPosition(Vec2(frame3->getContentSize().width/2, frame3->getContentSize().height - AspectR::getHeight(45)));
    frame3_info_bar->setVisible(false);
    frame3_info_bar->setName("info_bar");
    frame3_info_bar->setScale9Enabled(true);
    frame3_info_bar->setContentSize(Size(AspectR::getWidth(200), AspectR::getHeight(150)));
    frame3->addChild(frame3_info_bar);

    auto frame3_info_bar_txt = Label::createWithTTF(list_game_info[2].text, "fonts/KGWHATTHETEACHERWANTS.TTF", 23);
    frame3_info_bar_txt->setScale(1);
    frame3_info_bar_txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame3_info_bar_txt->setTextColor(Color4B::BLACK);
    frame3_info_bar_txt->setPosition(Vec2(frame3_info_bar->getContentSize().width/2, (frame3_info_bar->getContentSize().height/2) - AspectR::getHeight(5)));
    frame3_info_bar_txt->setDimensions(frame3_info_bar->getContentSize().width - AspectR::getWidth(10), frame3_info_bar->getContentSize().height - AspectR::getHeight(20));
    frame3_info_bar_txt->setAlignment(TextHAlignment::LEFT,TextVAlignment::TOP);
    frame3_info_bar->addChild(frame3_info_bar_txt);

    _holder.push_back(frame3);
    /////////////////////////////////

    //Solution//////////////////
    auto frame4 = ImageView::create("games/story_elements/solution_frame.png");
    frame4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame4->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(350)));
    frame4->setOpacity(0);
    frame4->setCascadeOpacityEnabled(true);
    frame4->setTag(0);
    _bookImage->addChild(frame4);

    auto frame4_top = ImageView::create("games/story_elements/solution_frame_top.png");
    frame4_top->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame4_top->setPosition(Vec2(frame4->getContentSize().width/2, frame4->getContentSize().height));
    frame4_top->setCascadeOpacityEnabled(true);
    frame4_top->setName("top");
    frame4->addChild(frame4_top);

    auto frame4_title = Label::createWithTTF(list_game_info[3].title, "fonts/KGWHATTHETEACHERWANTS.TTF",22);
    frame4_title->setScale(1.4);
    frame4_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame4_title->setTextColor(Color4B::BLACK);
    frame4_title->setPosition(Vec2((frame4_top->getContentSize().width/2), frame4_top->getContentSize().height/2));
    frame4_title->setAlignment(TextHAlignment::CENTER);
    frame4_top->addChild(frame4_title);

    auto frame4_info = ImageView::create("games/story_elements/icon_question.png");
    frame4_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame4_info->setPosition(Vec2(frame4_top->getContentSize().width - AspectR::getWidth(20),
                                  frame4_top->getContentSize().height - AspectR::getHeight(20)));
    frame4_top->addChild(frame4_info);

    auto frame4_info_bar = ImageView::create("games/story_elements/solution_dialog.png");
    frame4_info_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame4_info_bar->setPosition(Vec2(frame4->getContentSize().width/2, frame4->getContentSize().height - AspectR::getHeight(45)));
    frame4_info_bar->setVisible(false);
    frame4_info_bar->setName("info_bar");
    frame4_info_bar->setScale9Enabled(true);
    frame4_info_bar->setContentSize(Size(AspectR::getWidth(200), AspectR::getHeight(150)));
    frame4->addChild(frame4_info_bar);

    auto frame4_info_bar_txt = Label::createWithTTF(list_game_info[3].text, "fonts/KGWHATTHETEACHERWANTS.TTF", 23);
    frame4_info_bar_txt->setScale(1);
    frame4_info_bar_txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame4_info_bar_txt->setTextColor(Color4B::BLACK);
    frame4_info_bar_txt->setPosition(Vec2(frame4_info_bar->getContentSize().width/2, (frame4_info_bar->getContentSize().height/2) - AspectR::getHeight(5)));
    frame4_info_bar_txt->setDimensions(frame4_info_bar->getContentSize().width - AspectR::getWidth(10), frame4_info_bar->getContentSize().height - AspectR::getHeight(20));
    frame4_info_bar_txt->setAlignment(TextHAlignment::LEFT,TextVAlignment::TOP);
    frame4_info_bar->addChild(frame4_info_bar_txt);

    _holder.push_back(frame4);
    /////////////////////////////////

    if(_levelData.size() == 5) {
        //Supporting Character//////////////////
        auto frame5 = ImageView::create("games/story_elements/support_char_frame.png");
        frame5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        frame5->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(150)));
        frame5->setOpacity(0);
        frame5->setCascadeOpacityEnabled(true);
        frame5->setTag(0);
        _bookImage->addChild(frame5);

        auto frame5_top = ImageView::create("games/story_elements/support_char_frame_top.png");
        frame5_top->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        frame5_top->setPosition(Vec2(frame5->getContentSize().width/2, frame5->getContentSize().height));
        frame5_top->setCascadeOpacityEnabled(true);
        frame5_top->setName("top");
        frame5->addChild(frame5_top);

        auto frame5_title = Label::createWithTTF(list_game_info[4].title, "fonts/KGWHATTHETEACHERWANTS.TTF",22);
        frame5_title->setScale(1.4);
        frame5_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        frame5_title->setTextColor(Color4B::BLACK);
        frame5_title->setPosition(Vec2(frame5_top->getContentSize().width/2, frame5_top->getContentSize().height/2));
        frame5_title->setAlignment(TextHAlignment::CENTER);
        frame5_top->addChild(frame5_title);

        auto frame5_info = ImageView::create("games/story_elements/icon_question.png");
        frame5_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        frame5_info->setPosition(Vec2(frame5_top->getContentSize().width - AspectR::getWidth(20),
                                      frame5_top->getContentSize().height - AspectR::getHeight(20)));
        frame5_top->addChild(frame5_info);

        auto frame5_info_bar = ImageView::create("games/story_elements/support_char_dialog.png");
        frame5_info_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        frame5_info_bar->setPosition(Vec2(frame5->getContentSize().width/2, frame5->getContentSize().height - AspectR::getHeight(45)));
        frame5_info_bar->setVisible(false);
        frame5_info_bar->setName("info_bar");
        frame5_info_bar->setScale9Enabled(true);
        frame5_info_bar->setContentSize(Size(AspectR::getWidth(200), AspectR::getHeight(150)));
        frame5->addChild(frame5_info_bar);

        auto frame5_info_bar_txt = Label::createWithTTF(list_game_info[4].text, "fonts/KGWHATTHETEACHERWANTS.TTF",23);
        frame5_info_bar_txt->setScale(1);
        frame5_info_bar_txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        frame5_info_bar_txt->setTextColor(Color4B::BLACK);
        frame5_info_bar_txt->setPosition(Vec2(frame5_info_bar->getContentSize().width/2, (frame5_info_bar->getContentSize().height/2) - AspectR::getHeight(5)));
        frame5_info_bar_txt->setDimensions(frame5_info_bar->getContentSize().width - AspectR::getWidth(10), frame5_info_bar->getContentSize().height - AspectR::getHeight(20));
        frame5_info_bar_txt->setAlignment(TextHAlignment::LEFT,TextVAlignment::TOP);
        frame5_info_bar->addChild(frame5_info_bar_txt);

        _holder.push_back(frame5);
        /////////////////////////////////

        frame3->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(550)));
        frame4->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(350)));
    } else if(_levelData.size() == 4) {
        frame3->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(450)));
        frame4->setPosition(Vec2((_bookImage->getContentSize().width / 4) * 3, AspectR::getHeight(200)));
    }

    //create image tile
    for(int i=0;i<(int)_levelData.size();i++) {
        auto img = ImageTile::create(_levelData[i].ansImage);
        img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        img->holder = _holder[i];
        img->audioIndex = i;
        _pictures.push_back(img);
        _picturesGuideGame.push_back(img);
		//sorted_imgs.pushBack(img);
    }

	sorted_imgs.pushBack(_pictures.at(0));
	if (num_main_char == 1)
		audios.push_back("story_elements/instructions/audio1.mp3");
	else
		audios.push_back("story_elements/instructions/audio11.mp3");
	if ((int)_levelData.size() == 5){
		sorted_imgs.pushBack(_pictures.at(4));
		if (num_support==1)
			audios.push_back("story_elements/instructions/audio5.mp3");
		else
			audios.push_back("story_elements/instructions/audio51.mp3");
	}
	
	sorted_imgs.pushBack(_pictures.at(2));
	audios.push_back("story_elements/instructions/audio3.mp3");
	sorted_imgs.pushBack(_pictures.at(1));
	audios.push_back("story_elements/instructions/audio2.mp3");
	sorted_imgs.pushBack(_pictures.at(3));
	audios.push_back("story_elements/instructions/audio4.mp3");
	
    //overlay
    _overlay = Layout::create();
    _overlay->setContentSize(this->getContentSize());
    _overlay->setBackGroundColorType(BackGroundColorType::SOLID);
    _overlay->setBackGroundColor(Color3B::BLACK);
    _overlay->setOpacity(200);
    _overlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _overlay->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY()));
    _overlay->setVisible(false);
    _overlay->setTouchEnabled(true);
    _overlay->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (!canZoomOut) return;
        if (type == Widget::TouchEventType::ENDED) {
            zoomOut(_dragTile, false, _defaultElementScaled);
        }
    });
    this->addChild(_overlay);

    //do shuffle
    std::srand ( unsigned ( std::time(0) ) );
    std::random_shuffle ( _pictures.begin(), _pictures.end() );

    //positioning image tile
    float height = (visibleSize.height/(int)_levelData.size());
    float yy = height / 2;
    for(int i=0;i<(int)_levelData.size();i++) {
        _pictures[i]->setPosition(Vec2(AspectR::getMidX() - _pictures[i]->getContentSize().width - AspectR::getWidth(1024), yy));
        _pictures[i]->originalPos = Vec2(_bookImage->getPositionX()-_bookImage->getContentSize().width/2 - 10 - _pictures[i]->getBoundingBox().size.width/2,_pictures[i]->getPositionY());
        this->addChild(_pictures[i]);
        yy += height;
    }
    if(cocos2d::FileUtils::getInstance()->isFileExist("story_elements//instructions/gameName.mp3")){
        this->runAction(Sequence::create(CallFunc::create([=] {
            AUDIO_ENGINE->playSound("story_elements//instructions/gameName.mp3");
            }), DelayTime::create(2), CallFunc::create([=] {
                AUDIO_ENGINE->playSound("story_elements//instructions/mc1.mp3", false, [=]() {
                    this->schedule([=](float) {
                        this->showGuide();
                        }, 10, 100, 10, "show_guide_story_elements");
                });
            }), NULL));
            gameName->runAction(Sequence::create(DelayTime::create(1.5), FadeOut::create(0.3), NULL));
            _bookImage->runAction(Sequence::create(DelayTime::create(1.78),
                                                   CallFunc::create([=]() {
                                                       AudioEngine::play2d("story_elements//slide_1.mp3");
                                                   }),
                                                   EaseCubicActionOut::create(MoveTo::create(1, Vec2(_bookImage->getPositionX(), AspectR::getMidY()))),
                                                   CallFunc::create([=]() {
                                                       AudioEngine::play2d("story_elements//Paper_2.mp3");
                                                       titleTxt->runAction(FadeIn::create(0.1));
                                                       float delay = 0.1;
                                                       for (auto& h : _holder) {
                                                           h->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.1), NULL));
                                                           delay += 0.1;
                                                       }
                                                   }),
                                                   DelayTime::create(1),
                                                   CallFunc::create([=]() {
                                                       AudioEngine::play2d("story_elements//Paper_1.mp3");
                                                       for (auto& p : _pictures) {
                                                           auto pos_goal = Vec2(_bookImage->getPositionX() - _bookImage->getContentSize().width / 2 - 10 - p->getBoundingBox().size.width / 2, p->getPositionY());
                                                           p->runAction(EaseCubicActionOut::create(MoveTo::create(1, pos_goal)));
                                                       }
                                                       canTouch = true;

                                                       for (auto& h : _holder) {
                                                           auto top = (ImageView*)h->getChildByName("top");
                                                           top->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), RotateTo::create(0.1, -3), RotateTo::create(0.1, 3), RotateTo::create(0.1, 0), nullptr)));
                                                       }
                                                   }),
                                                   NULL));
    }
}

vector<LevelData> GamePlay::loadLevelData(std::vector<math::resource::Image*> images) {
    string path = FileUtils::getInstance()->fullPathForFilename(_json_file);
    Document document;
    document.Parse(FileUtils::getInstance()->getStringFromFile(path).c_str());

    vector<LevelData> totalLevels;
    vector<LevelData> totalLevelsClone;

    //overall array
    auto levelArray = document["list_items"].GetArray();
    int size = levelArray.Size() - 1;

    if (JSON_SAFE_CHECK(document, "Main Character", Array))
    {
        auto main__ = document["Main Character"].GetArray();
        if (main__.Size() > 0 && main__[0].IsObject()) {
            auto i = main__[0].GetObject();
            auto img_nature = math::resource::ResHelper::getImage(images, i["Main Character"].GetString());
            LevelData level;
            level.ansImage = img_nature->path;
            level.ansAudio = img_nature->getAudio();
            totalLevels.push_back(level);
        }
    }

    if (JSON_SAFE_CHECK(document, "Problem", String))
    {
        auto img_nature = math::resource::ResHelper::getImage(images, document["Problem"].GetString());
        LevelData level;
        level.ansImage = img_nature->path;
        level.ansAudio = img_nature->getAudio();
        totalLevels.push_back(level);
    }

    if (JSON_SAFE_CHECK(document, "Settings", String))
    {
        auto img_nature = math::resource::ResHelper::getImage(images, document["Settings"].GetString());
        LevelData level;
        level.ansImage = img_nature->path;
        level.ansAudio = img_nature->getAudio();
        totalLevels.push_back(level);
    }

    if (JSON_SAFE_CHECK(document, "Solution", String))
    {
        auto img_nature = math::resource::ResHelper::getImage(images, document["Solution"].GetString());
        LevelData level;
        level.ansImage = img_nature->path;
        level.ansAudio = img_nature->getAudio();
        totalLevels.push_back(level);
    }

    if (JSON_SAFE_CHECK(document, "Supporting Character", Array))
    {
        auto main__ = document["Supporting Character"].GetArray();
        if (main__.Size() > 0 && main__[0].IsObject()) {
            auto i = main__[0].GetObject();
            auto img_nature = math::resource::ResHelper::getImage(images, i["Supporting Character"].GetString());
            LevelData level;
            level.ansImage = img_nature->path;
            level.ansAudio = img_nature->getAudio();
            totalLevels.push_back(level);
        }
    }

    /*for (int i = 1; i <= size; i++) {
        auto img_nature = math::resource::ResHelper::getImage(images, levelArray[i].GetString());
        LevelData level;
        level.ansImage = img_nature->path;
        level.ansAudio = img_nature->getAudio();
        totalLevels.push_back(level);
    }*/
    
    if (JSON_SAFE_CHECK(document, "story_name", String))
    {
        story_name = document["story_name"].GetString();
    }
    if (JSON_SAFE_CHECK(document, "Main Character", Array))
    {
        auto main__ = document["Main Character"].GetArray();
        if (main__.Size() > 0 && main__[0].IsObject()) {
            auto i = main__[0].GetObject();
            num_main_char = i["Number of Main Characters:"].GetInt();
        }
    } 

    if (JSON_SAFE_CHECK(document, "Supporting Character", Array))
    {
        auto main__ = document["Supporting Character"].GetArray();
        if (main__.Size() > 0 && main__[0].IsObject()) {
            auto i = main__[0].GetObject();
            num_support = i["Number of Supporting Characters:"].GetInt();
        }
    }
    return totalLevels;
}

void GamePlay::zoomIn(ImageTile* img, bool inFinish /*= false*/) {
    stopShowGuide();
    stopRemind();
    canTouch = false;
    _overlay->getParent()->reorderChild(_overlay, _overlay->getParent()->getChildrenCount()-2);
    _overlay->setVisible(true);

    img->getParent()->reorderChild(img, img->getParent()->getChildrenCount()-1);
    img->runAction(MoveTo::create(0.3,Vec2(AspectR::getMidX(), AspectR::getMidY())));

	float maxHeight = Director::getInstance()->getVisibleSize().height - 50;
	float maxScaleY = maxHeight / img->getContentSize().height;
	float maxWidth = Director::getInstance()->getVisibleSize().width - 50;
	float maxScaleX = maxWidth / img->getContentSize().width;
	float maxScale = maxScaleX > maxScaleY ? maxScaleY : maxScaleX;
    float currentScale = img->getScale();
    

	img->runAction(Sequence::create(ScaleTo::create(0.3, maxScale), CallFunc::create([=](){
        
		if (inFinish){
			auto _ansAudio1 = cocos2d::experimental::AudioEngine::play2d(audios.at(0));
			cocos2d::experimental::AudioEngine::setFinishCallback(_ansAudio1, [=](int au_id, std::string path){
				_ansAudio = cocos2d::experimental::AudioEngine::play2d(_levelData[img->audioIndex].ansAudio);
				cocos2d::experimental::AudioEngine::setFinishCallback(_ansAudio, [=](int au_id, std::string path){
					if (inFinish){
						if (sorted_imgs.contains(img)){
							sorted_imgs.eraseObject(img);
						}
						audios.erase(audios.begin());
						zoomOut(img, true, currentScale);
					}
				});
			});
		}
		else{
			_ansAudio = cocos2d::experimental::AudioEngine::play2d(_levelData[img->audioIndex].ansAudio);
			//this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=](){
			canZoomOut = true;
			canTouch = false;
			//})));
		}
        //canZoomOut = true;
    }), NULL) );

    AudioEngine::play2d("story_elements//pop_and_paper.mp3");
}

void GamePlay::zoomOut(ImageTile* img, bool inFinish/* = false*/, float scaleImg) {
    stopRemind();
    remind();
    if (flag) {
        stopShowGuide();
        this->schedule([=](float) {
            this->showGuide();
            }, 12.5, 100, 10, "show_guide_story_elements");
    }
    canTouch = false;
	canZoomOut = false;
    cocos2d::experimental::AudioEngine::stop(_ansAudio);
    _overlay->setVisible(false);

	float maxHeght = (Director::getInstance()->getVisibleSize().height - 50) / 5;
	float maxScale = maxHeght / img->getContentSize().height;
	if (maxScale > 0.23)
		maxScale = 0.23f;

    img->runAction(MoveTo::create(0.3,img->originalPos));
    img->runAction(Sequence::create(ScaleTo::create(0.3, scaleImg), CallFunc::create([=](){
        img->getParent()->reorderChild(img, img->getParent()->getChildrenCount()-3);
		if (inFinish){
			if (sorted_imgs.size() >= 1){
				this->scheduleOnce([=](float dt){
					zoomIn(sorted_imgs.at(0), true);
				}, 0.2f, "zom_in_next");

			}
			else{
				onFinish();
                this->unschedule("remind");
			}
		}
		else{
			_dragTile = NULL;
			canTouch = true;
			canZoomOut = false;
		}
    }), nullptr));


    AudioEngine::play2d("story_elements//Paper_2.mp3");
}

void GamePlay::onEnter() {
    ms::BaseActivity::onEnter();
    //*** Event Listenter for DRAG ***//
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = [=](Touch* touch, Event* event) {
        if(!canTouch) return false;
        Vec2 pt = convertToNodeSpace(touch->getLocation());

        //stop shake
        for(auto& h : _holder) {
            auto top = (ImageView*)h->getChildByName("top");
            top->stopAllActions();
            top->setRotation(0);
        }

        //disable info bar
        AudioEngine::stop(_infoAudio);
        for(auto& h : _holder) {
            ((ImageView*)h->getChildByName("info_bar"))->setVisible(false);
        }

        //check info bar touch
        for(int i=0;i<_holder.size();i++) {
            this->unschedule("show_guide_story_elements");
            stopRemind();
            if(_holder[i]->getBoundingBox().containsPoint(_bookImage->convertToNodeSpace(pt)) && _holder[i]->getTag() == 0) {
                ((ImageView*)_holder[i]->getChildByName("info_bar"))->setVisible(true);
                if (flag) {
                    this->schedule([=](float) {
                        this->showGuide();
                        }, 12.5, 100, 15, "show_guide_story_elements");
                    stopRemind();
                }
                else
                {
                    stopRemind();
                    remind();
                }
                _infoAudio = AudioEngine::play2d(list_game_info[i].audio);
                break;
            }
            else
            {
                if (flag) {
                    this->schedule([=](float) {
                        this->showGuide();
                        }, 12.5, 100, 15, "show_guide_story_elements");
                    stopRemind();
                }
                else
                {
                    stopRemind();
                    remind();
                }
            }
        }

        for(auto& img : _pictures) {
            if(img->getBoundingBox().containsPoint(pt)) {
                _dragTile = img;
                if (_flag) {
                    AudioEngine::play2d("story_elements//Paper_1.mp3");
                }
                return true;
            }
        }

        return false;
    };
    _listener->onTouchMoved = [=](Touch* touch, Event* event) {
        Vec2 pt = convertToNodeSpace(touch->getLocation());
        if(_dragTile != NULL  && !_dragTile->placed) {
            _dragTile->setPosition(Vec2(_dragTile->getPosition().x + touch->getDelta().x, _dragTile->getPosition().y + touch->getDelta().y));
            //bring to front
            _dragTile->getParent()->reorderChild(_dragTile, _dragTile->getParent()->getChildrenCount()-1);
        }
    };
    _listener->onTouchEnded = [=](Touch* touch, Event* event) {
        Vec2 pt = convertToNodeSpace(touch->getLocation());

        CCLOG("%f,%f == %f,%f",_dragTile->getPosition().x,_dragTile->getPosition().y, _dragTile->originalPos.x,_dragTile->originalPos.y);

        if(_dragTile != NULL &&  (int)_dragTile->getPosition().x == (int)_dragTile->originalPos.x && (int)_dragTile->getPosition().y == (int)_dragTile->originalPos.y) {
            _defaultElementScaled = _dragTile->getScale();
            if (_flag) {
                zoomIn(_dragTile);
            };
        }
        else if(_dragTile != NULL && !_dragTile->placed) {
            auto draggedImage = _dragTile;
            auto holder = _dragTile->holder;
            auto tpt = _bookImage->convertToNodeSpace(touch->getLocation());

            if(_dragTile->holder->getBoundingBox().intersectsCircle (tpt, AspectR::getHeight(50))) {
                flag = false;
                stopShowGuide();
                stopRemind();
                remind();
                draggedImage->getParent()->reorderChild(draggedImage, draggedImage->getParent()->getChildrenCount() - 3);
                draggedImage->setPosition(_bookImage->convertToWorldSpace(holder->getPosition()) + Vec2(0,AspectR::getHeight(-30)));
                draggedImage->originalPos = draggedImage->getPosition();
                draggedImage->placed = true;
                draggedImage->showTick();
                Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
                holder->setTag(1);
				holder->setCascadeOpacityEnabled(false);
				holder->setOpacity(0);
                if (!game_play_enable) {
					canTouch = false;
					canZoomOut = false;
					this->scheduleOnce([=](float dt){
						this->pauseGame();
					}, 0.5f, "pause");
                    
                }
            } else {
                bool wrongDrag = false;
                for(auto& p : _pictures) {
                    if(p->holder->getBoundingBox().intersectsCircle(tpt, AspectR::getHeight(100))) {
                        wrongDrag = true;
                        break;
                    }
                }
                if(wrongDrag) {
                    //dragged on wrong note
                    AudioEngine::play2d("story_elements//wrong.mp3");
                    stopShowGuide();
                    stopRemind();
                    remind();
                    if (flag) {
                        this->schedule([=](float) {
                            this->showGuide();
                            }, 12.5, 100, 10, "show_guide_story_elements");
                    }
                } else {
                    stopShowGuide();
                    stopRemind();
                    if (flag) {
                        this->schedule([=](float) {
                            this->showGuide();
                            }, 10, 100, 10, "show_guide_story_elements");
                    }
                    //drgged on empty position
                }
                canTouch = false;
                _dragTile->runAction(Sequence::create(EaseBackInOut::create(MoveTo::create(0.5,_dragTile->originalPos)), CallFunc::create([=](){
                    canTouch = true;
                }), NULL));
                _dragTile->getParent()->reorderChild(_dragTile, _dragTile->getParent()->getChildrenCount() - 3);
            }

            //check game solved
            bool allSolved = true;
            for(auto& p : _pictures) {
                if(!p->placed) {
                    allSolved = false;
                    break;
                }
            }

            if(allSolved) {   
//                canTouch = false;
                _flag = false;
                auto dummy = Layout::create();
                dummy->setContentSize(_bookImage->getContentSize());
                dummy->setAnchorPoint(_bookImage->getAnchorPoint());
                this->addChild(dummy);

                _bookImage->runAction(Sequence::create(DelayTime::create(1),
                                                       ScaleTo::create(0.3, 1.1),
                                                       NULL));
                _bookImage->runAction(Sequence::create(
					DelayTime::create(1),
					MoveTo::create(0.3, Vec2(AspectR::getMidX(), AspectR::getMidY())),
					CallFunc::create([=](){
					auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d("story_elements//instructions/mc3.mp3");
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
						zoomIn(sorted_imgs.at(0), true);
					});
					
				}), nullptr));


                for(auto& p : _pictures) {
                    dummy->setPosition(Vec2(AspectR::getMidX() + AspectR::getWidth(80), AspectR::getMidY() - _bookImage->getContentSize().height - AspectR::getHeight(768)));
                    auto dn = Node::create();
                    dn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    dn->setPosition(p->holder->getPosition() + Vec2(0,AspectR::getHeight(-30)));
                    dn->setContentSize(p->holder->getContentSize());
                    dummy->addChild(dn);
                    dummy->setPosition(Vec2(AspectR::getMidX(), AspectR::getMidY()));
                    dummy->setScale(1.1);

                    p->runAction(Sequence::create(DelayTime::create(1),
                                                  ScaleTo::create(0.3, p->getScale() + 0.03),
                                                  NULL));

                    p->runAction(Sequence::create(DelayTime::create(1),
                                                  MoveTo::create(0.3, Vec2(dummy->convertToWorldSpace(dn->getPosition()))),
                                                  ScaleTo::create(0.3, p->getScale() + 0.03),
                                                  CallFunc::create([=](){
                                                      dummy->removeChild(dn);
                                                      p->originalPos = p->getPosition();
                                                  }), NULL));

                }

            }
        }
    };
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);
    //*********************//
}

//void GamePlay::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::Director::getInstance()->getWinSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 40);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void GamePlay::onFinishGame(){
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_COMMON);
	}), NULL));
}


void GamePlay::onExit() {
    Layout::onExit();
}

void story_elements::GamePlay::onFinish()
{
	AudioEngine::play2d("story_elements//win_end_game.mp3");

	auto doneButton = ImageView::create("games/story_elements/button.png");
	doneButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	doneButton->setPosition(Vec2(_bookImage->getContentSize().width / 4, AspectR::getHeight(80)));
	doneButton->setTouchEnabled(true);
	doneButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (!canTouch) return;
		if (type == Widget::TouchEventType::ENDED) {
			canTouch = false;
			CCLOG("@@@@ GAME OVER @@@@");
			this->onFinishGame();
		}
	});
	doneButton->setOpacity(0);
	doneButton->setCascadeOpacityEnabled(true);
	_bookImage->addChild(doneButton);

	auto doneButtonCaption = Label::createWithTTF("DONE", "fonts/KGWHATTHETEACHERWANTS.TTF", 32);
	doneButtonCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	doneButtonCaption->setTextColor(Color4B::BLACK);
	doneButtonCaption->setPosition(Vec2(doneButton->getContentSize().width / 2, doneButton->getContentSize().height / 2));
	doneButtonCaption->setAlignment(TextHAlignment::CENTER);
	doneButton->addChild(doneButtonCaption);

	
	auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d("story_elements//instructions/mc2.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
		_gameOver = true;
		canTouch = true;
		doneButton->runAction(FadeIn::create(0.5f));
	});

}

story_elements::GamePlay::~GamePlay()
{
	_eventDispatcher->removeEventListener(_listener);
}

/*
 * Image Tile
 */

ImageTile* ImageTile::create(string src)
{
    ImageTile* obj = new ImageTile();
    if(obj->init(src))
    {
        obj->autorelease();
        obj->initOptions();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void ImageTile::initOptions()
{
	float maxHeght = 120;
	float maxScale = maxHeght / this->getContentSize().height;
    this->setScale(maxScale);
//    if (maxScale < 0.23)
//        this->setScale(maxScale);
//    else
//        this->setScale(0.23);

    //tick
    const string tickPath = "gaf/story_elements/tick/tick.gaf";
    GAFAsset *tickAsset = gaf::GAFAsset::create(tickPath);
    _tickObject = tickAsset->createObject();
    _tickObject->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tickObject->setPosition(Vec2(this->getContentSize().width - AspectR::getWidth(10),this->getContentSize().height - AspectR::getHeight(10)));
    _tickObject->setScale(4);
    this->addChild(_tickObject);
}

void ImageTile::showTick() {
    _tickObject->start();
    AudioEngine::play2d("story_elements//correct.mp3");
}
//-------------------------------------------------

void GamePlay::showGuide() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    _audio = AUDIO_ENGINE->playSound("story_elements//instructions/mc1.mp3");
    remind();
    auto img = _picturesGuideGame.at(0);
    auto start = img->getPosition();
    start = img->getParent()->convertToWorldSpace(start);
    
    auto stop = img->holder->getPosition();
    stop = img->holder->getParent()->convertToWorldSpace(stop) - Point(0, 100);
    
    overLay = cocos2d::ui::Layout::create();
    overLay->setContentSize(Director::getInstance()->getVisibleSize());
    overLay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overLay, INT_MAX - 1);
    
    auto hand = cocos2d::ui::ImageView::create("mjstory/new_ui/hand.png");
    overLay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    hand->setName("hand_guide_game");
    
    hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}

void GamePlay::stopShowGuide() {
    if (Director::getInstance()->getRunningScene()->getChildByName("guide_overlay")) {
        Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    }
    this->unschedule("show_guide_story_elements");
    AUDIO_ENGINE->stopEffect(_audio);
}

void GamePlay::remind() {
    this->schedule([=](float) {
        AUDIO_ENGINE->playSound("story_elements//instructions/mc1.mp3");
    }, 15, 100, 12.5, "remind");
}

void GamePlay::stopRemind() {
    this->unschedule("remind");
    AUDIO_ENGINE->stopSound("story_elements//instructions/mc1.mp3");
}
