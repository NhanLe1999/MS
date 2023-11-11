//
//  MJ_PageLoadResource.cpp
//  M
//
//  Created by To Ngoc Linh on 5/12/17.
//
//

#include "MJ_PageLoadResource.h"
#include "cocos2d.h"
#include "MJ_AnimationCache.h"
#include "HSLib.h"
#include "StoryDataModel.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "MacCppBridge.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
namespace mj {

#define FILE_UTILS cocos2d::FileUtils::getInstance()
#define DIRECTOR cocos2d::Director::getInstance()
#define PAGELOAD mj::PageLoadResource::getInstance()

    PageLoadResource::PageLoadResource() {
        animations.clear();
        audios.clear();
        images.clear();
    }
    
    void PageLoadResource::deleteCacheAllStory(){
        StoryDataModel::getInstance()->deleteAllDownloadStory();
        auto path_mjstory = FILE_UTILS->getWritablePath() + std::string("mjstory/");
        FILE_UTILS->removeDirectory(path_mjstory);//xoá thư mục /mjstory và tất cả source ở bên trong.
        //tạo lại thư mục /mjstory
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        MAC_CPP_BRIDGE->createMacPathFileByString(path_mjstory);
#else
        if(!FILE_UTILS->isDirectoryExist(path_mjstory)) {
            FILE_UTILS->createDirectory(path_mjstory);
        }
#endif
    }
    
    void PageLoadResource::deleteCacheAllAudiobooks() {
        auto path_audiobook = FILE_UTILS->getWritablePath() + "audiobooks/";
        FILE_UTILS->removeDirectory(path_audiobook);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        MAC_CPP_BRIDGE->createMacPathFileByString(path_audiobook);
        MAC_CPP_BRIDGE->createMacPathFileByString(path_audiobook + "/audio");
        MAC_CPP_BRIDGE->createMacPathFileByString(path_audiobook + "/content");
#else
        
        if (!FILE_UTILS->isDirectoryExist(path_audiobook)) {
            FILE_UTILS->createDirectory(path_audiobook);
            FILE_UTILS->createDirectory(path_audiobook + "/audio");
            FILE_UTILS->createDirectory(path_audiobook + "/content");
        }
#endif
    }
    
    void PageLoadResource::deleteCacheAllLessons() {
        auto path_lessons = FILE_UTILS->getWritablePath() + "cached/";
        FILE_UTILS->removeDirectory(path_lessons);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        MAC_CPP_BRIDGE->createMacPathFileByString(path_lessons);
        MAC_CPP_BRIDGE->createMacPathFileByString(path_lessons + "/zip");
#else
        
        if (!FILE_UTILS->isDirectoryExist(path_lessons)) {
            FILE_UTILS->createDirectory(path_lessons);
            FILE_UTILS->createDirectory(path_lessons + "/zip");
        }
#endif
    }

    void PageLoadResource::deleteCacheOfStory(int _id, std::string file_zip) {
        ValueMap _storyInfo = StoryDataModel::getInstance()->getStoryWithId(_id);
        parseStory(_storyInfo["story_id"].asInt(), _storyInfo["lang_id"].asInt(), _storyInfo["total_page"].asInt());
    
        // TODO: delete resource in vector images, audios and animations
        for (auto path_file : PAGELOAD.audios) {
            CCLOG("delete audios: %s", path_file.c_str());
            FILE_UTILS->removeFile(XHSLib->getFilePathStory(path_file));
        }
        
        auto list_id = StoryDataModel::getInstance()->getListStoriesIdWithStoryId(_storyInfo["story_id"].asInt());
        auto check_delete = true;
        if ((int) list_id.size() > 1) {
            auto check_count = 0;
            for (int i = 0; i < (int) list_id.size(); i++) {
                if (XHSLib->isFavoritesStory(list_id[i].asInt())) {
                    check_count++;
                }
                if (check_count == 2) {
                    break;
                }
            }
            if (check_count < 2) {
                check_delete = false;
            }
        }
        
        if (check_delete) {//xoá tiếp image, gaf.
            for (auto path_file : PAGELOAD.images) {
                CCLOG("delete images: %s", path_file.c_str());
                FILE_UTILS->removeFile(XHSLib->getFilePathStory(path_file));
            }
            
            for (auto gaf_path : PAGELOAD.animations) {
                CCLOG("gaf: %s", gaf_path.c_str());

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                auto paths = mj::helper::split(gaf_path, '/');
                
                auto listpos = NativeBridge::findStringOnString(gaf_path, "/", false);
                if ((int)listpos.size() > 1) {
                    auto folder_name = NativeBridge::hsStringSub(gaf_path, 0, listpos[1]);
                    CCLOG("delete gaf = %s", folder_name.c_str());
                    FILE_UTILS->removeDirectory(XHSLib->getFilePathStory(folder_name));
                }
//#endif
            }
        }
        StoryDataModel::getInstance()->deleteDownloadStory(_id);
        releaseAllResource();
        FILE_UTILS->removeFile(file_zip);
    }

    void PageLoadResource::parseStory(int story_id, int lang_id, int total_page) {
        for (auto i = 0; i <= total_page; i++) {
            auto plist_name = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.plist", story_id, lang_id, i));
            if (FILE_UTILS->isFileExist(plist_name)) {
                PAGELOAD.parsePagePlist(plist_name);
                FILE_UTILS->removeFile(plist_name);
            }else {
                plist_name = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.json", story_id, lang_id, i));
                if (FILE_UTILS->isFileExist(plist_name)) {
                    PAGELOAD.parsePageJson(plist_name);
                    FILE_UTILS->removeFile(plist_name);
                }
            }
        }

        auto game_plist = StringUtils::format("%d_%d_game.plist", story_id, lang_id);
        if (FILE_UTILS->isFileExist(game_plist)) {
            PAGELOAD.parseGamePlist(game_plist);
            FILE_UTILS->removeFile(game_plist);
        }

        auto learn_plist = StringUtils::format("%d_%d_learning.plist", story_id, lang_id);
        if (FILE_UTILS->isFileExist(learn_plist)) {
            PAGELOAD.parseLearnPlist(learn_plist);
            FILE_UTILS->removeFile(learn_plist);
        }
    }

#define VM_SAFE_CHECK(a, b, c) !a[b].isNull() && a[b].getType() == c

    void PageLoadResource::releaseAllResource() {
        // TODO: remove resources in texture caches, animations cache and audio pool

        PAGELOAD.images.clear();
        PAGELOAD.audios.clear();
        PAGELOAD.animations.clear();
    }

    void PageLoadResource::parseLearnPlist(std::string name) {
        name = FILE_UTILS->fullPathForFilename(name);
        assert(FILE_UTILS->isFileExist(name) && "File must exist");
        auto value_map = FILE_UTILS->getValueMapFromFile(name);

        if (VM_SAFE_CHECK(value_map, "learn", Value::Type::MAP)) {
            auto learn = value_map["learn"].asValueMap();

            //audio_path
            if (VM_SAFE_CHECK(learn, "path_audio", Value::Type::STRING)) {
                audios.push_back(learn["path_audio"].asString());
            }

            //text objects
                if (VM_SAFE_CHECK(learn, "text_info", Value::Type::VECTOR)) {
                auto texts = learn["text_info"].asValueVector();

                for (auto it : texts) {
                    if (it.getType() == Value::Type::MAP) {
                        auto text = it.asValueMap();
                        if (VM_SAFE_CHECK(text, "audio_path", Value::Type::STRING)) {
                            audios.push_back(text["audio_path"].asString());
                        }
                    }
                }
            }
        }
    }

    void PageLoadResource::parseGamePlist(std::string name) {
        name = FILE_UTILS->fullPathForFilename(name);
        assert(FILE_UTILS->isFileExist(name) && "File must exist");
        auto value_map = FILE_UTILS->getValueMapFromFile(name);

        if (VM_SAFE_CHECK(value_map, "text_info", Value::Type::VECTOR)) {
            auto texts = value_map["text_info"].asValueVector();
            for (auto it : texts) {
                if (it.getType() == Value::Type::MAP) {
                    auto text = it.asValueMap();
                    if (VM_SAFE_CHECK(text, "audio_path", Value::Type::STRING)) {
                        audios.push_back(text["audio_path"].asString());
                    }
                }
            }
        }

        if (VM_SAFE_CHECK(value_map, "game", Value::Type::MAP)) {
            auto game_info = value_map["game"].asValueMap();
            if (VM_SAFE_CHECK(game_info, "audio_title", Value::Type::STRING)) {
                audios.push_back(game_info["audio_title"].asString());
            }

            if (VM_SAFE_CHECK(game_info, "audio_intro", Value::Type::VECTOR)) {
                auto intros = game_info["audio_intro"].asValueVector();
                for (auto it : intros) {
                    if (it.getType() != Value::Type::STRING) {
                        continue;
                    }
                    audios.push_back(it.asString());
                }
            }
        }

        if (VM_SAFE_CHECK(value_map, "sentence", Value::Type::VECTOR)) {
            auto sentences = value_map["sentence"].asValueVector();
            for (auto it : sentences) {
                if (it.getType() != Value::Type::MAP) {
                    continue;
                }

                auto sentence = it.asValueMap();
                if (VM_SAFE_CHECK(sentence, "image", Value::Type::STRING)) {
                    images.push_back(sentence["image"].asString());
                }

                if (VM_SAFE_CHECK(sentence, "audio_nature", Value::Type::MAP)) {
                    auto audio_info = sentence["audio_nature"].asValueMap();
                    if (VM_SAFE_CHECK(audio_info, "path", Value::Type::STRING)) {
                        audios.push_back(audio_info["path"].asString());
                    }
                }

                if (VM_SAFE_CHECK(sentence, "audio_slow", Value::Type::MAP)) {
                    auto audio_info = sentence["audio_slow"].asValueMap();
                    if (VM_SAFE_CHECK(audio_info, "path", Value::Type::STRING)) {
                        audios.push_back(audio_info["path"].asString());
                    }
                }
            }
        }

        if (VM_SAFE_CHECK(value_map, "question", Value::Type::VECTOR)) {
            auto questions = value_map["question"].asValueVector();
            for (auto it : questions) {
                if (it.getType() != Value::Type::MAP) {
                    continue;
                }

                auto question = it.asValueMap();
                if (VM_SAFE_CHECK(question, "image_ans_wm", Value::Type::STRING)) {
                    images.push_back(question["image_ans_wm"].asString());
                }

                if (VM_SAFE_CHECK(question, "audio_ans_wm", Value::Type::STRING)) {
                    audios.push_back(question["audio_ans_wm"].asString());
                }

                if (VM_SAFE_CHECK(question, "audio", Value::Type::MAP)) {
                    auto audio_info = question["audio"].asValueMap();
                    if (VM_SAFE_CHECK(audio_info, "path", Value::Type::STRING)) {
                        audios.push_back(audio_info["path"].asString());
                    }
                }

                if (VM_SAFE_CHECK(question, "answer", Value::Type::VECTOR)) {
                    auto answers = question["answer"].asValueVector();
                    for (auto ans_it : answers) {
                        if (ans_it.getType() != Value::Type::MAP) {
                            continue;
                        }

                        auto answer = ans_it.asValueMap();
                        if (VM_SAFE_CHECK(answer, "audio", Value::Type::MAP)) {
                            auto audio_info = answer["audio"].asValueMap();
                            if (VM_SAFE_CHECK(audio_info, "path", Value::Type::STRING)) {
                                audios.push_back(audio_info["path"].asString());
                            }
                        }
                    }
                }
            }
        }
    }

    void PageLoadResource::parsePageJson(std::string name){
        name = FILE_UTILS->fullPathForFilename(name);
        auto buffer_string = FileUtils::getInstance()->getStringFromFile(name);
        rapidjson::Document value_data;
        
        value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
        if (value_data.HasParseError()) {
            CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
            return;
        }
        // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//        rapidjson::StringBuffer rj_buffer;
//        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
//        value_data.Accept(writer);
//        std::cout << "JSON: " << rj_buffer.GetString() << std::endl;
#endif
        
        
        if (value_data.HasMember("image") && value_data["image"].IsArray()) {
            auto value_image = value_data["image"].GetArray();
            for (int i = 0; i < (int) value_image.Size(); i++) {
                auto image_item = value_image[i].GetObject();
                auto path = image_item["path"].GetString();
                if (path != std::string("")) {
                    if (image_item["type"].GetString() == std::string("animation")) {
                        animations.push_back(path);
                    }else if (image_item["type"].GetString() == std::string("image")) {
                        images.push_back(path);
                    }
                }
                
                auto effect = image_item["effect"].GetString();
                if (effect != std::string("")) {
                    audios.push_back(effect);
                }
                
                if (image_item.HasMember("audio") && image_item["audio"].IsObject()) {
                    auto value_audio = image_item["audio"].GetObject();
                    auto path = value_audio["path"].GetString();
                    if (path != std::string("")) {
                        audios.push_back(path);
                    }
                }
                
                if (image_item.HasMember("sequence") && image_item["sequence"].IsArray()) {
                    auto list_sequence = image_item["sequence"].GetArray();
                    for (int j = 0; j < (int) list_sequence.Size(); j++) {
                        auto effect_sequence = list_sequence[j].GetObject()["effect"].GetString();
                        if (effect_sequence != std::string("")) {
                            audios.push_back(effect_sequence);
                        }
                        
                    }
                }
            }
            
            auto bg_path= value_data["bg_img"].GetObject()["path"].GetString();
            if (bg_path != std::string("")) {
                images.push_back(bg_path);
            }
            
            if (value_data.HasMember("audio") && value_data["audio"].IsArray()) {
                auto list_audio = value_data["audio"].GetArray();
                for (int i = 0; i < (int)list_audio.Size(); i++) {
                    auto path = list_audio[i].GetObject()["path"].GetString();
                    if (path != std::string("")) {
                        audios.push_back(path);
                    }
                }
            }
        }
    }
    
    void PageLoadResource::parsePagePlist(std::string name) {
        name = FILE_UTILS->fullPathForFilename(name);
        CCLOG("File: %s", name.c_str());
        auto value_map = FILE_UTILS->getValueMapFromFile(name);

        // background
        
        if (!value_map["bg"].isNull() && value_map["bg"].getType() == Value::Type::MAP) {
            string background = value_map["bg"].asValueMap()["name"].asString();
            images.push_back(background);
        }
        

        //textstory
        if (!value_map["textstory"].isNull() && value_map["textstory"].getType() == Value::Type::VECTOR) {
            auto textstory = value_map["textstory"].asValueVector();
            for (auto bt_iterator : textstory) {
                auto boxtext = bt_iterator.asValueVector();
                for (auto text_iterator : boxtext) {
                    auto text_audio = text_iterator.asValueMap()["bgsoundname"].asString();
                    if (text_audio != "")
                        audios.push_back(text_audio);
                }
            }
        }

        
        //character
        if (!value_map["nhanvat"].isNull() && value_map["nhanvat"].getType() == Value::Type::VECTOR) {
            auto character_vector = value_map["nhanvat"].asValueVector();
            for (auto char_iterator : character_vector) {
                auto character = char_iterator.asValueMap();
                
                auto is_gaf = character["isgaf"].asBool();
                
                auto name = character["name"].asString();
                auto sound_effect = character["sound_effect"].asString();
                if (is_gaf) {
                    if (name != "")
                        animations.push_back(name);
                } else {
                    if (name != "")
                        images.push_back(name);
                }
                
                if (sound_effect != "") {
                    audios.push_back(sound_effect);
                }
                
                auto text_value = character["textnv"];
                if (text_value.getType() != cocos2d::Value::Type::VECTOR) {
                    continue;
                }
                auto text_vector = text_value.asValueVector();
                for (auto text_iterator : text_vector) {
                    auto text = text_iterator.asValueMap();
                    auto text_audio = text["audio"].asString();
                    if (text_audio != "")
                        audios.push_back(text_audio);
                }
            }
        }
    }
    
    void PageLoadResource::deleteCacheOfAudiobook(int _id) {
        auto info = StoryDataModel::getInstance()->getAudiobook(_id);
        auto sync_path = FileUtils::getInstance()->getWritablePath() + info.sync_path;
        auto cached_audio = FileUtils::getInstance()->getWritablePath() + "audiobooks/audio/" + mj::helper::getFilenameFromURL(info.audio_url);
        
        FileUtils::getInstance()->removeFile(cached_audio);
        FileUtils::getInstance()->removeFile(sync_path);
    }
    
    
}
