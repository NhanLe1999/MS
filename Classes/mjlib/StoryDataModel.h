//
//  StoryDataModel.h
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#ifndef __StoryDataModel_H__
#define __StoryDataModel_H__
//USING_NS_CC;
#include "cocos2d.h"
#include "sqlite3.h"
#include "JsonDump.h"
#include "base_include.h"
#include "MJStoryInfoModel.h"
#include "LicenseInfoModel.h"
#include "MJLicensePackage.h"
#include"MJAPI_DecodeObjectJsonInfo.h"
#define max_id_stories 1000000
#define max_id_audiobook 2000000

using namespace std;

class StoryDataModel {
public:
    enum QuerySelectType
    {
        ONE,
        FIRST,
        ALL,
    };

    enum TYPEURL
    {
        DEFAULT,
        STORY,
        AUDIOBOOK_ACTIVITY_GAME
    };
    static StoryDataModel* instance;
    static StoryDataModel* getInstance(string dataName = "database");
    
    void prepareDatabase();
    void dropDatabase(Node*node);
    void logError();
    
#pragma mark - Material
    void insertMaterialsWithVector(const std::vector<mj::model::MaterialInfo>& materials);
    void insertMultiMaterials(const std::vector<mj::model::MaterialInfo>& materials);
    void deleteMaterials(const std::vector<int>& ids);
    
    void sortMaterials(std::vector<mj::model::MaterialInfo>& vector);
    std::vector<mj::model::MaterialInfo> getAllMaterialsByType(mj::model::MaterialInfo::Type type, bool sort_by_readed = true);
    std::vector<mj::model::MaterialInfo> getAllMaterialsByTypeAndLevel(mj::model::MaterialInfo::Type type, int level, bool sort_by_readed = true);
    
    std::vector<mj::model::MaterialPopularInfo> getPopularMaterials(mj::model::MaterialPopularInfo::Type type);
    void setPopularMaterials(const std::vector<mj::model::MaterialPopularInfo>& vector);
    void insertPopularMaterialsWithVector(const std::vector<mj::model::MaterialPopularInfo>& populars);
    void insertMultiPopularMaterials(const std::vector<mj::model::MaterialPopularInfo>& populars);
    // --------
    
//    void insertStory(string plist);
    
    void insertGameInfo(std::vector<mj::model::GameInfo> info);

    std::unordered_map<int,mj::model::GameInfo> getGameByIds(std::vector<int> gameIDs);

    mj::model::GameInfo getGameById(int game_id, std::pair<int, std::string> valueError = {-1, ""}, int index = -1, std::vector<std::string> domainAudioBook = {}, std::string typeError = "");

    void insertNewStories(std::vector<int> ids);
    void insertPopularStories(std::vector<int> ids);
    void insertFeatureStories(std::vector<std::pair<int, std::string>> feature_info);
    
    std::vector<mj::model::StoryInfo> getFreeStoriesByLanguage(int lang_id);
    std::vector<mj::model::AudiobookInfo> getFreeAudiobooksByLanguage(int lang_id);
    
    std::vector<mj::model::AudiobookInfo> getAllAudiobooksByLanguage(int lang_id);
    mj::model::AudiobookInfo getAudiobook(int book_id);
    void updateAudiobookUrlPath(mj::model::AudiobookInfo info);
    void insertAudiobook(mj::model::AudiobookInfo info);
    void insertAudiobooks(std::vector<mj::model::AudiobookInfo> books);

	void insertPopularSearchAudiobooks(std::vector<mj::model::PopularSearchAudiobookInfo> series);
    void setPopularSearchAudiobooks(const std::vector<mj::model::PopularSearchAudiobookInfo>& books);
	std::vector<mj::model::PopularSearchAudiobookInfo> getPopularSearchAudiobooks();

	void insertPopularSearchStories(std::vector<mj::model::PopularSearchStoryInfo> series);
    void setPopularSearchStories(const std::vector<mj::model::PopularSearchStoryInfo>& stories);
	std::vector<mj::model::PopularSearchStoryInfo> getPopularSearchStories();

    void insertAudiobookSeries(std::vector<mj::model::AudiobookSeriesInfo> series);
    std::vector<mj::model::AudiobookSeriesInfo> getAudiobookSeries();
	std::vector<mj::model::AudiobookInfo> getAllAudiobooksByBookIDs(std::vector<int> bookIDs);

    std::vector<mj::model::AudiobookInfo> getAllAudiobooksBySeries(std::string name);
    void removeAudiobooks(std::vector<int> book_idx);
    void updateAudiobookChapter(std::vector<std::tuple<int, int, int>> chapter_list);
    bool isAudiobookChapter(int book_id);
    bool isChapterOfAudiobook(int book_id);
    mj::model::AudiobookInfo getOriginalBookOfChapter(int chapter_id);
    std::vector<mj::model::AudiobookInfo> getChapterOfAudiobook(int book_id);
    std::vector<mj::model::AudiobookInfo> getAudiobookChapters();
    
    std::vector<std::string> getGameZipUrl();
    
    void insertLessons(std::vector<mj::model::LessonInfo> lessons, mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall tag = mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Default);
    std::vector<mj::model::LessonInfo> getAllLessonsByLanguage(int lang_id);

    mj::model::LessonInfo getLessonsById(int lessonId);

    void removeLessonsById(std::vector<int> idx, mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall tag = mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Default);
    void setLessonLearned(std::vector<int> lesson_ids);
    void setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn);
    ValueVector getLessonLearned();
    void resetLessonLearned();
    std::vector<int> getDownloadedLessons();
    std::vector<int> getLessonLearnByID(int lessonId);
//    std::vector<mj::model::LessonInfo> getAllLessonsByGrade(int grade_id);
    
    std::vector<mj::model::StoryInfo> searchStoryByLanguage(int lang_id, std::string text, int category_id = -1);
    std::vector<mj::model::AudiobookInfo> searchAudiobookByLanguage(int lang_id, std::string text, int category_id = -1);
    std::vector<mj::model::AudiobookInfo> searchAudiobookFromVector(std::vector<mj::model::AudiobookInfo> audio, std::string text, int category_id = -1, bool is_IOS = false, bool has_qMark = false, std::string keyChache = "");
    
    void insertNewGrades(std::vector<mj::model::GradeInfo> grades);
    std::vector<mj::model::GradeInfo> getAllNewGrades(int lang_id);
    void removeNewGradesByLangId(std::vector<int> langIds);
    void insertProductList(std::vector<mj::model::ProductListInfo> productListInfo);
    std::vector<mj::model::ProductListInfo> getAllProductList(std::string city);

    /*-----------------------------DB handler for AISpeaking begin-------------------------*/


    void insertAiSpeakingTopic(std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingTopicInfos);
    bool removeAiSpeakingTopicById(std::vector<int> topicIDs);
    bool deleteAiSpeakingTopicTable();
    mj::model::AISpeakingCategoryInfo getAiSpeakingTopicById(int topicID);
    std::vector<mj::model::AISpeakingCategoryInfo> getAllAiSpeakingTopic();

    std::vector<mj::model::AISpeakingLessonInfo> getAllLessonsAISpeakingByLanguage(int lang_id);
    void setLessonAISpeakingLearned(std::vector<int> lesson_ids);
    void setLessonAISpeakingLearned(std::vector<std::pair<std::string, int>> listLessonLearn);
    ValueVector getLessonAISpeakingLearned();
    void resetLessonAISpeakingLearned();
    std::vector<int> getDownloadedLessonsAISpeaking();
    std::vector<int> getLessonLearnAISpeakingByID(int lessonId);
    std::vector<int> getLessonStateAISpeakingByID(int lessonId);
    mj::model::AISpeakingLessonInfo getLessonAISpeakingByID(int lessonId);
    void UpdateLessonStateAISpeaking(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo);

    std::vector<mj::model::AISpeakingLessonInfo> searchAISpeakingLessonByLanguage(int lang_id, std::string text);

    std::vector<mj::model::AISpeakingLessonInfo> getAiSpeakingLessonByTopicId(int topicID);
    std::vector<mj::model::AISpeakingLessonInfo> getAllAiSpeakingLessonItems();
    void insertAiSpeakingLessons(std::vector<mj::model::AISpeakingLessonInfo> aISpeakingItemInfos);
    std::vector < mj::model::AISpeakingLessonInfo> getAiSpeakingLessonByIds(std::vector<int> iteamIds);
    bool removeAiSpeakingLessonByIds(std::vector<int> iteamIds);
    bool removeAiSpeakingLessonByTopicId(int topicID);
    bool deleteAiSpeakingLessonTable();

    void insertSpeakingAIActivities(std::vector<mj::model::ActAISpeakingInfo> activities);
    std::vector<mj::model::ActAISpeakingInfo> getActivitiesOfSpeakingAILesson(int lesson_id, QuerySelectType type = QuerySelectType::ALL);
    mj::model::ActAISpeakingInfo getSpeakingAIActivityById(int activity_id);
    void removeSpeakingAIActivitiesOfLesson(int lesson_id);
    void removeAllSpeakingAIActivities();

    void insertAiSpeakingCommonInfo(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos);
    std::vector<mj::model::TalkingCommonInfo>  getAllAiSpeakingCommonInfos();
    std::vector<mj::model::TalkingCommonInfo>  getAiSpeakingCommonInfosByActor(std::vector<std::string> actors);
    std::vector<mj::model::TalkingCommonInfo>  getAiSpeakingCommonInfosByActor(std::string actor);
    bool removeAiSpeakingCommonInfoById(std::vector<int> commonInfoId);
    bool removeAiSpeakingCommonInfoByActor(std::vector<std::string> actors);
    void updateAiSpeakingCommonInfo(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos);
    bool deleteAiSpeakingCommonInfoTable();


    /*-----------------------------DB handler for AISpeaking end-------------------------*/


    /*--------------------Handler for f2P trial data begin-------------------------------*/

    void insertFreeTrialInfo(std::vector<mj::model::FreeItemInfo> freeItemInfo);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosByOptions(std::vector<ms::f2p::FreeContentOption> freeContentOptions);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosByOption(ms::f2p::FreeContentOption freeContentOption);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosByTypes(std::vector<ms::MsTypes::DocumentsType> documentsTypes);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosByType(ms::MsTypes::DocumentsType documentsType);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosByIds(std::vector<int> ids);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosById(int ids);
    std::vector<mj::model::FreeItemInfo> getFreeTrialInfosFromQuery();

    bool deleteFreeTrialInfosTable();
    /*------------------------Handler for f2P trial data end-----------------------------*/

    void insertActivities(std::vector<mj::model::ActInfo> activities);
    std::vector<mj::model::ActInfo> getActivitiesOfLessonNew(int lesson_id, QuerySelectType type = QuerySelectType::ALL);
    std::vector<mj::model::ActInfo> getActivitiesOfLesson(int lesson_id, QuerySelectType type = QuerySelectType::ALL, std::pair<int, std::string> valueError = { -1, "" }, int index = -1, std::vector<std::string> domainStory = {}, std::vector<std::string> domainAudioBook = {}, std::string typeError = "");
    mj::model::ActInfo getActivityById(int activity_id);
    void removeActivitiesOfLesson(int lesson_id);
    void removeAllActivities();

    std::vector<mj::model::StoryInfo> getAllStoriesByLanguage(int lang_id, int limitQuery = -1);
    std::vector<mj::model::StoryInfo> getNewStoriesByLanguage(int lang_id);
    std::vector<mj::model::StoryInfo> getPopularStoriesByLanguage(int lang_id);
    std::vector<mj::model::StoryInfo> getRecentStoriesByLanguage(int lang_id);
    std::vector<mj::model::StoryInfo> getFavouriteStoriesByLanguage(int lang_id);
    std::vector<std::pair<int, std::string>> getFeaturedStoriesByLanguage(int lang_id);
    
    std::vector<mj::model::AudiobookInfo> getFavouriteAudiobooksByLanguage(int lang_id);
    void addFavouriteAudiobooksByLanguage(int audiobook_id, int lang_id);
    void removeFavouriteAudiobookByLanguage(int audiobook_id, int lang_id);
    void removeFavouriteStoryByLanguage(int _id, int lang_id);

    void insertStoryLevelRelation(const std::vector<std::pair<int, int>> & list_data);
    void insertStoryTopicRelation(const std::vector<std::pair<int, int>> & list_data);
    void insertStoryCharaRelation(const std::vector<std::pair<int, int>> & list_data);

    void insertStoryRelation(std::string relation_name, const std::vector<std::pair<int, int>> & list_data);
    void insertStoryRelationChunk(std::string relation_name, const std::vector<std::pair<int, int>> & list_data);

    std::vector<int> getListLevelsByLang(int lang_id);

    void deleteStories(std::vector<int> ids);

    void insertMultiStory(std::vector<mj::model::StoryInfo> stories);
    void insertMultiStoryTutoring(std::vector<mj::model::StoryInfo> stories);
    void insertStoryWithData(std::vector<mj::model::StoryInfo> stories);

    void insertMultiStory(ValueVector vector);
    void insertMultiStoryTutoring(ValueVector vector);
    void deleteStoryReject(int _id);
    void downloadSuccess(int _id, int story_id, int language_id);
    void setDownloadMultiStory(ValueVector vector);
    void deleteDownloadStory(int _id);
    void deleteAllDownloadStory();
    
    bool getStoryIsDownloadedWith(int _id, int story_id, int lang_id);
    void insertStoryWithData(cocos2d::ValueVector vector);
    std::vector<mj::model::StoryInfoTutoring> GetAllInfoTableToturing();
    bool CheckStoryIsTutoring(int storyId);
    std::vector<int> getAllStoryDownloaded();
    std::vector<int> getAllStoryIDDownloaded();
    std::vector<std::pair<int, int>> getDownloadStoryIDWithVersion();
    
    std::vector<mj::model::AudiobookInfo> getAllAudiobooksDownloaded();


    void updateCharacters(std::vector<mj::model::CharacterInfo> characters);
    void updateCategories(std::vector<mj::model::CategoryInfo> categories);
    
    std::vector<mj::model::CategoryInfo> getCategories(int lang_id);
    
    void updateCharacterWithData(cocos2d::ValueVector vector);
    void updateCategoryWithData(cocos2d::ValueVector vector);
    void insertCharactor(ValueVector vector);
    void insertCategory(ValueVector vector);
    
    int searchID(int id, int start, int end, std::vector<std::pair<int ,int>> storyList);

    ValueVector getAllStory();
    ValueVector getAllStories(int level_id, int lang_id, string order = "");
    ValueMap getStoryWithId(int _id);

    mj::model::StoryInfo getStoryById(int id);
    mj::model::StoryInfo getStoryByStoryIdAndLang(int id, int lang_id);

    ValueVector getCharacter(int langid);
	ValueVector getCategory(int langid);
	cocos2d::ValueMap getCategoryByID(int catagoryid);
    ValueVector getFeature(int langid);
    ValueVector getCategoryStories(int category_id);
    ValueVector getCharacterStories(int charater_id);
    ValueVector getListIDStory(int level_id, int lang_id, string order = "");

    std::vector<mj::model::StoryInfo> getStoriesByCategory(int category_id, std::string text = "", int grade_id = -1);
    std::vector<mj::model::StoryInfo> getStoriesByCharacter(int character_id);
    
    void updateFeatureWithData(cocos2d::ValueVector vector);
    void insertFeature(ValueVector vector);
    
    void deleteAllFrom(string table, ValueMap data);
    
    void updateCharacterStories(int character_id, ValueVector vector);
    void insertCharacterStories(int character_id, ValueVector vector);
    
    void updateCategoryStories(int category_id, std::vector<mj::model::StoryInfo> stories);
    void insertCategoryStories(int category_id, std::vector<mj::model::StoryInfo> stories);
    void updateCategoryStories(int category_id, ValueVector vector);
    void insertCategoryStories(int category_id, ValueVector vector);
    void updateStoriesListByVersion(std::vector<std::pair<int, int>> list_id_version);
    
    ValueVector getStoriesWithListID(ValueVector list_id);
    ValueVector getListStoriesIdWithStoryId(int story_id);
    ValueVector getStoriesRecentWithListID(ValueVector list_id);
    
    ValueVector getSearchStories(int lang_id, string text_search);
    
    //message
    void addMessageFromServer(std::vector<mj::model::NotificationInfo> notifications);
    mj::model::NotificationInfo getNotificationById(int idx);
    //void addMessageFromServer(ValueVector list_message);
    ValueVector getAllMessage();
    
    //levelinfo
    void insertGradeInfo(std::vector<mj::model::GradeInfo> grades);
    void removeGradeInfo(int lang_id);

    void insertLevelInfo(std::vector<mj::model::LevelInfo> levels);
    std::vector<mj::model::GradeInfo> getAllGradesInfo(int lang_id);
    mj::model::GradeInfo getGradeInfoById(int grade_id, int lang_id);

    std::vector<mj::model::LevelInfo> getAllLevelsInfo(int lang_id);
    mj::model::LevelInfo getLevelInfoById(int level_id, int lang_id);

    void insertLevelInfo(ValueVector listLevelInfo, int lang_id);
    ValueVector getAllLevelInfo(int lang_id);
    
    void addStorySentDownloadSuccess(int _id, int story_id, int type);
    bool checkStoryIsSentDownloadSuccess(int _id);
    
    void addLanguageDisplay(ValueVector list_lang_display);
    ValueVector getListLanguageDisplay();
    
    void setStoryRead(int story_lang_id, int read_value = 1, int time_read = 0);
    void setStoryRead(std::vector<std::pair<std::string, int>> listStoryList);
    void resetStoryRead();
    ValueVector getAllStoryRead();
    void removeAllStoryRead();
    std::vector <int> getValueStoryReadById(int idStoryLang);
    
    void setAudioRead(int audio_id, int read_value = 1, int time_read = 0);
    void setAudioRead(std::vector<std::pair<std::string, int>> listAuidoBookList);
    ValueVector getAllAudioRead();
    std::vector <int> getValueAudioReadById(int audioId);
    void removeAllAudioRead();
    void removeAllAudiobookSeries();
    std::vector<mj::model::AudiobookInfo> getAudioBookRecentRead();
    void addAudioBookRecentRead(int _id);
    
    std::vector<int> getListCategoryWithStoryId(int _id);
    
    bool storyIsRated(int story_lang_id);
    void setStoryIsRated(int story_lang_id, int rate_value);
	void closeConnect();
    void openConnect();

    void removeAllStories();
    
    std::string getLinkDownloadStory(int story_id, int index = -1, std::vector<std::string> domainStory = {}, std::string typeError = "");
    void setLinkDownloadStory(std::vector<std::tuple<int, std::string, std::string>> data);
    
    //licensekey_manager
    void updateLicenseDeviceInfo(std::vector<license::LicenseDeviceInfo> list_licensedeviceinfo);
    std::vector<license::LicenseDeviceInfo> getLicenseDeviceInfo();

    //file_size
    void updateFileSize(std::vector<std::pair<std::string, int>> list_size);
    float getFileSize(string url);
    
    void initAudiobooksWithChapters();
    

    //story's activities
    void insertStoriesActivities(std::vector<mj::model::ActInfo> activities);
    mj::model::ActInfo getActivityOfStory(int activity_id);
    std::vector<mj::model::ActInfo> getActivitiesOfStory(int story_id, std::pair<int, std::string> valueError = {-1, ""}, int index = -1, std::vector<std::string> domainStory = {}, std::vector<std::string> domainAudioBook = {}, std::string typeError = "");

    void addLicensePackages(std::vector<license::Package> packages, bool append = false);
    std::vector<license::Package> getLicensePackages();
    license::Package getLicenseByLicenseKey(const std::string & key);
    
    void removeDeviceIDFromListLicenseDevice(int device_id_remove);
    
    std::vector<license::LicenseDeviceInfo> getLicenseDeviceInfoOfLicense(const std::string & license);
    void updateRelationshipLicenseDevice(std::vector<std::tuple<std::string, int, int>>, bool append = false);
    std::vector<mj::model::LessonInfo> searchLessonByLanguage(int lang_id, std::string text);

    void insertTableGameListError(std::vector<int> listGameError);
    void deleteTableGameListError();
    std::vector<int> getListGameError();

    void insertTableListDomainStories(std::vector<std::string> listUrl, StoryDataModel::TYPEURL type);
    void deleteTableListDomainStories(StoryDataModel::TYPEURL type);
    std::vector<std::string> getListDomainStories(StoryDataModel::TYPEURL type);
    std::string getUrlOfDomain(std::string url, mj::UrlType type, int index = -1, std::vector<std::string> listDomain = {}, std::string typeError = "", std::pair<int, std::string> valueError = { -1, "" });
    std::vector<mj::model::StoryInfo> getInforActivity();

private:
    sqlite3 *db;
    sqlite3_stmt *stmp;
    std::string db_path = "";
    void showError(int result);
    
    std::vector<int> _audiobooks_with_chapters;
    std::vector<int> _chapter_of_audiobooks;
    
    ValueMap getRow(string query, ValueVector query_vector);
    void executeQuery(string query, ValueVector query_vector);
    ValueMap fetchRow();
    ValueVector fetchAllRecord(string query);
    
    std::vector<mj::model::StoryInfo> _all_stories;
    
};


#endif /* __StoryDataModel_H__ */
