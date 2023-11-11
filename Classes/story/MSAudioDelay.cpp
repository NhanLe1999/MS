//
//  MSAudioDelay.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 10/11/18.
//

#include "MSAudioDelay.h"
#include "AudioEngine.h"
#include "../external/json/document.h"

USING_NS_CC;

namespace mj { namespace helper {
    void AudioDelay::preload(int story_id, int lang_id, std::function<void()> callback) {
        std::vector<std::string> list_audio;
        auto page_id = 1;
        do {
            auto page_filename = StringUtils::format("%d_%d_%d.json", story_id, lang_id, page_id);
            if (!FileUtils::getInstance()->isFileExist(page_filename)) {
                break;
            }
            
            auto json = FileUtils::getInstance()->getStringFromFile(page_filename);
            
            rapidjson::Document d;
            
            d.Parse(json.c_str());
            
            if (d.IsNull() || d.HasParseError() ||  !d.HasMember("audio")) {
                return;
            }
            
            auto audio = d["audio"].GetArray();
            for (auto i = 0; i < audio.Size(); ++i) {
                auto single_audio = audio[i].GetObject();
                auto audio_path = std::string(single_audio["path"].GetString());
                
                list_audio.push_back(audio_path);
            }
            
            page_id++;
        } while (true);
        preload(list_audio, callback);
    }
    
    void AudioDelay::preload(std::vector<std::string> files, std::function<void()> callback) {
        if (files.empty()) {
            callback();
            return;
        }
        auto file_to_preload = files.front();
        files.erase(files.begin());
        cocos2d::experimental::AudioEngine::preload(file_to_preload, [files, callback](bool) {
            preload(files, callback);
        });
    }
    
    int AudioDelay::play(const std::string &path, const std::vector<std::pair<float, float>> & delay) {
        auto job = new AudioDelay(path, delay);
        return job->_audio_id;
    }
    
    AudioDelay::AudioDelay(const std::string & path, const std::vector<std::pair<float, float>> & delays) {
        _job_id = cocos2d::random(10000000, 99999999);
        _delays = delays;
        for (auto &cf : _delays) {
            cf.first = cf.first / 1000.f;
            cf.second = cf.second / 1000.f;
        }
        _path = path;
        this->play();
    }
    
    void AudioDelay::play() {
        _audio_id = cocos2d::experimental::AudioEngine::play2d(_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_audio_id, [this](int, std::string) {
            delete this;
        });
        
        auto timestamp = 0.f;
        for (auto i = 0; i < (int)_delays.size(); ++i) {
            auto d = _delays[i];
            timestamp += d.first;
            auto duration = d.second;
            
            cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
                auto state = cocos2d::experimental::AudioEngine::getState(_audio_id);
                
                if (state == cocos2d::experimental::AudioEngine::AudioState::ERROR) {
                    delete this;
                    return;
                }
                cocos2d::experimental::AudioEngine::pause(_audio_id);
            }, cocos2d::Application::getInstance(), 0, 0, timestamp, false, cocos2d::StringUtils::format("job_delay_%d_%d_%d", _job_id, _audio_id, i));
            
            timestamp += duration;
            cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
                auto state = cocos2d::experimental::AudioEngine::getState(_audio_id);
                if (state == cocos2d::experimental::AudioEngine::AudioState::ERROR) {
                    delete this;
                    return;
                }
                cocos2d::experimental::AudioEngine::resume(_audio_id);
            }, cocos2d::Application::getInstance(), 0, 0, timestamp, false, cocos2d::StringUtils::format("job_resume_%d_%d_%d", _job_id, _audio_id, i));
            
        }
    }
    
    AudioDelay::~AudioDelay() {
        for (auto i = 0; i < (int)_delays.size(); ++i) {
            cocos2d::Director::getInstance()->getScheduler()->unschedule(cocos2d::StringUtils::format("job_delay_%d_%d_%d", _job_id, _audio_id, i), cocos2d::Application::getInstance());
            cocos2d::Director::getInstance()->getScheduler()->unschedule(cocos2d::StringUtils::format("job_resume_%d_%d_%d", _job_id, _audio_id, i), cocos2d::Application::getInstance());
        }
    }
}}
