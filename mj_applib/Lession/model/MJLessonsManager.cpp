//
//  MJLessonsManager.cpp
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#include "MJLessonsManager.h"
//#ifdef MJSQLiteDriver_h
#include "MJSQLiteDriver.h"
//#endif
#include <numeric>
mj::model::Manager::Manager(){
}

/**
 *
 *
 *
 */
void mj::model::Manager::loadDataModel() {
    _grades = mj::model::DBDriver::getInstance().readCourseInfo();
}

//TODO: re-write this func
mj::model::Category mj::model::Manager::getCategoryById(int categoryid) {
    auto r = std::find_if(_categories.begin(), _categories.end(), [&categoryid](const Category &g) {
        return g.getId() == categoryid;
    });
    return (*r);
}

#pragma mark - Grade
mj::model::Category mj::model::Manager::getGradeAtIndex(int index) {
    return _grades[index];
}

void mj::model::Manager::setCurrentGrade(int gradeid) {
    _current_grade = getCategoryById(gradeid);
    mj::model::DBDriver::getInstance().setCurrentGrade(gradeid);
}

mj::model::Category mj::model::Manager::getCurrentGrade() {
    return _current_grade;
}

int mj::model::Manager::getCurrentTotalSkills() {
    return static_cast<int>(_current_grade.getChildrent().size());
}

#pragma mark - SKill
void mj::model::Manager::setCurrentSkill(int skillid) {
    _current_skill = getCategoryById(skillid);
    mj::model::DBDriver::getInstance().setCurrentSkill(skillid);
}

mj::model::Category mj::model::Manager::getCurrentSkill() {
    return _current_skill;
}

mj::model::Category mj::model::Manager::getGradeOfSkill(int skillid) {
    auto skill = getCategoryById(skillid);
    return getCategoryById(skill.getParentId());
}

#pragma mark - Unit
void mj::model::Manager::setCurrentUnit(int unitid) {
    _current_unit = getCategoryById(unitid);
    mj::model::DBDriver::getInstance().setCurrentUnit(unitid);
}

mj::model::Category mj::model::Manager::getCurrentUnit() {
    return _current_unit;
}

mj::model::Category mj::model::Manager::getSkillOfUnit(int unitid) {
    auto unit = getCategoryById(unitid);
    return getCategoryById(unit.getParentId());
}

#pragma mark - Lesson
mj::model::Lesson mj::model::Manager::getLessonById(int lessonid) {
    mj::model::Lesson lesson;
    return lesson;
}

void mj::model::Manager::setCurrentLesson(int lessonid) {
    _current_lesson = getLessonById(lessonid);
    mj::model::DBDriver::getInstance().setCurrentLesson(lessonid);
}

mj::model::Lesson mj::model::Manager::getCurrentLesson() {
    return _current_lesson;
}

mj::model::Category mj::model::Manager::getUnitOfLesson(int lessonid) {
    auto lesson = getLessonById(lessonid);
    return getCategoryById(lesson.getUnitId());
}

#pragma mark - Activity
mj::model::Activity mj::model::Manager::getActivityById(int activityid) {
    mj::model::Activity activity;
    return activity;
}

#pragma mark - Game
mj::model::Game mj::model::Manager::getGameById(int gameid) {
    auto r = std::find_if(_games.begin(), _games.end(), [&gameid](const Game &g) {
        return g.getId() == gameid;
    });
    return (*r);
}

mj::model::Game mj::model::Manager::getGameByName(std::string name) {
    auto r = std::find_if(_games.begin(), _games.end(), [&name](const Game &g) {
        return g.getName() == name;
    });
    return (*r);
}
