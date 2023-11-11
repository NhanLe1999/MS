//
//  MJLessonsManager.h
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#ifndef MJLessonsManager_h
#define MJLessonsManager_h

#include <stdio.h> 
#include "cocos2d.h"
#include "MJCategory.h"
#include "MJGame.h"

namespace mj {
    namespace model {
        class Manager {
        public:
            static Manager& getInstance() {
                static Manager manager;
                return manager;
            }

            void loadDataModel();

        private:
            Manager();
            Manager(Manager const &);
            void operator = (Manager const &);

            //Grade
            CC_SYNTHESIZE_READONLY(std::vector<Category>, _grades, Grades);
            CC_SYNTHESIZE_READONLY(std::vector<Category>, _categories, Categories);
            CC_SYNTHESIZE_READONLY(std::vector<Game>, _games, Games);

            
            Category getCategoryById(int categoryid);
            inline int getTotalGrades() {return static_cast<int>(_grades.size());} //reused
            void setCurrentGrade(int gradeid);
            Category getCurrentGrade();
            Category getGradeAtIndex(int index);
            int getCurrentTotalSkills();
            
            //Skill
            void setCurrentSkill(int skillid);
            Category getCurrentSkill();
            Category getGradeOfSkill(int skillid);
            
            //Unit
            void setCurrentUnit(int unitid);
            Category getCurrentUnit();
            Category getSkillOfUnit(int unitid);

            //Lesson
            Lesson getLessonById(int lessonid);
            void setCurrentLesson(int lessonid);
            Lesson getCurrentLesson();
            Category getUnitOfLesson(int lessonid);
            
            //Activity
            Activity getActivityById(int activityid);
            
            //Game
            Game getGameById(int gameid);
            Game getGameByName(std::string name);

            Category _current_grade;
            Category _current_skill;
            Category _current_unit;
            Lesson   _current_lesson;
        };
    }
}

#endif /* MJLessonsManager_h */
