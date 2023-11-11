//
//  MSGradeManager.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/28/17.
//

#include "MSGradeManager.h"
#include "MJDefault.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
#include "APProfileManager.h"

namespace ms {
    const std::unordered_map<std::string, int> OLD_GRADE_MAP = {
        {"Pre-Kindergarten" , 1},
        {"Kindergarten", 2},
        {"Grade 1", 3},
        {"Grade 2", 4},
        {"Grade 3", 5},
        {"Grade 4", 6},
        {"Grade 5", 7}
    };

    const std::unordered_map<std::string, int> NEW_GRADE_MAP = {
        {"Pre-Kindergarten" , 84},
        {"Kindergarten", 216},
        {"Grade 1", 217},
        {"Grade 2", 375},
//        {"Grade 3", 5},
//        {"Grade 4", 6},
//        {"Grade 5", 7}
        {"0-2 tuổi", 473},
        {"2-4 tuổi", 474},
        {"4-6 tuổi", 475},
        {"6-8 tuổi", 476},
        {"8-10 tuổi", 477},
        {"Test", 469},
        {"Grade K", 479},
        {"Grade 1", 480},
        {"Grade 2", 481}
    };

    GradeManager::GradeManager() {
        updateGrades();
    }

    void GradeManager::setInitialGrade(std::string grade_name) {
        if (grade_name == "All Grades") {
            PROFILE_MANAGER.setIntegerForKey(key_grade_stories, -1);
            PROFILE_MANAGER.setIntegerForKey(key_grade_lessons, 84);
            PROFILE_MANAGER.setIntegerForKey(key_grade_audiobooks, -1);
            PROFILE_MANAGER.setIntegerForKey(key_grade_lessons_phonic, 479);
            return;
        }
        if (OLD_GRADE_MAP.find(grade_name) != OLD_GRADE_MAP.end()) {
            auto idx = OLD_GRADE_MAP.at(grade_name);
            PROFILE_MANAGER.setIntegerForKey(key_grade_stories, idx);
        }

        if (NEW_GRADE_MAP.find(grade_name) != NEW_GRADE_MAP.end()) {
            auto idx = NEW_GRADE_MAP.at(grade_name);
            PROFILE_MANAGER.setIntegerForKey(key_grade_lessons, idx);
            if (grade_name == "Pre-Kindergarten") {
                idx = NEW_GRADE_MAP.at("Kindergarten");
            }
            PROFILE_MANAGER.setIntegerForKey(key_grade_audiobooks, idx);
            PROFILE_MANAGER.setIntegerForKey(key_grade_lessons_phonic, idx);
        }
    }

    void GradeManager::setCurrentGradeId(int idx) {
        switch (_scope) {
            case Scope::AUDIOBOOKS:
                PROFILE_MANAGER.setIntegerForKey(key_grade_audiobooks, idx);
                break;

            case Scope::LESSONS:
                PROFILE_MANAGER.setIntegerForKey(key_grade_lessons, idx);
                break;

            case Scope::LESSONS_PHONIC:
                PROFILE_MANAGER.setIntegerForKey(key_grade_lessons_phonic, idx);
                break;

            case Scope::STORIES:
                PROFILE_MANAGER.setIntegerForKey(key_grade_stories, idx);
                break;
        }
    }

    int GradeManager::getCurrentGradeId() {
        switch (_scope) {
            case Scope::AUDIOBOOKS:
                return PROFILE_MANAGER.getIntegerForKey(key_grade_audiobooks, -1);

            case Scope::LESSONS:
                return PROFILE_MANAGER.getIntegerForKey(key_grade_lessons, 84);

            case Scope::LESSONS_PHONIC:
                 return PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_phonic, 479);

            case Scope::STORIES:
                return PROFILE_MANAGER.getIntegerForKey(key_grade_stories, -1);
        }
    }

    std::vector<int> const vmonkey_grade_id =
    {
        498, 499, 502, 507
    };
    std::vector<int> const vmonkey_lesson_grade_id =
    {
        764
    };

    bool GradeManager::isGrade(int grade_id, std::vector<int> grade_list){
        for (auto id : grade_list) {
            if (grade_id == id) {
                return true;
            }
        }
        return false;
    };

    std::vector<mj::model::GradeInfo> GradeManager::getGradesInfo()
    {
        std::vector<mj::model::GradeInfo> supported = {};
        
        if (_scope == Scope::STORIES)
        {
            supported = _old_grades;
        }
        else if (_scope == Scope::LESSONS)
        {
            if (CONFIG_MANAGER->isVietnameseStories()){
                for (int i = 0; i < _new_grades.size(); i++)
                   {
                       auto t = _new_grades.at(i);
                       if (isGrade(t.grade_id, vmonkey_lesson_grade_id))
                       {
                           supported.push_back(_new_grades.at(i));
                       }
                   }
            }else{
                for (int i = 0; i < _new_grades.size(); i++)
                {
                    auto t = _new_grades.at(i);
                    if (!isGrade(t.grade_id, phonic_grade_id))
                    {
                        supported.push_back(_new_grades.at(i));
                    }
                }
                
                supported.erase(supported.begin());
            }
            	
        }
        else if (_scope == Scope::LESSONS_PHONIC)
        {
            for (int i = 0; i < _new_grades.size(); i++)
            {
                auto t = _new_grades.at(i);
                if (isGrade(t.grade_id, phonic_grade_id)) {
                    supported.push_back(t);
                }
            }
        }
        else
        {
            if (CONFIG_MANAGER->isVietnameseStories())
            {
                for (auto t : _new_grades)
                {
                    if (!isGrade(t.grade_id, vmonkey_grade_id) && !isGrade(t.grade_id, vmonkey_lesson_grade_id))
                    {
                        supported.push_back(t);
                    }
                }
            }
            else
            {
                for (int i = 0; i < _new_grades.size(); i++)
                {
                    auto t = _new_grades.at(i);
                    
                    if (!isGrade(t.grade_id, phonic_grade_id))
                    {
                        supported.push_back(_new_grades.at(i));
                    }
                }
                if(supported.size()>1)
                    supported.erase(supported.begin() + 1);

            }
            
        }
        
        return supported;
    }

    mj::model::GradeInfo GradeManager::getGradeInfoById(int gid) {
        auto it = std::find_if(std::begin(_old_grades), std::end(_old_grades), [gid](mj::model::GradeInfo g) {
            return g.grade_id == gid;
        });

        if (it != std::end(_old_grades)) {
            return * it;
        }

        it = std::find_if(std::begin(_new_grades), std::end(_new_grades), [gid](mj::model::GradeInfo g) {
            return g.grade_id == gid;
        });

        if (it != std::end(_new_grades)) {
            return * it;
        }

        mj::model::GradeInfo r;
        r.grade_id = -1;
        return r;
    }

    void GradeManager::updateGrades() {
        auto levels = StoryDataModel::getInstance()->getAllLevelsInfo(LANGUAGE_MANAGER->getCurrentLangId());
        for (auto l : levels) {
            auto grade = StoryDataModel::getInstance()->getGradeInfoById(l.grade_id, LANGUAGE_MANAGER->getCurrentLangId());
            _map_level_to_grade.emplace(l.level_id, grade.grade_id);
        }

        mj::model::GradeInfo all_grades;
        all_grades.grade_id = -1;
        all_grades.lang_id = 1;

        all_grades.name = LANGUAGE_MANAGER->getDisplayTextByKey("key.all.grades"); //getCurrentLangId() ==1)? "All Grades" : "Tất cả cấp độ";
        all_grades.description = "";
        _old_grades = StoryDataModel::getInstance()->getAllGradesInfo(StoryLanguageManager::getCurrentLangId());
        _old_grades.clear();
        auto all_levels = StoryDataModel::getInstance()->getAllLevelsInfo(StoryLanguageManager::getCurrentLangId());
        for (auto i = 0; i < all_levels.size(); ++i) {
            mj::model::GradeInfo fake;
            fake.grade_id = all_levels[i].level_id;
            fake.name = all_levels[i].name;
            fake.description = all_levels[i].description;
            fake.grade_id = all_levels[i].level_id;

            _old_grades.push_back(fake);
        }

        _old_grades.insert(_old_grades.begin(), all_grades);
        _new_grades = StoryDataModel::getInstance()->getAllNewGrades(StoryLanguageManager::getCurrentLangId());
        _new_grades.insert(_new_grades.begin(), all_grades);
    }

    bool GradeManager::isGradeLocked(int idx) {
        switch (_scope) {
            case Scope::STORIES:
                return false;

            case Scope::AUDIOBOOKS:
                return false;

            case Scope::LESSONS:
                return idx == 375;

            case Scope::LESSONS_PHONIC:
                return false;
        }
    }

    std::string GradeManager::getNameByLanguage(std::string name, int lang_id) {

        if(CONFIG_MANAGER->isVietnameseStories()) //VM
        {
            if(lang_id == 4)
            {
                return name;
            }

            if(_scope == Scope::AUDIOBOOKS){
                static const std::unordered_map<std::string, std::string> name_map = {
                    {"Tất cả cấp độ", "All Grades" },
                    {"0-2 tuổi" , "Aged 0-2"},
                    {"2-4 tuổi" , "Aged 2-4"},
                    {"4-6 tuổi", "Aged 4-6"},
                    {"6-8 tuổi", "Aged 6-8"},
                    {"8-10 tuổi", "Aged 8-10"},
                    {"10-12 tuổi", "Aged 10-12"},
                    {"Cấp độ 6", "Grade 4"},
                    {"Cấp độ 7", "Grade 5"}
                };
//                static const std::unordered_map<std::string, std::string> name_map = {
//                    {"All Grades", "Tất cả cấp độ" },
//                    {"0-2" , "0-2 tuổi"},
//                    {"2-4 tuổi" , "Aged 2-4 tuổi"},
//                    {"4-6 tuổi", "Aged 4-6 tuổi"},
//                    {"6-8 tuổi", "Aged 6-8 tuổi"},
//                    {"8-10 tuổi", "Aged 8-10 tuổi"},
//                    {"10-12 tuổi", "Aged 10-12 tuổi"},
//                    {"Cấp độ 6", "Grade 4"},
//                    {"Cấp độ 7", "Grade 5"}
//                };
                if (name_map.find(name) != name_map.end()) {
                    return name_map.at(name);
                }
            }

            static const std::unordered_map<std::string, std::string> name_map = {
                {"Tất cả cấp độ", "All Grades"},
                {"Cấp độ Nhà trẻ", "Pre-K" },
                {"Cấp độ Mẫu giáo", "Pre-Kindergarten"},
                {"Cấp độ Tiểu học", "Kindergarten"},
                {"Cấp độ 3", "Grade 1"},
                {"Cấp độ 4", "Grade 2"},
                {"Cấp độ 5", "Grade 3"},
                {"Cấp độ 6", "Grade 4"},
                {"Cấp độ 7", "Grade 5"}
            };

            if (name_map.find(name) != name_map.end()) {
                return name_map.at(name);
            }

        }
        else  //MS
        {

            if (name == "Kindergarten" && _scope == Scope::AUDIOBOOKS)
            {
                return LANGUAGE_MANAGER->getDisplayTextByKey("key.grade.kindergarten");
            }

            if(_scope == Scope::LESSONS_PHONIC)
            {
                if(name == "Test") return name;

                if(lang_id == LANGUAGE_EN_CODE)
                {
                    std::string str = "MP - ";
                    return name.replace(name.find(str), str.length(), "");
                }
                
                static const std::unordered_map<std::string, std::string> thai_lan =
                {
                    {"MP - Grade K" , "ระดับ 1"},
                    {"MP - Grade 1" , "ระดับ 2"},
                    {"MP - Grade 2" , "ระดับ 3"}
                };
                
                if (lang_id == LANGUAGE_THAI_CODE)
                {
                    if (thai_lan.find(name) != thai_lan.end())
                    {
                        return thai_lan.at(name);
                    }
                }
                static const std::unordered_map<std::string, std::string> indo_lang =
                {
                    {"MP - Grade K" , "Level 1"},
                    {"MP - Grade 1" , "Level 2"},
                    {"MP - Grade 2" , "Level 3"}
                };
                
                if (lang_id == LANGUAGE_ID_CODE)
                {
                    if (indo_lang.find(name) != indo_lang.end())
                    {
                        return indo_lang.at(name);
                    }
                }
                static const std::unordered_map<std::string, std::string> name_map =
                {
                    {"MP - Grade K" , "Cấp độ 1"},
                    {"MP - Grade 1" , "Cấp độ 2"},
                    {"MP - Grade 2" , "Cấp độ 3"}
                };

                if (name_map.find(name) != name_map.end())
                {
                    return name_map.at(name);
                }
                
                //return getPhonicGradeName(name, lang_id);
            }
            
            if (lang_id == LANGUAGE_EN_CODE)
            {
                return name;
            }
            
            static const std::unordered_map<std::string, std::string> thai_lan =
            {
                {"All Grades" , "ทุกระดับ"},
                {"Pre-K" , "ระดับ 1"},
                {"Pre K" , "ระดับ 1"},
                {"Pre-Kindergarten" , "ระดับ 1"},
                {"Pre Kindergarten" , "ระดับ 1"},
                {"Kindergarten", "ระดับ 2"},
                {"Grade 1", "ระดับ 3"},
                {"Grade 2", "ระดับ 4"},
                {"Grade 3", "ระดับ 5"},
                {"Grade 4", "ระดับ 6"},
                {"Grade 5", "ระดับ 7"}
            };

            if (lang_id == LANGUAGE_THAI_CODE)
            {
                if (thai_lan.find(name) != thai_lan.end())
                {
                    return thai_lan.at(name);
                }
            }

			static const std::unordered_map<std::string, std::string> indo_lan =
			{
				{"All Grades" , "Semua tingkat"},
				{"Pre-K" , "Level 1"},
				{"Pre K" , "Level 1"},
				{"Pre-Kindergarten" , "Level 1"},
				{"Pre Kindergarten" , "Level 1"},
				{"Kindergarten", "Level 2"},
				{"Grade 1", "Level 3"},
				{"Grade 2", "Level 4"},
				{"Grade 3", "Level 5"},
				{"Grade 4", "Level 6"},
				{"Grade 5", "Level 7"}
			};

			if (lang_id == LANGUAGE_ID_CODE)
			{
				if (indo_lan.find(name) != indo_lan.end())
				{
					return indo_lan.at(name);
				}
			}
            
            static const std::unordered_map<std::string, std::string> name_map =
            {
                  {"All Grades" , "Tất cả cấp độ"},
                  {"Pre-K" , "Cấp độ 1"},
                  {"Pre K" , "Cấp độ 1"},
                  {"Pre-Kindergarten" , "Cấp độ 1"},
                  {"Pre Kindergarten" , "Cấp độ 1"},
                  {"Kindergarten", "Cấp độ 2"},
                  {"Grade 1", "Cấp độ 3"},
                  {"Grade 2", "Cấp độ 4"},
                  {"Grade 3", "Cấp độ 5"},
                  {"Grade 4", "Cấp độ 6"},
                  {"Grade 5", "Cấp độ 7"}
            };
            
            if (name_map.find(name) != name_map.end())
            {
                return name_map.at(name);
            }
        }


        return name;
    }

    int GradeManager::getGradeIdByLevelId(int level_id) {
        if (_map_level_to_grade.count(level_id) > 0) {
            return _map_level_to_grade.at(level_id);
        }

        return -1;
    }

    std::string GradeManager::getPhonicGradeName(std::string name, int lang_id)
    {
        if(name == "Test") return name;

        if(lang_id == 1)
        {
            std::string str = "MP - ";
            return name.replace(name.find(str), str.length(), "");
        }
        static const std::unordered_map<std::string, std::string> name_map =
        {
            {"MP - Grade K" , "Cấp độ 1"},
            {"MP - Grade 1" , "Cấp độ 2"},
            {"MP - Grade 2" , "Cấp độ 3"}
        };

        if (name_map.find(name) != name_map.end())
        {
            return name_map.at(name);
        }

        return name;
    }

    std::vector<std::string> GradeManager::getSubStringsInSeparatedVerticalBarString(std::string i_input)
    {
        if (i_input.empty())
        {
            return std::vector<std::string>();
        }

        std::vector<std::string> result;
        std::stringstream		strStream;
        strStream << i_input;
        std::string				valueStr;

        while (std::getline(strStream, valueStr, '|'))
        {
            std::string value;

            if (std::stringstream(valueStr) >> value)
            {
                result.push_back(value);
            }
        }

        return result;
    }
}
