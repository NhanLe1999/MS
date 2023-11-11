//
//  Math_Libs.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#include "Math_Libs.h"
#include "PoolManager.h"
#include "TestingModeUtils.h"
#include "ParseJSONHelper.hpp"
#include "Math_Func.h"

using namespace std;

#define image_new_struct false

#pragma mark - Theme
static math::theme::Theme* instance;
math::theme::Theme* math::theme::Theme::getInstance()
{
    if (instance==nullptr) {
        instance=new math::theme::Theme();
    }
    return instance;
}

void math::theme::Theme::setTheme(math::theme::MathTheme _theme)
{
    this->theme=_theme;
}

math::theme::MathTheme math::theme::Theme::getTheme()
{
    return theme;
}

std::string math::theme::Theme::getThemePath()
{
    switch (theme) {
        case math::theme::default_theme :
            return "themes/default/";
            break;
        case math::theme::story_theme :
            return "themes/story_theme/";
            break;
        default:
            break;
    }
    return "themes/default/";
}

std::string math::theme::Theme::getThemeSuffix()
{
    switch (theme) {
        case math::theme::default_theme :
            return "";
            break;
        default:
            break;
    }
    return "";
}



#pragma mark - Statistic
static math::statistic::Manager* s_instance;
math::statistic::Manager* math::statistic::Manager::getInstance()
{
    if (s_instance==nullptr) {
        s_instance=new math::statistic::Manager();
    }
    return s_instance;
}

void math::statistic::Manager::setCurrentGame(mj::model::Game game) {
    _game = game;
    
    //Set game path
    
    _path = getGamePath(_game);

    //Get game type
//    auto path = cocos2d::StringUtils::format("%sinfo.json", _path.c_str());
//    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
//    rapidjson::Document document;
//    document.Parse<0>(str.c_str());
//
//    _type = document.FindMember("game_type")->value.GetString();
//    _stars = document.FindMember("star")->value.GetInt();
//
//    rapidjson::Value& require = document.FindMember("require")->value;
//    _time_limit = require.FindMember("time")->value.FindMember("limit")->value.GetInt();
//    _time_exellent = require.FindMember("time")->value.FindMember("exellent")->value.GetInt();
//    _submit_limit = require.FindMember("submit")->value.FindMember("limit")->value.GetInt();
//    _submit_exellent = require.FindMember("submit")->value.FindMember("exellent")->value.GetInt();
}

std::string math::statistic::Manager::getGamePath() {
    return _path;
}

std::string math::statistic::Manager::getGamePath(mj::model::Game game) {
    
    if (testing::TestManager::getInstance()->getTestMode() == testing::activity) {
        return cocos2d::StringUtils::format("%sgames/ac_%05d/", cocos2d::FileUtils::getInstance()->getWritablePath().c_str(), testing::TestManager::getInstance()->getActivity()._id);
    } else if (testing::TestManager::getInstance()->getTestMode() == testing::stories) {
        return "";
    }

//    auto level = mj::model::Manager::getInstance().getLevelOfGame(game.getId());
//    auto skill = mj::model::Manager::getInstance().getSkillById(game.getSkillId());
//    auto grade = mj::model::Manager::getInstance().getGradeById(game.getGradeId());
//    auto path = cocos2d::StringUtils::format("game/%s/%s/%s/game%03d/", grade.getName().c_str(), skill.getName().c_str(), level.getName().c_str(), game.getOrder());
//    CCLOG("Game Path: %s", path.c_str());
    return "";
}

void math::statistic::Manager::copyFile() {
    auto r_path = cocos2d::FileUtils::getInstance()->getWritablePath() + _path;
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(r_path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(r_path);
    }
    
    if (!cocos2d::FileUtils::getInstance()->isFileExist(r_path+"info.json")) {
        //copy file
        auto data =  cocos2d::FileUtils::getInstance()->getDataFromFile(_path+"info.json");
        FILE* dest = std::fopen(cocos2d::StringUtils::format("%sinfo.json", r_path.c_str()).c_str(), "wb");
        fwrite(data.getBytes(), 1, data.getSize(), dest);
        fclose(dest);
    }
}

void math::statistic::Manager::copyFile(mj::model::Game game) {
    auto path = getGamePath(game);
    
    auto r_path = cocos2d::FileUtils::getInstance()->getWritablePath() + path;
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(r_path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(r_path);
    }
    
    if (!cocos2d::FileUtils::getInstance()->isFileExist(r_path+"info.json")) {
        //copy file
        auto data =  cocos2d::FileUtils::getInstance()->getDataFromFile(_path+"info.json");
        FILE* dest = std::fopen(cocos2d::StringUtils::format("%sinfo.json", r_path.c_str()).c_str(), "wb");
        fwrite(data.getBytes(), 1, data.getSize(), dest);
        fclose(dest);
    }
}


void math::statistic::Manager::addUsedIcon(std::string set, int _id) {

    auto r_path = cocos2d::FileUtils::getInstance()->getWritablePath() + _path + "info.json";
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(r_path);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    rapidjson::Value input (rapidjson::kObjectType);
    input.SetObject();
    input.AddMember("set", rapidjson::Value().SetString(set.c_str(), (rapidjson::SizeType)set.length()), allocator);
    input.AddMember("icon", _id, allocator);

    rapidjson::Value& icon_used = document.FindMember("icon_used")->value;
    icon_used.PushBack(input, allocator);
    
    FILE* fp = std::fopen(r_path.c_str(), "wb"); // non-Windows use "w"
    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    document.Accept(writer);
    fclose(fp);
}

void math::statistic::Manager::addPlayedInfo(int time, int submit) {
    
    auto r_path = cocos2d::FileUtils::getInstance()->getWritablePath() + _path + "info.json";
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(r_path);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    rapidjson::Value input (rapidjson::kObjectType);
    input.AddMember("time", time, allocator);
    input.AddMember("submit", submit, allocator);
    
    rapidjson::Value& statistic = document.FindMember("statistic")->value;
    statistic.PushBack(input, allocator);
    
    FILE* fp = std::fopen(r_path.c_str(), "wb"); // non-Windows use "w"
    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    document.Accept(writer);
    fclose(fp);
}


#pragma mark - Language
static math::lang::Language* l_instance;
math::lang::Language* math::lang::Language::getInstance()
{
    if (l_instance==nullptr) {
        l_instance=new math::lang::Language();
    }
    return l_instance;
}

void math::lang::Language::setLanguage(math::lang::MathLanguage _lang)
{
    lang=_lang;
    switch (lang) {
        case math::lang::english_us :
            language_name="en";
            break;
        case math::lang::vietnamese :
            language_name="vi";
            break;
        default:
            language_name="en";
            break;
    }
}

math::lang::MathLanguage math::lang::Language::getLanguage(std::string name)
{
    if (name == "en") {
        return math::lang::english_us;
    } else if (name == "vi") {
        return math::lang::vietnamese;
    }
    return lang;
}

std::string math::lang::Language::getLanguageName()
{
    return language_name;
}

#pragma mark - Image
void math::resource::Image::clear()
{
    used=false;
    enable=false;
    properties.clear();
    comparisions.clear();
    
    delete this;
}

void math::resource::Image::addProperty(std::string key, std::string value, std::vector<std::string> audios) {
    math::resource::Voice voice;
    voice.voice_id = 1;
    voice.paths = audios;
    
    math::resource::Text text;
    text.text = value;
    text.voices.push_back(voice);
    
    math::resource::Property prop_add;
    prop_add.type = math::resource::Property::PROP;
    prop_add.key = key;
    prop_add.value = text;
    
    if (this->properties.empty()) {
        math::resource::PropertiesSet prop_set;
        prop_set.lang = m_lang->getLanguage();
        prop_set.properties.push_back(prop_add);
        properties.push_back(prop_set);
    } else {
        this->properties.front().properties.push_back(prop_add);
    }
}

void math::resource::Image::addComparision(std::string key, int value) {
    math::resource::Property prop;
    prop.type = math::resource::Property::COMP;
    prop.compare_value = value;
    prop.key = "number";
    this->comparisions.push_back(prop);
}

math::resource::Text math::resource::Image::getTextValue(std::string key, lang::MathLanguage lang) {
    for (math::resource::PropertiesSet prop_set : properties) {
        if (prop_set.lang == lang) {
            for (math::resource::Property prop : prop_set.properties) {
                if (prop.key == key) {
                    if (prop.type == Property::PROP || prop.type == Property::PHONIC) {
                        return prop.value;
                    }
                    break;
                }
            }
            break;
        }
    }
    math::resource::Text txt;
    return txt;
}

std::string math::resource::Image::getText(std::string key, lang::MathLanguage lang) {
    return getTextValue(key, lang).text;
}

std::string math::resource::Image::getQuestionVMonkey(std::string key, lang::MathLanguage lang) {
	std::string text = this->getText(key, lang);
	std::vector<int>break_points;
	for (math::resource::Property prop : comparisions) {
		if (prop.key == "length") {
			if (prop.type == Property::COMP) {
				break_points.push_back(prop.compare_value);
			}
		}
	}
	if (!break_points.empty()) {
		int count = 0;
		int j = 0;
		for (int i = 0; i < text.size(); i++) {
			if (text[i] == ' ') count++;
			if (count == break_points[j]) {
				text.insert(i + 1, "\n");
				count = 0;
				j++;
				if (j >= break_points.size()) break;
			}
		}
	}
	return text;
}

std::string math::resource::Image::getAudio(std::string key, lang::MathLanguage lang, int voice_id) {
    return getTextValue(key, lang).getVoices(voice_id).front();
}

std::vector<std::string> math::resource::Image::getAudios(std::string key, lang::MathLanguage lang, int voice_id) {
    return getTextValue(key, lang).getVoices(voice_id);
}


std::vector<std::string> math::resource::Image::getTextPhonic(std::string key, lang::MathLanguage lang) {
    //TODO: get text phonic
    for (math::resource::PropertiesSet prop_set : properties) {
        if (prop_set.lang == lang) {
            for (math::resource::Property prop : prop_set.properties) {
                if (prop.key == key) {
                    if (prop.type == Property::PHONIC) {
                        std::vector<std::string> phonics;
                        for(auto v: prop.value.voices){
                            phonics.push_back(v.phonic_text);
                        }
                        return phonics;
                    }
                    break;
                }
            }
            break;
        }
    }
    return {};
}
std::vector<std::string> math::resource::Image::getAudioPhonic(std::string key, lang::MathLanguage lang) {
    //TODO: get phonic audio
    for (math::resource::PropertiesSet prop_set : properties) {
        if (prop_set.lang == lang) {
            for (math::resource::Property prop : prop_set.properties) {
                if (prop.key == key) {
                    if (prop.type == Property::PHONIC) {
                        std::vector<std::string> phonic_audios;
                        for(auto v: prop.value.voices){
                            phonic_audios.push_back(v.paths.front());
                        }
                        return phonic_audios;
                    }
                    break;
                }
            }
            break;
        }
    }
    return {};
}


math::resource::Video math::resource::Image::getVideo(std::string key , lang::MathLanguage lang) {
	return getTextValue(key, lang).video;
}

std::vector<math::resource::Sync_Text> math::resource::Image::getSyncText(std::string key, lang::MathLanguage lang, int voice_id)
{
	return getTextValue(key, lang).getSyncText(voice_id);
}

int math::resource::Image::getComparisionValue(std::string key)
{
    for (math::resource::Property prop : comparisions) {
        if (prop.key == key) {
            if (prop.type == Property::COMP) {
                return prop.compare_value;
            }
            break;
        }
    }
    return -1;
}

bool math::resource::Image::isExist(std::string key, lang::MathLanguage lang){
    for (math::resource::PropertiesSet prop_set : properties) {
        if (prop_set.lang == lang) {
            for (math::resource::Property prop : prop_set.properties) {
                if (prop.key == key) {
                    if (prop.type == Property::PROP) {
                        return true;
                    }
                    break;
                }
            }
            break;
        }
    }
    return false;
}

#pragma mark - Resource Helper
math::resource::Image* math::resource::ResHelper::createImage(std::string path)
{
    math::resource::Image* img = mpool->getImage();
    img->path = path;
    img->code = path;
    img->enable = true;
    img->comparisions.clear();
    img->properties.clear();
    
    return img;
}

math::resource::Image* math::resource::ResHelper::mergeImages(std::vector<math::resource::Image*> images)
{
    math::resource::Image* img = mpool->getImage();
    img->path = images.front()->path;
    img->enable = true;
    img->comparisions.clear();
    img->properties.clear();
    
    for (math::resource::PropertiesSet prop_set : images.front()->properties) {
        math::resource::PropertiesSet add_set;
        add_set.lang = prop_set.lang;
        for (math::resource::Property prop : prop_set.properties) {
            math::resource::Property add_prop;
            add_prop.type = prop.type;
            add_prop.key = prop.key;
            
            math::resource::Voice voice;
            voice.voice_id = 1;
            std::vector<std::string> voice_path;
            
            math::resource::Text text;
            std::string text_value;
        
            for (math::resource::Image* image : images) {
                for (std::string _voice : image->getAudios(prop.key)) {
                    voice_path.push_back(_voice);
                }
                text_value = cocos2d::StringUtils::format("%s %s", text_value.c_str(), image->getText(prop.key).c_str());
            }
            
            voice.paths = voice_path;
            text.text = text_value;
            text.voices.push_back(voice);
            add_prop.value = text;
            add_set.properties.push_back(add_prop);
        }
        img->properties.push_back(add_set);
    }
    
    for (math::resource::Property prop_cmp : images.front()->comparisions) {
        img->comparisions.push_back(prop_cmp);
    }
    return img;
}


std::vector<math::resource::Image*> math::resource::ResHelper::getImages(std::string set, std::string name, std::string prefix)
{
    std::vector<math::resource::Image*> set_images;
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(set);
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    CCLOG("json %s",str.c_str());
    if (document.IsArray()) {
        //New version
        rapidjson::Value& data_images = document[0];
        std::string lang_name = JSON_PARSE.getString("en", data_images, "language");
        auto image_lang = m_lang->getLanguage(lang_name);
        rapidjson::Value& icons_json = JSON_PARSE.getChild(data_images, "icons");
        
        for (rapidjson::SizeType index = 0; index<icons_json.Size(); index++) {
            rapidjson::Value& icon_json = icons_json[index];
            
            auto image = mpool->getImage();
            image->code = JSON_PARSE.getString(cocos2d::StringUtils::toString(index + 1), icon_json, "icon_id");
            image->media_type = JSON_PARSE.getInt(1, icon_json, "type_media");
            std::string path = JSON_PARSE.getString("", icon_json, "path");
            if (image->media_type == 3) {
                auto p = path.find("images");
                if (p != std::string::npos) {
                    path.replace(path.begin(), path.begin()+6, "gafs");
                }
            }
            image->path = prefix + path;
            image->tracing = JSON_PARSE.getString("", icon_json, "tracing");
            
            image->comparisions.clear();
            image->properties.clear();
            
            //properties
            PropertiesSet prop_set;
            prop_set.lang = image_lang;
            
            Property property_id;
            property_id.type = Property::PROP;
            property_id.key = "id";
            property_id.compare_value = 0;
            property_id.value.text = image->code;
            prop_set.properties.push_back(property_id);
            
            rapidjson::Value& props = JSON_PARSE.getChild(icon_json, "props");
            for (rapidjson::SizeType j = 0; j < props.Size(); j++) {
                Property property;
                bool is_phonic = JSON_PARSE.getBool(false, props[j], "phonic");
                if (is_phonic) {
                    property.type = Property::PHONIC;
                } else {
                    property.type = Property::PROP;
                }
                property.compare_value = 0;
                property.key = JSON_PARSE.getString("name_1", props[j], "key");
                property.value.text = JSON_PARSE.getString("", props[j], "text");
                
                //voices
                rapidjson::Value& audio_js = JSON_PARSE.getChild(props[j], "audio");
                if (audio_js.IsString()) {
                    Voice voice;
                    voice.voice_id = 0;
                    voice.paths.push_back(prefix + audio_js.GetString());
                    property.value.voices.push_back(voice);
                } else {
                    for (rapidjson::SizeType k = 0; k < audio_js.Size(); k++) {
                        Voice voice;
                        if (is_phonic) {
                            voice.phonic_text = JSON_PARSE.getString("", audio_js[k], "text");
                        }
                        voice.voice_id = JSON_PARSE.getInt(10, audio_js[k], "voice_id");
                        voice.paths.push_back(prefix + JSON_PARSE.getString("", audio_js[k], "path"));
                        if (audio_js[k].HasMember("sync_data")){
                            auto& syn_value = JSON_PARSE.getChild(audio_js[k], "sync_data");
                            if (syn_value.IsString()) {
                                std::string sync_json = syn_value.GetString();
                                rapidjson::Document sync_data;
                                sync_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(sync_json.c_str());
                                if (sync_data.HasParseError()) {
                                    //CCLOG("JSON ERROR! \n\n %s", sync_json.c_str());
                                }
                                else{
                                    if (sync_data.IsArray()) {
                                        //doc sync text o day, chua co mau chua lamf dc
                                        for (rapidjson::SizeType i = 0; i < sync_data.Size(); i++){
                                            rapidjson::Value& object_value = sync_data[i];
                                            Sync_Text st;
                                            st.w = JSON_PARSE.getString("", object_value, "w");
                                            st.s = JSON_PARSE.getDouble(0, object_value, "s");
                                            st.e = JSON_PARSE.getDouble(0, object_value, "e");
                                            st.ts = JSON_PARSE.getInt(0, object_value, "ts");
                                            st.te = JSON_PARSE.getInt(0, object_value, "te");
                                            voice.sync_texts.push_back(st);
                                        }
                                    }
                                }
                            }
                        }
                        property.value.voices.push_back(voice);
                    }
                }
                //video
                Video video;
                video.path = prefix + JSON_PARSE.getString("", props[j], "video", "path");
                video.duration = JSON_PARSE.getDouble(0, props[j], "video", "duration");
                
                if (property.key == "name_1") {
                    Property property_name;
                    property_name.type = Property::PROP;
                    property_name.key = "name";
                    property_name.compare_value = 0;
                    property_name.value.text = property.value.text;
                    
                    for (auto mvoice : property.value.voices) {
                        Voice voice;
                        voice.voice_id = mvoice.voice_id;
                        voice.paths = mvoice.paths;
                        voice.sync_texts = mvoice.sync_texts;
                        property_name.value.voices.push_back(voice);
                    }
                    prop_set.properties.push_back(property_name);
                }
                
                prop_set.properties.push_back(property);
            }
            image->properties.push_back(prop_set);
            
            //comparisions
            rapidjson::Value& icon_comparision = JSON_PARSE.getChild(icon_json, "comparision");
            for (rapidjson::SizeType i=0; i<icon_comparision.Size(); i++) {
                rapidjson::Value& comp_prop = icon_comparision[i];
                Property property;
                property.type = Property::COMP;
                property.key = JSON_PARSE.getString("number", comp_prop, "key");
                property.compare_value = JSON_PARSE.getInt(0, comp_prop, "value");
                image->comparisions.push_back(property);
            }
            
            Property property_none;
            property_none.type = Property::COMP;
            property_none.key = "none";
            property_none.compare_value = 0;
            image->comparisions.push_back(property_none);
            
            set_images.push_back(image);
        }
    }
    
    if (name.compare("")==0) {
        return set_images;
    } else {
        std::vector<math::resource::Image*> images_fillter;
        for (math::resource::Image* image : set_images) {
            if (image->getText("name_1") == name) {
                images_fillter.push_back(image);
            }
        }
        return images_fillter;
    }
}

//std::vector<math::resource::Image*> math::resource::ResHelper::getImages(std::string set, std::string name, std::string prefix)
//{
//    std::vector<math::resource::Image*> set_images;
//    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(set);
//
//    rapidjson::Document document;
//    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
//
//    if (document.IsObject()) {
//        rapidjson::Value& icons_json = document.FindMember("icons")->value;
//
//        for (rapidjson::SizeType index = 0; index<icons_json.Size(); index++) {
//            rapidjson::Value& icon_json = icons_json[index];
//
//            auto image = mpool->getImage();
//            image->enable = true;
//            if (icon_json.FindMember("icon_id")->value.IsInt()) {
//                image->code = cocos2d::StringUtils::toString(icon_json.FindMember("icon_id")->value.GetInt());
//            } else {
//                image->code = icon_json.FindMember("icon_id")->value.GetString();
//            }
//            image->path = prefix + icon_json.FindMember("path")->value.GetString();
//            if (icon_json.HasMember("tracing")) {
//                image->tracing = icon_json.FindMember("tracing")->value.GetString();
//            }
//
//            image->comparisions.clear();
//            image->properties.clear();
//
//            //properties
//            rapidjson::Value& languages = icon_json.FindMember("languages")->value;
//            for (rapidjson::SizeType i = 0; i < languages.Size(); i++) {
//
//                PropertiesSet prop_set;
//                rapidjson::Value& properties_set = languages[i];
//                std::string lang_name = properties_set.FindMember("lang")->value.GetString();
//                prop_set.lang = m_lang->getLanguage(lang_name);
//
//                rapidjson::Value& props = properties_set.FindMember("props")->value;
//                for (rapidjson::SizeType j = 0; j < props.Size(); j++) {
//                    Property property;
//                    property.type = Property::PROP;
//                    property.key = props[j].FindMember("key")->value.GetString();
//                    property.compare_value = 0;
//                    property.value.text = props[j].FindMember("text")->value.GetString();
//
//                    //voices
//                    rapidjson::Value& audio_js = props[j].FindMember("audio")->value;
//                    if (audio_js.IsString()) {
//                        Voice voice;
//                        voice.voice_id = 0;
//                        voice.paths.push_back(prefix + audio_js.GetString());
//                        property.value.voices.push_back(voice);
//                    } else {
//                        for (rapidjson::SizeType k = 0; k < audio_js.Size(); k++) {
//                            Voice voice;
//                            voice.voice_id = audio_js[k].FindMember("voice_id")->value.GetInt();
//                            voice.paths.push_back(prefix + audio_js[k].FindMember("path")->value.GetString());
//                            if (audio_js[k].HasMember("sync_data")){
//                                std::string sync_json = audio_js[k].FindMember("sync_data")->value.GetString();
//                                rapidjson::Document sync_data;
//                                sync_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(sync_json.c_str());
//                                if (sync_data.HasParseError()) {
//                                    //CCLOG("JSON ERROR! \n\n %s", sync_json.c_str());
//                                }
//                                else{
//                                    if (sync_data.IsArray()) {
//                                        //doc sync text o day, chua co mau chua lamf dc
//                                        for (rapidjson::SizeType i = 0; i < sync_data.Size(); i++){
//                                            rapidjson::Value& object_value = sync_data[i];
//                                            Sync_Text st;
//                                            st.w = object_value.FindMember("w")->value.GetString();
//                                            st.s = object_value.FindMember("s")->value.GetDouble();
//                                            st.e = object_value.FindMember("e")->value.GetDouble();
//                                            st.ts = object_value.FindMember("ts")->value.GetInt();
//                                            st.te = object_value.FindMember("te")->value.GetInt();
//                                            voice.sync_texts.push_back(st);
//                                        }
//                                    }
//                                }
//                            }
//                            property.value.voices.push_back(voice);
//                        }
//                    }
//                    //video
//                    if (props[j].HasMember("video")) {
//                        rapidjson::Value& video_js = props[j].FindMember("video")->value;
//                        Video video;
//                        video.path = video_js.FindMember("path")->value.GetString();
//                        if (video_js.HasMember("duration")) {
//                            video.duration = video_js.FindMember("duration")->value.GetDouble();
//                        }
//                    }
//
//                    prop_set.properties.push_back(property);
//                }
//                image->properties.push_back(prop_set);
//            }
//
//            //comparisions
//            rapidjson::Value& icon_comparision=icon_json.FindMember("comparision")->value;
//            for (rapidjson::SizeType i=0; i<icon_comparision.Size(); i++) {
//                rapidjson::Value& comp_prop=icon_comparision[i];
//                Property property;
//                property.key = comp_prop.FindMember("key")->value.GetString();
//                property.type = Property::COMP;
//                property.compare_value = comp_prop.FindMember("value")->value.GetInt();
//                image->comparisions.push_back(property);
//            }
//            set_images.push_back(image);
//        }
//    } else if (document.IsArray()) {
//        rapidjson::Value& data_images = document[0];
//        std::string lang_name = data_images.FindMember("language")->value.GetString();
//        auto image_lang = m_lang->getLanguage(lang_name);
//        rapidjson::Value& icons_json = data_images.FindMember("icons")->value;
//
//        for (rapidjson::SizeType index = 0; index<icons_json.Size(); index++) {
//            rapidjson::Value& icon_json = icons_json[index];
//
//            auto image = mpool->getImage();
//            image->enable = true;
//            if (icon_json.FindMember("icon_id")->value.IsInt()) {
//                image->code = cocos2d::StringUtils::toString(icon_json.FindMember("icon_id")->value.GetInt());
//            } else {
//                image->code = icon_json.FindMember("icon_id")->value.GetString();
//            }
//            image->path = prefix + icon_json.FindMember("path")->value.GetString();
//            if (icon_json.HasMember("tracing")) {
//                if (icon_json.FindMember("tracing")->value.IsString())
//                    image->tracing = icon_json.FindMember("tracing")->value.GetString();
//            }
//            image->comparisions.clear();
//            image->properties.clear();
//
//            //properties
//            PropertiesSet prop_set;
//            prop_set.lang = image_lang;
//
//            rapidjson::Value& props = icon_json.FindMember("props")->value;
//            for (rapidjson::SizeType j = 0; j < props.Size(); j++) {
//                Property property;
//                property.type = Property::PROP;
//                property.key = props[j].FindMember("key")->value.GetString();
//                property.compare_value = 0;
//                property.value.text = props[j].FindMember("text")->value.GetString();
//
//                //voices
//                rapidjson::Value& audio_js = props[j].FindMember("audio")->value;
//                if (audio_js.IsString()) {
//                    Voice voice;
//                    voice.voice_id = 0;
//                    voice.paths.push_back(prefix + audio_js.GetString());
//                    property.value.voices.push_back(voice);
//                } else {
//                    for (rapidjson::SizeType k = 0; k < audio_js.Size(); k++) {
//                        Voice voice;
//                        voice.voice_id = audio_js[k].FindMember("voice_id")->value.GetInt();
//                        voice.paths.push_back(prefix + audio_js[k].FindMember("path")->value.GetString());
//                        if (audio_js[k].HasMember("sync_data")){
//                            std::string sync_json = audio_js[k].FindMember("sync_data")->value.GetString();
//                            rapidjson::Document sync_data;
//                            sync_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(sync_json.c_str());
//                            if (sync_data.HasParseError()) {
//                                //CCLOG("JSON ERROR! \n\n %s", sync_json.c_str());
//                            }
//                            else{
//                                if (sync_data.IsArray()) {
//                                    //doc sync text o day, chua co mau chua lamf dc
//                                    for (rapidjson::SizeType i = 0; i < sync_data.Size(); i++){
//                                        rapidjson::Value& object_value = sync_data[i];
//                                        Sync_Text st;
//                                        st.w = object_value.FindMember("w")->value.GetString();
//                                        st.s = object_value.FindMember("s")->value.GetDouble();
//                                        st.e = object_value.FindMember("e")->value.GetDouble();
//                                        st.ts = object_value.FindMember("ts")->value.GetInt();
//                                        st.te = object_value.FindMember("te")->value.GetInt();
//                                        voice.sync_texts.push_back(st);
//                                    }
//                                }
//                            }
//                        }
//                        property.value.voices.push_back(voice);
//                    }
//                }
//                //video
//                if (props[j].HasMember("video")) {
//                    rapidjson::Value& video_js = props[j].FindMember("video")->value;
//                    Video video;
//                    video.path = video_js.FindMember("path")->value.GetString();
//                    if (video_js.HasMember("duration")) {
//                        video.duration = video_js.FindMember("duration")->value.GetDouble();
//                    }
//                }
//
//                prop_set.properties.push_back(property);
//            }
//            image->properties.push_back(prop_set);
//
//            //comparisions
//            rapidjson::Value& icon_comparision=icon_json.FindMember("comparision")->value;
//            for (rapidjson::SizeType i=0; i<icon_comparision.Size(); i++) {
//                rapidjson::Value& comp_prop=icon_comparision[i];
//                Property property;
//                property.key = comp_prop.FindMember("key")->value.GetString();
//                property.type = Property::COMP;
//                property.compare_value = comp_prop.FindMember("value")->value.GetInt();
//                image->comparisions.push_back(property);
//            }
//            set_images.push_back(image);
//        }
//    }
//
//    if (name.compare("")==0) {
//        return set_images;
//    } else {
//        std::vector<math::resource::Image*> images_fillter;
//        for (math::resource::Image* image : set_images) {
//            if (image->getText("name_1") == name) {
//                images_fillter.push_back(image);
//            }
//        }
//        return images_fillter;
//    }
//}

math::resource::Image* math::resource::ResHelper::getImage(std::string set, std::string icon_id)
{
    auto images=math::resource::ResHelper::getImages(set);
    math::resource::Image* image = nullptr;
    if (icon_id == "") {
        image = math::func::getRandomNode(images);
        for (math::resource::Image* img : images) {
            img->enable=false;
        }
        image->enable=true;
    } else {
        for (math::resource::Image* img : images) {
            if (img->code == icon_id) {
                image = img;
            } else {
                img->enable = false;
            }
        }
    }
    return image;
}

math::resource::Image* math::resource::ResHelper::getImage(std::vector<Image*> imgs, std::string icon_id /*= ""*/)
{
	std::string str_icon = icon_id;
	str_icon.erase(std::remove(str_icon.begin(), str_icon.end(), ' '), str_icon.end());

	if (str_icon.find("\t") != str_icon.npos){
		str_icon = str_icon.substr(0, str_icon.find("\t"));
	}
	for (math::resource::Image* img : imgs) {
		std::string str = img->code;
		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

		if (str.find("\t") != str.npos){
			str = str.substr(0, str.find("\t"));
		}
		if (str_icon == str) {
			return img;
		}
	}
	return nullptr;
}

std::vector<math::resource::Image*> math::resource::ResHelper::filterImages(std::vector<math::resource::Image *> images, std::string key)
{
    if (images.size()==0 || key == "" || key == "none") {
        return images;
    }
    
    std::vector<math::resource::Image*> result;
    if (key == "name") {
        key = "name_1";
    }
    
    //check filter is prop
    bool filter_prop = true;
    for (math::resource::Property prop : images.front()->comparisions) {
        if (prop.key == key) {
            filter_prop = false;
            break;
        }
    }
    
    if (filter_prop) {
        std::string prop_value =images[cocos2d::random(0, (int)images.size()-1)]->getText(key);
        for (math::resource::Image* image : images) {
            if (image->getText(key).compare(prop_value)==0) {
                result.push_back(image);
            }
        }
    } else {
        int compare_value = images[cocos2d::random(0, (int)images.size()-1)]->getComparisionValue(key);
        for (math::resource::Image* image : images) {
            if (image->getComparisionValue(key) == compare_value) {
                result.push_back(image);
            }
        }
    }
    return result;
}

std::vector<math::resource::Image*> math::resource::ResHelper::filterImages(std::vector<math::resource::Image*> images, std::vector<std::string> keys)
{
    if (keys.size()==0) {
        return images;
    }
    std::string comp_key = keys.front();
    keys.erase(keys.begin());
    return math::resource::ResHelper::filterImages(images, comp_key);
}

std::vector<math::resource::Image*> math::resource::ResHelper::filterImages(std::vector<math::resource::Image *> images, std::string key, cocos2d::Value value)
{
    if (value == cocos2d::Value::Null) {
        return math::resource::ResHelper::filterImages(images, key);
    } else {
        std::vector<math::resource::Image*> result;
        if (key == "name") {
            key = "name_1";
        }
        
        //check filter is prop
        bool filter_prop = true;
        for (math::resource::Property prop : images.front()->comparisions) {
            if (prop.key == key) {
                filter_prop = false;
                break;
            }
        }
        
        if (filter_prop) {
            std::string prop_value = value.asString();
            for (math::resource::Image* image : images) {
                if (image->getText(key).compare(prop_value)==0) {
                    result.push_back(image);
                }
            }
        } else {
            int compare_value = value.asInt();
            for (math::resource::Image* image : images) {
                if (image->getComparisionValue(key) == compare_value) {
                    result.push_back(image);
                }
            }
        }
        return result;
    }
}


#pragma mark - question
std::vector<math::question::Question> math::question::Parse::parseQuestion(std::string game_path, std::string prefix, bool loop, int loop_number) {
    
    std::vector<math::question::Question> question_list;
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%squestion.json", game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_questions=document.FindMember("question")->value;
    rapidjson::SizeType index_lang=0;
    for (; index_lang < json_questions.Size(); index_lang++) {
        std::string choose_lang=json_questions[index_lang].FindMember("lang")->value.GetString();
        if (choose_lang.compare("en")==0 || choose_lang.compare("english_us")==0 || choose_lang.compare("en-US")==0) {
            break;
        }
    }
    if (index_lang>=json_questions.Size()) {
        return question_list;
    }
    rapidjson::Value& questions = json_questions[index_lang].FindMember("value")->value;
    
    int number_question = (int)questions.Size();
    if (loop) {
        number_question = loop_number;
    }
    
    for (rapidjson::SizeType question_index = 0; question_index < number_question; question_index++) {
        rapidjson::SizeType ques_index = question_index;
        if (loop) {
            ques_index = 0;
        }
        rapidjson::Value& _question = questions[ques_index];
        
        math::question::Question question_data;
        question_data.index = question_index;
        
        //Lấy thông tin các thành phàn chính của question
        rapidjson::Value& _template = _question.FindMember("template")->value;
        rapidjson::Value& _keyword = _question.FindMember("keyword")->value;
        rapidjson::Value& _optional = _question.FindMember("optional")->value;
        
        for (rapidjson::SizeType i1 = 0; i1 < _template.Size(); i1++) {
            rapidjson::Value& m_temp = _template[i1];
            int order = m_temp.FindMember("order")->value.GetInt();
            std::string type = m_temp.FindMember("type")->value.GetString();
            
            math::question::Structure structure;
            structure.index = order;
            if (type.compare("template")==0) {
                structure.type = math::question::TEMPLATE;
            } else if (type.compare("keyword")==0) {
                structure.type = math::question::KEYWORD;
            } else {
                structure.type = math::question::OPTIONAL;
            }
            
            structure.highlight = false;
            structure.showicon = false;
            if (m_temp.HasMember("highlight")) {
                structure.highlight = m_temp.FindMember("highlight")->value.GetBool();
            }
            if (m_temp.HasMember("show_icon")) {
                structure.showicon = m_temp.FindMember("show_icon")->value.GetBool();
            }
            
            switch (structure.type) {
                case math::question::TEMPLATE:
                {
                    structure.template_text.text = m_temp.FindMember("text")->value.GetString();
                    if (m_temp.HasMember("audios")) {
                        rapidjson::Value& voice_json = m_temp.FindMember("audios")->value;
                        for (rapidjson::SizeType i2 = 0; i2 < voice_json.Size(); i2++) {
                            math::resource::Voice voice;
                            voice.voice_id = voice_json[i2].FindMember("voice_id")->value.GetInt();
                            voice.paths.push_back(prefix + voice_json[i2].FindMember("path")->value.GetString());
                            structure.template_text.voices.push_back(voice);
                        }
                    } else {
                        math::resource::Voice voice;
                        voice.voice_id = 1;
                        //voice.paths = "";
                        structure.template_text.voices.push_back(voice);
                    }
                }
                    break;
                case math::question::KEYWORD:
                {
                    for (rapidjson::SizeType i2 = 0; i2 < _keyword.Size(); i2++) {
                        rapidjson::Value& m_keyword = _keyword[i2];
                        if (order == m_keyword.FindMember("order")->value.GetInt()) {
                            rapidjson::Value& keyword_list = m_keyword.FindMember("keyword")->value;
                            for (rapidjson::SizeType i3 = 0; i3 < keyword_list.Size(); i3++) {
                                math::question::Keyword keyword;
                                keyword.value = keyword_list[i3].FindMember("value")->value.GetInt();
                                keyword.text.text = keyword_list[i3].FindMember("text")->value.GetString();
                                
                                if (keyword_list[i3].HasMember("audios")) {
                                    rapidjson::Value& voice_json = keyword_list[i3].FindMember("audios")->value;
                                    for (rapidjson::SizeType i4 = 0; i4 < voice_json.Size(); i4++) {
                                        math::resource::Voice voice;
                                        rapidjson::Value& voice_id = voice_json[i4].FindMember("voice_id")->value;
                                        if (voice_id.IsInt()) {
                                            voice.voice_id = voice_json[i4].FindMember("voice_id")->value.GetInt();
                                        } else {
                                            voice.voice_id = 3;
                                        }
                                        voice.paths.push_back(prefix + voice_json[i4].FindMember("path")->value.GetString());
                                        keyword.text.voices.push_back(voice);
                                    }
                                } else {
                                    math::resource::Voice voice;
                                    voice.voice_id = 1;
                                    //voice.path = "";
                                    keyword.text.voices.push_back(voice);
                                }
                                structure.keywords.push_back(keyword);
                            }
                            break;
                        }
                    }
                }
                    break;
                default:
                {
                    for (rapidjson::SizeType i2=0; i2<_optional.Size(); i2++) {
                        rapidjson::Value& m_option = _optional[i2];
                        if (order == m_option.FindMember("order")->value.GetInt()) {
                            if (loop) {
                                structure.optional.index = (int)question_index;
                            } else {
                                structure.optional.index = m_option.FindMember("index")->value.GetInt();
                            }
                            structure.optional.key = m_option.FindMember("type")->value.GetString();
                        }
                    }
                }
                    break;
            }
            question_data.structures.push_back(structure);
        }
        
        question_data.ending = _question.FindMember("ending")->value.GetString();
        question_list.push_back(question_data);
    }
    
    return question_list;
}

std::vector<math::question::Question> math::question::Parse::parseQuestionWithLoops(std::string game_path, std::string prefix, std::vector<int> loops) {
    
    std::vector<math::question::Question> question_list;
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%squestion.json", game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_questions=document.FindMember("question")->value;
    rapidjson::SizeType index_lang=0;
    for (; index_lang < json_questions.Size(); index_lang++) {
        std::string choose_lang=json_questions[index_lang].FindMember("lang")->value.GetString();
        if (choose_lang.compare("en")==0 || choose_lang.compare("english_us")==0) {
            break;
        }
    }
    if (index_lang>=json_questions.Size()) {
        return question_list;
    }
    rapidjson::Value& questions = json_questions[index_lang].FindMember("value")->value;
    
    int index_loop = 0;
    int number_loop = loops[index_loop];
    
    int total_ques = 0;
    int number_question = (int)questions.Size();
    
    for (rapidjson::SizeType question_index = 0; question_index < number_question; question_index++) {
        rapidjson::SizeType ques_index = question_index;
        rapidjson::Value& _question = questions[ques_index];
        
        for (int i=0; i<number_loop; i++) {
            math::question::Question question_data;
            question_data.index = total_ques;
            
            //Lấy thông tin các thành phàn chính của question
            rapidjson::Value& _template = _question.FindMember("template")->value;
            rapidjson::Value& _keyword = _question.FindMember("keyword")->value;
            rapidjson::Value& _optional = _question.FindMember("optional")->value;
            
            for (rapidjson::SizeType i1 = 0; i1 < _template.Size(); i1++) {
                rapidjson::Value& m_temp = _template[i1];
                int order = m_temp.FindMember("order")->value.GetInt();
                std::string type = m_temp.FindMember("type")->value.GetString();
                
                math::question::Structure structure;
                structure.index = order;
                if (type.compare("template")==0) {
                    structure.type = math::question::TEMPLATE;
                } else if (type.compare("keyword")==0) {
                    structure.type = math::question::KEYWORD;
                } else {
                    structure.type = math::question::OPTIONAL;
                }
                
                structure.highlight = false;
                structure.showicon = false;
                if (m_temp.HasMember("highlight")) {
                    structure.highlight = m_temp.FindMember("highlight")->value.GetBool();
                }
                if (m_temp.HasMember("show_icon")) {
                    structure.showicon = m_temp.FindMember("show_icon")->value.GetBool();
                }
                
                switch (structure.type) {
                    case math::question::TEMPLATE:
                    {
                        structure.template_text.text = m_temp.FindMember("text")->value.GetString();
                        if (m_temp.HasMember("audios")) {
                            rapidjson::Value& voice_json = m_temp.FindMember("audios")->value;
                            for (rapidjson::SizeType i2 = 0; i2 < voice_json.Size(); i2++) {
                                math::resource::Voice voice;
                                voice.voice_id = voice_json[i2].FindMember("voice_id")->value.GetInt();
                                voice.paths.push_back(prefix + voice_json[i2].FindMember("path")->value.GetString());
                                structure.template_text.voices.push_back(voice);
                            }
                        } else {
                            math::resource::Voice voice;
                            voice.voice_id = 1;
                            //voice.paths = "";
                            structure.template_text.voices.push_back(voice);
                        }
                    }
                        break;
                    case math::question::KEYWORD:
                    {
                        for (rapidjson::SizeType i2 = 0; i2 < _keyword.Size(); i2++) {
                            rapidjson::Value& m_keyword = _keyword[i2];
                            if (order == m_keyword.FindMember("order")->value.GetInt()) {
                                rapidjson::Value& keyword_list = m_keyword.FindMember("keyword")->value;
                                for (rapidjson::SizeType i3 = 0; i3 < keyword_list.Size(); i3++) {
                                    math::question::Keyword keyword;
                                    keyword.value = keyword_list[i3].FindMember("value")->value.GetInt();
                                    keyword.text.text = keyword_list[i3].FindMember("text")->value.GetString();
                                    
                                    if (keyword_list[i3].HasMember("audios")) {
                                        rapidjson::Value& voice_json = keyword_list[i3].FindMember("audios")->value;
                                        for (rapidjson::SizeType i4 = 0; i4 < voice_json.Size(); i4++) {
                                            math::resource::Voice voice;
                                            rapidjson::Value& voice_id = voice_json[i4].FindMember("voice_id")->value;
                                            if (voice_id.IsInt()) {
                                                voice.voice_id = voice_json[i4].FindMember("voice_id")->value.GetInt();
                                            } else {
                                                voice.voice_id = 3;
                                            }
                                            voice.paths.push_back(prefix + voice_json[i4].FindMember("path")->value.GetString());
                                            keyword.text.voices.push_back(voice);
                                        }
                                    } else {
                                        math::resource::Voice voice;
                                        voice.voice_id = 1;
                                        //voice.path = "";
                                        keyword.text.voices.push_back(voice);
                                    }
                                    structure.keywords.push_back(keyword);
                                }
                                break;
                            }
                        }
                    }
                        break;
                    default:
                    {
                        for (rapidjson::SizeType i2=0; i2<_optional.Size(); i2++) {
                            rapidjson::Value& m_option = _optional[i2];
                            if (order == m_option.FindMember("order")->value.GetInt()) {
                                structure.optional.index = total_ques;
                                structure.optional.key = m_option.FindMember("type")->value.GetString();
                            }
                        }
                    }
                        break;
                }
                
                question_data.structures.push_back(structure);
            }
            
            question_data.ending = _question.FindMember("ending")->value.GetString();
            question_list.push_back(question_data);
            
            total_ques++;
        }
        
        index_loop ++;
        if (index_loop < loops.size()) {
            number_loop = loops[index_loop];
        } else {
            number_loop = 0;
        }
    }
    
    return question_list;
}


#pragma mark - animation
cocos2d::Action* math::animation::Helper::shake(float speed) {
    auto shake=cocos2d::Sequence::create(cocos2d::MoveBy::create(speed, cocos2d::Vec2(0, 8)), cocos2d::MoveBy::create(speed, cocos2d::Vec2(0, -8)), NULL);
    cocos2d::Action* action = cocos2d::RepeatForever::create(shake);
    return action;
}

cocos2d::Action* math::animation::Helper::fade() {
    float speed = 0.5;
    auto fade=cocos2d::Sequence::create(cocos2d::FadeTo::create(speed, 100), cocos2d::FadeIn::create(speed), cocos2d::DelayTime::create(speed), NULL);
    cocos2d::Action* action = cocos2d::RepeatForever::create(fade);
    return action;
}

cocos2d::Action* math::animation::Helper::scale() {
    float speed = 0.25;
    int repeat = 3;
    float delay = 0.5;
    auto scale=cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleBy::create(speed, 1.15), cocos2d::ScaleBy::create(speed, 1.0/1.15));
    cocos2d::Action* action=cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::Repeat::create(scale, repeat), cocos2d::DelayTime::create(delay), NULL));
    return action;
}

cocos2d::Action* math::animation::Helper::blink() {
    float speed = 0.35;
    int repeat = 3;
    float delay = 0.5;
    
    auto blink=cocos2d::Sequence::create(cocos2d::FadeOut::create(.0), cocos2d::DelayTime::create(speed), cocos2d::FadeIn::create(.0), cocos2d::DelayTime::create(speed), NULL);
    cocos2d::Action* action=cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::Repeat::create(blink, repeat), cocos2d::DelayTime::create(delay), NULL));
    return action;
}

std::vector<std::string> math::resource::Text::getVoices(int voice_id)
{
    for (Voice voice : voices) {
        if (voice.voice_id == voice_id) {
            return voice.paths;
        }
    }
    if (!voices.empty()) {
        return voices.front().paths;
    }
    std::vector<std::string> rets;
    rets.push_back(text);
    return rets;
}

std::vector<math::resource::Sync_Text> math::resource::Text::getSyncText(int voice_id)
{
	for (Voice voice : voices) {
		if (voice.voice_id == voice_id) {
			return voice.sync_texts;
		}
	}
	if (!voices.empty()) {
		return voices.front().sync_texts;
	}
	std::vector<math::resource::Sync_Text> abc;
	return abc;
}
