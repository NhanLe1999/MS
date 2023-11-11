//
//  MSPlaylist_Player.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/18.
//

#include "MSPlaylist_Player.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "GAFWrapper.h"
#include "MSPlaylistHelper.h"
#include "MSPlaylistHelper.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

USING_NS_CC;
INITIALIZE_READER(MSPlaylist_Player);

std::once_flag ms_playlist_player_reader_flag;

MSPlaylist_Player * MSPlaylist_Player::createView()
{
    std::call_once(ms_playlist_player_reader_flag, []
    {
        REGISTER_CSB_READER(MSPlaylist_Player);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Playlist_Player.csb");
    
    auto view = reinterpret_cast<MSPlaylist_Player *>(CSLoader::createNode(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB();
        
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return view;
}

void MSPlaylist_Player::didLoadFromCSB()
{
    if(auto title = utils::findChild<ui::Text *>(this, "now_playing_title"))
    {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("playlist.nowplaying"));
    }

    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, this->getContentSize().height));
    ui::Helper::doLayout(this);
    _audiobooks = ms::playlist::ABPlayer::getInstance().getOriginalQueue();
    auto current_playing = ms::playlist::ABPlayer::getInstance().getCurrentPlayingId();
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    auto model = utils::findChild<ui::Layout *>(this, "model");
    model->setContentSize(Size(this->getContentSize().width, model->getContentSize().height));
    ui::Helper::doLayout(model);
    
    listview->removeItem(1);
    
    auto current_listview_index = 0;
    
    bool flag_set_current_playing = false;
    
    for (auto i = 0; i < _audiobooks.size(); ++i)
    {
        auto audiobook = _audiobooks[i];
        
        ui::Widget * item = nullptr;
        
        if (audiobook.book_id == current_playing)
        {
            flag_set_current_playing = true;
            current_listview_index = i;
            item = utils::findChild<ui::Widget *>(this, "now_playing");
            item->setContentSize(Size(this->getContentSize().width, model->getContentSize().height));
            
            auto gaf = GAFWrapper::createGAF("gaf/audio/audio.gaf");
            gaf->setPosition(Point(50, 40));
            item->addChild(gaf);
            gaf->play(true);
            
            ui::Helper::doLayout(item);
        }
        else
        {
            item = model->clone();
        }
        
        item->setTag(audiobook.book_id);
        auto name = item->getChildByName<ui::Text *>("name");
        auto duration = item->getChildByName<ui::Text *>("duration");
        name->setString(audiobook.name);
        
        auto second = (int)round(audiobook.duration / 1000.f);
        auto minute = second / 60;
        second = second % 60;
        duration->setString(StringUtils::format("%d:%02d", minute, second));
        
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(MSPlaylist_Player::onSelectItem, this));
        
        if (audiobook.book_id != current_playing)
        {
            if (flag_set_current_playing)
            {
                listview->pushBackCustomItem(item);
            }
            else
            {
                name->setOpacity(0x88);
                duration->setOpacity(0x88);
                listview->insertCustomItem(item, listview->getItems().size() - 1);
            }
        }
    }
    listview->jumpToTop();
    listview->jumpToItem(current_listview_index, Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE);
}

void MSPlaylist_Player::onEnter()
{
    ui::Layout::onEnter();
}

void MSPlaylist_Player::onDown(cocos2d::Ref *sender)
{
    this->runAction(Sequence::create(FadeOut::create(0.25), RemoveSelf::create(), NULL));
}

void MSPlaylist_Player::onSelectItem(cocos2d::Ref *sender)
{
    auto item = static_cast<ui::Widget *>(sender);
    item->setEnabled(false);
    auto item_id = item->getTag();

    if(auto listview = utils::findChild<ui::ListView *>(this, "listview"))
    {
        auto disableTouchLayout = cocos2d::ui::Button::create();
        disableTouchLayout->setScale9Enabled(true);
        disableTouchLayout->setContentSize(listview->getContentSize());
        disableTouchLayout->setPosition(listview->getPosition()+Vec2(listview->getContentSize()/2));

        auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        indicator->setPosition(disableTouchLayout->getContentSize()/2);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));

        this->addChild(disableTouchLayout,INT_MAX - 1);
        disableTouchLayout->addChild(indicator,INT_MAX - 1);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    while (!_audiobooks.empty() && _audiobooks.front().book_id != item_id)
    {
        _audiobooks.erase(_audiobooks.begin());
    }
    
    ms::playlist::ABPlayer::getInstance().stop();
    
    for (auto i = 0; i < _audiobooks.size(); ++i)
    {
        ms::playlist::ABPlayer::getInstance().enqueue(_audiobooks[i]);
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ms::playlist::ABPlayer::getInstance().playFromBookId(item_id);
#endif
}

ui::Widget::ccWidgetClickCallback MSPlaylist_Player::onLocateClickCallback(const std::string &name) {
    
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(MSPlaylist_Player, onDown)
    };
    
    if (function_map.find(name) != function_map.end())
    {
        return function_map.at(name);
    }
    
    return nullptr;
}

#endif
