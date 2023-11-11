
#include "MSCacheManager.h"
#include "StoryInfoCover.h"
#include "MSAudiobookPlayer.h"
#include "MSCollectionAudiobookItem.h"

namespace ms {
    CacheManager::CacheManager() {

    }

    void CacheManager::onBackStory()
    {
        _index = _index - 2;
        beginAutoPlay(true);
    }

    void CacheManager::onSkipStory()
    {
        beginAutoPlay(true);
    }

    void CacheManager::onBackAudiobook()
    {
        _indexAudiobook = _indexAudiobook - 2;
        beginAutoPlayAudiobook(true);
    }

    void CacheManager::onSkipAudibook()
    {
        beginAutoPlayAudiobook(true);
    }

    void CacheManager::setAutoAudiobook(int id, mj::model::AudiobookInfo info)
    {
        mj::helper::playButtonFXOpen();

        auto overlay = ui::Layout::create();
        overlay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        overlay->setBackGroundColor(Color3B::BLACK);
        overlay->setOpacity(0);
        overlay->runAction(FadeTo::create(0.5, 220));
        overlay->setTouchEnabled(true);
        overlay->setContentSize(Director::getInstance()->getVisibleSize());
        Director::getInstance()->getRunningScene()->addChild(overlay, 1001000);
        overlay->scheduleOnce([overlay](float dt) {
            overlay->removeFromParent();
            }, 1.6, "dismiss_touch_block");

        auto player = MSAudiobookPlayer::createView(info);
        Director::getInstance()->getRunningScene()->addChild(player, 1001001);
        player->setName(mjhelper_name_audiobookplayer);
        player->setVisible(false);

        auto bb = player->getBoundingBox();
        bb.origin = player->convertToNodeSpace(bb.origin);

        auto new_item = MSCollectionAudiobookItem::createThumb(info);
        new_item->setName(mjhelper_name_collectionaudiobookitem);
        new_item->setAnchorPoint(Point::ANCHOR_MIDDLE);
        new_item->setPosition(Point(bb.getMidX(), bb.getMidY()));
        Director::getInstance()->getRunningScene()->addChild(new_item, 1001002);

        bb = player->getThumbBoundingBox();
        new_item->runAction(EaseExponentialOut::create(MoveTo::create(0.5, Point(bb.getMidX(), bb.getMidY()) + Point(0, 10))));
        new_item->runAction(EaseBackOut::create(ScaleTo::create(1, 1.25)));
        new_item->disableNewRead();

        player->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([player, new_item] {
            new_item->removeFromParentAndCleanup(false);
            auto container = cocos2d::utils::findChild(player, "thumb_container");
            auto p = new_item->getPosition();
            p = container->convertToNodeSpace(p);
            new_item->setPosition(p);
            container->addChild(new_item);

            player->setVisible(true);
            player->playTransitionEnter();
            }), NULL));
    }

    void CacheManager::setAutoStory(int id)
    {
        auto info = StoryDataModel::getInstance()->getStoryById(id);

        //if(info.)

        auto storyinfocover = static_cast<StoryInfoCover*>(CSLoader::createNode("csb/StoryInfoCover.csb"));
        StoryInfoObj obj;
        obj.name = info.name;
        obj.mostpopular = info.total_words;
        if (!info.levels.empty())
        {
            obj.readinglevel = info.levels[0];
        }
        obj.languageid = info.lang_id;
        obj.storyid = info.story_id;
        obj.newest = info.unique_id;
        obj.total_page = info.total_page;
        obj.word_count = info.total_words;
        obj.lexile = info.lexile;
        obj._id = info.unique_id;
        obj._storyid = info.story_id;
        obj._version = info.version;
        obj.downloaded = false;
        obj.thumbnail = info.thumbnail_url;
        obj.written = info.author;
        obj.design = info.designer;
        obj.image = info.thumbnail_url;
        obj.typeStory = mj::model::ActInfo::Type::STORY;
        storyinfocover->is_activity_play = true;

        storyinfocover->initWithData(obj, false);

        Director::getInstance()->getRunningScene()->addChild(storyinfocover, 1000);
    }

    void CacheManager::beginAutoPlay(bool isNotSkipAndBack)
    {
        if (_listInfoStoryAutoPlay.empty() || _index >= _listInfoStoryAutoPlay.size() || _index < 0)
        {
            _index = 0;
            MJDEFAULT->setIntegerForKey("Key_story_id_auto_play", -1);
            return;
        }
         
        int idStory = _listInfoStoryAutoPlay[_index].unique_id;

        if (!isNotSkipAndBack)
        {
            auto id = MJDEFAULT->getIntegerForKey("Key_story_id_auto_play", -1);
            if (_idStoryIsAutoPlay == -1)
            {
                if (id == -1)
                {

                }
                else if (id != _listInfoStoryAutoPlay[_index].unique_id) {
                    _index++;
                    beginAutoPlay();
                    return;
                }
            }
        }
        
        _idStoryIsAutoPlay = 0;
        if (!isNotSkipAndBack)
        {
            MJDEFAULT->setIntegerForKey("Key_story_id_auto_play", idStory);
        }

        if (idStory == 0 || idStory == -1)
        {
            beginAutoPlay(isNotSkipAndBack);
            return;
        }

        setAutoStory(idStory);
        _index++;
    }

    void CacheManager::setListInfoAutoPlay()
    {
        if (_listStoryInfo.count(0) > 0)
        {
            _listInfoStoryAutoPlay = _listStoryInfo.at(0);
        }
        else if (_listStoryInfo.count(1) > 0)
        {
            _listInfoStoryAutoPlay = _listStoryInfo.at(1);
        }
        else if (_listStoryInfo.count(2) > 0)
        {
            _listInfoStoryAutoPlay = _listStoryInfo.at(2);
        }
        else if (_listStoryInfo.count(3) > 0)
        {
            _listInfoStoryAutoPlay = _listStoryInfo.at(3);
        }
        beginAutoPlay();
    }

    void CacheManager::setListLevelInfo(std::string key, mj::model::LevelInfo infoLevel)
    {
        _listLevelInfo.insert({ key, infoLevel });
    }

    std::map<std::string, mj::model::LevelInfo> CacheManager::getLevelInfo()
    {
        return _listLevelInfo;
    }

    void CacheManager::setListStoyInfo(int key, std::vector<mj::model::StoryInfo> listInfo)
    {
        if (!listInfo.empty())
        {
            _listStoryInfo.insert({ key, listInfo });
        }
    }

    std::map<int, std::vector<mj::model::StoryInfo>> CacheManager::getListStoryInfo()
    {
        return _listStoryInfo;
    }

    void CacheManager::setListStorySearch(std::string key, std::vector<mj::model::StoryInfo> listInfo)
    {
        if (!listInfo.empty())
        {
            _listStoryInfoSearch.insert({ key, listInfo });
        }
    }

    std::map<std::string, std::vector<mj::model::StoryInfo>> CacheManager::getListStoryInfoSearch()
    {
        return _listStoryInfoSearch;
    }

    void CacheManager::clearListStoryInfo()
    {
        _listStoryInfoSearch.clear();
    }
    
    void CacheManager::setListAudioBookSearch(std::string key, std::vector<mj::model::AudiobookInfo> listInfo)
    {
        if (!listInfo.empty())
        {
            _listAudiobookInfoSearch.insert({ key, listInfo });
        }
    }

    std::map<std::string, std::vector<mj::model::AudiobookInfo>> CacheManager::getListAudiobookInfoSearch()
    {
        return _listAudiobookInfoSearch;
    }

    void CacheManager::clearListAudiobookInfo()
    {
        _listAudiobookInfoSearch.clear();

    }

    void CacheManager::setCurrentLessonAndListActivityOfLesson(std::pair<int, std::vector<mj::model::ActInfo>> idLessonAndListActivity)
    {
        _idLessonAndListActivities.push_back(idLessonAndListActivity);
    }

    std::pair<int, std::vector<mj::model::ActInfo>> CacheManager::getCurrentLessonAndListActivityOfLesson(int idLesson)
    {
        auto it = std::find_if(_idLessonAndListActivities.begin(), _idLessonAndListActivities.end(), [=](std::pair<int, std::vector<mj::model::ActInfo>> a) {
            return idLesson == a.first;
            });

        if (it != _idLessonAndListActivities.end())
        {
            return *it;
        }

        return { -1, {} };
    }

    void CacheManager::setCurrentLessonAndListActivityOfStory(std::pair<int, std::vector<mj::model::ActInfo>> idLessonAndListActivity)
    {
        _idStoryAndListActivities.push_back(idLessonAndListActivity);
    }

    std::pair<int, std::vector<mj::model::ActInfo>> CacheManager::getCurrentLessonAndListActivityOfStory(int idLesson)
    {
        auto it = std::find_if(_idStoryAndListActivities.begin(), _idStoryAndListActivities.end(), [=](std::pair<int, std::vector<mj::model::ActInfo>> a) {
            return idLesson == a.first;
            });

        if (it != _idStoryAndListActivities.end())
        {
            return *it;
        }

        return { -1, {} };
    }

    void CacheManager::SetCacheInfoToturing(std::vector<mj::model::StoryInfoTutoring> listInfo)
    {
        _listInfoTutoring = listInfo;
    }

    std::vector<mj::model::StoryInfoTutoring> CacheManager::GetCacheInfoToturing()
    {
        return _listInfoTutoring;
    }

    void CacheManager::setListInfoAutoPlayAudiobook()
    {
        if (_listStoryInfo.count(0) > 0)
        {
            _listInfoAudiobookAutoPlay = _listAudiobookInfo.at(0);
        }
        else if (_listStoryInfo.count(1) > 0)
        {
            _listInfoAudiobookAutoPlay = _listAudiobookInfo.at(1);
        }
        else if (_listStoryInfo.count(2) > 0)
        {
            _listInfoAudiobookAutoPlay = _listAudiobookInfo.at(2);
        }
        else if (_listStoryInfo.count(3) > 0)
        {
            _listInfoAudiobookAutoPlay = _listAudiobookInfo.at(3);
        }
        beginAutoPlayAudiobook();
    }

    void CacheManager::beginAutoPlayAudiobook(bool isNotSkipAndBack)
    {
        if (_listInfoAudiobookAutoPlay.empty() || _indexAudiobook >= _listInfoAudiobookAutoPlay.size() || _indexAudiobook < 0)
        {
            _indexAudiobook = 0;
            MJDEFAULT->setIntegerForKey("Key_Audiobook_id_auto_play", -1);
            return;
        }

        int idAudiobook = _listInfoAudiobookAutoPlay[_indexAudiobook].book_id;

        if (!isNotSkipAndBack)
        {
            auto id = MJDEFAULT->getIntegerForKey("Key_Audiobook_id_auto_play", -1);
            if (_idStoryIsAutoPlay == -1)
            {
                if (id == -1)
                {

                }
                else if (id != _listInfoAudiobookAutoPlay[_indexAudiobook].book_id) {
                    _indexAudiobook++;
                    beginAutoPlayAudiobook();
                    return;
                }
            }
        }

        _idStoryIsAutoPlay = 0;
        if (!isNotSkipAndBack)
        {
            MJDEFAULT->setIntegerForKey("Key_Audiobook_id_auto_play", idAudiobook);
        }
        setAutoAudiobook(idAudiobook, _listInfoAudiobookAutoPlay[_indexAudiobook]);
        _indexAudiobook++;
    }

    void CacheManager::setListAudiobookInfo(int key, std::vector<mj::model::AudiobookInfo> listInfo)
    {
        if (!listInfo.empty())
        {
            _listAudiobookInfo.insert({ key, listInfo });
        }
    }

    std::map<int, std::vector<mj::model::AudiobookInfo>> CacheManager::getListAudiobookInfo()
    {
        return _listAudiobookInfo;
    }

}
