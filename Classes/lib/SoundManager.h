#pragma once
#include "BaseSingleton.h"

#define SOUND_MANAGER SoundManager::GetInstance()

class SoundManager : public BaseSingleton<SoundManager>
{
public:
    
    void init();
    
    void playProgressBarEffect();

    void playClickedEffect(std::function<void()> callback = nullptr);
    
    void playCloseWindowsEffect(std::function<void()> callback = nullptr);

    void playTapDigitsEffect();

    void playStartEffect();

    void playAppearEffect();

    void playAppearHiddenMenuEffect();
    
    void playDisappearEffect();

    void playChooseEffect();

    void pauseAllBackgroundMusics();
    
    void pauseAllSoundEffects();
    
    void resumeAllBackgroundMusics();
    
    void resumeAllSoundEffects();
    
    void stopAllBackgroundMusics();
    
    void stopAllSoundEffects();
    
    void preloadSoundEffectAndMusic();
    
    bool isSoundEffectEnable();
    
    bool isMusicEffectEnable();
};
