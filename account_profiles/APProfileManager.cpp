#include "APProfileManager.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "JsonDump.h"
#include "GlobalConstants.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSLessonManager.h"
#include "HSLib.h"
#include "MSLessonManager.h"

#include "APDatabase.h"

#define FOLDER_PROFILE cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/"

USING_NS_CC;

namespace ap
{
	ProfileManager & ap::ProfileManager::getInstance()
	{
		static ProfileManager shared_profile;
		return shared_profile;
	}

	void ProfileManager::initProfile(int profileID)
	{
		if (!FileUtils::getInstance()->isDirectoryExist(FOLDER_PROFILE))
		{
			FileUtils::getInstance()->createDirectory(FOLDER_PROFILE);
		}
        
		_db = new SQLite::Database(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID),
			SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
		try {
			_db->exec("CREATE TABLE IF NOT EXISTS tbl_profile ("
				"key TEXT, "
				"value TEXT, "
				"PRIMARY KEY (key))");
            
            _db->exec("CREATE TABLE IF NOT EXISTS tbl_story_read ("
                           "id_story_lang INTEGER, "
                           "read INTEGER, "
                           "time_read INTEGER, "
                           "PRIMARY KEY (id_story_lang))");
            
            _db->exec("CREATE TABLE IF NOT EXISTS tbl_audio_read ("
            "audio_id INTEGER, "
            "read INTEGER, "
            "time_read INTEGER, "
            "PRIMARY KEY (audio_id))");
            
            _db->exec("CREATE TABLE IF NOT EXISTS tbl_love_stories ("
                                   "story_id INTEGER, "
                                   "PRIMARY KEY (story_id))");
                    
            _db->exec("CREATE TABLE IF NOT EXISTS tbl_love_audiobooks ("
            "audio_id INTEGER, "
            "PRIMARY KEY (audio_id))");
            
            _db->exec("CREATE TABLE IF NOT EXISTS tbl_lesson_learned ("
            "id INTEGER, "
            "category_id INTEGER, "
            "learned INTEGER, "
            "PRIMARY KEY (id))");
		}
		catch (std::exception & e) {
			std::cout << "db exception: " << e.what() << std::endl;
		}
	}

    void ProfileManager::saveReadStories(cocos2d::ValueVector stories)
    {
        //load data from tbl_story_read of database.sqlite to tbl_story_read of profile
        for(auto s: stories )
        {
            setRow("tbl_story_read", "id_story_lang", "read", "time_read", s.asValueMap()["id_story_lang"].asInt(), s.asValueMap()["read"].asInt(), s.asValueMap()["time_read"].asInt());
        }
    }

    void ProfileManager::saveReadAudioBooks(cocos2d::ValueVector audios)
    {
        for(auto s: audios )
        {
            setRow("tbl_audio_read", "audio_id", "read", "time_read", s.asValueMap()["audio_id"].asInt(), s.asValueMap()["read"].asInt(), s.asValueMap()["time_read"].asInt());
        }
    }

    void ProfileManager::saveLessonLearned(cocos2d::ValueVector lessons)
    {
        for (auto l : lessons)
        {
            setRow("tbl_lesson_learned", "id", "category_id", "learned", l.asValueMap()["_id"].asInt(), l.asValueMap()["category_id"].asInt(), l.asValueMap()["learned"].asInt());
        }
    }

    void ProfileManager::loadReadStories(){
        //load data from tbl_story_read of profile to tbl_story_read of database.sqlite
         try {
               SQLite::Statement stmt(*_db, "SELECT id_story_lang FROM tbl_story_read ");
               
               while (stmt.executeStep()) {
                   StoryDataModel::getInstance()->setStoryRead(stmt.getColumn(0).getInt());
               }
           }
           catch (std::exception & e) {
               std::cout << "db exception: " << e.what() << std::endl;
             
           }
    }

    void ProfileManager::loadReadAudioBooks(){
        //load data from tbl_story_read of profile to tbl_story_read of database.sqlite
         try {
               SQLite::Statement stmt(*_db, "SELECT audio_id FROM tbl_audio_read ");

               while (stmt.executeStep()) {
                   StoryDataModel::getInstance()->setAudioRead(stmt.getColumn(0).getInt());
               }
           }
           catch (std::exception & e) {
               std::cout << "db exception: " << e.what() << std::endl;

           }
    }

    void ProfileManager::loadLessonLearned(){
        try {
            std::vector<int> lesson_id;
            SQLite::Statement stmt(*_db, "SELECT id FROM tbl_lesson_learned ");

            while (stmt.executeStep()) {
                lesson_id.push_back(stmt.getColumn(0).getInt());
            }
//            StoryDataModel::getInstance()->setLessonLearned(lesson_id);
            
			for(auto id : lesson_id){
                ms::LessonManager::getInstance().setLessonLearned(id);
				auto last_lesson = ms::LessonManager::getInstance().getLessonById(id);
                
                if (CONFIG_MANAGER->isVietnameseStories() || ms::LessonManager::getInstance().isPhonicCategory(last_lesson.category_id)) {
                    for (auto prev_lesson : StoryDataModel::getInstance()->getAllLessonsByLanguage(LANGUAGE_MANAGER->getCurrentLangId())) {
                        if(prev_lesson.category_id == last_lesson.category_id && prev_lesson.lesson_order < last_lesson.lesson_order){
//                            StoryDataModel::getInstance()->setLessonLearned({prev_lesson.lesson_id});
                            ms::LessonManager::getInstance().setLessonLearned(prev_lesson.lesson_id);
                        }
                    }
                }
			}
        }
        catch (std::exception & e) {
            std::cout << "db exception: " << e.what() << std::endl;

        }
    }

	void ProfileManager::switchProfile(int profile_id, bool user_choose)
	{
		try
		{
            if(user_choose){
                saveProcess();
                clearDataModel();
                setCurrentProfileId(profile_id);
                initProfile(profile_id);
                loadProcess();
            }else{
                setCurrentProfileId(profile_id);
                initProfile(profile_id);
            }
            
		}
		catch (std::exception & e)
		{
			CCLOG("db exception %s", e.what());
		}
	}

    void ProfileManager::saveProcess(){
        saveReadStories(StoryDataModel::getInstance()->getAllStoryRead());
        saveReadAudioBooks(StoryDataModel::getInstance()->getAllAudioRead());
        saveLessonLearned(StoryDataModel::getInstance()->getLessonLearned());
    }

    void ProfileManager::clearDataModel(){
        StoryDataModel::getInstance()->removeAllStoryRead();
        StoryDataModel::getInstance()->removeAllAudioRead();
        StoryDataModel::getInstance()->resetLessonLearned();
        //StoryDataModel::getInstance()->resetLessonLearned();
    }

    void ProfileManager::loadProcess(){
        loadReadStories();
        loadReadAudioBooks();
        loadLessonLearned();
        ms::LessonManager::getInstance().updateAllLesson();
    }
    
    std::map<std::string, std::string> ProfileManager::getAllDatabase(){
        std::map<std::string, std::string> map ;
        try {
            SQLite::Statement stmt(*_db, "SELECT * FROM tbl_profile ");
            
            while (stmt.executeStep()) {
                std::cout << "row (" << stmt.getColumn(0) << ", \"" << stmt.getColumn(1) << "\")\n";
                
                map[stmt.getColumn(0).getText()] = XHSLib->giaimaString( stmt.getColumn(1).getText());
            }
        }
        catch (std::exception & e) {
            std::cout << "db exception: " << e.what() << std::endl;
          
        }
        return map;
    }

    std::map<std::string, std::string> ProfileManager::getAllTable(std::string tbl_name){
        std::map<std::string, std::string> map ;
        try {
            auto query = StringUtils::format("SELECT * FROM %s ", tbl_name.c_str());
            SQLite::Statement stmt(*_db, query);
            
            while (stmt.executeStep()) {
                std::cout << "row (" << stmt.getColumn(0) << ", \"" << stmt.getColumn(1) << "\")\n";
                
                map[stmt.getColumn(0).getText()] = stmt.getColumn(1).getText();
            }
        }
        catch (std::exception & e) {
            std::cout << "db exception: " << e.what() << std::endl;
          
        }
        return map;
    }

	void ProfileManager::removeTrackingPhonics()
	{
		
		auto phonic_category = { 479,480,481 };

		std::string query = "DELETE FROM tbl_lesson_learned WHERE id IN( \
		530,529,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,778,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,574,575,576,577,578,579,580,581,582,583,585,586,587,588,589,590,591,593,594,595,596,597,598,599,600,601,602,603,604,605,606,779,608,781,610,611,612,613,783,737,\
		584,782,615,616,617,618,619,626,621,622,623,624,780,627,628,629,630,632,633,634,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,680,681,682,684,685,686,687,688,689,691,692,693,694,695,836,696,721,841,\
		573,717,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,718,719,720,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,834,845)";
			
		auto profiles = ap::Database::getInstance().getProfiles();
		for (auto profile : profiles)
		{
			try {
				SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile.profile_id),
					SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
				SQLite::Statement stmt(db, query);
				stmt.exec();

				SQLite::Statement stmt2(db, "REPLACE INTO tbl_profile (key, value) \
                                   VALUES (?,?),(?,?),(?,?),(?,?),(?,?),(?,?)");
				int bind_index = 1;
				for (auto cat : phonic_category) {
					stmt2.bind(bind_index++, cocos2d::StringUtils::format(key_last_activity_x, cat));
					stmt2.bind(bind_index++, "0");
					stmt2.bind(bind_index++, cocos2d::StringUtils::format(key_last_lesson_x, cat));
					stmt2.bind(bind_index++, "0");
				}
				stmt2.exec();
			}
			catch (std::exception & e) {
				CCLOG("%s db exception: %s", __FUNCTION__, e.what());
			}
		}
	}

	int ProfileManager::getCurrentProfileId() const
	{
		return MJDEFAULT->getIntegerForKey(key_current_profile_id, 0);
	}

	void ProfileManager::setCurrentProfileId(int profile_id)
	{
		_current_profile = profile_id;
		MJDEFAULT->setIntegerForKey(key_current_profile_id, profile_id);
	}


	void ProfileManager::loadDataFromMJDefault()
	{
		auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
		//key_stories_recentlyread
		auto story_rread = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang));
		setStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang), story_rread);
		//key_liststories_docroi
		auto story_dadoc = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang));
		setStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang), story_dadoc);
		//key_audio_recentlyread
		auto audio_rread = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang));
		setStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang), audio_rread);
		//key_listaudiobook_docroi
		auto audio_dadoc = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang));
		setStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang), audio_dadoc);

		////key_favorites
		//auto key_en = key_favorites + StringUtils::toString(1); // EN
		//auto list_favorites = MJDEFAULT->getStringForKey(key_en);
		//setStringForKey(key_en, list_favorites);
		////key_favorites
		//auto key_vi = key_favorites + StringUtils::toString(4); // VI
		//auto list_favorites = MJDEFAULT->getStringForKey(key_vi);
		//setStringForKey(key_vi, list_favorites);

		// key grade manager
		setIntegerForKey(key_grade_audiobooks, MJDEFAULT->getIntegerForKey(key_grade_audiobooks, -1));
		setIntegerForKey(key_grade_lessons, MJDEFAULT->getIntegerForKey(key_grade_lessons, CONFIG_MANAGER->isVietnameseStories()?764:84));
		setIntegerForKey(key_grade_lessons_phonic, MJDEFAULT->getIntegerForKey(key_grade_lessons_phonic, 479));
		setIntegerForKey(key_grade_stories, MJDEFAULT->getIntegerForKey(key_grade_stories, -1));

		// MS Lesson Manager key_last_lesson & key_last_activity
		auto categories = StoryDataModel::getInstance()->getCategories(StoryLanguageManager::getCurrentLangId());
		for (auto cate : categories)
		{
            auto key_lesson = StringUtils::format(key_last_lesson_x, cate.category_id);
            auto key_activity = StringUtils::format(key_last_activity_x, cate.category_id);
			setIntegerForKey(key_lesson, MJDEFAULT->getIntegerForKey(key_lesson));
			setIntegerForKey(key_activity, MJDEFAULT->getIntegerForKey(key_activity));
		}
		// Key play chapter
		auto chapters = StoryDataModel::getInstance()->getAudiobookChapters();
		for (auto chapter : chapters)
		{
			auto key_chapter = StringUtils::format(key_last_time_played_chapter_x, chapter.book_id);
			auto key_play = StringUtils::format(key_last_time_played_chapter_x, chapter.book_id);
			setIntegerForKey(key_chapter, MJDEFAULT->getIntegerForKey(key_chapter, -1));
			setIntegerForKey(key_play, MJDEFAULT->getIntegerForKey(key_play, -1));
		}
        
        //đối với profile đầu tiên đc tạo, lưu process vào db profile, và ko clear db chính
        saveProcess();
        
	}

	int ProfileManager::getProfileGradeId(ms::GradeManager::Scope scope) const
	{
		switch (scope) {
		case ms::GradeManager::Scope::AUDIOBOOKS:
			return getIntegerForKey(key_grade_audiobooks, -1);

		case ms::GradeManager::Scope::LESSONS:
			return getIntegerForKey(key_grade_lessons, 84);

		case ms::GradeManager::Scope::LESSONS_PHONIC:
			return getIntegerForKey(key_grade_lessons_phonic, 479);

		case ms::GradeManager::Scope::STORIES:
			return getIntegerForKey(key_grade_stories, -1);
		}
	}

	void ProfileManager::setProfileGradeId(ms::GradeManager::Scope scope, int grade_id) const
	{
		switch (scope) {
		case ms::GradeManager::Scope::AUDIOBOOKS:
			setIntegerForKey(key_grade_audiobooks, grade_id);
			break;

		case ms::GradeManager::Scope::LESSONS:
			setIntegerForKey(key_grade_lessons, grade_id);
			break;

		case ms::GradeManager::Scope::LESSONS_PHONIC:
			setIntegerForKey(key_grade_lessons_phonic, grade_id);
			break;

		case ms::GradeManager::Scope::STORIES:
			setIntegerForKey(key_grade_stories, grade_id);
			break;
		}
	}

	void ProfileManager::setStringForKey(const std::string & key, const std::string & value) const
	{
		getInstance().setDataBaseStringForKey(key, value);
	}

	void ProfileManager::setIntegerForKey(const std::string & key, int value) const
	{
		getInstance().setDataBaseStringForKey(key, StringUtils::format("%d", value));
	}

	int ProfileManager::getIntegerForKey(const std::string & key, int default_value) const
	{
		auto value = getInstance().getDataBaseStringForKey(key, StringUtils::format("%d", default_value));
        if(value == ""){
            return default_value;
        }
		return std::stoi(value);
	}

	std::string ProfileManager::getStringForKey(const std::string & key, std::string default_value) const
	{
		auto value = getInstance().getDataBaseStringForKey(key, default_value);
		return value;
	}

	void ProfileManager::addStoryToRecentlyRead(int _id)
	{
		auto maxItem = 15;//chỉ lưu 15 truyện đọc gần nhất.
		auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
		auto list_recentlyread = getStoryRecentlyRead();

		for (int i = 0; i < (int)list_recentlyread.size(); i++) {
			if (_id == list_recentlyread[i].asInt()) {
				list_recentlyread.erase(list_recentlyread.begin() + i);
				break;
			}
		}

		if ((int)list_recentlyread.size() >= maxItem) {
			list_recentlyread.erase(list_recentlyread.begin() + maxItem - 1);
		}

		list_recentlyread.insert(list_recentlyread.begin(), Value(_id));
		setStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang), JsonDump::getInstance()->VectorToString(list_recentlyread));


		//add vào list - lưu lại tất cả truyện đã đọc.
		ValueVector list_dadoc;
		auto str_list_dadoc = getStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang), "");
		if (str_list_dadoc != "") {
			list_dadoc = JsonDump::getInstance()->dumpValueVectorFromString(str_list_dadoc);
		}

		for (int i = 0; (int)i < list_dadoc.size(); i++) {
			if (_id == list_dadoc[i].asInt()) {
				list_dadoc.erase(list_dadoc.begin() + i);
				break;
			}
		}
		list_dadoc.push_back(Value(_id));
		setStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang), JsonDump::getInstance()->VectorToString(list_dadoc));
	}

	cocos2d::ValueVector ProfileManager::getStoryRecentlyRead()
	{
		ValueVector re_list;
		auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
		auto str_recentlyread = getStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang), "");
		if (str_recentlyread != "") {
			re_list = JsonDump::getInstance()->dumpValueVectorFromString(str_recentlyread);
		}
		return re_list;
	}

	void ProfileManager::addAudioToRecentlyRead(int _id)
	{
		auto maxItem = 15;//chỉ lưu 15 truyện đọc gần nhất.
		auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
		auto list_recentlyread = getAudioRecentlyRead();

		for (int i = 0; i < (int)list_recentlyread.size(); i++) {
			if (_id == list_recentlyread[i].asInt()) {
				list_recentlyread.erase(list_recentlyread.begin() + i);
				break;
			}
		}

		if ((int)list_recentlyread.size() >= maxItem) {
			list_recentlyread.erase(list_recentlyread.begin() + maxItem - 1);
		}

		list_recentlyread.insert(list_recentlyread.begin(), Value(_id));
		setStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang), JsonDump::getInstance()->VectorToString(list_recentlyread));


		//add vào list - lưu lại tất cả audio đã đọc.
		ValueVector list_dadoc;
		auto str_list_dadoc = getStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang), "");
		if (str_list_dadoc != "") {
			list_dadoc = JsonDump::getInstance()->dumpValueVectorFromString(str_list_dadoc);
		}

		for (int i = 0; (int)i < list_dadoc.size(); i++) {
			if (_id == list_dadoc[i].asInt()) {
				list_dadoc.erase(list_dadoc.begin() + i);
				break;
			}
		}
		list_dadoc.push_back(Value(_id));
		setStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang), JsonDump::getInstance()->VectorToString(list_dadoc));
	}

	cocos2d::ValueVector ProfileManager::getAudioRecentlyRead()
	{
		ValueVector re_list;
		auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
		auto str_recentlyread = getStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang), "");
		if (str_recentlyread != "") {
			re_list = JsonDump::getInstance()->dumpValueVectorFromString(str_recentlyread);
		}
		return re_list;
	}

	//void ProfileManager::addFavoritesStory(int _id) {
	//	int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
	//	auto maxItem = max_number_favorite;
	//	auto list_favorites = getFavoritesStory();
	//	bool is_find = false;

	//	for (int i = 0; i < (int)list_favorites.size(); i++) {
	//		if (list_favorites[i].asInt() == _id) {
	//			is_find = true;
	//		}
	//	}

	//	if (!is_find) {
	//		if ((int)list_favorites.size() >= maxItem) {
	//			list_favorites.erase(list_favorites.begin() + maxItem - 1);
	//		}
	//		list_favorites.insert(list_favorites.begin(), Value(_id));

	//		auto key = key_favorites + StringUtils::toString(lang_id);
	//		setStringForKey(key, JsonDump::getInstance()->VectorToString(list_favorites));
	//	}
	//}

	//void ProfileManager::removeFavoritesStory(int _id) {
	//	int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
	//	auto list_favorites = getFavoritesStory();
	//	for (int i = 0; i < (int)list_favorites.size(); i++) {
	//		if (_id == list_favorites[i].asInt()) {
	//			list_favorites.erase(list_favorites.begin() + i);
	//			//            StoryDataModel::getInstance()->setStoryRead(_id, 1, 1);
	//			//            mj::PageLoadResource::getInstance().deleteCacheOfStory(_id);
	//			break;
	//		}
	//	}
	//	auto key = key_favorites + StringUtils::toString(lang_id);
	//	setStringForKey(key, JsonDump::getInstance()->VectorToString(list_favorites));
	//}

	//ValueVector ProfileManager::getFavoritesStory() {
	//	int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
	//	auto key = key_favorites + StringUtils::toString(lang_id);
	//	auto str_list = getStringForKey(key,"");
	//	if (lang_id == 1 && str_list == "") {
	//		str_list =getStringForKey(key,"");
	//	}
	//	return JsonDump::getInstance()->dumpValueVectorFromString(str_list);
	//}

	//bool ProfileManager::isFavoritesStory(int _id) {
	//	auto list_favorites = getFavoritesStory();
	//	for (int i = 0; i < (int)list_favorites.size(); i++) {
	//		if (_id == list_favorites[i].asInt()) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}

    void ProfileManager::setRow(std::string tbl_name, std::string column_1, std::string column_2, std::string column_3, int value_1, int value_2, int value_3)
    {
        try
        {
            std::string query = StringUtils::format("REPLACE INTO %s (%s, %s, %s) VALUES (?, ?, ?)", tbl_name.c_str(), column_1.c_str(), column_2.c_str(), column_3.c_str());
            SQLite::Statement stmt(*_db, query);
            stmt.bind(1, value_1);
            stmt.bind(2, value_2);
            stmt.bind(2, value_3);
            stmt.exec();
        }
        catch (std::exception & e)
        {
            std::cout << "db exception: " << e.what() << std::endl;
        }
    }

	void ProfileManager::setDataBaseStringForKey(std::string key, std::string value) {
		try {
			SQLite::Statement stmt(*_db, "REPLACE INTO tbl_profile (key, value) VALUES (?, ?)");
			stmt.bind(1, key);
			stmt.bind(2, XHSLib->mahoaString(value));
			stmt.exec();
		}
		catch (std::exception & e) {
			std::cout << "db exception: " << e.what() << std::endl;
		}
	}

	std::string ProfileManager::getDataBaseStringForKey(std::string key, std::string default_value) {
		try {
			SQLite::Statement stmt(*_db, "SELECT value FROM tbl_profile WHERE key = ?");
			stmt.bind(1, key);
			stmt.executeStep();
			auto value = XHSLib->giaimaString(stmt.getColumn(0).getText());
			return value;
		}
		catch (std::exception & e) {
			std::cout << "db exception: " << e.what() << std::endl;
			return default_value;
		}
	}


	ProfileManager::ProfileManager() : _db(nullptr), _current_profile(0)
	{
	}

#pragma GENERAL UPDATE FUNCTIONS
#include "MSLessonManager.h"
    void ProfileManager::updateLearnedLesson(int profile_id, int lesson_id, int category_id) const {
        try {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            SQLite::Statement stmt(db, "INSERT OR IGNORE INTO tbl_lesson_learned (id, category_id, learned) \
                                   VALUES(?, ?, ?)");
            stmt.bind(1, lesson_id);
            stmt.bind(2, category_id);
            stmt.bind(3, 1);
            stmt.exec();
            
            if(profile_id == getCurrentProfileId() )
            {
//                StoryDataModel::getInstance()->setLessonLearned({lesson_id});
                ms::LessonManager::getInstance().setLessonLearned(lesson_id);
//                ms::LessonManager::getInstance().updateAllLesson();
            }
        }catch (std::exception & e) {
            CCLOG("%s db exception: %s", __FUNCTION__, e.what());
        }
    }
    
    void ProfileManager::updateReadStories(int profile_id, int story_id, int time_read) const {
            try {
                SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
                SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
                SQLite::Statement stmt(db, "INSERT OR IGNORE INTO tbl_story_read (id_story_lang, read, time_read) \
                                       VALUES(?, ?, ?)");
                stmt.bind(1, story_id);
                stmt.bind(2, 1);
                stmt.bind(3, time_read);
                stmt.exec();
                if(profile_id == getCurrentProfileId() )
                {
                    StoryDataModel::getInstance()->setStoryRead(story_id);
                }
            }catch (std::exception & e) {
                CCLOG("%s db exception: %s", __FUNCTION__, e.what());
            }
    }

    void ProfileManager::updateLoveStories(int profile_id, int story_id) const
    {
        try
        {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
                                SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            SQLite::Statement stmt(db, "INSERT OR IGNORE INTO tbl_love_stories (story_id) \
                                   VALUES(?)");
            stmt.bind(1, story_id);
            
            stmt.exec();
        }
        catch (std::exception & e)
        {
            CCLOG("%s db exception: %s", __FUNCTION__, e.what());
        }
    }

    void ProfileManager::updateReadAudios(int profile_id, int story_id, int time_read) const {
            try {
                SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
                SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
                SQLite::Statement stmt(db, "INSERT OR IGNORE INTO tbl_audio_read (audio_id, read, time_read) \
                                       VALUES(?, ?, ?)");
                stmt.bind(1, story_id);
                stmt.bind(2,  1);
                stmt.bind(3, time_read);
                stmt.exec();
                if(profile_id == getCurrentProfileId() )
                {
                    StoryDataModel::getInstance()->setAudioRead(story_id);
                }
            }catch (std::exception & e) {
                CCLOG("%s db exception: %s", __FUNCTION__, e.what());
            }
    }

    void ProfileManager::updateLoveAudios(int profile_id, int audio_id) const
    {
        try
        {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
                                SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            SQLite::Statement stmt(db, "INSERT OR IGNORE INTO tbl_love_audiobooks (audio_id) \
                                   VALUES(?)");
            stmt.bind(1, audio_id);
            
            stmt.exec();
        }
        catch (std::exception & e)
        {
            CCLOG("%s db exception: %s", __FUNCTION__, e.what());
        }
    }

    void ProfileManager::updateProfileKeys(int profile_id, std::string key, std::string value) const {
        if(value.empty()) return;
        try {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),
            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            SQLite::Statement stmt(db, "REPLACE INTO tbl_profile (key, value) \
                                   VALUES(?, ?)");
            stmt.bind(1, key);
            stmt.bind(2, value);
            stmt.exec();
        }catch (std::exception & e) {
            CCLOG("%s db exception: %s", __FUNCTION__, e.what());
        }
    }

    void ProfileManager::deleteLoveStories(int profile_id, int story_id) const
    {
        try
        {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),SQLite::OPEN_READWRITE);
            
            SQLite::Statement stmt(db, "DELETE FROM tbl_love_stories WHERE story_id = ?");
            stmt.bind(1, story_id);
            stmt.exec();
        }
        catch (std::exception & e)
        {
            CCLOG("db exception: %s", e.what());
        }
    }

    void ProfileManager::deleteLoveAudios(int profile_id, int story_id) const
    {
        try
        {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),SQLite::OPEN_READWRITE);
            
            SQLite::Statement stmt(db, "DELETE FROM tbl_love_stories WHERE audio_id = ?");
            stmt.bind(1, story_id);
            stmt.exec();
        }
        catch (std::exception & e)
        {
            CCLOG("db exception: %s", e.what());
        }
    }

    std::vector<int>  ProfileManager::getAllReadStories(int profileID)
    {
        std::vector<int> result ;

        if(profileID == -1)
        {
            CCLOG("ProfileManager::getAllReadStories| error | cannot excute this function because the profileID is invaild.");
            
            return result;
        }
        
        try
        {
            const auto k_filePath = FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID);

            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            SQLite::Statement stmtReadStories(db, "SELECT * from tbl_story_read");
            
            while (stmtReadStories.executeStep())
            {
                if(stmtReadStories.getColumnCount() > 0)
                {
                    result.push_back(stmtReadStories.getColumn(0).getInt());
                }
            }
        }
        catch (std::exception & e)
        {
            CCLOG("ProfileManager::getAllReadStories| %s db exception: %s", __FUNCTION__, e.what());
        }
        
        return result;
    }

    void ProfileManager::deleteLoveXXXBook(ProfileManager::BOOK_TYPE i_type ,int profile_id )
    {
        try
        {
            SQLite::Database db(FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profile_id),SQLite::OPEN_READWRITE);
            
            SQLite::Statement stmt(db,StringUtils::format("DELETE FROM %s",i_type ==ProfileManager::BOOK_TYPE::STORY ? "tbl_love_stories":"tbl_love_audiobooks"));
            stmt.exec();
        }
        catch (std::exception & e)
        {
            CCLOG("db exception: %s", e.what());
        }
    }

    std::vector<int>  ProfileManager::getAllReadAudiobooks(int profileID)
    {
        
        std::vector<int> result ;

        if(profileID == -1)
        {
            CCLOG("ProfileManager::getAllReadAudiobooks| error | cannot excute this function because the profileID is invaild.");
            
            return result;
        }
        
        try
        {
            const auto k_filePath = FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID);

            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            SQLite::Statement stmtReadStories(db, "SELECT * from tbl_audio_read");
            
            while (stmtReadStories.executeStep())
            {
                if(stmtReadStories.getColumnCount() > 0)
                {
                    result.push_back(stmtReadStories.getColumn(0).getInt());
                }
            }
        }
        catch (std::exception & e)
        {
            CCLOG("ProfileManager::getAllReadAudiobooks| %s db exception: %s", __FUNCTION__, e.what());
        }
        
        return result;
    }

    std::vector<int> ProfileManager::getAllLoveStories(int profileID)
    {
         std::vector<int> result ;

         if(profileID == -1)
         {
             CCLOG("ProfileManager::getAllLoveStories| error | cannot excute this function because the profileID is invaild.");
             
             return result;
         }
         
         try
         {
             const auto k_filePath = FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID);

             SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

             SQLite::Statement stmtReadStories(db, "SELECT * from tbl_love_stories");
             
             while (stmtReadStories.executeStep())
             {
                 if(stmtReadStories.getColumnCount() > 0)
                 {
                     result.push_back(stmtReadStories.getColumn(0).getInt());
                 }
             }
         }
         catch (std::exception & e)
         {
             CCLOG("ProfileManager::getAllLoveStories| %s db exception: %s", __FUNCTION__, e.what());
         }
         
         return result;
     }

    std::vector<int> ProfileManager::getAllLoveAudiobooks(int profileID)
    {
        std::vector<int> result ;

        if(profileID == -1)
        {
            CCLOG("ProfileManager::getAllLoveAudiobooks| error | cannot excute this function because the profileID is invaild.");
            
            return result;
        }
        
        try
        {
            const auto k_filePath = FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID);

            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            SQLite::Statement statement(db, "SELECT * from tbl_love_audiobooks");
            
            while (statement.executeStep())
            {
                if(statement.getColumnCount() > 0)
                {
                    result.push_back(statement.getColumn(0).getInt());
                }
            }
        }
        catch (std::exception & e)
        {
            CCLOG("ProfileManager::getAllLoveAudiobooks| %s db exception: %s", __FUNCTION__, e.what());
        }
        
        return result;
    }


	std::vector<int> ProfileManager::getAllLessonLearned(int profileID)
	{
		std::vector<int> result ;

		if(profileID == -1)
		{
			CCLOG("ProfileManager::getAllLoveAudiobooks| error | cannot excute this function because the profileID is invaild.");

			return result;
		}

		try
		{
			const auto k_filePath = FOLDER_PROFILE + StringUtils::format("profile_%d.sqlite", profileID);

			SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

			SQLite::Statement statement(db, "SELECT * from tbl_lesson_learned");

			while (statement.executeStep())
			{
				if(statement.getColumnCount() > 0)
				{
					result.push_back(statement.getColumn(0).getInt());
				}
			}
		}
		catch (std::exception & e)
		{
			CCLOG("ProfileManager::getAllLoveAudiobooks| %s db exception: %s", __FUNCTION__, e.what());
		}

		return result;
	}

	void ProfileManager::resetFavoriteBooks()
    {
        //delete old profile.
        int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
        
        auto key = key_favorites + StringUtils::toString(lang_id);
        
        MJDEFAULT->deleteValueForKey(key);
        
        //update loving audiobook.
        
        auto audiobooks = getAllLoveAudiobooks(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
        
        for (auto id : audiobooks)
        {
            XHSLib->addFavoritesStory(id+max_id_stories);
        }
        
        //update loving stories
        auto stories = getAllLoveStories(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
        
        for (auto id : stories)
        {
            XHSLib->addFavoritesStory(id);
        }
        
    }

}
