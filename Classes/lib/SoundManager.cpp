#include "SoundManager.h"
#include "HSAudioEngine.h"

USING_NS_CC;

void SoundManager::init()
{
}

void SoundManager::playProgressBarEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/scroll_progress_bar.mp3");
    }
}

void SoundManager::playClickedEffect(std::function<void()> callback)
{
	if (isSoundEffectEnable())  {
        
      int id =   AUDIO_ENGINE->playEffect("sounds/clicked_effect.mp3", false);
        
      cocos2d::experimental::AudioEngine::setFinishCallback(id,[callback](int,std::string){
            
          if(callback) {
              callback();
          }
      });
    } else  {
        if(callback) {
            callback();
        }
    }
}

void SoundManager::playCloseWindowsEffect(std::function<void()> callback)
{
    if (isSoundEffectEnable())  {
        
      int id =  cocos2d::experimental::AudioEngine::play2d("sounds/close_window.mp3", false);
        
      cocos2d::experimental::AudioEngine::setFinishCallback(id,[callback](int,std::string){
            
          if(callback) {
              callback();
          }
      });
    } else  {
        if(callback) {
            callback();
        }
    }
}

void SoundManager::playTapDigitsEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/tap_digits.mp3", false);
    }
}

void SoundManager::playStartEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/start.mp3", false);
    }
}

void SoundManager::playAppearEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/appear.mp3", false);
    }
}

void SoundManager::playAppearHiddenMenuEffect()
{
    if (isSoundEffectEnable()) {
        AUDIO_ENGINE->playEffect("sounds/show_hidden_menu.mp3", false);
    }
}

void SoundManager::playDisappearEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/disappear.mp3", false);
    }
}

void SoundManager::playChooseEffect()
{
    if (isSoundEffectEnable())  {
        AUDIO_ENGINE->playEffect("sounds/choose_interest.mp3", false);
    }
}

void SoundManager::preloadSoundEffectAndMusic()
{
    cocos2d::experimental::AudioEngine::preload("sounds/tap_digits.mp3");
    cocos2d::experimental::AudioEngine::preload("sounds/start.mp3");
    cocos2d::experimental::AudioEngine::preload("sounds/appear.mp3");
    cocos2d::experimental::AudioEngine::preload("sounds/disappear.mp3");
    cocos2d::experimental::AudioEngine::preload("sounds/choose_interest.mp3");
    cocos2d::experimental::AudioEngine::preload("sounds/clicked_effect.mp3");
}

void SoundManager::pauseAllBackgroundMusics()
{
   
}

void SoundManager::pauseAllSoundEffects()
{
    cocos2d::experimental::AudioEngine::pauseAll();
}

void SoundManager::resumeAllBackgroundMusics()
{
    cocos2d::experimental::AudioEngine::resumeAll();
}

void SoundManager::resumeAllSoundEffects()
{
}

void SoundManager::stopAllBackgroundMusics()
{
    cocos2d::experimental::AudioEngine::stopAll();
}

void SoundManager::stopAllSoundEffects()
{
    cocos2d::experimental::AudioEngine::stopAll();
}

bool SoundManager::isSoundEffectEnable()
{
	return true;
}

bool SoundManager::isMusicEffectEnable()
{
	return true;
}
