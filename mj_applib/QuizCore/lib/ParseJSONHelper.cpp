//
//  ParseJSONHelper.cpp
//  MonkeyMath
//
//  Created by HuyPhuong on 6/28/18.
//

#include "ParseJSONHelper.hpp"
#include "MJHelper.h"
#include "Math_Func.h"

using namespace math::JSON;

Parse & Parse::getInstance() {
    static Parse shared_manager;
    return shared_manager;
}

Parse::Parse() {
    init();
}

bool Parse::init() {
    return true;
}

cocos2d::Size Parse::getSize(rapidjson::Value& parent, std::string member) {
    if (parent.IsNull()) {
        CCLOG("NULL");
        return cocos2d::Size(0, 0);
    }
    int w = getInt(0, parent, member, "width");
    int h = getInt(0, parent, member, "height");
    return cocos2d::Size(w, h);
}

int Parse::randomRange(rapidjson::Value& parent, std::string member) {
    if (parent.IsNull()) {
        CCLOG("NULL");
        return 0;
    }
    int min = getInt(0, parent, member, "minimum");
    int max = getInt(0, parent, member, "maximum");
    return cocos2d::random(min, max);
}

rapidjson::Value& Parse::getChild(rapidjson::Value& parent, std::string child_name) {
    if (parent.IsNull()) {
        CCLOG("NULL");
        return parent;
    }
    if (parent.HasMember(child_name.c_str())) {
        return parent[child_name.c_str()];
    }
    rapidjson::Value v;
	return v.SetNull();
}

bool Parse::getBool(bool value, rapidjson::Value& parent, std::string child_name) {
    if (parent.IsObject() && parent.HasMember(child_name.c_str())) {
        rapidjson::Value& json_value = parent[child_name.c_str()];
        if (json_value.IsBool()) {
            return json_value.GetBool();
        }
    }
    return value;
}

int Parse::getInt(int value, rapidjson::Value& parent, std::string child_name) {
	if (parent.IsObject() && parent.HasMember(child_name.c_str())) {
        rapidjson::Value& json_value = parent[child_name.c_str()];
        if (json_value.IsInt()) {
            return json_value.GetInt();
        }
    }
    return value;
}

float Parse::getDouble(float value, rapidjson::Value& parent, std::string child_name) {
	if (parent.IsObject() && parent.HasMember(child_name.c_str())) {
        rapidjson::Value& json_value = parent[child_name.c_str()];
        if (json_value.IsDouble()) {
            return json_value.GetDouble();
        }
    }
    return value;
}

std::string Parse::getString(std::string value, rapidjson::Value& parent, std::string child_name) {
	if (parent.IsObject() && parent.IsObject() && parent.HasMember(child_name.c_str())) {
        rapidjson::Value& json_value = parent[child_name.c_str()];
        if (json_value.IsString()) {
            std::string ret = json_value.GetString();
            return math::func::trim(ret);
        }
    }
    return value;
}

#pragma mark - TRACING CONFIG POINT + PART
MMTrace MMTrace::create(std::string data) {
    MMTrace trace = MMTrace();
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseStopWhenDoneFlag>(data.c_str());
    rapidjson::Value& m_points = JSON_PARSE.getChild(doc, "position");
    if(m_points.IsNull()) return trace;
    for (rapidjson::SizeType i = 0; i<m_points.Size(); i++) {
        MMPoint point;
        point.index = JSON_PARSE.getInt(static_cast<int>(i), m_points[i], "order");
        point.x = JSON_PARSE.getInt(0, m_points[i], "horizontal_axis");
        point.y = JSON_PARSE.getInt(0, m_points[i], "vertical_axis");
        trace.points.push_back(point);
    }

    rapidjson::Value& m_parts = JSON_PARSE.getChild(doc, "paths");
    for (rapidjson::SizeType i = 0; i<m_parts.Size(); i++) {
        MMPart part;
        part.index = JSON_PARSE.getInt(static_cast<int>(i), m_parts[i], "path_number");
        std::string point_list = JSON_PARSE.getString("", m_parts[i], "point");
        auto splits = mj::helper::split(point_list, ',');
        for (auto num : splits) {
            part.points.push_back(std::atoi(num.c_str()));
        }
        trace.paths.push_back(part);
    }

    std::sort(trace.points.begin(), trace.points.end(), [=](MMPoint a, MMPoint b)->bool{
        return a.index < b.index;
    });
    std::sort(trace.paths.begin(), trace.paths.end(), [=](MMPart a, MMPart b)->bool{
        return a.index < b.index;
    });
    return trace;
}

MMPoint MMTrace::getPoint(int index) {
    for (auto point : points) {
        if (point.index == index) {
            return point;
        }
    }
    return MMPoint();
}

cocos2d::Vec2 MMTrace::getLocation(int point, cocos2d::Vec2 center, cocos2d::Size size) {
    auto mmpoint = getPoint(point);
    return getLocation(mmpoint, center, size);
}

cocos2d::Vec2 MMTrace::getLocation(MMPoint point, cocos2d::Vec2 center, cocos2d::Size size) {
    cocos2d::Vec2 origin = cocos2d::Vec2(center.x - size.width/2, center.y - size.height/2);
    float padding = 0;
    float fix_x = (0.5 - (float)point.x/1000.0) * padding + point.x;
    float fix_y = (0.5 - (float)point.y/1000.0) * padding + point.y;
    cocos2d::Vec2 point_in_img = cocos2d::Vec2(fix_x * size.width / 1000.0, fix_y * size.height / 1000.0);
    return  origin + point_in_img;
}

