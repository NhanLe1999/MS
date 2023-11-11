//
//  CoverStory.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "CoverStory.h"

#define CSB_NAME "csb/game/CoverStory/CoverStory.csb"
#define bg_music "sounds/coverStory/bg_music.mp3"

USING_NS_CC;
INITIALIZE_READER(CoverStory);

std::once_flag CoverStory_reader;

CoverStory* CoverStory::createGame(string game_path){
    
    std::call_once(CoverStory_reader, []{
        REGISTER_CSB_READER(CoverStory);
    });
    
    auto p = static_cast<CoverStory*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        
        p->didLoadFromCSB(game_path);
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void CoverStory::didLoadFromCSB(std::string game_path){
    
    this->generateData();
    
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
}

void CoverStory::generateData(){
    
//    // get data
    string json_file = "config.json";
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& value_data = document.FindMember("data")->value;

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    //get question
    auto question = images.front();
    images.erase(images.begin());
    CoverStory::answer question_1, question_2, question_3;
    question_1.text = question->getText("name_1");
    question_1.audio = question->getAudios("name_1").front();
    question_2.text = question->getText("name_n");
    question_2.audio = question->getAudios("name_n").front();
    question_3.text = question->getText("number_text");
    question_3.audio = question->getAudios("number_text").front();

    questions.push_back(question_1);
    questions.push_back(question_2);
    questions.push_back(question_3);
    
    cover_path = question->path;
    
    //get answers
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        rapidjson::Value& json_answer=value_data[i].FindMember("answers")->value;
        CoverStory::answers ans_list;
        ans_list.q_id = value_data[i].FindMember("question")->value.GetInt();
        for(rapidjson::SizeType j = 0; j < json_answer.Size(); j++){
            CoverStory::answer ans;
            auto letter = json_answer[j].FindMember("answer")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
            ans.text = pImage->getText();
            ans.audio = pImage->getAudios().front();
            ans.is_right = json_answer[j].FindMember("is_right")->value.GetBool();
            ans_list.answer_list.push_back(ans);
            
            cover_paths.push_back(pImage->path);
        }
        game.push_back(ans_list);
    }
}

void CoverStory::gameConfig(){
    
    position = {
        Vec2(-screen_size.width*0.3, screen_size.height*0.5), Vec2(screen_size.width*0.8, screen_size.height*1.5),  Vec2(screen_size.width*0.2, screen_size.height*1.5),   Vec2(screen_size.width*1.2, screen_size.height*0.7), Vec2(screen_size.width*0.5, screen_size.height*1.7), Vec2(root_layout->getContentSize().width*0.3, screen_size.height*0.5),
    };
    
}

void CoverStory::loadFrame(){
    
    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
    
    root_layout = utils::findChild<Layout*>(this, "root_panel");
    utils::findChild(root_layout, "cloud_F")->setLocalZOrder(100);
    
    gameConfig();
    //create books
    
    std::string book_path  = "games/coverStory/cover_cyen.png";
    if(cover_paths.size() < number_books){
        number_books = (int) cover_paths.size();
    }
    for(int i = 0; i < number_books; i++){
        Layout* book = Layout::create();
        book->setContentSize(Size(380, 475));
        book->setAnchorPoint(Point::ANCHOR_MIDDLE);
        book->setScale(0.5);
        
        ImageView* cover_book = ImageView::create(book_path);
        book->addChild(cover_book);
        cover_book->setPosition(book->getContentSize()/2);
        cover_book->setContentSize(book->getContentSize());
        cover_book->ignoreContentAdaptWithSize(false);
        
        ImageView* cover_img = ImageView::create(cover_paths[i]);
        if(i == number_books - 1){
            cover_img->loadTexture(cover_path);
        }
        book->addChild(cover_img);
        cover_img->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
        cover_img->setPosition(Vec2(165, book->getContentSize().height - 34));
        cover_img->setContentSize(Size(315, 425));
        cover_img->ignoreContentAdaptWithSize(false);
        
        root_layout->addChild(book, 10);
        book->setPosition(Vec2(root_layout->getContentSize().width/2, -root_layout->getContentSize().height*0.5));
        
        books.push_back(book);
    }
    
    
    GAFWrapper* sun_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_object3/cs_object3.gaf");
    sun_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(sun_gaf);
    sun_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.12, root_layout->getContentSize().height*0.8));
    sun_gaf->play(true);
    
    GAFWrapper* mercury_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_object1/cs_object1.gaf");
    mercury_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(mercury_gaf);
    mercury_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.25, root_layout->getContentSize().height*0.95));
    mercury_gaf->play(true);
    
    GAFWrapper* plannet_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_object2/cs_object2.gaf");
    plannet_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(plannet_gaf);
    plannet_gaf->setPosition(Vec2(root_layout->getContentSize().width, root_layout->getContentSize().height*0.7));
    plannet_gaf->play(true);
    
    GAFWrapper* ufo_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_object4/cs_object4.gaf");
    ufo_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(ufo_gaf);
    ufo_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.5, root_layout->getContentSize().height*0.5));
    ufo_gaf->play(true);

    GAFWrapper* rocket_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_rocket/cs_rocket.gaf");
    rocket_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(rocket_gaf);
    rocket_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.5, root_layout->getContentSize().height*0.5));
    rocket_gaf->setName("rocket");
    
    q_cloud_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_q_bg/cs_q_bg.gaf");
    q_cloud_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(q_cloud_gaf);
    q_cloud_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.7, root_layout->getContentSize().height*0.85));
    q_cloud_gaf->setName("q_bg");
    q_cloud_gaf->addClickEventListener(CC_CALLBACK_1(CoverStory::onQuestionClicked, this));
 
    
    GAFWrapper* title_gaf = GAFWrapper::createGAF("gaf/coverstory/cs_title/cs_title.gaf");
    title_gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    root_layout->addChild(title_gaf);
    title_gaf->setPosition(Vec2(root_layout->getContentSize().width*0.5, root_layout->getContentSize().height*0.5));
    title_gaf->setName("title");
    AUDIO_ENGINE->play("sounds/coverStory/cs_title.mp3", false, 1.0, [=]{
        AUDIO_ENGINE->play("sounds/coverStory/cover.mp3", false, 1.0, [=]{
            AUDIO_ENGINE->play("sounds/coverStory/instruction.mp3");
        });
    });
  
    title_gaf->play(false, [=](gaf::GAFObject* obj){
         AUDIO_ENGINE->play("sounds/coverStory/cs_missile.mp3");
        rocket_gaf->play(false, [=](gaf::GAFObject* obj){
            
            this->onGameStart();
        });
        
        //moving book
        int i = 0;
        AUDIO_ENGINE->play("sounds/coverStory/cs_cover_fadein.mp3");
        for(auto book : books){
            if(i < number_books - 1){
                book->runAction(Sequence::createWithTwoActions(DelayTime::create((i+1)*0.3), Spawn::createWithTwoActions(ScaleTo::create(1, 1), MoveTo::create(1.5, Vec2(position[i])))));
                i++;
            }else{
                 book->runAction(Sequence::createWithTwoActions(DelayTime::create((i+1)*0.3), Spawn::createWithTwoActions(ScaleTo::create(1, 1), MoveTo::create(1.5, Vec2(position.back())))));
            }
           
        }
    });
}

void CoverStory::onGameStart(){
    
    //reset
    answer_btn.clear();
    count_answer = 0;
    q_cloud_gaf->setTouchEnabled(false);
    
    //create question
    Text* question = Text::create(questions[count_game].text, font_name, font_sz);
//        Text* question = Text::create("dkljslkj klsdlkfjasdj dksljflkjalesj,kjsdlkj lkjl", font_name, font_sz);
    question->setTextColor(Color4B::BLACK);
    q_cloud_gaf->addChild(question);
    question->setOpacity(0);
    question->setContentSize(q_cloud_gaf->getContentSize()*0.9);
    question->setPosition(Vec2(q_cloud_gaf->getContentSize().width*0.55, q_cloud_gaf->getContentSize().height* 0.46));
    question->ignoreContentAdaptWithSize(false);
    question->setTextVerticalAlignment(TextVAlignment::CENTER);
    question->setTextHorizontalAlignment(TextHAlignment::CENTER);
    question->setName("question");
    

    //create answer
    mj::helper::shuffle(game[count_game].answer_list);
    for (int i = 0; i < game[count_game].answer_list.size(); i++) {
        CoverStory_Item* answer = CoverStory_Item::createItem(game[count_game].answer_list[i].text);
        answer->setAnchorPoint(Point::ANCHOR_MIDDLE);
        answer->setDelegate(this);
        if(game[count_game].answer_list[i].is_right){
            answer->setTag(0);
        }else{
            answer->setTag(1);
        }
        answer->setScale(0);
        root_layout->addChild(answer, 101);
        answer->setPosition(Vec2(q_cloud_gaf->getPositionX(), (q_cloud_gaf->getPositionY() - 40) - 160* (i+1)));
        answer->setEnabled(false);
        answer_btn.push_back(answer);
    }

    //cloudy
    q_cloud_gaf->runAction(Sequence::createWithTwoActions(DelayTime::create(2), CallFunc::create([=]{
        q_cloud_gaf->setScaleX(1.2);
        q_cloud_gaf->setVisible(true);
        AUDIO_ENGINE->play("sounds/coverStory/cs_cloud.mp3");
        q_cloud_gaf->play(false, [=](gaf::GAFObject* obj){
            question->runAction(FadeIn::create(0.2));
            question_id =  AUDIO_ENGINE->play(questions[count_game].audio, false, 1, [=]{
                this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([=]{
                        readAnswer(count_answer);
                })));
                
            });
            
            int i = 0;
            AUDIO_ENGINE->play("sounds/coverStory/cs_answer.mp3");
            for(auto ans : answer_btn){
                ans->runAction(Sequence::create(DelayTime::create((i+1)* 0.3), ScaleTo::create(0.3, 1), NULL));
                i++;
            }
            
        });
    })));
    
}

void CoverStory::readAnswer(int count){
    if(count_answer < answer_btn.size()) {
        utils::findChild<Text*>(answer_btn[count], "text")->setTextColor(Color4B::RED);
        answer_id =  AUDIO_ENGINE->play(game[count_game].answer_list[count].audio, false, 1.0, [=]{
             utils::findChild<Text*>(answer_btn[count_answer], "text")->setTextColor(Color4B::BLACK);
            count_answer++;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([=]{
                    this->readAnswer(count_answer);
            })));
            
        });
    }else{
        
        q_cloud_gaf->setTouchEnabled(true);
        for(auto a: answer_btn){
            a->setEnabled(true);
        }
    }
}

void CoverStory::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/CoverStory/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/CoverStory/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void CoverStory::onFinishGame(){
//    AUDIO_ENGINE->stopAllEffects();
    MJMainGBase::onFinishGame();
}

void CoverStory::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}
void CoverStory::onQuestionClicked(cocos2d::Ref *sender){
    AUDIO_ENGINE->stopEffect(question_id);
    question_id = AUDIO_ENGINE->play(questions[count_game].audio);
}
                                    
void CoverStory::onClickedDelegate(cocos2d::Ref *sender){
    
    AUDIO_ENGINE->stopEffect(answer_id);
    
    CoverStory_Item* ans = ((CoverStory_Item*)sender);
    ans->playEffect(ans->getParent()->getTag());
    
    std::string audio_path;
    for(auto ans_text: game[count_game].answer_list){
        if ( ans_text.text == utils::findChild<Text*>(ans, "text")->getString()) {
            audio_path = ans_text.audio;
        }
    }
    
    if(ans->getParent()->getTag() == 0){
        count_game++;
        q_cloud_gaf->setTouchEnabled(false);
        
        for(auto a: answer_btn){
            a->setEnabled(false);
        }
        answer_id =  AUDIO_ENGINE->play(audio_path, false, 1.0, [=]{
            
            if(count_game < game.size()){
                q_cloud_gaf->setVisible(false);
                for(auto ans: answer_btn){
                    ans->runAction(ScaleTo::create(0.5, 0));
                }
                utils::findChild<Text*>(q_cloud_gaf, "question")->removeFromParent();
                onGameStart();
            }else{
                this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    onFinishGame();
                }), NULL) );
                
            }
        });
    }
   
}


