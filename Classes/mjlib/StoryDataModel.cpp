//
//  StoryDataModel.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#include "StoryDataModel.h"
#include "GlobalFunc.h"
#include "HSLib.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "StoryFreeManager.h"

#include "APProfileManager.h"
#include "MSGradeManager.h"
#include "MSLessonManager.h"
#include <numeric>
#include "MSCacheManager.h"
#include "MJDefinitions.h""
#define NUMBER_QUERY 100
#define NUMBER_QUERY_ST_AU 50

#define SQLITE_COLUMN_INT(x) sqlite3_column_int(this->stmp, x)
#define SQLITE_COLUMN_STR(x) std::string(reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, x)))

#define SQLITE_COLUMN_INT_STMT(x, y) sqlite3_column_int(x, y)
#define SQLITE_COLUMN_STR_STMT(x, y) std::string(reinterpret_cast<const char *>(sqlite3_column_text(x, y)))

#define SQLITE_BIND_INT(x, v) sqlite3_bind_int(this->stmp, x, v)
#define SQLITE_BIND_STR(x, v) sqlite3_bind_text(this->stmp, x, v.c_str(), -1, SQLITE_TRANSIENT)

#define key_debugkey_dev_environment1 true

//Nếu list var có size lớn hơn 50
//thì mỗi lần hàm func chỉ add tối đa 50 model
//=> để tăng tốc độ add vào db.
#define MJ_DB_SPLIT_QUERY(func, model, var) \
if (static_cast<int>(var.size()) > 50) { \
auto first = 0; \
const auto chunk_size = 50; \
while (first < static_cast<int>(var.size())) { \
auto last = std::min(first + chunk_size, static_cast<int>(var.size())); \
func(std::vector<model>(std::begin(var) + first, std::begin(var) + last)); \
first = last; \
} \
return;\
}


StoryDataModel* StoryDataModel::instance = NULL;
StoryDataModel* StoryDataModel::getInstance(string dataName)
{
    string dbpath = FileUtils::getInstance()->getWritablePath() + dataName + ".sqlite";
    if (instance == nullptr) {
        instance = new StoryDataModel();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (dataName == "cards") {
            dbpath = FileUtils::getInstance()->getWritablePath() + "Documents/" + dataName + ".sqlite";
        }
#endif
        instance->db_path = dbpath;
        instance->openConnect();
        instance->prepareDatabase();
    }
    if(!FileUtils::getInstance()->isFileExist(dbpath))
    {
        
    }
    return instance;
}

void StoryDataModel::openConnect() {
    if (sqlite3_open(this->db_path.c_str(), &this->db) != SQLITE_OK) {
        sqlite3_close(this->db);
        CCLOG("Connect DB co loi");
    }
    else {
        sqlite3_exec(this->db, "PRAGMA journal_mode = MEMORY", nullptr, nullptr, nullptr);
        CCLOG("Connect DB thanh cong");
    }
}

void StoryDataModel::prepareDatabase() {
    std::string query = "CREATE TABLE if not exists tbl_story(id INTEGER PRIMARY KEY, story_id INTEGER, name TEXT, words INTEGER, lexile TEXT, level TEXT, lang_id INTEGER, total_page INTEGER, image TEXT, written TEXT, design TEXT, search TEXT, quality INTEGER, date_publish INTEGER, level_max INTEGER, version INTEGER)";
    
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story");
    }
    
    query = "CREATE TABLE if not exists tbl_story_tutoring_new(id INTEGER PRIMARY KEY, app_id INTEGER)";
    
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_tutoring_new");
    }

    query = "CREATE TABLE tbl_story_feature (id INTEGER PRIMARY KEY, story_id INTEGER, name TEXT, words INTEGER, lexile TEXT, level INTEGER, lang_id INTEGER, total_page INTEGER, image TEXT, written TEXT, design TEXT, search TEXT, _order INTEGER, image_feature TEXT, quality INTEGER)";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_feature");
    }

    query = "CREATE TABLE tbl_story_popular (id INTEGER PRIMARY KEY, story_id INTEGER, lang_id INTEGER)";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_popular");
    }

    query = "CREATE TABLE tbl_story_sentapi (id INTEGER PRIMARY KEY, story_id INTEGER, type INTEGER)";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_sentapi");
    }

    query = "CREATE TABLE tbl_story_read (id_story_lang INTEGER PRIMARY KEY, read INTEGER, time_read INTEGER)";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_read");
    }

    query = "CREATE TABLE tbl_story_download (id INTEGER PRIMARY KEY, storyid INTEGER, langid INTEGER, downloaded INTEGER DEFAULT (0))";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_story_download");
    }

    query = "create table if not exists tbl_story ("\
        "story_id integer,"\
        "level_id integer,"\
        "primary key(story_id, level_id))";
    auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
    }
    CCLOG("Created table tbl_story_level");

    // table of materials
    query = "create table if not exists tbl_material ("\
        "mat_id integer not null,"\
        "type integer,"\
        "lesson_id integer,"\
        "date_published integer,"\
        "name_worksheet text,"\
        "name_lesson text,"\
        "thumb text,"\
        "levels text,"\
        "categories text,"\
        "primary key (mat_id))";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_material");
    }

    // table of popular materials
    query = "create table if not exists tbl_material_popular ("\
        "popular_id integer not null,"\
        "date_published integer,"\
        "type integer,"\
        "sort integer,"\
        "keyword text,"\
        "thumb text,"\
        "primary key (popular_id))";
    if (auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        showError(r);
    }
    else {
        CCLOG("Created table tbl_material_popular");
    }

    query = "create table if not exists tbl_story_category (story_id integer, category_id integer, primary key(story_id, category_id))";
    r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
    }
    CCLOG("Created table tbl_story_category");

    query = "create table if not exists tbl_story_character (story_id integer, character_id integer, primary key(story_id, character_id))";
    r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
    }
    CCLOG("Created table tbl_story_character");

    query = "create table if not exists tbl_reading_levels ("\
        "level_id integer,"\
        "lang_id integer,"\
        "grade_id integer,"\
        "name text,"\
        "description text,"\
        "primary key (level_id, lang_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    CCLOG("Created table tbl_reading_levels");

    query = "create table if not exists tbl_grades ("\
        "grade_id integer, "\
        "lang_id integer, "\
        "name text, "\
        "description text,"\
        "primary key (grade_id, lang_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    CCLOG("Created table tbl_grades");

    query = "create table if not exists tbl_new_grade (grade_id integer, lang_id integer, name text, description text, primary key (grade_id, lang_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("Created table tbl_new_grade");
    }


    query = "create table if not exists tbl_story_rate ("\
        "story_lang_id integer, "\
        "rate_value integer, "\
        "primary key (story_lang_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    CCLOG("Created table tbl_story_rate");

    query = "create table if not exists tbl_audiobook_v2 ("
        "_id integer not null,"
        "lang_id integer,"
        "grade_id integer,"
        "name text,"
        "thumb_url text,"
        "content_path text,"
        "audio_url text,"
        "summary text,"
        "duration integer,"
        "quality integer,"
        "score integer,"
        "date_publish integer,"
        "extra text,"
        "primary key (_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("Created table tbl_audiobook_v2");
    }

    //create new table for filesize
    query = "create table if not exists tbl_file_size (url text not null, file_size integer not null,  primary key (url))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("Created table tbl_file_size");
    }

    query = "create table if not exists tbl_lesson_v2 (_id integer not null, _order integer, lang_id integer, category_id integer, thumb_url text, name text, learned integer DEFAULT(0), time_published integer, primary key (_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_lesson_v2");
    }

    query = "create table if not exists tbl_activity ("
        "_id integer not null,"
        "_order integer,"
        "lesson_id integer,"
        "sub_id integer,"
        "type integer,"
        "name text,"
        "content text,"
        "resource_zip text,"
        "data_zip text,"
        "primary key (_id, lesson_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("Created table tbl_activity");
    }

    query = "create table if not exists tbl_game (_id integer not null, name text, resource_zip text, thumb_url text, primary key(_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_game");
    }

    query = "create table if not exists tbl_audiobook_series2 ("
        "id integer not null,"
        "title text,"
        "thumb text,"
        "book text,"
        "hidden integer,"
        "primary key (id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_audiobook_series2");
    }

    query = "create table if not exists tbl_audiobook_popular_search ("
        "id integer not null,"
        "keyword text,"
        "thumb text,"
        "sort integer,"
        "date_publish integer,"
        "primary key (id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_audiobook_popular_search");
    }

    query = "create table if not exists tbl_story_popular_search ("
        "id integer not null,"
        "keyword text,"
        "thumb text,"
        "sort integer,"
        "date_publish integer,"
        "primary key (id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_story_popular_search");
    }


    query = "create table if not exists tbl_audio_read ("
        "audio_id integer,"
        "read integer,"
        "time_read integer,"
        "primary key (audio_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_audio_read");
    }

    query = "create table if not exists tbl_story_link ("
        "story_id integer,"
        "story_link_hd text,"
        "story_link_hdr text,"
        "primary key (story_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_stories_links");
    }

    query = "create table if not exists license_device_info ("
        "device_id integer not null,"
        "device_name text,"
        "device_status integer,"
        "active_date integer,"
        "deactive_date integer,"
        "primary key (device_id))";
    auto result = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK) {
        showError(result);
    }
    else {
        CCLOG("Created table license_device_info");
    }

    query = "CREATE TABLE IF NOT EXISTS tbl_license("
        "license CHAR(11) PRIMARY KEY,"
        "phone TEXT,"
        "mail TEXT,"
        "name TEXT,"
        "product_id TEXT,"
        "date_enter INTEGER,"
        "date_effect INTEGER,"
        "date_expire INTEGER,"
        "current INT NOT NULL DEFAULT '0',"
        "total_active INTEGER,"
        "current_active INTEGER)";
    result = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK) {
        showError(result);
    }

    query = "CREATE TABLE IF NOT EXISTS tbl_license_device(device_id INTEGER, license TEXT, status INTEGER, PRIMARY KEY(device_id, license))";
    result = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK) {
        showError(result);
    }

    query = "create table if not exists audiobook_chapters ("
        "main_id integer not null,"
        "chapter_id integer not null,"
        "chapter_order integer not null,"
        "primary key (main_id, chapter_id))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table audiobook_chapters");
    }

    //create table story _ activities
    query = "create table if not exists tbl_story_activities ("
        "_id integer not null,"
        "_order integer,"
        "lesson_id integer not null,"
        "page integer,"
        "sub_id integer,"
        "type integer,"
        "name text,"
        "content text,"
        "resource_zip text,"
        "data_zip text,"
        "primary key (_id, lesson_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_story_activities");
    }

    //create table tbl_new_product_list
    query = "create table if not exists tbl_new_product_list ("
        "id integer not null,"
        "city text,"
        "tien_ao integer,"
        "giam_40 integer,"
        "giam_30 integer,"
        "sale_off integer,"
        "product_id text,"
        "product_name text,"
        "product_description text,"
        "product_code text,"
        "current_unit text,"
        "primary key (id, product_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_new_product_list");
    }

    //create table tbl_ai_speaking_topic
    //topic_id, topic_name, thumb_url, free, status, cat_version, order_by,icon_zip_url, icon_thumb, check_icon, disable_icon, progress_bg_icon, selected_circle_icon, design_icon 
    query = "create table if not exists tbl_ai_speaking_topic ("
        "topic_id integer not null,"
        "topic_name text,"
        "thumb_url text,"
        "free integer,"
        "status integer,"
        "cat_version integer,"
        "order_by integer,"
        "icon_zip_url text,"
        "icon_thumb text,"
        "check_icon text,"
        "disable_icon text,"
        "progress_bg_icon text,"
        "selected_circle_icon text,"
        "design_icon text,"
        "primary key (topic_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_ai_speaking_topic");
    }


    //create table tbl_ai_speaking_lessons
    //_id integer not null, _order integer, lang_id integer, category_id integer, thumb_url text, name text, learned integer DEFAULT(0), time_published integer, primary key (_id)
    query = "create table if not exists tbl_ai_speaking_lessons ("
        "_id integer not null,"
        " _order integer,"
        " lang_id integer,"
        " category_id integer,"
        " thumb_url text,"
        " name text,"
        " learned integer DEFAULT(0),"
        " time_published integer,"
        " timeLearnedExpected integer,"
        " primary key (_id))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_ai_speaking_lessons");
    }
    //create table Game list error

    query = "create table if not exists tbl_game_list_error ("
        "game_id integer not null,"
        "primary key (game_id))";
        
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    {
        CCLOG("created table tbl_game_list_error");
    }

    // creat ai speaking common table 
    /*
                int id = 0;
            std::string actor_name = "";
            std::string version_name = "";
            std::string zipPath = "";
            std::string intro = "";
            std::string intro_mimic = "";
            std::string intro_talking = "";
            std::string your_turn = "";
            std::string cheer_up = "";
            std::string listening = "";
            std::string outro_mimic = "";
            std::string outro = "";
    */
    query = "create table if not exists tbl_ai_speaking_common ("
        "_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        " actor_name text,"
        " version_name text,"
        " zipPath text,"
        " intro text,"
        " intro_mimic text,"
        " intro_talking text,"
        " your_turn text,"
        " cheer_up text,"
        " listening text,"
        " outro_mimic text,"
        " outro text)";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_ai_speaking_common");
    }
    
    //create table list doamin dowload story

    query = "create table if not exists tbl_list_doamin_stories_dowload ("
        "domain_url text not null,"
        "primary key (domain_url))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_list_doamin_stories_dowload");
    }

    //create table list doamin dowload activity, audiobook

    query = "create table if not exists tbl_list_doamin_audiobook_activity_dowload ("
        "domain_url text not null,"
        "primary key (domain_url))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_list_doamin_audiobook_activity_dowload");
    }

    //create table tbl_free_trial
    /*
            int _id = 0;
            int _lang_id = 1;

            bool _isLearn = false;
            bool _isLock = false;
            FreeContentOption _option = FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY;
            DocumentsType _type = DocumentsType::STORY;
    */
    query = "create table if not exists tbl_free_trial ("
        
        " _idFree integer not null,"
        " _id integer not null,"
        " _lang_id integer,"
        " _isLearn integer,"
        " _isLock integer,"
        " _option integer,"
        " _type integer,"
        " _day integer,"
        " primary key (_idFree))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created table tbl_free_trial");
    }
}

void StoryDataModel::dropDatabase(Node* node)
{
    std::string dbName = this->db_path.c_str();
    std::vector<std::string> tableList;
    char **papszResult = nullptr;
   int nRowCount, nColCount;
   char *pszErrMsg = NULL;
    std::string query ="SELECT name FROM sqlite_master WHERE type = 'table'";
    if (sqlite3_get_table(this->db, query.c_str(),&papszResult, &nRowCount, &nColCount, &pszErrMsg)!= SQLITE_OK)
    {
        logError();

    }
    else {
        if ( nRowCount < 1 )
            logError();
         else
         {
           for (int i = 1; i <= nRowCount; i++ )
           {
             if ( papszResult[( i * nColCount ) + 0] )
             {
               const char *name = papszResult[( i * nColCount ) + 0];
               if ( name )
                   tableList.push_back(name);
             }
           }
         }
    }
    if(papszResult)
        sqlite3_free_table(papszResult);
    
    node->scheduleOnce([=](float dt) {
        for(std::string tableName: tableList){
            std::string querydeleteTable = StringUtils::format("DROP TABLE IF EXISTS %s", tableName.c_str());
            if (sqlite3_exec(this->db, querydeleteTable.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
                logError();
            }
            else {
                CCLOG("Deleted tables: %s", tableName.c_str());
            }
        }
    }, .5f, "dropTables");

}
void StoryDataModel::closeConnect() {
    sqlite3_close(this->db);
}

void StoryDataModel::showError(int result) {
    CCLOG("Database error: %d", result);
}

void StoryDataModel::executeQuery(string query, ValueVector query_vector) {
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    //for qua cac phan tu roi bind vao stmp
    int placeholder_index = 1;

    for (int i = 0; i < query_vector.size(); i++) {
        cocos2d::Value::Type vType = query_vector.at(i).getType();
        if (vType == cocos2d::Value::Type::STRING) {
            sqlite3_bind_text(this->stmp, placeholder_index++, query_vector.at(i).asString().c_str(), -1, SQLITE_TRANSIENT);
        }
        else if (vType == cocos2d::Value::Type::INTEGER) {
            sqlite3_bind_int(this->stmp, placeholder_index++, query_vector.at(i).asInt());
        }
        else if (vType == cocos2d::Value::Type::FLOAT || vType == cocos2d::Value::Type::DOUBLE) {
            sqlite3_bind_double(this->stmp, placeholder_index++, query_vector.at(i).asDouble());
        }
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
}

ValueMap StoryDataModel::getRow(string query, ValueVector query_vector) {
    ValueMap row;
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return row;
    }

    //for qua cac phan tu roi bind vao stmp
    int placeholder_index = 1;
    for (int i = 0; i < query_vector.size(); i++) {
        cocos2d::Value::Type vType = query_vector.at(i).getType();
        if (vType == cocos2d::Value::Type::STRING) {
            sqlite3_bind_text(this->stmp, placeholder_index++, query_vector.at(i).asString().c_str(), -1, SQLITE_TRANSIENT);
        }
        else if (vType == cocos2d::Value::Type::INTEGER) {
            sqlite3_bind_int(this->stmp, placeholder_index++, query_vector.at(i).asInt());
        }
        else if (vType == cocos2d::Value::Type::FLOAT || vType == cocos2d::Value::Type::DOUBLE) {
            sqlite3_bind_double(this->stmp, placeholder_index++, query_vector.at(i).asDouble());
        }
    }

    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        row = fetchRow();
    }
    return row;
}

ValueMap StoryDataModel::fetchRow() {
    ValueMap row;
    int column_count = sqlite3_column_count(this->stmp);
    for (int i = 0; i < column_count; i++) {
        int column_type = sqlite3_column_type(this->stmp, i);
        string column_name = sqlite3_column_name(this->stmp, i);
        if (column_type == SQLITE_INTEGER) {
            row[column_name] = sqlite3_column_int(this->stmp, i);
        }
        else if (column_type == SQLITE_FLOAT) {
            row[column_name] = sqlite3_column_double(this->stmp, i);
        }
        else if (column_type == SQLITE_TEXT) {
            row[column_name] = (const char*)sqlite3_column_text(this->stmp, i);
        }
        else {
            row[column_name] = "";
        }
    }
    return row;
}

ValueVector StoryDataModel::fetchAllRecord(string query) {
    ValueVector arrayData;
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return arrayData;
    }
    ValueMap dict;
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        dict.clear();
        dict = fetchRow();
        arrayData.push_back(Value(dict));
    }
    return arrayData;
}

#pragma mark - STORY
//void StoryDataModel::insertStory(string plist) {
//    if(!FileUtils::getInstance()->isFileExist(plist)) {
//        return;
//    }
//    ValueVector vector = FileUtils::getInstance()->getValueVectorFromFile(plist);
//    if(vector.size() == 0) {
//        return;
//    }
//    
//    ValueVector _arrData;
//    for (int i = 0; i < (int) vector.size(); i ++) {
//        if (vector[i].asValueMap()["publish"].asInt() == 0) {
//            deleteStoryReject(vector[i].asValueMap()["id"].asInt());
//        }else{
//            _arrData.push_back(Value(vector[i].asValueMap()));
//            //CCLOG("database insert = %s", vector[i].asValueMap()["name"].asString().c_str());
//        }
//    }
//    
//    //int count = 1;
//    ValueVector rows = GlobalFunc::chunkValueVector(_arrData, 50);
//    for(auto g : rows) {
//        //CCLOG("database insert = %d", count++);
//        ValueVector list = g.asValueVector();
//        insertMultiStory(list);
//    }
//}

void StoryDataModel::insertStoryWithData(std::vector<mj::model::StoryInfo> stories) {
    if (stories.empty()) {
        return;
    }

    auto index = 0;
    while (index < stories.size()) {
        auto last_index = std::min(index + NUMBER_QUERY_ST_AU, (int)stories.size());
        insertMultiStory(std::vector<mj::model::StoryInfo>(stories.begin() + index, stories.begin() + last_index));
        insertMultiStoryTutoring(std::vector<mj::model::StoryInfo>(stories.begin() + index, stories.begin() + last_index));
        index = last_index;
    }
}

void StoryDataModel::insertMultiStory(std::vector<mj::model::StoryInfo> stories) {
    if (stories.size() <= 0)
        return;
    std::string query = "REPLACE INTO tbl_story(id,story_id,name,words,lexile,level,lang_id,total_page,image,written,design,search,quality,date_publish,level_max,version) VALUES ";
    for (auto i = 0; i < stories.size(); i++) {
        query += "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?),";
    }
    query.resize(query.length() - 1);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        logError();
        return;
    }

    int placeholder_index = 0;
    for (auto story : stories) {
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.unique_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.story_id);
        sqlite3_bind_text(this->stmp, ++placeholder_index, story.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.total_words);
        sqlite3_bind_text(this->stmp, ++placeholder_index, story.lexile.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mj::helper::vectorIntToString(story.levels).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.lang_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.total_page);
        sqlite3_bind_text(this->stmp, ++placeholder_index, story.thumbnail_url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, story.author.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, story.designer.c_str(), -1, SQLITE_TRANSIENT);
        //        auto no_mark = NativeBridge::hsStringToKhongDau(story.name, story.lang_id);
        //        if (no_mark.empty()) {
        //            CCLOG("Database: cannot init search field: %s", no_mark.c_str());
        //        } 
        sqlite3_bind_text(this->stmp, ++placeholder_index, mj::helper::to_no_accents_v2(story.name).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.quality);
        sqlite3_bind_int(this->stmp, ++placeholder_index, static_cast<int>(story.date_published));
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.levels.empty() ? 1 : story.levels[0]);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.version);
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }

    CCLOG("db mem used: %lld", sqlite3_memory_used());

    CCLOG("db mem used: %lld", sqlite3_memory_used());

    CCLOG("insertStory = done");
}

void StoryDataModel::insertMultiStoryTutoring(std::vector<mj::model::StoryInfo> stories) {
    if (stories.size() <= 0)
        return;

    mj::model::StoryInfo info;

    stories.erase(std::remove_if(stories.begin(), stories.end(), [=](mj::model::StoryInfo storyInfo) {
        return storyInfo.app_id == 40 || storyInfo.app_id == 0;
        }), stories.end());

    std::string query = "REPLACE INTO tbl_story_tutoring_new(id,app_id) VALUES ";
    for (auto i = 0; i < stories.size(); i++) {
        query += "(?,?),";
    }
    query.resize(query.length() - 1);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        logError();
        return;
    }

    int placeholder_index = 0;
    for (auto story : stories) {
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.unique_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.app_id);
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
}


std::vector<mj::model::StoryInfoTutoring> StoryDataModel::GetAllInfoTableToturing()
{
    std::vector<mj::model::StoryInfoTutoring> listInfo = ms::CacheManager::getInstance().GetCacheInfoToturing();

    if (listInfo.size() > 0)
    {
        return listInfo;
    }
    std::string query = "select * from tbl_story_tutoring_new";
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return {};
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfoTutoring info;
        info.storyId = SQLITE_COLUMN_INT(0);
        info.appid = SQLITE_COLUMN_INT(1);
        listInfo.push_back(info);
    }
#if KEY_MEMMORY
    if (listInfo.size() > 0)
    {
        ms::CacheManager::getInstance().SetCacheInfoToturing(listInfo);
    }
#endif

    return listInfo;
}

bool StoryDataModel::CheckStoryIsTutoring(int storyId)
{
    std::string query = "select * from tbl_story_tutoring_new where id = " + std::to_string(storyId);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        return false;
    }

    return sqlite3_step(this->stmp) == SQLITE_ROW;
}


void StoryDataModel::insertStoryWithData(cocos2d::ValueVector vector) {
    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertMultiStory(list);

        insertMultiStoryTutoring(list);
    }
}

void StoryDataModel::insertMultiStoryTutoring(ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_story(id,story_id,name,app_id) VALUES ";

    CCLOG("query = %s", query.c_str());
    int total_placeholder = 16;
    std::vector<string> listPlaceholder;
    string placeholder = "(?,?,?,?)";

    for (auto g : vector) {
        listPlaceholder.push_back(placeholder);
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        auto list_level = map["level"].asValueVector();
        auto level_max = 1;
        if ((int)list_level.size() > 0) {
            level_max = list_level[0].asInt();
        }

        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["id"].asInt());
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, map["story_id"].asInt());
        sqlite3_bind_text(this->stmp, 3 + placeholder_index, map["name"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["app_id"].asInt());
        placeholder_index += total_placeholder;

    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertStoryTutoring = done");
}

void StoryDataModel::insertMultiStory(ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_story(id,story_id,name,words,lexile,level,lang_id,total_page,image,written,design,search,quality,date_publish,level_max,version) VALUES ";

    CCLOG("query = %s", query.c_str());
    int total_placeholder = 16;
    std::vector<string> listPlaceholder;
    string placeholder = "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

    for (auto g : vector) {
        listPlaceholder.push_back(placeholder);
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        auto list_level = map["level"].asValueVector();
        auto level_max = 1;
        if ((int)list_level.size() > 0) {
            level_max = list_level[0].asInt();
        }

        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["id"].asInt());
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, map["story_id"].asInt());
        sqlite3_bind_text(this->stmp, 3 + placeholder_index, map["name"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["words"].asInt());
        sqlite3_bind_text(this->stmp, 5 + placeholder_index, map["lexile"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 6 + placeholder_index, JsonDump::getInstance()->VectorToString(list_level).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 7 + placeholder_index, map["lang_id"].asInt());
        sqlite3_bind_int(this->stmp, 8 + placeholder_index, map["total_page"].asInt());
        sqlite3_bind_text(this->stmp, 9 + placeholder_index, map["image"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 10 + placeholder_index, map["written"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 11 + placeholder_index, map["design"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 12 + placeholder_index, NativeBridge::hsStringToKhongDau(map["name"].asString(), map["lang_id"].asInt()).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 13 + placeholder_index, map["quality"].asInt());
        sqlite3_bind_int(this->stmp, 14 + placeholder_index, map["date_publish"].asInt());
        sqlite3_bind_int(this->stmp, 15 + placeholder_index, level_max);
        sqlite3_bind_int(this->stmp, 16 + placeholder_index, map["version_story"].asInt());
        placeholder_index += total_placeholder;

    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertStory = done");
}

void StoryDataModel::downloadSuccess(int _id, int story_id, int language_id) {
    ValueVector vector;
    ValueMap map_input;
    map_input["id"] = _id;
    map_input["story_id"] = story_id;
    map_input["language_id"] = language_id;
    map_input["downloaded"] = 1;
    vector.push_back(Value(map_input));

    ValueVector _arrData;
    for (int i = 0; i < (int)vector.size(); i++) {
        _arrData.push_back(Value(vector[i].asValueMap()));
    }

    ValueVector rows = GlobalFunc::chunkValueVector(_arrData, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        setDownloadMultiStory(list);
    }

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
}

void StoryDataModel::setDownloadMultiStory(ValueVector vector) {
    string query = "REPLACE INTO tbl_story_download(id,storyid,langid,downloaded) VALUES ";

    int total_placeholder = 4;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();

        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["id"].asInt());
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, map["story_id"].asInt());
        sqlite3_bind_int(this->stmp, 3 + placeholder_index, map["language_id"].asInt());
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["downloaded"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("database set downloaded = 1");
}

void StoryDataModel::deleteDownloadStory(int _id) {
    string query = "DELETE FROM tbl_story_download WHERE id = ?";
    ValueVector vector_query;
    vector_query.push_back(Value(_id));
    executeQuery(query, vector_query);
}

void StoryDataModel::deleteAllDownloadStory() {
    string query = "DELETE FROM tbl_story_download";
    ValueVector vector_query;
    executeQuery(query, vector_query);
}

bool StoryDataModel::getStoryIsDownloadedWith(int _id, int story_id, int lang_id) {
    int isDownloaded = 0;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT downloaded FROM tbl_story_download WHERE id = %d", _id);
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return isDownloaded;
    }
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        isDownloaded = sqlite3_column_int(this->stmp, 0);
    }
    CCLOG("story_downloaded = %d", isDownloaded);
    return isDownloaded;
}

std::vector<int> StoryDataModel::getAllStoryDownloaded() {
    std::vector<int> re_listStories;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT id FROM tbl_story_download WHERE downloaded = 1");
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return re_listStories;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto _id = sqlite3_column_int(this->stmp, 0);
        re_listStories.push_back(_id);
    }
    return re_listStories;
}

std::vector<int> StoryDataModel::getAllStoryIDDownloaded() {
    /*
    * Get ID of downloaded stories
    */
    std::vector<int> re_listStories;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT storyid FROM tbl_story_download WHERE downloaded = 1");
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return re_listStories;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto _id = sqlite3_column_int(this->stmp, 0);
        re_listStories.push_back(_id);
    }
    return re_listStories;
}


int StoryDataModel::searchID(int id, int start, int end, std::vector<std::pair<int, int>> storyList) {
    /*
    * Search for a downloaded story id in story id list
    */
    if (start > end || end < 0) {
        return -1;
    }

    int middle = (start + end) / 2;
    if (id == storyList[middle].second) {
        return middle;
    }
    else {
        if (id < storyList[middle].second) {
            return searchID(id, start, middle - 1, storyList);
        }
        else {
            return searchID(id, middle + 1, end, storyList);
        }
    }
}

std::vector<std::pair<int, int>> StoryDataModel::getDownloadStoryIDWithVersion() {
    std::string query = "select tbl_story.version, tbl_story_download.storyid from tbl_story inner join tbl_story_download on tbl_story.story_id = tbl_story_download.storyid";
    std::vector<std::pair<int, int>> res;
    res.clear();
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return res;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto version = sqlite3_column_int(this->stmp, 0);
        auto id = sqlite3_column_int(this->stmp, 1);
        res.push_back({ version, id });
    }

    return res;
}

void StoryDataModel::updateStoriesListByVersion(std::vector<std::pair<int, int>> list_id_version) {
    /*
    * If a downloaded story has smaller version than the version of that story at server
    * Force user to download that story again.
    * Update story's links download too.
    */
    if (list_id_version.size() <= 0)
        return;
    std::sort(list_id_version.begin(), list_id_version.end(), [](const std::pair<int, int> a, const std::pair<int, int> b) -> bool {
        return a.second < b.second;
        });

    std::vector<std::pair<int, int>> downloadStories = getDownloadStoryIDWithVersion();
    for (int i = 0; i < downloadStories.size(); i++) {
        int pos = searchID(downloadStories[i].second, 0, list_id_version.size() - 1, list_id_version);
        if (pos != -1) {
            if (downloadStories[i].first < list_id_version[pos].first) {
                downloadStories[i].first = -1;
            }
        }
    }

    for (int i = 0; i < downloadStories.size(); i++) {
        if (downloadStories[i].first == -1) {
            // The story with this id has a new update, make it as undownloaded.
            deleteDownloadStory(downloadStories[i].second);
        }
    }
    CCLOG("hmm");
}

ValueVector StoryDataModel::getListStoriesIdWithStoryId(int story_id) {
    ValueVector re_listStories;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT id FROM tbl_story WHERE story_id = %d", story_id);
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return re_listStories;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto _id = sqlite3_column_int(this->stmp, 0);
        re_listStories.push_back(Value(_id));
    }
    return re_listStories;
}

ValueVector StoryDataModel::getAllStory() {
    //quality DESC, read, date_publish DESC
    string query = "SELECT * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang ORDER BY quality DESC, read, date_publish DESC";
    return fetchAllRecord(query);
}

ValueVector StoryDataModel::getAllStories(int level_id, int lang_id, string order) {
    ValueVector re_listStories;
    string query = StringUtils::format("SELECT * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d", lang_id);
    if (order != "") {
        query = query + " ORDER BY " + order;
    }

    if (level_id > 0) {
        ValueMap map;
        this->stmp = nullptr;
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return re_listStories;
        }
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            map.clear();
            map = fetchRow();
            auto list_str = map["level"].asString();
            auto list_level = JsonDump::getInstance()->dumpValueVectorFromString(list_str);
            for (int i = 0; i < (int)list_level.size(); i++) {
                if (list_level[i].asInt() == level_id) {
                    re_listStories.push_back(Value(map));
                    break;
                }
            }
        }
        CCLOG("getAllStories with level = %d | DONE - %d", level_id, (int)re_listStories.size());
        return re_listStories;

    }
    else {

        re_listStories = fetchAllRecord(query);
        CCLOG("getAllStories with level = %d ORDER BY = %s| DONE - %d", level_id, order.c_str(), (int)re_listStories.size());
        return re_listStories;
    }
}

void StoryDataModel::setStoryRead(std::vector<std::pair<std::string, int>> listStoryList)
{
    if (listStoryList.size() <= 0)
        return;
    if (listStoryList.size() > 100) {
        auto first = 0;
        const auto chunk_size = 100;
        while (first < listStoryList.size()) {
            auto last = std::min(first + chunk_size, (int)listStoryList.size());
            setStoryRead(std::vector<std::pair<std::string, int>>(std::begin(listStoryList) + first, std::begin(listStoryList) + last));
            first = last;
        }

        return;
    }

    std::string query = "REPLACE INTO tbl_story_read" + std::string("(id_story_lang,read,time_read)") + " values ";
    query = std::accumulate(std::begin(listStoryList), std::end(listStoryList), query, [](std::string q, std::pair<std::string, int> i) {
        return q.append("(?,?,?) ,");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    int count = 0;

    for (auto stories : listStoryList)
    {
        if (stories.second <= 0 || stories.second >= (int)(utils::getTimeInMilliseconds()))
        {
            stories.second = (int)(utils::getTimeInMilliseconds());
        }

        SQLITE_BIND_INT(++count, std::stoi(stories.first));
        SQLITE_BIND_INT(++count, 1);
        SQLITE_BIND_INT(++count, stories.second);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }

}

void StoryDataModel::setStoryRead(int story_lang_id, int read_value, int time_read) {
    std::string query = "REPLACE INTO tbl_story_read (id_story_lang,read,time_read) VALUES (?,?,?)";
    this->stmp = NULL;
    ValueMap item;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    //Tranh cãi sau :))
//    if (time_read == 0) {
//        time_read = (int)time(nullptr);
//    }else time_read = 1505382278;

    if (time_read <= 0 || time_read >= (int)(utils::getTimeInMilliseconds()))
    {
        time_read = (int)(utils::getTimeInMilliseconds());
    }
    sqlite3_bind_int(this->stmp, 1, story_lang_id);
    sqlite3_bind_int(this->stmp, 2, read_value);
    sqlite3_bind_int(this->stmp, 3, time_read);
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("setStoryRead Done ID = %d", story_lang_id);
}

void StoryDataModel::resetStoryRead() {
    std::string query = "delete from tbl_story_read";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}


ValueVector StoryDataModel::getAllStoryRead() {
    string query = "SELECT * FROM tbl_story_read ORDER BY time_read DESC";
    return fetchAllRecord(query);
}

std::vector <int> StoryDataModel::getValueStoryReadById(int storyIdLang)
{
    std::vector <int> valueStoryRead = {};

    try
    {
        std::string  query = StringUtils::format("select * from tbl_story_read where id_story_lang=%d", storyIdLang);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            return valueStoryRead;
        };

        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            valueStoryRead.push_back(sqlite3_column_int(this->stmp, 0));
            valueStoryRead.push_back(sqlite3_column_int(this->stmp, 1));
            valueStoryRead.push_back(sqlite3_column_int(this->stmp, 2));
        }
    }
    catch (const std::exception&)
    {

    }

    return valueStoryRead;
}

void StoryDataModel::removeAllStoryRead() {
    string query = "DELETE FROM tbl_story_read";
    ValueVector vector_query;
    executeQuery(query, vector_query);
}


ValueMap StoryDataModel::getStoryWithId(int _id) {
    ValueMap map;
    string query = StringUtils::format("SELECT * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE id = %d", _id);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return map;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
    }
    CCLOG("getStory id = %d", _id);
    return map;
}

ValueVector StoryDataModel::getListIDStory(int level_id, int lang_id, string order) {
    ValueVector re_listStories;
    ValueMap map;
    if (level_id > 0) {
        string query = StringUtils::format("SELECT * FROM tbl_story WHERE lang_id = %d", lang_id);
        if (order != "") {
            query = query + " ORDER BY " + order;
        }
        CCLOG("query = %s", query.c_str());
        this->stmp = nullptr;
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return re_listStories;
        }
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            map.clear();
            map = fetchRow();
            auto list_str = map["level"].asString();
            auto list_level = JsonDump::getInstance()->dumpValueVectorFromString(list_str);
            for (int i = 0; i < (int)list_level.size(); i++) {
                if (list_level[i].asInt() == level_id) {
                    re_listStories.push_back(Value(map["id"].asInt()));
                    break;
                }
            }
        }
        CCLOG("getListID with level = %d | DONE - %d", level_id, (int)re_listStories.size());
        return re_listStories;
    }
    else {

        string query = StringUtils::format("SELECT * FROM tbl_story WHERE lang_id = %d", lang_id);
        if (order != "") {
            query = query + " ORDER BY " + order;
        }
        CCLOG("query = %s", query.c_str());
        this->stmp = nullptr;
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return re_listStories;
        }
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            map.clear();
            map = fetchRow();
            re_listStories.push_back(Value(map["id"].asInt()));
        }
        CCLOG("getListID all level DONE - %d", (int)re_listStories.size());
        return re_listStories;

    }
}

ValueVector StoryDataModel::getStoriesWithListID(ValueVector list_id) {
    ValueVector re_listStories;
    for (int i = 0; i < (int)list_id.size(); i++) {
        ValueMap map;
        string query = "SELECT * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE id = ?";
        this->stmp = nullptr;
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return re_listStories;
        }
        sqlite3_bind_int(this->stmp, 1, list_id[i].asInt());
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            map.clear();
            map = fetchRow();
        }
        re_listStories.push_back(Value(map));
    }
    CCLOG("getStoriesWithListID = DONE - %d", (int)re_listStories.size());
    return re_listStories;
}

ValueVector StoryDataModel::getStoriesRecentWithListID(ValueVector list_id) {
    CCLOG("RECENT STORIES");
    ValueVector re_listStories;
    for (int i = 0; i < (int)list_id.size(); i++) {
        ValueMap map;
        string query = "SELECT * FROM tbl_story WHERE id = ?";
        this->stmp = nullptr;
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return re_listStories;
        }
        sqlite3_bind_int(this->stmp, 1, list_id[i].asInt());
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            map.clear();
            map = fetchRow();
        }
        CCLOG("story_id = %d", map["story_id"].asInt());

        if (!map.empty() && !map["story_id"].isNull() && map["story_id"].asInt() > 0) {
            re_listStories.push_back(Value(map));
        }
    }
    CCLOG("RECENT STORIES - DONE - %d", (int)re_listStories.size());
    return re_listStories;
}

#pragma mark - STORY ACTIVITIES
#include "../activity/GameList.h"
void StoryDataModel::insertStoriesActivities(std::vector<mj::model::ActInfo> activities) {
    if (activities.size() <= 0)
        return;
    std::string query = "replace into tbl_story_activities " + mj::model::ActInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(activities), std::end(activities), query, [](std::string q, mj::model::ActInfo i) {
        return q.append(mj::model::ActInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    auto counter = 0;
    for (auto activity : activities) {
        //(_id, _order, lesson_id, story_id, type, name, content, resource_zip, data_zip)
        SQLITE_BIND_INT(++counter, activity.activity_id);
        SQLITE_BIND_INT(++counter, activity.order);
        SQLITE_BIND_INT(++counter, activity.lesson_id);
        SQLITE_BIND_INT(++counter, activity.sub_id);
        SQLITE_BIND_INT(++counter, (int)activity.type);
        SQLITE_BIND_STR(++counter, activity.name);
        SQLITE_BIND_STR(++counter, activity.content);
        SQLITE_BIND_STR(++counter, activity.resource_zip);
        SQLITE_BIND_STR(++counter, activity.data_zip);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert activities done");
}

mj::model::ActInfo StoryDataModel::getActivityOfStory(int activity_id) {
    auto query = StringUtils::format("select * from tbl_story_activities where _id = %d", activity_id);
    mj::model::ActInfo r;
    r.activity_id = -1;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }
    int page = 0;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::ActInfo info;
        info.activity_id = SQLITE_COLUMN_INT(0);
        info.order = SQLITE_COLUMN_INT(1);
        info.lesson_id = SQLITE_COLUMN_INT(2);
        page = SQLITE_COLUMN_INT(3);
        info.sub_id = SQLITE_COLUMN_INT(4);
        info.type = (mj::model::ActInfo::Type)SQLITE_COLUMN_INT(5);
        info.name = SQLITE_COLUMN_STR(6);
        info.content = SQLITE_COLUMN_STR(7);
        info.resource_zip = SQLITE_COLUMN_STR(8);
        //info.resource_zip = mj::helper::getUrlFromDomain(r.resource_zip, mj::UrlType::MSURL_STORY);
        info.data_zip = SQLITE_COLUMN_STR(9);
        if (info.type == mj::model::ActInfo::Type::STORY)
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_STORY);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_STORY);
        }
        else
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES);
        }
        r = info;
    }

    if (!r.data_zip.empty()) r.zip_urls.push_back(r.data_zip);
    if (!r.resource_zip.empty()) r.zip_urls.push_back(r.resource_zip);

    if (r.type == mj::model::ActInfo::Type::GAME) {
        auto game_info = StoryDataModel::getInstance()->getGameById(r.sub_id);
        if (!game_info.resource_zip.empty()) r.zip_urls.push_back(r.resource_zip);
    }

    return r;
}

std::vector<mj::model::ActInfo> StoryDataModel::getActivitiesOfStory(int story_id, std::pair<int, std::string> valueError, int index, std::vector<std::string> domainStory, std::vector<std::string> domainAudioBook, std::string typeError) {
    auto query = StringUtils::format("select * from tbl_story_activities where lesson_id = %d order by _order asc", story_id);

    if (index == -1 && domainStory.empty() && domainAudioBook.empty())
    {
        auto idAndListLesson = ms::CacheManager::getInstance().getCurrentLessonAndListActivityOfStory(story_id);

        if (idAndListLesson.first != -1)
        {
            return idAndListLesson.second;
        }
    }

    std::vector<mj::model::ActInfo> r;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }
    int page = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        mj::model::ActInfo info;
        info.activity_id = SQLITE_COLUMN_INT_STMT(stmt, 0);
        info.order = SQLITE_COLUMN_INT_STMT(stmt, 1);
        info.lesson_id = SQLITE_COLUMN_INT_STMT(stmt, 2);
        page = SQLITE_COLUMN_INT_STMT(stmt, 3);
        info.sub_id = SQLITE_COLUMN_INT_STMT(stmt, 4);
        info.type = (mj::model::ActInfo::Type)SQLITE_COLUMN_INT_STMT(stmt, 5);
        info.name = SQLITE_COLUMN_STR_STMT(stmt, 6);
        info.content = SQLITE_COLUMN_STR_STMT(stmt, 7);
        info.resource_zip = SQLITE_COLUMN_STR_STMT(stmt, 8);
        info.data_zip = SQLITE_COLUMN_STR_STMT(stmt, 9);
        if (info.type == mj::model::ActInfo::Type::STORY)
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_STORY);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_STORY);
        }
        else
        {
            info.resource_zip = getUrlOfDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES, index, domainAudioBook, typeError, valueError);
            info.data_zip = getUrlOfDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES, index, domainAudioBook, typeError, valueError);
        }
        if (info.type == mj::model::ActInfo::Type::STORY) {
            auto story_id = info.sub_id;
            auto story = getStoryById(story_id);
            if (story.story_id == 0) {
                continue;
            }
        }

        //TODO: an game
        if (info.type == mj::model::ActInfo::Type::GAME) {
            auto game_id = info.sub_id;
        }


        r.push_back(info);
    }

    sqlite3_finalize(stmt);

    for (auto& activity : r) {
        if (!activity.data_zip.empty()) activity.zip_urls.push_back(activity.data_zip);
        if (!activity.resource_zip.empty()) activity.zip_urls.push_back(activity.resource_zip);

        if (activity.type == mj::model::ActInfo::Type::GAME) {


            if (valueError.second.find(activity.sub_id) == std::string::npos)
            {
                domainAudioBook = {};
            }

            mj::model::GameInfo game_info = StoryDataModel::getInstance()->getGameById(activity.sub_id, valueError, index, domainAudioBook);
            
            if (!game_info.resource_zip.empty()) activity.zip_urls.push_back(game_info.resource_zip);
        }
    }

    auto activities = r;
    std::vector<mj::model::ActInfo> activities_convert;
    cocos2d::ValueMap map_merge_quiz;
    int convert_index = 0;
    for (int i = 0; i < (int)activities.size(); i++) {
        //        CCLOG("id = %d | name = %s", activities[i].sub_id, activities[i].name.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        if (activities[i].sub_id == 66) continue;//game phonics sử dụng video và thu âm.
#endif
        int number_turn_play = 1;
        auto name_activity = activities[i].name;
        name_activity = name_activity.substr(0, name_activity.find_first_of(":"));

        auto list_splits = mj::helper::splitString(name_activity, '_');
        if ((int)list_splits.size() >= 4) {
            auto quiz_name = list_splits[0] + "_" + list_splits[1];
            auto quiz_id = atoi(list_splits[2].c_str());
            number_turn_play = atoi(list_splits[3].c_str());
            if (map_merge_quiz[quiz_name].asInt() == 0) {
                auto quiz_info = activities[i];
                quiz_info.quizz_id.push_back(activities[i].activity_id);
                quiz_info.list_num_turnplay.push_back(number_turn_play);
                activities_convert.push_back(quiz_info);
                convert_index = (int)activities_convert.size() - 1;
            }
            else {
                activities_convert[convert_index].quizz_id.push_back(activities[i].activity_id);
                activities_convert[convert_index].list_num_turnplay.push_back(number_turn_play);
                activities_convert[convert_index].zip_urls.insert(activities_convert[convert_index].zip_urls.end(), activities[i].zip_urls.begin(), activities[i].zip_urls.end());
            }
            map_merge_quiz[quiz_name] = quiz_id;
            //            CCLOG("hoisunglog - sub: %s - %d - %d", name_activity.c_str(), quiz_id, number_turn_play);
        }
        else {
            activities[i].quizz_id.push_back(activities[i].activity_id);
            activities[i].list_num_turnplay.push_back(number_turn_play);
            activities_convert.push_back(activities[i]);
        }
    }

    for (auto i = 0; i < activities_convert.size(); ++i) {
        activities_convert[i].order = i;
    }
    
    auto listGame = getListGameError();

    if (!listGame.empty())
    {
        for (int i = 0; i < listGame.size(); i++)
        {
            activities_convert.erase(std::remove_if(activities_convert.begin(), activities_convert.end(), [=](mj::model::ActInfo act) {
                return act.sub_id == listGame[i];
                }), activities_convert.end());
        }
    }

    if (index == -1 && domainStory.empty() && domainAudioBook.empty())
    {
        ms::CacheManager::getInstance().setCurrentLessonAndListActivityOfStory({ story_id, activities_convert });
    }

    return activities_convert;
}

#pragma mark - DELETE_TABLE
void StoryDataModel::deleteStoryReject(int _id) {
    string query = "DELETE FROM tbl_userstory WHERE id = ?";
    ValueVector vector_query;
    vector_query.push_back(Value(_id));
    executeQuery(query, vector_query);
}

void StoryDataModel::deleteAllFrom(string table, ValueMap data) {
    string query = StringUtils::format("DELETE FROM %s", table.c_str());
    string str_data;
    ValueVector vector_query;
    if ((int)data.size() > 0) {
        query += string(" WHERE ");
        for (auto item : data) {
            str_data += StringUtils::format("%s = ? AND ", item.first.c_str());
            vector_query.push_back(Value(item.second));
        }
        str_data.replace(str_data.end() - 5, str_data.end(), "");
    }

    query += str_data;
    //    CCLOG("deleteAllFrom:\n%s", query.c_str());
    executeQuery(query, vector_query);
}

#pragma mark - CHARACTER
void StoryDataModel::updateCharacters(std::vector<mj::model::CharacterInfo> info) {
    if (info.size() <= 0)
        return;
    std::string query = "delete from tbl_character";
    auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }

    query = "replace into tbl_character " + mj::model::CharacterInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(info), std::end(info), query, [](std::string q, mj::model::CharacterInfo inf) {
        return q + inf.getSQLValues() + ",";
        });
    query.resize(query.length() - 1);

    r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }
}

void StoryDataModel::updateCharacterWithData(cocos2d::ValueVector vector) {
    string query = "DELETE FROM tbl_character";
    ValueVector vector_query;
    executeQuery(query, vector_query);

    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertCharactor(list);
    }
}

void StoryDataModel::insertCharactor(ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_character(character_id,content,image,_order,lang_id) VALUES ";
    int total_placeholder = 5;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["character_id"].asInt());
        sqlite3_bind_text(this->stmp, 2 + placeholder_index, map["content"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 3 + placeholder_index, map["image"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["_order"].asInt());
        sqlite3_bind_int(this->stmp, 5 + placeholder_index, map["lang_id"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertCharactor = done");
}

void StoryDataModel::updateCharacterStories(int character_id, ValueVector vector) {
    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertMultiStory(list);
        insertMultiStoryTutoring(list);
        insertCharacterStories(character_id, list);
    }
}

void StoryDataModel::insertCharacterStories(int character_id, ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_character_stories(join_id,character_id,id) VALUES ";
    int total_placeholder = 3;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        auto key1 = StringUtils::format("%d_%d", character_id, map["id"].asInt());
        sqlite3_bind_text(this->stmp, 1 + placeholder_index, key1.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, character_id);
        sqlite3_bind_int(this->stmp, 3 + placeholder_index, map["id"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertCharacterStories = done");
}

ValueVector StoryDataModel::getCharacter(int langid) {
    ValueVector re_list;
    ValueMap map;
    string query = StringUtils::format("SELECT * FROM tbl_character WHERE lang_id = %d", langid);
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getCharacter = DONE - %d", (int)re_list.size());
    return re_list;
}

ValueVector StoryDataModel::getCharacterStories(int charater_id) {
    ValueVector re_list;
    ValueMap map;
    string query = StringUtils::format("SELECT * FROM tbl_story INNER JOIN tbl_character_stories ON tbl_story.id = tbl_character_stories.id AND tbl_character_stories.character_id = %d", charater_id);
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getCharacterStories = DONE - %d", (int)re_list.size());
    return re_list;
}
#pragma mark - CATEGORY
void StoryDataModel::updateCategories(std::vector<mj::model::CategoryInfo> info) {
    if (info.size() <= 0)
        return;
    std::string query = "delete from tbl_category";
    auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }

    query = "replace into tbl_category " + mj::model::CategoryInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(info), std::end(info), query, [](std::string q, mj::model::CategoryInfo inf) {
        return q + inf.getSQLValues() + ",";
        });
    query.resize(query.length() - 1);

    r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }
}

std::vector<mj::model::CategoryInfo> StoryDataModel::getCategories(int lang_id) {
    std::vector<mj::model::CategoryInfo> r;

    std::string query = StringUtils::format("select * from tbl_category where lang_id = %d", lang_id);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::CategoryInfo info;
        info.category_id = sqlite3_column_int(this->stmp, 0);
        info.content = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1)));
        info.image_url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.image_url = mj::helper::getUrlFromDomain(info.image_url, mj::UrlType::MSURL_STORY);
        info.order = sqlite3_column_int(this->stmp, 3);
        info.lang_id = sqlite3_column_int(this->stmp, 4);

        r.push_back(info);
    }

    return r;

    //    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    //    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
    //        mj::model::StoryInfo info;
    //
    //        info.unique_id = sqlite3_column_int(this->stmp, 0);
    //        info.story_id = sqlite3_column_int(this->stmp, 1);
    //        info.name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, 2)));
    //        info.total_words = sqlite3_column_int(this->stmp, 3);
    //        info.lexile = reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, 4));
    //        info.lang_id = sqlite3_column_int(this->stmp, 6);
    //        info.total_page = sqlite3_column_int(this->stmp, 7);
    //        info.thumbnail_url = reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, 8));
    //        info.author = reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, 9));
    //        info.designer = reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, 10));
    //        info.quality = sqlite3_column_int(this->stmp, 12);
    //        info.date_published = sqlite3_column_int(this->stmp, 13);
    //        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
    //        info.version = sqlite3_column_int(this->stmp, 15);
    //        info.is_read = sqlite3_column_int(this->stmp, 16);
    //
    //        r.push_back(info);
    //    }
    //
    //    return r;

    return r;
}

void StoryDataModel::updateCategoryWithData(cocos2d::ValueVector vector) {
    string query = "DELETE FROM tbl_category";
    ValueVector vector_query;
    executeQuery(query, vector_query);

    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertCategory(list);
    }
}

void StoryDataModel::insertCategory(ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_category(category_id,content,image,_order,lang_id) VALUES ";
    int total_placeholder = 5;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["category_id"].asInt());
        sqlite3_bind_text(this->stmp, 2 + placeholder_index, map["content"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 3 + placeholder_index, map["image"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["_order"].asInt());
        sqlite3_bind_int(this->stmp, 5 + placeholder_index, map["lang_id"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertCategory = done");
}

void StoryDataModel::updateCategoryStories(int category_id, std::vector<mj::model::StoryInfo> stories) {
    if (stories.empty()) {
        return;
    }

    auto index = 0;
    while (index < stories.size()) {
        auto last_index = index + 50;
        last_index = std::min(last_index, (int)stories.size());
        auto chunk = std::vector<mj::model::StoryInfo>(stories.begin() + index, stories.begin() + last_index);
        insertCategoryStories(category_id, chunk);
        index = last_index;
    }
}

void StoryDataModel::insertCategoryStories(int category_id, std::vector<mj::model::StoryInfo> stories) {
    std::string query = "REPLACE INTO tbl_category_stories(join_id, category_id, id) VALUES ";
    for (auto i = 0; i < stories.size(); i++) {
        query += "(?,?,?),";
    }
    query.resize(query.length() - 1);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    int placeholder_index = 0;
    for (auto story : stories) {
        sqlite3_bind_text(this->stmp, ++placeholder_index, StringUtils::format("%d_%d", category_id, story.unique_id).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++placeholder_index, category_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, story.unique_id);
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertCategoryStories = done");
}

void StoryDataModel::updateCategoryStories(int category_id, ValueVector vector) {
    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertMultiStory(list);
        insertMultiStoryTutoring(list);
        insertCategoryStories(category_id, list);
    }
}

void StoryDataModel::insertCategoryStories(int category_id, ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_category_stories(join_id,category_id,id) VALUES ";
    int total_placeholder = 3;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        auto key1 = StringUtils::format("%d_%d", category_id, map["id"].asInt());
        sqlite3_bind_text(this->stmp, 1 + placeholder_index, key1.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, category_id);
        sqlite3_bind_int(this->stmp, 3 + placeholder_index, map["id"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertCategoryStories = done");
}

ValueVector StoryDataModel::getCategory(int langid) {
    ValueVector re_list;
    ValueMap map;
    string query = StringUtils::format("SELECT * FROM tbl_category WHERE lang_id = %d", langid);
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getCategory = DONE - %d", (int)re_list.size());
    return re_list;
}

cocos2d::ValueMap StoryDataModel::getCategoryByID(int catagoryid)
{
    std::unordered_map<std::string, Value> re_list;

    string query = StringUtils::format("SELECT * FROM tbl_category WHERE category_id = %d", catagoryid);

    this->stmp = nullptr;

    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK)
    {
        this->showError(result);
        return re_list;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW)
    {
        int column_count = sqlite3_column_count(this->stmp);
        for (int i = 0; i < column_count; i++)
        {
            int column_type = sqlite3_column_type(this->stmp, i);
            string columnName = sqlite3_column_name(this->stmp, i);
            cocos2d::Value columValue;
            if (column_type == SQLITE_INTEGER) {
                columValue = cocos2d::Value(sqlite3_column_int(this->stmp, i));
            }
            else if (column_type == SQLITE_FLOAT)
            {
                columValue = cocos2d::Value(sqlite3_column_double(this->stmp, i));
            }
            else if (column_type == SQLITE_TEXT)
            {
                columValue = cocos2d::Value((const char*)sqlite3_column_text(this->stmp, i));
            }
            else
            {
                columValue = cocos2d::Value("");
            }

            re_list.insert({ columnName,columValue });
        }
    }

    CCLOG("getCategory = DONE - %d", (int)re_list.size());

    return re_list;
}

ValueVector StoryDataModel::getCategoryStories(int category_id) {
    ValueVector re_list;
    ValueMap map;
    string query = StringUtils::format("SELECT * FROM tbl_story INNER JOIN tbl_category_stories ON tbl_story.id = tbl_category_stories.id AND tbl_category_stories.category_id = %d", category_id);
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getCategoryStories = DONE - %d", (int)re_list.size());
    return re_list;
}

#pragma mark - FEATURE
void StoryDataModel::updateFeatureWithData(cocos2d::ValueVector vector) {
    string query = "DELETE FROM tbl_story_feature";
    ValueVector vector_query;
    executeQuery(query, vector_query);

    if (vector.size() == 0) {
        return;
    }
    ValueVector rows = GlobalFunc::chunkValueVector(vector, 50);
    for (auto g : rows) {
        ValueVector list = g.asValueVector();
        insertFeature(list);
        insertMultiStory(list);
        insertMultiStoryTutoring(list);
    }
}
void StoryDataModel::insertFeature(ValueVector vector) {
    string query;
    query = "REPLACE INTO tbl_story_feature(id,story_id,name,words,lexile,level,lang_id,total_page,image,written,design,search,_order,image_feature,quality) VALUES ";
    int total_placeholder = 15;
    std::vector<string> listPlaceholder;
    for (auto g : vector) {
        listPlaceholder.push_back("(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    }
    query.append(GlobalFunc::joinStringVector(listPlaceholder, ","));

    int result = 0;
    this->stmp = nullptr;
    result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        CCLOG("insertFeature = Error");
        return;
    }
    int placeholder_index = 0;
    for (auto g : vector) {
        ValueMap map = g.asValueMap();
        sqlite3_bind_int(this->stmp, 1 + placeholder_index, map["id"].asInt());
        sqlite3_bind_int(this->stmp, 2 + placeholder_index, map["story_id"].asInt());
        sqlite3_bind_text(this->stmp, 3 + placeholder_index, map["name"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 4 + placeholder_index, map["mostpopular"].asInt());
        sqlite3_bind_int(this->stmp, 5 + placeholder_index, map["newest"].asInt());
        sqlite3_bind_text(this->stmp, 6 + placeholder_index, JsonDump::getInstance()->VectorToString(map["level"].asValueVector()).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 7 + placeholder_index, map["lang_id"].asInt());
        sqlite3_bind_int(this->stmp, 8 + placeholder_index, map["total_page"].asInt());
        sqlite3_bind_text(this->stmp, 9 + placeholder_index, map["image"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 10 + placeholder_index, map["written"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 11 + placeholder_index, map["design"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 12 + placeholder_index, NativeBridge::hsStringToKhongDau(map["name"].asString(), map["lang_id"].asInt()).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 13 + placeholder_index, map["_order"].asInt());
        sqlite3_bind_text(this->stmp, 14 + placeholder_index, map["image_feature"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 15 + placeholder_index, map["quality"].asInt());

        placeholder_index += total_placeholder;
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("insertFeature = done");
}

std::vector<std::pair<int, std::string>> StoryDataModel::getFeaturedStoriesByLanguage(int lang_id) {
    std::vector<std::pair<int, std::string>> r;
    std::string query = StringUtils::format("select id, image_feature from tbl_story_feature");

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto story_id = sqlite3_column_int(stmt, 0);
        auto url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        auto story = getStoryById(story_id);
        if (story.story_id == 0) {
            continue;
        }
        r.push_back(std::make_pair(story_id, url));
    }

    sqlite3_finalize(stmt);

    return r;
}

ValueVector StoryDataModel::getFeature(int langid) {
    ValueVector re_list;
    ValueMap map;
    string query = StringUtils::format("select tbl_story.*, tbl_story_feature.image_feature from tbl_story inner join tbl_story_feature on tbl_story.id = tbl_story_feature.id where tbl_story.lang_id = %d", langid);
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getFeature = DONE - %d", (int)re_list.size());
    return re_list;
}

#pragma mark - SEARCH
ValueVector StoryDataModel::getSearchStories(int lang_id, string text_search) {
    text_search = NativeBridge::hsStringToKhongDau(text_search);
    ValueVector re_list;
    string query = StringUtils::format("SELECT * FROM tbl_story WHERE lang_id = %d AND search LIKE '%%%s%%'", lang_id, text_search.c_str());
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return re_list;
    }
    ValueMap map;
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        map.clear();
        map = fetchRow();
        re_list.push_back(Value(map));
    }

    CCLOG("getSearchStories = DONE - %d", (int)re_list.size());
    return re_list;
}

#pragma mark - LIST_MESSAGE
void StoryDataModel::addMessageFromServer(std::vector<mj::model::NotificationInfo> notifications)
{
    if (notifications.size() <= 0)
        return;
    std::string query = "DELETE FROM message";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        logError();
        return;
    }

    //(id,title_en,title_vi,title_th,content_en,content_vi,content_th,url,dateinsert)";

    query = "insert into message " + mj::model::NotificationInfo::getSQLStructure() + " values ";

    query = std::accumulate(std::begin(notifications), std::end(notifications), query, [](std::string q, mj::model::NotificationInfo notification)
        {
            return q + notification.getSQLPlaceholders() + ",";
        });

    query.resize(query.length() - 1);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;

    for (auto notification : notifications)
    {
        sqlite3_bind_int(stmt, ++counter, notification.notification_id);
        sqlite3_bind_text(stmt, ++counter, notification.m_titles.count("en") ? notification.m_titles["en"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.m_titles.count("vi") ? notification.m_titles["vi"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.m_titles.count("th") ? notification.m_titles["th"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.m_messages.count("en") ? notification.m_messages["en"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.m_messages.count("vi") ? notification.m_messages["vi"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.m_messages.count("th") ? notification.m_messages["th"].c_str() : "", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, ++counter, notification.url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, ++counter, notification.time_created);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        logError();
        return;
    }
    sqlite3_finalize(stmt);

    CCLOG("added new notification.");
}

mj::model::NotificationInfo StoryDataModel::getNotificationById(int idx)
{
    mj::model::NotificationInfo r;

    auto query = std::string("select * from message where id = ") + StringUtils::toString(idx);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK)
    {
        logError();
        return r;
    };

    if (sqlite3_step(this->stmp) == SQLITE_ROW)
    {
        r.notification_id = SQLITE_COLUMN_INT(0);
        r.m_titles.insert({ "en",SQLITE_COLUMN_STR(1) });
        r.m_titles.insert({ "vi",SQLITE_COLUMN_STR(2) });
        r.m_titles.insert({ "th",SQLITE_COLUMN_STR(3) });

        r.m_messages.insert({ "en",SQLITE_COLUMN_STR(4) });
        r.m_messages.insert({ "vi",SQLITE_COLUMN_STR(5) });
        r.m_messages.insert({ "th",SQLITE_COLUMN_STR(6) });

        r.url = SQLITE_COLUMN_STR(7);
        r.time_created = SQLITE_COLUMN_INT(8);
    }

    //    sqlite3_finalize(this->stmp);
    return r;
}

ValueVector StoryDataModel::getAllMessage() {
    string query = "SELECT * FROM message";
    return fetchAllRecord(query);
}

#pragma mark - LEVEL_INFO


void StoryDataModel::insertLevelInfo(std::vector<mj::model::LevelInfo> levels) {
    if (levels.size() <= 0)
        return;
    auto query = std::string("delete from tbl_reading_levels");
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    query = "replace into tbl_reading_levels " + mj::model::LevelInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(levels), std::end(levels), query, [](std::string q, mj::model::LevelInfo level) {
        return q + mj::model::LevelInfo::getSQLPlaceholders() + ",";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    int counter = 0;
    for (auto level : levels) {
        //            return "(level_id,lang_id,grade_id,name,description)";
        sqlite3_bind_int(this->stmp, ++counter, level.level_id);
        sqlite3_bind_int(this->stmp, ++counter, level.lang_id);
        sqlite3_bind_int(this->stmp, ++counter, level.grade_id);
        sqlite3_bind_text(this->stmp, ++counter, level.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, level.description.c_str(), -1, SQLITE_TRANSIENT);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }

    CCLOG("Updated level info");
}

void StoryDataModel::insertLevelInfo(ValueVector listLevelInfo, int lang_id) {
    //delete OLD level_info
    string query = StringUtils::format("DELETE FROM levelinfo WHERE lang_id = %d", lang_id);
    ValueVector vector_query;
    executeQuery(query, vector_query);

    query = "REPLACE INTO levelinfo (level_id,id,name,des,lang_id) VALUES (?,?,?,?,?)";

    for (auto g : listLevelInfo) {
        this->stmp = NULL;
        ValueMap item = g.asValueMap();
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return;
        }
        sqlite3_bind_text(this->stmp, 1, StringUtils::format("%d%d", lang_id, item["id"].asInt()).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 2, item["id"].asInt());
        sqlite3_bind_text(this->stmp, 3, item["name"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 4, item["des"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, 5, lang_id);
        if (sqlite3_step(this->stmp) != SQLITE_DONE) {
            this->showError(result);
            return;
        }
    }
    CCLOG("Insert LevelInfo Done Lang = %d", lang_id);
}

std::vector<mj::model::GradeInfo> StoryDataModel::getAllGradesInfo(int lang_id) {
    std::vector<mj::model::GradeInfo> grades;
    std::string query = StringUtils::format("select * from tbl_grades where lang_id = %d", lang_id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::GradeInfo grade;
        grade.grade_id = sqlite3_column_int(this->stmp, 0);
        grade.lang_id = sqlite3_column_int(this->stmp, 1);
        grade.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2));
        grade.description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        grades.push_back(grade);
    }

    return grades;
}

std::vector<mj::model::LevelInfo> StoryDataModel::getAllLevelsInfo(int lang_id) {
    std::vector<mj::model::LevelInfo> levels;
    std::string query = StringUtils::format("select * from tbl_reading_levels where lang_id = %d", lang_id);

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::LevelInfo level;

        level.level_id = sqlite3_column_int(this->stmp, 0);
        level.lang_id = sqlite3_column_int(this->stmp, 1);
        level.grade_id = sqlite3_column_int(this->stmp, 2);
        level.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        level.description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        levels.push_back(level);
    }

    return levels;
}

ValueVector StoryDataModel::getAllLevelInfo(int lang_id) {
    string query = StringUtils::format("SELECT * FROM levelinfo WHERE lang_id = %d", lang_id);
    return fetchAllRecord(query);
}

void StoryDataModel::addStorySentDownloadSuccess(int _id, int story_id, int type) {
    string query = "REPLACE INTO tbl_story_sentapi (id,story_id,type) VALUES (?,?,?)";

    this->stmp = NULL;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    sqlite3_bind_int(this->stmp, 1, _id);
    sqlite3_bind_int(this->stmp, 2, story_id);
    sqlite3_bind_int(this->stmp, 3, type);
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("Insert addStorySentDownloadSuccess Done");
}

bool StoryDataModel::checkStoryIsSentDownloadSuccess(int _id) {
    int story_id = 0;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT story_id FROM tbl_story_sentapi WHERE id = %d", _id);
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return story_id;
    }
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        story_id = sqlite3_column_int(this->stmp, 0);
    }
    if (story_id > 0) {
        story_id = 1;
    }
    CCLOG("story_sent = %d", story_id);
    return story_id;
}

#pragma mark - LANGUAGE_DISPLAY

void StoryDataModel::addLanguageDisplay(ValueVector list_lang_display) {
    //delete OLD level_info
    string query = "DELETE FROM language_display";
    ValueVector vector_query;
    executeQuery(query, vector_query);

    query = "REPLACE INTO language_display (code,name,icon) VALUES (?,?,?)";

    for (auto g : list_lang_display) {
        this->stmp = NULL;
        ValueMap item = g.asValueMap();
        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        if (result != SQLITE_OK) {
            this->showError(result);
            return;
        }

        sqlite3_bind_text(this->stmp, 1, item["code"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 2, item["name"].asString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, 3, item["icon"].asString().c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(this->stmp) != SQLITE_DONE) {
            this->showError(result);
            return;
        }
    }
    CCLOG("Insert language_display Done");
}
ValueVector StoryDataModel::getListLanguageDisplay() {
    string query = "SELECT * FROM language_display";
    return fetchAllRecord(query);
}

void StoryDataModel::deleteStories(std::vector<int> ids) {
    if (ids.empty()) {
        return;
    }
    std::string query = "DELETE FROM tbl_story WHERE id IN (";
    query = std::accumulate(ids.begin(), ids.end(), query, [](std::string q, int id) {
        return q += StringUtils::toString(id) + ",";
        });
    query.resize(query.length() - 1);
    query += ")";

    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }

    CCLOG("Deleted multi stories");
}

std::vector<int> StoryDataModel::getListLevelsByLang(int lang_id) {
    std::vector<int> levels;
    std::string query = StringUtils::format("select level_id from (tbl_story_level inner join tbl_story on tbl_story_level.story_id = tbl_story.id and tbl_story.lang_id = %d) group by level_id", lang_id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        levels.push_back(sqlite3_column_int(this->stmp, 0));
    }

    return levels;
}

void StoryDataModel::insertStoryRelation(std::string relation_name, const std::vector<std::pair<int, int>>& list_data) {
    if (list_data.empty()) {
        return;
    }

    auto index = 0;
    while (index < list_data.size()) {
        auto last_index = index + 50;
        last_index = std::min(last_index, (int)list_data.size());
        auto chunk = std::vector<std::pair<int, int>>(list_data.begin() + index, list_data.begin() + last_index);
        insertStoryRelationChunk(relation_name, chunk);
        index = last_index;
    }
}

void StoryDataModel::insertStoryRelationChunk(std::string relation_name, const std::vector<std::pair<int, int> >& list_data) {
    std::vector<int> ids;
    for (auto pair : list_data) {
        ids.push_back(pair.first);
    }
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(std::begin(ids), std::end(ids)), std::end(ids));

    auto table_name = "tbl_" + relation_name;
    auto second_column_name = "level_id";
    if (relation_name == "story_character") {
        second_column_name = "character_id";
    }
    if (relation_name == "story_category") {
        second_column_name = "category_id";
    }

    std::string query = "delete from " + table_name + " where story_id in (";
    query = std::accumulate(ids.begin(), ids.end(), query, [](std::string q, int id) {
        return q += StringUtils::toString(id) + ",";
        });
    query.resize(query.length() - 1);
    query += ")";
    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }

    query = "insert into " + table_name + " (story_id, " + second_column_name + ") values ";
    query = std::accumulate(std::begin(list_data), std::end(list_data), query, [](std::string q, std::pair<int, int> r) {
        return q += StringUtils::format("(%d,%d),", r.first, r.second);
        });
    query.resize(query.length() - 1);
    rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        showError(rc);
        return;
    }

    CCLOG("Updated %s", table_name.c_str());
}

void StoryDataModel::insertStoryLevelRelation(const std::vector<std::pair<int, int>>& list_data) {
    this->insertStoryRelation("story_level", list_data);
}

void StoryDataModel::insertStoryTopicRelation(const std::vector<std::pair<int, int>>& list_data) {
    this->insertStoryRelation("story_category", list_data);
}

void StoryDataModel::insertStoryCharaRelation(const std::vector<std::pair<int, int>>& list_data) {
    this->insertStoryRelation("story_character", list_data);
}

std::vector<mj::model::StoryInfo> StoryDataModel::getStoriesByCategory(int category_id, std::string text, int grade_id) {
    // tạo chuỗi query tìm kiếm
    text = mj::helper::to_no_accents_v2(text);

    bool is_IOS = false;
    bool has_qMark = false;
    size_t quotationMarkFound;

    if (text.find("’", 0) != std::string::npos)
    {
        is_IOS = true;
        has_qMark = true;
    }

    if (!is_IOS)
    {
        quotationMarkFound = text.find("'", 0);
        if (quotationMarkFound != std::string::npos)
            has_qMark = true;

        while (quotationMarkFound != std::string::npos)
        {
            text.replace(quotationMarkFound, 1, "’");
            quotationMarkFound = text.find("'", quotationMarkFound + 3);
        }
    }

    std::string query_search;
    if (grade_id > 0) {
        query_search = StringUtils::format(" WHERE tbl_story.level_max = %d AND tbl_story.search LIKE '%%%s%%'", grade_id, text.c_str());
    }
    else {
        query_search = StringUtils::format(" WHERE tbl_story.search LIKE '%%%s%%'", text.c_str());
    }

    // bắt đầu query
    std::vector<mj::model::StoryInfo> r;
    std::string query = "select tbl_story.*, tbl_story_read.read from tbl_story "\
        "inner join (select * from tbl_story_category where category_id = " + StringUtils::toString(category_id) + ") tbl_temp on tbl_story.id = tbl_temp.story_id "\
        "left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang" + query_search;
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 16);

        r.push_back(info);
    }

    return r;
}

std::vector<mj::model::StoryInfo> StoryDataModel::getStoriesByCharacter(int character_id) {
    std::vector<mj::model::StoryInfo> r;
    std::string query = "select tbl_story.*, tbl_story_read.read from tbl_story "\
        "inner join (select * from tbl_story_character where character_id = " + StringUtils::toString(character_id) + ") tbl_temp on tbl_story.id = tbl_temp.story_id "\
        "left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang";
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 16);

        r.push_back(info);
    }

    return r;
}

void StoryDataModel::insertNewStories(std::vector<int> ids) {
    if (ids.size() <= 0)
        return;
    auto str_new_stories = mj::helper::vectorIntToString(ids);
    MJDEFAULT->setStringForKey("key_new_stories_in_order", str_new_stories);

    std::string query = "delete from tbl_story_new";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    query = "insert into tbl_story_new (id) values ";
    query = std::accumulate(std::begin(ids), std::end(ids), query, [](std::string q, int i) {
        return q + StringUtils::format("(%d),", i);
        });
    query.resize(query.size() - 1);
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
}

void StoryDataModel::insertPopularStories(std::vector<int> ids) {
    if (ids.size() <= 0)
        return;
    auto str_popular_stories = mj::helper::vectorIntToString(ids);
    MJDEFAULT->setStringForKey("key_popular_stories_in_order", str_popular_stories);

    std::string query = "delete from tbl_story_popular";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    query = "insert into tbl_story_popular (id) values ";
    query = std::accumulate(std::begin(ids), std::end(ids), query, [](std::string q, int i) {
        return q + StringUtils::format("(%d),", i);
        });
    query.resize(query.size() - 1);
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
}

std::vector<mj::model::StoryInfo> StoryDataModel::searchStoryByLanguage(int lang_id, std::string text, int category_id) {


    std::string key = std::to_string(lang_id) + text + std::to_string(category_id);

    auto listInfo = ms::CacheManager::getInstance().getListStoryInfoSearch();

    if (listInfo.count(key) > 0)
    {
        return listInfo.at(key);
    }
    
    text = mj::helper::to_no_accents_v2(text);
    //    text = NativeBridge::hsStringToKhongDau(text);

    //// < Dinh Duc - Single Quote (') for Stories Search Bar
    std::vector<mj::model::StoryInfo> r;

    bool is_IOS = false;
    bool has_qMark = false;
    size_t quotationMarkFound;

    if (text.find("’", 0) != std::string::npos)
    {
        is_IOS = true;
        has_qMark = true;
    }

    CCLOG("Is IOS?: %d", is_IOS);

    if (!is_IOS)
    {
        quotationMarkFound = text.find("'", 0);
        if (quotationMarkFound != std::string::npos)
            has_qMark = true;

        while (quotationMarkFound != std::string::npos)
        {
            text.replace(quotationMarkFound, 1, "’");
            quotationMarkFound = text.find("'", quotationMarkFound + 3);
        }
    }

    std::string query;
    if (category_id == -1)
        query = StringUtils::format("SELECT * FROM tbl_story left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d AND search LIKE '%%%s%%'", lang_id, text.c_str());
    else
        query = StringUtils::format("SELECT * FROM tbl_story left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d AND level_max = %d AND search LIKE '%%%s%%'", lang_id, category_id, text.c_str());
#if key_debugkey_dev_environment1
 /*   auto is_user_tutoring = MJDEFAULT->getBoolForKey("key_check_user_is_tutoring");
    if(!is_user_tutoring)
    {
        query = "select * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang LEFT OUTER JOIN tbl_story_tutoring_new ON tbl_story.id = tbl_story_tutoring_new.id WHERE lang_id = " + StringUtils::toString(lang_id) + " AND tbl_story_tutoring_new.app_id = 40";
    }*/

    //query = query + " LEFT OUTER JOIN tbl_story_tutoring_new "

#endif
    
    CCLOG("Query 1: %s", query.c_str());

    auto listStoryToturing = GetAllInfoTableToturing();

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 17);

        if (listStoryToturing.size() > 0)
        {
            auto it = std::find_if(listStoryToturing.begin(), listStoryToturing.end(), [=](mj::model::StoryInfoTutoring a) {
                return info.unique_id == a.storyId;
                });

            if (it != listStoryToturing.end())
            {
                continue;
            }
        }

        r.push_back(info);
    }

    if (has_qMark)
    {
        quotationMarkFound = text.find("’", 0);
        while (quotationMarkFound != std::string::npos)
        {
            text.replace(quotationMarkFound, 3, "''");
            quotationMarkFound = text.find("’", quotationMarkFound + 2);
        }

        if (category_id == -1)
            query = StringUtils::format("SELECT * FROM tbl_story left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d AND search LIKE '%%%s%%'", lang_id, text.c_str());
        else
            query = StringUtils::format("SELECT * FROM tbl_story left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d AND level_max = %d AND search LIKE '%%%s%%'", lang_id, category_id, text.c_str());

        CCLOG("Query 2: %s", query.c_str());

        sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            mj::model::StoryInfo info;

            info.unique_id = sqlite3_column_int(this->stmp, 0);
            info.story_id = sqlite3_column_int(this->stmp, 1);
            info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
            info.total_words = sqlite3_column_int(this->stmp, 3);
            info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
            info.lang_id = sqlite3_column_int(this->stmp, 6);
            info.total_page = sqlite3_column_int(this->stmp, 7);
            info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
            info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
            info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
            info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
            info.quality = sqlite3_column_int(this->stmp, 12);
            info.date_published = sqlite3_column_int(this->stmp, 13);
            info.levels.push_back(sqlite3_column_int(this->stmp, 14));
            info.version = sqlite3_column_int(this->stmp, 15);
            info.is_read = sqlite3_column_int(this->stmp, 17);

            if (info.is_read)
            {
                if (listStoryToturing.size() > 0)
                {
                    auto it = std::find_if(listStoryToturing.begin(), listStoryToturing.end(), [=](mj::model::StoryInfoTutoring a) {
                        return info.unique_id == a.storyId;
                        });

                    if (it != listStoryToturing.end())
                    {
                        continue;
                    }
                }
            }

            r.push_back(info);
        }
    }
    //// >
#if KEY_MEMMORY
    if (!r.empty())
    {
        ms::CacheManager::getInstance().setListStorySearch(key, r);
    }
#endif
    return r;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::searchAudiobookFromVector(std::vector<mj::model::AudiobookInfo> audio, std::string text, int category_id, bool is_IOS, bool has_qMark, std::string keyChache) {
    text = mj::helper::to_lower(mj::helper::to_no_accents_v2(text));
    std::vector<mj::model::AudiobookInfo> rs;
    if (audio.size() <= 0)
        return rs;
    std::copy_if(audio.begin(), audio.end(), std::back_inserter(rs), [text, category_id](mj::model::AudiobookInfo a) {
        auto name_no_accents = mj::helper::to_lower(mj::helper::to_no_accents_v2(a.name));
        if (category_id == -1)
            return name_no_accents.find(text) != std::string::npos;
        else {
            // Trường hợp AudioBook thuộc cấp độ "Cấp độ 1 & 2" thì lấy luôn cho 2 id.
            // id của các cấp độ này đang được fix cứng là 84 và 216
            // Xem "MsGradeManager"
            if ((category_id == 84 || category_id == 216)) {
                return (name_no_accents.find(text) != std::string::npos && (a.grade_id == 84 || a.grade_id == 216));
            }
            else {
                return (name_no_accents.find(text) != std::string::npos && a.grade_id == category_id);
            }

        }

        });

    size_t quotationMarkFound;

    if (has_qMark)
    {
        if (is_IOS)
        {
            quotationMarkFound = text.find("’", 0);
            while (quotationMarkFound != std::string::npos)
            {
                text.replace(quotationMarkFound, 3, "'");
                quotationMarkFound = text.find("’", quotationMarkFound + 1);
            }
        }
        else
        {
            quotationMarkFound = text.find("'", 0);
            while (quotationMarkFound != std::string::npos)
            {
                text.replace(quotationMarkFound, 1, "’");
                quotationMarkFound = text.find("'", quotationMarkFound + 3);
            }
        }
        std::copy_if(audio.begin(), audio.end(), std::back_inserter(rs), [text, category_id](mj::model::AudiobookInfo a) {
            auto name_no_accents = mj::helper::to_lower(mj::helper::to_no_accents_v2(a.name));
            if (category_id == -1)
                return name_no_accents.find(text) != std::string::npos;
            else
                return (name_no_accents.find(text) != std::string::npos && a.grade_id == category_id);
            });
    }


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    rs.erase(std::remove_if(rs.begin(), rs.end(), [](const mj::model::AudiobookInfo& a) {
        return StoryDataModel::getInstance()->isAudiobookChapter(a.book_id) || StoryDataModel::getInstance()->isChapterOfAudiobook(a.book_id);
        }), rs.end());
#endif

#if KEY_MEMMORY
    if (!rs.empty())
    {
        ms::CacheManager::getInstance().setListAudioBookSearch(keyChache, rs);
    }
#endif

    return rs;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::searchAudiobookByLanguage(int lang_id, std::string text, int category_id) {
    //// < Dinh Duc - Single Quote (') for Audiobook Search Bar

    std::string key = std::to_string(lang_id) + text + std::to_string(category_id);

    auto listInfo = ms::CacheManager::getInstance().getListAudiobookInfoSearch();

    if (listInfo.count(key) > 0)
    {
        return listInfo.at(key);
    }

    bool is_IOS = false, has_qMark = false;

    if (text.find("’", 0) != std::string::npos)
    {
        is_IOS = true;
        has_qMark = true;
    }
    else if (text.find("'", 0) != std::string::npos)
    {
        has_qMark = true;
    }

    return searchAudiobookFromVector(getAllAudiobooksByLanguage(lang_id), text, category_id, is_IOS, has_qMark);
}

std::vector<mj::model::StoryInfo> StoryDataModel::getAllStoriesByLanguage(int lang_id, int limitQuery) {

    auto listStoryInfo = ms::CacheManager::getInstance().getListStoryInfo();

    if (listStoryInfo.count(lang_id) > 0)
    {
        return listStoryInfo.at(lang_id);
    }

    std::vector<mj::model::StoryInfo> r;
    std::string query = "select * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = " + StringUtils::toString(lang_id);
        
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
    {
        query = "select * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = " + StringUtils::toString(lang_id) + std::string(" ORDER BY id DESC");
    }
//#endif

  /*  if (limitQuery != -1)
    {
        query = query + " limit " + std::to_string(limitQuery);
    }*/

    auto listStoryToturing = GetAllInfoTableToturing();

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.app_id = sqlite3_column_int(this->stmp, 22);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 17);

        if (listStoryToturing.size() > 0)
        {
            auto it = std::find_if(listStoryToturing.begin(), listStoryToturing.end(), [=](mj::model::StoryInfoTutoring a) {
                return info.unique_id == a.storyId;
                });

            if (it != listStoryToturing.end())
            {
                continue;
            }
        }

        r.push_back(info);
    }

#if KEY_MEMMORY
    ms::CacheManager::getInstance().setListStoyInfo(lang_id, r);
#endif

    return r;
}

std::vector<mj::model::StoryInfo> StoryDataModel::getNewStoriesByLanguage(int lang_id) {
    std::vector<mj::model::StoryInfo> r;
    std::string query = "select * from tbl_story inner join tbl_story_new on tbl_story.id = tbl_story_new.id "\
        "left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang "\
        "where tbl_story.lang_id = " + StringUtils::toString(lang_id);

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 20);

        r.push_back(info);
    }

    auto new_stories_in_order = mj::helper::stringToVectorInt(MJDEFAULT->getStringForKey("key_new_stories_in_order"));

    std::vector<mj::model::StoryInfo> r_in_order;

    for (auto id_story : new_stories_in_order) {
        auto story = std::find_if(r.begin(), r.end(), [id_story](mj::model::StoryInfo info) {
            return info.unique_id == id_story;
            });
        if (story != r.end()) {
            r_in_order.push_back(*story);
        }
    }

    return r_in_order;
}

std::vector<int> StoryDataModel::getListCategoryWithStoryId(int _id) {
    std::vector<int> results;
    std::string query = "SELECT category_id FROM tbl_story_category WHERE story_id = " + StringUtils::toString(_id);

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        results.push_back(sqlite3_column_int(this->stmp, 0));
    }

    return results;
}

std::vector<mj::model::StoryInfo> StoryDataModel::getFavouriteStoriesByLanguage(int lang_id) {
    auto list_ids = XHSLib->getFavoritesStory();
    std::vector<mj::model::StoryInfo> stories;
    for (auto idx : list_ids) {
        if (idx.asInt() < max_id_stories) {
            auto story = StoryDataModel::getInstance()->getStoryById(idx.asInt());
            if (story.story_id != 0)
                stories.push_back(story);
        }
    }

    return stories;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getFavouriteAudiobooksByLanguage(int lang_id) {
    auto list_ids = XHSLib->getFavoritesStory();
    std::vector<mj::model::AudiobookInfo> audiobooks;
    for (auto idx : list_ids) {
        if (idx.asInt() > max_id_stories && idx.asInt() < max_id_audiobook) {
            auto audio_book = StoryDataModel::getInstance()->getAudiobook(idx.asInt() - max_id_stories);

            if (!audio_book.audio_url.empty()) {
                audiobooks.push_back(audio_book);
            }
        }
    }

    return audiobooks;
}

void StoryDataModel::addFavouriteAudiobooksByLanguage(int audiobook_id, int lang_id) {
    XHSLib->addFavoritesStory(audiobook_id + max_id_stories);
}

void StoryDataModel::removeFavouriteAudiobookByLanguage(int audiobook_id, int lang_id) {
    XHSLib->removeFavoritesStory(audiobook_id + max_id_stories);
}
void StoryDataModel::removeFavouriteStoryByLanguage(int _id, int lang_id) {
    XHSLib->removeFavoritesStory(_id);
}

std::vector<mj::model::StoryInfo> StoryDataModel::getPopularStoriesByLanguage(int lang_id) {
    std::vector<mj::model::StoryInfo> r;
    std::string query = "select * from tbl_story inner join tbl_story_popular on tbl_story.id = tbl_story_popular.id "\
        "left join tbl_story_read on tbl_story.id = tbl_story_read.id_story_lang "\
        "where tbl_story.lang_id = " + StringUtils::toString(lang_id);

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 20);

        r.push_back(info);
    }

    auto popular_stories_in_order = mj::helper::stringToVectorInt(MJDEFAULT->getStringForKey("key_popular_stories_in_order"));
    std::vector<mj::model::StoryInfo> r_in_order;

    for (auto id_story : popular_stories_in_order) {
        auto story = std::find_if(r.begin(), r.end(), [id_story](mj::model::StoryInfo info) {
            return info.unique_id == id_story;
            });
        if (story != r.end()) {
            r_in_order.push_back(*story);
        }
    }

    return r_in_order;
}

std::vector<mj::model::StoryInfo> StoryDataModel::getRecentStoriesByLanguage(int lang_id) {
    std::vector<int> ids;
    //auto str_recentlyread = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, lang_id));
    auto str_recentlyread = PROFILE_MANAGER.getStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, lang_id));
    if (str_recentlyread != "") {
        ids = mj::helper::stringToVectorInt(str_recentlyread);
    }

    std::vector<mj::model::StoryInfo> recent;
    for (auto i : ids) {
        auto story = getStoryById(i);
        if (story.story_id != 0)
            recent.push_back(getStoryById(i));
    }

    return recent;
}

void StoryDataModel::insertFeatureStories(std::vector<std::pair<int, std::string>> feature_info) {
    if (feature_info.size() <= 0)
        return;
    std::string query = "delete from tbl_story_feature";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    query = "replace into tbl_story_feature (id, image_feature) values ";
    query = std::accumulate(std::begin(feature_info), std::end(feature_info), query, [](std::string q, std::pair<int, std::string> info) {
        return q + StringUtils::format("(%d,\"%s\"),", info.first, info.second.c_str());
        });
    query.resize(query.size() - 1);

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr)) {
        logError();
        return;
    }
}

void StoryDataModel::logError() {
    CCLOG("Database Error: %s", sqlite3_errmsg(this->db));
}

void StoryDataModel::insertGradeInfo(std::vector<mj::model::GradeInfo> grades) {
    if (grades.size() <= 0)
        return;
    std::string query = "delete from tbl_grades";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }

    query = "replace into tbl_grades " + mj::model::GradeInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(grades), std::end(grades), query, [](std::string q, mj::model::GradeInfo g) {
        return q + mj::model::GradeInfo::getSQLPlaceholders() + ",";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    };

    auto counter = 0;
    for (auto grade : grades) {
        sqlite3_bind_int(this->stmp, ++counter, grade.grade_id);
        sqlite3_bind_int(this->stmp, ++counter, grade.lang_id);
        sqlite3_bind_text(this->stmp, ++counter, grade.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, grade.description.c_str(), -1, SQLITE_TRANSIENT);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }
}

void StoryDataModel::removeGradeInfo(int lang_id)
{
    auto query = StringUtils::format("delete from tbl_grades where lang_id = %d", lang_id);
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("remove grade info of grades %d", lang_id);
    }

    FileUtils::getInstance()->purgeCachedEntries();
}

mj::model::StoryInfo StoryDataModel::getStoryById(int id) {
    std::string query = StringUtils::format("select * from tbl_story left outer join tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang where id = %d", id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    mj::model::StoryInfo info;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 17);
    }
    //    sqlite3_finalize(this->stmp);
    return info;
}

mj::model::StoryInfo StoryDataModel::getStoryByStoryIdAndLang(int id, int lang_id) {
    std::string query = StringUtils::format("select * from tbl_story left outer join tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang where story_id = %d and lang_id = %d", id, lang_id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    mj::model::StoryInfo info;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 17);
    }
    return info;
}

mj::model::LevelInfo StoryDataModel::getLevelInfoById(int level_id, int lang_id) {

    std::string key = std::to_string(level_id) + std::to_string(lang_id);
    auto listInfoLevel = ms::CacheManager::getInstance().getLevelInfo();

    if (listInfoLevel.count(key) > 0)
    {
        return listInfoLevel.at(key);
    }

    std::string query = StringUtils::format("select * from tbl_reading_levels where lang_id = %d and level_id = %d", lang_id, level_id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    mj::model::LevelInfo level;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        level.level_id = sqlite3_column_int(this->stmp, 0);
        level.lang_id = sqlite3_column_int(this->stmp, 1);
        level.grade_id = sqlite3_column_int(this->stmp, 2);
        level.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        level.description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
    }
#if KEY_MEMMORY
    ms::CacheManager::getInstance().setListLevelInfo(key, level);
#endif

    return level;
}

mj::model::GradeInfo StoryDataModel::getGradeInfoById(int grade_id, int lang_id)
{
    std::string query = StringUtils::format("select * from tbl_grades where grade_id = %d and lang_id = %d", grade_id, lang_id);

    mj::model::GradeInfo grade;

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK)
    {
        logError();
        return grade;
    }

    if (sqlite3_step(this->stmp) == SQLITE_ROW)
    {
        grade.grade_id = sqlite3_column_int(this->stmp, 0);
        grade.lang_id = sqlite3_column_int(this->stmp, 1);
        grade.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2));
        grade.description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
    }

    return grade;
}

bool StoryDataModel::storyIsRated(int story_lang_id) {

    int rate_value = 0;
    this->stmp = nullptr;
    string query = StringUtils::format("SELECT rate_value FROM tbl_story_rate WHERE story_lang_id = %d", story_lang_id);
    CCLOG("query = %s", query.c_str());
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    if (result != SQLITE_OK) {
        this->showError(result);
        return rate_value;
    }
    if (sqlite3_step(this->stmp) == SQLITE_ROW)
    {
        rate_value = sqlite3_column_int(this->stmp, 0);
    }
    CCLOG("rate_value = %d", rate_value);

    if (rate_value > 0) {
        rate_value = 1;
    }
    return rate_value;
}
void StoryDataModel::setStoryIsRated(int story_lang_id, int rate_value) {
    std::string query = "REPLACE INTO tbl_story_rate (story_lang_id,rate_value) VALUES (?,?)";
    this->stmp = NULL;
    ValueMap item;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    sqlite3_bind_int(this->stmp, 1, story_lang_id);
    sqlite3_bind_int(this->stmp, 2, rate_value);
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        this->showError(result);
        return;
    }
    CCLOG("tbl_story_rate Done ID = %d", story_lang_id);
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAllAudiobooksByLanguage(int lang_id) {

    auto listAudiobookInfo = ms::CacheManager::getInstance().getListAudiobookInfo();

    if (listAudiobookInfo.count(lang_id) > 0)
    {
        return listAudiobookInfo.at(lang_id);
    }

    std::string query = StringUtils::format("SELECT * FROM tbl_audiobook_v2 LEFT OUTER JOIN tbl_audio_read ON tbl_audiobook_v2._id = tbl_audio_read.audio_id WHERE lang_id = %d", lang_id);
    //    string query = StringUtils::format("SELECT * FROM tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang WHERE lang_id = %d", lang_id);
//    list_data_stories = StoryDataModel::getInstance()->getAllStories(MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_level_choose, lang_id), 0), lang_id, "quality DESC, read, date_publish DESC");

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::AudiobookInfo> r;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AudiobookInfo info;

        info.book_id = SQLITE_COLUMN_INT(0);
        info.lang_id = SQLITE_COLUMN_INT(1);
        info.grade_id = SQLITE_COLUMN_INT(2);
        info.name = SQLITE_COLUMN_STR(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.sync_path = SQLITE_COLUMN_STR(5);
        info.audio_url = SQLITE_COLUMN_STR(6);
        info.audio_url = mj::helper::getUrlFromDomain(info.audio_url, mj::UrlType::MSURL_AUDIO);
        info.summary = SQLITE_COLUMN_STR(7);
        info.duration = SQLITE_COLUMN_INT(8);
        info.quality = SQLITE_COLUMN_INT(9);
        info.score = SQLITE_COLUMN_INT(10);
        info.date_publish = SQLITE_COLUMN_INT(11);
        info.extra = SQLITE_COLUMN_STR(12);
        info.read = SQLITE_COLUMN_INT(14);
        info.time_read = SQLITE_COLUMN_INT(15);

        if (info.audio_url.empty()) {
            continue;
        }

        if (isChapterOfAudiobook(info.book_id)) {
            continue;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (isAudiobookChapter(info.book_id)) {
            continue;
        }
#endif

        r.push_back(info);
    }

#if KEY_MEMMORY
    ms::CacheManager::getInstance().setListAudiobookInfo(lang_id, r);
#endif

    return r;
}

mj::model::AudiobookInfo StoryDataModel::getAudiobook(int book_id) {
    std::string query = StringUtils::format("select * FROM tbl_audiobook_v2 LEFT OUTER JOIN tbl_audio_read ON tbl_audiobook_v2._id = tbl_audio_read.audio_id where _id = %d", book_id);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        mj::model::AudiobookInfo r;
        return r;
    };
    mj::model::AudiobookInfo r;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        r.book_id = SQLITE_COLUMN_INT(0);
        r.lang_id = SQLITE_COLUMN_INT(1);
        r.grade_id = SQLITE_COLUMN_INT(2);
        r.name = SQLITE_COLUMN_STR(3);
        r.thumbnail_url = SQLITE_COLUMN_STR(4);
        r.thumbnail_url = mj::helper::getUrlFromDomain(r.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        r.sync_path = SQLITE_COLUMN_STR(5);
        r.audio_url = SQLITE_COLUMN_STR(6);
        r.summary = SQLITE_COLUMN_STR(7);
        r.duration = SQLITE_COLUMN_INT(8);
        r.quality = SQLITE_COLUMN_INT(9);
        r.score = SQLITE_COLUMN_INT(10);
        r.date_publish = SQLITE_COLUMN_INT(11);
        r.extra = SQLITE_COLUMN_STR(12);
        r.read = SQLITE_COLUMN_INT(14);
        r.time_read = SQLITE_COLUMN_INT(15);
    }
    sqlite3_reset(this->stmp);

    return r.audio_url.empty() ? mj::model::AudiobookInfo() : r;
}

void StoryDataModel::updateAudiobookChapter(std::vector<std::tuple<int, int, int>> chapter_list) {

    if (sqlite3_exec(this->db, "delete from audiobook_chapters", nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    if (chapter_list.size() <= 0)
        return;
    //    if (series.size() > 50) {
    auto first = 0;
    const auto chunk_size = 50;
    while (first < chapter_list.size()) {
        auto last = std::min(first + chunk_size, (int)chapter_list.size());
        auto data = std::vector<std::tuple<int, int, int>>(std::begin(chapter_list) + first, std::begin(chapter_list) + last);

        std::string query = std::string("replace into audiobook_chapters (main_id, chapter_id, chapter_order) values");
        query = std::accumulate(data.begin(), data.end(), query, [](std::string q, std::tuple<int, int, int> p) {
            return q.append("(?, ?, ?),");
            });
        query.resize(query.length() - 1);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            continue;
        }

        auto counter = 0;
        for (auto p : data) {
            SQLITE_BIND_INT(++counter, std::get<0>(p));
            SQLITE_BIND_INT(++counter, std::get<1>(p));
            SQLITE_BIND_INT(++counter, std::get<2>(p));
        }
        if (sqlite3_step(this->stmp) != SQLITE_DONE) {
            logError();
            continue;
        }
        CCLOG("insert chapter done");

        first = last;
    }
}

bool StoryDataModel::isAudiobookChapter(int book_id) {
    return mj::helper::isContain(_audiobooks_with_chapters, book_id);
}

bool StoryDataModel::isChapterOfAudiobook(int book_id) {
    return mj::helper::isContain(_chapter_of_audiobooks, book_id);
}

mj::model::AudiobookInfo StoryDataModel::getOriginalBookOfChapter(int chapter_id) {
    mj::model::AudiobookInfo info;
    auto query = "select main_id from audiobook_chapters where chapter_id = ?";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return info;
    }

    SQLITE_BIND_INT(1, chapter_id);

    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto book_id = SQLITE_COLUMN_INT(0);
        return getAudiobook(book_id);
    }

    return info;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAudiobookChapters() {
    std::vector<mj::model::AudiobookInfo> chapters;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return chapters;
#endif
    auto query = "select * from tbl_audiobook_v2 join (select DISTINCT main_id from audiobook_chapters) as a on tbl_audiobook_v2._id = a.main_id";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return chapters;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AudiobookInfo info;

        info.book_id = SQLITE_COLUMN_INT(0);
        info.lang_id = SQLITE_COLUMN_INT(1);
        info.grade_id = SQLITE_COLUMN_INT(2);
        info.name = SQLITE_COLUMN_STR(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.sync_path = SQLITE_COLUMN_STR(5);
        info.audio_url = SQLITE_COLUMN_STR(6);
        info.audio_url = mj::helper::getUrlFromDomain(info.audio_url, mj::UrlType::MSURL_AUDIO);
        info.summary = SQLITE_COLUMN_STR(7);
        info.duration = SQLITE_COLUMN_INT(8);
        info.quality = SQLITE_COLUMN_INT(9);
        info.score = SQLITE_COLUMN_INT(10);
        info.date_publish = SQLITE_COLUMN_INT(11);
        info.extra = SQLITE_COLUMN_STR(12);
        info.read = SQLITE_COLUMN_INT(16);
        info.time_read = SQLITE_COLUMN_INT(17);

        if (info.name.empty() || info.audio_url.empty()) {
            continue;
        }

        chapters.push_back(info);
    }
    return chapters;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getChapterOfAudiobook(int book_id) {
    //"select * from tbl_audiobook_v2 join (select * from audiobook_chapters where main_id = 797) as a on tbl_audiobook_v2._id = a.main_id"
    std::vector<mj::model::AudiobookInfo> chapters;
    auto query = "select * from tbl_audiobook_v2 join (select * from audiobook_chapters where main_id = ?) as a on tbl_audiobook_v2._id = a.chapter_id order by a.chapter_order asc";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return chapters;
    }

    SQLITE_BIND_INT(1, book_id);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AudiobookInfo info;

        info.book_id = SQLITE_COLUMN_INT(0);
        info.lang_id = SQLITE_COLUMN_INT(1);
        info.grade_id = SQLITE_COLUMN_INT(2);
        info.name = SQLITE_COLUMN_STR(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.sync_path = SQLITE_COLUMN_STR(5);
        info.audio_url = SQLITE_COLUMN_STR(6);
        info.audio_url = mj::helper::getUrlFromDomain(info.audio_url, mj::UrlType::MSURL_AUDIO);
        info.summary = SQLITE_COLUMN_STR(7);
        info.duration = SQLITE_COLUMN_INT(8);
        info.quality = SQLITE_COLUMN_INT(9);
        info.score = SQLITE_COLUMN_INT(10);
        info.date_publish = SQLITE_COLUMN_INT(11);
        info.extra = SQLITE_COLUMN_STR(12);
        info.read = SQLITE_COLUMN_INT(16);
        info.time_read = SQLITE_COLUMN_INT(17);

        if (info.audio_url.empty()) {
            continue;
        }

        chapters.push_back(info);
    }
    return chapters;
}


void StoryDataModel::updateAudiobookUrlPath(mj::model::AudiobookInfo info) {
    //    std::string query = "replace into tbl_audiobook() values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    std::string query = "update tbl_audiobook_v2 set content_path = ?, audio_url = ? where _id = ?";
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    SQLITE_BIND_STR(1, info.sync_path);
    SQLITE_BIND_STR(2, info.audio_url);
    SQLITE_BIND_INT(3, info.book_id);

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

void StoryDataModel::insertAudiobookSeries(std::vector<mj::model::AudiobookSeriesInfo> series) {

    if (series.size() <= 0)
        return;
    MJ_DB_SPLIT_QUERY(insertAudiobookSeries, mj::model::AudiobookSeriesInfo, series);

    std::string query = std::string("replace into tbl_audiobook_series2 (id,title,thumb,book,hidden) values");

    query = std::accumulate(series.begin(), series.end(), query, [](std::string q, mj::model::AudiobookSeriesInfo p) {
        return q.append("(?,?,?,?,?),");
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    }

    auto counter = 0;

    for (auto p : series) {
        SQLITE_BIND_INT(++counter, p.id);
        SQLITE_BIND_STR(++counter, p.title);
        SQLITE_BIND_STR(++counter, p.thumbnail_url);
        SQLITE_BIND_STR(++counter, p.jsonBooks);
        SQLITE_BIND_INT(++counter, p.hidden);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }

    CCLOG("insert series done");
}

std::vector<mj::model::AudiobookSeriesInfo> StoryDataModel::getAudiobookSeries() {

    std::string query = "select * from tbl_audiobook_series2";
    std::vector<std::string> series;
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::AudiobookSeriesInfo> result;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::AudiobookSeriesInfo info;

        info.id = SQLITE_COLUMN_INT(0);
        info.title = SQLITE_COLUMN_STR(1);
        info.thumbnail_url = SQLITE_COLUMN_STR(2);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.jsonBooks = SQLITE_COLUMN_STR(3);
        info.hidden = SQLITE_COLUMN_INT(4);

        info.convertBookJsonToBookArray();//importance

        result.push_back(info);
    }

    return result;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAllAudiobooksByBookIDs(std::vector<int> bookIDs)
{
    std::vector<mj::model::AudiobookInfo> r;
    if (bookIDs.size() <= 0)
        return r;
    std::string bookIDsSQL = "";

    for (int i = 0; i < bookIDs.size(); i++) {

        bookIDsSQL += StringUtils::format(i == bookIDs.size() - 1 ? "%d" : "%d,", bookIDs[i]);
    }

    std::string query = StringUtils::format("SELECT * FROM tbl_audiobook_v2 where _id in (%s)", bookIDsSQL.c_str());

    CCLOG("StoryDataModel::getAllAudiobooksByBookIDs: query:%s", query.c_str());



    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return r;
    };

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::AudiobookInfo info;

        info.book_id = SQLITE_COLUMN_INT(0);
        info.lang_id = SQLITE_COLUMN_INT(1);
        info.grade_id = SQLITE_COLUMN_INT(2);
        info.name = SQLITE_COLUMN_STR(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.sync_path = SQLITE_COLUMN_STR(5);
        info.audio_url = SQLITE_COLUMN_STR(6);
        info.audio_url = mj::helper::getUrlFromDomain(info.audio_url, mj::UrlType::MSURL_AUDIO);
        info.summary = SQLITE_COLUMN_STR(7);
        info.duration = SQLITE_COLUMN_INT(8);
        info.quality = SQLITE_COLUMN_INT(9);
        info.score = SQLITE_COLUMN_INT(10);
        info.date_publish = SQLITE_COLUMN_INT(11);
        info.extra = SQLITE_COLUMN_STR(12);
        info.read = SQLITE_COLUMN_INT(16);
        info.time_read = SQLITE_COLUMN_INT(17);

        if (info.audio_url.empty() || isChapterOfAudiobook(info.book_id)) {
            continue;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (isAudiobookChapter(info.book_id)) {
            continue;
        }
#endif

        r.push_back(info);
    }

    return r;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAllAudiobooksBySeries(std::string name) {
    std::string query = StringUtils::format("SELECT * FROM tbl_audiobook_v2 a inner JOIN tbl_audiobook_series b ON a._id = b.book_id LEFT OUTER JOIN tbl_audio_read c ON a._id = c.audio_id where b.series = ? and lang_id = ?");

    std::vector<mj::model::AudiobookInfo> r;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return r;
    };

    SQLITE_BIND_STR(1, name);
    SQLITE_BIND_INT(2, LANGUAGE_MANAGER->getCurrentLangId());

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AudiobookInfo info;

        info.book_id = SQLITE_COLUMN_INT(0);
        info.lang_id = SQLITE_COLUMN_INT(1);
        info.grade_id = SQLITE_COLUMN_INT(2);
        info.name = SQLITE_COLUMN_STR(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_AUDIO);
        info.sync_path = SQLITE_COLUMN_STR(5);
        info.audio_url = SQLITE_COLUMN_STR(6);
        info.audio_url = mj::helper::getUrlFromDomain(info.audio_url, mj::UrlType::MSURL_AUDIO);
        info.summary = SQLITE_COLUMN_STR(7);
        info.duration = SQLITE_COLUMN_INT(8);
        info.quality = SQLITE_COLUMN_INT(9);
        info.score = SQLITE_COLUMN_INT(10);
        info.date_publish = SQLITE_COLUMN_INT(11);
        info.extra = SQLITE_COLUMN_STR(12);
        info.read = SQLITE_COLUMN_INT(16);
        info.time_read = SQLITE_COLUMN_INT(17);

        if (info.audio_url.empty() || isChapterOfAudiobook(info.book_id)) {
            continue;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (isAudiobookChapter(info.book_id)) {
            continue;
        }
#endif

        r.push_back(info);
    }
    return r;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAudioBookRecentRead() {
    //auto list_ids = XHSLib->getAudioRecentlyRead();
    auto list_ids = PROFILE_MANAGER.getAudioRecentlyRead();
    std::vector<mj::model::AudiobookInfo> audiobooks;
    for (auto idx : list_ids) {
        auto audio_book = StoryDataModel::getInstance()->getAudiobook(idx.asInt());

        if (audio_book.audio_url.empty()) {
            continue;
        }

        audiobooks.push_back(audio_book);
    }
    return audiobooks;
}

void StoryDataModel::addAudioBookRecentRead(int _id) {
    //XHSLib->addAudioToRecentlyRead(_id);
    PROFILE_MANAGER.addAudioToRecentlyRead(_id);
}


void StoryDataModel::insertAudiobook(mj::model::AudiobookInfo info) {
    std::string query = std::string("replace into tbl_audiobook_v2") + mj::model::AudiobookInfo::getSQLStructure() + " values" + mj::model::AudiobookInfo::getSQLPlaceholders();
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    SQLITE_BIND_INT(1, info.book_id);
    SQLITE_BIND_INT(2, info.lang_id);
    SQLITE_BIND_INT(3, info.grade_id);
    SQLITE_BIND_STR(4, info.name);
    SQLITE_BIND_STR(5, info.thumbnail_url);
    SQLITE_BIND_STR(6, info.sync_path);
    SQLITE_BIND_STR(7, info.audio_url);
    SQLITE_BIND_STR(8, info.summary);
    SQLITE_BIND_INT(9, info.duration);
    SQLITE_BIND_STR(10, info.extra);

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

void StoryDataModel::insertAudiobooks(std::vector<mj::model::AudiobookInfo> books) {
    if (books.size() <= 0)
        return;
    if (books.size() > NUMBER_QUERY_ST_AU) {
        auto first = 0;
        const auto chunk_size = NUMBER_QUERY_ST_AU;
        while (first < books.size()) {
            auto last = std::min(first + chunk_size, (int)books.size());
            insertAudiobooks(std::vector<mj::model::AudiobookInfo>(std::begin(books) + first, std::begin(books) + last));
            first = last;
        }

        return;
    }

    std::string query = "replace into tbl_audiobook_v2" + mj::model::AudiobookInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(books), std::end(books), query, [](std::string q, mj::model::AudiobookInfo i) {
        return q.append(mj::model::AudiobookInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto book : books) {
        //_id, lang_id, grade_id, name, thumb_url, content_path, audio_url, summary, duration
        SQLITE_BIND_INT(++counter, book.book_id);
        SQLITE_BIND_INT(++counter, book.lang_id);
        SQLITE_BIND_INT(++counter, book.grade_id);
        SQLITE_BIND_STR(++counter, book.name);
        SQLITE_BIND_STR(++counter, book.thumbnail_url);
        SQLITE_BIND_STR(++counter, book.sync_path);
        SQLITE_BIND_STR(++counter, book.audio_url);
        SQLITE_BIND_STR(++counter, book.summary);
        SQLITE_BIND_INT(++counter, book.duration);
        SQLITE_BIND_INT(++counter, book.quality);
        SQLITE_BIND_INT(++counter, book.score);
        SQLITE_BIND_INT(++counter, book.date_publish);
        SQLITE_BIND_STR(++counter, book.extra);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert audiobooks done");
}

void StoryDataModel::setPopularSearchAudiobooks(const std::vector<mj::model::PopularSearchAudiobookInfo>& books) {
    // delete all popular materials
    std::string query = "DELETE FROM tbl_audiobook_popular_search";

    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }
    CCLOG("Deleted all popular audiobooks");

    // insert new
    insertPopularSearchAudiobooks(books);
}

void StoryDataModel::insertPopularSearchAudiobooks(std::vector<mj::model::PopularSearchAudiobookInfo> books)
{
    if (books.size() <= 0)
        return;
    MJ_DB_SPLIT_QUERY(insertPopularSearchAudiobooks, mj::model::PopularSearchAudiobookInfo, books);

    std::string query = "replace into tbl_audiobook_popular_search(id, keyword, thumb, sort, date_publish) values ";
    query = std::accumulate(std::begin(books), std::end(books), query, [](std::string q, mj::model::PopularSearchAudiobookInfo i) {
        return q.append("(?,?,?,?,?),");
        });

    CCLOG("StoryDataModel::insertPopularSearchAudiobook:%s", query.c_str());

    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto book : books) {
        SQLITE_BIND_INT(++counter, book.id);
        SQLITE_BIND_STR(++counter, book.keyword);
        SQLITE_BIND_STR(++counter, book.thumbnailUrl);
        SQLITE_BIND_INT(++counter, book.sort);
        SQLITE_BIND_INT(++counter, book.date_publish);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert audiobooks done");

}

std::vector<mj::model::PopularSearchAudiobookInfo> StoryDataModel::getPopularSearchAudiobooks()
{
    std::string query = "select * from tbl_audiobook_popular_search";
    std::vector<std::string> series;
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::PopularSearchAudiobookInfo> result;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::PopularSearchAudiobookInfo info;

        info.id = SQLITE_COLUMN_INT(0);
        info.keyword = SQLITE_COLUMN_STR(1);
        info.thumbnailUrl = SQLITE_COLUMN_STR(2);
        info.thumbnailUrl = mj::helper::getUrlFromDomain(info.thumbnailUrl, mj::UrlType::MSURL_MEDIA);
        info.sort = SQLITE_COLUMN_INT(3);
        info.date_publish = SQLITE_COLUMN_INT(4);

        result.push_back(info);
    }

    return result;
}

void StoryDataModel::setPopularSearchStories(const std::vector<mj::model::PopularSearchStoryInfo>& stories)
{
    // delete all popular materials
    if (stories.size() <= 0)
        return;
    std::string query = "DELETE FROM tbl_story_popular_search";

    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }
    CCLOG("Deleted all popular stories");

    // insert new
    insertPopularSearchStories(stories);
}

void StoryDataModel::insertPopularSearchStories(std::vector<mj::model::PopularSearchStoryInfo> books)
{
    if (books.size() <= 0)
        return;
    MJ_DB_SPLIT_QUERY(insertPopularSearchStories, mj::model::PopularSearchStoryInfo, books);

    std::string query = "replace into tbl_story_popular_search(id, keyword, thumb, sort, date_publish) values ";
    query = std::accumulate(std::begin(books), std::end(books), query, [](std::string q, mj::model::PopularSearchStoryInfo i) {
        return q.append("(?,?,?,?,?),");
        });

    CCLOG("StoryDataModel::insertPopularSearchAudiobook:%s", query.c_str());

    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto book : books) {
        SQLITE_BIND_INT(++counter, book.id);
        SQLITE_BIND_STR(++counter, book.keyword);
        SQLITE_BIND_STR(++counter, book.thumbnailUrl);
        SQLITE_BIND_INT(++counter, book.sort);
        SQLITE_BIND_INT(++counter, book.date_publish);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert audiobooks done");

}

std::vector<mj::model::PopularSearchStoryInfo> StoryDataModel::getPopularSearchStories()
{
    std::string query = "select * from tbl_story_popular_search ORDER BY sort";

    std::vector<std::string> series;
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::PopularSearchStoryInfo> result;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::PopularSearchStoryInfo info;

        info.id = SQLITE_COLUMN_INT(0);
        info.keyword = SQLITE_COLUMN_STR(1);
        info.thumbnailUrl = SQLITE_COLUMN_STR(2);
        info.thumbnailUrl = mj::helper::getUrlFromDomain(info.thumbnailUrl, mj::UrlType::MSURL_MEDIA);
        info.sort = SQLITE_COLUMN_INT(3);
        info.date_publish = SQLITE_COLUMN_INT(4);

        result.push_back(info);
    }

    return result;
}


void StoryDataModel::removeAudiobooks(std::vector<int> idx) {
    if (idx.size() <= 0)
        return;
    std::string query = "delete from tbl_audiobook_v2 where _id in (";
    query = std::accumulate(std::begin(idx), std::end(idx), query, [](std::string q, int i) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query = query + ")";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto i : idx) {
        SQLITE_BIND_INT(++counter, i);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }

    CCLOG("delete audiobooks done");
}

void StoryDataModel::insertLessons(std::vector<mj::model::LessonInfo> lessons, mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall tag) {
    if (lessons.empty()) {
        return;
    }
    if (lessons.size() > NUMBER_QUERY) {
        auto first = 0;
        const auto chunk_size = NUMBER_QUERY;
        while (first < lessons.size()) {
            auto last = std::min(first + chunk_size, (int)lessons.size());
            insertLessons(std::vector<mj::model::LessonInfo>(std::begin(lessons) + first, std::begin(lessons) + last), tag);
            first = last;
        }

        return;
    }

    if(tag!= mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install)
    {
        for (auto lesson : lessons) {

            removeActivitiesOfLesson(lesson.lesson_id);
        }
    }
    else
    {
        // don't remove file in here, because when is firstinstall update, size of lesson too much --> efect to performance if search remove files
    }


    std::string query = "replace into tbl_lesson_v2" + mj::model::LessonInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(lessons), std::end(lessons), query, [](std::string q, mj::model::LessonInfo i) {
        return q.append(mj::model::LessonInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto lesson : lessons) {
        //_id, _order, lang_id, name, thumb_url
        SQLITE_BIND_INT(++counter, lesson.lesson_id);
        SQLITE_BIND_INT(++counter, lesson.lesson_order);
        SQLITE_BIND_INT(++counter, lesson.lang_id);
        SQLITE_BIND_INT(++counter, lesson.category_id);
        SQLITE_BIND_STR(++counter, lesson.name);
        SQLITE_BIND_STR(++counter, lesson.thumbnail_url);
        SQLITE_BIND_INT(++counter, lesson.time_published);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert lessons done");
}

void StoryDataModel::removeLessonsById(std::vector<int> idx, mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall tag) {

    if (idx.size() <= 0)
        return;
    if (tag != mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install)

    {
        for (auto i : idx) {
            removeActivitiesOfLesson(i);
        }
    }
    else
    {
        // don't remove file in here, because when is firstinstall update, size of lesson too much --> efect to performance if search remove files
    }

    std::string query = "delete from tbl_lesson_v2 where _id in (";
    query = std::accumulate(std::begin(idx), std::end(idx), query, [](std::string q, int i) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query = query + ")";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto i : idx) {
        SQLITE_BIND_INT(++counter, i);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }

    CCLOG("delete lessons done");
}

void StoryDataModel::resetLessonLearned() {
    std::string query = "update tbl_lesson_v2 set learned = 0";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

ValueVector StoryDataModel::getLessonLearned() {
    std::string query = "select _id, category_id, learned from tbl_lesson_v2 where learned = 1";
    return fetchAllRecord(query);
}

std::vector<int> StoryDataModel::getLessonLearnByID(int lessonId)
{
    std::vector <int> valueLessonLearn = {};

    try
    {
        std::string  query = StringUtils::format("select  _id, category_id, learned from tbl_lesson_v2 where _id=%d AND learned = 1", lessonId);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            return valueLessonLearn;
        };

        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 0));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 1));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 2));
        }
    }
    catch (const std::exception& e)
    {

    }

    return valueLessonLearn;
}

void StoryDataModel::setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn)
{
    if (listLessonLearn.size() <= 0)
        return;
    if (listLessonLearn.size() > 100) {
        auto first = 0;
        const auto chunk_size = 100;
        while (first < listLessonLearn.size()) {
            auto last = std::min(first + chunk_size, (int)listLessonLearn.size());
            setLessonLearned(std::vector<std::pair<std::string, int>>(std::begin(listLessonLearn) + first, std::begin(listLessonLearn) + last));
            first = last;
        }

        return;
    }

    std::string query = "update tbl_lesson_v2 set learned = 1 where _id in (";
    query = std::accumulate(listLessonLearn.begin(), listLessonLearn.end(), query, [](std::string q, std::pair<std::string, int> i) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query.append(")");
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto bind_counter = 0;
    for (auto i : listLessonLearn) {
        SQLITE_BIND_INT(++bind_counter,std::stoi(i.first));
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

void StoryDataModel::setLessonLearned(std::vector<int> lesson_ids) {
    if (lesson_ids.size() <= 0)
        return;
    std::string query = "update tbl_lesson_v2 set learned = 1 where _id in (";
    query = std::accumulate(lesson_ids.begin(), lesson_ids.end(), query, [](std::string q, int) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query.append(")");
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    auto bind_counter = 0;
    for (auto i : lesson_ids) {
        SQLITE_BIND_INT(++bind_counter, i);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

std::vector<int> StoryDataModel::getDownloadedLessons() {
    std::vector<int> lessons;
    std::string query = "select _id, _order, category_id from tbl_lesson_v2 inner join (select lesson_id from tbl_activity group by lesson_id) t on tbl_lesson_v2._id = t.lesson_id order by category_id, _order asc";
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return lessons;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        lessons.push_back(SQLITE_COLUMN_INT(0));
    }
    return lessons;
}

std::vector<mj::model::LessonInfo> StoryDataModel::getAllLessonsByLanguage(int lang_id) {
    std::string query = StringUtils::format("select * from tbl_lesson_v2 order by _order asc");
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::LessonInfo> r;

    std::map<int, int> cat_count;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::LessonInfo info;

        info.lesson_id = SQLITE_COLUMN_INT(0);
        info.lesson_order = SQLITE_COLUMN_INT(1);
        info.lang_id = SQLITE_COLUMN_INT(2);
        info.category_id = SQLITE_COLUMN_INT(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        info.name = SQLITE_COLUMN_STR(5);
        info.state = mj::model::LessonInfo::State::LOCKED;
        info.is_learned = SQLITE_COLUMN_INT(6);
        info.time_published = SQLITE_COLUMN_INT(7);

        if (info.category_id == 479 /*|| info.category_id == 480 || (info.category_id == 481 && (info.lesson_order > 0 && info.lesson_order < 15))*/)
        {
            CCLOG("/%d/ : %d, ", info.lesson_id, info.category_id);
        }

        if (cat_count.find(info.category_id) != cat_count.end()) {
            info.lesson_order = cat_count[info.category_id];
        }
        else {
            info.lesson_order = 1;
        }
        cat_count[info.category_id] = info.lesson_order + 1;

        r.push_back(info);
    }
    return r;
}


mj::model::LessonInfo StoryDataModel::getLessonsById(int lessonId) {

    std::string query = StringUtils::format("SELECT * FROM tbl_lesson_v2 WHERE _id = %d", lessonId);

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    mj::model::LessonInfo r;

    std::map<int, int> cat_count;

    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::LessonInfo info;

        info.lesson_id = SQLITE_COLUMN_INT(0);
        info.lesson_order = SQLITE_COLUMN_INT(1);
        info.lang_id = SQLITE_COLUMN_INT(2);
        info.category_id = SQLITE_COLUMN_INT(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        info.name = SQLITE_COLUMN_STR(5);
        info.state = mj::model::LessonInfo::State::LOCKED;
        info.is_learned = SQLITE_COLUMN_INT(6);
        info.time_published = SQLITE_COLUMN_INT(7);

        if (info.category_id == 479 || info.category_id == 480 || (info.category_id == 481 && (info.lesson_order > 0 && info.lesson_order < 15)))
        {
            CCLOG("/%d/ : %d, ", info.lesson_id, info.category_id);
        }

        if (cat_count.find(info.category_id) != cat_count.end()) {
            info.lesson_order = cat_count[info.category_id];
        }
        else {
            info.lesson_order = 1;
        }
        cat_count[info.category_id] = info.lesson_order + 1;

        r = info;
    }
    return r;
}

//std::vector<mj::model::LessonInfo> StoryDataModel::getAllLessonsByGrade(int grade_id) {
//    std::string query = StringUtils::format("select * from tbl_lesson_v2 where category_id = %d order by _order asc", grade_id);
//    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
//
//    std::vector<mj::model::LessonInfo> r;
//
//    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
//        mj::model::LessonInfo info;
//
//        info.lesson_id = SQLITE_COLUMN_INT(0);
//        info.lesson_order = SQLITE_COLUMN_INT(1);
//        info.lang_id = SQLITE_COLUMN_INT(2);
//        info.category_id = SQLITE_COLUMN_INT(3);
//        info.thumbnail_url = SQLITE_COLUMN_STR(4);
//        info.name = SQLITE_COLUMN_STR(5);
//        info.state = mj::model::LessonInfo::State::UNLOCKED;
//
//        r.push_back(info);
//    }
//    return r;
//}

//// < Dinh Duc - Lesson Search Bar
std::vector<mj::model::LessonInfo> StoryDataModel::searchLessonByLanguage(int lang_id, std::string text)
{
    text = mj::helper::to_no_accents_v2(text);
    //    text = NativeBridge::hsStringToKhongDau(text);

        //// < Dinh Duc - Single Quote (') for Stories Search Bar

    std::vector<mj::model::LessonInfo> ret;

    bool is_IOS = false;
    bool has_apostrophe = false;
    size_t apostrophe_found;

    if (text.find("’", 0) != std::string::npos)
    {
        is_IOS = true;
        has_apostrophe = true;
    }

    if (!is_IOS)
    {
        apostrophe_found = text.find("'", 0);
        if (apostrophe_found != std::string::npos)
            has_apostrophe = true;

        while (apostrophe_found != std::string::npos)
        {
            text.replace(apostrophe_found, 1, "’");
            apostrophe_found = text.find("'", apostrophe_found + 3);
        }
    }

    std::string query = StringUtils::format("SELECT * FROM tbl_lesson_v2 WHERE lang_id = %d AND name LIKE '%%%s%%'", lang_id, text.c_str());

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {


        mj::model::LessonInfo info;

        info.lesson_id = SQLITE_COLUMN_INT(0);
        info.lesson_order = SQLITE_COLUMN_INT(1);
        info.lang_id = SQLITE_COLUMN_INT(2);
        info.category_id = SQLITE_COLUMN_INT(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        info.name = SQLITE_COLUMN_STR(5);
        info.is_learned = SQLITE_COLUMN_INT(6);
        info.time_published = SQLITE_COLUMN_INT(7);
        info.state = mj::model::LessonInfo::State::UNLOCKED;

        ret.push_back(info);
    }

    if (has_apostrophe)
    {
        apostrophe_found = text.find("’", 0);
        while (apostrophe_found != std::string::npos)
        {
            text.replace(apostrophe_found, 3, "''");
            apostrophe_found = text.find("’", apostrophe_found + 2);
        }

        query = StringUtils::format("SELECT * FROM tbl_lesson_v2 WHERE lang_id = %d AND name LIKE '%%%s%%'", lang_id, text.c_str());

        sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            mj::model::LessonInfo info;

            info.lesson_id = SQLITE_COLUMN_INT(0);
            info.lesson_order = SQLITE_COLUMN_INT(1);
            info.lang_id = SQLITE_COLUMN_INT(2);
            info.category_id = SQLITE_COLUMN_INT(3);
            info.thumbnail_url = SQLITE_COLUMN_STR(4);
            info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
            info.name = SQLITE_COLUMN_STR(5);
            info.is_learned = SQLITE_COLUMN_INT(6);
            info.time_published = SQLITE_COLUMN_INT(7);
            info.state = mj::model::LessonInfo::State::UNLOCKED;

            ret.push_back(info);
        }
    }
    return ret;

}


/*----------------Query data for aiSpeaking ----------------*/
void StoryDataModel::insertAiSpeakingTopic(std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingTopicInfos) {
    if (aISpeakingTopicInfos.size() <= 0)
        return;
    std::string query = "replace into tbl_ai_speaking_topic " + mj::model::AISpeakingCategoryInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(aISpeakingTopicInfos), std::end(aISpeakingTopicInfos), query, [](std::string q, mj::model::AISpeakingCategoryInfo i) {
        return q.append(mj::model::AISpeakingCategoryInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto aISpeakingTopicInfo : aISpeakingTopicInfos) {

        //(_id, _order, lesson_id, story_id, type, name, content, resource_zip, data_zip)
                 /*     
                 "topic_id integer not null,"
                "topic_name text,"
                "thumb_url text,"
                "free integer,"
                "status integer,"
                "cat_version integer,"
                "order_by integer,"
                "icon_zip_url text,"
                "icon_thumb text,"
                "check_icon text,"
                "disable_icon text,"
                "progress_bg_icon text,"
                "selected_circle_icon text,"
                "design_icon text,"
                "primary key (topic_id))";
            */
        SQLITE_BIND_INT(++counter, aISpeakingTopicInfo.topic_id);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.topic_name);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.thumb_url);
        SQLITE_BIND_INT(++counter, aISpeakingTopicInfo.free);
        SQLITE_BIND_INT(++counter, aISpeakingTopicInfo.status);
        SQLITE_BIND_INT(++counter, aISpeakingTopicInfo.cat_version);
        SQLITE_BIND_INT(++counter, aISpeakingTopicInfo.order_by);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.icon_zip_url);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.icon_thumb);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.check_icon);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.disable_icon);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.progress_bg_icon);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.selected_circle_icon);
        SQLITE_BIND_STR(++counter, aISpeakingTopicInfo.iconInfo.design_icon);

    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert ai speaking topic done");
}

bool StoryDataModel::removeAiSpeakingTopicById(std::vector<int> topicIDs)
{
    if (topicIDs.size() <= 0)
        return true;
    bool resultDeleteDB = false, resultDeleteAiSpeakingItems = false;

    // delete row in tbl_ai_speaking_topic table in topic_id
    std::string topicIDListStr = mj::model::ConvertArrayIntergerToString(topicIDs);
    auto query = StringUtils::format("delete from tbl_ai_speaking_topic where topic_id in (%s)", topicIDListStr.c_str());

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove tbl_ai_speaking_topic of topicIDs %s", topicIDListStr.c_str());
        resultDeleteDB = true;
    }
    for (int topicID : topicIDs) {
        resultDeleteAiSpeakingItems = removeAiSpeakingLessonByTopicId(topicID);

    }

    return resultDeleteAiSpeakingItems & resultDeleteDB;
}

bool StoryDataModel::deleteAiSpeakingTopicTable()
{
    return false;
}

mj::model::AISpeakingCategoryInfo StoryDataModel::getAiSpeakingTopicById(int topicID)
{

    mj::model::AISpeakingCategoryInfo aISpeakingTopicInfo;

    auto query = StringUtils::format("select * from tbl_ai_speaking_topic where topic_id = %d", topicID);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return aISpeakingTopicInfo;
    }

    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        aISpeakingTopicInfo.topic_id = SQLITE_COLUMN_INT(0);
        aISpeakingTopicInfo.topic_name = SQLITE_COLUMN_STR(1);
        aISpeakingTopicInfo.thumb_url = SQLITE_COLUMN_STR(2);
        aISpeakingTopicInfo.free = SQLITE_COLUMN_INT(3);
        aISpeakingTopicInfo.status = SQLITE_COLUMN_INT(4);
        aISpeakingTopicInfo.cat_version = SQLITE_COLUMN_INT(5);
        aISpeakingTopicInfo.order_by = SQLITE_COLUMN_INT(6);
        aISpeakingTopicInfo.icon_zip_url = SQLITE_COLUMN_STR(7);
        aISpeakingTopicInfo.icon_zip_url = mj::helper::getUrlFromDomain(aISpeakingTopicInfo.icon_zip_url, mj::UrlType::MSURL_AI_SPEAKING_CATEGORY_ICON);
        aISpeakingTopicInfo.iconInfo.icon_thumb = SQLITE_COLUMN_STR(8);
        aISpeakingTopicInfo.iconInfo.check_icon = SQLITE_COLUMN_STR(9);
        aISpeakingTopicInfo.iconInfo.disable_icon = SQLITE_COLUMN_STR(10);
        aISpeakingTopicInfo.iconInfo.progress_bg_icon = SQLITE_COLUMN_STR(11);
        aISpeakingTopicInfo.iconInfo.selected_circle_icon = SQLITE_COLUMN_STR(12);
        aISpeakingTopicInfo.iconInfo.design_icon = SQLITE_COLUMN_STR(13);

    }

    return aISpeakingTopicInfo;
}

std::vector<mj::model::AISpeakingCategoryInfo> StoryDataModel::getAllAiSpeakingTopic()
{
    std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingTopicInfos;
    auto query = StringUtils::format("select * from tbl_ai_speaking_topic");


    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return aISpeakingTopicInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::AISpeakingCategoryInfo aISpeakingTopicInfo;
        aISpeakingTopicInfo.topic_id = SQLITE_COLUMN_INT(0);
        aISpeakingTopicInfo.topic_name = SQLITE_COLUMN_STR(1);
        aISpeakingTopicInfo.thumb_url = SQLITE_COLUMN_STR(2);
        aISpeakingTopicInfo.free = SQLITE_COLUMN_INT(3);
        aISpeakingTopicInfo.status = SQLITE_COLUMN_INT(4);
        aISpeakingTopicInfo.cat_version = SQLITE_COLUMN_INT(5);
        aISpeakingTopicInfo.order_by = SQLITE_COLUMN_INT(6);
        aISpeakingTopicInfo.icon_zip_url = SQLITE_COLUMN_STR(7);
        aISpeakingTopicInfo.icon_zip_url = mj::helper::getUrlFromDomain(aISpeakingTopicInfo.icon_zip_url, mj::UrlType::MSURL_AI_SPEAKING_CATEGORY_ICON);
        aISpeakingTopicInfo.iconInfo.icon_thumb = SQLITE_COLUMN_STR(8);
        aISpeakingTopicInfo.iconInfo.check_icon = SQLITE_COLUMN_STR(9);
        aISpeakingTopicInfo.iconInfo.disable_icon = SQLITE_COLUMN_STR(10);
        aISpeakingTopicInfo.iconInfo.progress_bg_icon = SQLITE_COLUMN_STR(11);
        aISpeakingTopicInfo.iconInfo.selected_circle_icon = SQLITE_COLUMN_STR(12);
        aISpeakingTopicInfo.iconInfo.design_icon = SQLITE_COLUMN_STR(13);

        aISpeakingTopicInfos.push_back(aISpeakingTopicInfo);
    }


    return aISpeakingTopicInfos;
}

std::vector<mj::model::AISpeakingLessonInfo> StoryDataModel::getAllLessonsAISpeakingByLanguage(int lang_id)
{
    std::string query = StringUtils::format("select * from tbl_ai_speaking_lessons order by _order asc");
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);

    std::vector<mj::model::AISpeakingLessonInfo> r;

    std::map<int, int> cat_count;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AISpeakingLessonInfo info;
        info.lesson_id = SQLITE_COLUMN_INT(0);
        info.lesson_order = SQLITE_COLUMN_INT(1);
        info.lang_id = SQLITE_COLUMN_INT(2);
        info.category_id = SQLITE_COLUMN_INT(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        info.name = SQLITE_COLUMN_STR(5);
        info.state = (mj::model::AISpeakingLessonInfo::State)SQLITE_COLUMN_INT(6);
        info.time_published = SQLITE_COLUMN_INT(7);
        info._timeLearnedExpected = SQLITE_COLUMN_INT(8);
        if(info.state == mj::model::AISpeakingLessonInfo::State::LEARNED)
            info.is_learned = true;
        else
        {
            info.is_learned = false;
        }
        info.time_published = SQLITE_COLUMN_INT(7);

        if (info.category_id == 479 || info.category_id == 480 || (info.category_id == 481 && (info.lesson_order > 0 && info.lesson_order < 15)))
        {
            CCLOG("/%d/ : %d, ", info.lesson_id, info.category_id);
        }

        if (cat_count.find(info.category_id) != cat_count.end()) {
            info.lesson_order = cat_count[info.category_id];
        }
        else {
            info.lesson_order = 1;
        }
        cat_count[info.category_id] = info.lesson_order + 1;

        r.push_back(info);
    }
    return r;
}

void StoryDataModel::setLessonAISpeakingLearned(std::vector<int> lesson_ids)
{
    if (lesson_ids.size() <= 0)
        return;
    int timeInday = mj::helper::getTimeInDays();
    std::string query = "update tbl_ai_speaking_lessons set learned = 1 where _id in(";
    query = std::accumulate(lesson_ids.begin(), lesson_ids.end(), query, [](std::string q, int) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query.append(")");
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    auto bind_counter = 0;
    for (auto i : lesson_ids) {
        SQLITE_BIND_INT(++bind_counter, i);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}
void StoryDataModel::setLessonAISpeakingLearned(std::vector<std::pair<std::string, int>> listLessonLearn)
{
    if (listLessonLearn.size() > 100) {
        auto first = 0;
        const auto chunk_size = 100;
        while (first < listLessonLearn.size()) {
            auto last = std::min(first + chunk_size, (int)listLessonLearn.size());
            setLessonLearned(std::vector<std::pair<std::string, int>>(std::begin(listLessonLearn) + first, std::begin(listLessonLearn) + last));
            first = last;
        }

        return;
    }
    int timeInday = mj::helper::getTimeInDays();
    std::string query = "update tbl_ai_speaking_lessons set learned = 1 where _id in (";
    query = std::accumulate(listLessonLearn.begin(), listLessonLearn.end(), query, [](std::string q, std::pair<std::string, int> i) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query.append(")");
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto bind_counter = 0;
    for (auto i : listLessonLearn) {
        SQLITE_BIND_INT(++bind_counter, std::stoi(i.first));
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}
ValueVector StoryDataModel::getLessonAISpeakingLearned()
{
    std::string query = "select _id, category_id, learned from tbl_ai_speaking_lessons where learned = 1";
    return fetchAllRecord(query);
}

void StoryDataModel::resetLessonAISpeakingLearned()
{
    std::string query = "update tbl_ai_speaking_lessons set learned = 0";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}

std::vector<int> StoryDataModel::getDownloadedLessonsAISpeaking()
{
    std::vector<int> lessons;
    std::string query = "select _id, _order, category_id from tbl_ai_speaking_lessons inner join (select lesson_id from tbl_activity group by lesson_id) t on tbl_ai_speaking_lessons._id = t.lesson_id order by category_id, _order asc";
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return lessons;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        lessons.push_back(SQLITE_COLUMN_INT(0));
    }
    return lessons;
}

std::vector<int> StoryDataModel::getLessonLearnAISpeakingByID(int lessonId)
{
    std::vector <int> valueLessonLearn = {};

    try
    {
        std::string  query = StringUtils::format("select  _id, category_id, learned from tbl_ai_speaking_lessons where _id=%d AND learned = 1", lessonId);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            return valueLessonLearn;
        };

        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 0));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 1));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 2));
        }
    }
    catch (const std::exception& e)
    {

    }

    return valueLessonLearn;
}

std::vector<int> StoryDataModel::getLessonStateAISpeakingByID(int lessonId)
{
    std::vector <int> valueLessonLearn = {};

    try
    {
        std::string  query = StringUtils::format("select  _id, category_id, learned from tbl_ai_speaking_lessons where _id=%d", lessonId);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            return valueLessonLearn;
        };

        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 0));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 1));
            valueLessonLearn.push_back(sqlite3_column_int(this->stmp, 2));
        }
    }
    catch (const std::exception& e)
    {

    }

    return valueLessonLearn;
}

mj::model::AISpeakingLessonInfo StoryDataModel::getLessonAISpeakingByID(int lessonId)
{
    mj::model::AISpeakingLessonInfo  aISpeakingItemInfo;
    std::string  query = StringUtils::format("select * from tbl_ai_speaking_lessons where _id=%d", lessonId);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return aISpeakingItemInfo;
    }

    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        aISpeakingItemInfo.lesson_id = SQLITE_COLUMN_INT(0);
        aISpeakingItemInfo.lesson_order = SQLITE_COLUMN_INT(1);
        aISpeakingItemInfo.lang_id = SQLITE_COLUMN_INT(2);
        aISpeakingItemInfo.category_id = SQLITE_COLUMN_INT(3);
        aISpeakingItemInfo.thumbnail_url = SQLITE_COLUMN_STR(4);
        aISpeakingItemInfo.thumbnail_url = mj::helper::getUrlFromDomain(aISpeakingItemInfo.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        aISpeakingItemInfo.name = SQLITE_COLUMN_STR(5);
        aISpeakingItemInfo.state = (mj::model::AISpeakingLessonInfo::State)SQLITE_COLUMN_INT(6);
        aISpeakingItemInfo.time_published = SQLITE_COLUMN_INT(7);
        aISpeakingItemInfo._timeLearnedExpected = SQLITE_COLUMN_INT(8);
        if(aISpeakingItemInfo.state == mj::model::AISpeakingLessonInfo::LEARNED)
            aISpeakingItemInfo.is_learned = true;
        else
        {
            aISpeakingItemInfo.is_learned = false;
        }
        aISpeakingItemInfo.time_published = SQLITE_COLUMN_INT(7);
    }

    return aISpeakingItemInfo;
}

void StoryDataModel::UpdateLessonStateAISpeaking(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo)
{
    std::string query = StringUtils::format("update tbl_ai_speaking_lessons set learned = %d  where _id=%d ", aISpeakingLessonInfo.state, aISpeakingLessonInfo.lesson_id);
    if (aISpeakingLessonInfo.state == mj::model::AISpeakingLessonInfo::State::READY)
    {
        int timeInday = mj::helper::getTimeInDays();
        query = StringUtils::format("update tbl_ai_speaking_lessons set learned = %d, timeLearnedExpected =%d where _id=%d ", aISpeakingLessonInfo.state, timeInday, aISpeakingLessonInfo.lesson_id);
    }
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    auto bind_counter = 0;
    SQLITE_BIND_INT(++bind_counter, aISpeakingLessonInfo.lesson_id);

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
}


std::vector<mj::model::AISpeakingLessonInfo> StoryDataModel::searchAISpeakingLessonByLanguage(int lang_id, std::string text)
{
    std::vector<mj::model::AISpeakingLessonInfo> ret;
    text = mj::helper::to_no_accents_v2(text);
    bool is_IOS = false;
    bool has_apostrophe = false;
    size_t apostrophe_found;

    if (text.find("’", 0) != std::string::npos)
    {
        is_IOS = true;
        has_apostrophe = true;
    }

    if (!is_IOS)
    {
        apostrophe_found = text.find("'", 0);
        if (apostrophe_found != std::string::npos)
            has_apostrophe = true;

        while (apostrophe_found != std::string::npos)
        {
            text.replace(apostrophe_found, 1, "’");
            apostrophe_found = text.find("'", apostrophe_found + 3);
        }
    }

    std::string query = StringUtils::format("SELECT * FROM tbl_ai_speaking_lessons WHERE lang_id = %d AND name LIKE '%%%s%%'", lang_id, text.c_str());

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {


        mj::model::AISpeakingLessonInfo info;

        info.lesson_id = SQLITE_COLUMN_INT(0);
        info.lesson_order = SQLITE_COLUMN_INT(1);
        info.lang_id = SQLITE_COLUMN_INT(2);
        info.category_id = SQLITE_COLUMN_INT(3);
        info.thumbnail_url = SQLITE_COLUMN_STR(4);
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        info.name = SQLITE_COLUMN_STR(5);
        info.is_learned = SQLITE_COLUMN_INT(6);
        info.time_published = SQLITE_COLUMN_INT(7);
        info._timeLearnedExpected = SQLITE_COLUMN_INT(8);
        info.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;

        ret.push_back(info);
    }

    if (has_apostrophe)
    {
        apostrophe_found = text.find("’", 0);
        while (apostrophe_found != std::string::npos)
        {
            text.replace(apostrophe_found, 3, "''");
            apostrophe_found = text.find("’", apostrophe_found + 2);
        }

        query = StringUtils::format("SELECT * FROM tbl_ai_speaking_lessons WHERE lang_id = %d AND name LIKE '%%%s%%'", lang_id, text.c_str());

        sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            mj::model::AISpeakingLessonInfo info;

            info.lesson_id = SQLITE_COLUMN_INT(0);
            info.lesson_order = SQLITE_COLUMN_INT(1);
            info.lang_id = SQLITE_COLUMN_INT(2);
            info.category_id = SQLITE_COLUMN_INT(3);
            info.thumbnail_url = SQLITE_COLUMN_STR(4);
            info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
            info.name = SQLITE_COLUMN_STR(5);
            info.is_learned = SQLITE_COLUMN_INT(6);
            info.time_published = SQLITE_COLUMN_INT(7);
            info._timeLearnedExpected = SQLITE_COLUMN_INT(8);
            info.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;

            ret.push_back(info);
        }
    }
    return ret;
}

std::vector<mj::model::AISpeakingLessonInfo> StoryDataModel::getAiSpeakingLessonByTopicId(int topicID)
{
   std::vector<mj::model::AISpeakingLessonInfo> aISpeakingItemInfos;
   auto query = StringUtils::format("select * from tbl_ai_speaking_lessons where category_id = %d", topicID);

   if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
       logError();
       return aISpeakingItemInfos;
   }

   while (sqlite3_step(this->stmp) == SQLITE_ROW) {
       mj::model::AISpeakingLessonInfo aISpeakingItemInfo;
       /*
        * "create table if not exists tbl_ai_speaking_lessons ("
        "_id integer not null,"
        " _order integer,"
        " lang_id integer,"
        " category_id integer,"
        " thumb_url text, 
        " name text,"
        " learned integer DEFAULT(0),"
        " time_published integer,"
        " primary key (_id))";
        */
       aISpeakingItemInfo.lesson_id = SQLITE_COLUMN_INT(0);
       aISpeakingItemInfo.lesson_order = SQLITE_COLUMN_INT(1);
       aISpeakingItemInfo.lang_id = SQLITE_COLUMN_INT(2);
       aISpeakingItemInfo.category_id = SQLITE_COLUMN_INT(3);
       aISpeakingItemInfo.thumbnail_url = SQLITE_COLUMN_STR(4);
       aISpeakingItemInfo.name = SQLITE_COLUMN_STR(5);
       aISpeakingItemInfo.is_learned = SQLITE_COLUMN_INT(6);
       aISpeakingItemInfo.time_published = SQLITE_COLUMN_INT(7);
       aISpeakingItemInfo._timeLearnedExpected = SQLITE_COLUMN_INT(8);
       aISpeakingItemInfos.push_back(aISpeakingItemInfo);
   }

   return aISpeakingItemInfos;
}

std::vector<mj::model::AISpeakingLessonInfo> StoryDataModel::getAllAiSpeakingLessonItems()
{
    std::vector<mj::model::AISpeakingLessonInfo> aISpeakingItemInfos;
    auto query = StringUtils::format("select * from tbl_ai_speaking_lessons");

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return aISpeakingItemInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AISpeakingLessonInfo aISpeakingItemInfo;
        aISpeakingItemInfo.lesson_id = SQLITE_COLUMN_INT(0);
        aISpeakingItemInfo.lesson_order = SQLITE_COLUMN_INT(1);
        aISpeakingItemInfo.lang_id = SQLITE_COLUMN_INT(2);
        aISpeakingItemInfo.category_id = SQLITE_COLUMN_INT(3);
        aISpeakingItemInfo.thumbnail_url = SQLITE_COLUMN_STR(4);
        aISpeakingItemInfo.thumbnail_url = mj::helper::getUrlFromDomain(aISpeakingItemInfo.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        aISpeakingItemInfo.name = SQLITE_COLUMN_STR(5);
        aISpeakingItemInfo.state = mj::model::AISpeakingLessonInfo::State::LOCKED;
        aISpeakingItemInfo.is_learned = SQLITE_COLUMN_INT(6);
        aISpeakingItemInfo.time_published = SQLITE_COLUMN_INT(7);
        aISpeakingItemInfo._timeLearnedExpected = SQLITE_COLUMN_INT(8);
        aISpeakingItemInfos.push_back(aISpeakingItemInfo);
    }

    return aISpeakingItemInfos;
}

void StoryDataModel::insertAiSpeakingLessons(std::vector<mj::model::AISpeakingLessonInfo> aISpeakingItemInfos)
{
    if (aISpeakingItemInfos.size() <= 0)
        return;
    std::string query = "replace into tbl_ai_speaking_lessons " + mj::model::AISpeakingLessonInfo::getLessonAISpeakingSQLStructure() + " values ";
    query = std::accumulate(std::begin(aISpeakingItemInfos), std::end(aISpeakingItemInfos), query, [](std::string q, mj::model::AISpeakingLessonInfo i) {
        return q.append(mj::model::AISpeakingLessonInfo::getLessonAISpeakingSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto aISpeakingItemInfo : aISpeakingItemInfos) {
        /*
         * "create table if not exists tbl_ai_speaking_lessons ("
         "_id integer not null,"
         " _order integer,"
         " lang_id integer,"
         " category_id integer,"
         " thumb_url text,
         " name text,"
         " learned integer DEFAULT(0),"
         " time_published integer,"
         " primary key (_id))";
         */
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo.lesson_id);
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo.lesson_order);
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo.lang_id);
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo.category_id);
        SQLITE_BIND_STR(++counter, aISpeakingItemInfo.name);
        SQLITE_BIND_STR(++counter, aISpeakingItemInfo.thumbnail_url);
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo.time_published);
        SQLITE_BIND_INT(++counter, aISpeakingItemInfo._timeLearnedExpected);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert ai speaking topic done");
}

std::vector < mj::model::AISpeakingLessonInfo> StoryDataModel::getAiSpeakingLessonByIds(std::vector<int> iteamIds)
{

    std::vector < mj::model::AISpeakingLessonInfo> aISpeakingItemInfos;

    if (iteamIds.size() <= 0)
    {
        return aISpeakingItemInfos;
    }
    std::string itemsIDListStr = mj::model::ConvertArrayIntergerToString(iteamIds);

    auto query = StringUtils::format("select * from tbl_ai_speaking_lessons where _id in (%s)", itemsIDListStr.c_str());

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return aISpeakingItemInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::AISpeakingLessonInfo  aISpeakingItemInfo;
        aISpeakingItemInfo.lesson_id = SQLITE_COLUMN_INT(0);
        aISpeakingItemInfo.lesson_order = SQLITE_COLUMN_INT(1);
        aISpeakingItemInfo.lang_id = SQLITE_COLUMN_INT(2);
        aISpeakingItemInfo.category_id = SQLITE_COLUMN_INT(3);
        aISpeakingItemInfo.thumbnail_url = SQLITE_COLUMN_STR(4);
        aISpeakingItemInfo.thumbnail_url = mj::helper::getUrlFromDomain(aISpeakingItemInfo.thumbnail_url, mj::UrlType::MSURL_ACTIVITIES);
        aISpeakingItemInfo.name = SQLITE_COLUMN_STR(5);
        aISpeakingItemInfo.state = mj::model::AISpeakingLessonInfo::State::LOCKED;
        aISpeakingItemInfo.is_learned = SQLITE_COLUMN_INT(6);
        aISpeakingItemInfo.time_published = SQLITE_COLUMN_INT(7);
        aISpeakingItemInfo._timeLearnedExpected = SQLITE_COLUMN_INT(8);

        aISpeakingItemInfos.push_back(aISpeakingItemInfo);
    }

    return aISpeakingItemInfos;
}

bool StoryDataModel::removeAiSpeakingLessonByIds(std::vector<int> iteamIds)
{
    if (iteamIds.size() <= 0)
    {
        return true;
    }
    bool resultDeleteDB = false, resultDeleteFile = false;
    auto aiSpeakingItem = getAiSpeakingLessonByIds(iteamIds);
    auto writable = FileUtils::getInstance()->getWritablePath();
    // delete row in tbl_ai_speaking_topic table in topic_id
    std::string itemsIDListStr = mj::model::ConvertArrayIntergerToString(iteamIds);
    auto query = StringUtils::format("delete from tbl_ai_speaking_lessons where _id in (%s)", itemsIDListStr.c_str());

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove tbl_ai_speaking_lessons of iteamId %s", itemsIDListStr.c_str());
        resultDeleteDB = true;
    }

    /*std::vector <std::string> subFolders;
    std::string imageCacheFolder = "cached/zip/";
    std::vector < std::string > fileList = FileUtils::getInstance()->listFiles(FileUtils::getInstance()->getWritablePath() + imageCacheFolder);
    for (std::string item : fileList) {
        if (FileUtils::getInstance()->isFileExist(item))
            subFolders.push_back(item);
    }
    if (subFolders.size() <= 0)
    {
        resultDeleteFile = true;
    }
    else
    {
        auto zip = aiSpeakingItem.;
        zip = mj::helper::getFilenameFromURL(zip);
        CCLOG("removing file %s", (writable + "cached/zip/" + zip).c_str());
        auto removed = FileUtils::getInstance()->removeFile(writable + "cached/zip/" + zip);
        if (removed) {
            CCLOG("removed %s", zip.c_str());
        }
        else
        {
            resultDeleteFile = false;
        }

      
    }*/

   // FileUtils::getInstance()->purgeCachedEntries();
    resultDeleteFile = true;
    return resultDeleteFile& resultDeleteDB;
}

bool StoryDataModel::removeAiSpeakingLessonByTopicId(int topicID)
{
    std::vector<int> aiSpeakingItemsIDs;
    bool resultDeleteDB = false, resultDeleteFiles = true;
    auto writable = FileUtils::getInstance()->getWritablePath();

    auto aiSpeakingItems = getAiSpeakingLessonByTopicId(topicID);
    for (auto aiSpeakingItem : aiSpeakingItems) {
        aiSpeakingItemsIDs.push_back(aiSpeakingItem.lesson_id);
    }

    std::string aiSpeakingItemsIDsStr = mj::model::ConvertArrayIntergerToString(aiSpeakingItemsIDs);
    // delete rows in tbl_ai_speaking_topic table in item_id array
    auto query = StringUtils::format("delete from tbl_ai_speaking_lessons where _id in (%s)", aiSpeakingItemsIDsStr.c_str());

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove tbl_ai_speaking_lessons of item_id %s", aiSpeakingItemsIDsStr.c_str());
        resultDeleteDB = true;
    }

    //std::vector <std::string> subFolders;
    //std::string imageCacheFolder = "cached/zip/";
    //std::vector < std::string > fileList = FileUtils::getInstance()->listFiles(FileUtils::getInstance()->getWritablePath() + imageCacheFolder);
    //for (std::string item : fileList) {
    //    if (FileUtils::getInstance()->isFileExist(item))
    //        subFolders.push_back(item);
    //}
    //if (subFolders.size() <= 0)
    //{
    //    resultDeleteFiles = true;
    //}
    //else
    //{
    //    for (auto activity : aiSpeakingItems) {
    //        auto zip = activity.url_download;
    //        zip = mj::helper::getFilenameFromURL(zip);
    //        CCLOG("removing file %s", (writable + "cached/zip/" + zip).c_str());
    //        auto removed = FileUtils::getInstance()->removeFile(writable + "cached/zip/" + zip);
    //        if (removed) {
    //            CCLOG("removed %s", zip.c_str());
    //        }
    //        else
    //        {
    //            resultDeleteFiles = false;
    //        }

    //    }
    //}

    //FileUtils::getInstance()->purgeCachedEntries();
    resultDeleteFiles = true;
    return resultDeleteFiles& resultDeleteDB;
}

bool StoryDataModel::deleteAiSpeakingLessonTable()
{
    return false;
}

void StoryDataModel::insertSpeakingAIActivities(std::vector<mj::model::ActAISpeakingInfo> activities)
{
    if (activities.size() <= 0)
        return;
    std::vector<mj::model::ActInfo> activities_;
    for (auto activity: activities)
    {
        activities_.push_back(mj::model::ActInfo(activity));
    }
    insertActivities(activities_);

}

std::vector<mj::model::ActAISpeakingInfo> StoryDataModel::getActivitiesOfSpeakingAILesson(int lesson_id, QuerySelectType type)
{
    std::vector<mj::model::ActInfo> actInfo = getActivitiesOfLessonNew(lesson_id, type);
    std::vector<mj::model::ActAISpeakingInfo> activities;
    for (auto activity : actInfo)
    {
        activities.push_back(mj::model::ActAISpeakingInfo(activity));
    }
    return activities;
}

mj::model::ActAISpeakingInfo StoryDataModel::getSpeakingAIActivityById(int activity_id)
{
    mj::model::ActInfo actInfo = getActivityById(activity_id);

    return mj::model::ActAISpeakingInfo(actInfo);
}

void StoryDataModel::removeSpeakingAIActivitiesOfLesson(int lesson_id)
{
    removeActivitiesOfLesson(lesson_id);
}

void StoryDataModel::removeAllSpeakingAIActivities()
{
    //removeAllActivities();
}

void StoryDataModel::insertAiSpeakingCommonInfo(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos)
{
    if (talkingCommonInfos.size() <= 0)
        return;
    std::string query = "replace into tbl_ai_speaking_common " + mj::model::TalkingCommonInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(talkingCommonInfos), std::end(talkingCommonInfos), query, [](std::string q, mj::model::TalkingCommonInfo i) {
        return q.append(mj::model::TalkingCommonInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto talkingCommonInfo : talkingCommonInfos) {

        /*            int id = 0;
            std::string actor_name = "";
            std::string version_name = "";
            std::string zipPath = "";
            std::string intro = "";
            std::string intro_mimic = "";
            std::string intro_talking = "";
            std::string your_turn = "";
            std::string cheer_up = "";
            std::string listening = "";
            std::string outro_mimic = "";
            std::string outro = "";
            */

        //SQLITE_BIND_INT(++counter, NULL);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.actor_name);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.version_name);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.zipPath);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.intro);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.intro_mimic);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.intro_talking);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.your_turn);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.cheer_up);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.listening);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.outro_mimic);
        SQLITE_BIND_STR(++counter, talkingCommonInfo.outro);

    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert ai speaking topic done");
}

std::vector<mj::model::TalkingCommonInfo> StoryDataModel::getAllAiSpeakingCommonInfos()
{
    std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos;
    auto query = StringUtils::format("select * from tbl_ai_speaking_common");

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return talkingCommonInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::TalkingCommonInfo talkingCommonInfo;
        talkingCommonInfo.id = SQLITE_COLUMN_INT(0);
        talkingCommonInfo.actor_name = SQLITE_COLUMN_STR(1);
        talkingCommonInfo.version_name = SQLITE_COLUMN_STR(2);
        talkingCommonInfo.zipPath = SQLITE_COLUMN_STR(3);
        talkingCommonInfo.intro = SQLITE_COLUMN_STR(4);
        talkingCommonInfo.intro_mimic = SQLITE_COLUMN_STR(5);;
        talkingCommonInfo.intro_talking = SQLITE_COLUMN_STR(6);
        talkingCommonInfo.your_turn = SQLITE_COLUMN_STR(7);
        talkingCommonInfo.cheer_up = SQLITE_COLUMN_STR(8);
        talkingCommonInfo.listening = SQLITE_COLUMN_STR(9);
        talkingCommonInfo.outro_mimic = SQLITE_COLUMN_STR(10);
        talkingCommonInfo.outro = SQLITE_COLUMN_STR(11);
        talkingCommonInfos.push_back(talkingCommonInfo);
    }
    return talkingCommonInfos;
}


std::vector<mj::model::TalkingCommonInfo> StoryDataModel::getAiSpeakingCommonInfosByActor(std::vector<std::string> actors)
{
    std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos;

    if (actors.size() <= 0)
    {
        return talkingCommonInfos;
    }
    std::string itemsIDListStr = mj::model::ConvertArrayStringToString(actors);

    itemsIDListStr = mj::helper::to_lower(itemsIDListStr);
    auto query = StringUtils::format("select * from tbl_ai_speaking_common where actor_name in (%s)", itemsIDListStr.c_str());

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return talkingCommonInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::TalkingCommonInfo talkingCommonInfo;
        talkingCommonInfo.id = SQLITE_COLUMN_INT(0);
        talkingCommonInfo.actor_name = SQLITE_COLUMN_STR(1);
        talkingCommonInfo.version_name = SQLITE_COLUMN_STR(2);
        talkingCommonInfo.zipPath = SQLITE_COLUMN_STR(3);
        talkingCommonInfo.intro = SQLITE_COLUMN_STR(4);
        talkingCommonInfo.intro_mimic = SQLITE_COLUMN_STR(5);;
        talkingCommonInfo.intro_talking = SQLITE_COLUMN_STR(6);
        talkingCommonInfo.your_turn = SQLITE_COLUMN_STR(7);
        talkingCommonInfo.cheer_up = SQLITE_COLUMN_STR(8);
        talkingCommonInfo.listening = SQLITE_COLUMN_STR(9);
        talkingCommonInfo.outro_mimic = SQLITE_COLUMN_STR(10);
        talkingCommonInfo.outro = SQLITE_COLUMN_STR(11);
        talkingCommonInfos.push_back(talkingCommonInfo);
    }
    return talkingCommonInfos;
}

std::vector<mj::model::TalkingCommonInfo> StoryDataModel::getAiSpeakingCommonInfosByActor(std::string actor)
{
    std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos;
    actor = mj::helper::to_lower(actor);
    auto query = StringUtils::format("select * from tbl_ai_speaking_common where actor_name = '%s'", actor.c_str());
   // auto query = "select * from tbl_ai_speaking_common where actor_name = '" + actor + "'";
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return talkingCommonInfos;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::TalkingCommonInfo talkingCommonInfo;
        talkingCommonInfo.id = SQLITE_COLUMN_INT(0);
        talkingCommonInfo.actor_name = SQLITE_COLUMN_STR(1);
        talkingCommonInfo.version_name = SQLITE_COLUMN_STR(2);
        talkingCommonInfo.zipPath = SQLITE_COLUMN_STR(3);
        talkingCommonInfo.intro = SQLITE_COLUMN_STR(4);
        talkingCommonInfo.intro_mimic = SQLITE_COLUMN_STR(5);;
        talkingCommonInfo.intro_talking = SQLITE_COLUMN_STR(6);
        talkingCommonInfo.your_turn = SQLITE_COLUMN_STR(7);
        talkingCommonInfo.cheer_up = SQLITE_COLUMN_STR(8);
        talkingCommonInfo.listening = SQLITE_COLUMN_STR(9);
        talkingCommonInfo.outro_mimic = SQLITE_COLUMN_STR(10);
        talkingCommonInfo.outro = SQLITE_COLUMN_STR(11);
        talkingCommonInfos.push_back(talkingCommonInfo);
    }
    return talkingCommonInfos;
}

bool StoryDataModel::removeAiSpeakingCommonInfoById(std::vector<int> commonInfoIds)
{
    if (commonInfoIds.size() <= 0)
        return true;
    bool resultDeleteDB = false, resultDeleteFile = false;

    // delete row in tbl_ai_speaking_topic table in topic_id
    std::string commonInfoIdStr = mj::model::ConvertArrayIntergerToString(commonInfoIds);
    auto query = StringUtils::format("delete from tbl_ai_speaking_common where _id in (%s)", commonInfoIdStr.c_str());

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove tbl_ai_speaking_topic of _id %s", commonInfoIdStr.c_str());
        resultDeleteDB = true;
    }

    return resultDeleteFile & resultDeleteDB;
}

bool StoryDataModel::removeAiSpeakingCommonInfoByActor(std::vector<std::string> actors)
{
    if (actors.size() <= 0)
        return true;

    bool resultDeleteDB = false, resultDeleteFile = false;

    // delete row in tbl_ai_speaking_topic table in topic_id
    std::string commonInfoActorsStr = mj::model::ConvertArrayStringToString(actors);
    commonInfoActorsStr = mj::helper::to_lower(commonInfoActorsStr);
    auto query = StringUtils::format("delete from tbl_ai_speaking_common where actor_name in (%s)", commonInfoActorsStr.c_str());

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove tbl_ai_speaking_common of actor_name %s", commonInfoActorsStr.c_str());
        resultDeleteDB = true;
    }

    return resultDeleteFile & resultDeleteDB;
}

void StoryDataModel::updateAiSpeakingCommonInfo(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos)
{
    if (talkingCommonInfos.size() <= 0)
        return;
}

bool StoryDataModel::deleteAiSpeakingCommonInfoTable()
{
    // we are need delete both info from DB and file from store

    // delete from db
    bool resultDeleteFiles = false, resultDeleteDB = false;
    auto query = StringUtils::format("delete from tbl_ai_speaking_common");
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove all activities");
        resultDeleteDB = true;
    }

    //delete file from store

    std::vector <std::string> subFolders;
    std::string video_cache_folder = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING;
    std::string video_cache_zip_folder = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING;
    try
    {
        if (FileUtils::getInstance()->isDirectoryExist(video_cache_folder))
            auto removed = FileUtils::getInstance()->removeDirectory(video_cache_folder);
        if (FileUtils::getInstance()->isDirectoryExist(video_cache_zip_folder))
            auto removed = FileUtils::getInstance()->removeDirectory(video_cache_zip_folder);
        resultDeleteFiles = true;
    }
    catch (const std::exception&)
    {
        CCLOG("Can't remove video common folders");
        resultDeleteFiles = false;

    }

    FileUtils::getInstance()->purgeCachedEntries();
    return resultDeleteFiles && resultDeleteDB;
}

/*----------------------------------------------------------*/

/*--------------------handler for f2P trial data-----------------------*/
void StoryDataModel::insertFreeTrialInfo(std::vector<mj::model::FreeItemInfo> freeItemInfos)
{
    if (freeItemInfos.size() <= 0)
        return;
    std::string query = "replace into tbl_free_trial " + mj::model::FreeItemInfo::getFreeItemInfoSQLStructure() + " values ";
    query = std::accumulate(std::begin(freeItemInfos), std::end(freeItemInfos), query, [](std::string q, mj::model::FreeItemInfo i) {
        return q.append(mj::model::FreeItemInfo::getFreeItemInfoSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto freeItemInfo : freeItemInfos) {

        /*
            int _id = 0;
            int _lang_id = 1;
            bool _isLearn = false;
            bool _isLock = false;
            FreeContentOption _option = FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY;
            DocumentsType _type = DocumentsType::STORY;
            */
        SQLITE_BIND_INT(++counter, counter);
        SQLITE_BIND_INT(++counter, freeItemInfo._id);
        SQLITE_BIND_INT(++counter, freeItemInfo._lang_id);
        SQLITE_BIND_INT(++counter, freeItemInfo._isLearn);
        SQLITE_BIND_INT(++counter, freeItemInfo._isLock);
        SQLITE_BIND_INT(++counter, (int)freeItemInfo._option);
        SQLITE_BIND_INT(++counter, (int)freeItemInfo._type);
        SQLITE_BIND_INT(++counter, freeItemInfo._day);

    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert tbl_free_trial tbl done");
}
std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosByOptions(std::vector<ms::f2p::FreeContentOption> freeContentOptions)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;
    std::vector<int> freeContentOptionsInt;
    for (ms::f2p::FreeContentOption freeContentOption:freeContentOptions)
    {
        freeContentOptionsInt.push_back((int)freeContentOption);
    }

    if (freeContentOptions.size() <= 0)
    {
        return freeItemInfos;
    }
    std::string freeContentOptionsStr = mj::model::ConvertArrayIntergerToString(freeContentOptionsInt);
    auto query = StringUtils::format("select * from tbl_free_trial where _option in (%s)  order by _day asc", freeContentOptionsStr.c_str());

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }

    freeItemInfos = getFreeTrialInfosFromQuery();

    return freeItemInfos;
}

std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosByOption(ms::f2p::FreeContentOption freeContentOption)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;

    auto query = StringUtils::format("select * from tbl_free_trial where _option in (%d) order by _day asc", (int)freeContentOption);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }

    freeItemInfos = getFreeTrialInfosFromQuery();

    return freeItemInfos;
}


std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosByTypes(std::vector<ms::MsTypes::DocumentsType> documentsTypes)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;
    if (documentsTypes.size() <= 0)
    {
        return freeItemInfos;
    }
    std::vector<int> documentsTypesInt;
    for (ms::MsTypes::DocumentsType documentsType : documentsTypes)
    {
        documentsTypesInt.push_back((int)documentsType);
    }

    if (documentsTypesInt.size() <= 0)
    {
        return freeItemInfos;
    }
    std::string documentsTypesStr = mj::model::ConvertArrayIntergerToString(documentsTypesInt);
    auto query = StringUtils::format("select * from tbl_free_trial where _type in (%s)  order by _day asc", documentsTypesStr.c_str());

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }

    freeItemInfos = getFreeTrialInfosFromQuery();

    return freeItemInfos;
}

std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosByType(ms::MsTypes::DocumentsType documentsType)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;

    auto query = StringUtils::format("select * from tbl_free_trial where _type in (%d)  order by _day asc", (int)documentsType);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }

    freeItemInfos = getFreeTrialInfosFromQuery();

    return freeItemInfos;
}


std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosByIds(std::vector<int> ids)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;
    if (ids.size() <= 0)
    {
        return freeItemInfos;
    }

    std::string iDsStr = mj::model::ConvertArrayIntergerToString(ids);
    auto query = StringUtils::format("select * from tbl_free_trial where _id in (%s)  order by _day asc", iDsStr.c_str());

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }

    freeItemInfos = getFreeTrialInfosFromQuery();

    return freeItemInfos;
}

std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosById(int ids)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;

    auto query = StringUtils::format("select * from tbl_free_trial where _id in (%d)  order by _day asc", ids);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return freeItemInfos;
    }
    freeItemInfos = getFreeTrialInfosFromQuery();
    return freeItemInfos;
}
std::vector<mj::model::FreeItemInfo> StoryDataModel::getFreeTrialInfosFromQuery()
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::FreeItemInfo freeItemInfo;
        freeItemInfo._idFree = SQLITE_COLUMN_INT(0);
        freeItemInfo._id = SQLITE_COLUMN_INT(1);
        freeItemInfo._lang_id = SQLITE_COLUMN_INT(2);
        freeItemInfo._isLearn = SQLITE_COLUMN_INT(3);
        freeItemInfo._isLock = SQLITE_COLUMN_INT(4);
        freeItemInfo._option = (ms::f2p::FreeContentOption)SQLITE_COLUMN_INT(5);
        freeItemInfo._type = (ms::MsTypes::DocumentsType)SQLITE_COLUMN_INT(6);
        freeItemInfo._day = SQLITE_COLUMN_INT(7);

        freeItemInfos.push_back(freeItemInfo);
    }
    return freeItemInfos;
}
bool StoryDataModel::deleteFreeTrialInfosTable()
{
    // delete tbl_free_trial tbl from db
    bool resultDeleteDB = false;
    auto query = StringUtils::format("drop table tbl_free_trial");
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
        resultDeleteDB = false;
    }
    else {
        CCLOG("remove all free trial ");
        resultDeleteDB = true;
    }

    //create again free_trial_tbl
    query = "create table if not exists tbl_free_trial ("
        
        " _idFree integer not null,"
        " _id integer not null,"
        " _lang_id integer,"
        " _isLearn integer,"
        " _isLock integer,"
        " _option integer,"
        " _type integer,"
        " _day integer,"
        " primary key (_idFree))";

    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("created again table tbl_free_trial");
    }
    return resultDeleteDB;
}

/*--------------------end handler for f2P trial data-------------------*/


void StoryDataModel::insertActivities(std::vector<mj::model::ActInfo> activities) {
    if (activities.size() <= 0)
        return;
    std::string query = "replace into tbl_activity " + mj::model::ActInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(activities), std::end(activities), query, [](std::string q, mj::model::ActInfo i) {
        return q.append(mj::model::ActInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto activity : activities) {
        //(_id, _order, lesson_id, story_id, type, name, content, resource_zip, data_zip)
        SQLITE_BIND_INT(++counter, activity.activity_id);
        SQLITE_BIND_INT(++counter, activity.order);
        SQLITE_BIND_INT(++counter, activity.lesson_id);
        SQLITE_BIND_INT(++counter, activity.sub_id);
        SQLITE_BIND_INT(++counter, (int)activity.type);
        SQLITE_BIND_STR(++counter, activity.name);
        SQLITE_BIND_STR(++counter, activity.content);
        SQLITE_BIND_STR(++counter, activity.resource_zip);
        SQLITE_BIND_STR(++counter, activity.data_zip);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert activities done");
}

void StoryDataModel::removeActivitiesOfLesson(int lesson_id) {
    auto activities = getActivitiesOfLesson(lesson_id);
    auto writable = FileUtils::getInstance()->getWritablePath();

    std::vector <std::string> subFolders;
    std::string imageCacheFolder = "cached/zip/";
    std::vector < std::string > fileList = FileUtils::getInstance()->listFiles(FileUtils::getInstance()->getWritablePath() + imageCacheFolder);
    for (std::string item : fileList) {
        if (FileUtils::getInstance()->isFileExist(item))
            subFolders.push_back(item);
    }
    if (subFolders.size() <= 0)
    {
        return;
    }
    for (auto activity : activities) {
        if (activity.type == mj::model::ActInfo::GAME) {
            for (auto zip : activity.zip_urls) {
                auto urls = getGameZipUrl();
                if (mj::helper::isContain(urls, zip)) {
                    continue; //là zip resource game
                }
                zip = mj::helper::getFilenameFromURL(zip);

                CCLOG("removing file %s", (writable + "cached/zip/" + zip).c_str());
                auto removed = FileUtils::getInstance()->removeFile(writable + "cached/zip/" + zip);
                if (removed) {
                    CCLOG("removed %s", zip.c_str());
                }
            }
        }
    }

    auto query = StringUtils::format("delete from tbl_activity where lesson_id = %d", lesson_id);
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("remove activities of lesson %d", lesson_id);
    }

    FileUtils::getInstance()->purgeCachedEntries();
}

void StoryDataModel::removeAllActivities() {
    auto query = StringUtils::format("delete from tbl_activity");
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    }
    else {
        CCLOG("remove all activities");
    }

    FileUtils::getInstance()->purgeCachedEntries();
}

mj::model::ActInfo StoryDataModel::getActivityById(int activity_id) {
    auto query = StringUtils::format("select * from tbl_activity where _id = %d", activity_id);
    mj::model::ActInfo r;
    r.activity_id = -1;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::ActInfo info;
        info.activity_id = SQLITE_COLUMN_INT(0);
        info.order = SQLITE_COLUMN_INT(1);
        info.lesson_id = SQLITE_COLUMN_INT(2);
        info.sub_id = SQLITE_COLUMN_INT(3);
        info.type = (mj::model::ActInfo::Type)SQLITE_COLUMN_INT(4);
        info.name = SQLITE_COLUMN_STR(5);
        info.content = SQLITE_COLUMN_STR(6);
        info.resource_zip = SQLITE_COLUMN_STR(7);
        info.data_zip = SQLITE_COLUMN_STR(8);
        if (info.type == mj::model::ActInfo::Type::STORY)
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_STORY);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_STORY);
        }
        else
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES);
        }

        r = info;
    }

    if (!r.data_zip.empty()) r.zip_urls.push_back(r.data_zip);
    if (!r.resource_zip.empty()) r.zip_urls.push_back(r.resource_zip);

    if (r.type == mj::model::ActInfo::Type::GAME) {
        auto game_info = StoryDataModel::getInstance()->getGameById(r.sub_id);
        if (!game_info.resource_zip.empty()) r.zip_urls.push_back(r.resource_zip);
    }
    return r;
}

std::vector<mj::model::StoryInfo> StoryDataModel::getInforActivity()
{
    std::string query = "select * from tbl_story LEFT OUTER JOIN tbl_story_read ON tbl_story.id = tbl_story_read.id_story_lang where id > 0 order by id asc";

    std::vector<mj::model::StoryInfo> r;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }

    int count = 0;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::StoryInfo info;

        info.unique_id = sqlite3_column_int(this->stmp, 0);
        info.story_id = sqlite3_column_int(this->stmp, 1);
        info.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2)));
        info.total_words = sqlite3_column_int(this->stmp, 3);
        info.lexile = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.lang_id = sqlite3_column_int(this->stmp, 6);
        info.total_page = sqlite3_column_int(this->stmp, 7);
        info.thumbnail_url = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        info.thumbnail_url = mj::helper::getUrlFromDomain(info.thumbnail_url, mj::UrlType::MSURL_STORY);
        info.author = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        info.designer = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));
        info.quality = sqlite3_column_int(this->stmp, 12);
        info.date_published = sqlite3_column_int(this->stmp, 13);
        info.levels.push_back(sqlite3_column_int(this->stmp, 14));
        info.version = sqlite3_column_int(this->stmp, 15);
        info.is_read = sqlite3_column_int(this->stmp, 17);

        r.push_back(info);
    }

    return r;
}

std::vector<mj::model::ActInfo> StoryDataModel::getActivitiesOfLesson(int lesson_id, QuerySelectType type, std::pair<int, std::string> valueError, int index, std::vector<std::string> domainStory, std::vector<std::string> domainAudioBook, std::string typeError) {

    if(lesson_id == 14409)
        CCLOG("insert games done");
    if (index == -1 && domainStory.empty() && domainAudioBook.empty())
    {
        auto idAndListLesson = ms::CacheManager::getInstance().getCurrentLessonAndListActivityOfLesson(lesson_id);

        if (idAndListLesson.first != -1)
        {
            return idAndListLesson.second;
        }
    }

    auto query = StringUtils::format("select * from tbl_activity where lesson_id = %d order by _order asc", lesson_id);
    if (type == QuerySelectType::FIRST) {// only query first item

        query = StringUtils::format("select * from tbl_activity where lesson_id = %d order by _order asc limit 1", lesson_id);
    }
  std::vector<mj::model::ActInfo> r;
  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      logError();
      return r;
  }
  while (sqlite3_step(stmt) == SQLITE_ROW) {
      mj::model::ActInfo info;
      info.activity_id = SQLITE_COLUMN_INT_STMT(stmt, 0);
      info.order = SQLITE_COLUMN_INT_STMT(stmt, 1);
      info.lesson_id = SQLITE_COLUMN_INT_STMT(stmt, 2);
      info.sub_id = SQLITE_COLUMN_INT_STMT(stmt, 3);
      info.type = (mj::model::ActInfo::Type)SQLITE_COLUMN_INT_STMT(stmt, 4);
      info.name = SQLITE_COLUMN_STR_STMT(stmt, 5);
      info.content = SQLITE_COLUMN_STR_STMT(stmt, 6);
      info.resource_zip = SQLITE_COLUMN_STR_STMT(stmt, 7);
      info.data_zip = SQLITE_COLUMN_STR_STMT(stmt, 8);

      if (info.type == mj::model::ActInfo::Type::STORY)
      {
          info.resource_zip = getUrlOfDomain(info.resource_zip, mj::UrlType::MSURL_STORY, index, domainStory, typeError, valueError);
          
          info.data_zip = getUrlOfDomain(info.data_zip, mj::UrlType::MSURL_STORY, index, domainStory, typeError, valueError);
      }
      else
      {
          info.resource_zip = getUrlOfDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES, index, domainAudioBook, typeError, valueError);
          info.data_zip = getUrlOfDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES, index, domainAudioBook, typeError, valueError);
      }
      if (info.type == mj::model::ActInfo::Type::STORY) {
          auto story_id = info.sub_id;
          auto story = getStoryById(story_id);
          if (story.story_id == 0) {
              continue;
          }
      }

      //TODO: an game
      if (info.type == mj::model::ActInfo::Type::GAME) {
          auto game_id = info.sub_id;
      }


      r.push_back(info);
  }

  sqlite3_finalize(stmt);

  for (auto& activity : r) {
      if (!activity.data_zip.empty() && FileUtils::getInstance()->getFileExtension(activity.data_zip) == ".zip")
          activity.zip_urls.push_back(activity.data_zip);
      if (!activity.resource_zip.empty() && FileUtils::getInstance()->getFileExtension(activity.resource_zip) == ".zip")
          activity.zip_urls.push_back(activity.resource_zip);

      if (activity.type == mj::model::ActInfo::Type::GAME) {

          mj::model::GameInfo game_info = StoryDataModel::getInstance()->getGameById(activity.sub_id, valueError, index, domainAudioBook, typeError);
         
          if (!game_info.resource_zip.empty()  && FileUtils::getInstance()->getFileExtension(game_info.resource_zip) == ".zip")
              activity.zip_urls.push_back(game_info.resource_zip);
      }
  }

  auto activities = r;
  std::vector<mj::model::ActInfo> activities_convert;
  cocos2d::ValueMap map_merge_quiz;
  int convert_index = 0;
  for (int i = 0; i < (int)activities.size(); i++) {
      //        CCLOG("id = %d | name = %s", activities[i].sub_id, activities[i].name.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
      if (activities[i].sub_id == 66) continue;//game phonics sử dụng video và thu âm.
#endif
      int number_turn_play = 1;
      auto name_activity = activities[i].name;
      name_activity = name_activity.substr(0, name_activity.find_first_of(":"));

      auto list_splits = mj::helper::splitString(name_activity, '_');
      if ((int)list_splits.size() >= 4) {
          auto quiz_name = list_splits[0] + "_" + list_splits[1];
          auto quiz_id = atoi(list_splits[2].c_str());
          number_turn_play = atoi(list_splits[3].c_str());
          if (map_merge_quiz[quiz_name].asInt() == 0) {
              auto quiz_info = activities[i];
              quiz_info.quizz_id.push_back(activities[i].activity_id);
              quiz_info.list_num_turnplay.push_back(number_turn_play);
              activities_convert.push_back(quiz_info);
              convert_index = (int)activities_convert.size() - 1;
          }
          else {
              activities_convert[convert_index].quizz_id.push_back(activities[i].activity_id);
              activities_convert[convert_index].list_num_turnplay.push_back(number_turn_play);
              std::vector<std::string> zip_urls;
              for (auto url : activities_convert[convert_index].zip_urls) {
                  if(!url.empty() && FileUtils::getInstance()->getFileExtension(url) == ".zip")
                  {
                      zip_urls.push_back(url);
                  }
              }
              activities_convert[convert_index].zip_urls = zip_urls;
                  activities_convert[convert_index].zip_urls.insert(activities_convert[convert_index].zip_urls.end(), activities[i].zip_urls.begin(), activities[i].zip_urls.end());
            
          }
          map_merge_quiz[quiz_name] = quiz_id;
          //            CCLOG("hoisunglog - sub: %s - %d - %d", name_activity.c_str(), quiz_id, number_turn_play);
      }
      else {
          activities[i].quizz_id.push_back(activities[i].activity_id);
          activities[i].list_num_turnplay.push_back(number_turn_play);
          activities_convert.push_back(activities[i]);
      }
  }
  for (auto i = 0; i < activities_convert.size(); ++i) {
      activities_convert[i].order = i;
  }

  auto listGame = getListGameError();

  if (!listGame.empty())
  {
      for (int i = 0; i < listGame.size(); i++)
      {
          activities_convert.erase(std::remove_if(activities_convert.begin(), activities_convert.end(), [=](mj::model::ActInfo act) {
              return act.sub_id == listGame[i];
              }), activities_convert.end());
      }
  }

  if (index == -1 && domainStory.empty() && domainAudioBook.empty())
  {
      ms::CacheManager::getInstance().setCurrentLessonAndListActivityOfLesson({lesson_id, activities_convert});
  }

  return activities_convert;
}

std::vector<mj::model::ActInfo> StoryDataModel::getActivitiesOfLessonNew(int lesson_id, QuerySelectType type) {
    auto query = StringUtils::format("select * from tbl_activity where lesson_id = %d order by _order asc", lesson_id);
    if(type == QuerySelectType::FIRST)
        query = StringUtils::format("select * from tbl_activity where lesson_id = %d order by _order asc limit 1", lesson_id);
    std::vector<mj::model::ActInfo> r;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logError();
        return r;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        mj::model::ActInfo info;
        info.activity_id = SQLITE_COLUMN_INT_STMT(stmt, 0);
        info.order = SQLITE_COLUMN_INT_STMT(stmt, 1);
        info.lesson_id = SQLITE_COLUMN_INT_STMT(stmt, 2);
        info.sub_id = SQLITE_COLUMN_INT_STMT(stmt, 3);
        info.type = (mj::model::ActInfo::Type)SQLITE_COLUMN_INT_STMT(stmt, 4);
        info.name = SQLITE_COLUMN_STR_STMT(stmt, 5);
        info.content = SQLITE_COLUMN_STR_STMT(stmt, 6);
        info.resource_zip = SQLITE_COLUMN_STR_STMT(stmt, 7);
        //info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES);
        info.data_zip = SQLITE_COLUMN_STR_STMT(stmt, 8);
        //info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES);

        if (info.type == mj::model::ActInfo::Type::STORY)
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_STORY);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_STORY);
        }
        else
        {
            info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_ACTIVITIES);
            info.data_zip = mj::helper::getUrlFromDomain(info.data_zip, mj::UrlType::MSURL_ACTIVITIES);
        }
        if (info.type == mj::model::ActInfo::Type::STORY) {
            auto story_id = info.sub_id;
            auto story = getStoryById(story_id);
            if (story.story_id == 0) {
                continue;
            }
        }

        //TODO: an game
        if (info.type == mj::model::ActInfo::Type::GAME) {
            auto game_id = info.sub_id;
        }


        r.push_back(info);
    }

    sqlite3_finalize(stmt);
    
    //std::unordered_map<int,std::string> gameID
    
    std::vector<int> gameURLZips;
    
    for (auto& activity : r) {
        if (!activity.data_zip.empty()) activity.zip_urls.push_back(activity.data_zip);
        if (!activity.resource_zip.empty()) activity.zip_urls.push_back(activity.resource_zip);

        if (activity.type == mj::model::ActInfo::Type::GAME) {
            
            gameURLZips.push_back(activity.sub_id);
            //auto game_info = StoryDataModel::getInstance()->getGameById(activity.sub_id);
            //if (!game_info.resource_zip.empty()) activity.zip_urls.push_back(game_info.resource_zip);
        }
    }

    
   auto gameIDsMap = StoryDataModel::getInstance()->getGameByIds(gameURLZips);

    auto activties = r;
    
    for (auto& activity : activties) {
      
        if (activity.type == mj::model::ActInfo::Type::GAME) {
            
            if(gameIDsMap.count(activity.sub_id) > 0) {
                
                auto game_info = gameIDsMap.at(activity.sub_id);
                if (!game_info.resource_zip.empty())
                    activity.zip_urls.push_back(game_info.resource_zip);
            }
        }
    }
    
    auto activities = r;
    std::vector<mj::model::ActInfo> activities_convert;
    cocos2d::ValueMap map_merge_quiz;
    int convert_index = 0;
    for (int i = 0; i < (int)activities.size(); i++) {
        //        CCLOG("id = %d | name = %s", activities[i].sub_id, activities[i].name.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        if (activities[i].sub_id == 66) continue;//game phonics sử dụng video và thu âm.
#endif
        int number_turn_play = 1;
        auto name_activity = activities[i].name;
        name_activity = name_activity.substr(0, name_activity.find_first_of(":"));

        auto list_splits = mj::helper::splitString(name_activity, '_');
        if ((int)list_splits.size() >= 4) {
            auto quiz_name = list_splits[0] + "_" + list_splits[1];
            auto quiz_id = atoi(list_splits[2].c_str());
            number_turn_play = atoi(list_splits[3].c_str());
            if (map_merge_quiz[quiz_name].asInt() == 0) {
                auto quiz_info = activities[i];
                quiz_info.quizz_id.push_back(activities[i].activity_id);
                quiz_info.list_num_turnplay.push_back(number_turn_play);
                activities_convert.push_back(quiz_info);
                convert_index = (int)activities_convert.size() - 1;
            }
            else {
                activities_convert[convert_index].quizz_id.push_back(activities[i].activity_id);
                activities_convert[convert_index].list_num_turnplay.push_back(number_turn_play);
                activities_convert[convert_index].zip_urls.insert(activities_convert[convert_index].zip_urls.end(), activities[i].zip_urls.begin(), activities[i].zip_urls.end());
            }
            map_merge_quiz[quiz_name] = quiz_id;
            //            CCLOG("hoisunglog - sub: %s - %d - %d", name_activity.c_str(), quiz_id, number_turn_play);
        }
        else {
            activities[i].quizz_id.push_back(activities[i].activity_id);
            activities[i].list_num_turnplay.push_back(number_turn_play);
            activities_convert.push_back(activities[i]);
        }
    }
    for (auto i = 0; i < activities_convert.size(); ++i) {
        activities_convert[i].order = i;
    }

    return activities_convert;
}

void StoryDataModel::insertGameInfo(std::vector<mj::model::GameInfo> games) {
    if (games.empty()) {
        return;
    }
    if (games.size() > NUMBER_QUERY) {
        auto first = 0;
        const auto chunk_size = NUMBER_QUERY;
        while (first < games.size()) {
            auto last = std::min(first + chunk_size, (int)games.size());
            insertGameInfo(std::vector<mj::model::GameInfo>(games.begin() + first, games.begin() + last));
            first = last;
        }
        return;
    }

    std::string query = "replace into tbl_game " + mj::model::GameInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(games), std::end(games), query, [](std::string q, mj::model::GameInfo i) {
        return q.append(mj::model::GameInfo::getSQLPlaceholders() + ",");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto game : games) {
        //(_id, _order, lesson_id, story_id, type, name, content, resource_zip, data_zip)
        SQLITE_BIND_INT(++counter, game.game_id);
        SQLITE_BIND_STR(++counter, game.name);
        SQLITE_BIND_STR(++counter, game.resource_zip);
        SQLITE_BIND_STR(++counter, game.thumbnail_url);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert games done");
}

std::vector<std::string> StoryDataModel::getGameZipUrl() {
    std::vector<std::string> urls;
    auto query = "select resource_zip from tbl_game";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return urls;
    }
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        urls.push_back(mj::helper::getUrlFromDomain(SQLITE_COLUMN_STR(0), mj::UrlType::MSURL_GAME));
    }

    return urls;
}

std::unordered_map<int,mj::model::GameInfo> StoryDataModel::getGameByIds(std::vector<int> gameIDs) {

    std::string gameIDStr = "";
    std::unordered_map<int, mj::model::GameInfo> result;

    if (gameIDs.size() <= 0)
        return result;
    for(auto id:gameIDs) {
        
        gameIDStr +=(StringUtils::format("%d,",id));
    }
    
    if(!gameIDStr.empty()) {
        gameIDStr.pop_back();
    }
    
    auto query = StringUtils::format("select * from tbl_game where _id in (%s)", gameIDStr.c_str());


    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return result;
    };
  
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {

        mj::model::GameInfo info;
        info.game_id = SQLITE_COLUMN_INT(0);
        info.name = SQLITE_COLUMN_STR(1);
        info.resource_zip = SQLITE_COLUMN_STR(2);
        info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_GAME);
        info.thumbnail_url = mj::helper::getUrlFromDomain(SQLITE_COLUMN_STR(3), mj::UrlType::MSURL_GAME);

        result.insert({info.game_id,info});
    }

    return result;
}

mj::model::GameInfo StoryDataModel::getGameById(int game_id, std::pair<int, std::string> valueError, int index, std::vector<std::string> domainAudioBook, std::string typeError) {
    mj::model::GameInfo info;
    auto query = StringUtils::format("select * from tbl_game where _id=%d", game_id);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return info;
    };
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        info.game_id = SQLITE_COLUMN_INT(0);
        info.name = SQLITE_COLUMN_STR(1);
        info.resource_zip = SQLITE_COLUMN_STR(2);

        info.thumbnail_url = mj::helper::getUrlFromDomain(SQLITE_COLUMN_STR(3), mj::UrlType::MSURL_GAME);

        info.resource_zip = mj::helper::getUrlFromDomain(info.resource_zip, mj::UrlType::MSURL_GAME, index, domainAudioBook, typeError);

    }
    return info;
}

void StoryDataModel::insertNewGrades(std::vector<mj::model::GradeInfo> grades) {
    if (grades.size() <= 0)
        return;
    std::string query = "";
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    //    std::string query = "delete from tbl_new_grade";
    //    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
    //        logError();
    //    }
#endif
    query = "replace into tbl_new_grade " + mj::model::GradeInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(grades), std::end(grades), query, [](std::string q, mj::model::GradeInfo g) {
        return q + mj::model::GradeInfo::getSQLPlaceholders() + ",";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    };

    auto counter = 0;
    for (auto grade : grades) {
        sqlite3_bind_int(this->stmp, ++counter, grade.grade_id);
        sqlite3_bind_int(this->stmp, ++counter, grade.lang_id);
        sqlite3_bind_text(this->stmp, ++counter, grade.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, grade.description.c_str(), -1, SQLITE_TRANSIENT);
    }

    std::string description = "";

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }
}

std::vector<mj::model::GradeInfo> StoryDataModel::getAllNewGrades(int lang_id) {
    std::vector<mj::model::GradeInfo> grades;
    std::string query = StringUtils::format("select * from tbl_new_grade where lang_id = %d", lang_id);
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::GradeInfo grade;
        grade.grade_id = sqlite3_column_int(this->stmp, 0);
        grade.lang_id = sqlite3_column_int(this->stmp, 1);
        grade.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2));
        grade.description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        grades.push_back(grade);
    }

    return grades;
}
void StoryDataModel::removeNewGradesByLangId(std::vector<int> langIds) {
    if (langIds.size() <= 0)
        return;
    std::string query = "delete from tbl_new_grade where lang_id in (";
    query = std::accumulate(std::begin(langIds), std::end(langIds), query, [](std::string q, int i) {
        return q.append("?,");
        });
    query.resize(query.length() - 1);
    query = query + ")";

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto i : langIds) {
        SQLITE_BIND_INT(++counter, i);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }

    CCLOG("delete lessons done");
}

void StoryDataModel::insertProductList(std::vector<mj::model::ProductListInfo> productListInfos) {
    if (productListInfos.size() <= 0)
        return;
    std::string query = "";
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
#endif
    query = "replace into tbl_new_product_list " + mj::model::ProductListInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(productListInfos), std::end(productListInfos), query, [](std::string q, mj::model::ProductListInfo productListInfo) {
        return q + mj::model::ProductListInfo::getSQLPlaceholders() + ",";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    };

    auto counter = 0;
    for (auto productListInfo : productListInfos) {
        sqlite3_bind_int(this->stmp, ++counter, productListInfo.id);
        sqlite3_bind_text(this->stmp, ++counter, productListInfo.city.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++counter, productListInfo.tien_ao);
        sqlite3_bind_int(this->stmp, ++counter, productListInfo.giam_40);
        sqlite3_bind_int(this->stmp, ++counter, productListInfo.giam_30);
        sqlite3_bind_int(this->stmp, ++counter, productListInfo.sale_off);

        sqlite3_bind_text(this->stmp, ++counter, productListInfo.product_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, productListInfo.product_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, productListInfo.product_description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, productListInfo.product_code.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, productListInfo.current_unit.c_str(), -1, SQLITE_TRANSIENT);
    }
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }
}
std::vector<mj::model::ProductListInfo> StoryDataModel::getAllProductList(std::string city) {
    std::vector<mj::model::ProductListInfo> productListInfos;
    std::string query = "select * from tbl_new_product_list where city = " + city;
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::ProductListInfo productListInfo;
        productListInfo.tien_ao = sqlite3_column_int(this->stmp, 0);
        productListInfo.city = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1));
        productListInfo.tien_ao = sqlite3_column_int(this->stmp, 2);
        productListInfo.giam_40 = sqlite3_column_int(this->stmp, 3);
        productListInfo.giam_30 = sqlite3_column_int(this->stmp, 4);
        productListInfo.sale_off = sqlite3_column_int(this->stmp, 5);
        productListInfo.product_id = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 6));
        productListInfo.product_name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 7));
        productListInfo.product_description = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8));
        productListInfo.product_code = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 9));
        productListInfo.current_unit = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 10));

        productListInfos.push_back(productListInfo);
    }

    return productListInfos;
}
std::vector<mj::model::StoryInfo> StoryDataModel::getFreeStoriesByLanguage(int lang_id) {
    std::vector<mj::model::StoryInfo> r;
    auto list_free = FREE_STORY_MANAGER->getListFreeStoriesOfDay(lang_id);
    for (auto index : list_free) {
        auto i = index.asInt();
        auto story = getStoryById(i);
        // hieunt:  hiện tại server đang trả về id_truyen_chung
        //          nên nếu đúng phải gọi hàm getStoryByStoryIdAndLang
        //          đề xuất server trả về id_truyen_rieng
//        auto story = StoryDataModel::getInstance()->getStoryByStoryIdAndLang(i, LANGUAGE_EN_CODE);
        if (story.story_id != 0)
            r.push_back(story);
    }

    return r;
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getFreeAudiobooksByLanguage(int lang_id) {
    std::vector<mj::model::AudiobookInfo> r;
    auto list_free = FREE_STORY_MANAGER->getListFreeAudioBookOfDay(lang_id);
    for (auto index : list_free) {
        auto i = index.asInt();
        auto audiobook = getAudiobook(i);
        if (audiobook.book_id > 0 && !audiobook.audio_url.empty()) {
            r.push_back(audiobook);
        }
    }

    return r;
}

void StoryDataModel::removeAllStories() {
    MJDEFAULT->setIntegerForKey(key_api_version, 0);
    std::vector<std::string> list_table = { "tbl_story", "tbl_activity", "tbl_audio_read", "tbl_audiobook_v2",
        "tbl_audiobook_series", "tbl_category", "tbl_category_stories", "tbl_character",
        "tbl_character_stories", "tbl_game", "tbl_grades", "tbl_inapp", "tbl_lesson_v2", 
        "tbl_lesson_v2_status", "tbl_new_grade", "tbl_reading_levels", "tbl_story_category", 
        "tbl_story_character", "tbl_story_download", "tbl_story_feature", "tbl_story_level",
        "tbl_story_new", "tbl_story_popular", "tbl_story_rate", "tbl_story_read", "tbl_story_sentapi", 
        "tbl_story_popular", "tbl_story_popular", "tbl_story_popular",  "tbl_material_popular", "tbl_story_activities", "tbl_story_link","tbl_story_popular_search",
        "tbl_ai_speaking_common" , "tbl_ai_speaking_lessons", "tbl_ai_speaking_topic", "tbl_new_product_list" };

    for (int i = 0; i < (int)list_table.size(); i++) {
        string query = "DELETE FROM " + list_table[i];
        ValueVector vector_query;
        executeQuery(query, vector_query);
    }
}

void StoryDataModel::setAudioRead(std::vector<std::pair<std::string, int>> listAuidoBookList)
{
    if (listAuidoBookList.size() <= 0)
        return;
    if (listAuidoBookList.size() > 100) {
        auto first = 0;
        const auto chunk_size = 100;
        while (first < listAuidoBookList.size()) {
            auto last = std::min(first + chunk_size, (int)listAuidoBookList.size());
            setAudioRead(std::vector<std::pair<std::string, int>>(std::begin(listAuidoBookList) + first, std::begin(listAuidoBookList) + last));
            first = last;
        }

        return;
    }

    std::string query = "REPLACE INTO tbl_audio_read" + std::string("(audio_id,read,time_read)") + " values ";
    query = std::accumulate(std::begin(listAuidoBookList), std::end(listAuidoBookList), query, [](std::string q, std::pair<std::string, int> i) {
        return q.append("(?,?,?),");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    int count = 0;

    for (auto stories : listAuidoBookList)
    {
        if (stories.second <= 0 || stories.second >= (int)(utils::getTimeInMilliseconds()))
        {
            stories.second = (int)(utils::getTimeInMilliseconds());
        }

        SQLITE_BIND_INT(++count, std::stoi(stories.first));
        SQLITE_BIND_INT(++count, 1);
        SQLITE_BIND_INT(++count, stories.second);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }

}

void StoryDataModel::setAudioRead(int audio_id, int read_value, int time_read) {
    std::string query = "REPLACE INTO tbl_audio_read (audio_id,read,time_read) VALUES (?,?,?)";
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    //Tranh cãi sau :))
    //    if (time_read == 0) {
    //        time_read = (int)time(nullptr);
    //    }else time_read = 1505382278;
//    time_read = (int)time(nullptr);
    if (time_read <= 0 || time_read >= (int)(utils::getTimeInMilliseconds()))
    {
        time_read = (int)(utils::getTimeInMilliseconds());
    }
    sqlite3_bind_int(this->stmp, 1, audio_id);
    sqlite3_bind_int(this->stmp, 2, read_value);
    sqlite3_bind_int(this->stmp, 3, time_read);
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        this->showError(result);
        return;
    }
}
ValueVector StoryDataModel::getAllAudioRead() {
    string query = "SELECT * FROM tbl_audio_read ORDER BY time_read DESC";
    return fetchAllRecord(query);
}

std::vector <int> StoryDataModel::getValueAudioReadById(int audioId)
{
    std::vector <int> valueAudioRead = {};

    try
    {
        std::string  query = StringUtils::format("select * from tbl_audio_read where audio_id=%d", audioId);
        if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
            logError();
            return valueAudioRead;
        };

        while (sqlite3_step(this->stmp) == SQLITE_ROW) {
            valueAudioRead.push_back(sqlite3_column_int(this->stmp, 0));
            valueAudioRead.push_back(sqlite3_column_int(this->stmp, 1));
            valueAudioRead.push_back(sqlite3_column_int(this->stmp, 2));
        }
    }
    catch (const std::exception&)
    {

    }

    return valueAudioRead;
}

void StoryDataModel::removeAllAudioRead() {

    try
    {
        string query = "DELETE FROM tbl_audio_read";
        ValueVector vector_query;
        executeQuery(query, vector_query);
    }
    catch (const std::exception&)
    {

    }
}

void StoryDataModel::removeAllAudiobookSeries() {

    try
    {
        string query = "DELETE FROM tbl_audiobook_series2";
        ValueVector vector_query;
        executeQuery(query, vector_query);
    }
    catch (const std::exception&)
    {

    }
}

std::vector<mj::model::AudiobookInfo> StoryDataModel::getAllAudiobooksDownloaded() {
    FileUtils::getInstance()->purgeCachedEntries();
    auto audiobooks = getAllAudiobooksByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    std::vector<mj::model::AudiobookInfo> downloaded;

    for (auto info : audiobooks) {
        auto needed_getinfo = false;
        //        if (info.sync_path.empty() || info.audio_url.empty()) {
        //            needed_getinfo = true;
        //        } else {
        //            if (!FileUtils::getInstance()->isFileExist(info.sync_path)) {
        //                needed_getinfo = true;
        //            }
        if (!FileUtils::getInstance()->isFileExist(std::string("audiobooks/audio/") + mj::helper::getFilenameFromURL(info.audio_url))) {
            needed_getinfo = true;
        }
        //        }
        if (!needed_getinfo) {
            downloaded.push_back(info);
        }
    }

    return downloaded;
}

std::string StoryDataModel::getLinkDownloadStory(int story_id, int index, std::vector<std::string> domainStory, std::string typeError) {
    std::string query = StringUtils::format("select %s from tbl_story_link where story_id = %d", StoryData::getInstance()->isHdr ? "story_link_hdr" : "story_link_hd", story_id);

    std::string url = "";
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        url = mj::helper::getUrlFromDomain(SQLITE_COLUMN_STR(0), mj::UrlType::MSURL_STORY, index, domainStory, typeError);
    }
    else {
        logError();
    }

    //    sqlite3_finalize(this->stmp);
    return url;
}

void StoryDataModel::setLinkDownloadStory(std::vector<std::tuple<int, std::string, std::string>> data) {
    if (data.size() <= 0)
        return;
    if (data.size() > NUMBER_QUERY) {
        auto first = 0;
        const auto chunk_size = NUMBER_QUERY;
        while (first < data.size()) {
            auto last = std::min(first + chunk_size, (int)data.size());
            setLinkDownloadStory(std::vector<std::tuple<int, std::string, std::string>>(std::begin(data) + first, std::begin(data) + last));
            first = last;
        }

        return;
    }

    std::string query = "replace into tbl_story_link (story_id, story_link_hd, story_link_hdr) values ";
    for (auto d : data) {
        query += "(?, ?, ?),";
    }
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto d : data) {
        int unique_id;
        std::string link_hd, link_hdr;
        std::tie(unique_id, link_hd, link_hdr) = d;
        SQLITE_BIND_INT(++counter, unique_id);
        SQLITE_BIND_STR(++counter, link_hd);
        SQLITE_BIND_STR(++counter, link_hdr);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert link done");
}

#pragma mark - LICENSE_MANAGER
void StoryDataModel::updateLicenseDeviceInfo(std::vector<license::LicenseDeviceInfo> list_licensedeviceinfo) {
    if (list_licensedeviceinfo.size() <= 0)
        return;
    std::string query = "delete from license_device_info";
    auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }

    query = "replace into license_device_info " + license::LicenseDeviceInfo::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(list_licensedeviceinfo), std::end(list_licensedeviceinfo), query, [](std::string q, license::LicenseDeviceInfo inf) {
        return q + inf.getSQLValues() + ",";
        });
    query.resize(query.length() - 1);

    r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }
}

std::vector<license::LicenseDeviceInfo> StoryDataModel::getLicenseDeviceInfo() {
    std::vector<license::LicenseDeviceInfo> r;

    std::string query = StringUtils::format("select * from license_device_info ORDER BY device_status DESC, device_id");
    auto result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return r;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        license::LicenseDeviceInfo info;
        info.device_id = sqlite3_column_int(this->stmp, 0);
        info.device_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1)));
        info.device_status = sqlite3_column_int(this->stmp, 2);
        info.active_date = sqlite3_column_int(this->stmp, 3);
        info.deactive_date = sqlite3_column_int(this->stmp, 4);
        r.push_back(info);
    }

    return r;
}

void StoryDataModel::updateFileSize(std::vector<std::pair<std::string, int>> list_size) {
    return;
    if (list_size.empty())
    {
        return;
    }
    if (list_size.size() > NUMBER_QUERY) {
        auto first = 0;
        const auto chunk_size = NUMBER_QUERY;
        while (first < list_size.size()) {
            auto last = std::min(first + chunk_size, (int)list_size.size());
            updateFileSize(std::vector<std::pair<std::string, int>>(list_size.begin() + first, list_size.begin() + last));
            first = last;
        }

        return;
    }

    std::string query = "replace into tbl_file_size (url, file_size) values ";
    query = std::accumulate(std::begin(list_size), std::end(list_size), query, [](std::string q, std::pair<std::string, int> i) {
        return q.append("(?, ?),");
        });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    auto counter = 0;
    for (auto object : list_size) {
        SQLITE_BIND_STR(++counter, object.first);
        SQLITE_BIND_INT(++counter, object.second);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }
    CCLOG("insert file size done");
}

float StoryDataModel::getFileSize(string url) {

    std::string domain;
    // because url in database doesn't contain domain, check if url contain domain, need delete domain before query url in database
    if (int pos = url.find(MJDEFAULT->getStringForKey(DOMAIN_STORY_USING_KEY)) != std::string::npos)
    {
        domain = MJDEFAULT->getStringForKey(DOMAIN_STORY_USING_KEY);
    }
    if (url.find(MJDEFAULT->getStringForKey(DOMAIN_GAME_USING_KEY)) != std::string::npos)
    {
        domain = MJDEFAULT->getStringForKey(DOMAIN_GAME_USING_KEY);
    }
    if (url.find(MJDEFAULT->getStringForKey(DOMAIN_AUDIO_USING_KEY)) != std::string::npos)
    {
        domain = MJDEFAULT->getStringForKey(DOMAIN_AUDIO_USING_KEY);
    }
    if (url.find(MJDEFAULT->getStringForKey(DOMAIN_MEDIA_USING_KEY)) != std::string::npos)
    {
        domain = MJDEFAULT->getStringForKey(DOMAIN_MEDIA_USING_KEY);
    }

    size_t pos = 0;
    if (!domain.empty())
    {
        while ((pos = url.find(domain)) != std::string::npos)
        {
            std::string domainFormUrl = url.substr(0, pos);
            url.erase(0, pos + domain.length());
        }
    }

    // after delete domain, check url from db
    std::string query = ("select file_size from tbl_file_size where url = ?");
    float size = 0.f;

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    SQLITE_BIND_STR(1, url);
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        size = SQLITE_COLUMN_INT(0);
    }
    else {
        logError();
    }
    return size / 1000;
}

void StoryDataModel::initAudiobooksWithChapters() {
    auto query = "select DISTINCT main_id from audiobook_chapters";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto book_id = SQLITE_COLUMN_INT(0);
        _audiobooks_with_chapters.push_back(book_id);
    }

    query = "select DISTINCT chapter_id from audiobook_chapters";
    if (sqlite3_prepare_v2(this->db, query, -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        auto book_id = SQLITE_COLUMN_INT(0);
        _chapter_of_audiobooks.push_back(book_id);
    }
}

std::vector<license::Package> StoryDataModel::getLicensePackages() {
    std::vector<license::Package> packages;
    std::string query = StringUtils::format("select * from tbl_license");
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        license::Package p;
        p.license = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 0));
        p.phone = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1));
        p.mail = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2));
        p.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        p.product_id = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        p.time_enter = sqlite3_column_int(this->stmp, 5);
        p.time_effective = sqlite3_column_int(this->stmp, 6);
        p.time_expiry = sqlite3_column_int(this->stmp, 7);
        p.total_active = sqlite3_column_int(this->stmp, 9);
        p.current_active = sqlite3_column_int(this->stmp, 10);

        packages.push_back(p);
    }

    return packages;
}

std::vector<license::LicenseDeviceInfo> StoryDataModel::getLicenseDeviceInfoOfLicense(const std::string& license) {
    std::vector<license::Package> packages;
    std::string query = StringUtils::format("select * from license_device_info join tbl_license_device on license_device_info.device_id = tbl_license_device.device_id where tbl_license_device.license = ?");
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    sqlite3_bind_text(this->stmp, 1, license.c_str(), -1, SQLITE_TRANSIENT);

    std::vector<license::LicenseDeviceInfo> r;

    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        license::LicenseDeviceInfo info;
        info.device_id = sqlite3_column_int(this->stmp, 0);
        info.device_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1)));
        info.device_status = sqlite3_column_int(this->stmp, 7);
        info.active_date = sqlite3_column_int(this->stmp, 3);
        info.deactive_date = sqlite3_column_int(this->stmp, 4);
        r.push_back(info);
    }

    return r;
}

license::Package StoryDataModel::getLicenseByLicenseKey(const std::string& key) {
    std::vector<license::Package> packages;
    std::string query = StringUtils::format("select * from tbl_license where license = ?");
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    sqlite3_bind_text(this->stmp, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    license::Package p;
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        p.license = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 0));
        p.phone = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 1));
        p.mail = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 2));
        p.name = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 3));
        p.product_id = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        p.time_enter = sqlite3_column_int(this->stmp, 5);
        p.time_effective = sqlite3_column_int(this->stmp, 6);
        p.time_expiry = sqlite3_column_int(this->stmp, 7);
        p.total_active = sqlite3_column_int(this->stmp, 9);
        p.current_active = sqlite3_column_int(this->stmp, 10);
    }

    return p;
}

void StoryDataModel::removeDeviceIDFromListLicenseDevice(int device_id_remove) {
    std::string query;
    if (device_id_remove > 0) {
        query = StringUtils::format("delete from license_device_info");
    }
    else {
        query = StringUtils::format("delete from license_device_info where device_id = %d", device_id_remove);
    }

    auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
        return;
    }
}

void StoryDataModel::addLicensePackages(std::vector<license::Package> packages, bool append) {
    std::string query;
    int r;

    if (!append) {
        query = "delete from tbl_license";
        auto r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
        if (r != SQLITE_OK) {
            showError(r);
        }
    }
    if (packages.size() <= 0)
        return;
    query = "replace into tbl_license " + license::Package::getSQLStructure() + " values ";
    query = std::accumulate(std::begin(packages), std::end(packages), query, [](std::string q, license::Package p) {
        return q + p.getSQLValues() + ",";
        });
    query.resize(query.length() - 1);

    r = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
    };

    auto counter = 0;
    for (auto p : packages) {
        sqlite3_bind_text(this->stmp, ++counter, p.license.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, p.product_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, p.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, p.mail.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++counter, p.phone.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(this->stmp, ++counter, p.time_enter);
        sqlite3_bind_int(this->stmp, ++counter, p.time_effective);
        sqlite3_bind_int(this->stmp, ++counter, p.time_expiry);
        sqlite3_bind_int(this->stmp, ++counter, p.total_active);
        sqlite3_bind_int(this->stmp, ++counter, p.current_active);
    }

    r = sqlite3_step(this->stmp);
    if (r != SQLITE_DONE) {
        showError(r);
    }
}

void StoryDataModel::updateRelationshipLicenseDevice(std::vector<std::tuple<std::string, int, int>> list, bool append) {
    std::string query;
    int r;

    if (!append) {
        query = "delete from tbl_license_device";
        r = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
        if (r != SQLITE_OK) {
            showError(r);
        }
    }
    if (list.size() <= 0)
        return;
    query = "replace into tbl_license_device (device_id, license, status) values ";
    query = std::accumulate(std::begin(list), std::end(list), query, [](std::string q, std::tuple<std::string, int, int> p) {
        return q + "(?, ?, ?),";
        });
    query.resize(query.length() - 1);

    sqlite3_stmt* stmt = nullptr;

    r = sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr);
    if (r != SQLITE_OK) {
        showError(r);
    };

    auto counter = 0;
    for (auto i : list) {
        std::string license;
        int device_id, status;
        std::tie(license, device_id, status) = i;
        sqlite3_bind_int(stmt, ++counter, device_id);
        sqlite3_bind_text(stmt, ++counter, license.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, ++counter, status);
    }

    r = sqlite3_step(stmt);
    if (r != SQLITE_DONE) {
        showError(r);
    }
}

#pragma mark - MATERIAL
void StoryDataModel::insertMaterialsWithVector(const std::vector<mj::model::MaterialInfo>& materials) {
    if (materials.empty()) {
        return;
    }

    auto index = 0;
    while (index < materials.size()) {
        auto last_index = std::min(index + 50, (int)materials.size());
        insertMultiMaterials(std::vector<mj::model::MaterialInfo>(materials.begin() + index, materials.begin() + last_index));
        index = last_index;
    }
}

void StoryDataModel::insertMultiMaterials(const std::vector<mj::model::MaterialInfo>& materials) {
    std::string query = "REPLACE INTO tbl_material(mat_id,type,lesson_id,date_published,name_worksheet,name_lesson,thumb,levels,categories) VALUES ";
    for (auto i = 0; i < materials.size(); i++) {
        query += "(?,?,?,?,?,?,?,?,?),";
    }
    query.resize(query.length() - 1);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    int placeholder_index = 0;
    for (auto mat : materials) {
        sqlite3_bind_int(this->stmp, ++placeholder_index, mat.mat_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, (int)mat.type);
        sqlite3_bind_int(this->stmp, ++placeholder_index, mat.lesson_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, mat.date_published);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mat.name_worksheet.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mat.name_lesson.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mat.thumb.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mj::helper::vectorIntToString(mat.levels).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, mj::helper::vectorIntToString(mat.categories).c_str(), -1, SQLITE_TRANSIENT);
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }

    CCLOG("db mem used: %lld", sqlite3_memory_used());
    CCLOG("insert materials = done");
}

void StoryDataModel::deleteMaterials(const std::vector<int>& ids) {
    if (ids.empty()) {
        return;
    }
    std::string query = "DELETE FROM tbl_material WHERE mat_id IN (";
    query = std::accumulate(ids.begin(), ids.end(), query, [](std::string q, int id) {
        return q += StringUtils::toString(id) + ",";
        });
    query.resize(query.length() - 1);
    query += ")";

    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }

    CCLOG("Deleted multi materials");
}

void StoryDataModel::sortMaterials(std::vector<mj::model::MaterialInfo>& vector) {
    int min = 0;
    if (vector.size() <= 0)
        return;
    int i = (int)vector.size() - 1;
    while (i >= min) {
        auto info = &vector.at(i);

        bool is_readed = false;

        if (info->type == mj::model::MaterialInfo::Type::STORY) {
            is_readed = getStoryById(info->lesson_id).is_read;
        }
        else if (info->type == mj::model::MaterialInfo::Type::PHONICS) {
            auto lesson_info = ms::LessonManager::getInstance().getLessonById(info->lesson_id);
            is_readed = lesson_info.is_learned || lesson_info.state == mj::model::LessonInfo::State::LEARNED;
        }

        info->is_readed = is_readed;

        if (is_readed) {
            auto vl = *info;
            vector.erase(vector.begin() + i);
            vector.insert(vector.begin(), vl);
            min++;
        }
        else {
            i--;
        }
    }
}

std::vector<mj::model::MaterialInfo> StoryDataModel::getAllMaterialsByType(mj::model::MaterialInfo::Type type, bool sort_by_readed) {
    std::vector<mj::model::MaterialInfo> vector;
    std::string query = "select * FROM tbl_material WHERE type = " + StringUtils::toString((int)type) + " ORDER BY date_published DESC";

    // lấy từ database và cho vào vector
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::MaterialInfo info;
        info.mat_id = sqlite3_column_int(this->stmp, 0);
        info.type = info.toMaterialType(sqlite3_column_int(this->stmp, 1));
        info.lesson_id = sqlite3_column_int(this->stmp, 2);
        info.date_published = sqlite3_column_int(this->stmp, 3);
        info.name_worksheet = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.name_lesson = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 5));
        info.thumb = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 6));
        if(info.type == mj::model::MaterialInfo::Type::STORY)
            info.thumb = mj::helper::getUrlFromDomain(info.thumb, mj::UrlType::MSURL_STORY);
        else if(info.type == mj::model::MaterialInfo::Type::PHONICS)
        {
            info.thumb = mj::helper::getUrlFromDomain(info.thumb, mj::UrlType::MSURL_GAME);
        }
        info.levels = mj::helper::stringToVectorInt(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 7)));
        info.categories = mj::helper::stringToVectorInt(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 8)));

        vector.push_back(info);
    }

    // sort lại vector
    if (sort_by_readed) {
        sortMaterials(vector);
    }

    return vector;
}

std::vector<mj::model::MaterialInfo> StoryDataModel::getAllMaterialsByTypeAndLevel(mj::model::MaterialInfo::Type type, int level, bool sort_by_readed) {
    std::vector<mj::model::MaterialInfo> vector = getAllMaterialsByType(type, false);

    int i = 0;
    while (i < vector.size()) {
        std::vector<int> levels;

        // với STORY thì level nằm trong thuộc tính levels,
        // với PHONICS thì level nằm trong thuộc tính categories
        if (type == mj::model::MaterialInfo::Type::STORY) {
            levels = vector.at(i).levels;
        }
        else if (type == mj::model::MaterialInfo::Type::PHONICS) {
            levels = vector.at(i).categories;
        }

        if (std::find(levels.begin(), levels.end(), level) == levels.end()) {
            vector.erase(vector.begin() + i);
        }
        else {
            i++;
        }
    }

    // sort lại vector
    if (sort_by_readed) {
        sortMaterials(vector);
    }

    return vector;
}

std::vector<mj::model::MaterialPopularInfo> StoryDataModel::getPopularMaterials(mj::model::MaterialPopularInfo::Type type) {
    std::vector<mj::model::MaterialPopularInfo> popular_materials;

    std::string query = "select * FROM tbl_material_popular WHERE type=" + std::to_string((int)type) + " ORDER BY sort ASC";

    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        mj::model::MaterialPopularInfo info;
        info.popular_id = sqlite3_column_int(this->stmp, 0);
        info.date_published = sqlite3_column_int(this->stmp, 1);
        info.type = info.toMaterialPopularType(sqlite3_column_int(this->stmp, 2));
        info.sort = sqlite3_column_int(this->stmp, 3);
        info.keyword = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 4));
        info.thumb = reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 5));
        info.thumb = mj::helper::getUrlFromDomain(info.thumb, mj::UrlType::MSURL_MEDIA);
        popular_materials.push_back(info);
    }

    return popular_materials;
};

void StoryDataModel::setPopularMaterials(const std::vector<mj::model::MaterialPopularInfo>& vector) {
    insertPopularMaterialsWithVector(vector);
}

void StoryDataModel::insertPopularMaterialsWithVector(const std::vector<mj::model::MaterialPopularInfo>& populars) {
    if (populars.empty()) {
        return;
    }

    // delete all popular materials
    std::string query = "DELETE FROM tbl_material_popular";

    auto rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        this->showError(rc);
        return;
    }
    CCLOG("Deleted all popular materials");

    // insert new popular materials
    auto index = 0;
    while (index < populars.size()) {
        auto last_index = std::min(index + 50, (int)populars.size());
        insertMultiPopularMaterials(std::vector<mj::model::MaterialPopularInfo>(populars.begin() + index, populars.begin() + last_index));
        index = last_index;
    }
}

void StoryDataModel::insertMultiPopularMaterials(const std::vector<mj::model::MaterialPopularInfo>& populars) {
    std::string query = "REPLACE INTO tbl_material_popular(popular_id,date_published,type,sort,keyword,thumb) VALUES ";
    for (auto i = 0; i < populars.size(); i++) {
        query += "(?,?,?,?,?,?),";
    }
    query.resize(query.length() - 1);

    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if (result != SQLITE_OK) {
        this->showError(result);
        return;
    }

    int placeholder_index = 0;
    for (auto pop : populars) {
        sqlite3_bind_int(this->stmp, ++placeholder_index, pop.popular_id);
        sqlite3_bind_int(this->stmp, ++placeholder_index, pop.date_published);
        sqlite3_bind_int(this->stmp, ++placeholder_index, (int)pop.type);
        sqlite3_bind_int(this->stmp, ++placeholder_index, pop.sort);
        sqlite3_bind_text(this->stmp, ++placeholder_index, pop.keyword.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(this->stmp, ++placeholder_index, pop.thumb.c_str(), -1, SQLITE_TRANSIENT);
    }

    result = sqlite3_step(this->stmp);
    if (result != SQLITE_DONE) {
        this->showError(result);
        return;
    }

    CCLOG("db mem used: %lld", sqlite3_memory_used());
    CCLOG("insert popular materials = done");
}

void StoryDataModel::deleteTableGameListError()
{
    string query = "DELETE FROM tbl_game_list_error";
    ValueVector vector_query;
    executeQuery(query, vector_query);
}

void StoryDataModel::insertTableGameListError(std::vector<int> listGameError)
{
    if (listGameError.size() <= 0)
        return;
    deleteTableGameListError();
    std::string query = "replace into tbl_game_list_error (game_id) values ";
    query = std::accumulate(std::begin(listGameError), std::end(listGameError), query, [](std::string q, int idGame) {
        return q + "(?) ,";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    };

    auto counter = 0;

    for (int i = 0; i < listGameError.size(); i++)
    {
        sqlite3_bind_int(this->stmp, ++counter, listGameError[i]);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }
}

std::vector<int> StoryDataModel::getListGameError()
{
    std::string query = "select * FROM tbl_game_list_error";

    std::vector<int> lisGameError = {};

    // lấy từ database và cho vào vector
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        int idGame = sqlite3_column_int(this->stmp, 0);
        lisGameError.push_back(idGame);
    }
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    lisGameError.push_back(184);
    lisGameError.push_back(1488);
    lisGameError.push_back(66);
#endif

    return lisGameError;
}

void StoryDataModel::insertTableListDomainStories(std::vector<std::string> listUrl, StoryDataModel::TYPEURL type)
{
    deleteTableListDomainStories(type);

    std::string nameTable = "tbl_list_doamin_stories_dowload";

    if (type == StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME)
    {
        nameTable = "tbl_list_doamin_audiobook_activity_dowload";
    }

    std::string query = "replace into " + nameTable + " (domain_url)values ";
    query = std::accumulate(std::begin(listUrl), std::end(listUrl), query, [](std::string q, std::string url) {
        return q + "(?) ,";
        });
    query.resize(query.length() - 1);

    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
    };

    auto counter = 0;

    for (int i = 0; i < listUrl.size(); i++)
    {
        sqlite3_bind_text(this->stmp, ++counter, listUrl[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
    }
}

std::vector<std::string> StoryDataModel::getListDomainStories(StoryDataModel::TYPEURL type)
{

    std::string nameTable = "tbl_list_doamin_stories_dowload";

    if (type == StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME)
    {
        nameTable = "tbl_list_doamin_audiobook_activity_dowload";
    }

    std::string query = "select * FROM " + nameTable;

    std::vector<std::string> lisUrl = {};

    // lấy từ database và cho vào vector
    sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        ;
        std::string url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->stmp, 0)));
        lisUrl.push_back(url);
    }

    return lisUrl;
}

void StoryDataModel::deleteTableListDomainStories(StoryDataModel::TYPEURL type)
{
    std::string nameTable = "tbl_list_doamin_stories_dowload";

    if (type == StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME)
    {
        nameTable = "tbl_list_doamin_audiobook_activity_dowload";
    }

    string query = "DELETE FROM " + nameTable;
    ValueVector vector_query;
    executeQuery(query, vector_query);
}

std::string StoryDataModel::getUrlOfDomain(std::string url , mj::UrlType type, int index, std::vector<std::string> listDomain, std::string typeError, std::pair<int, std::string> valueError)
{
    if (valueError.second.find(url) != std::string::npos && index != -1)
    {
        url = mj::helper::getUrlFromDomain(url, type, index, listDomain, typeError);
    }
    else {
        url = mj::helper::getUrlFromDomain(url, type);
    }
    
    return url;
}
