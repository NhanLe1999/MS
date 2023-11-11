//
//  StoryTheEndPage.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryTheEndPage.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "StoryDataModel.h"
#include "AppDelegate.h"
#include "NativeBridge.h"
#include "StoryEndingPage.h"
#include "StoryPageVC.h"
#include "StoryFreeManager.h"
#include "MJFirebaseListener.h"

INITIALIZE_READER_INSTANCE(StoryTheEndPageReader);

USING_NS_CC;
using namespace ui;

StoryTheEndPage * StoryTheEndPage::createPage(int type, bool isHaveGame) {
    auto p = static_cast<StoryTheEndPage *>(CSLoader::createNodeWithVisibleSize("csb/StoryTheEndPage.csb"));
    if (p) {
        p->preload(type, isHaveGame);
    }
    return p;
}

void StoryTheEndPage::preload(int type, bool isHaveGame) {
    endingpage_type = type;
    _isHaveGame = isHaveGame;
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    auto btplay = this->getChildByName("btplay");
    btplay->setVisible(false);
    btplay->setOpacity(0);
    title = this->getChildByName<Text *>("title");
    title->setLocalZOrder(1000);
    title->setVisible(false);
}

void StoryTheEndPage::didLoadFromCSB() {
    size_h = XHSLib->getSizeScreen().height;
    size_w = XHSLib->getSizeScreen().width;
    
    auto bg = this->getChildByName("bg");
    if (bg->getContentSize().width < (size_w+50)) {
        bg->setScaleX((size_w+50)/bg->getContentSize().width);
    }
    
    line_run = cocos2d::utils::findChild((StoryPageVC *) this->getParent(), "line_run");
    this->setClippingEnabled(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        this->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
        this->setAnchorPoint(Vec2(0, 0));
        this->setPosition(Vec2(0, 0));
    }
#endif
    this->setContentSize(cocos2d::Size(0, size_h));
    this->schedule(schedule_selector(StoryTheEndPage::updateClipShowView));
    
}

void StoryTheEndPage::runAnimShow(){
    //    auto title_img = this->getChildByName("title_img");
    //    title_img->runAction(Sequence::create(DelayTime::create(3.5), FadeOut::create(1.0f), CallFunc::create([=]{
    //        AUDIO_ENGINE->playEffect("theendpage/theendpage_showtitle.mp3");
    //        showYouHaveLearn();
    //    }), NULL));
}


void StoryTheEndPage::showViewWhenEndPage(){
    //showBtPlay();
    auto img_title = ImageView::create("mjstory/theendpage/theendpage_title_learn_new.png");
    auto w_img = XHSLib->getSizeScreen().width/2;
    img_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    img_title->setPosition(Vec2(w_img+50, XHSLib->getSizeScreen().height*1.3));
    img_title->setName("img_title");
    this->addChild(img_title, 10);
    AUDIO_ENGINE->playEffect("theendpage/endingpage_titledown1.mp3");
    img_title->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.75f, Vec2(w_img+50, XHSLib->getSizeScreen().height+30))), CallFunc::create([=]{
        showYouHaveLearn();
    }), NULL));
    
    auto img_hill = ImageView::create("mjstory/theendpage/theendpage_hill.png");
    img_hill->setPosition(Vec2(w_img, img_hill->getContentSize().height*(-0.6)));
    this->addChild(img_hill);
    img_hill->runAction(EaseSineOut::create(MoveTo::create(0.75f, Vec2(w_img, img_hill->getContentSize().height*0.45))));
    img_hill->setName("img_hill");
//    img_hill->setVisible(false);
}


void StoryTheEndPage::showViewWhenEndGame(){
    AUDIO_ENGINE->playEffect("theendpage/endingpage_yeahyeah.mp3");
    auto img_title = ImageView::create("mjstory/theendpage/theendpage_title_done.png");
    if(CONFIG_MANAGER->isVietnameseStories()){
        img_title->loadTexture("mjstory/theendpage/theendpage_vmonkey.png");
    }
    
    auto w_img = XHSLib->getSizeScreen().width/2;
    img_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    img_title->setPosition(Vec2(w_img, XHSLib->getSizeScreen().height*1.3));
    img_title->setName("img_title");
    this->addChild(img_title, 10);
    
    monkey_dance = GAFWrapper::createGAF("gaf/khi_dance/khi_dance.gaf");
    img_title->addChild(monkey_dance);
//    monkey_dance->setPosition(Vec2(img_title->getContentSize().width*0.57, 90));
    monkey_dance->setPosition(Vec2(img_title->getContentSize().width*0.35, -320));
    monkey_dance->setVisible(false);

    auto img_hill = ImageView::create("mjstory/theendpage/theendpage_hill.png");
    img_hill->setPosition(Vec2(w_img, img_hill->getContentSize().height*(-0.6)));
    this->addChild(img_hill);
    img_hill->runAction(EaseSineOut::create(MoveTo::create(1.0f, Vec2(w_img, img_hill->getContentSize().height*0.45))));
    img_hill->setName("img_hill");
    
    std::string mc_audio_path = "theendpage/endingpage_welldonemc.mp3";
    if (CONFIG_MANAGER->isVietnameseStories()) {
//        mc_audio_path = StringUtils::format("theendpage/TuyetVoi_%d.mp3", random(1, 2)) ;
        mc_audio_path = "theendpage/TuyetVoi_1.mp3" ;
    }
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(mc_audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        AUDIO_ENGINE->playEffect("theendpage/endingpage_weldonebg.mp3");
        AUDIO_ENGINE->playEffect("theendpage/endingpage_tungho.mp3");
    });
    monkey_dance->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        phao_giay->setVisible(true);
        phao_trai->setVisible(true);
        phao_phai->setVisible(true);
        phao_hoa->setVisible(true);
        monkey_dance->setVisible(true);
        AUDIO_ENGINE->playEffect("theendpage/endingpage_monkeyjump.mp3");
        monkey_dance->play(false);
        monkey_dance->runAction(Sequence::create(DelayTime::create(3.75f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_monkeyjump.mp3");
        }), NULL));
        phao_giay->play(true);
        phao_trai->play(true);
        phao_phai->play(true);
        phao_hoa->play(true);
    }), DelayTime::create(4.0f), CallFunc::create([=]{
        img_title->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f, 1.1f), CallFunc::create([=]{
            monkey_dance->pauseAnimation();
            AUDIO_ENGINE->playEffect("theendpage/endingpage_titleout.mp3");
            img_title->runAction(MoveBy::create(0.5f, Vec2(0, 600)));
            img_hill->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -img_hill->getContentSize().height)), CallFunc::create([=]{
                this->showRecomentStories();
            }), NULL));
            
            phao_giay->removeFromParent();
            phao_trai->removeFromParent();
            phao_phai->removeFromParent();
            phao_hoa->removeFromParent();
            
        }), NULL));
    }), NULL));
    
    img_title->runAction(EaseSineOut::create(MoveTo::create(1.0f, Vec2(w_img, XHSLib->getSizeScreen().height+20))));
}

void StoryTheEndPage::showFireWork(){
    phao_giay = GAFWrapper::createGAF(HSLib::anims[2]);
    phao_giay->setPosition(Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height/2));
    this->addChild(phao_giay, 100);
    phao_giay->play(true);
    
    phao_trai = GAFWrapper::createGAF(HSLib::anims[3]);
    phao_trai->setPosition(Vec2(250, 330));
    this->addChild(phao_trai, 100);
    phao_trai->play(true);
    
    
    phao_phai = GAFWrapper::createGAF(HSLib::anims[3]);
    phao_phai->setScaleX(-1);
    phao_phai->setPosition(Vec2(XHSLib->getSizeScreen().width - 250, 330));
    this->addChild(phao_phai, 100);
    phao_phai->play(true);
    
    phao_hoa = GAFWrapper::createGAF(HSLib::anims[4]);
    phao_hoa->setPosition(Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height/2));
    this->addChild(phao_hoa, 100);
    phao_hoa->play(true);
    
    
    phao_giay->setVisible(false);
    phao_trai->setVisible(false);
    phao_phai->setVisible(false);
    phao_hoa->setVisible(false);
}

void StoryTheEndPage::updateClipShowView(float dt){
    auto pos_x = line_run->getPositionX();
    if (pos_x <= 0) return;
    if (pos_x >= size_w) {
        this->unschedule(schedule_selector(StoryTheEndPage::updateClipShowView));
        this->setContentSize(cocos2d::Size(size_w, size_h));
        line_run = nullptr;
        if (endingpage_type == ENDPAGE_TYPE) {
            showViewWhenEndPage();
        }
    }else {
        this->setContentSize(cocos2d::Size(pos_x, size_h));
    }
}

#pragma mark - onEnter
void StoryTheEndPage::onEnter() {
    Layout::onEnter();
    didLoadFromCSB();
    
    if (endingpage_type == ENDPAGE_TYPE) {
        auto path_file = StringUtils::format("%d_%d_learning.plist", XHSLib->story_id, XHSLib->lang_id);
        auto check = FileUtils::getInstance()->isFileExist(path_file);
        if (!check) {
            cocos2d::MessageBox("Not Found You Have Learn", "Error!");
            return;
        }
        auto learning = FileUtils::getInstance()->getValueMapFromFile(path_file);
        auto map_content = learning["learn"].asValueMap();
        _type = map_content["category"].asInt();
        title->setString(map_content["intro"].asString());
        
        path_audio = map_content["path_audio"].asString();
        if (!map_content["text_info"].isNull() && map_content["text_info"].getType() == Value::Type::VECTOR) {
            auto list_text = map_content["text_info"].asValueVector();
            if (_type != Other_Type) {
                addTextKeyWithList(list_text);
            }else{
                path_audio_other = list_text[0].asValueMap()["audio_path"].asString();
                addTextOtherCategory(list_text[0].asValueMap()["text"].asString());
            }
        }
        
        if (title->getContentSize().width > XHSLib->getSizeScreen().width*0.8) {
            title->setTextAreaSize(cocos2d::Size(XHSLib->getSizeScreen().width*0.8, 100));
            title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            title->setTextVerticalAlignment(cocos2d::TextVAlignment::TOP);
        }
    }else{
        showFireWork();
        this->runAction(Sequence::create(DelayTime::create(0.75f), CallFunc::create([=]{
            this->showViewWhenEndGame();
        }), NULL));
    }
}

void StoryTheEndPage::addTextKeyWithList(ValueVector list_key){
    
    auto layout_conten = this->getChildByName<Layout *>("layout_content");
    layout_conten->setLocalZOrder(1000);
    int size_key = (int)list_key.size();
    size_key = min(size_key, 8);
    auto cot = 1, hang1 = size_key, hang2 = 0;
    if (size_key > 3) {
        cot = 2;
        hang2 = size_key/2;
        hang1 = hang2 + size_key%2;
    }
    
    //left = 110
    //middle = 150
    
    std::vector<Vec2> list_pos;
    std::vector<float> list_x = {-50, 50};
    auto start_y = 160.0f;
    auto heigh_item = 80.0f;
    auto max_w_go = 350.0f;
    if (cot == 1) {
        list_x[0] = 0;
        max_w_go = 650.0f;
    }
    
    if (hang1 == 3) {
        start_y = 195.0f;
        heigh_item = 90.0f;
    }else if (hang1 == 2){
        start_y = 230.0f;
        heigh_item = 100.0f;
    }else if (hang1 == 1){
        start_y = 285.0f;
        heigh_item = 0;
    }
    
    int count_pos = 0;
    for (int i = 0; i < cot; i++) {
        for (int j = hang1-1; j >= 0; j--) {
            list_pos.push_back(Vec2(list_x[i], start_y + heigh_item*j));
            count_pos++;
            
            if (count_pos >= size_key) {
                break;
            }
        }
        
        if (count_pos >= size_key) {
            break;
        }
    }
    
    auto min_w_go = 197.0f;
    auto min_h_go = 73.0f;
    auto  width_text = min_w_go;
    for (int i = 0; i < size_key; i++) {
        auto khucgo = ui::Scale9Sprite::create("mjstory/theendpage/theendpage_manhgo.png");
        khucgo->setPreferredSize(Size(min_w_go, min_h_go));
        layout_conten->addChild(khucgo);
        khucgo->setPosition(list_pos[i]);
        khucgo->setCascadeOpacityEnabled(true);
        auto text = Label::createWithTTF(list_key[i].asValueMap()["text"].asString(), "fonts/monkeystories_medium.ttf", 36);
        khucgo->addChild(text);
        if (cot == 2) {
            if (i < hang1) {
                khucgo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            }else {
                khucgo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            }
        }
        
        if (text->getContentSize().width > width_text) {
            width_text = text->getContentSize().width;
        }
        
        text->setPosition(Vec2(khucgo->getContentSize().width/2, khucgo->getContentSize().height/2));
        text->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        text->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        khucgo->setScale(0);
        list_text.push_back(text);
        list_khucgo.push_back(khucgo);
        
    }
    CCLOG("list_khucgo[i]->getBoundingBox().size.w = %f", min_w_go);

    if (width_text > max_w_go) {
        width_text = max_w_go;
    }
    
    for (int i = 0; i < size_key; i++) {
        list_khucgo[i]->setPreferredSize(cocos2d::Size(width_text+75, min_h_go));
        if (list_text[i]->getContentSize().width >= width_text) {
            list_text[i]->setScale(width_text/list_text[i]->getContentSize().width);
            CCLOG("list_khucgo[i]->getBoundingBox().size.w = %f", list_khucgo[i]->getContentSize().width);
        }
        list_text[i]->setPosition(Vec2(list_khucgo[i]->getContentSize().width/2, list_khucgo[i]->getContentSize().height/2+5));
    }
}

void StoryTheEndPage::addTextOtherCategory(std::string other_str){
    auto layout_conten = this->getChildByName<Layout *>("layout_content");
    layout_conten->setLocalZOrder(1000);
    auto khucgo = ui::Scale9Sprite::create("mjstory/theendpage/theendpage_manhgo_2.png");
    khucgo->setPreferredSize(Size(100, 100));
    layout_conten->addChild(khucgo);
    khucgo->setPosition(Vec2(0, 285.0f));
    khucgo->setCascadeOpacityEnabled(true);
    khucgo->setOpacity(0);
    
    auto text = Label::createWithTTF(other_str, "fonts/monkeystories_medium.ttf", 36);
    khucgo->addChild(text);
    list_khucgo.push_back(khucgo);
    auto max_w_go = 700.0f;
    auto min_h_go = 134;
    if (text->getContentSize().width > max_w_go) {
        text->setWidth(max_w_go);
    }else {
        max_w_go = text->getContentSize().width;
    }
    auto numline = text->getStringNumLines();
    if (numline == 2) {
        min_h_go = min_h_go*1.25;
    }else if (numline == 3){
        min_h_go = min_h_go*1.5;
    }else if (numline == 4){
        min_h_go = min_h_go*1.75;
    }
    khucgo->setPreferredSize(Size(max_w_go + 125, min_h_go));
    
    text->setPosition(Vec2(khucgo->getContentSize().width/2, khucgo->getContentSize().height/2-17));
    list_str = explode(text->getString(), ' ');
    text->setString("");
    list_text.push_back(text);
}

void StoryTheEndPage::showYouHaveLearn(){
    auto check_intro_audio = true;
    if (path_audio == "") {
        check_intro_audio = false;
    }else {
        auto check_path = path_audio[path_audio.length()-1] == '/';
        if (!cocos2d::FileUtils::getInstance()->isFileExist(path_audio) || check_path)
        {
            check_intro_audio = false;;
        }
    }
    
    if (check_intro_audio) {
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            if (_type != Other_Type) {
                showAnimKeyWord();
                title->runAction(Sequence::create(DelayTime::create((int) list_text.size()*0.15f), CallFunc::create([=]{
                    showBtPlay();
                }), NULL));
                
            }else {
                showAnimOtherCategory();
            }
            
        });
        
    }else {
        if (_type != Other_Type) {
            showAnimKeyWord();
            title->runAction(Sequence::create(DelayTime::create((int) list_text.size()*0.15f), CallFunc::create([=]{
                showBtPlay();
            }), NULL));
            
        }else {
            showAnimOtherCategory();
        }
        
    }
}

void StoryTheEndPage::showAnimKeyWord(){
    for (int i = 0; i < (int) list_khucgo.size(); i++) {
        this->runAction(Sequence::create(DelayTime::create(i*0.15f), CallFunc::create([=]{
            showTextWithAnim(list_khucgo[i]);
        }), NULL));
    }
}

void StoryTheEndPage::showTextWithAnim(ui::Scale9Sprite *khucgo){
//    CCLOG("text->getWidth() = %f", text->getWidth());
    AUDIO_ENGINE->playEffect("theendpage/theendpage_showword.mp3");
//    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    text->setPositionX(text->getPositionX() + text->getWidth()/2);
    auto scale_to = EaseBackOut::create(ScaleTo::create(0.25f, 1.0f));
    auto fade = FadeIn::create(0.25f);
    auto y1 = 4, y2 = 2;
    auto duration = 0.04f;
    auto move1 = MoveBy::create(duration, Vec2(0, y1));
    auto move2 = MoveBy::create(duration*2, Vec2(0, -2*y1));
    
    auto move3 = MoveBy::create(duration/2, Vec2(0, y2));
    auto move4 = MoveBy::create(duration, Vec2(0, -2*y2));
    
    auto sequence = Sequence::create(move1->clone(), move2->clone(), move1->clone() ,move1->clone(), move2->clone(), move1->clone(), move1->clone(), move2->clone(), move1->clone(), move3->clone(), move4->clone(), move3->clone(),move3->clone(), move4->clone(), NULL);
    
    khucgo->runAction(Spawn::create(scale_to, sequence, fade, NULL));
}

void StoryTheEndPage::showAnimOtherCategory(){
    //    auto text = list_text[0];
    list_khucgo[0]->runAction(FadeIn::create(0.25f));
    if ((int)list_text.size() == 0) {
        showBtPlay();
    }else this->schedule(schedule_selector(StoryTheEndPage::updateQuesLb), 0.05f);
}

void StoryTheEndPage::updateQuesLb(float time){
    if (count_str >= (int)list_str.size()) {
        this->unschedule(schedule_selector(StoryTheEndPage::updateQuesLb));
        //        list_text[0]->runAction(Sequence::create(ScaleTo::create(0.5f, 1.3f), ScaleTo::create(0.5f, 1.0f),NULL));
        
        auto check_intro_audio = true;
        if (path_audio_other == "") {
            check_intro_audio = false;
        }else {
            auto check_path = path_audio_other[path_audio_other.length()-1] == '/';
            if (!cocos2d::FileUtils::getInstance()->isFileExist(path_audio_other) || check_path)
            {
                check_intro_audio = false;;
            }
        }
        if (check_intro_audio) {
            list_text[0]->setTextColor(Color4B::RED);
            auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio_other);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                list_text[0]->setTextColor(Color4B::WHITE);
                showBtPlay();
            });
        }else {
            showBtPlay();
        }
        
        return;
    }
    AUDIO_ENGINE->playEffect("theendpage/endingpage_textpoup.mp3");
    if (count_str == 0) {
        list_text[0]->setString(list_str[count_str]);
    }else list_text[0]->setString(list_text[0]->getString()+ " " + list_str[count_str]);
    
    //    question_lb->setFontSize(100 - count_str*(100-32)/(int)list_text.size());
    count_str++;
}


void StoryTheEndPage::showBtPlay(){
    AUDIO_ENGINE->playEffect("theendpage/endingpage_playgamebt.mp3");
    auto btplay = this->getChildByName("btplay");
    if (!_isHaveGame) {
        btplay->getChildByName("btskip")->setVisible(false);
        btplay->getChildByName<Text *>("btname")->setString("Ok");
    }
    btplay->setLocalZOrder(1000);
    btplay->setVisible(true);
    auto posY = btplay->getPositionY();
    btplay->setPositionY(-200);
    auto duration = 0.75f;
    btplay->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(duration, Vec2(btplay->getPositionX(), posY))), NULL));
    btplay->runAction(FadeIn::create(duration+0.25f));
}

void StoryTheEndPage::showRecomentStories(){
    auto parent = (StoryPageVC *) this->getParent();
    parent->showRecomentStories();
    parent = nullptr;
    this->removeFromParent();
}

vector<string> StoryTheEndPage::explode(const string& str, const char& ch) {
    string next;
    vector<string> result;
    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}

void StoryTheEndPage::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}


#pragma mark - onExit
void StoryTheEndPage::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

#pragma mark - BUTTON
void StoryTheEndPage::callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest){
    
}

void StoryTheEndPage::onSkip(Ref *sender){
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_skip_game, "story_id", XHSLib->_id);
    auto layout_conten = this->getChildByName<Layout *>("layout_content");
    layout_conten->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layout_conten->setPosition(layout_conten->getPosition()+layout_conten->getContentSize()/2);
    layout_conten->runAction(Sequence::create(FadeOut::create(0.25f), DelayTime::create(0.1f), CallFunc::create([=]{
        this->showRecomentStories();
    }), NULL));
    
    auto btplay = this->getChildByName("btplay");
    btplay->runAction(MoveBy::create(0.25f, Vec2(0, -300)));
    title->runAction(FadeOut::create(0.25f));
    
}
void StoryTheEndPage::onPlayGame(Ref *sender){
    auto bt = static_cast<Button *>(sender);
    bt->setEnabled(false);
    if (_isHaveGame) {
        auto parent = (StoryPageVC *) this->getParent();
        if (!CONFIG_MANAGER->isAppActive()) {
            if (parent->_free_play_type == parent->FREE_OFDAY_TYPE) {
                classes_name = "storypagevc";
                showAlertPlayGame(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_playgame_content"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_playgame_btok"), "", 100);
                parent = nullptr;
                return;
            }
        }
        
        parent->showGame();
        parent = nullptr;
        
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }else{
        onSkip(nullptr);
    }
}


void StoryTheEndPage::onAlertOK(cocos2d::Ref * sender){
    auto _tag = (int)static_cast<StoryAlert *>(sender)->getTag();
    if (_tag == 100) {
        XHSLib->isEndingPageClick = false;
        XHSLib->fontNameStory = "";
        XHSLib->isPopupSubscribeNow = true;
//        DIRECTOR->popScene();
        DIRECTOR->popScene([](Scene * scene) {
            return TransitionSlideInL::create(0.5, scene);
        });
    }
}

void StoryTheEndPage::onAlertCancel(cocos2d::Ref * sender){
    auto _tag = (int)static_cast<StoryAlert *>(sender)->getTag();
    if (_tag == 100) {
        onSkip(nullptr);
    }

}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryTheEndPage::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryTheEndPage, onSkip),
        CLICK_MAP(StoryTheEndPage, onPlayGame),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


