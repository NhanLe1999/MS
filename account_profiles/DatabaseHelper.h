//
//  DatabaseHelper.h
//  Demo
//
//  Created by AnhBNG on 8/20/19.
//
//

#ifndef __DatabaseHelper_H__
#define __DatabaseHelper_H__
//USING_NS_CC;
#include "cocos2d.h"
#include "sqlite3.h"
#include "JsonDump.h"
#include "base_include.h"
#include "MJStoryInfoModel.h"
#include "LicenseInfoModel.h"
#include "MJLicensePackage.h"

#define max_id_stories 1000000
#define max_id_audiobook 2000000

using namespace std;
class DatabaseHelper {
public:
    static DatabaseHelper* instance;
    static DatabaseHelper* getInstance(string dataName = "");
    
    void prepareDatabase();

    void logError();
  
    ValueVector getAllDatabase();
    void insertProfileInfo(std::vector<std::pair<std::string, std::string>> profileInfo);
    void prepareDB();
	void closeConnect();
    void openConnect();

   
private:
    sqlite3 *db;
    sqlite3_stmt *stmp;
    std::string db_path = "";
    std::string table_name = "";
    void showError(int result);
    
    std::vector<int> _audiobooks_with_chapters;
    std::vector<int> _chapter_of_audiobooks;
    
    ValueMap getRow(string query, ValueVector query_vector);
    void executeQuery(string query, ValueVector query_vector);
    ValueMap fetchRow();
    ValueVector fetchAllRecord(string query);
    
    
    
};


#endif /* __DatabaseHelper_H__ */
