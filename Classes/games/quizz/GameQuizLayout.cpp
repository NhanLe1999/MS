//
//  MJ_GameQuizLayout.cpp
//  M
//
//  Created by To Ngoc Linh on 5/6/17.
//
//

#include "GameQuizLayout.h"
#include "MJHelper.h"
#include "TestingModeUtils.h"
#include "ChooseCorrectGroupQuiz.h"
#include "GameQuizHelper.h"
#include "Math_Libs.h"
#include "MSBookConfig.h"
#include "HSAudioEngine.h"
#include "MJGame.h"
#define time_delay_next 0.1

INITIALIZE_READER(GameQuizLayout);
std::once_flag quizlayout_reader;
#define CSB_NAME "csb/game/quiz/GameQuizLayout.csb"

using namespace cocos2d;
GameQuizLayout * GameQuizLayout::createGame(std::vector<int> list_quiz_id, std::vector<int> list_num_turn) {
    std::call_once(quizlayout_reader, [] {
        REGISTER_CSB_READER(GameQuizLayout);
    });
    
    auto p = static_cast<GameQuizLayout *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(list_quiz_id, list_num_turn);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameQuizLayout::didLoadFromCSB(std::vector<int> list_quiz_id, std::vector<int> list_num_turn){
    quiz_play_type = LIST_QUIZ;
    _list_quiz_id = list_quiz_id;
    _list_num_turnplay = list_num_turn;
    
    auto black_layout = cocos2d::ui::Layout::create();
    black_layout->setContentSize(quiz_size);
    black_layout->setPosition(cocos2d::Vec2::ZERO);
    black_layout->setBackGroundColor(Color3B::BLACK);
    black_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    this->addChild(black_layout);
    
    //AUDIO_ENGINE->playBackgroundMusic("quizz/quizz_bg.mp3");
    createCource();
}

void GameQuizLayout::onEnter(){
    ms::BaseActivity::onEnter();
}

void GameQuizLayout::createCource()
{
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        audio_helper->playMusic();
    }
    cur_exercise = 0;
    this->scheduleOnce(schedule_selector(GameQuizLayout::loadExercise), 0.5);
}

std::string GameQuizLayout::getQuizClassNameWithID(int quiz_id){
    std::string game_name = "";
    if (quiz_id == 17) {
        game_name = "ChooseCorrectGroupQuiz";
    }
    else if (quiz_id == 26){
        game_name = "ColorUseKeysQuiz";
    }
    else if (quiz_id == 16){
        game_name = "CountAndAnswerQuiz";
    }
    else if (quiz_id == 37){
        game_name = "SortedToGroupQuiz";
    }
    else if (quiz_id == 43){
        game_name = "MoveNumberObjectToGroupQuiz";
    }
    else if (quiz_id == 64){
        game_name = "BA_HowManyKindQuiz";
    }
    else if (quiz_id == 65){
        game_name = "My_MissingNumber";
    }
    else if (quiz_id == 70){
        game_name = "ChooseCorrectObjectsQuiz";
    }
    else if (quiz_id == 71){
        game_name = "Huy_ReadingNumber";
    }
    else if (quiz_id == 76){
        game_name = "EscapeMazeQuiz";
    }
    else if (quiz_id == 139){
        game_name = "ColorPictureQuiz";
    }
    else if (quiz_id == 140){
        game_name = "TracingQuiz";
    }
    return game_name;
}

void GameQuizLayout::onPrev(cocos2d::Ref * sender) {
    if (cur_exercise==0) {
//        cocos2d::MessageBox("Đang ở game đầu tiên", "MJ Math");
        return;
    }
}

void GameQuizLayout::onNext(cocos2d::Ref * sender) {

    loadNextExercise(0);
}

void GameQuizLayout::onBack(cocos2d::Ref *) {
    GuideCenter->stopGuide();
    removeExercise();
    mpool->resetAll();
    this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(1.25), cocos2d::CallFunc::create([=](){
        for (int i=0; i<list_quizes.size(); i++) {
            list_quizes[i]->removeFromParentAndCleanup(true);
        }
    })));
}


#include "MSActivityWrapper.h"
#pragma mark - Control
#pragma mark -
void GameQuizLayout::loadNextExercise(float dt)
{
    auto searchpath = FileUtils::getInstance()->getSearchPaths();
    CCLOG("Remove directory: %s", searchpath.begin()->c_str());
    searchpath.erase(searchpath.begin());
    FileUtils::getInstance()->setSearchPaths(searchpath);
    
    removeExercise();
    
    if (count_turn_play < (int) _list_num_turnplay[cur_exercise]-1){
        count_turn_play++;
        list_quizes.erase(list_quizes.begin()+cur_exercise);
    }else {
        count_turn_play = 0;
        cur_exercise++;
    }
    
    /*
    if (!game_play_enable && count_turn_play >= 1) {
        pauseGame();
        return;
    }
     */
    
    if (cur_exercise<(int)_list_quiz_id.size()) {
        //remove
        loadExercise(0);
    } else {
        if (cur_exercise < (int) list_quizes.size()) {
            list_quizes[cur_exercise]->enableControlQuiz();
        } else list_quizes.back()->enableControlQuiz();
        onFinishGame();
    }
}

#include "StoryConfigManager.h"
void GameQuizLayout::onFinishGame(){
    for (Math_Quiz* quiz :list_quizes) {
        quiz->enableControlQuiz();
        this->getEventDispatcher()->removeEventListenersForTarget(quiz);
    }
    
    GuideCenter->stopGuide();
    audio_helper->stopMusic();
    //AUDIO_ENGINE->stopBackgroundMusic();
    //    auto old_searchpaths = FileUtils::getInstance()->getSearchPaths();
    //    old_searchpaths.erase(std::remove_if(old_searchpaths.begin(), old_searchpaths.end(), [=](std::string path) {
    //        return path == search_path_quiz_add;
    //    }), old_searchpaths.end());
    //    FileUtils::getInstance()->setSearchPaths(old_searchpaths);
    
    this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
        if(CONFIG_MANAGER->isVietnameseStories()){
            this->onCloseGameVmonkey();
        }else{
            commonEndGame(ms::BaseActivity::EGT_COMMON);
            //_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        }
    }), NULL));
}

void GameQuizLayout::onCloseGame() {
    
    clearData();
    
    if(CONFIG_MANAGER->isVietnameseStories()){
               this->onCloseGameVmonkey(true);
    }

    //this->removeFromParentAndCleanup(false);
    GuideCenter->stopGuide();
    audio_helper->stopMusic();
    //AUDIO_ENGINE->stopBackgroundMusic();
}

void GameQuizLayout::loadPrevExercise(float dt)
{
    float delay_time=0;
    if (cur_exercise>=0) {
        delay_time+=1.25;
    }
    if (cur_exercise>0) {
        removeExercise();
        cur_exercise--;
        this->scheduleOnce(schedule_selector(GameQuizLayout::loadExercise), delay_time);
    } else {
        onPrev(nullptr);
    }
}

#include "StoryConfigManager.h"
void GameQuizLayout::loadExercise(float dt)
{
    mpool->resetAll();
    
    //add
    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + "extracted/" + StringUtils::toString(cur_exercise) + "/", true);

    int quiz_id = 0;
    
    if(this->getTag()== STARTING_FROM_STORY_TAG)
    {
        quiz_id = StoryDataModel::getInstance()->getActivityOfStory(_list_quiz_id[cur_exercise]).sub_id;
    }
    else
    {
        if (cur_exercise < _list_quiz_id.size())
        {
            quiz_id = StoryDataModel::getInstance()->getActivityById(_list_quiz_id[cur_exercise]).sub_id;
        }
        else {
            this->getParent()->removeFromParent();
            return;
        }
    }
    std::string game_name = getQuizClassNameWithID(quiz_id);
    auto game_model = mj::model::Game(0, game_name, "");    
    testing::TestManager::getInstance()->setTestMode(testing::stories);
    testing::TestManager::getInstance()->setClassname(game_name);
    Math_Quiz* quiz = math::quiz::createQuiz(game_model);
    if (!quiz)
        return;
    list_quizes.push_back(quiz);
    quiz->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    quiz->setPosition(quiz_size/2);
    this->addChild(quiz);
    quiz->onStart();
    quiz->setCompleteCallback(CC_CALLBACK_1(GameQuizLayout::onAnswered, this));
    
    //Condition to stop trial
    if (!game_play_enable) {
        bool stop_when_completed = false;
        if (_list_quiz_id.size() > 1 || _list_num_turnplay[cur_exercise] > 1) {
            stop_when_completed = true;
        }
        
        //TODO: test stop_when_completed
        quiz->setStopTrialCallback([=](){
            GuideCenter->stopGuide();
            audio_helper->stopMusic();
            for (Math_Quiz* quiz :list_quizes) {
                quiz->enableControlQuiz();
                //quiz->onHide(true);
            }
            this->pauseGame();
        }, stop_when_completed);
    }
}

void GameQuizLayout::removeExercise()
{
    if (cur_exercise < (int) list_quizes.size() -1){
        list_quizes[cur_exercise]->onHide();
    }
}

#pragma mark - Callback Functions
//Prev & back
void GameQuizLayout::onPrevExercise(cocos2d::Ref *sender)
{
    this->scheduleOnce(schedule_selector(GameQuizLayout::loadPrevExercise), time_delay_next);
}

void GameQuizLayout::onNextExercise(cocos2d::Ref *sender)
{
    this->scheduleOnce(schedule_selector(GameQuizLayout::loadNextExercise), time_delay_next);
}

#pragma mark - Delegate
#pragma mark -
void GameQuizLayout::onAnswered(bool correct)
{
    loadNextExercise(0);
}

void GameQuizLayout::clearData() {
    
    for (Math_Quiz* quiz :list_quizes) {
        
        quiz->enableControlQuiz();
        
        while (quiz->getReferenceCount()>=2) {
            quiz->release();
        }
        
        this->getEventDispatcher()->removeEventListenersForTarget(quiz);
    }
}
