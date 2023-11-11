#ifndef __HSAudioEngine__
#define __HSAudioEngine__
#define AUDIO_ENGINE HSAudioEngine::getInstance()

#include <stdio.h>
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class HSAudioEngine {
public:
    static HSAudioEngine * getInstance();
    static HSAudioEngine * __instance;
    static void resetAudioEngine();
    static void resetAudioSecsson();
    int playEffect(std::string path_audio, bool loop = false,
                   float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
    void stopAllEffects();
    void stopEffect(int index_audio);
    void pauseEffects(int index_audio);
    void resumEffects(int index_audio);
    void preloadEffect(std::string pathfile);
    
    void playBackgroundMusic(std::string path_backgroundmusic, float volume = 0.2f);
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void stopBackgroundMusic();

    void playClickedEffect(std::function<void()> callback = nullptr);
    
    void pauseAllAudio();
    void resumAllAudio();
    void stopAllAudio();
    //advance methods
    int play(std::string audio, bool loop = false, float volume = 1, std::function<void()> finishCallback = nullptr);
    int playSound(std::string path, bool loop = false, std::function<void()> finishCallback = nullptr, float volume = 1.0f);
    void playChain(std::vector<std::string> paths, std::function<void()> finishCallback = nullptr, float volume = 1.0f, int startIndex = 0);
    void stopSound(std::string path);
	void stopAllSound();
    unsigned int getDuration(int audio_id);
     std::map<std::string, int> m_soundMap;
    //end advance methods
    
    std::vector<int> _listIDAudio;
    
    int index_bgmusic = -1;
    static bool isIOS;
    static bool _needResetAudioEngine;
};

#endif
