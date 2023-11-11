//
//  ParseJSONHelper.hpp
//  MonkeyMath
//
//  Created by HuyPhuong on 6/28/18.
//

#ifndef ParseJSONHelper_hpp
#define ParseJSONHelper_hpp

#include <stdio.h>
#include "Math_Libs.h"

#define JSON_PARSE math::JSON::Parse::getInstance()

namespace math {namespace JSON {
    class Parse {
    public:
        static Parse& getInstance();
        
        //Simple
        cocos2d::Size getSize(rapidjson::Value& parent, std::string member);
        int randomRange(rapidjson::Value& parent, std::string member);
        
        rapidjson::Value& getChild(rapidjson::Value& parent, std::string child_name);
        bool getBool(bool value, rapidjson::Value& parent, std::string child_name);
        int getInt(int value, rapidjson::Value& parent, std::string child_name);
        float getDouble(float value, rapidjson::Value& parent, std::string child_name);
        std::string getString(std::string value, rapidjson::Value& parent, std::string child_name);
        
        
        //Expand
        template<typename ...T>
        cocos2d::Size getSize(rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getSize(child, args...);
        }
        
        template<typename ...T>
        int randomRange(rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return randomRange(child, args...);
        }
        
        template<typename ...T>
        rapidjson::Value& getChild(rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getChild(child, args...);
        }

        template<typename ...T>
        bool getBool(bool value, rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getBool(value, child, args...);
        }

        template<typename ...T>
        int getInt(int value, rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getInt(value, child, args...);
        }
        
        template<typename ...T>
        float getDouble(float value, rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getDouble(value, child, args...);
        }
        
        template<typename ...T>
        std::string getString(std::string value, rapidjson::Value& parent, std::string child_name, T... args) {
            rapidjson::Value& child = getChild(parent, child_name);
            return getString(value, child, args...);
        }
        
    protected:
        explicit Parse();
    private:
        bool init();
    };
    
    
    //TRACING POINT
    struct MMPoint {
        int index = 0;
        int x = 0;
        int y = 0;
    };
    
    struct MMPart {
        int index = 0;
        std::vector<int> points;
    };
    
    class MMTrace {
    public:
        static MMTrace create(std::string data);
        MMPoint getPoint(int index);
        
        cocos2d::Vec2 getLocation(int point, cocos2d::Vec2 center, cocos2d::Size size);
        cocos2d::Vec2 getLocation(MMPoint point, cocos2d::Vec2 center, cocos2d::Size size);
        
        std::vector<MMPoint> points;
        std::vector<MMPart> paths;
    };
    
    
}}

#endif /* ParseJSONHelper_hpp */
