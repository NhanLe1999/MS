//
//  Math_Libs.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#ifndef Math_Libs_h
#define Math_Libs_h

#include <stdio.h>
#include "cocos2d.h"

#include "MJLessonsManager.h"

#include "json/document.h"
#include "json/rapidjson.h"
#include "json/prettywriter.h"
#include "json/filewritestream.h"
#include <cstdio>

#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>

#include <iostream>
#include <fstream>
#include <ctime>

#include "Shape2DHitUtility.h"

#pragma mark - Const
#define m_theme         math::theme::Theme::getInstance()
#define math_statistic  math::statistic::Manager::getInstance()
#define m_lang          math::lang::Language::getInstance()
#define current_lang    math::lang::Language::getInstance()->getLanguageName()

#define quiz_size       cocos2d::Director::getInstance()->getVisibleSize()
#define m_text_color    cocos2d::Color4B(99, 51, 15, 255)
#define m_text_color_3b cocos2d::Color3B(99, 51, 15)
#define m_text_font     "Maths-Bold.ttf"

#define verson_1 false
#define debugging_mode true

namespace math {

    
#pragma mark - Theme
    namespace theme {
        enum MathTheme
        {
            default_theme,
            story_theme
        };

        class Theme
        {
        public:
            Theme(){};
            ~Theme(){};
            
            static Theme* getInstance();
            
            void setTheme(MathTheme theme);
            MathTheme getTheme();
            std::string getThemePath();
            std::string getThemeSuffix();
            
        private:
            MathTheme theme = math::theme::default_theme;
        };
    }
    
#pragma mark - Statistic
    namespace statistic {
        class Manager
        {
        public:
            Manager(){};
            ~Manager(){};
            static Manager* getInstance();
            void setCurrentGame(mj::model::Game game);
            
            std::string getGamePath();
            std::string getGamePath(mj::model::Game game);
            
            void addUsedIcon(std::string set, int _id);
            void addPlayedInfo(int time, int submit);
            
            void copyFile();
            void copyFile(mj::model::Game game);
            
        private:
            mj::model::Game _game;
                        
            std::string _path="";
            std::string _type="";
            
            int _stars = 2;
            int _time_limit = 1;
            int _submit_limit = 1;
            int _time_exellent = 1;
            int _submit_exellent = 1;

        };
    }
    
#pragma mark - Common
    namespace common {
        enum QuizType {
            normal = 0,
            full_scr,
            has_right_bar,
            has_bottom_bar,
            has_color_pad_right,
            has_color_pad_bottom
        };
        
        enum TouchPermission {
            deny,
            touch,
            move
        };
        
        enum DetectedType {
            box,
            circle,
            nontrans,
            custom
        };
        
        enum SelectedAnimation {
            fade,
            dance,
            blink,
            scale,
            background_color
        };
        
        enum AnswerType {
            answer = 0,
            icon_name,
            done,
            custom_bg,
            yes_no
        };
    }
    
    namespace group {
        enum GroupType {
            frame, //doi tuong sap xep mot cach tuan tu va deu dan.
            queue,
            custom
        };
        
        enum GroupAlign {
            left,
            right,
            random
        };

        enum GroupArange {
            kind,
            mix
        };

        enum TouchDetect {
            box,
            non_trans
        };
    }
    
    
#pragma mark - Comparision
    namespace comparision {
        enum Key {
            exactly,
            name,
            kind,
            family,
            color,
            like,
            
            size,
            height,
            width,
            length,
            weight,
            hold,
            number,
            character,
            position,
            speed,
            time,
            distance
        };
        
        struct Value {
            std::string id;
            std::string set;
            std::string name;
            std::string family;
            std::string kind;
            std::string color;
            std::string position;
            std::string like;

            int size;
            int height;
            int width;
            int length;
            int weight;
            int hold;
            int number;
            int character;
            int speed;
            int time;
            int distance;
        };
    }
    
#pragma mark - Language
    namespace lang {
        enum MathLanguage
        {
            english_us,
            vietnamese
        };
        
        class Language
        {
        public:
            Language(){};
            ~Language(){};
            
            static Language* getInstance();
            
            void setLanguage(MathLanguage lang);
            MathLanguage getLanguage(std::string name = "");
            std::string getLanguageName();
            
        private:
            MathLanguage lang = MathLanguage::english_us;
            std::string language_name="english_us";
        };
        
    }
    
#pragma mark - Resource
    namespace resource {
		struct Sync_Text
		{
			std::string w;
			float e;
			int ts;
			float s;
			int te;
		};
        
        struct Voice {
        public:
            int voice_id;
            //std::string path;
            std::string phonic_text = "";
			std::vector<Sync_Text> sync_texts;
            std::vector<std::string> paths;
        };
        
        struct Video {
        public:
            std::string path;
            float duration;
        };
        
        struct Text {
        public:
            std::string text;
            std::vector<Voice> voices;
            Video video;
            
            bool highlight = false;
            bool showicon = false;
            std::string icon = "";
            
			//std::string getVoice(int voice_id);
            std::vector<std::string> getVoices(int voice_id);
			std::vector<Sync_Text> getSyncText(int voice_id);
        };
        
        struct Property {
        public:
            enum Type {
                PROP,
                COMP,
                PHONIC
            };
            Type type;
            std::string key;
            Text value;
            int compare_value;
        };
        
        struct PropertiesSet {
            lang::MathLanguage lang;
            std::vector<Property> properties;
        };
        
        class Image {
        public:
            Image(){};
            ~Image(){
                //CCLOG ("Delete image");
            }
            
            std::string code;
            std::string path;
            std::vector<PropertiesSet> properties;
            std::vector<Property> comparisions;
            std::string tracing;
            
            bool enable;
            bool used;
            int media_type;
            void clear();
            
            void addProperty(std::string key, std::string value, std::vector<std::string> audios);
            void addComparision(std::string key, int value);
            
            Text getTextValue(std::string key, lang::MathLanguage lang=lang::english_us);
			std::string getQuestionVMonkey(std::string key, lang::MathLanguage lang = lang::english_us);
			std::string getText(std::string key = "name_1", lang::MathLanguage lang = lang::english_us);
            CC_DEPRECATED_ATTRIBUTE std::string getAudio(std::string key = "name_1", lang::MathLanguage lang = lang::english_us, int voice_id = 0);
            std::vector<std::string> getAudios(std::string key = "name_1", lang::MathLanguage lang = lang::english_us, int voice_id = 0);
            std::vector<std::string> getTextPhonic(std::string key, lang::MathLanguage lang = lang::english_us);
            std::vector<std::string> getAudioPhonic(std::string key, lang::MathLanguage lang = lang::english_us);
            Video getVideo(std::string key = "name_1", lang::MathLanguage lang = lang::english_us);
			std::vector<Sync_Text> getSyncText(std::string key = "name_1", lang::MathLanguage lang = lang::english_us, int voice_id = 0);
            int getComparisionValue(std::string key);
            bool isExist(std::string key, lang::MathLanguage lang = lang::english_us);
        };
                
        class ResHelper
        {
        public:
            
            static Image* createImage(std::string path);
            static Image* getImage(std::string set, std::string icon_id = "");
			static Image* getImage(std::vector<Image*> imgs, std::string icon_id = "");
            
            static std::vector<Image*> getImages(std::string set, std::string name="", std::string prefix = "");
            static std::vector<Image*> filterImages(std::vector<Image*> images, std::string key);
            static std::vector<Image*> filterImages(std::vector<Image*> images, std::string key, cocos2d::Value value);
            
            static std::vector<Image*> filterImages(std::vector<Image*> images, std::vector<std::string> keys);
            
            static Image* mergeImages(std::vector<Image*> images);
            
        };
    }

#undef OPTIONAL
#pragma mark - Question
    namespace question {
        enum StructureType {
            TEMPLATE = 0,
            OPTIONAL,
            KEYWORD
        };
        
        struct Keyword {
            int value;
            math::resource::Text text;
            
        };
        
        struct OptionalWord {
            int index = -1;
            std::string key;
            std::vector<math::resource::Text> texts;
            
            math::resource::Text getText() {
                return texts[index];
            }
        };
        
        struct Structure {
            int index;
            StructureType type;
            math::resource::Text template_text;
            std::vector<Keyword> keywords;
            int keyword_index = -1;
            OptionalWord optional;
            
            bool highlight = false;
            bool showicon = false;
            math::resource::Image* icon = nullptr;
        };
        
        struct Question {
            int index;
            std::vector<Structure> structures;
            std::string ending;
        };
        
        struct Highlight {
            int start = 0;
            int number = 0;
            bool show_icon = false;
            std::string icon = "";
        };
        
        class Parse {
        public:
            static std::vector<Question> parseQuestion(std::string game_path, std::string prefix = "", bool loop = false, int loop_number = 1);
            static std::vector<Question> parseQuestionWithLoops(std::string game_path, std::string prefix, std::vector<int> loops);
        };
        
    }

#pragma mark - ui helper
    namespace uihelper {
        
        inline cocos2d::Node * getChild(cocos2d::Node * root, std::string value) {
            return root->getChildByName(value);
        }
        
        template<typename ...T>
        cocos2d::Node * getChild(cocos2d::Node * root, std::string value, T... args) {
            auto r = root;
            
            r = root->getChildByName(value);
            r = getChild(r, args...);
            
            return r;
        }
        
        template<typename R, typename ...T>
        R getChild(cocos2d::Node * root, std::string value, T... args) {
            auto r = getChild(root, value, args...);
            return static_cast<R>(r);
        }
    };
    
#pragma mark - callback
    namespace callback { 
        //quiz callback:
        typedef std::function<void(bool)> onCompleteQuiz;
        
        //callback khi bat dau cham vao obj
        typedef std::function<void(cocos2d::Node *)> onTouchObjectBegan;
        typedef std::function<void(cocos2d::Node *, cocos2d::Vec2)> onTouchObjectEnded;
        
        //callback khi click answer
        typedef std::function<void(cocos2d::Node *)> onAnswerClicked;
    };
    
#pragma mark - animation
    namespace animation {
        
        enum ObjectShowType {
            ScaleUp = 0,
            SlideUp,
            SlideDown,
            SlideLeft,
            SlideRight,
            ScaleUpAndRotation,
            No_Effect
        };
        
        //Animation
        class Helper {
        public:
            static cocos2d::Action* shake(float speed = 0.3);
            static cocos2d::Action* fade();
            static cocos2d::Action* scale();
            static cocos2d::Action* blink();
        };
        
        struct CapturedNode {
            cocos2d::Vec2 position;
            float scale;
            float rotation;
        };
    }    
}

#endif /* Math_Libs_h */
