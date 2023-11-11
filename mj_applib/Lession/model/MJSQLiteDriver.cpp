//
//  MJSQLiteDriver.cpp
//  M
//
//  Created by To Ngoc Linh on 4/29/17.
//
//

#include "MJSQLiteDriver.h"
#include "cocos2d.h"
#include <iostream>
#include <string>

mj::model::DBDriver::DBDriver() {
    try {

    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}

std::vector<mj::model::Category> mj::model::DBDriver::readCourseInfo() {

    //prepare read-write database for user preferences
    auto db_readwrite = createUserPreferences();

    std::vector<mj::model::Category> grades;
    auto db_readonly = openReadOnlyDatabase();

    /*
    db_readonly << "select _id, name, _order from tbl_grade" >> [&db_readonly, &grades, &db_readwrite](int grade_id, std::string grade_name, int grade_order) {
        db_readwrite << "insert or ignore into tbl_grade_extras (_id) values (?) " << grade_id;

        auto grade = mj::model::Grade(grade_id, grade_name, grade_order);
        db_readonly << "select _id, name, _order, grade_id from tbl_skill where grade_id = ? " << grade_id >> [&db_readonly, &grade, &db_readwrite](int skill_id, std::string skill_name, int skill_order, int grade_id) {
            assert(grade_id == grade.getId());
            
            db_readwrite << "insert or ignore into tbl_skill_extras (_id) values (?)" << skill_id;
            auto skill = mj::model::Skill(skill_id, skill_name, skill_order);
            skill.setGradeId(grade_id);
            db_readonly << "select _id, name, _order, skill_id, grade_id from tbl_level where skill_id = ? " << skill_id >> [&db_readonly, &skill, &db_readwrite](int level_id, std::string level_name, int level_order, int skill_id, int grade_id) {
                assert(grade_id == skill.getGradeId());
                assert(skill_id == skill.getId());
                
                db_readwrite << "insert or ignore into tbl_unit_extras (_id) values (?)" << level_id;

                auto level = mj::model::Level(level_id, level_name, level_order);
                level.setGradeId(skill.getGradeId());
                level.setSkillId(skill.getId());
                db_readonly << "select _id, tbl_game.name, _order, level_id, skill_id, grade_id, class_name, tbl_game_type.schema_path "
                        "from tbl_game inner join tbl_game_type on tbl_game.class_name = tbl_game_type.name "
                        "where level_id = ? " << level_id >> [&level, &db_readwrite](int game_id, std::string game_name, int game_order, int level_id, int skill_id, int grade_id, std::string class_name, std::string schema_path) {
                    assert(level_id == level.getId());
                    assert(skill_id == level.getSkillId());
                    assert(grade_id == level.getGradeId());
                    
                    db_readwrite << "insert or ignore into tbl_lesson_extras (_id) values (?)" << game_id;

                    //auto game = mj::model::Game(game_id, game_name, "", "");
                    auto game = mj::model::Game(game_id, game_name, schema_path);
//                    game.setLevelId(level.getId());
//                    game.setSkillId(level.getSkillId());
//                    game.setGradeId(level.getGradeId());
                    game.setClassName(class_name);
                            
                    level.addGame(game);
                };

                skill.addLevel(level);
            };

            grade.addSkill(skill);
        };

        grades.push_back(grade);
    };
     */
    
//    getGames("abc", 1);
//    getDifferentGameClass(1);
     
    return grades;
}

//should not throw exception
sqlite::database mj::model::DBDriver::openReadOnlyDatabase() {
    auto sql_file = cocos2d::FileUtils::getInstance()->fullPathForFilename("res/math_db.db");
//    struct sqlite::sqlite_config config;
//    config.flags = sqlite::OpenFlags::READONLY;
    sqlite::database db = sqlite::database(sql_file);
	//sqlite::database(sql_file);
    
    return db;
}

//should not throw exception
sqlite::database mj::model::DBDriver::createUserPreferences() {
    auto storage = cocos2d::FileUtils::getInstance()->getWritablePath() + "user_prefs.sqlite";
    std::cout << storage << std::endl;
    auto db_rw = sqlite::database(storage);
    db_rw <<   "create table if not exists tbl_lesson_extras ("
            "_id integer primary key not null,"
            "_save integer(1) not null,"
            "_count integer not null,"
            "last_played integer"
            ");";
    db_rw <<   "create table if not exists tbl_unit_extras ("
            "_id integer primary key not null,"
            "_save integer(1) not null,"
            "_count integer not null,"
            "last_played integer"
            ");";
    db_rw <<   "create table if not exists tbl_skill_extras ("
            "_id integer primary key not null,"
            "_save integer(1) not null"
            ");";
    db_rw <<   "create table if not exists tbl_grade_extras ("
            "_id integer primary key not null,"
            "_save integer(1) not null"
            ");";
    return db_rw;
}

void mj::model::DBDriver::closeDatabase() {
    // TODO: close db
}

void mj::model::DBDriver::setCurrentGrade(int grade_id) {
    auto storage = cocos2d::FileUtils::getInstance()->getWritablePath() + "user_prefs.sqlite";

    auto db_rw = sqlite::database(storage);
    db_rw << "update tbl_grade_extras set _save = 0";
    db_rw << "update tbl_grade_extras set _save = 1 where _id = ?" << grade_id;
}

void mj::model::DBDriver::setCurrentSkill(int skill_id) {
    auto storage = cocos2d::FileUtils::getInstance()->getWritablePath() + "user_prefs.sqlite";
    auto db_rw = sqlite::database(storage);
    db_rw << "update tbl_skill_extras set _save = 0";
    db_rw << "update tbl_skill_extras set _save = 1 where _id = ?" << skill_id;
}

void mj::model::DBDriver::setCurrentUnit(int unit_id) {
    auto storage = cocos2d::FileUtils::getInstance()->getWritablePath() + "user_prefs.sqlite";
    auto db_rw = sqlite::database(storage);
    db_rw << "update tbl_unit_extras set _save = 0";
    db_rw << "update tbl_unit_extras set _save = 1 where _id = ?" << unit_id;
}

void mj::model::DBDriver::setCurrentLesson(int lesson_id) {
    auto storage = cocos2d::FileUtils::getInstance()->getWritablePath() + "user_prefs.sqlite";
    auto db_rw = sqlite::database(storage);
    db_rw << "update tbl_lesson_extras set _save = 0";
    db_rw << "update tbl_lesson_extras set _save = 1 where _id = ?" << lesson_id;
}

//std::vector<std::string> mj::model::DBDriver::getDifferentGameClass(int grade_id) {
//    auto db_r = openReadOnlyDatabase();
//    std::vector<std::string> list;
//    db_r << "select tbl_game_type.name from tbl_game_type inner join tbl_game on tbl_game_type.name = tbl_game.class_name where tbl_game.grade_id = ? group by tbl_game_type.name" << grade_id >> [&list](std::string name) {
//        list.push_back(name);
//    };
//
//    return list;
//}
//
//std::vector<mj::model::Game> mj::model::DBDriver::getGames(std::string class_name, int grade_id) {
//    auto db_r = openReadOnlyDatabase();
//    std::vector<mj::model::Game> list;
//    db_r << "select _id, level_id, skill_id, grade_id, tbl_game.name, _order, class_name, tbl_game_type.schema_path from tbl_game inner join tbl_game_type on tbl_game.class_name = tbl_game_type.name where tbl_game.class_name = ? and tbl_game.grade_id = ?" << class_name << grade_id >> [&list](int game_id, int level_id, int skill_id, int grade_id, std::string game_name, int game_order, std::string class_name, std::string schema_path) {
//        auto game = mj::model::Game(game_id, game_name, schema_path);
////        game.setGradeId(grade_id);
////        game.setSkillId(skill_id);
////        game.setLevelId(level_id);
//        game.setClassName(class_name);
//        list.push_back(game);
//    };
//
//    return list;
//}
