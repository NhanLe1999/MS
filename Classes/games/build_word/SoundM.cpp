#include "SoundM.h"
#include "MJDefault.h"

void SoundM::preload() {
}

void SoundM::playLetter(std::string s) {
    playSound(s);
}

void SoundM::playWord(std::string s) {
    playSound(s);
}

void SoundM::playSound(std::string src, float volumn ) {
    int id = cocos2d::experimental::AudioEngine::play2d(src);
    cocos2d::experimental::AudioEngine::setVolume(id, volumn);
}

void SoundM::playTrayExit() {
    playSound("sounds/build_word/tray_exit.mp3");
}

void SoundM::playTrayEnter() {
    playSound("sounds/build_word/tray_enter.mp3");
}

void SoundM::playDragStart() {
    playSound("sounds/build_word/drag_start.mp3");
}

void SoundM::playDrop() {
    playSound("sounds/build_word/drop.wav", 0.7f);
}

void SoundM::playShuffleMove() {
    playSound("sounds/build_word/shuffle_move.mp3");
}

#include "MJDefinitions.h"
void SoundM::playInstruction() {
	if (/*is_phonic*//*ms::GradeManager::getInstance().getScope() == ms::GradeManager::Scope::LESSONS_PHONIC*/ MJDEFAULT->getBoolForKey(key_is_phonic_lesson , false)) {
		playSound("sounds/build_word/instruction1.mp3");
	}
	else {
		playSound("sounds/build_word/instruction.mp3");
	}
   
}

int SoundM::playBg() {
    auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/build_word/music_bg.mp3");
    cocos2d::experimental::AudioEngine::setLoop(audio_id, true);
    cocos2d::experimental::AudioEngine::setVolume(audio_id, 0.3);
    return audio_id;
}
