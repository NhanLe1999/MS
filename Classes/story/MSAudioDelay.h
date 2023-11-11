//
//  MSAudioDelay.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 10/11/18.
//

#ifndef MSAudioDelay_hpp
#define MSAudioDelay_hpp

#include <stdio.h>
#include "cocos2d.h"
namespace mj {
    namespace helper {
        class AudioDelay {
        public:
            static void preload(int story_id, int lang_id, std::function<void()> callback);
            static void preload(std::vector<std::string> files, std::function<void()> callback);
            static int play(const std::string &path, const std::vector<std::pair<float, float>> & delay = {});
            AudioDelay(const std::string &path, const std::vector<std::pair<float, float>> & delay  = {});
            ~AudioDelay();
            
        private:
            void play();
            int _job_id = -1;
            int _audio_id = -1;
            std::string _path;
            std::vector<std::pair<float, float>> _delays;
        };
    }
}

#endif /* MSAudioDelay_hpp */
