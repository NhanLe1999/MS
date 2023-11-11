
#ifndef MSManager_hpp
#define MSManager_hpp

#define KEY_MEMMORY true

#include "cocos2d.h"
#include "StoryDataModel.h"
#include "MJHelper.h"

namespace ms {
    class CacheManager {
    public:
        static CacheManager& getInstance() {
            static CacheManager sharedManager;
            return sharedManager;
        };

        std::map<std::string, mj::model::LevelInfo> getLevelInfo();
        void setListLevelInfo(std::string key, mj::model::LevelInfo infoLevel);

        void setListStoyInfo(int key, std::vector<mj::model::StoryInfo> listInfo);
        std::map<int, std::vector<mj::model::StoryInfo>> getListStoryInfo();

        void setListAudiobookInfo(int key, std::vector<mj::model::AudiobookInfo> listInfo);
        std::map<int, std::vector<mj::model::AudiobookInfo>> getListAudiobookInfo();

        void setListStorySearch(std::string key, std::vector<mj::model::StoryInfo> listInfo);
        std::map<std::string, std::vector<mj::model::StoryInfo>> getListStoryInfoSearch();
        void clearListStoryInfo();
        void setListAudioBookSearch(std::string key, std::vector<mj::model::AudiobookInfo> listInfo);
        std::map<std::string, std::vector<mj::model::AudiobookInfo>> getListAudiobookInfoSearch();
        void clearListAudiobookInfo();

        void setCurrentLessonAndListActivityOfLesson(std::pair<int, std::vector<mj::model::ActInfo>> idLessonAndListActivity);
        std::pair<int, std::vector<mj::model::ActInfo>> getCurrentLessonAndListActivityOfLesson(int idLesson);

        void setCurrentLessonAndListActivityOfStory(std::pair<int, std::vector<mj::model::ActInfo>> idLessonAndListActivity);
        std::pair<int, std::vector<mj::model::ActInfo>> getCurrentLessonAndListActivityOfStory(int idLesson);

        void SetCacheInfoToturing(std::vector<mj::model::StoryInfoTutoring> listInfo);
        std::vector<mj::model::StoryInfoTutoring> GetCacheInfoToturing();

        inline std::string getUrlFromDomainNew(std::string url, mj::UrlType urlType, int index, const std::vector<std::string>& listUrl, const std::string& typeError)
        {
            return "" ;
        }

        void setListUrl(std::vector<std::string> listUrl)
        {
            _listUrl = listUrl;
        }

        void setTypeError(std::string type)
        {
            _typeUrl = type;
        }

        void setAutoStory(int idStory);
        
        void setAutoAudiobook(int idStory, mj::model::AudiobookInfo info);

        void setListInfoAutoPlay();

        void beginAutoPlay(bool isNotSkipAndBack = false);

        void onBackStory();

        void onSkipStory();
        
        void onBackAudiobook();

        void onSkipAudibook();

        void setListInfoAutoPlayAudiobook();

        void beginAutoPlayAudiobook(bool isNotSkipAndBack = false);

    protected:
        explicit CacheManager();
    private:
        std::map<std::string, mj::model::LevelInfo> _listLevelInfo;

        std::map<int, std::vector<mj::model::StoryInfo>> _listStoryInfo;
        std::map<int, std::vector<mj::model::AudiobookInfo>> _listAudiobookInfo;
        std::map<std::string, std::vector<mj::model::StoryInfo>> _listStoryInfoSearch;
        std::map<std::string, std::vector<mj::model::AudiobookInfo>> _listAudiobookInfoSearch;
        std::vector <std::pair<int, std::vector<mj::model::ActInfo>>> _idLessonAndListActivities = {};
        std::vector <std::pair<int, std::vector<mj::model::ActInfo>>> _idStoryAndListActivities = {};

        std::vector<std::string> _listUrl = {};
        std::string _typeUrl = "";
        std::vector<mj::model::StoryInfo> _listInfoStoryAutoPlay = {};
        std::vector<mj::model::AudiobookInfo> _listInfoAudiobookAutoPlay = {};
        std::vector<mj::model::StoryInfoTutoring> _listInfoTutoring = {};

        int _idStoryIsAutoPlay = -1;

        int _index = 0;

        int _indexAudiobook = 0;

    };
}

#endif /* MSLessonManager_hpp */
