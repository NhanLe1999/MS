//
//  DatabaseHelper.cpp
//  Demo
//
//  Created by AnhBNG on 8/20/19.
//
//

#include "DatabaseHelper.h"
#include "GlobalFunc.h"
#include "HSLib.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "StoryFreeManager.h"
#include "APProfileManager.h"
#include <numeric>

#define SQLITE_COLUMN_INT(x) sqlite3_column_int(this->stmp, x)
#define SQLITE_COLUMN_STR(x) std::string(reinterpret_cast<const char *>(sqlite3_column_text(this->stmp, x)))

#define SQLITE_COLUMN_INT_STMT(x, y) sqlite3_column_int(x, y)
#define SQLITE_COLUMN_STR_STMT(x, y) std::string(reinterpret_cast<const char *>(sqlite3_column_text(x, y)))

#define SQLITE_BIND_INT(x, v) sqlite3_bind_int(this->stmp, x, v)
#define SQLITE_BIND_STR(x, v) sqlite3_bind_text(this->stmp, x, v.c_str(), -1, SQLITE_TRANSIENT)

DatabaseHelper* DatabaseHelper::instance = NULL;
DatabaseHelper* DatabaseHelper::getInstance(string dataName)
{
    if (instance == nullptr) {
        instance = new DatabaseHelper();
        string dbpath = FileUtils::getInstance()->getWritablePath() + "profile/" + dataName + ".sqlite";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//        if(dataName == "cards") {
//            dbpath = FileUtils::getInstance()->getWritablePath() + "Documents/" + "profile/" + dataName + ".sqlite";
//        }
#endif
        instance->db_path = dbpath;
        instance->table_name = dataName;
        instance->openConnect();
        instance->prepareDB();
    }
    return instance;
}
void DatabaseHelper::prepareDB(){
    std::string  query = "create table if not exists tbl_profile (key text not null, value text,  primary key (key))";
    if (sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        logError();
    } else {
        CCLOG("Created table tbl_profile");
    }
}
void DatabaseHelper::openConnect() {
    if(sqlite3_open(this->db_path.c_str(), &this->db) != SQLITE_OK) {
        sqlite3_close(this->db);
        CCLOG("Connect DB co loi");
    } else {
        sqlite3_exec(this->db, "PRAGMA journal_mode = MEMORY", nullptr, nullptr, nullptr);
        CCLOG("Connect DB thanh cong");
    }
}

void DatabaseHelper::closeConnect() {
    sqlite3_close(this->db);
}

void DatabaseHelper::showError(int result) {
    CCLOG("Database error: %d", result);
}

void DatabaseHelper::logError() {
    CCLOG("Database Error: %s", sqlite3_errmsg(this->db));
}

void DatabaseHelper::executeQuery(string query, ValueVector query_vector) {
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    
    if(result != SQLITE_OK) {
        this->showError(result);
        return;
    }
    
    //for qua cac phan tu roi bind vao stmp
    int placeholder_index = 1;
    
    for(int i = 0; i < query_vector.size(); i++) {
        cocos2d::Value::Type vType = query_vector.at(i).getType();
        if(vType == cocos2d::Value::Type::STRING) {
            sqlite3_bind_text(this->stmp, placeholder_index++, query_vector.at(i).asString().c_str(), -1, SQLITE_TRANSIENT);
        }else if(vType == cocos2d::Value::Type::INTEGER) {
            sqlite3_bind_int(this->stmp, placeholder_index++, query_vector.at(i).asInt());
        }else if(vType == cocos2d::Value::Type::FLOAT || vType == cocos2d::Value::Type::DOUBLE) {
            sqlite3_bind_double(this->stmp, placeholder_index++, query_vector.at(i).asDouble());
        }
    }
    
    result = sqlite3_step(this->stmp);
    if(result != SQLITE_DONE) {
        this->showError(result);
        return;
    }
}

ValueMap DatabaseHelper::getRow(string query, ValueVector query_vector) {
    ValueMap row;
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if(result != SQLITE_OK) {
        this->showError(result);
        return row;
    }
    
    //for qua cac phan tu roi bind vao stmp
    int placeholder_index = 1;
    for(int i = 0; i < query_vector.size(); i++) {
        cocos2d::Value::Type vType = query_vector.at(i).getType();
        if(vType == cocos2d::Value::Type::STRING) {
            sqlite3_bind_text(this->stmp, placeholder_index++, query_vector.at(i).asString().c_str(), -1, SQLITE_TRANSIENT);
        }else if(vType == cocos2d::Value::Type::INTEGER) {
            sqlite3_bind_int(this->stmp, placeholder_index++, query_vector.at(i).asInt());
        }else if(vType == cocos2d::Value::Type::FLOAT || vType == cocos2d::Value::Type::DOUBLE) {
            sqlite3_bind_double(this->stmp, placeholder_index++, query_vector.at(i).asDouble());
        }
    }
    
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        row = fetchRow();
    }
    return row;
}

ValueMap DatabaseHelper::fetchRow() {
    ValueMap row;
    int column_count = sqlite3_column_count(this->stmp);
    for(int i = 0; i < column_count; i++) {
        int column_type = sqlite3_column_type(this->stmp, i);
        string column_name = sqlite3_column_name(this->stmp, i);
        if(column_type == SQLITE_INTEGER) {
            row[column_name] = sqlite3_column_int(this->stmp, i);
        }else if(column_type == SQLITE_FLOAT) {
            row[column_name] = sqlite3_column_double(this->stmp, i);
        }else if(column_type == SQLITE_TEXT) {
            row[column_name] = (const char*)sqlite3_column_text(this->stmp, i);
        } else {
            row[column_name] = "";
        }
    }
    return row;
}

ValueVector DatabaseHelper::fetchAllRecord(string query) {
    ValueVector arrayData;
    this->stmp = nullptr;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if(result != SQLITE_OK) {
        this->showError(result);
        return arrayData;
    }
    ValueMap dict;
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        dict.clear();
        dict = fetchRow();
        arrayData.push_back(Value(dict));
    }
    closeConnect();
    return arrayData;
}

ValueVector DatabaseHelper::getAllDatabase(){
    string query = StringUtils::format("SELECT * FROM tbl_profile");
    return fetchAllRecord(query);
}

void DatabaseHelper::insertProfileInfo(std::vector<std::pair<std::string, std::string>> profileInfo){
    if (profileInfo.size() > 50) {
        auto first = 0;
        const auto chunk_size = 50;
        while (first < profileInfo.size()) {
            auto last = std::min(first + chunk_size, (int) profileInfo.size());
            insertProfileInfo(std::vector<std::pair<std::string, std::string>>( profileInfo.begin()+first, profileInfo.begin()+last));
            first = last;
        }
        
        return;
    }
    
    std::string query = "replace into tbl_profile (key, value) values ";
    query = std::accumulate(std::begin(profileInfo), std::end(profileInfo), query, [](std::string q, std::pair<std::string, std::string> i) {
        return q.append("(?, ?),");
    });
    query.resize(query.length() - 1);
    if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr) != SQLITE_OK) {
        logError();
        return;
    }
    
    auto counter = 0;
    for (auto object : profileInfo) {
        SQLITE_BIND_STR(++counter, object.first);
        SQLITE_BIND_STR(++counter, XHSLib->mahoaString(object.second));
    }
    
    if (sqlite3_step(this->stmp) != SQLITE_DONE) {
        logError();
        return;
    }else{
        CCLOG("insert profile done");
        closeConnect();
        instance = nullptr;
    }
}
