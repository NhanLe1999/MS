//
//  MJStoryInfoModel.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/11/17.
//

#ifndef MJStoryInfoModel_hpp
#define MJStoryInfoModel_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "../external/json/document.h"
#include "cocos2d.h"
#include "MJHelper.h"
#include <iostream>
#include <sstream>
#include "F2PCommon.h"
static const std::unordered_map<int, cocos2d::Color3B> EXPLORE_GRADE_COLOR = {
    {-1, cocos2d::Color3B(0, 153, 254)},
    {0, cocos2d::Color3B::BLACK},
    {1, cocos2d::Color3B(86, 167, 4)},
    {2, cocos2d::Color3B(0, 153, 254)},
    {3, cocos2d::Color3B(255, 102, 1)},
    {4, cocos2d::Color3B(154, 2, 154)},
    {5, cocos2d::Color3B(255, 54, 103)},
    {6, cocos2d::Color3B(2, 1, 103)},
    {7, cocos2d::Color3B(165, 42, 42)},
};

namespace mj {
    namespace model {
        using namespace std;
        using namespace ms::MsTypes;
        using namespace ms::f2p;
        static std::string ConvertArrayIntergerToString(std::vector<int> intergerArray) {
            std::ostringstream itemsIdListStr;
            if (!intergerArray.empty())
            {
                // Convert all but the last element to avoid a trailing ","
                std::copy(intergerArray.begin(), intergerArray.end() - 1,
                    std::ostream_iterator<int>(itemsIdListStr, ","));

                // Now add the last element with no delimiter
                itemsIdListStr << intergerArray.back();
            }
            else
            {
                return "";
            }
            return itemsIdListStr.str();
        };
        static std::string ConvertArrayStringToString(std::vector<std::string> stringArray) {
            std::ostringstream itemsIdListStr;
            if (!stringArray.empty())
            {
                // Convert all but the last element to avoid a trailing ","
                std::copy(stringArray.begin(), stringArray.end() - 1,
                    std::ostream_iterator < std::string > (itemsIdListStr, ","));

                // Now add the last element with no delimiter
                itemsIdListStr << stringArray.back();
            }
            else
            {
                return "";
            }
            return itemsIdListStr.str();
        };
        static std::vector<int> ConvertStringToArrayInterger(std::string  intergerArrayString) {
            std::vector<int> vectorInterger = {};
            std::string item;
            if(intergerArrayString.empty())
                return vectorInterger;
            std::stringstream ss(intergerArrayString);
            int n;
            char ch =',';
            // use while loop to check the getline() function condition  
            while (std::getline(ss, item, ch)) {

                int itemOutPut = -1;
                try
                {
                    // string -> integer
                    itemOutPut = std::stoi(item);
                }
                catch (const std::exception&)
                {

                }
                vectorInterger.push_back(itemOutPut);
            }
            return vectorInterger;
        };

        static std::vector<std::string> ConvertStringToArrayString(std::string  string) {
            std::vector<std::string> vectorInterger = {};
            std::stringstream ss(string);
            std::string item;
            char ch = ',';
            // use while loop to check the getline() function condition  
            while (std::getline(ss, item, ch)) {
                vectorInterger.push_back(item);
            }
            return vectorInterger;
        };
        struct MaterialInfo {
            enum class Type {
                NONE        = 0,
                STORY       = 1, // giá trị bên dev-server quy định
                PHONICS     = 3, // giá trị bên dev-server quy định
            };
            
            int                     mat_id;         // id của material
            MaterialInfo::Type      type;           // kiểu của material: STORY(giá trị 1) hoặc PHONICS(giá trị 3)
            int                     lesson_id;      //
            int                     date_published; //
            std::string             name_worksheet; // tên của worksheet
            std::string             name_lesson;    // tên của story/lesson
            std::string             thumb;          // url của ảnh thumb
            std::vector<int>        levels;         //
            std::vector<int>        categories;     //
            bool                    is_readed;      //
            
            MaterialInfo();
            MaterialInfo(rapidjson::Value d);
            
            MaterialInfo::Type toMaterialType(int number);
        };
        
        struct MaterialPopularInfo {
            enum class Type {
                NONE        = 0,
                STORY       = 4, // giá trị bên dev-server quy định
                PHONICS     = 3, // giá trị bên dev-server quy định
            };
            
            int                         popular_id;     //
            int                         date_published; //
            MaterialPopularInfo::Type   type;           //
            int                         sort;           //
            std::string                 keyword;        //
            std::string                 thumb;          //
            
            MaterialPopularInfo();
            MaterialPopularInfo(rapidjson::Value d);
            
            MaterialPopularInfo::Type toMaterialPopularType(int number);
        };

        struct GradeInfo {
            int grade_id = 0;
            int lang_id = 0;
            int parent_id = 0;
            int order_by = 0;
            std::string name = "";

            std::string description = "";
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
            std::string getSQLValues();
        };
        struct ProductListInfo {
            int id = 0;
            std::string city  = "VN";
            int tien_ao = 0;
            int giam_40 = 0;
            int giam_30 = 0;
            int sale_off = 0;
            std::string product_id = "";
            std::string product_name = "";
            std::string product_description = "";
            std::string product_code = "";
            std::string current_unit = "VND";

            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
            std::string getSQLValues();
        };
        struct LevelInfo {
            int level_id = 0;
            int lang_id = 0;
            int grade_id = 0;
            std::string name = "";
            std::string description = "";

            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
            std::string getSQLValues();

        };

        struct CategoryInfo {
            int category_id = 0;
            std::string content = "";
            std::string image_url = "";
            int lang_id = 0;
            int order = 0;

            CategoryInfo() : category_id(0), content(""), image_url(""), lang_id(0), order(0) {}
            CategoryInfo(rapidjson::Value d);
            static std::string getSQLStructure();
            std::string getSQLValues();
        };

        struct CharacterInfo {
            int character_id = 0;
            std::string content = "";
            std::string image_url = "";
            int lang_id = 0;
            int order = 0;

            CharacterInfo(rapidjson::Value d);
            static std::string getSQLStructure();
            std::string getSQLValues();
        };

        struct StoryInfo {
            int unique_id = 0;
            int story_id = 0;
            int lang_id = 0;
            int app_id = 0;
            std::vector<int> levels;
            std::vector<int> categories;
            std::vector<int> characters;

            std::string designer = "";
            std::string author = "";
            int total_page = 0;
            int version = 0;
            std::string thumbnail_url = "";
            std::string name = "";
            int date_published = 0;
            int total_words = 0;
            std::string lexile = "";
            int quality = 0;

            bool is_read = false;

            StoryInfo() : is_read(false) {
                
            }

            StoryInfo(rapidjson::Value d);
        };

        struct StoryInfoTutoring
        {
            int storyId = -1;
            int appid = -1;
        };

        struct SyncBlock {
            int text_start, text_end;
            float time_start, time_end;
            std::string content;
        };
        
        struct AudiobookInfo {
            int book_id;
            int lang_id;
            int grade_id;
            std::string name;
            int duration;
            std::string thumbnail_url;
            std::string summary;
            std::string sync_path;
            std::string audio_url;
            int quality;
            int score;
            int date_publish;
            int read;
            int time_read;
            std::string extra;
            
            AudiobookInfo() : book_id(0), lang_id(0), grade_id(0), name(""), duration(0), thumbnail_url(""), summary(""), sync_path(""), audio_url(""), quality(0), score(0), date_publish(0), read(0), time_read(0), extra("") {}
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
        };

		struct AudiobookSeriesInfo {
			int id;
			std::string title;
			std::string thumbnail_url;
			std::vector<int> books;
			std::string jsonBooks;
			int hidden;

			void convertBookJsonToBookArray() {

				if (jsonBooks.empty()) {
					return;
				}

				rapidjson::Document doc;

				doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(jsonBooks.c_str());

				if (doc.HasParseError()|| !doc.IsArray()) {
					return;
				}

				books.clear();

				auto bookArr = doc.GetArray();

				for (int i = 0; i < bookArr.Size(); i++) {

					books.push_back(bookArr[i].GetInt());
				}
			}
		};

		struct PopularSearchInfo {
			int id = -1;
			std::string keyword = "";
			std::string thumbnailUrl = "";
			int sort = -1;
			int date_publish = -1;
           // int type = -1;
		};

		struct PopularSearchAudiobookInfo : public PopularSearchInfo {
			// add more private variables
		};

		struct PopularSearchStoryInfo : public PopularSearchInfo {
			// add more private variables
		};

        struct NotificationInfo
        {
            int notification_id = 0;
            //std::string message = "";
            std::string url = "";
            int time_created = 0;
            //std::string title = "";
            
            std::unordered_map<std::string,std::string> m_titles;//for multi-languages
            std::unordered_map<std::string,std::string> m_messages;//for multi-languages

            NotificationInfo() : notification_id(0), url(""), time_created(0)  {}
            NotificationInfo(rapidjson::Value d);
            static std::string getSQLStructure();
            std::string getSQLValues();
            static std::string getSQLPlaceholders();
        };
        struct AISpeakingCategoryIconInfo {
            std::string icon_thumb = "";
            std::string check_icon = "";
            std::string disable_icon = "";
            std::string progress_bg_icon = "";
            std::string selected_circle_icon = "";
            std::string design_icon = "";
            AISpeakingCategoryIconInfo() : icon_thumb(""), check_icon(""), disable_icon(""), progress_bg_icon(""), selected_circle_icon(""), design_icon("") {}
        };
        struct AISpeakingCategoryInfo {
            int topic_id = 0;
            std::string topic_name = "";
            std::string thumb_url = "";
            AISpeakingCategoryIconInfo iconInfo;// = new AISpeakingCategoryIconInfo();
            std::string icon_zip_url = "";
            int free = 0;
            int status = 0;
            int cat_version = 0;
            int order_by = 0;

            AISpeakingCategoryInfo() : topic_id(0), topic_name(""), thumb_url(""), free(0), status(0), cat_version(0), order_by(0), icon_zip_url("") , iconInfo(AISpeakingCategoryIconInfo()){}
            static std::string getSQLStructure();
            std::string getSQLValues();
            static std::string getSQLPlaceholders();
        };

        struct LessonInfo {
            enum State {
                LOCKED =  0,
                UNLOCKED = 2,
                PREPARING = 3,
                READY=4,
                LEARNED = 1
            };
            int lesson_id;
            int lesson_order;
            int lang_id;
            int category_id;
            std::string name;
            std::string thumbnail_url;
            State state;
            bool is_learned;
            int time_published;
            
            LessonInfo() : lesson_id(0), lesson_order(0), lang_id(0), category_id(0), name(""), thumbnail_url(""), state(LessonInfo::State::LOCKED), is_learned(false), time_published(0) {}
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
        };
        struct AISpeakingLessonInfo :LessonInfo {
            
            std::string _actorName = "";
            State _stateOfPreLesson = State::UNLOCKED;
            unsigned int _timeLearnedExpected = 0;
            AISpeakingLessonInfo():AISpeakingLessonInfo(LessonInfo()) {
            };
            
            AISpeakingLessonInfo(LessonInfo lessonInfo) {
                lesson_id = lessonInfo.lesson_id;
                lesson_order = lessonInfo.lesson_order;
                lang_id = lessonInfo.lang_id;
                category_id = lessonInfo.category_id;
                name = lessonInfo.name;
                thumbnail_url = lessonInfo.thumbnail_url;
                state = lessonInfo.state;
                is_learned = lessonInfo.is_learned;
                time_published = lessonInfo.time_published;
                _actorName = "";
                _stateOfPreLesson = State::UNLOCKED;
                _timeLearnedExpected = 0;
            };
            static std::string getLessonAISpeakingSQLStructure();
            static std::string getLessonAISpeakingSQLPlaceholders();
        };
        struct FreeItemInfo {
            int _idFree = 0;
            int _id = 0;
            int _lang_id = 1;
            bool _isLearn = false;
            bool _isLock = false;
            int _day = 0;
            FreeContentOption _option = FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY;
            DocumentsType _type = DocumentsType::STORY;
            std::string _url = "";

            FreeItemInfo() : _idFree(0), _id(0), _lang_id(1), _isLearn(false), _isLock(false), _day(0){}
            FreeItemInfo(int id , int lang_id, bool isLearn, bool isLock, DocumentsType type, int day) {
                _idFree = 0;
                _id = id;
                _lang_id = lang_id;
                _isLearn = isLearn;
                _isLock = isLock;
                _type = type;
                _day = day;

            };
            static std::string getFreeItemInfoSQLStructure();
            static std::string getFreeItemInfoSQLPlaceholders();
        };
        struct TalkingCommonInfo {
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
            bool isDownloaded = false;
            TalkingCommonInfo() : id(0), actor_name(""), version_name(""), zipPath(""),
                intro( ""),
                intro_mimic( ""),
                intro_talking( ""),
                your_turn( ""),
                cheer_up(""),
                listening(""),
                outro_mimic(""),
                outro("") {};
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
        };

        struct GameInfo {
            int game_id;
            std::string name;
            std::string resource_zip;
            std::string thumbnail_url;
            std::string zip_size;
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
        };

        

        struct ActInfo {
            enum State {
                LOCKED,
                LEARNED,
                READY
            };
            
            enum Type {
                QUIZZES,
                GAME,
                STORY
            };
            
            enum START_FROM {
                START_LESOON,
                START_STORY
            };

            int lesson_id;
            int activity_id;
            int quantity;
            int part;
            int order;

            std::string storyName;
            std::string name;
            std::string content;
            std::string resource_zip;
            std::string data_zip;
            State state;
            Type type;
            START_FROM startFrom;
            int sub_id;
            std::vector<int> quizz_id;
            std::vector<int> list_num_turnplay;
            
            std::vector<std::string> zip_urls;
            
            ActInfo() : activity_id(0), order(0), name(""), content(""), resource_zip(""), data_zip(""), state(LOCKED), type(GAME), sub_id(0), startFrom(START_LESOON){}
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
            bool isUnzipError = false;
        };

        struct ActAISpeakingInfo :ActInfo {
            int fileSize = 0;
            ActAISpeakingInfo() {
                ActInfo();
                //fileSize = 0;
            };
            ActAISpeakingInfo(ActInfo actInfo) {
                lesson_id = actInfo.lesson_id;
                activity_id = actInfo.activity_id;
                quantity = actInfo.quantity;
                part = actInfo.part;
                order = actInfo.order;
                name = actInfo.name;
                content = actInfo.content;
                resource_zip = actInfo.resource_zip;
                data_zip = actInfo.data_zip;
                state = actInfo.state;
                type = actInfo.type;
                sub_id = actInfo.sub_id;
                quizz_id = actInfo.quizz_id;
                list_num_turnplay = actInfo.list_num_turnplay;
                zip_urls = actInfo.zip_urls;
            };
            static std::string getSQLStructure();
            static std::string getSQLPlaceholders();
        };
    }
}

#endif /* MJStoryInfoModel_hpp */
