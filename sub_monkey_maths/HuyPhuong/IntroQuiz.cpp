//
//  IntroQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#include "IntroQuiz.h"

#define k_play_begining "play_begin_intro"
#define k_play_content "play_content_intro"
#define k_play_ending "play_ending_intro"

IntroQuiz* IntroQuiz::createQuiz()
{
    IntroQuiz* quiz=new IntroQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool IntroQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void IntroQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    audio_helper->pauseMusic();

    rapidjson::Value& data_json = document.FindMember("data")->value;
    grade = getJsonInt(data_json, "grade", 1);
    skill = getJsonInt(data_json, "skill", 1);
    unit = getJsonInt(data_json, "unit", 1);
    
    rapidjson::Value& GAF_json = data_json.FindMember("GAF_position")->value;
    delta_GAF = cocos2d::Vec2(getJsonInt(GAF_json, "delta_x", 0), getJsonInt(GAF_json, "delta_y", 0));
    
    std::string intro_name = cocos2d::StringUtils::format("L%dS%dU%d", grade, skill, unit);
    intro_path = cocos2d::StringUtils::format("game/intros/%s/%s", intro_name.c_str(), intro_name.c_str());
    
    img_list = getImageList("icons");
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
}

void IntroQuiz::onStart() {
    showQuestionBox();
    getQuestion()->setVisible(false);
    
    //
    center = cocos2d::Vec2(quiz_size.width, quiz_size.height)/2;
    
    //GAF Monkey
    auto gaf_asset_monkey = gaf::GAFAsset::create("game/intros/monkey/monkey.gaf");
    monkey = gaf_asset_monkey->createObjectAndRun(true);
    monkey->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    monkey->setPosition(cocos2d::Vec2(center.x - 250, -400));
    addChildNode(monkey);
    monkey->runAction(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, 550)));
    
    //Show board
    img_board = cocos2d::ui::ImageView::create(getUIPath("board.png"));
    img_board->setPosition(cocos2d::Vec2(center.x + 200, center.y + 650));
    addChildNode(img_board);
    img_board->runAction(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, -550)));
    
    auto board_size = img_board->getContentSize();
    auto text = cocos2d::ui::Text::create(math::func::upperString(img_list.front()->getText()), m_text_font, 65);
    text->setPosition(cocos2d::Vec2(board_size.width*0.5, board_size.height*0.55));
    text->setTextAreaSize(cocos2d::Size(board_size.width * 0.7, board_size.height*0.7));
    text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    img_board->addChild(text);
    
    //Play audio begining
    scheduleOnce([=](float){
        std::vector<std::string> audios;
        audios.push_back("game/intros/end/begin.mp3");
        auto topic = img_list.front()->getAudios();
        for (std::string au : topic) {
            audios.push_back(au);
        }
        
        seq_playing = new math::SequenceAudio();
        seq_playing->audios = audios;
        seq_playing->name = k_play_begining;
        seq_playing->func = CC_CALLBACK_1(IntroQuiz::playBeginingDone, this);
        
        audio_helper->play_sequence(seq_playing);
    }, 1.0, k_play_begining);

}

void IntroQuiz::startQuestion() {

}

void IntroQuiz::onExplanation(bool correct) {
    this->nextQuestion(0.5);
}

void IntroQuiz::onComplete() {
    if (monkey) {
        monkey->removeFromParentAndCleanup(true);
    }
    if (content) {
        content->removeFromParentAndCleanup(true);
    }
    if (ending) {
        ending->removeFromParentAndCleanup(true);
    }
    if (img_board) {
        img_board->removeFromParentAndCleanup(true);
    }
    if (seq_playing) {
        audio_helper->stop_sequence(seq_playing);
    }

    scheduleOnce([=](float){
        audio_helper->resumeMusic();
    }, 1.0, "resume music");
    hideQuestionBox(0.6);
}

void IntroQuiz::playBeginingDone(std::string name) {
    CCLOG("Play audio done : %s", name.c_str());
    
    if (name == k_play_begining) {
        scheduleOnce([=](float){
            monkey->runAction(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, -550)));
            img_board->runAction(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, 550)));
        }, 0.75, "remove begining");
        
        scheduleOnce([=](float){
            //GAF intro
            std::string intro_gaf = cocos2d::StringUtils::format("%s.gaf", intro_path.c_str());
            std::string intro_mp3 = cocos2d::StringUtils::format("%s.mp3", intro_path.c_str());
            
            //play GAF
            auto gaf_asset = gaf::GAFAsset::create(intro_gaf);
            content = gaf_asset->createObjectAndRun(false);
            content->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            content->setPosition(center + delta_GAF);
            this->addChildNode(content);
            
            //Play audio
            std::vector<std::string> audios;
            audios.push_back(intro_mp3);

            seq_playing = new math::SequenceAudio();
            seq_playing->audios = audios;
            seq_playing->name = k_play_content;
            seq_playing->func = CC_CALLBACK_1(IntroQuiz::playBeginingDone, this);
            
            audio_helper->play_sequence(seq_playing);
        }, 1.5, k_play_content);
    } else if (name == k_play_content) {
        scheduleOnce([=](float) {
            std::string intro_mp3 = "game/intros/end/end.mp3";
            //GAF Ending
            auto gaf_asset_ending = gaf::GAFAsset::create("game/intros/end/end.gaf");
            ending = gaf_asset_ending->createObjectAndRun(false);
            ending->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            ending->setPosition(center);
            this->addChildNode(ending);
            
            //Play audio
            std::vector<std::string> audios;
            audios.push_back(intro_mp3);
            
            seq_playing = new math::SequenceAudio();
            seq_playing->audios = audios;
            seq_playing->name = k_play_ending;
            seq_playing->func = CC_CALLBACK_1(IntroQuiz::playBeginingDone, this);
            
            audio_helper->play_sequence(seq_playing);
        }, 1.5, k_play_ending);
    } else {
        nextQuestion(0.5);
    }
}

#pragma mark - object touch events callback
void IntroQuiz::onAnswerDone(Node* node)
{
    this->onExplanation(true);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 4
void IntroQuiz::autoTesting() {
    
    
}

void IntroQuiz::autoTestingRun(float delaytime)
{
    _answers[0]->onAutoClick();
}

