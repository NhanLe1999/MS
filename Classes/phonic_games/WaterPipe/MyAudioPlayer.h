#include "AudioEngine.h"
#include <string>

class audioPlayer {
public:
	void playCustomSound(std::string str);
	void playBgSound();
	void playDragSound();
	void playDropRightSound();
	void playHuzzSound();
	void playSlideSound();
	void playWinSound();
	void playWrongSound();
	void playPipeRunSound();
	void playPipeShowSound();

private:
	/*
	const char* bgSound = "../Resources/bg_music.mp3";
	const char* dragSound = "../Resources/drag.mp3";
	const char* dropRightSound = "../Resources/drop_correct.mp3";
	const char* huzzSound = "../Resources/huzz.mp3";
	const char* slideSound = "../Resources/slide.mp3";
	const char* winSound = "../Resources/win.mp3";
	const char* wrongSound = "../Resources/wrong.mp3";
	*/
	const char* bgSound = "sounds/waterpipe/bg_music.mp3";
	const char* dragSound = "sounds/waterpipe/drag.mp3";
	const char* dropRightSound = "sounds/waterpipe/drop_correct.mp3";
	const char* huzzSound = "sounds/waterpipe/huzz.mp3";
	const char* slideSound = "sounds/waterpipe/slide.mp3";
	const char* winSound = "sounds/waterpipe/win.mp3";
	const char* wrongSound = "sounds/waterpipe/wrong.mp3";
	const char* pipeRunSound = "sounds/waterpipe/pipe_run.mp3";
	const char* pipeShowSound = "sounds/waterpipe/pipe_pop_up2.mp3";
	bool _isPlaySoundStr = true;
	bool _isPlaySoundDrag = true;
};