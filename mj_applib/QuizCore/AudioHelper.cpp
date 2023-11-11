//
//  AudioHelper.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 11/24/17.
//

#include "AudioHelper.h"

math::AudioHelper* _instance = nullptr;

math::AudioHelper* math::AudioHelper::getInstance() {
    
    if (_instance == nullptr) {
        _instance = new math::AudioHelper();
        AudioEngine::lazyInit();
    }
    return _instance;
}

void math::AudioHelper::setMusicVol(float value) {
    _music_vol = value;
}

float math::AudioHelper::getMusicVol() {
    return _music_vol;
}

void math::AudioHelper::setSoundVol(float value) {
    _sound_vol = value;
}

float math::AudioHelper::getSoundVol() {
    return _sound_vol;
}

#pragma mark - preload
void math::AudioHelper::preload(std::string audio) {
    AudioEngine::preload(audio);
}

void math::AudioHelper::preload_sequence (SequenceAudio* seq) {
    preload_next(seq);
}

void math::AudioHelper::preload_sequence(std::vector<std::string> &audios, audio_callback func) {
    auto seq = new SequenceAudio();
    seq->name = "preload";
    seq->func = func;
    seq->audios = audios;
    seq->index = -1;
    
    preload_sequence(seq);
}

void math::AudioHelper::preload_next(SequenceAudio* seq) {
    seq->index++;
    if (seq->index < seq->audios.size()) {
        AudioEngine::preload(seq->audios[seq->index], [=](bool success){
            if (success) {
                this->preload_next(seq);
            } else {
                CCLOG("Preload failure and skipped: %s", seq->audios[seq->index].c_str());
                this->preload_next(seq);
            }
        });
    } else {
        CCLOG("Preload done");
        if (seq->func != nullptr) {
            seq->func(seq->name);
        }
    }
}

#pragma mark - play
int math::AudioHelper::play(std::string audio) {
    return AudioEngine::play2d(audio, false, _sound_vol);
}

void math::AudioHelper::pause(int audio_id) {
    AudioEngine::pause(audio_id);
}

void math::AudioHelper::resume(int audio_id) {
    AudioEngine::resume(audio_id);
}

void math::AudioHelper::stop(int audio_id) {
    AudioEngine::stop(audio_id);
}

void math::AudioHelper::playMusic() {
    stopMusic();
    std::string music = cocos2d::StringUtils::format("bgmusic/bg_%d.mp3", cocos2d::random(1, 4));
    music_id = AudioEngine::play2d(music, true, _music_vol);
}

void math::AudioHelper::stopMusic() {
    if (music_id != -1) {
        stop(music_id);
        music_id = -1;
    }
}

void math::AudioHelper::pauseMusic() {
    if (music_id != -1) {
        stop(music_id);
        music_id = -1;
    }
}

void math::AudioHelper::resumeMusic() {
    if (music_id != -1) {
        stop(music_id);
        music_id = -1;
    }
}

#pragma mark - play sequence
void math::AudioHelper::pause_sequence(SequenceAudio* seq) {
    if (seq->current != -1) {
        pause(seq->current);
    }
}

void math::AudioHelper::resume_sequence(SequenceAudio* seq) {
    if (seq->current != -1) {
        resume(seq->current);
    }
}

void math::AudioHelper::stop_sequence(SequenceAudio* seq) {
    if (seq->current != -1) {
        stop(seq->current);
        seq->current = -1;
    }
}

void math::AudioHelper::play_sequence(SequenceAudio* seq) {
    seq->index = -1;
    seq->current = -1;
    play_next(seq);
}

void math::AudioHelper::play_sequence_wait(SequenceAudio* seq) {
    if (!playing_sequence) {
        playing_sequence = true;
        seq->index = -1;
        play_next(seq);
    } else {
        waits_list.push_back(seq);
    }
}

void math::AudioHelper::play_sequence(std::vector<std::string> audios, audio_callback func, std::string key) {
    auto seq = new SequenceAudio();
    seq->name = key;
    seq->func = func;
    seq->audios = audios;
    
    play_sequence(seq);
}

void math::AudioHelper::play_next(SequenceAudio* seq) {
    seq->index++;
    
    if (seq->index < seq->audios.size()) {
        seq->current = play(seq->audios[seq->index]);
        if (seq->current == -1) {
            play_next(seq);
        } else {
            AudioEngine::setFinishCallback(seq->current, [=](int audioID, const std::string& file) {
                this->play_next(seq);
            });
        }
    } else {
        CCLOG("Play Seq Audio done!");
        if (seq->func) {
            seq->func(seq->name);
        }
        
        playing_sequence = false;
        if (!waits_list.empty()) {
            auto next_seq = waits_list.front();
            waits_list.erase(waits_list.begin());
            play_sequence(next_seq);
        }
    }
}

#pragma mark - play sequence arsyn









