#include "MyAudioPlayer.h"

USING_NS_CC;
using namespace experimental;

void audioPlayer::playPipeShowSound() {
	AudioEngine::play2d(this->pipeShowSound, false);
}

void audioPlayer::playPipeRunSound() {
	AudioEngine::play2d(this->pipeRunSound, false);
}

void audioPlayer::playCustomSound(std::string str) {
	if (this->_isPlaySoundStr)
	{
		auto id = AudioEngine::play2d(str, false);
		this->_isPlaySoundStr = false;
		AudioEngine::setFinishCallback(id, [this](int, std::string) {
			this->_isPlaySoundStr = true;
			});
	}
}

void audioPlayer::playBgSound() {	
	int id = AudioEngine::play2d(this->bgSound, true);
    AudioEngine::setVolume(id, 0.5);
    
}

void audioPlayer::playDragSound() {

	if (this->_isPlaySoundDrag)
	{
		auto id = AudioEngine::play2d(this->dragSound, false);
		this->_isPlaySoundDrag = false;
		AudioEngine::setFinishCallback(id, [this](int, std::string) {
			this->_isPlaySoundDrag = true;
			});
	}
}

void audioPlayer::playDropRightSound() {	
	AudioEngine::play2d(this->dropRightSound, false);
}

void audioPlayer::playHuzzSound() {
	AudioEngine::play2d(this->huzzSound, false);
}

void audioPlayer::playSlideSound() {
	AudioEngine::play2d(this->slideSound, false);
}

void audioPlayer::playWinSound() {
	AudioEngine::play2d(this->winSound, false);
}

void audioPlayer::playWrongSound() {
	AudioEngine::play2d(this->wrongSound, false);
}
