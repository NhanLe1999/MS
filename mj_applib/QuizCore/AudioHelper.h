//
//  AudioHelper.h
//  MJQuiz
//
//  Created by HuyPhuong on 11/24/17.
//

#ifndef AudioHelper_h
#define AudioHelper_h

#include <stdio.h>
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

#define AudioEngine cocos2d::experimental::AudioEngine
#define audio_helper math::AudioHelper::getInstance()

namespace math {
    typedef std::function<void(std::string)> audio_callback;
    
    class SequenceAudio {
    public:
        std::string name = "";
        audio_callback func = nullptr;
        std::vector<std::string> audios;
        int current = -1;
        int index = 0;
        bool enable = false;
        
        SequenceAudio(){};
        ~SequenceAudio(){};
        
    };
    
    class AudioHelper {
    public:
        AudioHelper(){};
        ~AudioHelper(){};
        
        static AudioHelper* getInstance();
        
        void setMusicVol(float value);
        float getMusicVol();
        void setSoundVol(float value);
        float getSoundVol();
        
        //basic methods
        void preload(std::string audio);
        int play(std::string audio);
        void pause(int audio_id);
        void resume(int audio_id);
        void stop(int audio_id);
        
        //background music
        void playMusic();
        void stopMusic();
        void pauseMusic();
        void resumeMusic();
        
        //play sequence list of audio
        void play_sequence(SequenceAudio* seq);
        void pause_sequence(SequenceAudio* seq);
        void resume_sequence(SequenceAudio* seq);
        void stop_sequence(SequenceAudio* seq);
        /*
        void pause_sequence(std::string name);
        void resume_sequence(std::string name);
        void stop_sequence(std::string name);
         */
        
        //Play sequence
        void play_sequence(std::vector<std::string> audios, audio_callback func, std::string key);
        
        void play_sequence(SequenceAudio* seq, std::string audio) {
            seq->audios.push_back(audio);
            play_sequence(seq);
        }
        template<typename ...T>
        void play_sequence(SequenceAudio* seq, std::string audio, T... args) {
            seq->audios.push_back(audio);
            play_sequence(seq, args...);
        }
        
        //Play sequence
        void play_sequence_wait(SequenceAudio* seq);
        void play_sequence_wait(SequenceAudio* seq, std::string audio) {
            seq->audios.push_back(audio);
            play_sequence_wait(seq);
        }
        template<typename ...T>
        void play_sequence_wait(SequenceAudio* seq, std::string audio, T... args) {
            seq->audios.push_back(audio);
            play_sequence_wait(seq, args...);
        }
        
        //preload sequence list of audio
        void preload_sequence(SequenceAudio* seq);
        void preload_sequence(std::vector<std::string> &audios, audio_callback func);
        
    private:
        void play_next(SequenceAudio* seq);
        void preload_next(SequenceAudio* seq);
        int music_id = -1;
        bool playing_sequence = false;
        
        float _music_vol = 1.0;
        float _sound_vol = 1.0;
        
        std::vector<SequenceAudio*> waits_list;
        std::vector<SequenceAudio*> async_list;
    };
}

#endif /* AudioHelper_h */
