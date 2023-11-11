//
//  StoryEndingPage.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryEndingPage.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "StoryDataModel.h"
#include "AppDelegate.h"
#include "NativeBridge.h"
#include "StoryPageVC.h"
#include "StoryFreeManager.h"
#include "MJFirebaseListener.h"
#include "StoryFeedback.h"

#include "MSEventLogger.h"
#include "StoryRateAction.h"
#define key_count_loveit "key_count_loveit"
#define key_count_hateit "key_count_hateit"
#include "MSCollectionStoryItem.h"
#include "StoryInfoCover.h"
#include "MSGradeManager.h"
#include "GAFWrapper.h"
#include "MSScreenRecorder.h"
#include "APProfileManager.h"

INITIALIZE_READER_INSTANCE(StoryEndingPageReader);

USING_NS_CC;
using namespace ui;

class RateStoriesListener : public mj::RequestDelegate {
public:
    void onRequestSuccess(int tag, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_ratestories, "");
        delete this;
    }

    void onRequestFailed(int tag, int error_code, std::string error_string, rapidjson::Value data) {
        delete this;
    }
};

StoryEndingPage * StoryEndingPage::createPage() {
    auto p = static_cast<StoryEndingPage *>(CSLoader::createNodeWithVisibleSize("csb/StoryEndingPage.csb"));
    if (p) {
        p->preload();
    }
    return p;
}

void StoryEndingPage::preload() {
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    classes_name = "endingpage";
    this->setClippingEnabled(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        this->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
        this->setAnchorPoint(Vec2(0, 0));
        this->setPosition(Vec2(0, 0));
    }
    
#endif
    bg = this->getChildByName<ImageView *>("bg");
    queslb = bg->getChildByName<Text *>("queslb");
    suggestlb = bg->getChildByName<Text *>("suggestlb");
    auto size_w = Director::getInstance()->getWinSize().width;
    if (bg->getContentSize().width < (size_w+50)) {
        bg->setScaleX((size_w+50)/bg->getContentSize().width);
    }
    
    queslb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "queslovestory"));
    suggestlb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "recoment"));
    queslb->setVisible(false);
    suggestlb->setVisible(false);
    addFaceToRate();
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        suggestlb->setString(LANGUAGE_MANAGER->getDisplayLangId()== LANGUAGE_EN_CODE?"Our recommended stories:": "Cùng đọc tiếp nào!");
    }
}

void StoryEndingPage::didLoadFromCSB() {
    
}
#pragma mark - onEnter
void StoryEndingPage::onEnter() {
    //recommended page
    //from game (finish/skip)
    //from stories
    //from you have learn (skip game)
    
    Layout::onEnter();
    NativeBridge::setCrashlyticsKeyValue(key_crashlytic, StringUtils::format("storyid%d_langid%d_end", XHSLib->story_id, XHSLib->lang_id));
    ani_count_done = 0;
    isShow = false;
}

void StoryEndingPage::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}

#include "GroupPromotion.h"
void StoryEndingPage::runAnimShow(){
//    ms::recorder::stopRecord();
    
    for (int i = 0; i < (int)list_face_rate.size(); i++) {
        list_face_rate[i]->setVisible(true);
    }
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/StoryEndingPage.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    runani->play("action", false);
    runani->setLastFrameCallFunc([=]{
        isShow = true;
        showTextQues();
    });
    
    AUDIO_ENGINE->playEffect("theendpage/endingpage_titledown2.mp3");
    if (!is_hide_face_rate) {
        auto title_doyoulike = this->getChildByName("title_doyoulike");
        title_doyoulike->runAction(Sequence::create(MoveTo::create(0.75f, Vec2(XHSLib->getSizeScreen().width/2, 598)), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_doyoulike.mp3");
        }), NULL));
    }else {
        auto title_recomment = this->getChildByName("title_recomment");
        auto pos_y = 650-35;
        if(CONFIG_MANAGER->isVietnameseStories()){
            pos_y = 550-35;
        }
        title_recomment->runAction(Sequence::create(MoveTo::create(0.75f, Vec2(XHSLib->getSizeScreen().width/2, pos_y)), CallFunc::create([=]{
            onSuggestStories();
            auto parent = (StoryPageVC *) this->getParent();
            if (parent && !parent->is_sample_read){
                mjrate::StoryRateAction::updateActionCompleted();
                if (mjrate::StoryRateAction::shouldShowRate()) {
                    auto rate_action = mjrate::StoryRateAction::createPage(1);
                    Director::getInstance()->getRunningScene()->addChild(rate_action, INT_MAX - 1);
                } else {                
                    GroupPromotion::attempToShow();
                }
            }
            
        }), NULL));
    }
}

void StoryEndingPage::addFaceToRate(){
    is_hide_face_rate = (CONFIG_MANAGER->isAppActive() && StoryDataModel::getInstance()->storyIsRated(XHSLib->_id));
    is_hide_face_rate = true;
    if (is_hide_face_rate){
        for (int i = 0; i < 3; i++) {
            auto layout_face = bg->getChildByName<Layout *>(StringUtils::format("face%d", i+1));
            layout_face->setVisible(false);
            layout_face->setTouchEnabled(false);
            layout_face = nullptr;
        }
    }else {
        for (int i = 0; i < 3; i++) {
            auto layout_face = bg->getChildByName<Layout *>(StringUtils::format("face%d", i+1));
            queslb = bg->getChildByName<Text *>("queslb");
            layout_face->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
            auto monkey_facerate = ImageView::create(StringUtils::format("mjstory/theendpage/theendpage_monkey%d.png", i+1));
            layout_face->addChild(monkey_facerate);
            monkey_facerate->setPosition(layout_face->getContentSize()/2);
            monkey_facerate->setScale(220.0f/monkey_facerate->getContentSize().width);
            list_face_rate.push_back(layout_face);
            auto face_text = layout_face->getChildByName<Text *>("text");
            face_text->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, StringUtils::format("rate%d", i+1)));
            list_face_text.push_back(face_text);
            layout_face = nullptr;
        }
        
        auto title_doyoulike = ImageView::create("mjstory/theendpage/theendpage_doyoulike.png");
        title_doyoulike->setPosition(Vec2(XHSLib->getSizeScreen().width/2, 995));
        title_doyoulike->setName("title_doyoulike");
        this->addChild(title_doyoulike, 100);
       
    }
    
    auto image = "mjstory/theendpage/theendpage_recomment_new.png";
    if(CONFIG_MANAGER->isVietnameseStories()){
        image = "mjstory/monkey_logo_vi_new.png";
    }
    auto title_recomment = ImageView::create(image);
    title_recomment->setPosition(Vec2(XHSLib->getSizeScreen().width/2, 995));
    title_recomment->setName("title_recomment");
    this->addChild(title_recomment, 100);
    
}

void StoryEndingPage::showTextQues(){
    if (is_hide_face_rate) return;
    for (int i = 0; i < 3; i++) {
        list_face_text[i]->runAction(FadeIn::create(0.5f));
    }
    queslb->runAction(FadeIn::create(0.5f));
}


void StoryEndingPage::onSuggestStories(){
    XHSLib->isEndingPageClick = true;
    this->runAction(Sequence::create(DelayTime::create(0.75f), CallFunc::create([=]{
        auto parent = (StoryPageVC *) this->getParent();
        parent->btclose->setVisible(true);
        parent->btclose->runAction(FadeIn::create(0.25f));
        parent = nullptr;
    }), NULL));
    auto list_stories = choose4StoriesSuggest();
    
    auto num_stories = (int)list_stories.size();
    num_stories = MIN(num_stories, 3);
    auto list_suggest = utils::findChild<ui::ListView *>(this, "list_suggest");
    list_suggest->setScrollBarEnabled(false);
    list_suggest->setVisible(true);
    
    auto seed = random(0, 1);
    std::string gaf_name, label;
    auto info = StoryDataModel::getInstance()->getStoryById(XHSLib->_id);
    auto grade_id = ms::GradeManager::getInstance().getGradeIdByLevelId(info.levels[0]);
    
    if (grade_id == 4) {//force audiobooks
        seed = 1;
    }
    
    if (seed) {
        //audiobook
        gaf_name = "gaf/sidebar/audiobooks/audiobooks.gaf";
        label = "Audiobooks";
    } else {
        //stories
        gaf_name = "gaf/sidebar/lessons/lessons.gaf";
        label = "Lessons";
    }
    
    auto gaf = GAFWrapper::createGAF(gaf_name);
    auto button_recommend = utils::findChild<ui::Button *>(this, "button_recommend");
//    button_recommend->setVisible(false);
    button_recommend->addChild(gaf);
    button_recommend->getParent()->setOpacity(0);
    button_recommend->setCascadeOpacityEnabled(true);
    gaf->setCascadeColorEnabled(true);
    gaf->setPosition(Point(button_recommend->getContentSize() / 2) + Point(0, 2));
    gaf->setScale(2.5);
    gaf->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create([](Node * n) {
        auto g = reinterpret_cast<GAFWrapper *>(n);
        g->playSequence("action", false, [g](gaf::GAFObject * obj, const std::string) {
            g->runAction(Sequence::create(DelayTime::create(0.5), CallFuncN::create([](Node * n) {
                auto g = reinterpret_cast<GAFWrapper *>(n);
                g->playSequence("action2", false);
            }), NULL));
        });
    }), NULL));
    button_recommend->setTag(seed);
    button_recommend->setCascadeColorEnabled(false);
    button_recommend->getChildByName<ui::Text *>("label_recommend")->setString(label);
    
    if (grade_id > 4) {
        list_suggest->removeAllItems();
        num_stories = 4;
    }
    
    for (int i = 0; i < num_stories; i++) {
//        auto map_stories = list_stories[i].asValueMap();
        CCLOG("hoisunglog - index = %d", list_stories[i].asInt());
        auto item = MSCollectionStoryItem::createItem(StoryDataModel::getInstance()->getStoryById(list_stories[i].asInt()));
//        StoryHomeStoriesItem* item=(StoryHomeStoriesItem*)CSLoader::createNode("csb/StoryHomeStoriesItem.csb");
        list_suggest->insertCustomItem(item, 0);
//        item->setStoriesItemWithData(map_stories);
//        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        item->setPosition(Vec2(bg->getContentSize().width/2 + (i-1.5f)*(item->getContentSize().width+40), bg->getContentSize().height*0.285));
//        if (i == 0) {
//            suggestlb->setPosition(Vec2(item->getPositionX() - item->getContentSize().width/2 - 5,suggestlb->getPositionY()));
//        }
        
        auto layout_touch = cocos2d::ui::Layout::create();
        layout_touch->setContentSize(item->getContentSize());
        layout_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layout_touch->setPosition(item->getContentSize()/2);
        item->addChild(layout_touch, 1000000);
        layout_touch->setTouchEnabled(true);
        layout_touch->addClickEventListener(CC_CALLBACK_1(StoryEndingPage::onStoryRecomentClicked, this));
        layout_touch->setTag(100+list_stories[i].asInt());
    }
    
    suggestlb->setOpacity(0);
    suggestlb->setVisible(true);
    suggestlb->runAction(FadeIn::create(0.5));
    
    auto delay_time = 0.f;
    auto items = list_suggest->getItems();
    for (auto i = 0; i < 1; ++i) {
        auto c = items.at(i);
        c->setOpacity(0);
        c->runAction(Sequence::create(DelayTime::create(delay_time), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_recommentstory.mp3");
        }), Show::create(), FadeIn::create(0.3), NULL));
        delay_time += 0.3f;
    }
}

void StoryEndingPage::onStoryRecomentClicked(cocos2d::Ref *sender){
    std::string readFrom = MJDEFAULT->getStringForKey(key_read_from) +".recommendation";
    std::string readFromTheSecond = MJDEFAULT->getStringForKey("key_read_from_the_second");
    MJDEFAULT->setStringForKey("key_read_from_the_second", readFromTheSecond + readFrom);
    MJDEFAULT->setStringForKey(key_read_from, readFrom);
    auto story_id = (int)static_cast<cocos2d::ui::Layout *>(sender)->getTag() - 100;
    std::string csb_path = "csb/StoryInfoCover.csb";
    auto _info = StoryDataModel::getInstance()->getStoryById(story_id);
    auto storyinfocover = static_cast<StoryInfoCover *>(CSLoader::createNode(csb_path));
    StoryInfoObj obj;
    obj.name = _info.name;
    obj.mostpopular = _info.total_words;
    obj.readinglevel = _info.levels[0];
    obj.languageid = _info.lang_id;
    obj.storyid = _info.story_id;
    obj.newest = _info.unique_id;
    obj.total_page = _info.total_page;
    obj.word_count = _info.total_words;
    obj.lexile = _info.lexile;
    obj._id = _info.unique_id;
    obj._storyid = _info.story_id;
    obj._version = _info.version;
    obj.downloaded = false;
    obj.thumbnail = _info.thumbnail_url;
    obj.written = _info.author;
    obj.design = _info.designer;
    obj.image = _info.thumbnail_url;
    storyinfocover->initWithData(obj, false);

    MJDEFAULT->setStringForKey("STORY_START_FROM","recommendation");
    auto background = utils::findChild(storyinfocover, "img_book");

    background->setContentSize(Director::getInstance()->getVisibleSize());
    background->setOpacity(0);
    background->runAction(Sequence::create(DelayTime::create(0.25), ResizeTo::create(0.25, Director::getInstance()->getRunningScene()->getContentSize() + Size(10, 10)), nullptr));
    background->runAction(Sequence::create(DelayTime::create(0.25), FadeIn::create(0.25), nullptr));

    DIRECTOR->getRunningScene()->addChild(storyinfocover, 19999999);
}

ValueVector StoryEndingPage::choose4StoriesSuggest(){
    string str_order = "quality DESC, date_publish DESC";
    auto lang_id = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    CCLOG("XHSLib->level_current = %d",XHSLib->level_current);
    //=> lấy từ list stories thuộc level.
    auto list_id_storieslevel = StoryDataModel::getInstance()->getListIDStory(XHSLib->level_current, lang_id, str_order);
    //lọc đã đọc hay chưa.
    ValueVector list_stories_chuadoc;
    list_stories_chuadoc = filterStoriesCHUADOC(list_id_storieslevel, list_choose);
    int size_list_chuadoc = (int)list_stories_chuadoc.size();
    if (size_list_chuadoc >= 4) {//đủ 4 truyện.
        auto index = 0;
        for (int i = 0; i < (int)list_stories_chuadoc.size(); i++) {
            if (list_stories_chuadoc[i].asInt() > XHSLib->_id) {
                index = i;
                break;
            }
            //
        }
        
        int count = 0;
        for (int i = index; i < (int)list_stories_chuadoc.size(); i++) {
            if (count == 4){
                break;
            }
            count++;
            list_choose.push_back(list_stories_chuadoc[i]);
        }
        
        if (count < 4) {
            count = 4 - count;
            for (int i = count; i >= 0; i--) {
                list_choose.push_back(list_stories_chuadoc[i]);
            }
        }
        
        list_stories_chuadoc.clear();
        CCLOG("LEVEL");
    }else{//không đủ 4 truyện
        for (int i = 0; i < size_list_chuadoc; i++) {
            list_choose.push_back(list_stories_chuadoc[i]);
        }
        list_stories_chuadoc.clear();
        auto num_stories_thieu = 4 - size_list_chuadoc;
        //=> lấy từ list NEW
        auto list_popular = getStoriesFromPopular(num_stories_thieu);
        if ((int) list_popular.size() == num_stories_thieu) {//đã đủ 4 truyện.
            for (int i = 0; i < num_stories_thieu; i++) {
                list_choose.push_back(list_popular[i]);
            }
            list_popular.clear();
            CCLOG("POPULAR");
        }else {//Vẫn chưa đủ 4 truyện
            for (int i = 0; i < (int) list_popular.size(); i++) {
                list_choose.push_back(list_popular[i]);
            }
            list_popular.clear();
            num_stories_thieu = 4 - (int) list_choose.size();
            //=> lấy từ list NEW
            auto list_new = getStoriesFromNew(num_stories_thieu);
            if ((int)list_new.size() == num_stories_thieu) {//Đã đủ 4 truyện.
                for (int i = 0; i < num_stories_thieu; i++) {
                    list_choose.push_back(list_new[i]);
                }
                CCLOG("NEW");
                list_new.clear();
            }else {//Vẫn chưa đủ 4 truyện.
                for (int i = 0; i < (int) list_new.size(); i++) {
                    list_choose.push_back(list_new[i]);
                }
                list_new.clear();
                num_stories_thieu = 4 - (int) list_choose.size();
                //=> lấy từ list ALL STORIES
                auto list_all_stories = StoryDataModel::getInstance()->getListIDStory(0, lang_id, str_order);
                auto list_all = list_all_stories;
                list_all_stories = filterStoriesCHUADOC(list_all_stories, list_choose);
                
                if ((int)list_all_stories.size() > num_stories_thieu) {
                    for (int i = 0; i < num_stories_thieu; i++) {
                        auto size_list_all = (int)list_all_stories.size();
                        if (size_list_all == 0) {
                            break;
                        }
                        auto ran = random(0, size_list_all -1);
                        list_choose.push_back(list_all_stories[i]);
                        list_all_stories.erase(list_all_stories.begin()+ran);
                    }
                    CCLOG("ALL CHƯA ĐỌC");
                    list_all_stories.clear();
                    list_all.clear();
                }else{
                    for (int i = 0; i < (int)list_all_stories.size(); i++) {
                        auto size_list_all = (int)list_all_stories.size();
                        if (size_list_all == 0) {
                            break;
                        }
                        auto ran = random(0, size_list_all -1);
                        list_choose.push_back(list_all_stories[i]);
                        list_all_stories.erase(list_all_stories.begin()+ran);
                    }
                    
                    num_stories_thieu = 4 - (int) list_choose.size();
                    for (int i = 0; i < num_stories_thieu+1; i++) {
                        auto size_all = (int)list_all.size();
                        if (size_all == 0) {
                            break;
                        }
                        auto ran = random(0, size_all -1);
                        if (list_all[i].asInt() != XHSLib->_id) {
                            list_choose.push_back(list_all[i]);
                        }
                        list_all.erase(list_all.begin()+ran);
                        
                    }
                    list_all_stories.clear();
                    list_all.clear();
                    CCLOG("ALL CÓ CẢ ĐỌC RỒI");
                }
            }
        }
    }
    return list_choose;
}



ValueVector StoryEndingPage::getStoriesFromPopular(int num_stories){
    ValueVector re_list;
    auto list_popular = filterStoriesCHUADOC(XHSLib->getPopularStory(), list_choose);
    auto size_popular = (int) list_popular.size();
    if (size_popular < num_stories) {
        for (int i = 0; i < size_popular; i++) {
            re_list.push_back(list_popular[i]);
        }
    }else{
        for (int i = 0; i < num_stories ; i++) {
            size_popular = (int) list_popular.size();
            auto rand = random(0, size_popular-1);
            re_list.push_back(list_popular[rand]);
            list_popular.erase(list_popular.begin()+rand);
        }
    }
    return re_list;
    
}
ValueVector StoryEndingPage::getStoriesFromNew(int num_stories) {
    ValueVector re_list;
    auto list_new = filterStoriesCHUADOC(XHSLib->getNewStory(), list_choose);
    auto size_new = (int) list_new.size();
    if (size_new < num_stories) {
        for (int i = 0; i < size_new ; i++) {
            re_list.push_back(list_new[i]);
        }
    }else{
        for (int i = 0; i < num_stories ; i++) {
            size_new = (int) list_new.size();
            auto rand = random(0, size_new-1);
            re_list.push_back(list_new[rand]);
            list_new.erase(list_new.begin()+rand);
        }
    }
    return re_list;
}

ValueVector StoryEndingPage::filterStoriesCHUADOC(ValueVector list_stories, ValueVector list_filter){
    ValueVector list_stories_chuadoc, list_dadoc;
    auto lang_id = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    //auto str_list_dadoc = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_liststories_docroi, lang_id));
    auto str_list_dadoc = PROFILE_MANAGER.getStringForKey(StringUtils::format("%s%d", key_liststories_docroi, lang_id));
    if (str_list_dadoc != "") {
        list_dadoc = JsonDump::getInstance()->dumpValueVectorFromString(str_list_dadoc);
    }
    
    for (int i = 0; i < (int) list_stories.size(); i++) {
        auto check = true;
        for (int j = 0; j < (int) list_dadoc.size(); j++) {
            if (list_stories[i].asInt() == list_dadoc[j].asInt()) {
                check = false;
                break;
            }
        }
        
        if (check) {
            for (int j = 0; j < (int) list_filter.size(); j++) {
                if (list_stories[i].asInt() == list_filter[j].asInt()) {
                    check = false;
                    break;
                }
            }
        }
        
        if (check) {
            CCLOG("list_id_storieslevel = %d", list_stories[i].asInt());
            list_stories_chuadoc.push_back(list_stories[i]);
        }
    }
    return list_stories_chuadoc;
}

ValueVector StoryEndingPage::filterStoriesWithListStories(ValueVector list_stories, ValueVector list_filter){
    ValueVector list_stories_chuadoc;
    for (int i = 0; i < (int) list_stories.size(); i++) {
        auto check = true;
        for (int j = 0; j < (int) list_filter.size(); j++) {
            if (list_stories[i].asInt() == list_filter[j].asInt()) {
                check = false;
                break;
            }
        }
        if (check) {
            CCLOG("list_id_storieslevel = %d", list_stories[i].asInt());
            list_stories_chuadoc.push_back(list_stories[i]);
        }
    }
    return list_stories_chuadoc;
}


#pragma mark - onExit
void StoryEndingPage::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
    
}

#pragma mark - BUTTON

void StoryEndingPage::onRate(cocos2d::Ref *sender){
    if (isTapRate) return;
    AUDIO_ENGINE->playEffect("theendpage/endingpage_rateclick.mp3");
    isTapRate = true;
    AUDIO_ENGINE->playEffect("sounds/endingpage_bongno.mp3");
    auto tag = (int)static_cast<Layout *>(sender)->getTag()-11;
    CCLOG("tag = %d", tag);
    auto face = list_face_rate[tag];
    auto dich_x = 5, dich_y = 5;
    
    Vec2 pos_to = Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height*0.6);
    pos_to = face->getParent()->convertToNodeSpace(pos_to);
    if (tag == 1) {
        dich_x = 0;
        dich_y = 0;
    }else if (tag == 2){
        dich_x = -5;
    }
    pos_to += Vec2(2*dich_x, 2*dich_y);
    
    auto callback = CallFunc::create([=]{
        face->removeFromParent();
        list_face_text.clear();
        list_face_rate.clear();
        if (tag == 0) {
            if (!XHSLib->isFavoritesStory(XHSLib->_id)) {
                XHSLib->addFavoritesStory(XHSLib->_id);
            }
            showRatePopup();
        }
        
        if (tag == 2) {
            showFeedbackPopup();
        }
        
        StoryDataModel::getInstance()->setStoryIsRated(XHSLib->_id, tag+1);
        
        auto title_recomment = this->getChildByName("title_recomment");
        auto pos_y = 700-35;
        if(CONFIG_MANAGER->isVietnameseStories()){
            pos_y = 550-35;
        }
        title_recomment->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(XHSLib->getSizeScreen().width/2, pos_y)), CallFunc::create([=]{
            onSuggestStories();
        }), NULL));
        
    });
    if (tag == 1) {
        face->runAction(Sequence::create(Spawn::create(EaseBackOut::create(MoveTo::create(0.3f, pos_to)), EaseBackOut::create(ScaleTo::create(0.5f, 2.0f)), NULL), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_thankyou.mp3");
        }), DelayTime::create(1.5f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_titleout.mp3");
        }), EaseBackIn::create(MoveTo::create(0.5f, Vec2(pos_to.x, -XHSLib->getSizeScreen().height/2))), callback,NULL));

    }else {
        face->runAction(Sequence::create(Spawn::create(MoveTo::create(0.3f, pos_to), ScaleTo::create(0.3f, 2.0f), NULL), MoveBy::create(0.15f, Vec2(-3*dich_x, -3*dich_y)), MoveBy::create(0.15f, Vec2(dich_x, dich_y)),CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_thankyou.mp3");
        }), DelayTime::create(1.5f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/endingpage_titleout.mp3");
        }), EaseBackIn::create(MoveTo::create(0.5f, Vec2(pos_to.x, -XHSLib->getSizeScreen().height/2))), callback,NULL));
    }
    auto title_doyoulike = this->getChildByName("title_doyoulike");
    title_doyoulike->runAction(MoveTo::create(0.4f, Vec2(XHSLib->getSizeScreen().width/2, 995)));

    
    list_face_rate.erase(list_face_rate.begin()+tag);
    queslb->removeFromParent();
    for (int i = 0; i < (int)list_face_rate.size(); i++) {
        list_face_rate[i]->removeFromParent();
    }
    
    XHSLib->addRateStory(XHSLib->_id, tag+1);

    auto request = new mj::Request(mj::Request::ActionType::RATE_STORY, {
        {"ratecontent", MJDEFAULT->getStringForKey(key_ratestories)}
    });
    request->setDelegate(new RateStoriesListener());
    request->send();
    
//    ValueMap _dict_event;
//    _dict_event["rate_id"] = tag;
//    _dict_event["story_id"] = XHSLib->_id;
//    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsParametersEvent(analytics_event_rate_story, _dict_event);
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(cocos2d::StringUtils::format("%s_%s", analytics_event_rate_story, StringUtils::toString(tag).c_str()), "story_id", StringUtils::toString(XHSLib->_id));
    
    auto info = StoryDataModel::getInstance()->getStoryById(XHSLib->_id);
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventRateStory, {
        {ms::analytics::kPropertyStoryId, cocos2d::Value(info.unique_id)},
        {ms::analytics::kPropertyStoryName, cocos2d::Value(info.name)},
        {ms::analytics::kPropertyRateType, cocos2d::Value(tag)}
    });
}


void StoryEndingPage::showRatePopup(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return;
#endif
    auto count_loveit = MJDEFAULT->getIntegerForKey(key_count_loveit);
    MJDEFAULT->setIntegerForKey(key_count_loveit, count_loveit++);
    if (MJDEFAULT->getBoolForKey(key_rate_app) || MJDEFAULT->getBoolForKey(key_rate_app_won)) return;
    if (CONFIG_MANAGER->isAppActive()) {
        if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) {
            if (MJDEFAULT->getIntegerForKey(key_count_loveit) >= 10) {
                MJDEFAULT->setIntegerForKey(key_count_loveit, 0);
                auto parent = (StoryPageVC *) this->getParent();
                parent->showRatePopupBuy();
                parent = nullptr;
            }
        }
    }else {
        if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) {
            //đã sang ngày thứ 2.
            auto parent = (StoryPageVC *) this->getParent();
            parent->showRatePopup();
            parent = nullptr;
        }
    }
}

void StoryEndingPage::showFeedbackPopup(){
    auto count_hateapp = MJDEFAULT->getIntegerForKey(key_count_hateit);
    MJDEFAULT->setIntegerForKey(key_count_hateit, count_hateapp++);
    if (MJDEFAULT->getBoolForKey(key_rate_app) || MJDEFAULT->getBoolForKey(key_rate_app_won)) return;
    if (CONFIG_MANAGER->isAppActive()) {
        if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) {
            if (MJDEFAULT->getIntegerForKey(key_count_hateit) >= 10) {
                MJDEFAULT->setIntegerForKey(key_count_hateit, 0);
                auto feedback = StoryFeedback::createPage();
                DIRECTOR->getRunningScene()->addChild(feedback, 99999998);
            }
        }
    }else {
        if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) {
            //đã sang ngày thứ 2.
            auto feedback = StoryFeedback::createPage();
            DIRECTOR->getRunningScene()->addChild(feedback, 99999998);
        }
    }
}


void StoryEndingPage::onClickButton(cocos2d::Ref *sender){
    XHSLib->isEndingPageClick = false;
    XHSLib->fontNameStory = "";
    DIRECTOR->popScene([](Scene * scene) {
        return TransitionSlideInL::create(0.5, scene);
    });
    return;
    
}

void StoryEndingPage::callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest){
    
}
#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryEndingPage::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryEndingPage, onClickButton),
        CLICK_MAP(StoryEndingPage, onRate),
        CLICK_MAP(StoryEndingPage, onRecommendTab)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void StoryEndingPage::onRecommendTab(Ref * sender) {
    CCLOG("go to tab");
    XHSLib->isEndingPageClick = false;
    
    auto info = StoryDataModel::getInstance()->getStoryById(XHSLib->_id);
    auto grade_id = ms::GradeManager::getInstance().getGradeIdByLevelId(info.levels[0]);
    auto grade_info = ms::GradeManager::getInstance().getGradeInfoById(grade_id);

    auto seed = reinterpret_cast<Node *>(sender)->getTag();
    if (seed == 0) {
        if (grade_id == 1 || grade_id == 2) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
            ms::GradeManager::getInstance().setCurrentGradeId(216);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        } else if (grade_id == 3) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
            ms::GradeManager::getInstance().setCurrentGradeId(217);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        } else if (grade_id == 4) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
            ms::GradeManager::getInstance().setCurrentGradeId(375);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        }
        
        MJDEFAULT->setBoolForKey(key_event_go_to_lessons_page, true);
        Director::getInstance()->popScene([](Scene * s) {
            return TransitionSlideInL::create(0.25, s);
        });
        
//        Director::getInstance()->getScheduler()->schedule([](float) {
//            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_lessons_page);
//        }, Application::getInstance(), 0, 0, 0.5, false, "delay_event_after_popscene_audiobooks");
    } else {
        if (grade_id == 1) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
            ms::GradeManager::getInstance().setCurrentGradeId(84);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        } else if (grade_id == 3) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
            ms::GradeManager::getInstance().setCurrentGradeId(217);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        } else if (grade_id == 2) {
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
            ms::GradeManager::getInstance().setCurrentGradeId(216);
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        }
        
        MJDEFAULT->setBoolForKey(key_event_go_to_audiobooks_page, true);
        Director::getInstance()->popScene([](Scene * s) {
            return TransitionSlideInL::create(0.25, s);
        });
        
//        Director::getInstance()->getScheduler()->schedule([](float) {
//            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_audiobooks_page);
//        }, Application::getInstance(), 0, 0, 0.5, false, "delay_event_after_popscene_lessons");
    }
}
