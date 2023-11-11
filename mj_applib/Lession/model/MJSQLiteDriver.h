//
//  MJSQLiteDriver.h
//  M
//
//  Created by To Ngoc Linh on 4/29/17.
//
//

#ifndef MJSQLiteDriver_h
#define MJSQLiteDriver_h

#include <stdio.h>
#include "sqlite_modern_cpp.h"
#include "cocos2d.h"
#include "MJCategory.h"
#include "MJGame.h"

namespace mj {
    namespace model {
        class DBDriver {
        public:
            static DBDriver & getInstance() {
                static DBDriver driver;
                return driver;
            }

            std::vector<Category> readCourseInfo();

            sqlite::database createUserPreferences();

            void setCurrentGrade(int grade_id);
            void setCurrentSkill(int skill_id);
            void setCurrentUnit(int unit_id);
            void setCurrentLesson(int lesson_id);
            
        private:
            DBDriver();
            DBDriver(DBDriver const &);
            void operator = (DBDriver const &);

            void copyDBToDocumentFolder();

            sqlite::database openReadOnlyDatabase();
            void closeDatabase();
        };
    }
}

#endif /* MJSQLiteDriver_h */
