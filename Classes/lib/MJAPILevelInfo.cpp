//
// Created by To Ngoc Linh on 8/31/17.
//

#include "MJAPILevelInfo.h"
#include "MJHelper.h"
#include "MJStoryInfoModel.h"
#include "StoryDataModel.h"

void mj::LevelInfoAPI::call(std::function<void(bool)> callback) {
    auto api = new LevelInfoAPI();
    api->_callback = callback;
    api->send();
}

void mj::LevelInfoAPI::send() {
    if (_request) {
        _request->send();
    }
}

mj::LevelInfoAPI::LevelInfoAPI() {
    _request = new mj::Request(mj::Request::ActionType::LEVEL_INFO);
    _request->setDelegate(this);
}

void mj::LevelInfoAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    auto d = data.GetObject();

    std::vector<mj::model::LevelInfo> levels;

    std::vector<mj::model::GradeInfo> grades;
    std::unordered_map<int, std::pair<int, int> > grade_id_map;

    if (JSON_SAFE_CHECK(d, "grade", Object)) {
        auto grades_obj = d["grade"].GetObject();
        for (auto i = grades_obj.MemberBegin(); i != grades_obj.MemberEnd(); ++i) {
            if (!i->value.IsArray()) {
                continue;
            }
            auto lang_id = ATOI(i->name.GetString());
            auto grade_lang_arr = i->value.GetArray();
            for (auto c = 0; c < grade_lang_arr.Size(); c++) {
                if (!grade_lang_arr[c].IsObject()) {
                    continue;
                }
                auto grade_info_obj = grade_lang_arr[c].GetObject();

                mj::model::GradeInfo grade;
                if (JSON_SAFE_CHECK(grade_info_obj, "name", String)) {
                    grade.name = grade_info_obj["name"].GetString();
                }
                if (JSON_SAFE_CHECK(grade_info_obj, "des", String)) {
                    grade.description = grade_info_obj["des"].GetString();
                }
                if (JSON_SAFE_CHECK(grade_info_obj, "order", Int)) {
                    grade.grade_id = grade_info_obj["order"].GetInt();
                }
                grade.lang_id = lang_id;
                grades.push_back(grade);

                if (JSON_SAFE_CHECK(grade_info_obj, "id", Int)) {
                    grade_id_map.insert({grade_info_obj["id"].GetInt(), std::make_pair(grade.grade_id, grade.lang_id)});
                }
            }
        }
        StoryDataModel::getInstance()->insertGradeInfo(grades);
    }

    if (JSON_SAFE_CHECK(d, "level", Object)) {
        auto levels_obj = d["level"].GetObject();
        for (auto i = levels_obj.MemberBegin(); i != levels_obj.MemberEnd(); ++i) {
            if (!i->value.IsArray()) {
                continue;
            }

            auto lang_id = ATOI(i->name.GetString());
            auto level_arr = i->value.GetArray();
            for (auto c = 0; c < level_arr.Size(); ++c) {
                if (!level_arr[c].IsObject()) {
                    continue;
                }

                auto level_obj = level_arr[c].GetObject();
                mj::model::LevelInfo level;
                if (JSON_SAFE_CHECK(level_obj, "id", Int)) {
                    level.level_id = level_obj["id"].GetInt();
                }
                if (JSON_SAFE_CHECK(level_obj, "name", String)) {
                    level.name = level_obj["name"].GetString();
                }
                if (JSON_SAFE_CHECK(level_obj, "grade_id", Int)) {
                    auto temp_id = level_obj["grade_id"].GetInt();
                    if (grade_id_map.count(temp_id)) {
                        auto pair_id = grade_id_map[temp_id];
                        level.grade_id = pair_id.first;
                    } else {
                        level.grade_id = 0;
                    }
                }
                level.lang_id = lang_id;
                levels.push_back(level);
            }
        }
    }

    if (JSON_SAFE_CHECK(d, "description", Array)) {
        auto levels_description_arr = d["description"].GetArray();
        for (auto i = 0; i < levels_description_arr.Size(); ++i) {
            if (!levels_description_arr[i].IsObject()) {
                continue;
            }

            auto level_description = levels_description_arr[i].GetObject();
            auto level_id = 0;
            auto lang_id = 0;
            auto description = std::string("");
            if (JSON_SAFE_CHECK(level_description, "order", Int)) {
                level_id = level_description["order"].GetInt();
            }
            if (JSON_SAFE_CHECK(level_description, "lang_display_id", Int)) {
                lang_id = level_description["lang_display_id"].GetInt();
            };
            if (JSON_SAFE_CHECK(level_description, "description", String)) {
                description = level_description["description"].GetString();
            };

            for (auto &level : levels) {
                if (level.lang_id == lang_id && level.level_id == level_id) {
                    level.description = description;
                }
            }
        }
    }

    StoryDataModel::getInstance()->insertLevelInfo(levels);

    if (_callback) {
        _callback(true);
    }

    delete this;
}

void mj::LevelInfoAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
