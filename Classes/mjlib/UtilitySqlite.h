//
//  UtilitySqlite.h
//  UtilitiesDemo
//
//  Created by HuyPhuong on 3/24/15.
//
//

#ifndef __UtilitiesDemo__UtilitySqlite__
#define __UtilitiesDemo__UtilitySqlite__
#include <iostream>
#include "cocos2d.h"
#include "sqlite3.h"
#include <fstream>
using namespace std;
using namespace cocos2d;
class UtilitySqlite
{
public:
    UtilitySqlite();
    static UtilitySqlite *shareInstance();
    
    bool onOpenDatabase();
    bool onCloseDatabase();
    
    void disposeStmt(sqlite3_stmt *stmt);
    
    bool execDatabase(std::string query);
    sqlite3_stmt* prepareDatabase(std::string query);
    
    int  getCount(std::string table);
    int  getCount(std::string table, std::string where);
    
    sqlite3_stmt* getData(std::string table);
    sqlite3_stmt* getData(std::string table, std::string select);
    sqlite3_stmt* getData(std::string table, std::string select, std::string where);
    sqlite3_stmt* getData(std::string table, std::string select, std::string where, std::string order);
    sqlite3_stmt* getData(std::string table, std::string select, std::string where, std::string order, int offset, int limit);
    
    bool insertData(std::string table, std::string field_list, std::string value_list);
    
    bool updateData(std::string table, std::string set);
    bool updateData(std::string table, std::string set, std::string where);
    
    bool deleteData(std::string table);
    bool deleteData(std::string table, std::string where);

	void setAllData(std::string info);
    std::string getAllData();
    
    sqlite3*database;
    void executeQuery(string query, ValueVector query_vector);
    string getValueFromKey(string skey);
    
public:
    void encryptData();
};
#endif /* defined(__UtilitiesDemo__UtilitySqlite__) */
