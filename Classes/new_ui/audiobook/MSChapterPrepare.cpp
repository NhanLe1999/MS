//
//  MSChapterPrepare.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/15/19.
//

#include "MSChapterPrepare.h"
#include "MJStoryInfoModel.h"

#include "StoryDataModel.h"
#include "MJDefinitions.h"
#include "MSAudiobookPlayer.h"
#include "MSGradeManager.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "APProfileManager.h"

#include "MSPlaylistHelper.h"
#include <map>
#include <numeric>
#include <cctype>
#include "APWaitView.h"
#include "NativeBridge.h"

USING_NS_CC;
INITIALIZE_READER(MSChapter_Prepare);

std::once_flag ms_chapter_prepare_reader_flag;

MSChapter_Prepare * MSChapter_Prepare::createView(mj::model::AudiobookInfo info, std::vector<mj::model::AudiobookInfo> chapters) {
    std::call_once(ms_chapter_prepare_reader_flag, [] {
        REGISTER_CSB_READER(MSChapter_Prepare);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Chapter_Prepare.csb");
    auto view = reinterpret_cast<MSChapter_Prepare *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (view) {
        view->didLoadFromCSB(info, chapters);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

#include "RemoteImageView.h"

void MSChapter_Prepare::didLoadFromCSB(mj::model::AudiobookInfo info, std::vector<mj::model::AudiobookInfo> chapters) {
    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height - 240));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender) {
        this->onBack(nullptr);
    });
    this->addChild(fake_layout);
    
    _info = info;
    _audiobooks = chapters;
    
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_info.name));
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "title"),
            utils::findChild<ui::Text *>(this, "lb.playlist.play"),
            utils::findChild<ui::Text *>(this, "lb.playlist.for"),
            utils::findChild<ui::Text *>(this, "selected_duration"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }

    {
         std::vector<cocos2d::ui::Text*> textSameFonts =
         {
             utils::findChild<ui::Text *>(this, "extra_chapter_title"),
             utils::findChild<ui::Text *>(this, "extra_duration_title"),
             utils::findChild<ui::Text *>(this, "extra_title"),
         };
         
         for(auto text : textSameFonts)
         {
             if(!text)
             {
                 continue;
             }
             
             text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
         }
     }
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
           utils::findChild<ui::Text *>(this, "extra_duration"),
           utils::findChild<ui::Text *>(this, "extra_level"),
           utils::findChild<ui::Text *>(this, "extra"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }
    
    auto temp_name = _info.name;
    
    auto title_bb = title->getBoundingBox();
    if (title_bb.getMaxX() > Director::getInstance()->getVisibleSize().width - 410) {
        auto saved_trim = 0;
        //trim title
        Rect bb = Rect(title_bb.origin.x, title_bb.origin.y, 0, 0);
        for (auto i = 0; i < StringUtils::getCharacterCountInUTF8String(temp_name); ++i) {
            auto letter = title->getLetter(i);
            if (letter) {
                auto bb_letter = letter->getBoundingBox();
                bb_letter.origin = bb_letter.origin + title_bb.origin;
                bb = bb.unionWithRect(bb_letter);
                if (bb.getMaxX() > Director::getInstance()->getVisibleSize().width - 425) {
                    saved_trim = i - 1;
                    break;
                }
            }
        }
        
        StringUtils::StringUTF8 utf = StringUtils::StringUTF8(temp_name);
        while (utf.length() > saved_trim) {
            utf.deleteChar(saved_trim);
        }
        
        temp_name = utf.getAsCharSequence();
        temp_name.erase(temp_name.begin(), std::find_if(temp_name.begin(), temp_name.end(), [](int c) {
            return (!std::isspace(c)) || (!std::isalnum(c));
        }));
        temp_name += "...";
        title->setString(temp_name);
    }
    
    auto button_play = utils::findChild<ui::Button *>(this, "button_play");
    button_play->setCascadeColorEnabled(false);
    
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    
    auto duration = utils::findChild<ui::Button *>(this, "button_duration");
    duration->setCascadeColorEnabled(false);
    auto duration_background = duration->getChildByName("background");
    duration_background->setLocalZOrder(-1);
    duration_background->setCascadeColorEnabled(false);
    duration_background->setColor(mj::helper::colorFromHex(0xe0e0e0));
    if(CONFIG_MANAGER->isVietnameseStories()){
        duration_background->setColor(mj::helper::colorFromHex(0xF6DE00));
    }
    
    for (auto c : duration_background->getChildren()) {
        c->setCascadeColorEnabled(false);
        c->setColor(mj::helper::colorFromHex(0x26B0E6));
        if(CONFIG_MANAGER->isVietnameseStories()){
            c->setColor(mj::helper::colorFromHex(0x6B64FA));
        }
    }
    //    duration->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), NULL)));
    
    auto last_x = title->getPositionX() + title->getContentSize().width;
    dot->setPosition(Point(last_x + 25, dot->getPositionY()));
    line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
    line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    
    auto button_summary = utils::findChild(this, "button_summary");
    button_summary->setCascadeColorEnabled(false);
    
    auto summary_background = utils::findChild(this, "summary_background");
    auto summary_content = utils::findChild<ui::Text *>(this, "summary");
    summary_content->setString(_info.summary);
    mj::helper::setTextWidth(summary_content, 500);
    summary_background->setContentSize(summary_content->getContentSize() + Size(50, 50));
    
    auto summary_overlay = utils::findChild(this, "summary_overlay");
    summary_overlay->setVisible(false);
    
    ui::Helper::doLayout(this);
    
    _model = utils::findChild<ui::Layout *>(this, "model");
//    _scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    _listview = utils::findChild<ui::ListView *>(this, "listview");
    _listview->setTopPadding(25);
//    _model->setContentSize(Size(_scrollview->getContentSize().width, _model->getContentSize().height));
    _model->setContentSize(Size(_listview->getContentSize().width, _model->getContentSize().height));
    _model->addClickEventListener(CC_CALLBACK_1(MSChapter_Prepare::onSelectItem, this));
    ui::Helper::doLayout(_model);
    
    //auto last_time_chapter = MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    auto last_time_chapter = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    if (last_time_chapter == -1) {
        //not played
    } else {
        utils::findChild(this, "lb.playlist.play")->setName("lb.playlist.play.continue");
    }
    
    reloadData();
    
    onChangeDisplayLanguage();
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.update.last.time.played.chapter", [this](EventCustom *) {
        this->reloadData();
    }), this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    ms::playlist::ABPlayer::getInstance().initInfor();

    for (auto it = _audiobooks.begin(); it != _audiobooks.end(); ++it)
    {
        ms::playlist::ABPlayer::getInstance().enqueue(* it);
    }

    ms::playlist::ABPlayer::getInstance().init();

    Director::getInstance()->getEventDispatcher()->addCustomEventListener("EXOPLAYER.onError", [=](EventCustom * eventCustom) {

        const std::string k_alertName = "alertnetwork";

        StoryAlert* alert = static_cast<StoryAlert*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName(k_alertName));

        if(alert) return;

        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");

        alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->setDelegate(this);
        alert->setName(k_alertName);
        alert->setOnExitCallback([=] {

            if(auto audiobookPlayer = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer*>("audiobook_player"))
            {
                audiobookPlayer->playTransitionExit();
            }
            else
            {
                this->removeFromParent();
            }
        });

        auto audiobookPlayer = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer*>("audiobook_player");

        cocos2d::Director::getInstance()->getRunningScene()->addChild(alert,audiobookPlayer != nullptr ? audiobookPlayer->getLocalZOrder():this->getLocalZOrder());

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener("MSChapter_Prepare.setLoadingLayerVisible", [=](EventCustom *) {

        setLoadingLayerVisible(false);

    });

#endif
}

void MSChapter_Prepare::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    auto title = utils::findChild<ui::Text *>(this, "title");
    
    if (dot && line && title) {
        auto last_x = title->getPositionX() + title->getContentSize().width;
        dot->setPosition(Point(last_x + 25, dot->getPositionY()));
        line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
        line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    }
    
    auto summary_overlay = utils::findChild(this, "summary_overlay");
    if (summary_overlay) {
        summary_overlay->setContentSize(size);
        ui::Helper::doLayout(summary_overlay);
    }
    
    auto button_play = utils::findChild(this, "button_play");
    if (button_play) {
        button_play->setPosition(size.width - 300, button_play->getPositionY());
    }
    
    auto collection_layout = utils::findChild(this, "collection");
    if (collection_layout) {
        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto wait_view = cocos2d::ui::Button::create();
        wait_view->setScale9Enabled(true);
        wait_view->setContentSize(collection_layout->getContentSize());
        wait_view->setPosition(collection_layout->getContentSize()/2);
        wait_view->setName("waiting_view");

        auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        indicator->setPosition(wait_view->getContentSize()/2);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
        wait_view->addChild(indicator);

        collection_layout->addChild(wait_view,INT_MAX - 1);
        wait_view->setVisible(false);
#endif
    }
    
    auto controller = utils::findChild(this, "controller");
    if (controller) {
        controller->setPositionX(size.width / 2);
    }
    
    auto button_summary = utils::findChild(this, "button_summary");
    if (button_summary) {
        button_summary->setPosition(Point(size.width - button_summary->getContentSize().width - 25, button_summary->getPositionY()));
    }
    
    if (_listview) {
        _listview->setContentSize(Size(size.width - 100, _listview->getContentSize().height));
        auto items = _listview->getChildren();
        for (auto i : items) {
            i->setContentSize(Size(size.width - 100, i->getContentSize().height));
            i->getChildByName("drag")->setPositionX(size.width - 100);
            i->getChildByName("audiobook_duration")->setPositionX(size.width - 150);
        }
        
        if (_model) {
            _model->setContentSize(Size(_listview->getContentSize().width, _model->getContentSize().height));
        }
        ui::Helper::doLayout(_listview);
    }
    
//    if (_scrollview) {
//        _scrollview->setContentSize(Size(size.width - 100, _scrollview->getContentSize().height));
//        auto items = _scrollview->getChildren();
//        for (auto i : items) {
//            i->setContentSize(Size(size.width - 100, i->getContentSize().height));
//            i->getChildByName("drag")->setPositionX(size.width - 100);
//        }
//
//        if (_model) {
//            _model->setContentSize(Size(_scrollview->getContentSize().width, _model->getContentSize().height));
//            ui::Helper::doLayout(_model);
//        }
//    }
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->onChangeDisplayLanguage();
    }), this);
}

void MSChapter_Prepare::setLoadingLayerVisible(bool i_visible )
{
    if (auto collectionLayout = utils::findChild(this, "collection"))
    {
        if(auto waitingView = collectionLayout->getChildByName("waiting_view"))
        {
            waitingView->setVisible(i_visible);
        }
    }
}

#include <numeric>
void MSChapter_Prepare::onChangeDisplayLanguage() {
    this->enumerateChildren("//audiobook_duration", [](Node * n) {
        auto label = reinterpret_cast<ui::Text *>(n);
        auto str = label->getString();
        str = mj::helper::replace_string(str, "mins", LANGUAGE_MANAGER->getDisplayTextByKey("audiobooks.duration"));
        str = mj::helper::replace_string(str, "phút", LANGUAGE_MANAGER->getDisplayTextByKey("audiobooks.duration"));
        label->setString(str);
        return false;
    });
    
    auto total_duration = std::accumulate(_audiobooks.begin(), _audiobooks.end(), 0, [](int r, mj::model::AudiobookInfo a) {
        return r + a.duration;
    });
    
    auto duration_in_minute = floor(total_duration / 60000.f);
    auto duration_in_second = total_duration / 1000.f - duration_in_minute * 60;
    if (duration_in_second >= 30) {
        duration_in_minute += 1;
    }
    
    auto list_extra = utils::findChild<ui::ListView *>(this, "list_extra");
    list_extra->setGravity(ui::ListView::Gravity::LEFT);
    list_extra->setScrollBarEnabled(false);
    list_extra->setLeftPadding(25);
    list_extra->setItemsMargin(25);
    utils::findChild<ui::Text *>(this, "extra_duration")->setString(StringUtils::format("%0.0f %s", duration_in_minute, LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audiobooks.duration").c_str()));
    utils::findChild<ui::Text *>(this, "extra_chapter")->setString(StringUtils::toString(_audiobooks.size()));
    utils::findChild<ui::Text *>(this, "extra_duration_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audio.player.duration"));
    utils::findChild<ui::Text *>(this, "extra_chapter_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audio.player.number.chapters"));
    
    auto label_summary = utils::findChild<ui::Text *>(this, "label_summary");
    label_summary->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    label_summary->setString(LANGUAGE_MANAGER->getDisplayTextByKey("label.summary"));

    if (_info.extra.empty()) {
        list_extra->removeItem(2);
    } else {
        auto extra_split = mj::helper::split(_info.extra, ':');
        if (extra_split.size() > 1) {
            auto text_extra = extra_split[1];
            while (text_extra.front() == ' ') {
                text_extra.erase(text_extra.begin());
            }
            utils::findChild<ui::Text *>(this, "extra")->setString(text_extra);
            if(CONFIG_MANAGER->isVietnameseStories()){
                if(extra_split[0].find("lại") != std::string::npos){
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("author.rewrite"));
                }else{
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
                }
//                utils::findChild<ui::Text *>(this, "extra_title")->setString(extra_split[0] + ":");
            }else{
                utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
            }
        } else {
            utils::findChild<ui::Text *>(this, "extra")->setString(_info.extra);
            utils::findChild<ui::Text *>(this, "extra_title")->setString("");
        }
    }
    
    auto lw = 0.f;
    for (auto item : list_extra->getItems()) {
        auto w = 0.f;
        for (auto c : item->getChildren()) {
            w = MAX(w, c->getContentSize().width);
        }
        item->setContentSize(Size(w, item->getContentSize().height));
        lw += w + list_extra->getItemsMargin();
    }
    lw -= list_extra->getItemsMargin() - list_extra->getLeftPadding();
    list_extra->setContentSize(Size(lw, list_extra->getContentSize().height));
    
    auto controller = utils::findChild<ui::ListView *>(this, "controller");
    
    controller->enumerateChildren("//lb.*", [](Node * n) {
        auto text = dynamic_cast<ui::Text *>(n);
        if (text) {
            text->setString(LANGUAGE_MANAGER->getDisplayTextByKey(text->getName()));
            text->setContentSize(Size(text->getVirtualRendererSize().width + 1, text->getContentSize().height));
        }
        return false;
    });
    
    auto label_play = utils::findChild(this, "lb.playlist.play");
    if (!label_play) {
        label_play = utils::findChild(this, "lb.playlist.play.continue");
    }
    
    if (label_play) {
        label_play->getParent()->setContentSize(Size(label_play->getContentSize().width + 60, label_play->getParent()->getContentSize().height));
    }
    
    controller->enumerateChildren("//lb.playlist.minute", [](Node * n) {
        auto text = dynamic_cast<ui::Text *>(n);
        if (text) {
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
            text->getParent()->setContentSize(Size(text->getPositionX() + text->getContentSize().width + 35, text->getParent()->getContentSize().height));
        }
        return false;
    });
    
    auto background = utils::findChild(controller, "background");
    background->setContentSize(Size(background->getParent()->getContentSize().width, background->getContentSize().height));
    ui::Helper::doLayout(background);
    
    controller->setTouchEnabled(false);
    controller->setScrollBarEnabled(false);
    auto controller_width = (controller->getItems().size() - 1) * controller->getItemsMargin();
    for (auto item : controller->getItems()) {
        controller_width += item->getContentSize().width;
    }
    
    auto dt = (this->getContentSize().width - controller_width) / 4;
    utils::findChild(controller, "controller_space")->setContentSize(Size(dt, 80));
    
    controller->setContentSize(Size(controller_width + dt, 50));
}

void MSChapter_Prepare::onEnter() {
    ui::Layout::onEnter();
    this->unscheduleUpdate();
//    if (_audiobooks.size() * 110 >= _scrollview->getContentSize().height) {
//        this->schedule(schedule_selector(MSChapter_Prepare::updateScrollviewItems), 0.25);
//    } else {
//        this->updateScrollviewItems(0);
//        _scrollview->setTouchEnabled(false);
//        _scrollview->setScrollBarEnabled(false);
//        _scrollview->setInnerContainerSize(Size(_scrollview->getContentSize().width, _audiobooks.size() * 110));
//        _scrollview->setPosition(_scrollview->getPosition() + Point(0, (_scrollview->getContentSize().height - _audiobooks.size() * 110 - 20)));
//        _scroll_velocity = 0;
//    }

}

void MSChapter_Prepare::onExit()
{
    ui::Layout::onExit();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("MSChapter_Prepare.setLoadingLayerVisible");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("EXOPLAYER.onError");
    ms::playlist::ABPlayer::getInstance().stop();
#endif
}

void MSChapter_Prepare::onBack(cocos2d::Ref * sender) {
    this->removeFromParent();
}

void MSChapter_Prepare::onPlay(cocos2d::Ref * sender) {

    if (!NativeBridge::networkAvaiable()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }

    ms::playlist::ABPlayer::getInstance().setCurrentAudioBookPlayerType(MSAudiobookPlayer::MSAudiobookPlayerType::NOVEL_LIST);

    //auto last_time_chapter = MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    auto last_time_chapter = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    if (last_time_chapter == -1) {
        auto duration = utils::findChild<ui::Text *>(this, "selected_duration");
        auto current = std::atoi(duration->getString().c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) //on android platform , we will init exoplayer in didLoadFromCSB function.
        ms::playlist::ABPlayer::getInstance().init();
#endif
        ms::playlist::ABPlayer::getInstance().setDuration(current);
        ms::playlist::ABPlayer::getInstance().setStartTimestamp((long)mj::helper::getTime());
        ms::playlist::ABPlayer::getInstance().setOriginalQueue(_audiobooks);

        if(!_audiobooks.empty())
        {
            setLoadingLayerVisible(true);
            ms::playlist::ABPlayer::getInstance().playFromBookId(_audiobooks[0].book_id, true);
        }

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) //on android platform , we will enqueue in didLoadFromCSB function.
        for (auto a : _audiobooks) {
            ms::playlist::ABPlayer::getInstance().enqueue(a);
        }
#endif
//        onSelectItem(_listview->getItem(0));
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ms::playlist::ABPlayer::getInstance().beginReceivingRemoteEvents();
    #else
//        ms::playlist::ABPlayer::getInstance().playFromBookId(_audiobooks[0].book_id, true);
    #endif
    } else {
        auto index = 0;
        for (auto i = 0; i < (int)_audiobooks.size(); ++i){
            if (_audiobooks[i].book_id == last_time_chapter) {
                index = i;
                break;
            }
        }
        
        onSelectItem(_listview->getItem(index));
    }
}

void MSChapter_Prepare::onChangeDuration(cocos2d::Ref *sender) {
    auto background = reinterpret_cast<Node *>(sender)->getChildByName("background");
    if (background->isVisible()) {
        background->runAction(Sequence::create(FadeOut::create(0.15), Hide::create(), NULL));
    } else {
        background->setVisible(true);
        background->setOpacity(0);
        background->runAction(FadeIn::create(0.15));
    }
}

void MSChapter_Prepare::onSelectDuration(cocos2d::Ref * sender) {
    auto duration = reinterpret_cast<Node *>(sender)->getTag();
    utils::findChild<ui::Text *>(this, "selected_duration")->setString(StringUtils::toString(duration));
    onChangeDuration(utils::findChild(this, "button_duration"));
}

void MSChapter_Prepare::onSummary(cocos2d::Ref *sender) {
    auto summary_overlay = utils::findChild(this, "summary_overlay");
    summary_overlay->setOpacity(0x00);
    summary_overlay->setVisible(true);
    summary_overlay->runAction(FadeIn::create(0.5));
}

void MSChapter_Prepare::onSummaryClose(cocos2d::Ref * sender) {
    auto summary_overlay = utils::findChild(this, "summary_overlay");
    summary_overlay->runAction(Sequence::create(FadeOut::create(0.5), Hide::create(), nullptr));
}

void MSChapter_Prepare::addItem(int index, int state) {
    auto item = reinterpret_cast<ui::Layout *>(_model->clone());
    
    auto info = _audiobooks[index];
    
    auto text = item->getChildByName<ui::Text *>("audiobook_title");
    auto split = mj::helper::split(info.name, ':');
    if (split.size() > 1) {
        while (split[1].front() == ' ')
            split[1].erase(split[1].begin());
        text->setString(split[1]);
    } else {
        text->setString(info.name);
    }
    
    auto duration_in_minute = floor(info.duration / 60000.f);
    auto duration_in_second = info.duration / 1000.f - duration_in_minute * 60;
    if (duration_in_second >= 30) {
        duration_in_minute += 1;
    }
    
    auto text_duration = item->getChildByName<ui::Text *>("audiobook_duration");
    
    if (text_duration)
    {
        
        text_duration->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

        text_duration->setString(StringUtils::format("%0.0f %s", duration_in_minute, LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audiobooks.duration").c_str()));
    }
    
    auto url = _audiobooks[index].thumbnail_url;
    auto chapter_order = utils::findChild<ui::Text *>(item, "chapter_order");
    
    chapter_order->setString(StringUtils::toString(index + 1));
    
    item->setPosition(Point(0, ((int) _audiobooks.size() - index - 1) * 110));
    
    item->getChildByName("check")->setVisible(_audiobooks[index].read);
    item->getChildByName("check")->setLocalZOrder(1);
    item->getChildByName("check")->setPosition(Point(95, 95));
    
    item->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(StringUtils::format("event.audiobook.change.state.read.%d", _audiobooks[index].book_id), [item](EventCustom *) {
        item->getChildByName("check")->setVisible(true);
    }), item);
    
    item->setTag((int) _audiobooks.size() - index - 1);
    
    if (state == 1) {
        item->setBackGroundColor(mj::helper::colorFromHex(0xDAF5FD));
        item->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    } else if (state == 0) {
        item->setOpacity(0xaa);
    }
    
//    _scrollview->addChild(item);
    _listview->pushBackCustomItem(item);
}

void MSChapter_Prepare::reloadData() {
//    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
//    scrollview->removeAllChildren();
//
//    scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, _audiobooks.size() * 110 + 10));
    
    _listview->removeAllItems();
    
    //auto last_time_chapter = MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    auto last_time_chapter = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), -1);
    
    int state = 0;
    if (last_time_chapter == -1) {
        state = 2;
    }
    
    if (last_time_chapter == _audiobooks.back().book_id) {
        //auto last_time_played = MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_time_played_x, last_time_chapter), 0);
        auto last_time_played = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_time_played_x, last_time_chapter), 0);
        if (last_time_played - _audiobooks.back().duration / 1000.f < -10) { //nếu chỉ còn khoảng 10s nữa là hết chương, thì không tiếp tục nghe mà nghe lại từ đầu
            last_time_chapter = -1;
            state = 2;
            
            auto lb = utils::findChild(this, "lb.playlist.play.continue");
            if (lb) lb->setName("lb.playlist.play");
            
            onChangeDisplayLanguage();
        }
    }
    
    int idx = 0;
    
    for (auto i = 0; i < _audiobooks.size(); ++i) {
        if (_audiobooks[i].book_id == last_time_chapter) {
            state = 1;
            idx = i;
        }
        addItem(i, state);
        if (_audiobooks[i].book_id == last_time_chapter) {
            state = 2;
        }
    }
    
    _listview->forceDoLayout();
    _listview->jumpToTop();
    _listview->scrollToItem(idx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

void MSChapter_Prepare::onSelectItem(cocos2d::Ref *sender) {

    if (!NativeBridge::networkAvaiable()) {
        MJDEFAULT->setStringForKey(key_type_error, "No Internet");
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }

    auto wg = reinterpret_cast<ui::Widget *>(sender);
    wg->setTouchEnabled(false);
    wg->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([wg] {
        wg->setTouchEnabled(true);
    }), NULL));
    
    if (_scroll_velocity > 10) {
        return;
    }
    
    auto tag = reinterpret_cast<Node *>(sender)->getTag();
    auto index = (int)_audiobooks.size() - 1 - tag;
    
    auto duration = utils::findChild<ui::Text *>(this, "selected_duration");

    ms::playlist::ABPlayer::getInstance().setCurrentAudioBookPlayerType(MSAudiobookPlayer::MSAudiobookPlayerType::NOVEL_LIST);

    auto current = std::atoi(duration->getString().c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    ms::playlist::ABPlayer::getInstance().init();
#endif
    ms::playlist::ABPlayer::getInstance().setDuration(current);
    ms::playlist::ABPlayer::getInstance().setStartTimestamp((long)mj::helper::getTime());
    ms::playlist::ABPlayer::getInstance().setOriginalQueue(_audiobooks);
    
    //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), _audiobooks[index].book_id);
    PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, _info.book_id), _audiobooks[index].book_id);
    for (auto a : _audiobooks) if (a.book_id != _audiobooks[index].book_id) {
        //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_time_played_x, a.book_id), 0);
        PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_time_played_x, a.book_id), 0);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (auto it = _audiobooks.begin() + index; it != _audiobooks.end(); ++it) {
        ms::playlist::ABPlayer::getInstance().enqueue(* it);
    }
    ms::playlist::ABPlayer::getInstance().beginReceivingRemoteEvents();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    setLoadingLayerVisible(true);

    ms::playlist::ABPlayer::getInstance().playFromBookId(_audiobooks[index].book_id,true, MSAudiobookPlayer::MSAudiobookPlayerType::NOVEL_LIST);

#endif
}

void MSChapter_Prepare::updateScrollviewItems(float dt) {
//    auto viewport = _scrollview->getBoundingBox();
//    viewport.origin = _scrollview->getInnerContainer()->convertToNodeSpace(viewport.origin);
//
//    _scroll_velocity = std::abs(viewport.origin.y - _last_scroll_position_y);
//    _last_scroll_position_y = viewport.origin.y;
//
//    auto top_row = (int)_audiobooks.size() - 1 - ((int)viewport.origin.y + 110) / 110;
//    auto bottom_row = (int)_audiobooks.size() - 1 - (int)(viewport.origin.y + viewport.size.height) / 110;
//
//    bottom_row = std::max(0, bottom_row - 5);
//    top_row = std::min(top_row + 5, (int)_audiobooks.size() - 1);
//
//    auto items = _scrollview->getChildren();
//    bool flag_removed = false;
//    for (auto i : items) {
//        auto tag = i->getTag();
//        tag = (int)_audiobooks.size() - tag -1;
//        if (tag < bottom_row || tag > top_row) {
//            flag_removed = true;
//            i->removeFromParent();
//        }
//    }
//
//    for (auto i = bottom_row; i <= top_row; ++i) {
//        addItem(i);
//    }
//
//    if (flag_removed) {
//        auto action_clear_cache = Sequence::create(DelayTime::create(0.5), CallFunc::create([] {
//            Director::getInstance()->getTextureCache()->removeUnusedTextures();
//            std::cout << Director::getInstance()->getTextureCache()->getCachedTextureInfo() << std::endl;
//        }), NULL);
//        action_clear_cache->setFlags(0x231954);
//        this->stopActionsByFlags(0x231954);
//        this->runAction(action_clear_cache);
//    }
}

ui::Widget::ccWidgetClickCallback MSChapter_Prepare::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSChapter_Prepare, onBack),
        CLICK_MAP(MSChapter_Prepare, onChangeDuration),
        CLICK_MAP(MSChapter_Prepare, onPlay),
        CLICK_MAP(MSChapter_Prepare, onSelectDuration),
        CLICK_MAP(MSChapter_Prepare, onSummary),
        CLICK_MAP(MSChapter_Prepare, onSummaryClose),
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void MSChapter_Prepare::updateLastTimePlayedChapter() {
    reloadData();
}

void MSChapter_Prepare::onAlertOK(cocos2d::Ref * sender)
{
    this->removeFromParentAndCleanup(true);
}

void MSChapter_Prepare::onAlertCancel(cocos2d::Ref * sender)
{
    this->removeFromParentAndCleanup(true);
}
