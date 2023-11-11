//
//  TestingModeUtils.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 11/21/17.
//

#include "TestingModeUtils.h"


testing::TestManager* instance;

testing::TestManager* testing::TestManager::getInstance() {
    if (!instance) {
        instance = new testing::TestManager();
    }
    return instance;
}

void testing::TestManager::setTestMode(testing::TestMode _mode) {
    mode = _mode;
}

testing::TestMode testing::TestManager::getTestMode() {
    return mode;
}

void testing::TestManager::setGrade(int _grade_id) {
    grade_id = _grade_id;
}

int testing::TestManager::getGrade() {
    return this->grade_id;
}

void testing::TestManager::setClassname(std::string name) {
    CCLOG("%s", class_name.c_str());
    class_name = name;
}

std::string testing::TestManager::getClassName() {
    CCLOG("%s", class_name.c_str());
    return class_name;
}

void testing::TestManager::setActivity(mj::network::api::Activity _activity) {
    activity = _activity;
}

mj::network::api::Activity testing::TestManager::getActivity() {
    return activity;
}

