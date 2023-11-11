#ifndef SOUND_M_H
#define SOUND_M_H

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d;

class SoundM
{
private:
    static void playSound(std::string src, float volumn = 1.0f);
public:
    static void preload();
    static void playLetter(std::string s);
    static void playWord(std::string s);
    static void playTrayEnter();
    static void playTrayExit();
    static void playDragStart();
    static void playDrop();
    static void playShuffleMove();
    static void playInstruction();
    static int playBg();

};

#endif /* SOUND_M_H */
