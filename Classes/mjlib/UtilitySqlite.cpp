//
//  UtilitySqlite.cpp
//  UtilitiesDemo
//
//  Created by HuyPhuong on 3/24/15.
//
//

#include "UtilitySqlite.h"

#include "MJHelper.h"
USING_NS_CC;
using namespace std;

static UtilitySqlite *shareInstance1 = NULL;
UtilitySqlite* UtilitySqlite::shareInstance() {
    if (!shareInstance1) {
        shareInstance1 = new UtilitySqlite();
    }
    return shareInstance1;
}


UtilitySqlite::UtilitySqlite()
{
    database=NULL;
}
#pragma mark Utility Sqlite
bool UtilitySqlite::onOpenDatabase()
{
    string database_path = FileUtils::getInstance()->getWritablePath()+"mjdefault.sqlite";
    FileUtils *fileManager = FileUtils::getInstance();
    if (!fileManager->isFileExist(database_path)){
        Data db_data = fileManager->getDataFromFile("mjdefault.sqlite");
        fileManager->writeDataToFile(db_data, database_path);
    }
    
    if (sqlite3_open(database_path.c_str(), &database) != SQLITE_OK) {
        CCLOG("UtilitySqlite - Open fail");
    }
    else{
        CCLOG("UtilitySqlite - Open success");
		// data đã encrypt khi insert không cần encrypt ở đây nữa
        //encryptData();
    }
    return true;
}

#include "HSLib.h"
void UtilitySqlite::encryptData() {
    auto temp = getValueFromKey("dbee");
    if (!temp.empty()) {
        //đã encrypt rồi
        return;
    }
    
    ValueMap map;
    string query = "SELECT mj_key, mj_value FROM mjdefault";
    sqlite3_stmt *stmp = NULL;
    string strReturn = "";
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmp, NULL);
    if(result != SQLITE_OK) {
        CCLOG("getValueFromKey Error: %i", result);
        CCLOG("Database Error: %s", sqlite3_errmsg(database));
        return ;
    }
    
    std::vector<std::pair<std::string, std::string> > key_value;
    
    while (sqlite3_step(stmp) == SQLITE_ROW) {
        auto key = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmp, 0)));
        auto value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmp, 1)));
        key_value.push_back(std::make_pair(key, value));
    }
    
    deleteData("mjdefault");
    
    key_value.push_back(std::make_pair("dbee", StringUtils::toString(utils::getTimeInMilliseconds())));
    for (auto p : key_value) {
        string query = StringUtils::format("REPLACE INTO mjdefault (mj_key, mj_value) VALUES (?,?)");
        ValueVector query_vector;
        query_vector.push_back(Value(p.first));
        query_vector.push_back(Value(XHSLib->mahoaString(p.second)));
        UtilitySqlite::shareInstance()->executeQuery(query,query_vector);
    }
}

bool UtilitySqlite::onCloseDatabase()
{
    return true;
    int result = sqlite3_close(database);
    if (result != SQLITE_OK){
        CCLOG("UtilitySqlite - Close database fail: %d", result);
        return false;
    } else {
        CCLOG("UtilitySqlite - Close database successful");
        return true;
    }
}

void UtilitySqlite::disposeStmt(sqlite3_stmt *stmt)
{
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}

#pragma mark PREPARE DATABASE
sqlite3_stmt* UtilitySqlite::prepareDatabase(std::string query)
{
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - PREPARE data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - PREPARE data success");
    }
    return stmt;
}

#pragma mark get count
int UtilitySqlite::getCount(std::string table)
{    
    int count=0;
    string query = "SELECT COUNT(*) FROM "+table;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result == SQLITE_OK) {
        if (sqlite3_step(stmt)==SQLITE_ROW) {
            count=sqlite3_column_int(stmt, 0);
            CCLOG("UtilitySqlite - count table success: %d", count);
        } else {
            CCLOG("UtilitySqlite - count table not success");
        }
        disposeStmt(stmt);
    } else {
        CCLOG("UtilitySqlite - count table error: %d", result);
    }
    
    return count;
}

int UtilitySqlite::getCount(std::string table, std::string where)
{
    int count=0;
    string query = "SELECT COUNT(*) FROM "+table+" WHERE "+ where;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result == SQLITE_OK) {
        if (sqlite3_step(stmt)==SQLITE_ROW) {
            count=sqlite3_column_int(stmt, 0);
            CCLOG("UtilitySqlite - count table success: %d", count);
        } else {
            CCLOG("UtilitySqlite - count table not success");
        }
        disposeStmt(stmt);
    } else {
        CCLOG("UtilitySqlite - count table error: %d", result);
    }
    return count;
}

#pragma mark get data
sqlite3_stmt* UtilitySqlite::getData(std::string table)
{
    string query = "SELECT * FROM "+table;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - SELECT data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - SELECT data success");
    }
    return stmt;
}

sqlite3_stmt* UtilitySqlite::getData(std::string table, std::string select)
{
    string query = "SELECT "+select+" FROM "+table;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - SELECT data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - SELECT data success");
    }
    return stmt;
}

sqlite3_stmt* UtilitySqlite::getData(std::string table, std::string select, std::string where)
{
    string query = "SELECT "+select+" FROM "+table+ " WHERE "+where;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - SELECT data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - SELECT data success");
    }
    return stmt;
}

sqlite3_stmt* UtilitySqlite::getData(std::string table, std::string select, std::string where, std::string order)
{
    string query = "SELECT "+select+" FROM "+table+" WHERE "+where+" ORDER BY "+order;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - SELECT data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - SELECT data success");
    }
    return stmt;
}

sqlite3_stmt* UtilitySqlite::getData(std::string table, std::string select, std::string where, std::string order, int offset, int limit)
{
    string query = "SELECT "+select+" FROM "+table+" WHERE "+where+" ORDER BY "+order+" LIMIT "+StringUtils::toString(offset)+", "+ StringUtils::toString(limit);
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    sqlite3_stmt *stmt = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        CCLOG("UtilitySqlite - SELECT data error: %d", result);
    } else {
        CCLOG("UtilitySqlite - SELECT data success");
    }
    return stmt;
}

#pragma mark EXEC DATABASE
bool UtilitySqlite::execDatabase(std::string query)
{
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    int result=sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - EXEC Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - EXEC Error: %d", result);
        return false;
    }
}

#pragma mark insert data
bool UtilitySqlite::insertData(std::string table, std::string field_list, std::string value_list)
{
    string query = "INSERT INTO "+table+" ("+field_list+") VALUES ("+value_list+")";
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    int result=sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - INSERT Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - INSERT Error: %d", result);
        return false;
    }
}
void UtilitySqlite::executeQuery(string query, ValueVector query_vector) {
    sqlite3_stmt *stmp = NULL;
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmp, NULL);
    if(result != SQLITE_OK) {
        CCLOG("executeQuery Error: %i", result);
        return;
    }
    
    //for qua cac phan tu roi bind vao stmp
    int placeholder_index = 1;
    
    for(int i = 0; i < query_vector.size(); i++) {
        cocos2d::Value::Type vType = query_vector.at(i).getType();
        if(vType == cocos2d::Value::Type::STRING) {
            sqlite3_bind_text(stmp, placeholder_index++, query_vector.at(i).asString().c_str(), -1, SQLITE_TRANSIENT);
        }else if(vType == cocos2d::Value::Type::INTEGER) {
            sqlite3_bind_int(stmp, placeholder_index++, query_vector.at(i).asInt());
        }else if(vType == cocos2d::Value::Type::FLOAT || vType == cocos2d::Value::Type::DOUBLE) {
            sqlite3_bind_double(stmp, placeholder_index++, query_vector.at(i).asDouble());
        }
    }
    
    result = sqlite3_step(stmp);
    if(result != SQLITE_DONE) {
        CCLOG("executeQuery Error 2: %i", result);
        return;
    }
}

string UtilitySqlite::getValueFromKey(string skey) {
    ValueMap map;
    string query = "SELECT mj_value FROM mjdefault WHERE mj_key = ?";
    sqlite3_stmt *stmp = NULL;
    string strReturn = "";
    int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmp, NULL);
    if(result != SQLITE_OK) {
        CCLOG("getValueFromKey Error: %i", result);
		CCLOG("Database Error: %s", sqlite3_errmsg(database));

        return strReturn;
    }
    
    sqlite3_bind_text(stmp, 1, skey.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmp) == SQLITE_ROW) {
        strReturn = (const char*)sqlite3_column_text(stmp, 0);
    }
    return strReturn;
}



#pragma mark update data
bool UtilitySqlite::updateData(std::string table, std::string set)
{
    string query = "UPDATE "+table+" SET "+set;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    int result=sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - UPDATE Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - UPDATE Error: %d", result);
        return false;
    }
}

bool UtilitySqlite::updateData(std::string table, std::string set, std::string where)
{
    string query = "UPDATE "+table+" SET "+set+" WHERE "+where;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    int result=sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - UPDATE Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - UPDATE Error: %d", result);
        return false;
    }
}

#pragma mark delete data
bool UtilitySqlite::deleteData(std::string table)
{
    string query = "DELETE FROM "+table;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    int result=sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - DELETE Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - DELETE Error: %d", result);
        return false;
    }
}

bool UtilitySqlite::deleteData(std::string table, std::string where)
{
    string query = "DELETE FROM "+table+" WHERE "+where;
    CCLOG("UtilitySqlite - query: %s", query.c_str());
    
    int result=sqlite3_exec(database, query.c_str(),NULL,NULL,NULL);;
    if (result == SQLITE_OK) {
        CCLOG("UtilitySqlite - DELETE Success");
        return true;
    } else {
        CCLOG("UtilitySqlite - DELETE Error: %d", result);
        return false;
    }
}

void UtilitySqlite::setAllData(std::string info) {
    auto map = mj::helper::string_to_map(info);
	string query = StringUtils::format("INSERT INTO %s (mj_key, mj_value) VALUES (?,?)", "mjdefault");

	for (auto m : map) {
		ValueVector key_value;
		key_value.push_back(Value(m.first));
		key_value.push_back(Value(m.second));
		executeQuery(query, key_value);
	}
}

std::string UtilitySqlite::getAllData() {
	std::map<std::string, std::string> map;
	try {
		sqlite3_stmt* stmt = getData("mjdefault");

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			auto key = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
			auto value = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			map[key] = value;
			
			
		}
	}
	catch (std::exception& e) {
		std::cout << "db exception: " << e.what() << std::endl;

	}

    return mj::helper::map_to_string(map);
}

