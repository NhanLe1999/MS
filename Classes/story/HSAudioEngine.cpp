#include "HSAudioEngine.h"
//#include "SimpleAudioEngine.h"

bool HSAudioEngine::isIOS = false;
bool HSAudioEngine::_needResetAudioEngine = false;

HSAudioEngine * HSAudioEngine::__instance = NULL;

HSAudioEngine * HSAudioEngine::getInstance()
{
    if(_needResetAudioEngine)
    {
        resetAudioSecsson();
        _needResetAudioEngine = false;
    }
        
    if (!__instance)
    {
        __instance = new HSAudioEngine();
        
        isIOS = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        isIOS = false;
#endif
        if (!isIOS)
        {
            cocos2d::experimental::AudioEngine::end();
            cocos2d::experimental::AudioEngine::lazyInit();
        }
    }
    
    return __instance;
}
void HSAudioEngine::resetAudioSecsson(){
    cocos2d::experimental::AudioEngine::uncacheAll();
    CC_SAFE_DELETE(__instance);
}
void HSAudioEngine::resetAudioEngine() {
    _needResetAudioEngine = true;
}
int HSAudioEngine::playEffect(std::string path_audio, bool loop, float pitch, float pan, float gain)
{
    auto audio_id = -1;
    
    if (path_audio == "")
    {
        return audio_id;
    }
   
    auto check_path = path_audio[path_audio.length()-1] == '/';
    
    if (!cocos2d::FileUtils::getInstance()->isFileExist(path_audio) || check_path)
    {
        return audio_id;
    }
//    if (isIOS) {
//        audio_id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(path_audio.c_str(), loop, pitch, pan, gain);
//    }else{
        audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio, loop, gain);
//        cocos2d::experimental::AudioEngine::setLoop(audio_id, loop);
        
//    }
    return audio_id;
}

void HSAudioEngine::preloadEffect(std::string pathfile){
    if (pathfile == "") {
        return;
    }
    auto check_path = pathfile[pathfile.length()-1] == '/';
    if (!cocos2d::FileUtils::getInstance()->isFileExist(pathfile) || check_path)
    {
        return;
    }

//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(pathfile.c_str());
//
//    }else
        cocos2d::experimental::AudioEngine::preload(pathfile);
}

void HSAudioEngine::stopAllEffects(){
//    CCLOG("stop_all_effect");
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
//    }else
        cocos2d::experimental::AudioEngine::stopAll();
    
    //_listIDAudio.clear();
}

void HSAudioEngine::stopEffect(int index_audio){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(index_audio);
//    }else{
        cocos2d::experimental::AudioEngine::stop(index_audio);
//    }
    for (int i = 0; i < (int)_listIDAudio.size(); i++) {
        if (_listIDAudio[i] == index_audio) {
            _listIDAudio.erase(_listIDAudio.begin() + i);
            break;
        }
    }
}

void HSAudioEngine::pauseEffects(int index_audio){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(index_audio);
//    }else
        cocos2d::experimental::AudioEngine::pause(index_audio);
    
}

void HSAudioEngine::resumEffects(int index_audio){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(index_audio);
//    }else
        cocos2d::experimental::AudioEngine::resume(index_audio);
    
}


void HSAudioEngine::playBackgroundMusic(std::string path_backgroundmusic, float volume){
    if (path_backgroundmusic == "") {
        return;
    }

    cocos2d::experimental::AudioEngine::stop(index_bgmusic);
    index_bgmusic = cocos2d::experimental::AudioEngine::play2d(path_backgroundmusic);
    cocos2d::experimental::AudioEngine::setLoop(index_bgmusic, true);
    cocos2d::experimental::AudioEngine::setVolume(index_bgmusic, volume);
}

void HSAudioEngine::pauseBackgroundMusic(){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    }else{
        cocos2d::experimental::AudioEngine::pause(index_bgmusic);
//    }
}

void HSAudioEngine::resumeBackgroundMusic(){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
//    }
//    else
        cocos2d::experimental::AudioEngine::resume(index_bgmusic);
    
    
}

void HSAudioEngine::stopBackgroundMusic(){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    }
//    else
        cocos2d::experimental::AudioEngine::stop(index_bgmusic);
}

void HSAudioEngine::pauseAllAudio(){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
//    }
//    else
        cocos2d::experimental::AudioEngine::pauseAll();
}

void HSAudioEngine::resumAllAudio(){
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
//    }
//    else
        cocos2d::experimental::AudioEngine::resumeAll();
}

void HSAudioEngine::stopAllAudio(){
    cocos2d::experimental::AudioEngine::stopAll();
//    if (isIOS) {
//        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
//    }
    m_soundMap.clear();
}

unsigned int HSAudioEngine::getDuration(int audio_id) {
    
    auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id) ;
    return duration;
}


#pragma mark - advance methods
int HSAudioEngine::play(std::string audio, bool loop, float volume, std::function<void()> finishCallback) {
    int id = cocos2d::experimental::AudioEngine::play2d(audio, loop, volume);
    
    if(finishCallback){
        cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int id, std::string path){
            finishCallback();
        });
    }
    
    return id;
}

int HSAudioEngine::playSound(std::string path, bool loop, std::function<void()> finishCallback /*= nullptr*/, float volume /*= 1.0f*/)
{
    //path = std::string("sounds/gymwiththemonkey/") + path;
    if (loop) {
        for (auto m : m_soundMap) {
            if (m.first == path) {
                return m.second;
            }
        }
    }
    
    while (path.substr(0, 1) == "/")
    {
        path = path.substr(1);
    }

    if(m_soundMap.find(path) != m_soundMap.end()) return m_soundMap.at(path);
    
    auto audioId = HSAudioEngine::play(path, loop, volume);
    if (audioId == cocos2d::experimental::AudioEngine::INVALID_AUDIO_ID) {
        if (finishCallback)
            finishCallback();
        return -1;
    }
    auto result = m_soundMap.insert(std::pair<std::string, int>(path, audioId));
    if (!loop) {
        cocos2d::experimental::AudioEngine::setFinishCallback(audioId, [=](int, std::string path) {
            auto it = m_soundMap.find(path);
            if (it != m_soundMap.end())
                it = m_soundMap.erase(it);
            if (finishCallback)
                finishCallback();
        });
    }
    return audioId;
}

void HSAudioEngine::playChain(std::vector<std::string> paths, std::function<void()> finishCallback, float volume, int startIndex)
{
    if (paths.size() == 0 || paths.size() <= startIndex) {
        if (finishCallback)
            finishCallback();
        return;
    }
    
    playSound(paths[startIndex], false, [=]() {
        playChain(paths, finishCallback, volume, startIndex + 1);
    });
}

void HSAudioEngine::stopSound(std::string path)
{
    auto it = m_soundMap.find(path);
    if (it == m_soundMap.end()) {
        // no sound
        return;
    }
    cocos2d::experimental::AudioEngine::stop(it->second);
    m_soundMap.erase(it);
}

void HSAudioEngine::stopAllSound()
{
	auto it = m_soundMap.begin();
	while (it != m_soundMap.end())
	{
		cocos2d::experimental::AudioEngine::stop(it->second);
		it = m_soundMap.erase(it);
	}
}


void HSAudioEngine::playClickedEffect(std::function<void()> callback)
{
    int id = AUDIO_ENGINE->playEffect("sounds/clicked_effect.mp3", false);

    cocos2d::experimental::AudioEngine::setFinishCallback(id, [callback](int, std::string) {

        if (callback) {
            callback();
        }
    });

}






