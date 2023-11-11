//
//  MJStoryInfoModel.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/11/17.
//

#include "MJStoryInfoModel.h"
#undef GetObject 


namespace mj {
    namespace model {
#pragma mark - Material
        MaterialInfo::Type MaterialInfo::toMaterialType(int number) {
            if (number == (int)MaterialInfo::Type::STORY) {
                return MaterialInfo::Type::STORY;
            } else if (number == (int)MaterialInfo::Type::PHONICS) {
                return MaterialInfo::Type::PHONICS;
            }
            
            return MaterialInfo::Type::NONE;
        }
        
        MaterialInfo::MaterialInfo() : mat_id(0), type(MaterialInfo::Type::NONE), lesson_id(0), date_published(0), name_worksheet(""), name_lesson(""), thumb(""), levels({}), categories({}), is_readed(false) {
            // nothing here
        }
        
        MaterialInfo::MaterialInfo(rapidjson::Value d)  {
            assert(d.IsObject());
            auto data = d.GetObject();
            
            // kiểu Number
            if (JSON_SAFE_CHECK(d, "id", Number)) {
                this->mat_id = d["id"].GetInt();
            }
            
            if (JSON_SAFE_CHECK(d, "type", Number)) {
                this->type = toMaterialType(d["type"].GetInt());
            }
            
            if (JSON_SAFE_CHECK(d, "lesson_id", Number)) {
                this->lesson_id = d["lesson_id"].GetInt();
            }
            
            if (JSON_SAFE_CHECK(d, "date_publish", Number)) {
                this->date_published = d["date_publish"].GetInt();
            }
            
            // kiểu String
            if (JSON_SAFE_CHECK(d, "name_worksheet", String)) {
                this->name_worksheet = d["name_worksheet"].GetString();
            }
            
            if (JSON_SAFE_CHECK(d, "name_lesson", String)) {
                this->name_lesson = d["name_lesson"].GetString();
            }
            
            if (JSON_SAFE_CHECK(d, "thumb", String)) {
                this->thumb = d["thumb"].GetString();
            }
            
            // kiểu Array
            if (JSON_SAFE_CHECK(d, "level", Array)) {
                auto levels = d["level"].GetArray();
                for (auto i = 0; i < levels.Size(); i++) {
                    auto level = &levels[i];
                    if (level->IsInt()) {
                        this->levels.push_back(level->GetInt());
                    }
                }
            }
            
            if (JSON_SAFE_CHECK(d, "category", Array)) {
                auto categories = d["category"].GetArray();
                for (auto i = 0; i < categories.Size(); i++) {
                    auto category = &categories[i];
                    if (category->IsInt()) {
                        this->categories.push_back(category->GetInt());
                    }
                }
            }
        }
        
        MaterialPopularInfo::MaterialPopularInfo() : popular_id(0), date_published(0), type(MaterialPopularInfo::Type::NONE), sort(0), keyword(""), thumb("") {
            // nothing here
        }
        
        MaterialPopularInfo::Type MaterialPopularInfo::toMaterialPopularType(int number) {
            if (number == (int)MaterialPopularInfo::Type::STORY) {
                return MaterialPopularInfo::Type::STORY;
            } else if (number == (int)MaterialPopularInfo::Type::PHONICS) {
                return MaterialPopularInfo::Type::PHONICS;
            }
            
            return MaterialPopularInfo::Type::NONE;
        }
        
        MaterialPopularInfo::MaterialPopularInfo(rapidjson::Value d) {
            assert(d.IsObject());
            auto data = d.GetObject();
            
            // kiểu Number
            if (JSON_SAFE_CHECK(d, "id", Number)) {
                this->popular_id = d["id"].GetInt();
            }
            
            if (JSON_SAFE_CHECK(d, "date_publish", Number)) {
                this->date_published = d["date_publish"].GetInt();
            }
            
            if (JSON_SAFE_CHECK(d, "type", Number)) {
                this->type = toMaterialPopularType(d["type"].GetInt());
            }
            
            if (JSON_SAFE_CHECK(d, "sort", Number)) {
                this->sort = d["sort"].GetInt();
            }
            
            // kiểu String
            if (JSON_SAFE_CHECK(d, "keyword", String)) {
                this->keyword = d["keyword"].GetString();
            }
            
            if (JSON_SAFE_CHECK(d, "thumb", String)) {
                this->thumb = d["thumb"].GetString();
            }
        }

#pragma mark - Category
        CategoryInfo::CategoryInfo(rapidjson::Value d)  {
            assert(d.IsObject());
            auto data = d.GetObject();

            if (JSON_SAFE_CHECK(data, "category_id", Int)) {
                this->category_id = data["category_id"].GetInt();
            }

            if (JSON_SAFE_CHECK(data, "content", String)) {
                this->content = data["content"].GetString();
            }

            if (JSON_SAFE_CHECK(data, "image", String)) {
                this->image_url = data["image"].GetString();
            }

            if (JSON_SAFE_CHECK(data, "_order", String)) {
                this->order = ATOI(data["_order"].GetString());
            }

            if (JSON_SAFE_CHECK(data, "lang_id", Int)) {
                this->lang_id = data["lang_id"].GetInt();
            }
        }
        
        std::string CategoryInfo::getSQLStructure() {
            return "(category_id, content, image, _order, lang_id)";
        }

        std::string CategoryInfo::getSQLValues() {
            return cocos2d::StringUtils::format("(%d,\"%s\",\"%s\",%d,%d)",
                                                this->category_id,
                                                this->content.c_str(),
                                                this->image_url.c_str(),
                                                this->order,
                                                this->lang_id);
        }

#pragma mark - Character
        CharacterInfo::CharacterInfo(rapidjson::Value d){
            assert(d.IsObject());
            auto data = d.GetObject();

            if (JSON_SAFE_CHECK(data, "character_id", String)) {
                this->character_id = ATOI(data["character_id"].GetString());
            }

            if (JSON_SAFE_CHECK(data, "content", String)) {
                this->content = data["content"].GetString();
            }

            if (JSON_SAFE_CHECK(data, "image", String)) {
                this->image_url = data["image"].GetString();
            }

            if (JSON_SAFE_CHECK(data, "_order", String)) {
                this->order = ATOI(data["_order"].GetString());
            }

            if (JSON_SAFE_CHECK(data, "lang_id", String)) {
                this->lang_id = ATOI(data["lang_id"].GetString());
            }
        }

        std::string CharacterInfo::getSQLStructure()  {
            return "(character_id, content, image, _order, lang_id)";
        }

        std::string CharacterInfo::getSQLValues()  {
            return cocos2d::StringUtils::format("(%d,\"%s\",\"%s\",%d,%d)",
                                                this->character_id,
                                                this->content.c_str(),
                                                this->image_url.c_str(),
                                                this->order,
                                                this->lang_id);
        }

#pragma mark - Story
        StoryInfo::StoryInfo(rapidjson::Value d)  {
            assert(d.IsObject());
            auto data = d.GetObject();

            if (JSON_SAFE_CHECK(d, "id", String)) {
                this->unique_id = ATOI(d["id"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "story_id", String)) {
                this->story_id = ATOI(d["story_id"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "lang_id", String)) {
                this->lang_id = ATOI(d["lang_id"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "level", Array)) {
                auto levels = d["level"].GetArray();
                for (auto i = 0; i < levels.Size(); i++) {
                    auto level = &levels[i];
                    if (level->IsInt()) {
                        this->levels.push_back(level->GetInt());
                    }
                }
            }

            if (JSON_SAFE_CHECK(d, "category", Array)) {
                auto categories = d["category"].GetArray();
                for (auto i = 0; i < categories.Size(); i++) {
                    auto category = &categories[i];
                    if (category->IsInt()) {
                        this->categories.push_back(category->GetInt());
                    }
                }
            }

            if (JSON_SAFE_CHECK(d, "character", Array)) {
                auto characters = d["character"].GetArray();
                for (auto i = 0; i < characters.Size(); i++) {
                    auto character = &characters[i];
                    if (character->IsInt()) {
                        this->characters.push_back(character->GetInt());
                    }
                }
            }

            if (JSON_SAFE_CHECK(d, "design", String)) {
                this->designer = d["design"].GetString();
            }

            if (JSON_SAFE_CHECK(d, "written", String)) {
                this->author = d["written"].GetString();
            }

            if (JSON_SAFE_CHECK(d, "total_page", String)) {
                this->total_page = ATOI(d["total_page"].GetString());
            }
            
            if (JSON_SAFE_CHECK(d, "app_id", String)) {
                this->app_id = ATOI(d["app_id"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "version_story", String)) {
                this->version = ATOI(d["version_story"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "image", String)) {
                this->thumbnail_url = d["image"].GetString();
            }

            if (JSON_SAFE_CHECK(d, "name", String)) {
                this->name = d["name"].GetString();
            }

            if (JSON_SAFE_CHECK(d, "date_publish", String)) {
                this->date_published = ATOI(d["date_publish"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "words", String)) {
                this->total_words = ATOI(d["words"].GetString());
            }

            if (JSON_SAFE_CHECK(d, "lexile", String)) {
                this->lexile = d["lexile"].GetString();
            }

            if (JSON_SAFE_CHECK(d, "quality", String)) {
                this->quality = ATOI(d["quality"].GetString());
            }
        }

#pragma mark - Notification Info
//        NotificationInfo::NotificationInfo(rapidjson::Value d)
//        {
//            assert(d.IsObject());
//            auto data = d.GetObject();
//
//            this->notification_id = this->time_created = 0;
//            this->message = this->url = this->title = "";
//
//            if (JSON_SAFE_CHECK(data, "id", String)) {
//                this->notification_id = ATOI(data["id"].GetString());
//            }
//
//            if (JSON_SAFE_CHECK(data, "title", String)) {
//                this->title = data["title"].GetString();
//            }
//
//            if (JSON_SAFE_CHECK(data, "message", String)) {
//                this->message = data["message"].GetString();
//            }
//
//            if (JSON_SAFE_CHECK(data, "url", String)) {
//                this->url = data["url"].GetString();
//            }
//
//            if (JSON_SAFE_CHECK(data, "created", String)) {
//                this->time_created = ATOI(data["created"].GetString());
//            }
//        }

        std::string NotificationInfo::getSQLStructure()
        {
            return "(id,title_en,title_vi,title_th,content_en,content_vi,content_th,url,dateinsert)";
        }

        std::string NotificationInfo::getSQLValues()
        {
            return cocos2d::StringUtils::format("(%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d)",
                                                notification_id,
                                                m_titles.count("en")?m_titles.at("en").c_str():"",
                                                m_titles.count("vi")?m_titles.at("vi").c_str():"",
                                                m_titles.count("th")?m_titles.at("th").c_str():"",
                                                m_messages.count("en")?m_messages.at("en").c_str():"",
                                                m_messages.count("vi")?m_messages.at("vi").c_str():"",
                                                m_messages.count("th")?m_messages.at("th").c_str():"",
                                                url.c_str(),
                                                time_created);

        }

        std::string NotificationInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?,?,?)";
        }

        std::string LevelInfo::getSQLStructure() {
            return "(level_id,lang_id,grade_id,name,description)";
        }

        std::string LevelInfo::getSQLValues() {
            return cocos2d::StringUtils::format("(%d, %d, %d, \"%s\", \"%s\")",
                    level_id,
                    lang_id,
                    grade_id,
                    name.c_str(),
                    description.c_str()
            );
        }

        std::string LevelInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?)";
        }

        std::string GradeInfo::getSQLStructure() {
            return "(grade_id, lang_id, name, description)";
        }

        std::string GradeInfo::getSQLValues() {
            return cocos2d::StringUtils::format("(%d, %d, \"%s\", \"%s\")",
                    grade_id,
                    lang_id,
                    name.c_str(),
                    description.c_str()
            );
        }

        std::string GradeInfo::getSQLPlaceholders() {
            return "(?,?,?,?)";
        }

        std::string ProductListInfo::getSQLStructure() {
            return "(id, city, tien_ao, giam_40, giam_30, sale_off, product_id, product_name, product_description, product_code, current_unit)";
        }

        std::string ProductListInfo::getSQLValues() {
            return cocos2d::StringUtils::format("( %d, \"%s\" , %d, %d, %d, %d,\"%s\", \"%s\" , \"%s\" , \"%s\" , \"%s\")",
                 id,
                 city.c_str(),
                 tien_ao,
                 giam_40,
                 giam_30,
                 sale_off,
                 product_id.c_str(),
                 product_name.c_str(),
                 product_description.c_str(),
                 product_code.c_str(),
                 current_unit.c_str()
            );
        }

        std::string ProductListInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?,?,?,?,?)";
        }
        std::string AISpeakingCategoryInfo::getSQLStructure() {
            return "(topic_id, topic_name, thumb_url, free, status, cat_version, order_by, icon_zip_url, icon_thumb, check_icon, disable_icon, progress_bg_icon, selected_circle_icon, design_icon )";
        }
        std::string AISpeakingCategoryInfo::getSQLValues() {

            return cocos2d::StringUtils::format("( %d, \"%s\", \"%s\" , %d, %d, %d, %d, \"%s\" , \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")",
                topic_id,
                topic_name.c_str(),
                thumb_url.c_str(),
                free,
                status, 
                cat_version,
                order_by,
                icon_zip_url.c_str(),
                iconInfo.icon_thumb.c_str(),
                iconInfo.check_icon.c_str(),
                iconInfo.disable_icon.c_str(),
                iconInfo.progress_bg_icon.c_str(),
                iconInfo.selected_circle_icon.c_str(),
                iconInfo.design_icon.c_str()
            );
        }
        std::string AISpeakingCategoryInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
        }

        //std::string AISpeakingLessonInfo::getSQLStructure() {
        //    return "(_id, _order, lang_id, category_id, name, thumb_url, time_published)";
        //}

        //std::string AISpeakingLessonInfo::getSQLValues() {
        //        return cocos2d::StringUtils::format("( %d,  %d, \"%s\" , %d, \"%s\", %d, %d, \"%s\")",
        //            unique_id,
        //            item_id,
        //            item_name.c_str(),
        //            topic_id,
        //            thumb_url.c_str(),
        //            order_by,
        //            file_size,
        //            url_download.c_str()
        //        );
        //}

        //std::string AISpeakingLessonInfo::getSQLPlaceholders() {
        //    return "(?,?,?,?,?,?,?)";
        //}
        std::string AudiobookInfo::getSQLStructure() {
            return "(_id, lang_id, grade_id, name, thumb_url, content_path, audio_url, summary, duration, quality, score, date_publish, extra)";
        }
        
        std::string AudiobookInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?,?,?,?,?,?, ?)";
        }
        
        std::string LessonInfo::getSQLStructure() {
            return "(_id, _order, lang_id, category_id, name, thumb_url, time_published)";
        }

        std::string LessonInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?)";
        }
        
        std::string AISpeakingLessonInfo::getLessonAISpeakingSQLStructure(){
            return "(_id, _order, lang_id, category_id, name, thumb_url, time_published, timeLearnedExpected)";
        }
        std::string AISpeakingLessonInfo::getLessonAISpeakingSQLPlaceholders(){
            return "(?,?,?,?,?,?,?,?)";
        }

        std::string ActInfo::getSQLStructure() {
            return "(_id, _order, lesson_id, sub_id, type, name, content, resource_zip, data_zip)";
        }
        
        std::string ActInfo::getSQLPlaceholders() {
            return "(?,?,?,?,?,?,?,?,?)";
        }
        
        std::string GameInfo::getSQLStructure() {
            return "(_id, name, resource_zip, thumb_url)";
        }
        
        std::string GameInfo::getSQLPlaceholders() {
            return "(?,?,?,?)";
        }
        std::string ActAISpeakingInfo::getSQLStructure()
        {
            return ActInfo::getSQLStructure();

        }
        std::string ActAISpeakingInfo::getSQLPlaceholders()
        {
            return ActInfo::getSQLPlaceholders();
        }
        std::string TalkingCommonInfo::getSQLStructure()
        {
            /*
            int id = 0;
            std::string actor_name = "";
            std::string version_name = "";
            std::string zipPath = "";
            std::string intro = "";
            std::string intro_mimic = "";
            std::string intro_talking = "";
            std::string your_turn = "";
            std::string cheer_up = "";
            std::string listening = "";
            std::string outro_mimic = "";
            std::string outro = "";
            */
            return "(actor_name, version_name, zipPath, intro, intro_mimic, intro_talking, your_turn, cheer_up, listening, outro_mimic, outro)";
        }
        std::string TalkingCommonInfo::getSQLPlaceholders() {
            return "(?,?,?, ?,?,?,?, ?,?,?,?)";
        }
        /*
            int _id = 0;
            int _lang_id = 1;
            bool _isLearn = false;
            bool _isLock = false;
            FreeContentOption _option = FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY;
            DocumentsType _type = DocumentsType::STORY
            */
        std::string FreeItemInfo::getFreeItemInfoSQLStructure()
        {
             return "(_idFree, _id, _lang_id, _isLearn, _isLock, _option, _type, _day)";
        }
        std::string FreeItemInfo::getFreeItemInfoSQLPlaceholders()
        {
            return "(?,?,?,?,?,?,?,?)";
        }
}
}
