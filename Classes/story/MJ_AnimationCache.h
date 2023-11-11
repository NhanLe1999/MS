//
//  MJ_AnimationCache.hpp
//  M
//
//  Created by To Ngoc Linh on 5/10/17.
//
//

#ifndef MJ_AnimationCache_hpp
#define MJ_AnimationCache_hpp
#include "cocos2d.h"
#include <stdio.h>
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"

namespace mj {

    class AnimationCache {
    public:
        static AnimationCache & getInstance() {
            static AnimationCache cache;
            return cache;
        }

        static gaf::GAFAsset * create(std::string animation_name, std::function<void()> callback = nullptr);
        static void startSession();
        static void endSession();

    private:
        AnimationCache();
        AnimationCache(AnimationCache const &);
        void operator = (AnimationCache const &);
        
        std::unordered_map<std::string, gaf::GAFAsset *> _cache;

        void removeAnimation(std::string name);

        std::vector<std::string> _animsInSession;
        bool _isInSession;
        int _sessionID;
    };
}

#endif /* MJ_AnimationCache_hpp */

